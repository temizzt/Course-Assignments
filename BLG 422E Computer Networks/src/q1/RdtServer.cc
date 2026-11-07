#include <omnetpp.h>
#include <queue>
#include "RdtPacket_m.h"

using namespace omnetpp;

class RdtServer : public cSimpleModule
{
private:
    // Parameters
    int maxMsgSize;
    int windowSize;
    int fileSizeBytes;
    simtime_t timeout;

    // State variables
    int totalSegments;           // Total number of segments to send
    int nextSeqNum;              // Next sequence number to send
    int base;                    // Oldest unacknowledged segment
    bool transferActive;
    bool finSent;
    bool finAcked;
    // Timer
    cMessage *timeoutTimer;
    cMessage *sendNextMsg;       // Self-message for sending next queued packet

    // Transmission queue
    cPacketQueue transmitQueue;

    // Signals for statistics
    simsignal_t dataPkSentSignal;
    simsignal_t dataPkRetxSignal;
    simsignal_t ackPkRcvdSignal;
    simsignal_t reqPkRcvdSignal;
    simsignal_t corruptedPkDropSignal;

    // Statistics tracking
    simtime_t transferStartTime;
    simtime_t transferEndTime;
    int totalRetransmissions;

    // Packet buffer for retransmissions
    std::map<int, RdtPacket*> sentPackets;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    void startTransfer();
    void sendDataPacket(int seqNum, bool isRetransmission);
    void sendWindow();
    void handleAck(RdtPacket *ack);
    void handleTimeout();
    void sendFin();
    void enqueuePacket(cPacket *pkt);
    void sendFromQueue();
};

Define_Module(RdtServer);

void RdtServer::initialize()
{
    // Read parameters
    maxMsgSize = par("maxMsgSize");
    windowSize = par("windowSize");
    fileSizeBytes = par("fileSizeBytes");
    timeout = par("timeout");

    // Calculate total segments
    totalSegments = (fileSizeBytes + maxMsgSize - 1) / maxMsgSize;

    // Initialize state
    nextSeqNum = 0;
    base = 0;
    transferActive = false;
    finSent = false;
    finAcked = false;
    totalRetransmissions = 0;
    transferStartTime = 0;
    transferEndTime = 0;

    // Create timers
    timeoutTimer = new cMessage("timeout");
    sendNextMsg = new cMessage("sendNext");

    // Initialize queue
    transmitQueue.setName("transmitQueue");

    // Register signals
    dataPkSentSignal = registerSignal("dataPkSent");
    dataPkRetxSignal = registerSignal("dataPkRetx");
    ackPkRcvdSignal = registerSignal("ackPkRcvd");
    reqPkRcvdSignal = registerSignal("reqPkRcvd");
    corruptedPkDropSignal = registerSignal("corruptedPkDrop");

    EV << "Server initialized: " << totalSegments << " segments, window="
       << windowSize << "\n";
}

void RdtServer::handleMessage(cMessage *msg)
{
    if (msg == timeoutTimer) {
        handleTimeout();
    }
    else if (msg == sendNextMsg) {
        // Time to send next packet from queue
        sendFromQueue();
    }
    else {
        RdtPacket *pkt = check_and_cast<RdtPacket*>(msg);

        // Check for bit errors
        if (pkt->hasBitError()) {
            EV << "Server: Received corrupted packet, dropping\n";
            emit(corruptedPkDropSignal, 1L);
            delete pkt;
            return;
        }

        if (pkt->isRequest()) {
            EV << "Server: Received file request\n";
            emit(reqPkRcvdSignal, 1L);
            startTransfer();
            delete pkt;
        }
        else if (pkt->isAck()) {
            handleAck(pkt);
            delete pkt;
        }
        else {
            EV << "Server: Received unknown packet type\n";
            delete pkt;
        }
    }
}

void RdtServer::startTransfer()
{
    EV << "Server: Starting file transfer\n";
    transferActive = true;
    nextSeqNum = 0;
    base = 0;
    finSent = false;
    finAcked = false;
    totalRetransmissions = 0;
    transferStartTime = simTime();

    // Clear any old buffered packets
    for (auto &pair : sentPackets) {
        delete pair.second;
    }
    sentPackets.clear();
    // Clear transmit queue
    while (!transmitQueue.isEmpty()) {
        delete transmitQueue.pop();
    }
    // Send initial window
    sendWindow();
}

void RdtServer::sendWindow()
{
    // Send packets up to window limit
    while (nextSeqNum < base + windowSize && nextSeqNum < totalSegments) {
        sendDataPacket(nextSeqNum, false);
        nextSeqNum++;
    }

    // Start timer if there are unacknowledged packets
    if (base < nextSeqNum) {
        if (timeoutTimer->isScheduled()) {
            cancelEvent(timeoutTimer);
        }
        scheduleAt(simTime() + timeout, timeoutTimer);
    }
}

void RdtServer::sendDataPacket(int seqNum, bool isRetransmission)
{
    EV << "Server: Preparing " << (isRetransmission ? "RETX" : "DATA")
       << " packet seqNum=" << seqNum << "\n";

    // Calculate payload size for this segment
    int remainingBytes = fileSizeBytes - (seqNum * maxMsgSize);
    int payloadBytes = (remainingBytes < maxMsgSize) ? remainingBytes : maxMsgSize;

    RdtPacket *pkt = new RdtPacket("DATA");
    pkt->setIsData(true);
    pkt->setSeqNum(seqNum);
    pkt->setPayloadBytes(payloadBytes);
    pkt->setByteLength(64 + payloadBytes);  // Header + payload

    // Store copy for potential retransmission
    if (!isRetransmission) {
        sentPackets[seqNum] = pkt->dup();
        emit(dataPkSentSignal, 1L);
    } else {
        emit(dataPkRetxSignal, 1L);
        totalRetransmissions++;
    }

    // Enqueue packet instead of sending directly
    enqueuePacket(pkt);
}

void RdtServer::enqueuePacket(cPacket *pkt)
{
    // Add packet to queue
    transmitQueue.insert(pkt);

    // If no send is scheduled, schedule one
    if (!sendNextMsg->isScheduled()) {
        sendFromQueue();
    }
}

void RdtServer::sendFromQueue()
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
        EV << "Server: Sending packet from queue at t=" << simTime() << "\n";
        send(pkt, outGate);

        // If more packets in queue, schedule next send
        if (!transmitQueue.isEmpty()) {
            // Calculate when this transmission will finish
            simtime_t txFinishTime = outGate->getTransmissionChannel()->getTransmissionFinishTime();
            // Schedule next send slightly after transmission finishes
            scheduleAt(txFinishTime + SimTime(1, SIMTIME_NS), sendNextMsg);
        }
    } else {
        // channel is busy, schedule send
        EV << "Server: Channel busy until " << finishTime << ", scheduling send\n";
        scheduleAt(finishTime + SimTime(1, SIMTIME_NS), sendNextMsg);
    }
}

void RdtServer::handleAck(RdtPacket *ack)
{
    int ackNum = ack->getAckNum();

    EV << "Server: Received ACK for seqNum " << ackNum
       << " (base=" << base << ", nextSeqNum=" << nextSeqNum << ")\n";

    emit(ackPkRcvdSignal, 1L);

    if (finSent && ackNum == totalSegments) {
        EV << "Server: Received ACK for FIN, transfer complete\n";
        finAcked = true;
        transferActive = false;

        // Record completion time
        transferEndTime = simTime();
        simtime_t completionTime = transferEndTime - transferStartTime;

        // Calculate throughput (bits per second)
        double throughput = (fileSizeBytes * 8.0) / completionTime.dbl();

        EV << "=== Transfer Statistics ===\n";
        EV << "Completion Time: " << completionTime << " seconds\n";
        EV << "Throughput: " << throughput << " bps (" << throughput/1000000 << " Mbps)\n";
        EV << "Total Retransmissions: " << totalRetransmissions << "\n";
        EV << "File Size: " << fileSizeBytes << " bytes\n";

        // Record scalar statistics
        recordScalar("completionTime", completionTime);
        recordScalar("throughput", throughput);
        recordScalar("totalRetransmissions", totalRetransmissions);

        if (timeoutTimer->isScheduled()) {
            cancelEvent(timeoutTimer);
        }
        return;
    }

    // Cumulative ACK: ackNum acknowledges all packets up to and including ackNum
    if (ackNum >= base) {
        // Move window forward
        for (int i = base; i <= ackNum && i < totalSegments; i++) {
            // Remove acknowledged packet from buffer
            auto it = sentPackets.find(i);
            if (it != sentPackets.end()) {
                delete it->second;
                sentPackets.erase(it);
            }
        }

        base = ackNum + 1;

        // Cancel and restart timer
        if (timeoutTimer->isScheduled()) {
            cancelEvent(timeoutTimer);
        }

        // If all sent packets are acknowledged, no need for timer
        if (base == nextSeqNum) {
            // Timer is already cancelled above
        } else {
            // Restart timer for remaining packets
            scheduleAt(simTime() + timeout, timeoutTimer);
        }

        // Check if transfer is complete
        if (base >= totalSegments && !finSent) {
            EV << "Server: All data acknowledged, sending FIN\n";
            sendFin();
        } else if (base < totalSegments) {
            // Send more packets if window allows
            sendWindow();
        }
    }
    else {
        // Duplicate ACK or out-of-order, ignore
        EV << "Server: Ignoring ACK " << ackNum << " (already acknowledged)\n";
    }
}

void RdtServer::handleTimeout()
{
    EV << "Server: TIMEOUT! ";

    if (finSent && !finAcked) {
        EV << "Retransmitting FIN\n";
        sendFin();
        // Timer will be rescheduled in sendFin()
        return;
    }

    EV << "Retransmitting from base=" << base << "\n";

    if (windowSize == 1) {
        // Stop-and-Wait: retransmit the single unacknowledged packet
        if (base < totalSegments) {
            sendDataPacket(base, true);
        }
    } else {
        // Go-Back-N: retransmit all packets in the window
        for (int i = base; i < nextSeqNum && i < totalSegments; i++) {
            sendDataPacket(i, true);
        }
    }

    // Restart timer
    if (timeoutTimer->isScheduled()) {
        cancelEvent(timeoutTimer);
    }
    scheduleAt(simTime() + timeout, timeoutTimer);
}

void RdtServer::sendFin()
{
    EV << "Server: Sending FIN\n";

    RdtPacket *fin = new RdtPacket("FIN");
    fin->setIsFin(true);
    fin->setSeqNum(totalSegments);  // FIN has sequence number after last data
    fin->setByteLength(64);

    enqueuePacket(fin);

    finSent = true;

    // Start timer for FIN acknowledgment
    if (timeoutTimer->isScheduled()) {
        cancelEvent(timeoutTimer);
    }
    scheduleAt(simTime() + timeout, timeoutTimer);
}

void RdtServer::finish()
{
    // Clean up buffered packets
    for (auto &pair : sentPackets) {
        delete pair.second;
    }
    sentPackets.clear();

    // Clear transmit queue
    while (!transmitQueue.isEmpty()) {
        delete transmitQueue.pop();
    }

    cancelAndDelete(timeoutTimer);
    cancelAndDelete(sendNextMsg);
}
