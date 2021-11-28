/*
    Author Jacob Waters
    fierywaters13@gmail.com
    Designed for CSULB Beach Launch Team
*/
#include <Arduino.h>
#include <Streaming.h>
#include "AbstractedCanPacket.h"
#include "CanBitBuffer.h"
#include "FlexCAN.h"

int busSpeed = 500000; //baudrate
bool canBus = 0;	   //use 0 CAN0 or 1 for CAN1
FlexCAN CANbus0(busSpeed, canBus);

AbstractedCanPacket abstractCanPacket;

uint8_t idLength = 5;	 //5 bit ID field
uint8_t dataLength = 12; //12 bit ADC read
MiniPacket filler(idLength, dataLength);
void setup()
{
	CANbus0.begin();

	filler.setID(1);   //remember zero ID is forbidden
	filler.setData(0); //zero data is fine

	while (!Serial)
		;
}
int counter = 0;

uint8_t maxNodeID = ((1 << nodeIDLength) - 1);	   //2^ID_length -1
uint8_t maxPriority = ((1 << priorityLength) - 1); //2^ID_length -1
void loop()
{

	abstractCanPacket.init();
	//abstractCanPacket.printCanMessage();

	//KNOWN ISSUE: setting nodeID or priority to 0 breaks it.
	abstractCanPacket.setNodeID(counter % maxNodeID +1);
	abstractCanPacket.setPriority(counter % maxPriority +1); //010

	//MiniPacket sensorData;

	while (abstractCanPacket.add(filler)) //add duplicates until full. Note add returns true if successful, false otherwise.
	{

		//if setting data fails (overflow) reset to lowest value
		if (!filler.setID(filler.getID() + 1))
			filler.setID(1);

		if (!filler.setData(filler.getData() + 1))
			filler.setData(0);

		Serial << "id: " << filler.getID() << ", data: " << filler.getData() << "\n";
	}

	MiniPacket *packetArray = abstractCanPacket.getPacketBuffer();
	for (int i = 0; i < abstractCanPacket.getBufferSize(); i++)
	{
		//Serial << "packetArray[" << i << "]: ";
		//packetArray[i].print();
		//Serial << endl;
	}
	Serial << "write success: " << CANbus0.write(abstractCanPacket.getCanMessage()) << endl;
	abstractCanPacket.printCanMessage();
	Serial << "/n";

	delay(1000);
	counter++;
	abstractCanPacket.reset();
}
