#include "Bartender.h"

Bartender::Bartender(unsigned int glasses, GlassConfig* config){
    this->glasses = glasses;
    this->volume = EEPROM.read(EEPROM_VOLUME);

    // LED Strip init
    led = new MyLeds(glasses);

    // SERVO INIT AND SET
    myServo = new MyServo(glasses, config);

    // Allocate an array of pointers to MyGlass
    myGlasses = new MyGlass*[glasses];
    for(unsigned int i = 0; i < glasses; i++){
        myGlasses[i] = new MyGlass((config + sizeof(*config)/sizeof(GlassConfig) * i), &myPump, led, i);
        myGlasses[i]->Calibrate();
    }

    // DISPLAY INIT
    DisplayInit();

    // Set Bartender to idle - trigger display
    SetState(idle);
}

Bartender::~Bartender(){
    delete led;
    delete myServo;
    // Clean up the dynamically allocated MyGlass objects
    for (unsigned int i = 0; i < glasses; ++i) {
        delete myGlasses[i];
    }
    delete[] myGlasses;
}

BartenderState Bartender::GetState(){ return this->status; }
void Bartender::SetState(BartenderState status){
    if(this->status == serving)
        DisplayClear();

    this->status = status;

    switch (status) {
        case idle:
            DrawText(" Bartender ", 0);
            break;
        case serving:
            for(unsigned int i = 0; i < glasses; i++){
                myGlasses[i]->SetGlassWeight();
            }
            DrawText("  Pouring  ", 0);
            DrawText("Progress", 3);
            break;
        case calibration:
            DrawText("Calibration", 0);
            break;
    }
}

unsigned int Bartender::GetVolume(){ return this->volume; }
void Bartender::SetVolume(unsigned int volume){ this->volume = volume; }


void Bartender::Update(){
    GlassUpdate();
    EncoderUpdate();
    DisplayUpdate();

    switch (status){
        default:
        case idle:{
            // Set LED in idle
            for(unsigned int i = 0; i < glasses; i++)
                myGlasses[i]->StatusLED();
            break;
        }
        case serving:{
            ServeDrinks();
            break;
        }
        case calibration:{
            Calibrate();
            break;
        }
    }
}

void Bartender::GlassUpdate(){
    for(unsigned int i = 0; i < glasses; i++) {
        myGlasses[i]->StatusCheck(volume);
        myGlasses[i]->GetState() != No_Glass ? glass_counter |= (1 << i) : glass_counter &= ~(1 << i);
    }
}

void Bartender::EncoderUpdate(){
    /* READ ENCODER ROTATION AND SAVE VOLUME */
    myEncoder.Update(&volume);

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
            (GetState() == idle && glass_counter != 0) ? SetState(serving) :  SetState(idle);
            break;
        }
        /* ENCODER HOLD */
        case HOLD:{
            /* Start calibration */
            SetState(calibration);

            /* RESET ENCODER VALUE TO 1 */
            // myEncoder.SetVolume(1);
            /* CHANGE CURRENT PAGE */
            // myDisplay.SwitchPage();
            break;
        }
        /* ENCODER RELEASED */
        default:
        case RELEASED:
            break;
    }
}

void Bartender::DisplayUpdate(){
    DrawGlassCounter(glass_counter, glasses);
}

void Bartender::ServeDrinks(){
    /* Current glass iterator */
    static unsigned int i = 0;

    /* Finished or no glass */
    if(i >= glasses || glass_counter == 0){
        i = 0;
        SetState(idle);
        return;
    }

    switch (myGlasses[i]->GetState()){
        /* Skip glass */
        case No_Glass:
        case Filled:{
            // myPump.Stop(); //FIXME
            // Serial.println("-Pump Stop");
            i++;
            break;
        }
        /* Fill glass */
        case Empty:
        case Half:{
            // myServo->MoveTo(i); //TODO move once
            myGlasses[i]->Fill(volume);

            /* Start pouring */
            // myPump->Start(); //FIXME //TODO once
            // Serial.println("-Pump Start");

            int percent = int((float)myGlasses[i]->GetFilled() / (float)volume * 100);
            /* Set LED according to the volume - Gradient in range 0-100% */
            led->SetGlassPercent(i, percent);

            DrawProgress(percent);
            DrawText((String(myGlasses[i]->GetFilled()) + " / " + String(volume) + " ml").c_str(), 5);
            break;
        }
    }
}

void Bartender::Calibrate(){
    for(unsigned int i = 0; i < glasses; i++) {
        myGlasses[i]->Calibrate();
    }
    SetState(idle);
}