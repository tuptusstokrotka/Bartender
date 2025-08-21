#ifndef MYPUMP_H
#define MYPUMP_H
#pragma once

#include "Arduino.h"
#include "pin_config.h"

class MyPump{
public:
    MyPump();
    ~MyPump();

    /**
     * @brief Start water pump to pour drink
     */
    void Start();

    /**
     * @brief Stop water pump
     */
    void Stop();
};

#endif