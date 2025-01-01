#include "MyEncoder.h"

//FIXME - alternative when using interrupt pins
// MyEncoder::MyEncoder(unsigned int pinA, unsigned int pinB) : MyButton(PIN_SW), Encoder(pinA, pinB) {

MyEncoder::MyEncoder(unsigned int pinA, unsigned int pinB) : MyButton(PIN_SW, 1000){
    this->pinA = pinA;
    this->pinB = pinB;

    pinMode(pinA, INPUT_PULLUP);  // Set pinA (A) as input with internal pull-up
    pinMode(pinB, INPUT_PULLUP);  // Set pinB (B) as input with internal pull-up
}

MyEncoder::~MyEncoder() {}

void MyEncoder::Update(int *value){
    #ifdef Encoder_h_
        UpdateISR(value);
        return;
    #endif
    // Has to use this crap until we switch pins to the interrupt

    // Read the current state of the encoder pins
    uint8_t encA = digitalRead(pinA);
    uint8_t encB = digitalRead(pinB);

    // Combine the states into a single byte
    unsigned int enc = (encA | (encB << 1));

    // Determine the direction using state transitions
    switch ((last_Position << 2) | enc) {  // Encode previous and current states into 4 bits
        case 0b0001:  // Transition: 00 -> 01
        case 0b0111:  // Transition: 01 -> 11
        case 0b1110:  // Transition: 11 -> 10
        case 0b1000:  // Transition: 10 -> 00
            (*value)+=1;    // Right rotation
        break;

        case 0b0010:  // Transition: 00 -> 10
        case 0b1011:  // Transition: 10 -> 11
        case 0b1101:  // Transition: 11 -> 01
        case 0b0100:  // Transition: 01 -> 00
            (*value)-=1;  // Left rotation
        break;

        default:
        // Ignore invalid or no-change transitions
        break;
    }
    // Update the last encoder state
    last_Position = enc;
}

void MyEncoder::UpdateISR(int *value){
    #ifdef Encoder_h_
        if (millis() - last_tick < DEBOUNCE)
            return;

        /* GET ENCODER READINGS */
        int current_position = read();
        int increment = (current_position - last_Position);
        Serial.println("Pos: "+String(current_position) + " - Inc: "+String(increment));

        /* CHANGE NOT TRIGGERED */
        if(abs(increment) < RESOLUTION)
            return;

        /* BASED ON THE ENCODER DIRECTION ADD-or-SUBTRACT */
        (increment > 0) ? (*value += 1) : (*value -= 1);

        /* LIMIT VALUE IN RANGE [0, 100] */
        *value = (int)constrain(*value, 0, 100);

        /* UPDATE LAST VALUES */
        last_Position = current_position;
        last_tick = millis();
    #endif
}