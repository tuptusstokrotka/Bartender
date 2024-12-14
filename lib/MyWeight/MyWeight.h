#ifndef MYWEIGHT_H
#define MYWEIGHT_H
#pragma once

#include "Arduino.h"
#include "HX711.h"              // weight library

// HX711 PINS
#define LOADCELL_DOUT_PIN 15    // was 2
#define LOADCELL_SCK_PIN  13    // was 3

class MyWeight{
private:
    HX711 myScale;              // HX711 instance
    unsigned int dout;
    unsigned int sck;

    long  adc_reading = 0;      // ADC reading
    float converted   = 0;      // ADC converted by scale factor
    long  offset      = 0;      // Offset when tare

    void Showcase();

public:
    MyWeight(unsigned int dout, unsigned int sck);
    ~MyWeight();

    void Init();

    void Calibrate();

    int Measure();

    void Zero();
};

#endif