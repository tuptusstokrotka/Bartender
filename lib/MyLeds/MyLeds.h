#ifndef MYLEDS_H
#define MYLEDS_H
#pragma once

#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "globals.h"

#define LEDS_PIN 4
#define NUMPIXELS GLASSES
#define LEDS_TYPE (NEO_GRB + NEO_KHZ800)

enum COLOR {
    // uint32_t = WHITE, RED, GREEN, BLUE
    _red     = 0x0f00, // Only the red component is fully on
    _green   = 0x00f0, // Only the green component is fully on
    _blue    = 0x000f, // Only the blue component is fully on
    _yellow  = 0x0ff0, // Combination of red and green
    _cyan    = 0x00ff, // Combination of green and blue
    _magenta = 0x0f0f, // Combination of red and blue
    _orange  = 0x0f70, // Combination of red and a bit of green
    _white   = 0x0fff, // Combination of red, green, and blue
};

class MyLeds : public Adafruit_NeoPixel{
private:

public:
    MyLeds();
    ~MyLeds();

    void SetGlass(uint8_t n, uint32_t c);
};

#endif