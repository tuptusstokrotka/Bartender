#include "MyEncoder.h"

MyEncoder::MyEncoder(uint8_t pin1, uint8_t pin2) : Encoder(pin1, pin2) {
    pinMode(PIN_SW, INPUT_PULLUP);
}

MyEncoder::~MyEncoder() {}

uint8_t MyEncoder::Pressed(){
    /* BUTTON RELEASED */
    if(digitalRead(PIN_SW) == HIGH){                    // BUTTON RELEASED
        lastState = HIGH;                               // UPDATE LASTSTATE
        return RELEASED;
    }

    /* BUTTON PRESSED && LASTSTATE PRESSED */
    if(lastState == LOW){                               // BUTTON HAS NOT BEEN RELEASED YET
        return -1;                                      // SKIP ITERATION //TODO SNAP TO ERROR
    }

    /* BUTTON PRESSED && LASTSTATE RELEASED */
    lastState = LOW;                                    // BUTTON JUST PRESSED AND HAS BEEN RELEASED

    /* BUTTON HOLD */
    unsigned long holding_time = millis();              // COUNT HOLDING TIME
    while(digitalRead(PIN_SW) != HIGH){                 // WAIT UNTIL RELEASED
        if(millis() - holding_time > HOLD_MS)           // BREAK AFTER 400ms
            return HOLD;                                // HOLDING A BUTTON C:
    }

    /* BUTTON PRESS */
    return PRESS;                                       // JUST A PRESS :C
}

void MyEncoder::Update(){
    if (millis() - lastTick > DEBOUNCE) {
        long newPosition = read() / 4;                  // cut int32 into 4 xD
        volume = constrain(newPosition, 0, 100);        // Ograniczenie volume do maksymalnie 100 ml
        lastTick = millis();                            // Get last encoder tick
    }

    //TODO IF HAVE ANY TIME
    // RATHER THAN ASSIGNING VALUE VIA LINEAR FUNCTION
    // INCREMENT VOLUME COUNTER ON CHANGE UP AND DOWN
    // IF READ() (INT32) CHANGES BY SOME VALUE
}

uint8_t MyEncoder::GetVolume(){
    return volume;
}
void MyEncoder::SetVolume(uint8_t volume){
    // BUG THIS WILL NOT WORK DUE TO UPDATE()
    // VOLUME VALUE IS NOT INCREMENTED WITH EACH CHANGE
    // VOLUME IS CALCULATED VIA LINEAR FUNCTION
    // THUS THIS WILL FAIL TO SET VALUE
    MyEncoder::volume = volume;
}