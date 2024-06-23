#include "MyLeds.h"

MyLeds::MyLeds(){
    #if 0
        Adafruit_NeoPixel(NUMPIXELS, LEDS_PIN, LEDS_TYPE);
    #else
        //this should work as a constructor //FIXME
        updateType(LEDS_TYPE);
        updateLength(NUMPIXELS);
        setPin(LEDS_PIN);
    #endif

    for(int i=0; i<NUMPIXELS; i++){
        setPixelColor(i, _white);
    }
    show();
}

MyLeds::~MyLeds(){}

void MyLeds::SetGlass(uint8_t num, uint32_t color){
    setPixelColor(num, color);
    show();
}
