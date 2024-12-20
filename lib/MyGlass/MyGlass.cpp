#include "MyGlass.h"
#define GLAS_DEV     if(glass_index == 2){ Serial.println("TOTAL: "+String(reading)+" - GLASS: "+String(glass_weight)+" - FILLED: "+String(filled_ml)+"/"+String(volume));}

MyGlass::MyGlass(const GlassConfig* config, MyPump* myPump, MyLeds* led, unsigned int index){
    this->glass_index = index;
    this->myPump = myPump;
    this->led = led;

    beam = new MyWeight(config->dout, config->sck);
}

MyGlass::~MyGlass(){}

void MyGlass::SetState(GlassState state){ status = state; }
GlassState MyGlass::GetState(void){ return status; }

void MyGlass::SetGlassWeight(unsigned int grams){ glass_weight = grams; }
unsigned int MyGlass::GetGlassWeight(void){ return glass_weight; }

unsigned int MyGlass::GetFilled(void){
    return filled_ml;
}
unsigned int MyGlass::GetDifference(unsigned int volume){
    return (volume < filled_ml) ? 0 : (volume - filled_ml);
}


void MyGlass::Calibrate(void){
    led->SetGlass(glass_index, _yellow);
    beam->Calibrate();
    led->SetGlass(glass_index, _black);
}

void MyGlass::Fill(unsigned int volume){
    /* Refresh glass reading */
    int reading = beam->Measure(1);
    reading <= 0 ? filled_ml = 0 : filled_ml = reading - glass_weight;

    GLAS_DEV//DEBUG

    /* Glass removed */
    if(reading <= 0){
        SetState(Empty);
        filled_ml = 0;
        // myPump->Stop();
        return;
    }

    /* Set LED according to the volume - Gradient in range 0-100% */
    led->SetGlassPercent(glass_index, int((float)filled_ml / (float)volume * 100) );

    /* Glass full - Stop pouring */
    if(filled_ml >= volume - STOP_ML_OFFSET){
        SetState(Filled);
        // myPump->Stop();
        // Serial.println("-Pump Stop");
        return;
    }

    /* Start pouring */
    // myPump->Start();
    // Serial.println("-Pump Start");
}

void MyGlass::Check(unsigned int volume){
    int reading = beam->Measure(2);                     // Measure total weight
    GLAS_DEV//DEBUG

    /* No glass */
    if(reading < GLASS_THRESHOLD){
        led->SetGlass(glass_index, _black);             // Reset glass led
        SetGlassWeight(0);                              // Reset glass weight
        SetState(GlassState::No_Glass);                 // Reset glass state
        filled_ml = 0;
        return;
    }

    /* Glass on the strain gauge */
    /* New Glass and weight not set */
    if(GetGlassWeight() == 0){
        led->SetGlass(glass_index, _white);             // Set glass led
        SetGlassWeight(reading);                        // Set glass weight
        SetState(GlassState::Empty);                    // Set glass state
        return;
    }

    /* Does not apply to new glass */
    if(filled_ml == 0)
        return;

    /* Glass half full */
    if(GetDifference(volume) > 0){
        led->SetGlass(glass_index, _orange);            // Set glass led
        SetState(GlassState::Half);                     // Set glass state
        return;
    }
    /* Glass full */
    led->SetGlass(glass_index, _green);                 // Set glass led
    SetState(GlassState::Filled);                       // Set glass state
}

//void weight_to_status(int volume, int filled, glass wieght){

//void Check == set led based on the status