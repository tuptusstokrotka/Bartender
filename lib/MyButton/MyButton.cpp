#include "MyButton.h"

MyButton::MyButton(int pin, int hold){
    pinMode(pin , INPUT);
    this->pinnum = pin;
    this->hold_ms = hold;
}

MyButton::~MyButton(){}


int MyButton::GetState(){
    // due to the analog input only pin, a way around
    int btn_state = analogRead(pinnum);

    /* BUTTON RELEASED */
    if(btn_state > THRESHOLD){                          // BUTTON RELEASED
        if(press_time != 0){
            last_State = PRESS;                         // JUST A PRESS :C
            press_time = 0;
            return PRESS;
        }
        else{
            last_State = RELEASE;
            press_time = 0;
            return RELEASE;
        }
    }
    else{
        /* BUTTON JUST BEEN PRESSED */
        if (last_State == RELEASE) {
            press_time = millis();                      // Save pressing time
            last_State = UNKNOWN;                       // Wait for the state
        }

        /* BUTTON HELD FOR LONG ENOUGH */
        if ((millis() - press_time) > hold_ms && last_State != HOLD) {
            last_State = HOLD;
            press_time = 0;
            return HOLD;
        }
    }
    return UNKNOWN;                                     // STATE NOT READY
}

int MyButton::GetStateRepeat(){
    /* BUTTON RELEASED */
    if(digitalRead(pinnum) == LOW){                     // BUTTON RELEASED
        last_State = RELEASE;
        return RELEASE;
    }

    /* BUTTON PRESSED */
    if (last_State == RELEASE) {                        // Pressed but was released
        press_time = millis();
        last_State = PRESS;                             // JUST A PRESS :C
        return PRESS;
    }
    else if ((millis() - press_time) > hold_ms) {
        last_State = HOLD;
        return HOLD;                                    // REPEAT LAST STATE
    }
    return -1;                                          // DO NOT REPEAT PRESS

    return last_State;                                  // REPEAT LAST STATE
}

int MyButton::GetStateOnRelease(){
    // due to the analog input only pin, a way around
    int btn_state = analogRead(pinnum);

    /* BUTTON RELEASED */
    if(btn_state > THRESHOLD){                          // BUTTON RELEASED
        if(last_State != RELEASE){
            int temp = last_State;
            last_State = RELEASE;
            return temp;
        }
        return RELEASE;
    }

    /* BUTTON PRESSED */
    if (last_State == RELEASE) {                        // Pressed but was released
        press_time = millis();
        last_State = PRESS;                             // JUST A PRESS :C
    }
    else if ((millis() - press_time) > hold_ms) {
        last_State = HOLD;
    }
    return -1;                                          // DO NOT REGISTER KEY
}