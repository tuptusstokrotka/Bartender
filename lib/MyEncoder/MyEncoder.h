#ifndef MYENCODER_H
#define MYENCODER_H
#pragma once

#include <Encoder.h>

// Encoder naciśnij / przytrzymaj definicje
#define HOLD_MS     400
#define HOLD        2
#define PRESS       1
#define RELEASED    0

#define DEBOUNCE    50  // Debouncing 50ms

// pin setup
#define PIN_CLK     2
#define PIN_DT      3
#define PIN_SW      A1

class MyEncoder : public Encoder {
private:
    bool lastState = LOW;               // Last Encoder Switch state
    unsigned long lastTick = 0;         // Last encoder revolution ms
    uint8_t volume = 0;                 // Drink volume to pour

public:
    MyEncoder(uint8_t pin1 = PIN_CLK, uint8_t pin2 = PIN_DT);
    ~MyEncoder();

    /**
     * @brief Check if encoder button is pressed, holding or released
     * @return + HOLD (2)
     * @return + PRESS (1)
     * @return + RELEASE (0)
     */
    uint8_t Pressed();

    /**
     * @brief Update Encoder while rotating
     */
    void Update();

    /**
     * @brief Get volume counter value
     * @return Current volume desired to pour in milliliters
     */
    uint8_t GetVolume();
    /**
     * @brief Set volume to pour
     * @param volume in milliliters
     */
    void SetVolume(uint8_t volume);
};

#endif