/*
 * Recursion.Space
 * OpenSesame
 *
 * Compatable with Sesame Seed board version P-0.12.0+
 *
 * Input/Output Pins Configuration:
 * PIN  D0  PROG PIN RX     - Conneccted to prgramming headder pin.
 * PIN  D1  PROG PIN TX     - Connected to programming headder pin.
 * PIN  D2  Wiegand D0      - Connected to wigand device.
 * PIN  D3  Wiegand D1      - Connected to wigand device.
 * PIN  D4  Wiegand LED     - Connected wigand device.
 * PIN  D5  DoorA Trig      - Connected to "DoorA Signal" on the controller to unlock door with relay.
 * PIN  D6  Button Signal
 * PIN  D7  Motion Signal
 * PIN  D8  XBee Dout (tx)  - Connected to XBee module.
 * PIN  D9  XBee Din (rx)   - Connected to XBee module.
 * PIN  D10 SS              - MicroSD
 * PIN  D11 MOSI            - MicroSD
 * PIN  D12 MISO            - MicroSD
 * PIN  D13 SCK             - MiroSD
 * PIN  A0  SYS STAT
 *
 * Power and Additional Connections Needed For Pheriferals:
 * GND, Wiegand, Relay
 * 3.3v XBee
 */

/* --------------------------- Required Libraries --------------------------- */
#include <SoftwareSerial.h>          //Communication with XBee
#include <Wiegand.h>                 //RFID Reader
#include <SPI.h>                     //MicroSD Card Communication
#include <EEPROM.h>                  //Used to store data after power cycle
#include <SD.h>                      //Libary to access SD card
#include <ArduinoJson.h>             //Libary to parse and create Json formatted data
#include <avr/wdt.h>                 //Watchdog libary used to automaticly restart arduino and prevent hangups.
#include "src/XBeeIO/XBeeIO.h"       //Custom libary to manage XBee communication.
#include "src/LatchCtrl/LatchCtrl.h" //Custom libary to control the behavior of a relay and visual indicators.

/* ------------------------ Setup pins and variables ------------------------ */
WIEGAND wg; //Create an instance of wg
SoftwareSerial XBee(8, 9);
XBeeIO XBee_IO(&XBee);

#define WiegandD0 2  //Weigand D0 on PIN 2
#define WiegandD1 3  //Weigand D1 on PIN 3
#define DoorA 5      //Door A manual relay on PIN 4
#define WiegandLED 4 //Weigand LED on PIN 7
#define SDSS 10      //SD card Save Select Pin
#define SystemLED A0 //General Status LED
#define WDTO_8S 9    //Watch dog timer variable

/* --------------------- System configuration structure. -------------------- */
struct Config
{
    bool debug;           //Bypasses some features when testing.
    bool networked;       //Determins if the device is stand alone or communicates with a central hub.
    long xbeeID;          //XBee network PAN ID
    unsigned long xbeeKY; //XBee network key
    bool FactoryReset;    //If set to true it will reprogram XBee and perform configurations
};
Config config;

File file; //Variable to read and write from a file.

int LatchTime = 8000;                         //Variable used to define how long a door should be unlocked after authorization.
LatchCtrl Door(DoorA, WiegandLED, LatchTime); //creates and instance of LatchCtrl to manage a door relay and visual indicator. Custom libary.

void setup()
{
    Serial.begin(9600); //Setting serial connection baud rate.
    XBee.begin(9600);   //Setting XBee communication baud rate.
    wg.begin();         //Initialize Wiegand communication.

    pinMode(SystemLED, OUTPUT);   //Onboard system status indicator pin configuration.
    digitalWrite(SystemLED, LOW); //Start with system LED off

    Serial.println(F("--- OpenSesame ---"));
    Serial.println();

    POST();     //First function called on bootup to check configurations and connections have been properly made and an SD card is available.
    FirstUse(); //Only ran when system is first initiated, configures one time settings.

    if (config.networked) //XBee only used with networked mode is enabled.
    {
        HWcheck();             //Last initilization function, checks if the XBee MAC has been stored in EEPROM and matches. If it does not match it will update the stored MAC and set the PAN ID and Encryption Key.
        XBee_IO.JoinNetwork(); //Checks that XBee is connected to a network and then if it is the first time connecting or not.
        delay(5000);
        XBee_IO.FlushReceive();
    }
}

//Core program that runs continuously.
void loop()
{
    byte LastPosition = 00;  //Stores the last access code set so that the device is not constanly updating.
    byte access = 48;        //Varriable used to determin access result.
    bool Processing = false; //Used to indicate that an ID confirmation is being processed.
    bool Override = false;   //Used when default actiosn are not desired and a manual state has been set.

    XBee_IO.FlushReceive();

    wdt_enable(WDTO_8S); //Configure 8sec delay before the arduino auto resets.
    while (1)
    {
        if (wg.available() && Processing == false && Override == false)
        {
            Processing = true; //Prevents redundant scanning while processing.

            if (config.networked) //Only pulls remote data if networked is enabled.
            {
                XBee_IO.TX(wg.getCode()); //Calls function to transmit the code read.
                access = XBee_IO.RX();    //Sets access to the responce from XBee.
                Processing = false;       //Allows scanning to resume.
            }
            else
            {
                if (config.debug)
                    Serial.println(F("Checking SD for user"));
                unsigned long code = wg.getCode();       //Stores RFID code
                String stringOne PROGMEM = "Allowed/";   //First string section
                String stringTwo = stringOne + code;     //Merge first string section with the code number
                String stringThree = stringTwo + ".txt"; //Add final string section
                if (config.debug)
                    Serial.println(stringThree); //Checks if file exsists to determin if user is allowed, sends compiled string from above

                if (SD.exists(stringThree)) //If the users ID is availavle allow the user to pass
                {
                    access = 49;
                }
                else
                {
                    access = 50;
                }

                Processing = false;
            }
        }

        if (XBee.available()) //Once in main loop the unit passes all recived data as access data. !!!Might be causeing issue if the xbee recives a code it can not process.
        {
            access = XBee_IO.RX();
            LastPosition = 00;
            if (config.debug)
                Serial.print(F("Code Recived: "));
            Serial.println(access);
        }

        if (access == 48 && LastPosition != 48 && Override == false)
        {
            if (config.debug)
                Serial.println(F("Locked"));
            if (config.debug)
                Serial.println(); //Only prints in debug mode.
            Door.Lock();          //Call door lock function.
            LastPosition = 48;
        }
        else if (access == 49)
        {
            if (config.debug)
                Serial.println(F("Allow"));
            if (config.debug)
                Serial.println(); //Only prints in debug mode.
            //Door.Allow();         //Call door allow function
            Allow();
            access = 48;
            LastPosition = 49;
        }
        else if (access == 50 && LastPosition != 50)
        {
            if (config.debug)
                Serial.println(F("Deny"));
            if (config.debug)
                Serial.println();
            //Door.Deny();         //Call door deny function
            Deny();
            LastPosition = 50;
        }
        else if (access == 51 && LastPosition != 51)
        {
            if (config.debug)
                Serial.print(F("Hold"));
            if (config.debug)
                Serial.println();
            Door.Hold(); //Call door hold (latch) function
            LastPosition = 51;
            Override = true;
        }
        else if (access == 57 && LastPosition != 57)
        {
            access = 48;
            Override = false;
        }
        else
        {
            access = 48;
        }

        if (Processing == false)
        {
            wdt_reset();
        }

        //delay(100);     //Tryign to prevent more false positives.
    }
}

//Hardware change function checks if the XBee has been used before, otherwise configures network parapeters.
void HWcheck()
{
    bool NewXBee = false; //Until the system can confirm that the XBee is new it is set to false.
    if (XBee_IO.AddressingCommand("SH") != false)
    { //Reads the XBee Serial Number High, returning 4 bytes.
        for (int i = 0; i < 4; i++)
        {
            if (EEPROM.read(i) != XBee_IO.Incoming[i])
            {
                EEPROM.update(i, XBee_IO.Incoming[i]);
                Serial.println(F("SH did not match EEPROM.")); //Can be commented out when in production.
                NewXBee = true;
            }
        }
    }
    if (XBee_IO.AddressingCommand("SL") != false)
    { //Reads the XBee Serial Number Low, returning 4 bytes.
        for (int i = 0; i < 4; i++)
        {
            if (EEPROM.read(i + 4) != XBee_IO.Incoming[i])
            {
                EEPROM.update(i + 4, XBee_IO.Incoming[i]);
                Serial.println(F("SL did not match EEPROM.")); //Can be commented out when in production.
                NewXBee = true;
            }
        }
    }
    if (NewXBee || config.FactoryReset) //If it is determined that the XBee is a new one, the network parameters are now updated.
    {
        Serial.println(F("New XBee detected."));

        //Serial.print(F("ID: ")); Serial.println(config.xbeeID, HEX);
        XBee_IO.AddressingCommand("ID", 0x09, config.xbeeID); //XBee PAN ID set to one stored in configuration.
        XBee_IO.AddressingCommand("WR", 0x09);
        XBee_IO.AddressingCommand("AC", 0x09);

        XBee_IO.AddressingCommand("EE", 0x09, 0x01); //Encryption is Enabled
        XBee_IO.AddressingCommand("WR", 0x09);
        XBee_IO.AddressingCommand("AC", 0x09);

        //Serial.print(F("Passcode: ")); Serial.println(config.xbeeKY);
        XBee_IO.AddressingCommand("KY", 0x09, config.xbeeKY); //XBee Encryption Key is set to one stored in configuration.
        XBee_IO.AddressingCommand("WR", 0x09);
        XBee_IO.AddressingCommand("AC", 0x09);

        XBee_IO.AddressingCommand("JV", 0x09, 0x01);
        XBee_IO.AddressingCommand("WR", 0x09);
        XBee_IO.AddressingCommand("AC", 0x09);
    }
    return;
}

/* -------------------------------------------------------------------------- */
/*                          Power On Self Test (POST)                         */
/* -------------------------------------------------------------------------- */
void POST()
{
    bool PASS = HIGH; //System considered passable until otherwise proven.
    Serial.print(F("Initializing SD card..."));

    if (!SD.begin(10)) //Checks that an SD card is available and can be read, will not proceed until card is properly inserted.
    {
        Serial.println(F("FAILED - SD Initialization"));
        POSTfail(1000); //Halts program and flashes error.
    }

    Serial.println(F("SUCCESS"));
    Serial.println();

    file = SD.open("settings.txt"); //Reads in the content from the settings.text file.

    const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 136;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, file);

    if (error)
    {
        Serial.print(F("FAILED - deserializeJson"));
        Serial.println(error.c_str());
        POSTfail(2000);
    }

    config.debug = false;        //Set a default to prevent lockup.
    config.debug = doc["debug"]; //Sets the debug configuration vairiable from the SD card.
    config.networked = doc["networked"];
    config.FactoryReset = doc["FactoryReset"];
    JsonObject xbee = doc["xbee"]; //Create a new accessible object to extract the XBee configuration varriables.
    config.xbeeID = xbee["id"];    //Stores the XBee PAN ID variable.
    config.xbeeKY = xbee["ky"];    //Stores the XBee Encryption Key variable.

    if (!config.debug) //Only run if DEBUG is false.
    {
        //--- Testing Wiegand RFID reader. ---
        pinMode(WiegandLED, INPUT);

        if (digitalRead(WiegandD0) != HIGH)
        {
            Serial.print(F("! -> RFID D0 Not Connected, ")); //Changed "Data LOW" to "D0"
            Serial.println(F("Check PIN 2 Connections."));
            PASS = LOW;
        }
        if (digitalRead(WiegandD1) != HIGH)
        {
            Serial.print(F("! -> RFID D1 Not Connected, ")); //changed "Data HIGH" to "D1"
            Serial.println(F("Check PIN 3 Connections."));
            PASS = LOW;
        }
        if (digitalRead(WiegandLED) != HIGH)
        {
            Serial.print(F("! -> RFID LED Not Connected, "));
            Serial.println(F("Check PIN 11 Connections."));
            PASS = LOW;
        }
        pinMode(WiegandLED, OUTPUT);

        if (config.networked == true)
        {
            //--- Testing XBee connection. ---
            if (XBee_IO.AddressingCommand("SH") == 0)
            {
                Serial.print(F("! -> Unable to read XBee serial high 32 bits, "));
                Serial.println(F("Check XBee in properly inserted."));
                PASS = LOW;
            }
        }
    }
    else
    {
        Serial.println(F("--- DEBUG ENABLED ---"));
        Serial.println();
        PASS = HIGH;
    }

    //--- Validating POST restuls. ---
    if (PASS == HIGH)
    {
        Serial.println();
        Serial.println(F("POST PASSED"));
        Serial.println();
        digitalWrite(SystemLED, HIGH);
    }

    else
    {
        POSTfail(500);
    }
}

//Quick function to escape to if post fails.
void POSTfail(int BlinkRate)
{
    Serial.println();
    Serial.println(F("POST FAILED. HALTED."));
    while (1)
    {
        digitalWrite(SystemLED, HIGH);
        delay(BlinkRate);
        digitalWrite(SystemLED, LOW);
        delay(BlinkRate);
    }
}

//Function to check if the system is configuring for the first time, and if so to start XBee configuration.
//This will try to run even when XBee is not available and while DEBUG is enabled. Could prevent this from running while DEBUG is true or confirm XBee is present first.
int FirstUse()
{
    byte xbeeAttn[3] PROGMEM = {0x2B, 0x2B, 0x2B};                //Called when XBee is in AT mode to wake it up
    byte ATAP1[6] PROGMEM = {0x41, 0x54, 0x41, 0x50, 0x31, 0x0D}; //Configure AT XBee for API mode
    byte ATWR[5] PROGMEM = {0x41, 0x54, 0x57, 0x52, 0x0D};        //Write setting chages in AT mode
    byte ATAC[5] PROGMEM = {0x41, 0x54, 0x41, 0x43, 0x0D};        //Apply setting chages in AT mode
    byte ATCN[5] PROGMEM = {0x41, 0x54, 0x43, 0x4E, 0x0D};        //Exit command mode

    XBee.read();
    if (EEPROM.read(100) != 01 || config.FactoryReset)
    {
        Serial.println(F("Starting First Time Config."));
        for (int i = 0; i < 3; i++)
        {
            XBee.write(xbeeAttn[i]);
        }
        delay(100);
        for (int i = 0; i < 6; i++)
        {
            XBee.write(ATAP1[i]);
        }
        delay(100);
        for (int i = 0; i < 5; i++)
        {
            XBee.write(ATWR[i]);
        }
        delay(100);
        for (int i = 0; i < 5; i++)
        {
            XBee.write(ATAC[i]);
        }
        delay(100);
        XBee_IO.AddressingCommand("NR");

        EEPROM.write(100, 01);
        Serial.println(F("Configuration Complete"));
        Serial.println();
    }
    return;
}

/* -------------------------------------------------------------------------- */
/*                           Access Granted Handler                           */
/* -------------------------------------------------------------------------- */
void Allow()
{
    unsigned long LCcurrentMillis = 0; //Current mills reading from arduino.
    unsigned long LCpreviousMillis = 0;
    LCcurrentMillis = millis();
    LCpreviousMillis = LCcurrentMillis;
    while ((LCcurrentMillis - LCpreviousMillis) <= LatchTime)
    {
        wdt_reset();
        digitalWrite(DoorA, HIGH);
        digitalWrite(WiegandLED, LOW);
        XBee.read();
        LCcurrentMillis = millis();
    }
    digitalWrite(DoorA, LOW);
    digitalWrite(WiegandLED, HIGH);
    return;
}

/* -------------------------------------------------------------------------- */
/*                            Access Denied Handler                           */
/* -------------------------------------------------------------------------- */
void Deny()
{
    unsigned long LCcurrentMillis = 0;
    unsigned long LCpreviousMillis = 0;

    LCcurrentMillis = millis();
    LCpreviousMillis = LCcurrentMillis;
    digitalWrite(DoorA, LOW);
    digitalWrite(WiegandLED, LOW);
    while ((LCcurrentMillis - LCpreviousMillis) <= 500)
    {
        XBee.read();
        LCcurrentMillis = millis();
    }
    LCcurrentMillis = millis();
    LCpreviousMillis = LCcurrentMillis;
    digitalWrite(WiegandLED, HIGH);
    while ((LCcurrentMillis - LCpreviousMillis) <= 500)
    {
        XBee.read();
        LCcurrentMillis = millis();
    }
    LCcurrentMillis = millis();
    LCpreviousMillis = LCcurrentMillis;
    digitalWrite(WiegandLED, LOW);
    while ((LCcurrentMillis - LCpreviousMillis) <= 500)
    {
        XBee.read();
        LCcurrentMillis = millis();
    }
    digitalWrite(WiegandLED, HIGH);
    return;
}
