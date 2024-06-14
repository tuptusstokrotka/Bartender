#ifndef MYSERVO_H
#define MYSERVO_H
#pragma once

#include <Servo.h>

#define SERVO_PIN_SIG A2
#define OFFSET 5                // PROJECT OFFSET FOR 1ST GLASS
#define MAX_MOVE_TIME 1000      // HOW LONG IT CAN MOVE

class MyServo : public Servo {
private:
    // Angle for each glass in range 0 - 180
    float btn_pos[7] = { 0, 27.5, 55, 83, 112, 140, 170 };
public:
    MyServo();
    ~MyServo();

    /**
     * @brief Move servo to the desired glass position
     * @param glass number of glass
     */
    void MoveTo(uint8_t glass);

    /**
     * @brief Move servo to the default position 0
     * @note If offset is set it will be added to that
     */
    void Back();
};

#endif