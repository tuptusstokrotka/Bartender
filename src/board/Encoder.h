#ifndef MYENCODER_H
#define MYENCODER_H
#pragma once

#include <Arduino.h>
#include "pin_config.h"

#include "FastButton.h"
#include <Encoder.h>

// Encoder press / hold defines
#define HOLD_MS     2000
#define RESOLUTION  4                   // How many values is one tick (rotation)

class MyEncoder : public FastButton, Encoder{
private:
    // Encoder knob
    int last_Position       = 0;        // Last encoder reading value
    unsigned long last_tick = 0;        // Last encoder revolution
    // Encoder button
    unsigned long last_press_tick = 0;
    int last_State = LOW;               // Last Encoder Switch state

public:
    MyEncoder(uint8_t pinA = ENC_PIN_CLK, uint8_t pinB = ENC_PIN_DT);
    ~MyEncoder();

    /**
     * @brief Read Encoder and update value
     * @note This will have poor performance over IS
     * return true if value has been updated
     */
    bool Update(long &value);
};

#endif