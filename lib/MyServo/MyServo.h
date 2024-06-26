#ifndef MYSERVO_H
#define MYSERVO_H
#pragma once

#include <Servo.h>

#define SERVO_PIN_SIG A2

#define OFFSET 0
#define MAX_MOVE_TIME 1000      // HOW LONG IT CAN MOVE

class MyServo : public Servo {
private:
    // kąty dla każdego przycisku / szklanki
    // float btn_pos[7] = { 0, 28, 60, 90, 124, 160, 180 };
    float btn_pos[7] = { 0, 27, 58, 91, 121, 152, 190 };
public:
    MyServo();
    ~MyServo();

    // Initialize Servo
    void Init();

    // Write position to the glass
    void MoveTo(uint8_t glass);

};

#endif