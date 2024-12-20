#ifndef BARTENDER_H
#define BARTENDER_H

#pragma once

#include "EEPROM.h"
#include "eeprom_address.h"

#include "MyLeds.h"
#include "MyServo.h"
#include "MyGlass.h"
#include "MyOled.h"
#include "MyEncoder.h"
#include "MyPump.h"

enum BartenderState{
    idle,
    serving,
    calibration
};

class Bartender{
private:
    BartenderState status   = idle;
    int volume              = 0;
    unsigned int glasses    = 0;

    // Dynamically allocated based on the # glasses
    MyLeds *led         = nullptr;
    MyServo *myServo    = nullptr;
    MyGlass **myGlasses = nullptr;

    // Static independent on glasses
    // MyOled myDisplay;
    MyEncoder myEncoder;
    MyPump myPump;

    /**
     * @brief Check each glass strain gauge reading.
     * Update glass status.
     * @note Status includes selected volume,
     * it marks glass as full or half (with liquid but not full)
     */
    void GlassCheck(void);
    /**
     * @brief Update currently displayed data on the OLED
     */
    void DisplayUpdate(void);
    /**
     * @brief Read and check encoder status
     * Update both encoder revolutions and button press.
     *
     * @note default behaviour:
     *
     * - PRESS - serve drinks
     *
     * - HOLD - calibrate
     */
    void EncoderUpdate(void);

    BartenderState GetState();
    void SetState(BartenderState status);

public:
    Bartender(unsigned int glasses, GlassConfig* config);
    ~Bartender();

    unsigned int GetVolume();
    void SetVolume(unsigned int volume);

    void Update();
    void Calibrate();
    void ServeDrinks();
};

#endif