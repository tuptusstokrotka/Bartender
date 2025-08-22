#include "board/Encoder.h"

MyEncoder::MyEncoder(uint8_t pinA, uint8_t pinB) : FastButton(ENC_PIN_SW, HOLD_MS, false), Encoder(pinA, pinB) {
    // Use analog input for button
    ConfigInputType(InputType::analog);     // Using analog-only as input
    ConfigAnalogThreshold(800);             // Trigger from ADC
    ConfigNormallyClosed();                 // Pulled Up

    ConfigWaitForHold(true);                // Do NOT return pressed before release or hold
    ConfigStateRepeat(true, false, false);  // Do NOT repeat pressed and held
}

MyEncoder::~MyEncoder() {}

bool MyEncoder::Update(int32_t &value){
    /* GET ENCODER READINGS */
    int32_t current_position = read();
    int32_t increment = (current_position - last_Position);

    /* CHANGE NOT TRIGGERED */
    if(abs(increment) < RESOLUTION)
        return false;

    /* BASED ON THE ENCODER DIRECTION ADD-or-SUBTRACT */
    (increment > 0) ? (value += 1) : (value -= 1);

    /* LIMIT VALUE IN RANGE */
    value = (int32_t)constrain(value, MIN_VALUE, MAX_VALUE);

    /* UPDATE LAST VALUES */
    if(last_Position != current_position){
        last_Position = current_position;
        return true;
    }
    return false;
}