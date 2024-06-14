#include "MyGlass.h"

MyGlass::MyGlass(uint8_t pin){
  MyGlass::pin = pin;
  pinMode(pin, INPUT_PULLUP);
}

MyGlass::~MyGlass(){}

uint8_t MyGlass::State(uint8_t volume){
  bool state = digitalRead(MyGlass::pin);         // Check if glass still on the button

  /* NO GLASS ON THE BUTTON */
  if(state == HIGH)
    filled_ml = 0;                                // CLEAR POURED VOLUME
    return NO_GLASS;                              // NO glass

  /* GLASS ON THE BUTTON */
  if(filled_ml == 0){                             // HAS NOT BEEN FILLED
    return EMPTY;                                 // NEW glass = No LIQUID
  }
  else if(filled_ml < volume){                    // FILLED LESS THAN CURRENT VOLUME
    return HALF;                                  // OLD glass half full
  }
  else{                                           // GLASS FULL OR HAVE MORE THAN CURRENT VOLUME
    return FILLED;                                // OLD glass full
  }
}

void MyGlass::Fill(uint8_t volume){
  filled_ml += volume;
}

uint8_t MyGlass::Difference(uint8_t volume){
    if(volume < filled_ml)                        // JUST IN CASE...
      return 0;
    return (volume - filled_ml);
}