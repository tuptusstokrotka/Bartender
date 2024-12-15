#include "Bartender.h"

Bartender::Bartender(unsigned int glasses, const GlassConfig* config){
    this->glasses = glasses;
    this->volume = EEPROM.read(EEPROM_VOLUME);

    // LED Strip init
    led = new MyLeds(glasses);
    // SERVO INIT AND SET
    myServo = new MyServo(glasses, config);

    // Allocate an array of pointers to MyGlass
    myGlasses = new MyGlass*[glasses];
    for(unsigned int i = 0; i < glasses; i++){
        myGlasses[i] = new MyGlass(config, &myPump, led, i);
    }

    // DISPLAY INIT AND SPLASH
    myDisplay.Init();
}

Bartender::~Bartender(){
    // Clean up the dynamically allocated MyGlass objects
    for (unsigned int i = 0; i < glasses; ++i) {
        delete myGlasses[i];
    }
    delete[] myGlasses;
    delete led;
    delete myServo;
}

BartenderState Bartender::GetState(){ return this->status; }
void Bartender::SetState(BartenderState status){ this->status = status; }

unsigned int Bartender::GetVolume(){ return this->volume; }
void Bartender::SetVolume(unsigned int volume){ this->volume = volume; }


void Bartender::Update(){
    GlassesUpdate();

    EncoderUpdate();
    DisplayUpdate();

    switch (status){
        default:
        case idle:{
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
}

void Bartender::ServeDrinks(){
    for(unsigned int i = 0; i < glasses; i++){
        myServo->MoveTo(i);
        myGlasses[i]->Fill(volume);
    }
    /* return to idle */
    SetState(idle);
}

void Bartender::GlassesUpdate(){
    for(unsigned int i = 0; i < glasses; i++){
        myGlasses[i]->Check(volume);
    }
}

void Bartender::DisplayUpdate(){
    //TODO
}

void Bartender::EncoderUpdate(){
    /* READ ENCODER ROTATION AND SAVE VOLUME */
    myEncoder.Update(&volume);

    /* READ ENCODER BUTTON */
    switch(myEncoder.Pressed()){
        /* ENCODER PRESSED */
        case PRESS:{
            if(volume<=0)
                break;

            /* Start serving */
            SetState(serving);
            break;
        }
        /* ENCODER HOLD */
        case HOLD:{
            /* Start serving */
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

void Bartender::Test(){
    for(unsigned int i = 0; i < glasses; i++){
        led->ResetAll();

        // OLED TEST
        myDisplay.Clear();
        myDisplay.PrintSmallText("GLASS",0);
        myDisplay.PrintText(String(glasses),0);
        delay(500);

        // SERVO TEST
        myServo->MoveTo(i);
        delay(500);

        // PUMP TEST
        myPump.Start(100);

        // SCALE TEST
        myGlasses[i]->Check(0);
        myGlasses[i]->Check(100);
        myGlasses[i]->GetVolume();
        // myGlasses[i]->beam->Measure();
        delay(500);

        // LED TEST - white presence
        led->SetGlass(i, _white);
        delay(500);

        // LED TEST - gradient fill
        for(unsigned int j = 0; j < 100; j++){
            led->SetGlassPercent(i, j);
        }
        delay(500);
    }
}