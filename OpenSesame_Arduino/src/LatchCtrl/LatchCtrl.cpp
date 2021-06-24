#include <Arduino.h>
#include "LatchCtrl.h"

LatchCtrl::LatchCtrl(byte RelayPin, byte VisualIndicatorPin, int LatchTime)
{
    pinMode(RelayPin, OUTPUT);
    pinMode(VisualIndicatorPin, OUTPUT);
    _RelayPin = RelayPin;
    _IndicatorPin = VisualIndicatorPin;
    _LatchTime = LatchTime;
}

void LatchCtrl::Lock()
{
    digitalWrite(_RelayPin, LOW);
    digitalWrite(_IndicatorPin, HIGH);
}

void LatchCtrl::Allow()
{

    digitalWrite(_RelayPin, HIGH);
    digitalWrite(_IndicatorPin, LOW);
    delay(_LatchTime);
    digitalWrite(_RelayPin, LOW);
    digitalWrite(_IndicatorPin, HIGH);
}

void LatchCtrl::Deny()
{
    digitalWrite(_RelayPin, LOW);
    digitalWrite(_IndicatorPin, LOW);
    delay(500);
    digitalWrite(_IndicatorPin, HIGH);
    delay(500);
    digitalWrite(_IndicatorPin, LOW);
    delay(500);
    digitalWrite(_IndicatorPin, HIGH);
}

void LatchCtrl::Hold()
{
    digitalWrite(_RelayPin, HIGH);
    digitalWrite(_IndicatorPin, LOW);
}
