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
    // Dynamically allocated based on the # of the glasses
    MyLeds *led         = nullptr;
    MyServo *myServo    = nullptr;
    MyGlass **myGlasses = nullptr;
    unsigned int glasses = 0;       // the # of the glasses

    // Static independent on glasses
    MyEncoder myEncoder;
    MyPump myPump;

    BartenderState status   = idle; // Bartender working state
    int volume              = 0;    // Currently set desired volume
    int glass_counter       = 0;    // Monitor number of placed glasses (1 bit per glass)

    /**
     * @brief Check each glass strain gauge reading.
     * Update glass status and glass counter.
     * @note Status includes selected volume,
     * it marks glass as full or half (with liquid but not full)
     */
    void GlassUpdate(void);

    /**
     * @brief Update currently displayed data on the OLED
     */
    void DisplayUpdate(void);

    /**
     * @brief Read and check encoder status
     * Update both encoder revolutions and button press.
     *
     * @note default behaviour:
     * @note + PRESS - serve drinks
     * @note + HOLD - calibrate
     */
    void EncoderUpdate(void);

    /**
     * @brief Get the Bartender state
     * @return + idle
     * @return + serving
     * @return + calibration
     */
    BartenderState GetState(void);

    /**
     * @brief Set the Bartender state
     * @param status idle / serving / calibration
     */
    void SetState(BartenderState status);

public:
    Bartender(unsigned int glasses, GlassConfig* config);
    ~Bartender();

    void Update();
    void Calibrate();
    void ServeDrinks();
};

#endif