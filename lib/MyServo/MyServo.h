#ifndef MYSERVO_H
#define MYSERVO_H
#pragma once

#include "Arduino.h"
#include "Servo.h"
#include "MyGlass.h"

#define SERVO_PIN_SIG A2

#define OFFSET 0
#define MAX_MOVE_TIME 1000      // HOW LONG IT CAN MOVE

class MyServo : public Servo {
private:
    float* angles;              // Dynamically allocated array for glasses positions
    unsigned int glasses;       // Number of glasses
public:
    MyServo(unsigned int glasses, const GlassConfig* config);
    ~MyServo();

    // Write position to the glass
    void MoveTo(unsigned int glass);

};

#endif