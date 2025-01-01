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

    long glass_reading        = 0;  // Last Strain Gauge beam reading
    unsigned int glass_weight = 0;  // Glass measured weight
    unsigned int glass_filled = 0;  // Glass poured mililiters

    void SetState(GlassState state);
    unsigned int GetGlassWeight(void);
    void ResetGlassWeight(void);

    /**
     * @brief Find difference between desired volume of drink and volume in glass
     *
     * @param volume Current volume desired to pour in milliliters
     * @return Difference between volumes in milliliters
     */
    unsigned int GetDifference(unsigned int volume);
public:
    MyGlass(const GlassConfig* config, MyPump* myPump, MyLeds* led, unsigned int index);
    ~MyGlass();

    /**
     * @brief Set the Glass Weight
     */
    void SetGlassWeight(void);
    /**
     * @brief Get the Filled volume in glass
     * @return Volume in milliliters
     */
    unsigned int GetFilled(void);
    /**
     * @brief Get the State of a glass
     * @return + FILLED
     * @return + HALF
     * @return + EMPTY
     * @return + NO_GLASS
     */
    GlassState GetState(void);

    /**
     * @brief Calibrate strain gauge
     */
    void Calibrate(void);

    /**
     * @brief Update glass volume poured. Used to verify glass status
     * @param volume poured milliliters of drink that is in glass
     */
    void Fill(unsigned int volume);

    /**
     * @brief Check Glass switch state to find glass state
     * @param volume Current volume desired to pour in milliliters
     */
    void StatusCheck(unsigned int volume);

    /**
     * @brief Set glass LED according to the state
     */
    void StatusLED(void);
    void StatusDEBUG(int reading);
};

#endif