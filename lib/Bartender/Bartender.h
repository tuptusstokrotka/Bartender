#ifndef BARTENDER_H
#define BARTENDER_H

#pragma once

#include "EEPROM.h"
#include "eeprom_address.h"

#include "MyGlass.h"
#include "MyLeds.h"
#include "MyOled.h"
#include "MyEncoder.h"
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
    MyLeds *led;
    MyOled myDisplay;
    MyEncoder myEncoder;
    MyServo *myServo;

    /**
     * @brief Check each glass strain gauge reading.
     * Update glass status.
     * @note Status includes selected volume,
     * it marks glass as full or half (with liquid but not full)
     */
    void GlassesUpdate(void);
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
    Bartender(unsigned int glasses, const GlassConfig* config);
    ~Bartender();

    unsigned int GetVolume();
    void SetVolume(unsigned int volume);

    void Calibrate();
    void ServeDrinks();

    void Update();

    void Test();
};

#endif