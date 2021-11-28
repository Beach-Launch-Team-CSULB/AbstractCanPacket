/*
    Author Jacob Waters
    fierywaters13@gmail.com
    Designed for CSULB Beach Launch Team
*/
#include <Arduino.h>
#include <Streaming.h>
#include "AbstractedCanPacket.h"
#include "CanBitBuffer.h"

//testing
#include "FlexCAN.h"
int busSpeed = 500000; //baudrate
bool canBus = 0;	   //use 0 CAN0 or 1 for CAN1
FlexCAN CANbus0(busSpeed, canBus);

void setup()
{
	CANbus0.begin();
	while (!Serial)
	{
		// wait for serial port to connect.
	}
}
//AbstractedCanPacket abstractCanPacket;
uint8_t idLength = 5; //5 bit ID field

CAN_message_t msg;

void loop()
{
	if (CANbus0.available())
	{

		CANbus0.read(msg);

		//////////////////////////////////////////Reading from CAN
		//Here, you'd send the CAN Frame out the CAN Bus (This is a cheap simulation(I'm lazy))

		//ON OTHER NODE
		//Read our CAN Frame off the CAN Bus (still a sumulation)
		//wow, we are now on the other node,
		//and shallowCopy is there and what we read from the bus, look at that. Wasn't that nifty?
		//Serial << "We have teleported across space and time to another micro (wow, magic)" << endl;

		Serial << "Building AbstractedCanPacket from CAN_message_t!" << endl;
		AbstractedCanPacket incomingCanPacket(idLength, msg);

		Serial << "nodeID: " << incomingCanPacket.getNodeID();
		Serial << ", Priority: " << incomingCanPacket.getPriority() << endl;

		Serial << "\nReading incomingCanPacket MiniPackets:" << endl;
		MiniPacket *incomingArray = incomingCanPacket.getPacketBuffer();
		for (int i = 0; i < incomingCanPacket.getBufferSize(); i++)
		{
			Serial << "packetArray[" << i << "]:";

			int id, data;
			id = incomingArray[i].getID();
			data = incomingArray[i].getData();
			Serial << "ID: " << id << ", Data: " << data;

			Serial << endl;
		}
		Serial << endl;

		/*
	Rather than make a new AbstractCanPacket each time, simply reset them
	so they can be reused without additional memory use. UNTESTED
	*/
		//incomingCanPacket.reset();
		//abstractCanPacket.reset();
	}

	//while (true)
	//	delay(1000); //disable loop
}
