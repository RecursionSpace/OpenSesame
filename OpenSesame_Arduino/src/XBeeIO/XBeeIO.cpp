#include <Arduino.h>
#include "XBeeIO.h"

XBeeIO::XBeeIO(SoftwareSerial *XBee)
{
    XBeeSerial = XBee;
}

void XBeeIO::TX(long Data)
{
    int adding = 0;
    Serial.print(F("TX ID: "));
    Serial.println(Data);
    String serarray = String(Data);
    int strleng = ((serarray.length() + 14));
    XBeeSerial->write((byte)0x7E);
    XBeeSerial->write((byte)0x00);
    XBeeSerial->write(strleng);
    XBeeSerial->write((byte)0x10); //This is for transmit request when the XBee's are set to explicit mode.
    XBeeSerial->write((byte)0x00);
    for (int i = 0; i < 8; i++)
    {
        XBeeSerial->write((byte)0x00);
    }
    XBeeSerial->write((byte)0xFF);
    XBeeSerial->write((byte)0xFE);
    XBeeSerial->write((byte)0x00);
    XBeeSerial->write((byte)0x00);
    for (int i = 0; i < serarray.length(); i++)
    {
        XBeeSerial->write(serarray[i]);
        adding = adding + serarray[i];
    }
    adding = (byte)0x10 + (byte)0x00 + (byte)0x00 + (byte)0xFF + (byte)0xFE + adding;
    int mask = 255 - (adding & 255);
    XBeeSerial->write(mask);
    // Serial.print(F("Ttransmitted ID")); Serial.println();
    return;
}

//Processes incoming Response from XBee and returns data from "Response" field of frame.
unsigned long XBeeIO::RX()
{
    currentMillis = millis();
    previousMillis = currentMillis;
    bool DataAvailable = false; //Used to escape the while loop once all information has been read.

    int x = 0;         //Tracks what byte number is currently being read.
    byte Checksum = 0; //Stores the checksum to know when the end of the frame is reached.
    byte NextByte = 0; //Store the incoming byte to compare with the checksum for a match.
    int y = 0;         //Variable to track where to store data in the incomign variable.

    while (((currentMillis - previousMillis) <= interval) && DataAvailable == false)
    {
        //Serial.println(F("")); Serial.print(F("XBee Response: ")); //Can be commented out during production.

        while (XBeeSerial->available()) //Only proceed if data is available to read from the XBee.
        {
            delay(5);  //Gives it a moment to fill the queue
            if (x > 7) //Do not store the first 6 bits from the frame.
            {
                NextByte = XBeeSerial->read();
                if ((255 - Checksum) != NextByte)
                {
                    Incoming[y] = NextByte;
                    //Serial.print(Incoming[x], HEX); //Can be commented out during production.
                    Checksum = Checksum + Incoming[y];
                    y++;
                }
            }
            else if (x > 2 && x <= 7) //Start creating the checksum with bytes between length and checksum.
            {
                Checksum = Checksum + (byte)XBeeSerial->read();
            }
            else
            {
                XBeeSerial->read();
            }
            x++;
            DataAvailable = true;
        }
        currentMillis = millis();
    }

    if (DataAvailable == true) //Only send out Response if a response was received.
    {
        unsigned long Sum = 0; //Converting the array to a single long to send.
        for (int i = 0; i < y; i++)
        {
            Sum = Sum << 8;
            Sum = Incoming[i] | Sum;
        }
        //Serial.println(F("")); Serial.print(F("Sum: ")); Serial.println(Sum); //Can be commented out during production.
        return Sum;
    }
    else
    {
        Serial.println(F("Timed Out - No XBee Response "));
        return false;
    }
}

/*
/ Funtion first checks that the XBee is connected to a network before proceeding.
/ Once a network connection is confirmed the XBee will broadcast the type of node it is.
/ The hub will respond back with a confirmation and wether or not this is the first time connecting.
*/
int XBeeIO::JoinNetwork() //Kinda works, needs more debugging.
{
    AddressingCommand("FR"); //Restarts the XBee incase the coordinator left since last restart.
    delay(3000);             //There is a three second delay after the FR command is sent, an additional second buffer is added.

    Serial.println(F("Attempting To Join XBee Network"));

    word OperatingChannel = 00;  //Channel 0x00 means that the XBee has not yet selected a channel.
    word NetworkAddress = 65534; //Network Address 0xFFFE means that the XBee has not been assigned an address yet.
    while (OperatingChannel == 00)
    {
        OperatingChannel = AddressingCommand("CH");
        if (OperatingChannel == 00)
        {
            Serial.println(F("Not Connected To Network"));
        }
    }
    //This is being ignored.
    word ClearBuffer = AddressingCommand("MY");
    while (NetworkAddress == 65534)
    {
        NetworkAddress = AddressingCommand("MY");
        if (NetworkAddress == 65534)
        {
            Serial.println(F("Not Connected To Network (Address)"));
        }
    }

    Serial.println(F("Sucess - Joined XBee Network"));
    Serial.println();

    /*
  	int x = 0;
  	for (int i=0; i<22; i++)
  	{
    	XBeeSerial->write(frameDOOR[i]);
  	}
	RX();
	*/

    //int ResponseFilter = x-3;
    //if ((StartResponse[ResponseFilter]) == 0){ return false; }
    return;
}

/*
* Function to execute API commands. 1 Required parameter, 2 optional parameters.
* char* AT_Command -> Two letter command formatted within double quotations.
* byte FrameType ->  Optional with default 0x08. Has only been tested with 0x09.
* long Parameter -> Optional with default -1 which is used for API without a parameter. Provides the API request with variables.
*/
unsigned long XBeeIO::AddressingCommand(char *AT_Command, char FrameType, long Parameter)
{
    byte byte3, byte2, byte1, byte0; //Variables used to convert the parameter into individual bytes.

    byte3 = (Parameter >> 0) & 0xFF;  //Serial.println(byte3, HEX);
    byte2 = (Parameter >> 8) & 0xFF;  //Serial.println(byte2, HEX);
    byte1 = (Parameter >> 16) & 0xFF; //Serial.println(byte1, HEX);
    byte0 = (Parameter >> 32) & 0xFF; //Serial.println(byte0, HEX);

    int ByteCount; //Variable used as part of the framelength calculation.

    if (Parameter == -1)
        ByteCount = 0;
    if (Parameter >= 0 && Parameter <= 255)
        ByteCount = 1;
    if (Parameter >= 256 && Parameter <= 65535)
        ByteCount = 2;
    if (Parameter >= 65536 && Parameter <= 16777215)
        ByteCount = 3;
    if (Parameter >= 16777216 && Parameter <= 4294967295)
        ByteCount = 4;

    //Serial.print(F("Parameter:")); Serial.println(Parameter);
    //Serial.print(F("Size of Parameter: ")); Serial.println(ByteCount);

    int FrameLength = (2 + sizeof(AT_Command) + ByteCount); //Number of bytes between length and Checksum.

    int CommandByteSum = 0; //Sum of bytes from command variable.

    for (int i = 0; i < sizeof(AT_Command); i++)
    {
        CommandByteSum = CommandByteSum + AT_Command[i];
    }

    XBeeSerial->write((byte)0x7E); //Serial.print(F("Start delimiter: ")); Serial.println((byte)0x7E, HEX);

    if (FrameLength < 255)
    {
        XBeeSerial->write((byte)0x00); //Serial.print(F("MSB 1: ")); Serial.println((byte)0x00);
    }

    XBeeSerial->write(FrameLength); //Serial.print(F("Length: ")); Serial.println(FrameLength, HEX);

    XBeeSerial->write((byte)FrameType); //Serial.print(F("Frame Type: ")); Serial.println((byte)FrameType, HEX);

    XBeeSerial->write((byte)0x01); //Serial.print(F("Frame ID: ")); Serial.println((byte)0x01, HEX);

    XBeeSerial->write(AT_Command); //Serial.print(F("AT Command: ")); Serial.println(AT_Command);

    if (Parameter != -1)
    {
        if (ByteCount >= 4)
            XBeeSerial->write(byte0);
        if (ByteCount >= 3)
            XBeeSerial->write(byte1);
        if (ByteCount >= 2)
            XBeeSerial->write(byte2);
        if (ByteCount >= 1)
            XBeeSerial->write(byte3);
        //Serial.print(F("Parameter: "));Serial.println((byte)Parameter);

        XBeeSerial->write((byte)(0xFF - ((byte)FrameType + (byte)0x01 + (byte)CommandByteSum + byte0 + byte1 + byte2 + byte3))); //Serial.print(F("Checksum: "));Serial.println((byte)(0xFF - ((byte)FrameType + (byte)0x01 + (byte)Adding + byte0 + byte1 + byte2 + byte3)), HEX);
    }
    else
    {
        XBeeSerial->write(255 - ((byte)FrameType + (byte)0x01 + (byte)CommandByteSum)); //Serial.print(F("Checksum: ")); Serial.println(255 - ((byte)FrameType + (byte)0x01 + (byte)Adding));
    }

    unsigned long Response = RX();
    //Serial.print(F("Response HEX: ")); Serial.println(Response,HEX);
    //Serial.print(F("Response DEC: ")); Serial.println(Response);
    return Response;
}

void XBeeIO::FlushReceive()
{
    while (XBeeSerial->available())
    {
        XBeeSerial->read();
    }
    return;
}
