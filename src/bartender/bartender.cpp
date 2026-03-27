#include "bartender/bartender.h"

Bartender::Bartender(void){
    this->volume = EEPROM.read(EEPROM_VOLUME);
    volume = constrain(volume, MIN_VALUE, MAX_VALUE);

    DisplayInit();
    DrawText("Bootup", 0);
}
Bartender::~Bartender(void){
    delete led;
}

void Bartender::AddGlass(GlassConfig &config){
    myGlasses.push_back(MyGlass(config));
}
void Bartender::AddGlass(uint8_t dout, uint8_t sck, int angle, int8_t led_index){
    GlassConfig cfg;
    cfg.dout      = dout;
    cfg.sck       = sck;
    cfg.angle     = angle;
    cfg.led_index = (led_index == -1) ? (int8_t)myGlasses.size() : led_index;

    AddGlass(cfg);
}

void Bartender::SetState(BartenderState status){
    this->status = status;
    cur_glass = 0;
    display_state_changed = true;

    switch (status) {
        case BartenderState::idle:
            /* Stop pump */
            if(myPump.IsRunning())
                myPump.Stop();

            /* Reset servo position */
            myServo.MoveTo(0);
            break;
        case BartenderState::serving:
            /* Set glasses weights */
            for(uint8_t i = 0; i < myGlasses.size(); i++) {
                myGlasses[i].SetGlassWeight();
            }
            break;
        case BartenderState::finished:
            if(myPump.IsRunning())
                myPump.Stop();      // This is a MUST in current program flow
            break;
        case BartenderState::calibration:
            cal_step = PREPARE;
            cal_button_pressed = false;
            /* Reset glasses leds */
            for(uint8_t i = 0; i < myGlasses.size(); i++) {
                led->SetGlass(myGlasses[i].GetLedIndex(), _black);
            }
            led->SetGlassPercent(myGlasses[0].GetLedIndex(), 0);
            break;
        default:
            break;
    }
}

void Bartender::SetNextGlass(void){
    /* Stop pump before changing glass */
    if(myPump.IsRunning()){
        myPump.Stop();
        SERVO_PUMP_DELAY //DEBUG
    }

    cur_glass++;
}

void Bartender::Init(void){
    // Get the highest led index and Initialize leds
    int8_t pixels = myGlasses.size();
    led = new MyLeds(pixels);

    // load float from eeprom
    float scale_factor = EEPROM.read(EEPROM_FACTOR);

    // Calibrate
    DrawText("Calibration", 0); // Dummy text to show just after bootup
    Calibrate();
}
void Bartender::Update(void){
    GlassUpdate();
    EncoderUpdate();
    LedUpdate();
    DisplayUpdate();

    switch (status){
        default:
        case BartenderState::idle:{
            /* wait */
            break;
        }
        case BartenderState::serving:{
            ServeDrinks();
            break;
        }
        case BartenderState::calibration:{
            UpdateAdvancedCalibration();
            break;
        }
        case BartenderState::finished:{
            static unsigned long last_update = 0;
            if(last_update == 0){
                last_update = millis();
            }
            /* Return to idle after 1 second */
            if(millis() - last_update > 1000){
                last_update = 0;
                SetState(idle);
            }
            break;
        }
    }
}

void Bartender::GlassUpdate(void){
    UPDATE_INTERVAL(GLASS_REFRESH_INTERVAL);

    uint8_t counter = glass_counter;
    for(uint8_t i = 0; i < myGlasses.size(); i++) {
        myGlasses[i].StatusCheck(volume);
        myGlasses[i].GetState() != No_Glass ? glass_counter |= (1 << i) : glass_counter &= ~(1 << i);
    }

    // Set flag - Do not clear if set
    display_glass_counter |= (counter != glass_counter);
}
void Bartender::EncoderUpdate(void){
    UPDATE_INTERVAL(ENCODER_REFRESH_INTERVAL);

    /* READ ENCODER ROTATION AND SAVE VOLUME */
    if(myEncoder.Update(volume) == true){
        EEPROM.put(EEPROM_VOLUME, volume);
        display_volume_changed = true;
    }

    /* READ ENCODER BUTTON */
    switch(myEncoder.GetState()){
        /* ENCODER PRESSED */
        case ButtonState::pressed:{
            /* Start serving / Abort serving */
            if(GetState() == idle && glass_counter != 0){
                SetState(serving);
            }
            else if(GetState() == calibration){
                cal_button_pressed = true;
            }
            else{
                SetState(idle);
            }
            break;
        }
        /* ENCODER HOLD */
        case ButtonState::held:{
            /* Start calibration */
            SetState(calibration);
            break;
        }
        /* ENCODER RELEASED */
        default:
        case ButtonState::released:
            break;
    }
}
void Bartender::LedUpdate(void){
    // Sync with glass update - otherwise it will reuse old data
    UPDATE_INTERVAL(GLASS_REFRESH_INTERVAL);

    // Skip during calibration - as long as hx711 is reading in blocking way
    if(status == BartenderState::calibration)
        return;

    for(uint8_t i = 0; i < myGlasses.size(); i++) {
        int8_t led_index = myGlasses[i].GetLedIndex();

        switch (myGlasses[i].GetState()){
            case No_Glass:{
                led->SetGlass(led_index, _black);               // Reset glass led
                break;
            }
            case Empty:{
                led->SetGlass(led_index, _white);               // Set glass led WHITE
                break;
            }
            case Half:{
                switch (status){
                    default:
                    case BartenderState::idle:{
                        led->SetGlass(led_index, _orange);      // Set glass led ORANGE
                        break;
                    }
                    case BartenderState::serving:{
                        // Glass not selected to pour
                        if(i != cur_glass){
                            led->SetGlass(led_index, _yellow);  // Set glass led YELLOW
                            break;
                        }

                        /* Get filled percentage - clamp in range [0, 100] */
                        int percent = int((float)myGlasses[i].GetFilled() / (float)volume * 100);
                        percent = constrain(percent, 0, 100);

                        /* Set LED according to the volume - Gradient in range 0-100% */
                        led->SetGlassPercent(led_index, percent);
                        break;
                    }
                }
                break;
            }
            case Filled:{
                led->SetGlass(led_index, _green);               // Set glass led GREEN
                break;
            }
        }
    }
}
void Bartender::DisplayUpdate(void){
    UPDATE_INTERVAL(DISPLAY_REFRESH_INTERVAL);

    if(display_state_changed){
        switch (status) {
            case BartenderState::idle:
                DrawText("Bartender", 0);
                DrawVolume(volume);
                DisplayClearLine(5); // Clear progress bar
                break;
            case BartenderState::serving:
                DrawText("Pouring", 0);
                break;
            case BartenderState::calibration:
                DrawText("Calibration", 0);
                /* Clear middle data */
                // DisplayClearLine(3); //CHECK
                // DisplayClearLine(4); //CHECK
                DisplayClearLine(5);
                break;
            case BartenderState::finished:
                DrawFinished();
                break;
            default:
                break;
        }
        display_state_changed = false;
    }

    if(display_glass_counter){
        DrawGlassCounter(glass_counter, myGlasses.size());
        display_glass_counter = false;
    }

    if(display_volume_changed){
        DrawVolume(volume);
        display_volume_changed = false;
    }

    if(status != BartenderState::serving)
        return;

    // Progress Bar
    static int last_percent = 0;
    int percent = int((float)myGlasses[cur_glass].GetFilled() / (float)volume * 100);
    percent = constrain(percent, 0, 100);

    if(last_percent != percent)
        DrawProgressBar((uint8_t)percent);
    last_percent = percent;
}

void Bartender::ServeDrinks(void){
    /* Finished or No Glass */
    if(cur_glass >= myGlasses.size() || glass_counter == 0){
        SetState(finished);
        return;
    }

    /* Skip glass with unset weight */
    if(myGlasses[cur_glass].GetGlassWeight() == 0){
        SetNextGlass();
        return;
    }

    switch (myGlasses[cur_glass].GetState()){
        /* Skip glass */
        case No_Glass:{
            SetNextGlass();
            break;
        }
        /* Fill glass */
        case Empty:
        case Half:{
            int angle = myGlasses[cur_glass].GetAngle();
            if(myServo.CheckIfSet(angle) == false){
                /* Set servo position */
                myServo.MoveTo(angle);
            }
            else if(!myPump.IsRunning()){
                SERVO_PUMP_DELAY //DEBUG
                /* Start pouring */
                myPump.Start();
            }

            if(myGlasses[cur_glass].GetFilled() >= volume - STOP_ML_OFFSET){
                myGlasses[cur_glass].SetState(GlassState::Filled);
            }
            break;
        }
        /* Finished pouring */
        case Filled:{
            SetNextGlass();
            break;
        }
    }
}
void Bartender::Calibrate(void){
    // Must set the led here - as hx711 is reading in blocking way
    for(auto &glass : myGlasses){
        int8_t led_index = glass.GetLedIndex();
        led->SetGlass(led_index, _yellow);

        glass.SetOffset();
        glass.SetFactor(scale_factor); // 2.15

        delay(100);

        led->SetGlass(led_index, _black);
    }
    SetState(idle);
}

void Bartender::UpdateAdvancedCalibration(void){
    static long cal_raw_empty = 0;
    static long cal_raw_weight = 0;
    static long cal_grams = 0;

    // Check for button press
    if(!cal_button_pressed)
        return;

    cal_button_pressed = false;

    switch(cal_step){
        case PREPARE:{
            DrawText("EMPTY BEAM", 5);
            led->SetGlassPercent(myGlasses[0].GetLedIndex(), 25);
            break;
        }
        case EMPTY_BEAM:{
            // Get reading with no weight
            cal_raw_empty = myGlasses[0].GetRawReading(5);

            cal_step = KNOWN_WEIGHT;
            DrawText("PLACE WEIGHT", 5);
            led->SetGlassPercent(myGlasses[0].GetLedIndex(), 50);
            break;
        }
        case KNOWN_WEIGHT:{
            // Get reading with known weight
            cal_raw_weight = myGlasses[0].GetRawReading(5);

            cal_step = SET_GRAMS;
            DrawText("SET GRAMS", 5);
            led->SetGlassPercent(myGlasses[0].GetLedIndex(), 75);
            break;
        }
        case SET_GRAMS:{
            cal_grams = volume;

            DrawText("CALCULATE", 5);
            cal_step = CALCULATE;
            led->SetGlassPercent(myGlasses[0].GetLedIndex(), 100);
            break;
        }
        case CALCULATE:{
            // Calculate new scale factor
            float factor = myGlasses[0].CalculateScaleFactor(cal_raw_empty, cal_raw_weight, cal_grams);

            // Set factor to all glasses
            for(uint8_t i = 0; i < myGlasses.size(); i++) {
                myGlasses[i].SetFactor(factor);
            }
            EEPROM.put(EEPROM_FACTOR, factor);
            led->SetGlass(myGlasses[0].GetLedIndex(), _black);

            SetState(idle);
            break;
        }
    }
}