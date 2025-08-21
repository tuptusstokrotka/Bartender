#ifndef BARTENDER_H
#define BARTENDER_H

#pragma once

#include "EEPROM.h"
#include "eeprom_address.h"
#include <ArduinoSTL.h>
#include <vector>

#include "bartender/glass.h"
#include "board/Leds.h"
#include "board/Servo.h"
#include "board/Oled.h"
#include "board/Encoder.h"
#include "board/Pump.h"

#define STOP_ML_OFFSET   (long)2
#define SERVO_PUMP_DELAY delay(500); //DEBUG Delay between powering servo & pump

#define UPDATE_INTERVAL(ms)     static unsigned long last_update = 0;  \
                                if(millis() - last_update > ms){       \
                                    last_update = millis();            \
                                    return;                            \
                                }

enum BartenderState{
    idle,
    serving,
    calibration
};

class Bartender{
private:
    // Dynamically allocated based on the # of the glasses
    std::vector<MyGlass> myGlasses;
    MyLeds *led = nullptr;

    // Static independent on glasses
    MyServo myServo;
    MyEncoder myEncoder;
    MyPump myPump;

    // Runtime variables
    BartenderState status   = idle; // Bartender working state
    long volume             = 0;    // Currently set desired volume
    uint8_t glass_counter   = 0;    // Monitor number of placed glasses (1 bit per glass)
    uint8_t cur_glass       = 0;    // Currently chosen glass to pour or calibrate

    // Display flags
    bool display_glass_counter  = false;
    bool display_state_changed  = false;
    bool display_volume_changed = true;  // Show on bootup

    /**
     * @brief Check each glass strain gauge reading.
     * Update glass status and glass counter.
     * @note Status includes drink volume,
     * it marks glass as full or half (with liquid but not full)
     */
    void GlassUpdate(void);

    /**
     * @brief Read and check encoder status
     * Update both encoder revolutions and button press.
     *
     * @note + PRESS - serve drinks
     * @note + HOLD - calibrate
     */
    void EncoderUpdate(void);

    /**
     * @brief Update glasses LEDs according to the bartender status and glass state
     */
    void LedUpdate(void);

    /**
     * @brief Update currently displayed data on the OLED
     */
    void DisplayUpdate(void);

    /**
     * @brief Get the Bartender state
     * @return + idle
     * @return + serving
     * @return + calibration
     */
    BartenderState GetState(void) { return this->status; }

    /**
     * @brief Set the Bartender state
     * @param status idle / serving / calibration
     */
    void SetState(BartenderState status);

public:
    Bartender(void);
    ~Bartender(void);

    void AddGlass(GlassConfig &config);
    void AddGlass(uint8_t dout, uint8_t sck, int angle);

    void Init(void);
    void Update(void);

    void Calibrate(void);
    void ServeDrinks(void);
};

#endif