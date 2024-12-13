#include "MyPump.h"

MyPump::MyPump(){
    pinMode(DCMOTOR_PIN_COIL1, OUTPUT);                 // SET PIN AS OUTPUT
}

MyPump::~MyPump(){}

void MyPump::Start(){
    digitalWrite(DCMOTOR_PIN_COIL1, HIGH);              // TURN ON WATER PUMP
}

void MyPump::Stop(){
    digitalWrite(DCMOTOR_PIN_COIL1, LOW);               // TURN OFF WATER PUMP
}
