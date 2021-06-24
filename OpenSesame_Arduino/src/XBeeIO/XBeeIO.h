/*
 * XBee.h - Libary for XBee Communication
 * Created by Justin D. Merrell, April 9th, 2020.
 */

#ifndef XBeeIO_h
#define XBeeIO_h

#include <SoftwareSerial.h>
#include <Arduino.h>

class XBeeIO
{
private:
    SoftwareSerial *XBeeSerial;
    unsigned long currentMillis = 0;  //Current mills reading from arduino.
    unsigned long previousMillis = 0; //Will store the last time processing indication was update.

    const int PROGMEM interval = 5000; //Interval at which to timeout. (Might want to change this with calculated delay time from XBee.)

    //byte frameDOOR[22] PROGMEM = {0x7E, 0x00, 0x12, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x00, 0x64, 0x6F, 0x6F, 0x72, 0x3D};

public:
    XBeeIO(SoftwareSerial *XBee);
    byte Incoming[20];
    void TX(long Data);
    unsigned long RX();
    int JoinNetwork();
    unsigned long AddressingCommand(char *AT_Command, char FrameType = 0x08, long Parameter = -1);
    void FlushReceive();
};

#endif
