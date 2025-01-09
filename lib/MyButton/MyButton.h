#ifndef MYBUTTON_H
#define MYBUTTON_H
#pragma once

#include <Arduino.h>

#define UNKNOWN     3
#define RELEASE     0
#define PRESS       1
#define HOLD        2

enum ButtonState{
    Unknown = -1,   // Between states, not defined
    Release = 0,
    Press   = 1,
    Hold    = 2,
};

#define THRESHOLD 800               // analog value that will define HIGH state

class MyButton{
private:
    int pinnum;
    unsigned long hold_ms;          // Time required to trigger HOLD
    int last_State = RELEASE;       // Last Switch state
    unsigned long press_time = 0;   // Press time in ms

public:
    MyButton(int pin, int hold = 400);
    ~MyButton();

    /**
     * @brief Check if button is pressed, held or released
     * @note Function is not returning state until determined based on pressing time and pin state
     *
     * @return + HOLD once after trigger time
     * @return + PRESS once on button up
     * @return + RELEASE once
     */
    int GetState();

    /**
     * @brief Check if button is pressed, holding or released
     * @note Function is always returning state
     *
     * @return + HOLD continuously
     * @return + PRESS once on button down
     * @return + RELEASE continuously
     */
    int GetStateRepeat();

    int GetStateOnRelease();
};

#endif