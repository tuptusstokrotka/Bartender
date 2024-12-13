#ifndef MYWEIGHT_H
#define MYWEIGHT_H
#pragma once

#include "Arduino.h"
#include "HX711.h"              // weight library

// HX711 PINS
#define LOADCELL_DOUT_PIN 15    // was 2
#define LOADCELL_SCK_PIN  13    // was 3

class MyWeight : public HX711{
private:
    HX711 myScale;              // HX711 instance
    unsigned int dout;
    unsigned int sck;

    long  adc_reading = 0;      // ADC reading
    float converted   = 0;      // ADC converted by scale factor
    long  offset      = 0;      // Offset when tare
    bool  refresh     = false;  // True if new data to showcase
    bool  hold        = false;  // True if hold the weight measurement

    void Showcase();

public:
    MyWeight(unsigned int dout, unsigned int sck);
    ~MyWeight();

    void Init();

    void Calibrate();

    void Measure();

    void Zero();

    void Hold();
};

#endif