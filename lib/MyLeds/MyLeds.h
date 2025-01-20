#ifndef MYLEDS_H
#define MYLEDS_H
#pragma once

#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

#define LEDS_PIN A1
#define LEDS_TYPE (NEO_GRB + NEO_KHZ800)

enum COLOR {
    _white   = ((uint32_t)0xff << 16) | ((uint32_t)0xff << 8) | 0xff,
    _black   = ((uint32_t)0x00 << 16) | ((uint32_t)0x00 << 8) | 0x00,

    _red     = ((uint32_t)0xff << 16) | ((uint32_t)0x00 << 8) | 0x00, // Full red
    _green   = ((uint32_t)0x00 << 16) | ((uint32_t)0xff << 8) | 0x00, // Full green
    _blue    = ((uint32_t)0x00 << 16) | ((uint32_t)0x00 << 8) | 0xff, // Full blue

    _yellow  = ((uint32_t)0xff << 16) | ((uint32_t)0xff << 8) | 0x00, // Red + Green
    _cyan    = ((uint32_t)0x00 << 16) | ((uint32_t)0xff << 8) | 0xff, // Green + Blue
    _magenta = ((uint32_t)0xff << 16) | ((uint32_t)0x00 << 8) | 0xff, // Red + Blue

    _orange  = ((uint32_t)0xff << 16) | ((uint32_t)0x78 << 8) | 0x00, // Orange
};

class MyLeds : public Adafruit_NeoPixel{
private:
    unsigned int pixels;
public:
    MyLeds(unsigned int pixels);
    ~MyLeds();

    void ResetAll(void);

    void ResetGlass(unsigned int num);

    /**
     * @brief Set the pixel static color
     * @param num pixel number
     * @param color pixel color
     */
    void SetGlass(unsigned int num, uint32_t color);
    /**
     * @brief Set the pixel as gradient based on percent (red -> green)
     * @param num pixel number
     * @param percent gradient percent (0-100%)
     */
    void SetGlassPercent(unsigned int num, int percent);
};

#endif