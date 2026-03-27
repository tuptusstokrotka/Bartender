#ifndef MYSERVO_H
#define MYSERVO_H
#pragma once

#include <Arduino.h>
#include "pin_config.h"
#include <Servo.h>

#define OFFSET 0                // Offset from desired angle to actual angle - offset every set angle
#define MAX_MOVE_TIME 1000      // How long the servo will block the program via Delay()
#define SERVO_MIN 0
#define SERVO_MAX 180

class MyServo {
private:
    Servo servo;
    int set_angle = 0;
public:
    MyServo();
    ~MyServo();

    /**
     * @brief Write angle to the servo
     * @param angle in degrees
     * @return void
     */
    void MoveTo(int angle);

    /**
     * @brief Check if servo position has been set.
     * @param angle desired angle in degrees
     * @note This will only check if servo has angle set, not if reached (unsupported by servo)
     * @return true if angle has been set
     */
    bool CheckIfSet(int angle);

};

#endif