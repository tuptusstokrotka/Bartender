#ifndef MyGlass_H
#define MyGlass_H
#pragma once

#include "GlassConfig.h"
#include "MyWeight.h"
#include "MyLeds.h"
#include "MyPump.h"

#define STOP_ML_OFFSET 0

enum GlassState{
    No_Glass,
    Empty,
    Half,
    Filled,
};

class MyGlass{
private:
    MyWeight* beam = nullptr;       // Strain Gauge beam pointer
    MyLeds* led = nullptr;          // Led pointer
    MyPump* myPump = nullptr;       // Pump instance

    GlassState status = No_Glass;   // Glass status
    unsigned int glass_index;       // Glass index for the aRGB led
    unsigned int glass_weight = 0;
    unsigned int filled_ml = 0;     // Glass poured mililiters

    void SetState(GlassState state);
    void SetGlassWeight(unsigned int grams);

    unsigned int GetGlassWeight(void);
    /**
     * @brief Find difference between desired volume of drink and volume in glass
     *
     * @param volume Current volume desired to pour in milliliters
     * @return volume - filled_ml in milliliters
     */
    unsigned int GetDifference(unsigned int volume);
public:
    MyGlass(const GlassConfig* config, MyPump* myPump, MyLeds* led, unsigned int index);
    ~MyGlass();

    unsigned int GetFilled(void);
    GlassState GetState(void);

    void Calibrate(void);

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
};

#endif