#include "MyPump.h"

MyPump::MyPump(){
    pinMode(DCMOTOR_PIN_COIL1, OUTPUT);                 // SET PIN AS OUTPUT
}

MyPump::~MyPump(){}

void MyPump::Start(uint16_t volume){
    digitalWrite(DCMOTOR_PIN_COIL1, HIGH);              // TURN ON WATER PUMP
    turn_off_time = millis();                           // CAPUTRE TURNING ON MOMENT
    turn_off_time += (volume / ML_PER_SECOND * 1000);   // SET DELAY BASED ON THE WATERPUMP FLOW_RATE
}

bool MyPump::Working(){
    return (millis() < turn_off_time);                  // TIME HAS PASSED?
}

void MyPump::Stop(){
    digitalWrite(DCMOTOR_PIN_COIL1, LOW);               // TURN OFF WATER PUMP
}
