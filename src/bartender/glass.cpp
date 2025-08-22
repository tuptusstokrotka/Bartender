#include "glass.h"

MyGlass::MyGlass(const GlassConfig &config){
    this->beam      = new MyWeight(config.dout, config.sck);
    this->angle     = config.angle;
    this->led_index = config.led_index;
}
MyGlass::~MyGlass(){}

void MyGlass::SetGlassWeight(void){
    /* Do not overwrite */
    if(glass_tare != 0)
        return;
    /* Reading value */
    glass_tare = glass_brutto;
}

long MyGlass::GetDifference(long volume){
    return (volume < glass_netto) ? 0 : (volume - glass_netto);
}

void MyGlass::Calibrate(void){
    beam->Calibrate();
    // beam->Calibrate(step); // Advanced calibration - WIP
}

void MyGlass::StatusCheck(unsigned int volume){
    glass_brutto = beam->Measure(1);                    // Measure total weight

    StatusDEBUG(glass_brutto);

    /* No glass */
    if(glass_brutto < GLASS_THRESHOLD){
        ResetGlassWeight();                             // Reset glass weight
        SetState(GlassState::No_Glass);                 // Reset glass state
        glass_netto = 0;                                // Reset glass filled milliliters
        return;
    }

    /* New Glass on the strain gauge */
    if(glass_tare == 0){
        SetState(GlassState::Empty);                    // Set glass state
        return;
    }

    /* Glass weight set */
    /* Update filled milliliters */
    glass_brutto <= 0 ? glass_netto = 0 : glass_netto = glass_brutto - glass_tare;

    /* Glass not full */
    if(GetDifference(volume) > 0){
        SetState(GlassState::Half);                     // Set glass state
        return;
    }

    /* Glass full */
    SetState(GlassState::Filled);                       // Set glass state
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