#ifndef MYPUMP_H
#define MYPUMP_H
#pragma once

#include "Arduino.h"

#define DCMOTOR_PIN_COIL1 5

#define ML_PER_SECOND (44.5 / 5.0)          // WATERPUMP FLOW_RATE

class MyPump{
private:
    unsigned long turn_off_time;            // PUMP SHIFT TIME END
public:
    MyPump();
    ~MyPump();

    /**
     * @brief Start water pump to pour drink
     * @param volume Current volume desired to pour in milliliters
     */
    void Start(uint16_t volume);

    /**
     * @brief Check if water pump is still working based on the time required to pour desired ml.
     * @return true - Pump is working
     */
    bool Working();

    /**
     * @brief Stop water pump
     */
    void Stop();
};

#endif