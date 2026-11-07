#include <omnetpp.h>
#include "RdtPacket_m.h"

using namespace omnetpp;

class RdtClient : public cSimpleModule
{
private:
    // parameters
    int maxMsgSize;
    simtime_t requestTime;
    simtime_t requestTimeout;  // timeout for retransmission

    // state variables
    int expectedSeqNum;  // Next expected sequence number
    bool transferActive;
    bool requestSent;
    bool requestAcked;   // first data packet received

    // Timer
    cMessage *requestTimer;  // Timer for request retransmission

    // Transmission queue
    cPacketQueue transmitQueue;
    cMessage *sendNextMsg;  // Self-message for sending next queued packet

    // Signals for statistics
    simsignal_t dataPkRcvdSignal;
    simsignal_t ackPkSentSignal;
    simsignal_t corruptedPkDropSignal;
    simsignal_t dataDelaySignal;

    // Statistics tracking
    double totalDelay;
    int packetsReceived;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    void sendRequest();
    void sendAck(int ackNum);
    void processDataPacket(RdtPacket *pkt);
    void enqueuePacket(cPacket *pkt);
    void sendFromQueue();
};

Define_Module(RdtClient);

void RdtClient::initialize()
{
    // Read parameters
    maxMsgSize = par("maxMsgSize");
    requestTime = par("requestTime");
    requestTimeout = 0.5;  // 500ms timeout for request

    // Initialize state
    expectedSeqNum = 0;
    transferActive = false;
    requestSent = false;
    requestAcked = false;
    totalDelay = 0;
    packetsReceived = 0;

    // Create self-messages
    sendNextMsg = new cMessage("sendNext");
    requestTimer = new cMessage("requestTimeout");

    // Initialize queue
    transmitQueue.setName("transmitQueue");

    // Register signals
    dataPkRcvdSignal = registerSignal("dataPkRcvd");
    ackPkSentSignal = registerSignal("ackPkSent");
    corruptedPkDropSignal = registerSignal("corruptedPkDrop");
    dataDelaySignal = registerSignal("dataDelay");

    // Schedule request
    cMessage *requestMsg = new cMessage("sendRequest");
    scheduleAt(requestTime, requestMsg);
}

void RdtClient::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getName(), "sendRequest") == 0) {
            // Time to send request
            sendRequest();
            delete msg;
        }
        else if (msg == sendNextMsg) {
            // Time to send next packet from queue
            sendFromQueue();
        }
        else if (msg == requestTimer) {
            // request timeout - retransmit
            EV << "Client: REQUEST timeout, retransmitting\n";
            sendRequest();
        }
        else {
            delete msg;
        }
    }
    else {
        // Received packet from server
        RdtPacket *pkt = check_and_cast<RdtPacket*>(msg);

        // Check for bit errors
        if (pkt->hasBitError()) {
            EV << "Client: Received corrupted packet, dropping\n";
            emit(corruptedPkDropSignal, 1L);
            delete pkt;
            return;
        }

        if (pkt->isFin()) {
            // Transfer complete
            EV << "Client: Received FIN, transfer complete\n";
            transferActive = false;
            // Cancel REQUEST timer if still active
            if (requestTimer->isScheduled()) {
                cancelEvent(requestTimer);
            }
            // Send ACK for FIN with the FIN's sequence number
            sendAck(pkt->getSeqNum());
            delete pkt;
        }
        else if (pkt->isData()) {
            // Receiving data means REQUEST was successful
            if (!requestAcked) {
                EV << "Client: First data packet received, REQUEST acknowledged\n";
                requestAcked = true;
                if (requestTimer->isScheduled()) {
                    cancelEvent(requestTimer);
                }
            }
            processDataPacket(pkt);
            delete pkt;
        }
        else {
            EV << "Client: Received unknown packet type\n";
            delete pkt;
        }
    }
}

void RdtClient::sendRequest()
{
    EV << "Client: Sending file request at time " << simTime() << "\n";

    RdtPacket *request = new RdtPacket("REQUEST");
    request->setIsRequest(true);
    request->setByteLength(64);  // Small header

    transferActive = true;
    expectedSeqNum = 0;
    requestSent = true;

    enqueuePacket(request);

    // Start REQUEST timeout timer
    if (requestTimer->isScheduled()) {
        cancelEvent(requestTimer);
    }
    scheduleAt(simTime() + requestTimeout, requestTimer);
}

void RdtClient::sendAck(int ackNum)
{
    EV << "Client: Preparing ACK for seqNum " << ackNum << "\n";

    RdtPacket *ack = new RdtPacket("ACK");
    ack->setIsAck(true);
    ack->setAckNum(ackNum);
    ack->setByteLength(64);  // Small ACK packet

    emit(ackPkSentSignal, 1L);
    enqueuePacket(ack);
}

void RdtClient::enqueuePacket(cPacket *pkt)
{
    // Add packet to queue
    transmitQueue.insert(pkt);

    // If no send is scheduled, schedule one
    if (!sendNextMsg->isScheduled()) {
        sendFromQueue();
    }
}

void RdtClient::sendFromQueue()
{
    if (transmitQueue.isEmpty()) {
        return;
    }

    cGate *outGate = gate("out");

    // Check if channel is busy
    simtime_t finishTime = outGate->getTransmissionChannel()->getTransmissionFinishTime();

    if (finishTime <= simTime()) {
        // Channel is free, send packet
        cPacket *pkt = (cPacket*)transmitQueue.pop();
        EV << "Client: Sending packet from queue at t=" << simTime() << "\n";
        send(pkt, outGate);
        if (!transmitQueue.isEmpty()) {
            // Calculate when this transmission will finish
            simtime_t txFinishTime = outGate->getTransmissionChannel()->getTransmissionFinishTime();
            // Schedule next send slightly after transmission finishes
            scheduleAt(txFinishTime + SimTime(1, SIMTIME_NS), sendNextMsg);
        }
    } else {
        EV << "Client: Channel busy until " << finishTime << ", scheduling send\n";
        scheduleAt(finishTime + SimTime(1, SIMTIME_NS), sendNextMsg);
    }
}

void RdtClient::processDataPacket(RdtPacket *pkt)
{
    int seqNum = pkt->getSeqNum();

    EV << "Client: Received data packet with seqNum " << seqNum
       << ", expecting " << expectedSeqNum << "\n";

    if (seqNum == expectedSeqNum) {
        // In-order packet
        emit(dataPkRcvdSignal, 1L);

        // Calculate one-way delay (creation time to arrival)
        simtime_t delay = simTime() - pkt->getCreationTime();
        emit(dataDelaySignal, delay);

        // Update statistics
        totalDelay += delay.dbl();
        packetsReceived++;

        EV << "Client: Accepting packet " << seqNum << ", delay=" << delay << "\n";

        expectedSeqNum++;

        // Send cumulative ACK
        sendAck(seqNum);
    }
    else if (seqNum < expectedSeqNum) {
        // Duplicate packet
        EV << "Client: Duplicate packet " << seqNum << ", discarding but ACKing\n";
        // Still send ACK to help sender move forward
        sendAck(seqNum);
    }
    else {
        // Out-of-order packet (seqNum > expectedSeqNum)
        EV << "Client: Out-of-order packet " << seqNum << ", discarding\n";
        // Send ACK for last in-order packet
        if (expectedSeqNum > 0) {
            sendAck(expectedSeqNum - 1);
        }
    }
}

void RdtClient::finish()
{
    // Clear transmit queue
    while (!transmitQueue.isEmpty()) {
        delete transmitQueue.pop();
    }

    cancelAndDelete(sendNextMsg);
    cancelAndDelete(requestTimer);
}
