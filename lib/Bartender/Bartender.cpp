#include "Bartender.h"

Bartender::Bartender(unsigned int glasses, const GlassConfig* config){
    this->glasses = glasses;
    this->volume = EEPROM.read(EEPROM_VOLUME);

    // Allocate an array of pointers to MyGlass
    myGlasses = new MyGlass*[glasses];
    for(unsigned int i = 0; i < glasses; i++){
        myGlasses[i] = new MyGlass(config, i);
    }

    /*
    myGlasses[0] = new MyGlass(MICROSWITCH_2_PIN_COM);
    myGlasses[1] = new MyGlass(MICROSWITCH_3_PIN_COM);
    myGlasses[2] = new MyGlass(MICROSWITCH_4_PIN_COM);
    myGlasses[3] = new MyGlass(MICROSWITCH_5_PIN_COM);
    myGlasses[4] = new MyGlass(MICROSWITCH_6_PIN_COM);
    myGlasses[5] = new MyGlass(MICROSWITCH_7_PIN_COM);
    */

    // SERVO INIT AND SET
    myServo = new MyServo(glasses, config);
    // DISPLAY INIT AND SPLASH
    myDisplay.Init();
}

Bartender::~Bartender(){
    // Clean up the dynamically allocated MyGlass objects
    for (unsigned int i = 0; i < glasses; ++i) {
        delete myGlasses[i];
    }
    delete[] myGlasses;
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
}

void Bartender::GlassesUpdate(){
    for(unsigned int i = 0; i < glasses; i++){
        myGlasses[i]->Check(volume);
    }
}

void Bartender::DisplayUpdate(){

}

void Bartender::EncoderUpdate(){
    /* READ ENCODER ROTATION AND SAVE VOLUME */
    // myEncoder.Update(&volume);

    /* READ ENCODER BUTTON */
    switch(myEncoder.Pressed()){
        /* ENCODER PRESSED */
        case PRESS:{
        if(volume<=0)
            return;

        switch (myDisplay.page){
            case PAGE::AUTO:
            ServeDrinks();
            // EEPROM_Save(EEPROM_VOLUME);
            break;
            case PAGE::CALIBRATE_START:
            case PAGE::CALIBRATE_STOP:
            Calibrate();
            break;
        }
        break;
        }
        /* ENCODER HOLD */
        case HOLD:{
        /* RESET ENCODER VALUE TO 1 */
        myEncoder.SetVolume(1);
        /* CHANGE CURRENT PAGE */
        myDisplay.SwitchPage();
        break;
        }
        /* ENCODER RELEASED */
        default:
        case RELEASED:
        break;
    }
}
