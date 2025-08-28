#ifndef MyGlass_H
#define MyGlass_H
#pragma once

#include "bartender/glassConfig.h"
#include "board/Weight.h"
#include "board/Leds.h"

enum GlassState{
    No_Glass,
    Empty,
    Half,
    Filled,
};

class MyGlass{
private:
    GlassState status   = No_Glass; // Glass status

    MyWeight* beam      = nullptr;  // Strain Gauge beam pointer
    int angle           = 0;        // Glass angle
    int8_t led_index    = -1;       // LED index (-1 if not set)

    long glass_brutto   = 0;        // Last Strain Gauge beam reading
    long glass_tare     = 0;        // Glass measured weight
    long glass_netto    = 0;        // Glass poured mililiters


    /**
     * @brief Find difference between desired volume of drink and volume in glass
     *
     * @param volume Current volume desired to pour in milliliters
     * @return Difference between volumes in milliliters
     */
    long GetDifference(long volume);
public:

    MyGlass(const GlassConfig &config);
    ~MyGlass();

    /**
     * @brief Set the Glass Weight
     */
    void SetGlassWeight(void);
    long GetGlassWeight(void) { return glass_tare; }
    void ResetGlassWeight(void) { glass_tare = 0; }

    /**
     * @brief Get the Filled volume in glass
     * @return Volume in milliliters
     */
    long GetFilled(void) { return glass_netto; }

    void SetState(GlassState state) { status = state; }
    /**
     * @brief Get the State of a glass
     * @return + FILLED
     * @return + HALF
     * @return + EMPTY
     * @return + NO_GLASS
     */
    GlassState GetState(void) { return status; }

    int GetAngle(void) { return angle; }
    int8_t GetLedIndex(void) { return led_index; }

    /**
     * @brief Calibrate strain gauge
     */
    void Calibrate(void);

    // Advanced calibration methods
    long GetRawReading(uint8_t samples = 5) { return beam->GetRawReading(samples); }
    float CalculateScaleFactor(long raw_empty, long raw_weight, long known_weight) {
        return beam->CalculateScaleFactor(raw_empty, raw_weight, known_weight);
    }
    void SetFactor(float factor) { beam->SetFactor(factor); }

    /**
     * @brief Check Glass switch state to find glass state
     * @param volume Current volume desired to pour in milliliters
     */
    void StatusCheck(unsigned int volume);
};

#endif