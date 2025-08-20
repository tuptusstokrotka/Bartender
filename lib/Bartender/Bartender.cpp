#include "Bartender.h"

Bartender::Bartender(unsigned int glasses, GlassConfig* config){
    this->glasses = glasses;
    this->volume = EEPROM.read(EEPROM_VOLUME);

    // LED Strip init
    led = new MyLeds(glasses);

    // SERVO INIT AND SET
    myServo = new MyServo(glasses, config);

    // DISPLAY INIT
    DisplayInit();
    DrawText("Bootup", 0);

    // Allocate an array of pointers to MyGlass
    myGlasses = new MyGlass*[glasses];
    for(unsigned int i = 0; i < glasses; i++){
        myGlasses[i] = new MyGlass((config + sizeof(*config)/sizeof(GlassConfig) * i), led, i);
        myGlasses[i]->Calibrate();
    }

    // Set Bartender to idle - trigger display
    SetState(idle);
}

Bartender::~Bartender(void){
    delete led;
    delete myServo;
    // Clean up the dynamically allocated MyGlass objects
    for (unsigned int i = 0; i < glasses; ++i) {
        delete myGlasses[i];
    }
    delete[] myGlasses;
}

BartenderState Bartender::GetState(void){ return this->status; }

void Bartender::SetState(BartenderState status){
    this->status = status;

    switch (status) {
        case idle:
            DrawText("Bartender", 0);
            myPump.Stop(); //CHECK if needed
            myServo->MoveTo(0);
            /* Clear progress bar */
            DisplayClearLine(5);
            /* Display volume with state change (when no data has changed) */
            DisplayVolume(volume);
            break;
        case serving:
            DrawText("Pouring", 0);
            /* Set glasses weights */
            for(unsigned int i = 0; i < glasses; i++){
                myGlasses[i]->SetGlassWeight();
            }
            break;
        case calibration:
            DrawText("Calibration", 0);
            /* Clear middle data */
            DisplayClearLine(3);
            DisplayClearLine(4);
            DisplayClearLine(5);
            break;
    }
}

void Bartender::Update(void){
    GlassUpdate();
    EncoderUpdate();
    LedUpdate();
    DisplayUpdate();

    switch (status){
        default:
        case BartenderState::idle:{
            /* do something */
            break;
        }
        case BartenderState::serving:{
            ServeDrinks();
            break;
        }
        case BartenderState::calibration:{
            Calibrate();
            break;
        }
    }
}

void Bartender::GlassUpdate(void){
    //CHECK one by one in each loop
    // static unsigned int index = 0;
    // myGlasses[index]->StatusCheck(volume);
    // myGlasses[index]->GetState() != No_Glass ? glass_counter |= (1 << index) : glass_counter &= ~(1 << index);

    // ++index %= glasses;
    // return;

    //CHECK all glasses per loop
    for(unsigned int i = 0; i < glasses; i++) {
        myGlasses[i]->StatusCheck(volume);
        myGlasses[i]->GetState() != No_Glass ? glass_counter |= (1 << i) : glass_counter &= ~(1 << i);
    }
}

void Bartender::EncoderUpdate(void){
    /* READ ENCODER ROTATION AND SAVE VOLUME */
    myEncoder.Update(volume);

    static int last_vol = volume;
    if(last_vol != volume){
        last_vol = volume;
        EEPROM.write(EEPROM_VOLUME, volume);
    }

    /* READ ENCODER BUTTON */
    switch(myEncoder.GetState()){
        /* ENCODER PRESSED */
        case PRESS:{
            /* Start serving / Abort serving */
            if(GetState() == idle && glass_counter != 0){
                SetState(serving);
            }
            else{
                SetState(idle);
            }
            break;
        }
        /* ENCODER HOLD */
        case HOLD:{
            /* Start calibration */
            SetState(calibration);
            break;
        }
        /* ENCODER RELEASED */
        default:
        case RELEASED:
            break;
    }
}

void Bartender::LedUpdate(void){
    switch (status){
        default:
        case BartenderState::idle:{
            /* Set LEDs in idle */
            for(unsigned int i = 0; i < glasses; i++)
                myGlasses[i]->StatusLED();
            break;
        }
        case BartenderState::serving:{
            /* Skip LED for NO_GLASS and with Weight not set */
            if(myGlasses[cur_glass]->GetState() == GlassState::No_Glass || myGlasses[cur_glass]->GetGlassWeight() == 0)
                break;

            /* Get filled percentage - clamp in range [0, 100] */
            int percent = int((float)myGlasses[cur_glass]->GetFilled() / (float)volume * 100);
            percent = constrain(percent, 0, 100);

            /* Set LED according to the volume - Gradient in range 0-100% */
            myGlasses[cur_glass]->PercentLED(percent);
            break;
        }
        case BartenderState::calibration:{
            /* do nothing */
            break;
        }
    }
}

void Bartender::DisplayUpdate(void){
    /* Glass counter */
    static int last_count = 0;
    if(last_count != glass_counter){
        DrawGlassCounter(glass_counter, glasses);
        last_count = glass_counter;
    }

    switch (status){
        default:
        case BartenderState::serving:{
            /* Progress Bar*/
            static int last_percent = 0;
            int percent = int((float)myGlasses[cur_glass]->GetFilled() / (float)volume * 100);
            /* Clamp the percentage to the range [0, 100] */
            percent = constrain(percent, 0, 100);
            if(last_percent != percent){
                DrawProgressBar(percent);
                last_percent = percent;
            }
            // break; // Do not break here!
        }
        case BartenderState::idle:{
            /* Volume */
            static long last_volume = 0;
            if(last_volume != volume){
                DisplayVolume(volume);
                last_volume = volume;
            }
            break;
        }
        case BartenderState::calibration:{
            break;
        }
    }
}

void Bartender::ServeDrinks(void){
    /* Finished or no glass */
    if(cur_glass >= glasses || glass_counter == 0){
        cur_glass = 0;      // reset counter
        DrawFinished();     // draw Finished string
        SetState(idle);     // return to IDLE
        return;
    }

    /* Skip glass with unset weight */
    if(myGlasses[cur_glass]->GetGlassWeight() == 0){
        cur_glass++;
        return;
    }

    switch (myGlasses[cur_glass]->GetState()){
        /* Skip glass */
        case No_Glass:
        case Filled:{
            myPump.Stop();   //CHECK If this 'Filled' stops the pump or 'Half'
            SERVO_PUMP_DELAY //CHECK wait for the drops to fall - this might be redundant if offset works
            cur_glass++;
            break;
        }
        /* Fill glass */
        case Empty:
        case Half:{
            //TODO This is stupid bypass to pour to glass 0. Need better solution
            if(myServo->CheckIfSet(cur_glass) == false || cur_glass == 0){
                /* Set servo position */
                myServo->MoveTo(cur_glass);
                SERVO_PUMP_DELAY
                /* Start pouring */
                myPump.Start();
            }

            if(myGlasses[cur_glass]->GetFilled() >= volume - STOP_ML_OFFSET){
                //CHECK maybe set glass to filled instead
                /* Stop pouring */
                myPump.Stop();
                SERVO_PUMP_DELAY
            }

            break;
        }
    }
}

void Bartender::Calibrate(void){
    for(unsigned int i = 0; i < glasses; i++) {
        myGlasses[i]->Calibrate();
    }
    SetState(idle);
}
