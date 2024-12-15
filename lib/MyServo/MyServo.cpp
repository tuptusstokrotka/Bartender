#include "MyServo.h"

MyServo::MyServo(unsigned int glasses, const GlassConfig* config) : Servo () {
    pinMode(SERVO_PIN_SIG, OUTPUT);

    attach(SERVO_PIN_SIG);                      // LET IT MOVE

    write(0);                                   // Set default position
    delay(200);                                 // LITTLE DELAY TO SET SERVO

    detach();                                   // STOP JITTERING

    this->glasses = glasses;
    angles = new float[glasses];

    for (unsigned int i = 0; i < glasses; i++) {
        angles[i] = i;                  //THIS WORKS
        // angles[i] = config->angle[i];   //THIS FAILED
        *angles = config->angle;        //THIS WORKS
    }

    //DEBUG
    for (unsigned int i = 0; i < glasses; i++) {
        Serial.print(angles[i]);
        Serial.print(" ");
    }
    Serial.println(" ");
    Serial.println(" ");
}

MyServo::~MyServo(){}

void MyServo::MoveTo(unsigned int glass){
    attach(SERVO_PIN_SIG);                      // LET IT MOVE

    write(angles[glass] + OFFSET);              // Set glass position

    // while (read() != (angles[glass] + OFFSET)){;}
    delay(MAX_MOVE_TIME);                       // LITTLE DELAY TO SET SERVO

    detach();                                   // STOP JITTERING
}
