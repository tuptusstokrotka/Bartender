#ifndef MyGlass_H
#define MyGlass_H
#pragma once

#include "GlassConfig.h"
#include "MyWeight.h"
#include "MyLeds.h"

#ifndef GLASS_DEBUG_PRINT_ENABLE
#define GLASS_DEBUG_PRINT_ENABLE 0
#endif

#if GLASS_DEBUG_PRINT_ENABLE == 1
    #include <Arduino.h>
    #define GLASS_DEBUG_PRINT(s)    Serial.print(s);
    #define GLASS_DEBUG_PRINTLN(s)  Serial.println(s);
#else
    #define GLASS_DEBUG_PRINT(s)    ;
    #define GLASS_DEBUG_PRINTLN(s)  ;
#endif

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

    GlassState status = No_Glass;   // Glass status
    unsigned int glass_index;       // Glass index for the aRGB led

    long glass_reading  = 0;        // Last Strain Gauge beam reading
    long glass_weight   = 0;        // Glass measured weight
    long glass_filled   = 0;        // Glass poured mililiters

    void SetState(GlassState state);
    void ResetGlassWeight(void);

    /**
     * @brief Find difference between desired volume of drink and volume in glass
     *
     * @param volume Current volume desired to pour in milliliters
     * @return Difference between volumes in milliliters
     */
    long GetDifference(long volume);
public:
    MyGlass(const GlassConfig* config, MyLeds* led, unsigned int index);
    ~MyGlass();

    /**
     * @brief Set the Glass Weight
     */
    void SetGlassWeight(void);
    long GetGlassWeight(void);

    /**
     * @brief Get the Filled volume in glass
     * @return Volume in milliliters
     */
    long GetFilled(void);
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
     * @brief Check Glass switch state to find glass state
     * @param volume Current volume desired to pour in milliliters
     */
    void StatusCheck(unsigned int volume);

    /**
     * @brief Set glass LED according to the state
     */
    void StatusLED(void);
    /**
     * @brief Set glass LED according volume percent
     */
    void PercentLED(int percent);
    void StatusDEBUG(int reading);
};

#endif