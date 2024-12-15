#ifndef MYPUMP_H
#define MYPUMP_H
#pragma once

#include "Arduino.h"

#define DCMOTOR_PIN_COIL1 5

class MyPump{
public:
    MyPump();
    ~MyPump();

    /**
     * @brief Start water pump to pour drink
     */
    void Start();
    void Start(unsigned int time);

    /**
     * @brief Stop water pump
     */
    void Stop();
};

#endif