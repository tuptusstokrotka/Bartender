#ifndef BARTENDER_H
#define BARTENDER_H

#pragma once

#include "EEPROM.h"
#include "eeprom_address.h"

#include "MyGlass.h"
#include "MyEncoder.h"
#include "MyOled.h"
#include "MyServo.h"

enum BartenderState{
    idle,
    serving,
    calibration
};

class Bartender{
private:
    BartenderState status   = idle;
    unsigned int volume     = 0;
    unsigned int glasses    = 0;

    MyGlass **myGlasses = nullptr;
    MyOled myDisplay;
    MyEncoder myEncoder;
    MyServo *myServo;

    void GlassesUpdate();
    void DisplayUpdate();
    void EncoderUpdate();

    BartenderState GetState();
    void SetState(BartenderState status);

public:
    Bartender(unsigned int glasses, const GlassConfig* config);
    ~Bartender();

    unsigned int GetVolume();
    void SetVolume(unsigned int volume);

    void Calibrate();
    void ServeDrinks();

    void Update();
};

#endif