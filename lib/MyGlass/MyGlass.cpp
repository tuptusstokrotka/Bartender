#include "MyGlass.h"

MyGlass::MyGlass(const GlassConfig* config, MyLeds* led, unsigned int index){
    this->pin = config->pin;
    this->glass_index = index;
    this->led = led;

    pinMode(pin, INPUT_PULLUP);
    beam = new MyWeight(config->dout, config->sck);
}

MyGlass::~MyGlass(){}

unsigned int MyGlass::Difference(unsigned int volume){
    if(volume < filled_ml)                              // JUST IN CASE...
        return 0;
    return (volume - filled_ml);
}

void MyGlass::Fill(unsigned int volume){
    switch (status){
        default:
        case No_Glass:
        case Filled:{
            return;
        }
        case Empty:
        case Half:{
            //TODO tare glass weight, offset this value in get_value()
            while(filled_ml < volume){                  // Excpected volume is reached
                filled_ml = beam->Measure();            // Measure glass weight
                /* LED - Gradient in range 0-100% */
                led->SetGlassPercent(glass_index, int(float(filled_ml / volume) * 100) );
                myPump.Start();                         // Start pouring
            }

            myPump.Stop();                              // Stop pouring
            break;
        }
    }
}

void MyGlass::Check(unsigned int volume){
    bool state = digitalRead(MyGlass::pin);             // Check if glass still on the button

    //CHECK If this is not colliding with tare
    int current_weight = beam->Measure();               // Measure glass weight
    Serial.print(String(glass_index));
    (current_weight < 1234) ? Serial.println(": NO_GLASS") : Serial.println(": GLASS");

    /* NO GLASS ON THE BUTTON */
    if(state == HIGH){
        filled_ml = 0;                                  // CLEAR POURED VOLUME
        status = GlassState::No_Glass;                  // NO glass
        led->ResetGlass(glass_index);                   // LED - BLACK
    }
    /* GLASS ON THE BUTTON */
    if(filled_ml == 0){                                 // HAS NOT BEEN FILLED
        status = GlassState::Empty;                     // NEW glass = No LIQUID
        beam->Zero();                                   // Offset glass weight
        led->SetGlass(glass_index, _white);             // LED - WHITE
    }
    else if(filled_ml < volume){                        // FILLED LESS THAN CURRENT VOLUME
        status = GlassState::Half;                      // OLD glass half full
    }
    else{                                               // GLASS FULL OR HAVE MORE THAN CURRENT VOLUME
        status = GlassState::Filled;                    // OLD glass full
    }
}

void MyGlass::Calibrate(void){
    beam->Calibrate();
}

unsigned int MyGlass::GetVolume(void){ return filled_ml; }
