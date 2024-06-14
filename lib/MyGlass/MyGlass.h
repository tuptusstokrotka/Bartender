#ifndef MyGlass_H
#define MyGlass_H
#pragma once

#include "Arduino.h"

#define MICROSWITCH_1_PIN_COM 4
#define MICROSWITCH_2_PIN_COM 6
#define MICROSWITCH_3_PIN_COM 7
#define MICROSWITCH_4_PIN_COM 8
#define MICROSWITCH_5_PIN_COM 9
#define MICROSWITCH_6_PIN_COM 10
#define MICROSWITCH_7_PIN_COM 11
#define MICROSWITCH_8_PIN_COM 12


#define FILLED      3
#define HALF        2
#define EMPTY       1
#define NO_GLASS    0

class MyGlass{
private:
    uint8_t pin;                            // ARDUINO PIN ASSIGNED TO THE SWITCH
    uint8_t filled_ml = 0;                  // MEASURE POURED MILLILITERS OF DRINK

public:
    MyGlass(uint8_t pin);
    ~MyGlass();

    /**
     * @brief Check Glass switch state to find glass state
     *
     * @param volume Current volume desired to pour in milliliters
     * @return + FILLED (3)
     * @return + HALF (2)
     * @return + EMPTY (1)
     * @return + NO_GLASS (0)
     */
    uint8_t State(uint8_t volume);

    /**
     * @brief Update glass volume poured. Used to verify glass status
     * @param volume poured milliliters of drink that is in glass
     */
    void Fill(uint8_t volume);

    /**
     * @brief Find difference between desired volume of drink and volume in glass
     *
     * @param volume Current volume desired to pour in milliliters
     * @return volume - filled_ml in milliliters
     */
    uint8_t Difference(uint8_t volume);     // Count the remaining volume to pour
};

#endif