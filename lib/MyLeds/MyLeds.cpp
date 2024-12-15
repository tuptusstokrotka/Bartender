#include "MyLeds.h"

MyLeds::MyLeds(unsigned int pixels){
    #if 0
        Adafruit_NeoPixel(NUMPIXELS, LEDS_PIN, LEDS_TYPE);
    #else
        //this should work as a constructor //CHECK
        this->pixels = pixels;
        updateType(LEDS_TYPE);
        updateLength(pixels);
        setPin(LEDS_PIN);
    #endif

    // for(int i=0; i<NUMPIXELS; i++){
    //     setPixelColor(i, _white);
    // }
    // show();
}

MyLeds::~MyLeds(){}

void MyLeds::ResetAll(void){
    fill(_black, 0, pixels);
    show();
}

void MyLeds::ResetGlass(unsigned int num){
    SetGlass(num, _black);
    show();
}

void MyLeds::SetGlass(unsigned int num, unsigned int color){
    setPixelColor(num, color);
    show();
}

void MyLeds::SetGlassPercent(unsigned int num, unsigned int percent){
    // Clamp the percentage to the range [0, 100]
    if (percent > 100) percent = 100;

    // Calculate red and green intensity based on percentage
    uint8_t red   = (255 * (100 - percent)) / 100;  // Red decreases as percent increases
    uint8_t green = (255 * percent) / 100;          // Green increases as percent increases
    uint8_t blue  = 0;                              // Blue stays off

    // Combine the components into a 32-bit color
    uint32_t color = ((uint32_t)red << 16) | ((uint32_t)green << 8) | blue;

    setPixelColor(num, color);
    show();
}