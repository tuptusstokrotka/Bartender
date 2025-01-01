#ifndef MYPUMP_H
#define MYPUMP_H
#pragma once

#include "Arduino.h"

#define DCMOTOR_PIN_COIL1 1 // D1

class MyPump{
public:
    MyPump();
    ~MyPump();

    /**
     * @brief Start water pump to pour drink
     */
    void Start();

    /**
     * @brief Start water pump and wait given ms to stop
     * @note Function is blocking rest of the program
     * @param time milliseconds to wait
     */
    void Start(unsigned int time);

    /**
     * @brief Stop water pump
     */
    void Stop();
};

#endif