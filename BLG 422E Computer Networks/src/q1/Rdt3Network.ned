//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

package hw1q1.rdt;
import ned.DatarateChannel;
simple RdtClient
{
	parameters:
		int maxMsgSize @unit(B) = default(1000B); // also used as max segment size
		double requestTime @unit(s) = default(0s);
		@signal[dataPkRcvd](type=long);
		@signal[ackPkSent](type=long);
		@signal[corruptedPkDrop](type=long);
		@signal[dataDelay](type=simtime_t);
		@statistic[dataPkRcvd](title="Data packets received"; source=dataPkRcvd; record=count;);
		@statistic[ackPkSent](title="ACK packets sent"; source=ackPkSent; record=count;);
		@statistic[corruptedPkDrop](title="Corrupted packets dropped"; source=corruptedPkDrop; record=count;);
		@statistic[dataDelay](title="One-way data delay"; source=dataDelay; record=mean, histogram;);
	gates:
		input in;
		output out;
}
simple RdtServer
{
	parameters:
		int maxMsgSize @unit(B) = default(1000B); // also used as segment size
		int windowSize = default(100); // N: max in-flight segments
		int fileSizeBytes @unit(B) = default(100000B);
		double timeout @unit(s) = default(0.2s);
		@signal[dataPkSent](type=long);
		@signal[dataPkRetx](type=long);
		@signal[ackPkRcvd](type=long);
		@signal[reqPkRcvd](type=long);
		@signal[corruptedPkDrop](type=long);
		@statistic[dataPkSent](title="Data packets sent"; source=dataPkSent; record=count;);
		@statistic[dataPkRetx](title="Data packets retransmitted"; source=dataPkRetx; record=count;);
		@statistic[ackPkRcvd](title="ACK packets received"; source=ackPkRcvd; record=count;);
		@statistic[reqPkRcvd](title="Request packets received"; source=reqPkRcvd; record=count;);
		@statistic[corruptedPkDrop](title="Corrupted packets dropped"; source=corruptedPkDrop; record=count;);
	gates:
		input in;
		output out;
}
channel LossyLink extends ned.DatarateChannel
{
	parameters:
		datarate = default(10Mbps);
		delay = default(10ms);
		per = default(0.1); // packet error rate 0..1
		ber = default(0); // bit error rate 0..1
}
network Rdt3Network
{
	submodules:
		client: RdtClient { @display("p=100,200"); }
		server: RdtServer { @display("p=300,200"); }
	connections:
		client.out --> c2s: LossyLink --> server.in;
		server.out --> s2c: LossyLink --> client.in;
}