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

#define STOP_ML_OFFSET   (long)2        // Offset from filled volume to stop pouring
#define SERVO_PUMP_DELAY delay(500);    //DEBUG Delay between powering servo & pump

#define UPDATE_INTERVAL(ms)     static unsigned long last_update = 0;  \
                                if(millis() - last_update > ms){       \
                                    last_update = millis();            \
                                    return;                            \
                                }

#define ENCODER_REFRESH_INTERVAL 1
#define GLASS_REFRESH_INTERVAL   5
#define DISPLAY_REFRESH_INTERVAL 20


enum BartenderState{
    idle,
    serving,
    calibration,
    finished
};

enum CalibrationStep {
    PREPARE,
    EMPTY_BEAM,
    KNOWN_WEIGHT,
    SET_GRAMS,
    CALCULATE
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
    int32_t volume          = 0;    // Currently set desired volume
    uint8_t glass_counter   = 0;    // Monitor number of placed glasses (1 bit per glass)
    uint8_t cur_glass       = 0;    // Currently chosen glass for action

    // Calibration variables
    CalibrationStep cal_step = PREPARE;
    bool cal_button_pressed = false;

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

    void SetNextGlass(void);

public:
    Bartender(void);
    ~Bartender(void);

    void AddGlass(GlassConfig &config);
    void AddGlass(uint8_t dout, uint8_t sck, int angle, int8_t led_index = -1);

    void Init(void);
    void Update(void);

    void Calibrate(void);
    void UpdateAdvancedCalibration(void);
    void ServeDrinks(void);
};

#endif