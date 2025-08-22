#ifndef MYENCODER_H
#define MYENCODER_H
#pragma once

#include <Arduino.h>
#include "pin_config.h"

#include "FastButton.h"
#include <Encoder.h>

// Encoder press / hold defines
#define HOLD_MS     1500
#define RESOLUTION  4                   // How many values is one tick (rotation)

#define MIN_VALUE 0
#define MAX_VALUE 100

class MyEncoder : public FastButton, Encoder{
private:
    // Encoder knob
    int32_t last_Position = 0;          // Last encoder reading value

public:
    MyEncoder(uint8_t pinA = ENC_PIN_CLK, uint8_t pinB = ENC_PIN_DT);
    ~MyEncoder();

    /**
     * @brief Read Encoder and update value
     * @note This will have poor performance over IS
     * return true if value has been updated
     */
    bool Update(int32_t &value);
};

#endif