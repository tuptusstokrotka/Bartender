#include "MyGlass.h"
#include "MyEncoder.h"
#include "MyOled.h"
#include "MyPump.h"
#include "MyServo.h"
#include "MyLeds.h"

#include "globals.h"
// FLOW RATE
float ML_PER_SECOND = (44.5 / 5.0);
// DRINK VOLUME
uint8_t volume = 0;

// KRAŃCÓWKI - kieliszki
MyGlass myGlasses[GLASSES]={
  (MICROSWITCH_2_PIN_COM),
  (MICROSWITCH_3_PIN_COM),
  (MICROSWITCH_4_PIN_COM),
  (MICROSWITCH_5_PIN_COM),
  (MICROSWITCH_6_PIN_COM),
  (MICROSWITCH_7_PIN_COM),
  (MICROSWITCH_8_PIN_COM)
};
// ENCODER
MyEncoder myEncoder;
// OLED
MyOled myDisplay;
// POMPA
MyPump myPump;
// SERVO
MyServo myServo;
// LEDS
MyLeds myLeds;


void CheckGlasses(){
  /* CHECK STATES - NEW GLASSES */
  for(uint8_t i=0; i<GLASSES; i++){
    /* SETUP LEDS */
    switch( myGlasses[i].State(volume) ){
      case EMPTY:{
        /* SET LED TO RED */
        myLeds.SetGlass(i, _red);
        break;
      }
      case HALF:{
        /* SET LED TO YELLOW */
        myLeds.SetGlass(i, _yellow);
        break;
      }
      case FILLED:{
        /* SET LED TO GREEN */
        myLeds.SetGlass(i, _green);
        break;
      }
      case NO_GLASS:
      default:{
        /* SET LED TO WHITE */
        myLeds.SetGlass(i, _white);
        break;
      }
    }
  }
}
void Calibrate(){
  static uint8_t sekundy=1;
  if(myDisplay.page == CALIBRATE_START){
    /* START PUMPING */
    myPump.Start();
    delay(volume*1000);
    myPump.Stop();
    sekundy=volume;
  }
  else{
    ML_PER_SECOND = volume/sekundy;        // this SHOULD provide milliliters per second rate
    myDisplay.Complete();
  }
  myDisplay.SwitchPage();
}
void ServeDrinks(){
  /* COUNT FILLED GLASSES */
  uint8_t filled =0;
  for(uint8_t i=0; i<GLASSES; i++){
    /* CHECK IF GLASS CAN BE FILLED */
    switch( myGlasses[i].State(volume) ){
      case EMPTY:
      case HALF:{
        myDisplay.Clear();
        /* MOVE SERVO TO GLASS */
        myServo.MoveTo(i);

        /* COUNT REMINDING MILLILITERS */
        uint8_t pour_ml = myGlasses[i].Difference(volume);
        /* ADD MILLILITERS AS FILLED */
        myGlasses[i].Fill(pour_ml);
        /* Start FIlling Timer */
        unsigned long progress_timer = millis();
        /* START PUMPING */
        myPump.Start(pour_ml);
        filled++;
        /* SET LED TO ORANGE */
        myLeds.SetGlass(i, _orange);

        while (myPump.Working()){
          /* TURN ON ANIMATION */
          myDisplay.Pouring(progress_timer, pour_ml);
        }
        // DRAW PROGRESS BAR
        myDisplay.ProgressBar(100);

        /* STOP PUMPING */
        myPump.Stop();
        delay(350);

        /* SET LED TO GREEN */
        //myLeds.setPixelColor(i, GREEN);            //TODO
        break;
      }
      case FILLED:
      case NO_GLASS:
        break;
    }
  }

  /* ANY GLASSED FILLED */
  if(filled!=0){
    myServo.MoveTo(0);
  }

  /* ALWAYS DRAW SUCCESS */
  myDisplay.Complete();
}
void encoderButton(){
  switch(myEncoder.Pressed()){
    /* ENCODER PRESSED */
    case PRESS:{
      if(volume<=0)
        return;
      switch (myDisplay.page){
        case PAGE::AUTO:
          ServeDrinks();
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


void setup() {
  // SERIAL COM INIT
  Serial.begin(9600);
  // SERVO INIT AND SET
  myServo.Init();
  // DISPLAY INIT AND SPLASH
  myDisplay.Init();
}

void loop() {
  /* CHECK ENCODER ROTATION AND SAVE VOLUME */
  myEncoder.Update(&volume);

  /* PRINT OLED PAGE */
  myDisplay.PageUpdate(volume);

  /* CHECK GLASSES STATES -*/
  CheckGlasses();

  /* CHECK ENCODER BUTTON */
  encoderButton();

  delay(10);
}