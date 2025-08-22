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
    glass_brutto = beam->Measure(1);

    /* No glass */
    if(glass_brutto < GLASS_THRESHOLD){
        ResetGlassWeight();
        SetState(GlassState::No_Glass);
        glass_netto = 0;
        return;
    }

    /* New Glass on the strain gauge */
    if(glass_tare == 0){
        SetState(GlassState::Empty);
        return;
    }

    /* Glass weight set */
    /* Update filled milliliters */
    glass_brutto <= 0 ? glass_netto = 0 : glass_netto = glass_brutto - glass_tare;

    /* Glass not full */
    if(GetDifference(volume) > 0){
        SetState(GlassState::Half);
        return;
    }

    /* Glass full */
    SetState(GlassState::Filled);
}