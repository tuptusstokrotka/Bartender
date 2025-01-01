#include "MyGlass.h"
#define GLAS_DEV if(glass_index == 2){ Serial.println("TOTAL: "+String(glass_reading)+" - GLASS: "+String(glass_weight)+" - FILLED: "+String(filled_ml)+"/"+String(volume));}

MyGlass::MyGlass(const GlassConfig* config, MyPump* myPump, MyLeds* led, unsigned int index){
    this->glass_index = index;
    this->myPump = myPump;
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
unsigned int MyGlass::GetGlassWeight(void){ return glass_weight; }

unsigned int MyGlass::GetFilled(void){
    return glass_filled;
}
unsigned int MyGlass::GetDifference(unsigned int volume){
    return (volume < glass_filled) ? 0 : (volume - glass_filled);
}


void MyGlass::Calibrate(void){
    led->SetGlass(glass_index, _yellow);
    beam->Calibrate();
    led->SetGlass(glass_index, _black);
}

void MyGlass::Fill(unsigned int volume){
    /* Glass full - Stop pouring */
    if(glass_filled >= volume - STOP_ML_OFFSET){
        SetState(Filled);
        Serial.println("fill - filled");
    }
}

void MyGlass::StatusCheck(unsigned int volume){
    glass_reading = beam->Measure(1);                   // Measure total weight
    StatusDEBUG(glass_reading);//DEBUG

    /* No glass */
    if(glass_reading < GLASS_THRESHOLD){
        led->SetGlass(glass_index, _black);             // Reset glass led
        ResetGlassWeight();                             // Reset glass weight
        SetState(GlassState::No_Glass);                 // Reset glass state
        glass_filled = 0;
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
    return; //CHECK maybe ignore it there? so it is set ONLY in  glass::fill

    /* Glass full */
    SetState(GlassState::Filled);                       // Set glass state
    Serial.println("status check - filled");
}

void MyGlass::StatusLED(void){
    switch (GetState()){
        case No_Glass:{
            led->SetGlass(glass_index, _black);         // Reset glass led
            break;
        }
        case Filled:{
            led->SetGlass(glass_index, _green);         // Set glass led
            break;
        }
        case Empty:{
            led->SetGlass(glass_index, _white);         // Reset glass led
            break;
        }
        case Half:{
            led->SetGlass(glass_index, _orange);        // Reset glass led
            break;
        }
    }
}

void MyGlass::StatusDEBUG(int reading){
    Serial.println("GLASS: "+String(glass_index));
    Serial.println("Reading: "+String(reading));
    Serial.println("Glassweight: "+String(GetGlassWeight()));
    Serial.println("FIlled: "+String(glass_filled));
}