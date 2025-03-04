#include "MyServo.h"

MyServo::MyServo(unsigned int glasses, GlassConfig* config) : Servo () {
    pinMode(SERVO_PIN_SIG, OUTPUT);

    attach(SERVO_PIN_SIG);                      // LET IT MOVE

    write(0);                                   // Set default position
    delay(200);                                 // LITTLE DELAY TO SET SERVO

    detach();                                   // STOP JITTERING

    this->glasses = glasses;
    angles = new int[glasses];

    for (unsigned int i = 0; i < glasses; i++) {
        angles[i] = (config + sizeof(*config)/sizeof(GlassConfig) * i)->angle;
    }
}

MyServo::~MyServo(){}

void MyServo::MoveTo(unsigned int glass){
    attach(SERVO_PIN_SIG);                      // LET IT MOVE

    write(angles[glass] + OFFSET);              // Set glass position

    delay(MAX_MOVE_TIME);                       // LITTLE DELAY TO SET SERVO

    detach();                                   // STOP JITTERING
}

bool MyServo::CheckIfSet(unsigned int glass){
    return read() == int(angles[glass]);
}
