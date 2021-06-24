/*
 * LatchCtrl.h - Library for user called latching.
 * Created by Justin D. Merrell, April 22nd, 2020.
 *
 * Specifies what position a relay should be in.
 * Provides user with corrisponding visual feedback.
 */

#ifndef LatchCtrl_h
#define LatchCtrl_h

#include <Arduino.h>

class LatchCtrl
{
private:
    byte _RelayPin;
    byte _IndicatorPin;
    byte _LatchTime;

    //unsigned long LCcurrentMillis = 0;		//Current mills reading from arduino.
    //unsigned long LCpreviousMillis = 0;

public:
    LatchCtrl(byte RelayPin, byte VisualIndicatorPin, int LatchTime = 10000);
    void Lock();
    void Allow();
    void Deny();
    void Hold();
};

#endif
