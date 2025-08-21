#include "board/Servo.h"

MyServo::MyServo() {
    pinMode(SERVO_PIN_SIG, OUTPUT);

    servo.attach(SERVO_PIN_SIG);                  // LET IT MOVE

    servo.write(0);                               // Set default position
    delay(200);                                   // LITTLE DELAY TO SET SERVO

    servo.detach();                               // STOP JITTERING
}

MyServo::~MyServo(){}

void MyServo::MoveTo(int angle){
    servo.attach(SERVO_PIN_SIG);                  // LET IT MOVE

    set_angle = angle;                            // Update the set angle
    servo.write(angle + OFFSET);                  // Set glass position
    delay(MAX_MOVE_TIME);                         // LITTLE DELAY TO SET SERVO

    servo.detach();                               // STOP JITTERING
}

bool MyServo::CheckIfSet(int angle){
    int currentPos = servo.read();
    int targetPos = angle;                        // Use the passed angle parameter
    return abs(currentPos - targetPos) <= 2;      // 2 degree tolerance
}
