#ifndef MYWEIGHT_H
#define MYWEIGHT_H
#pragma once

#include "Arduino.h"
#include "HX711.h"              // weight library

class MyWeight{
private:
    HX711 myScale;              // HX711 instance
    unsigned int dout;
    unsigned int sck;

    long  adc_reading = 0;      // ADC reading
    float converted   = 0;      // ADC converted by scale factor
    long  offset      = 0;      // Offset when tare

public:
    MyWeight(unsigned int dout, unsigned int sck);
    ~MyWeight();

    void Calibrate();                   // Fixed scale calibration - offset only
    void Calibrate(unsigned int step);  // Advanced calibration with known weight

    long Measure(unsigned int samples = 1);

    void Zero();
};

#endif