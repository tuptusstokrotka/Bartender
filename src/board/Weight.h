#ifndef MYWEIGHT_H
#define MYWEIGHT_H
#pragma once

#include "Arduino.h"
#include "HX711.h"                  // Weight library

class MyWeight{
private:
    HX711 myScale;                  // HX711 instance
    uint8_t dout;
    uint8_t sck;

    long  adc_reading = 0;          // ADC reading
    float converted   = 0;          // ADC converted by scale factor
    long  offset      = 0;          // Offset when tare

public:
    MyWeight(uint8_t dout, uint8_t sck);
    ~MyWeight();

    void Calibrate();              // Fixed scale calibration - offset only
    void SetOffset();
    void SetFactor(float factor);
    float CalculateScaleFactor(long raw_empty, long raw_weight, long known_weight);

    long Measure(uint8_t samples = 1);
    long GetRawReading(uint8_t samples = 5);  // Get raw ADC reading

    void Zero();
};

#endif