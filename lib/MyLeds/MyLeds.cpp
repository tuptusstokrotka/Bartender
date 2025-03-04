#include "MyLeds.h"

MyLeds::MyLeds(unsigned int pixels) : Adafruit_NeoPixel(pixels, LEDS_PIN, LEDS_TYPE) {
    begin();            // Initialize the NeoPixel library
    setBrightness(200); // Set maximum brightness
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

void MyLeds::SetGlass(unsigned int num, uint32_t color){
    setPixelColor(num, color);
    show();
}

void MyLeds::SetGlassPercent(unsigned int num, int percent){
    // Calculate red and green intensity based on percentage
    uint8_t red   = (255 * (100 - percent)) / 100;  // Red decreases as percent increases
    uint8_t green = (255 * percent) / 100;          // Green increases as percent increases
    uint8_t blue  = 0;                              // Blue stays off

    // Combine the components into a 32-bit color
    uint32_t color = ((uint32_t)red << 16) | ((uint32_t)green << 8) | blue;

    setPixelColor(num, color);
    show();
}