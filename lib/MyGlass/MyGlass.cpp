#include "MyGlass.h"

MyGlass::MyGlass(const GlassConfig* config, MyLeds* led, unsigned int index){
    this->glass_index = index;
    this->led = led;

    beam = new MyWeight(config->dout, config->sck);
}
MyGlass::~MyGlass(){}

void MyGlass::SetState(GlassState state){ status = state; }
GlassState MyGlass::GetState(void){ return status; }

void MyGlass::ResetGlassWeight(void){
    glass_weight = 0;
}
void MyGlass::SetGlassWeight(void){
    /* Do not overwrite */
    if(glass_weight != 0)
        return;
    /* Reading value */
    glass_weight = glass_reading;
}
long MyGlass::GetGlassWeight(void){ return glass_weight; }

long MyGlass::GetFilled(void){ return glass_filled; }

long MyGlass::GetDifference(long volume){
    return (volume < glass_filled) ? 0 : (volume - glass_filled);
}


void MyGlass::Calibrate(void){
    led->SetGlass(glass_index, _yellow);
    beam->Calibrate();
    // beam->Calibrate(step); // Advanced calibration - WIP
    led->SetGlass(glass_index, _black);
}

void MyGlass::StatusCheck(unsigned int volume){
    glass_reading = beam->Measure(1);                   // Measure total weight

    StatusDEBUG(glass_reading);

    /* No glass */
    if(glass_reading < GLASS_THRESHOLD){
        led->SetGlass(glass_index, _black);             // Reset glass led
        ResetGlassWeight();                             // Reset glass weight
        SetState(GlassState::No_Glass);                 // Reset glass state
        glass_filled = 0;                               // Reset glass filled milliliters
        return;
    }

    /* New Glass on the strain gauge */
    if(GetGlassWeight() == 0){
        SetState(GlassState::Empty);                    // Set glass state
        return;
    }

    /* Glass weight set */
    /* Update filled milliliters */
    glass_reading <= 0 ? glass_filled = 0 : glass_filled = glass_reading - GetGlassWeight();

    /* Glass not full */
    if(GetDifference(volume) > 0){
        SetState(GlassState::Half);                     // Set glass state
        return;
    }

    /* Glass full */
    SetState(GlassState::Filled);                       // Set glass state
}

void MyGlass::StatusLED(void){
    switch (GetState()){
        case No_Glass:{
            led->SetGlass(glass_index, _black);         // Reset glass led
            break;
        }
        case Empty:{
            led->SetGlass(glass_index, _white);         // Set glass led WHITE
            break;
        }
        case Half:{
            led->SetGlass(glass_index, _orange);        // Set glass led ORANGE
            break;
        }
        case Filled:{
            led->SetGlass(glass_index, _green);         // Set glass led GREEN
            break;
        }
    }
}
void MyGlass::PercentLED(int percent){
    led->SetGlassPercent(glass_index, percent);
}

void MyGlass::StatusDEBUG(int reading){
    // plotter
    #if GLASS_DEBUG_PRINT_ENABLE == 1
        String name = "GLASS_"+String(glass_index)+":";

        GLASS_DEBUG_PRINT(name);
        if(glass_index == 5){ //DEBUG set max glass index
            GLASS_DEBUG_PRINTLN(reading);
        }
        else {
            GLASS_DEBUG_PRINT(reading);
            GLASS_DEBUG_PRINT(",");
        }
    #endif
}