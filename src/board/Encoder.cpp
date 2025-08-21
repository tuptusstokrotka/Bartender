#include "board/Encoder.h"

MyEncoder::MyEncoder(uint8_t pinA, uint8_t pinB) : FastButton(ENC_PIN_SW, HOLD_MS, false), Encoder(pinA, pinB) {
    // Use analog input for button
    ConfigInputType(InputType::analog);     // Using analog onlyinput
    ConfigAnalogThreshold(800);             // Trigger from ADC
    ConfigNormallyClosed();                 // Pulled Up

    ConfigWaitForHold(true);                // Do NOT return pressed before release or hold
    ConfigStateRepeat(true, false, false);  // Do NOT Repeat pressed and held
}

MyEncoder::~MyEncoder() {}

bool MyEncoder::Update(long &value){
    /* GET ENCODER READINGS */
    long current_position = read();
    long increment = (current_position - last_Position);

    /* CHANGE NOT TRIGGERED */
    if(abs(increment) < RESOLUTION)
        return false;

    /* BASED ON THE ENCODER DIRECTION ADD-or-SUBTRACT */
    (increment > 0) ? (value += 1) : (value -= 1);

    /* LIMIT VALUE IN RANGE [0, 100] */
    value = (long)constrain(value, 0, 100);

    /* UPDATE LAST VALUES */
    if(last_Position != current_position){
        last_Position = current_position;
        return true;
    }
    return false;
}