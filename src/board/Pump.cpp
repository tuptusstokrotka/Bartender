#include "board/Pump.h"

MyPump::MyPump(){
    pinMode(PUMP_PIN_COIL1, OUTPUT);                 // SET PIN AS OUTPUT
    digitalWrite(PUMP_PIN_COIL1, LOW);               // TURN OFF WATER PUMP
}

MyPump::~MyPump(){}


void MyPump::Start(){
    digitalWrite(PUMP_PIN_COIL1, HIGH);              // TURN ON WATER PUMP
}

void MyPump::Stop(){
    digitalWrite(PUMP_PIN_COIL1, LOW);               // TURN OFF WATER PUMP
}