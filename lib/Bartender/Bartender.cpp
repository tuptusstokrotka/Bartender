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

    // DISPLAY INIT AND SPLASH
    // myDisplay->Init();
    DrawBegin();
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
    this->status = status;
    // myDisplay.SetPage(status);
}

unsigned int Bartender::GetVolume(){ return this->volume; }
void Bartender::SetVolume(unsigned int volume){ this->volume = volume; }


void Bartender::Update(){
    DrawLine();
    EncoderUpdate();
    // DisplayUpdate();

    switch (status){
        default:
        case idle:{
            GlassCheck();
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

void Bartender::Calibrate(){
    for(unsigned int i = 0; i < glasses; i++){
        myGlasses[i]->Calibrate();
    }
    SetState(idle);
}

void Bartender::ServeDrinks(){
    /* Current glass iterator */
    static unsigned int i = 0;

    /* Finished */
    if(i >= glasses){
        i = 0;
        SetState(idle);
    }

    // Serial.print("Glass "+String(i)+": ");
    switch (myGlasses[i]->GetState()){
        /* Skip glass */
        case No_Glass:
        case Filled:{
            i++;
            break;
        }
        /* Fill glass */
        case Empty:
        case Half:{
            // myServo->MoveTo(i); //TODO move once
            myGlasses[i]->Fill(volume);
            // myDisplay.Pouring(myGlasses[i]->GetFilled(), volume);
            break;
        }
    }
}

void Bartender::GlassCheck(){
    for(unsigned int i = 0; i < glasses; i++){
        myGlasses[i]->Check(volume);
    }
}

void Bartender::DisplayUpdate(){
    // myDisplay->PrintText("volume");
    //TODO
}

void Bartender::EncoderUpdate(){
    /* READ ENCODER ROTATION AND SAVE VOLUME */
    myEncoder.Update(&volume);

    static unsigned int last_vol = volume;
    if(last_vol != volume){
        last_vol = volume;
        EEPROM.write(EEPROM_VOLUME, volume);
    }

    /* READ ENCODER BUTTON */
    switch(myEncoder.GetState()){
        /* ENCODER PRESSED */
        case PRESS:{
            /* Start serving, Abort serving */
            GetState() == idle ? SetState(serving) :  SetState(idle);
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