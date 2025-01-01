#ifndef MYENCODER_H
#define MYENCODER_H
#pragma once

// #include <Encoder.h>
#include <Arduino.h>
#include "MyButton.h"

// pin setup
#define PIN_CLK     A2
#define PIN_DT      A3
#define PIN_SW      A6

// Encoder press / hold defines
#define HOLD_MS     2000
#define HOLD        2
#define PRESS       1
#define RELEASED    0

#define DEBOUNCE    50                  // Debouncing 50ms
#define RESOLUTION  4                   // How many values is one tick (rotation)

//FIXME - alternative when using interrupt pins
// class MyEncoder : public MyButton, Encoder{
class MyEncoder : public MyButton{
private:
    unsigned int pinA;                  // Encoder pin A
    unsigned int pinB;                  // Encoder pin B

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
    void Update(int *value);

    /**
     * @brief Read Encoder and update value using INTERRUPTS
     */
    void UpdateISR(int *value);
};

#endif