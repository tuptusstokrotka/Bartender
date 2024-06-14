#include "MyServo.h"
#include "Arduino.h"

MyServo::MyServo() : Servo () {
    pinMode(SERVO_PIN_SIG, OUTPUT);
    Back();                                     // MOVE SERVO TO DEFAULT POSITION
}

MyServo::~MyServo(){}

void MyServo::MoveTo(uint8_t glass){
    attach(SERVO_PIN_SIG);                      // LET IT MOVE

    write(btn_pos[glass] + OFFSET);             // SET POSITION TO THE DESIRED GLASS
    delay(MAX_MOVE_TIME);                       // LITTLE DELAY TO LET IT SET SERVO

    detach();                                   // STOP JITTERING
}

void MyServo::Back(){
    MoveTo(0 + OFFSET);                         // MOVE SERVO TO DEFAULT POSITION
}
