#include "MyEncoder.h"

MyEncoder::MyEncoder(unsigned int pin1, unsigned int pin2) : Encoder(pin1, pin2) {
    pinMode(PIN_SW, INPUT_PULLUP);
}

MyEncoder::~MyEncoder() {}

unsigned int MyEncoder::Pressed(){

    /* BUTTON RELEASED */
    if(digitalRead(PIN_SW) == HIGH){                    // BUTTON RELEASED
        last_State = HIGH;                              // UPDATE LASTSTATE
        return RELEASED;
    }

    /* BUTTON PRESSED && LASTSTATE PRESSED */
    if(last_State == LOW){                              // BUTTON HAS NOT BEEN RELEASED YET
        return -1;                                      // SKIP ITERATION
    }

    /* BUTTON PRESSED && LASTSTATE RELEASED */
    last_State = LOW;                                   // BUTTON JUST PRESSED AND HAS BEEN RELEASED

    /* BUTTON HOLD */
    unsigned long holding_Time = millis();              // COUNT HOLDING TIME
    while(digitalRead(PIN_SW) != HIGH){                 // WAIT UNTIL RELEASED
        if(millis() - holding_Time > HOLD_MS)           // BREAK AFTER 400ms
            return HOLD;                                // HOLDING A BUTTON C:
    }
    /* BUTTON PRESS */
    return PRESS;                                       // JUST A PRESS :C
}

unsigned int MyEncoder::Update(unsigned int *volume){
    if (millis() - last_Tick > DEBOUNCE) {
        /* GET ENCODER READINGS */
        int current_position = read();
        int increment = (current_position - last_Position);

        /* CHANGE NOT TRIGGERED */
        if(abs(increment) < RESOLUTION)
            return *volume;

        /* BASED ON THE ENCODER DIRECTION ADD-or-SUBTRACT */
        (increment > 0) ? (*volume + 1) : (*volume - 1);

        /* LIMIT VOLUME IN RANGE [0, 100] */
        *volume = (unsigned int)constrain(*volume, 0, 100);

        /* UPDATE LAST VALUES */
        last_Position = current_position;
        last_Tick = millis();
    }
    return *volume;

    // if (millis() - last_Tick > DEBOUNCE) {
    //     long newPosition = read() / 4;                  // cut int32 into 4 xD
    //     *volume = (unsigned int)constrain(newPosition, 0, 100);        // Ograniczenie volume do maksymalnie 100 ml
    //     last_Tick = millis();                            // Get last encoder tick
    // }
    // return *volume;
}

void MyEncoder::SetVolume(unsigned int volume){
    MyEncoder::volume = volume;
}