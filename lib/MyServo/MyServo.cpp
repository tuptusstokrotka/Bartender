#include "MyServo.h"
#include "Arduino.h"

MyServo::MyServo() : Servo () {
    pinMode(SERVO_PIN_SIG, OUTPUT);
}

MyServo::~MyServo(){}

void MyServo::Init(){
    // Servo Inicjalizacja
    attach(SERVO_PIN_SIG);                      // LET IT MOVE

    write(0);                                   // Pozycja początkowa
    delay(200);                                 // LITTLE DELAY TO SET SERVO

    detach();                                   // STOP JITTERING
}

void MyServo::MoveTo(uint8_t glass){
    attach(SERVO_PIN_SIG);                      // LET IT MOVE

    write(btn_pos[glass] + OFFSET);
    delay(1000);                                // LITTLE DELAY TO SET SERVO

    detach();                                   // STOP JITTERING

}
