#ifndef MYENCODER_H
#define MYENCODER_H
#pragma once

#define ENC_ISR

#include <Arduino.h>
#ifdef ENC_ISR
#include <Encoder.h>
#endif

#include "MyButton.h"

// pin setup
#define PIN_CLK     3 // A2 -> D3 (ISR)
#define PIN_DT      2 // A3 -> D2 (ISR)
#define PIN_SW      A6

// Encoder press / hold defines
#define HOLD_MS     2000
#define HOLD        2
#define PRESS       1
#define RELEASED    0

#define DEBOUNCE    20                  // Debouncing time in milliseconds
#define RESOLUTION  4                   // How many values is one tick (rotation)

#ifdef ENC_ISR
class MyEncoder : public MyButton, Encoder{
private:

#else
class MyEncoder : public MyButton{
private:
    unsigned int pinA;                  // Encoder pin A
    unsigned int pinB;                  // Encoder pin B

#endif
    // Encoder knob
    int last_Position       = 0;        // Last encoder reading value
    unsigned long last_tick = 0;        // Last encoder revolution
    // Encoder button
    unsigned long last_press_tick = 0;
    int last_State = LOW;               // Last Encoder Switch state

public:
    MyEncoder(unsigned int pinA = PIN_CLK, unsigned int pinB = PIN_DT);
    ~MyEncoder();

    /**
     * @brief Read Encoder and update value
     * @note This will have poor performance over ISR
     */
    void Update(long &value);
};

#endif