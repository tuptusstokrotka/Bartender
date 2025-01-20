#include "MyPump.h"

MyPump::MyPump(){
    pinMode(DCMOTOR_PIN_COIL1, OUTPUT);                 // SET PIN AS OUTPUT
    digitalWrite(DCMOTOR_PIN_COIL1, LOW);               // TURN OFF WATER PUMP
}

MyPump::~MyPump(){}

void MyPump::Start(){
    digitalWrite(DCMOTOR_PIN_COIL1, HIGH);              // TURN ON WATER PUMP
}

void MyPump::Start(unsigned int time){
    unsigned long current_time = millis();
    Start();

    while(current_time + time > millis()){
        /* wait */
    }

    Stop();
}

void MyPump::Stop(){
    digitalWrite(DCMOTOR_PIN_COIL1, LOW);               // TURN OFF WATER PUMP
}