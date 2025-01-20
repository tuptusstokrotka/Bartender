#ifndef MYSERVO_H
#define MYSERVO_H
#pragma once

#include "Arduino.h"
#include "Servo.h"
#include "MyGlass.h"

#define SERVO_PIN_SIG A0

#define OFFSET 0
#define MAX_MOVE_TIME 1000      // HOW LONG IT CAN MOVE

class MyServo : public Servo {
private:
    int* angles;                // Dynamically allocated array for glasses positions
    unsigned int glasses;       // Number of glasses
public:
    MyServo(unsigned int glasses, GlassConfig* config);
    ~MyServo();

    // Write position to the glass
    void MoveTo(unsigned int glass);

    /**
     * @brief Check if servo position has been set.
     * @param glass desired angle
     * @note This will only check if servo has angle set, not reached
     * @return true if angle has been set
     */
    bool CheckIfSet(unsigned int glass);

};

#endif