#include "MyGlass.h"
#include "MyEncoder.h"
#include "MyOled.h"
#include "MyPump.h"
#include "MyServo.h"

#define GLASSES 7

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

void setup() {
  Serial.begin(9600);
  myDisplay.Init();
  myDisplay.Splash();
}

void loop() {
  /* CHECK ENCODER ROTATOR */
  myEncoder.Update();
  /* SET GLOBAL VOLUME */
  uint8_t volume = myEncoder.GetVolume();

  /* PRINT VOLUME */
  myDisplay.Clear();
  myDisplay.Volume(volume);

  /* CHECK STATES - NEW GLASSES */
  for(uint8_t i=0; i<GLASSES; i++){
    /* SETUP LEDS */
    switch( myGlasses[i].State(volume) ){
      case EMPTY:{
        /* SET LED TO RED */
        //myLed.Set(i, RED);
        break;
      }
      case HALF:{
        /* SET LED TO YELLOW */
        //myLed.Set(i, YELLOW);
        break;
      }
      case FILLED:{
        /* SET LED TO GREEN */
        //myLed.Set(i, GREEN);
        break;
      }
      case NO_GLASS:
      default:{
        /* SET LED TO WHITE */
        //myLed.Set(i, WHITE);
        break;
      }
    }
  }

  /* CHECK ENCODER BUTTON */
  switch(myEncoder.Pressed()){
    /* ENCODER PRESSED */
    case PRESS:{
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
            /* SET LED TO ORANGE */           //TODO
            //myLed.Set(i, ORANGE);
            while (myPump.Working()){
              /* TURN ON ANIMATION */
              myDisplay.Pouring(progress_timer, pour_ml);
            }
            /* STOP PUMPING */
            myPump.Stop();
            delay(200);

            /* SET LED TO GREEN */
            //myLed.Set(i, GREEN);            //TODO
            break;
          }
          case FILLED:
          case NO_GLASS:
            break;
        }
      }
      myServo.Back();
      myDisplay.Complete();
      break;
    }
    /* ENCODER HOLD */
    case HOLD:{
      Serial.println("HOLD");
      /* Do something... */
      break;
    }
    /* ENCODER RELEASED */
    default:
    case RELEASED:
      break;
  }

  delay(10);
}