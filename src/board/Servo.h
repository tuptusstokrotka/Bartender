#ifndef MYSERVO_H
#define MYSERVO_H
#pragma once

#include <Arduino.h>
#include "pin_config.h"
#include <Servo.h>

#define OFFSET 0
#define MAX_MOVE_TIME 1000      // How long the servo will block the program via Delay()

class MyServo {
private:
    Servo servo;
    int set_angle = 0;
public:
    MyServo();
    ~MyServo();

    // Write position to the glass
    void MoveTo(int angle);

    /**
     * @brief Check if servo position has been set.
     * @param angle desired angle
     * @note This will only check if servo has angle set, not reached
     * @return true if angle has been set
     */
    bool CheckIfSet(int angle);

};

#endif