#ifndef MYPUMP_H
#define MYPUMP_H
#pragma once

#include "Arduino.h"
#include "pin_config.h"

class MyPump{
private:
    bool running = false;

public:
    MyPump();
    ~MyPump();

    bool IsRunning(void) { return running; }

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