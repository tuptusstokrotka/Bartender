#ifndef MyGlass_H
#define MyGlass_H
#pragma once

#include "GlassConfig.h"
#include "MyWeight.h"
#include "MyLeds.h"
#include "MyPump.h"

#define FILLED      3
#define HALF        2
#define EMPTY       1
#define NO_GLASS    0

enum GlassState{
    No_Glass,
    Empty,
    Half,
    Filled,
};

class MyGlass{
private:
    //DELETE the pin - deprecated
    unsigned int pin;               // ARDUINO PIN ASSIGNED TO THE SWITCH
    unsigned int glass_index;       // Glass index for the aRGB led

    MyWeight* beam = nullptr;       // Strain Gauge beam pointer
    MyLeds* led = nullptr;          // Led pointer
    MyPump myPump;                  // Pump instance

    GlassState status = No_Glass;   // Glass status
    unsigned int filled_ml = 0;     // Glass poured mililiters

    /**
     * @brief Find difference between desired volume of drink and volume in glass
     *
     * @param volume Current volume desired to pour in milliliters
     * @return volume - filled_ml in milliliters
     */
    unsigned int Difference(unsigned int volume);     // Count the remaining volume to pour

public:
    MyGlass(const GlassConfig* config, MyLeds* led, unsigned int index);
    ~MyGlass();

    /**
     * @brief Update glass volume poured. Used to verify glass status
     * @param volume poured milliliters of drink that is in glass
     */
    void Fill(unsigned int volume);

    /**
     * @brief Check Glass switch state to find glass state
     *
     * @param volume Current volume desired to pour in milliliters
     * @return + FILLED (3)
     * @return + HALF (2)
     * @return + EMPTY (1)
     * @return + NO_GLASS (0)
     */
    void Check(unsigned int volume);

    void Calibrate(void);

    unsigned int GetVolume(void);
};

#endif