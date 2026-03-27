#include "board/Weight.h"

MyWeight::MyWeight(uint8_t dout, uint8_t sck){
    this->dout = dout;
    this->sck = sck;
    // Scale begin
    myScale.begin(dout, sck);
}

MyWeight::~MyWeight(){}

void MyWeight::Calibrate(){
    // dummy hardcode test value
    myScale.set_scale(0.802);

    // Set offset on init
    offset = myScale.get_value(5);
}

void MyWeight::SetOffset(){
    offset = myScale.get_value(5);
}

void MyWeight::SetFactor(float factor){
    // if(factor > 2.0f)   //CHECK OUT OF RANGE
    //     factor = 0.802f; // if factor is out of range
    myScale.set_scale(factor);
}

long MyWeight::Measure(uint8_t samples){
    if(myScale.is_ready()){
        /* Get fresh reading */
        adc_reading = myScale.get_value(samples);

        /* PARSING, CONVERTING, OFFSETING */
        adc_reading = constrain(adc_reading - offset, -1000000, 1200000); // This should limit in range [-1250, 1500]g

        /* UPDATE VALUE */
        converted = adc_reading / myScale.get_scale();          // This should be grams or whatever
    }

    /* Converted to grams */
    return long(converted / 1000);
}

void MyWeight::Zero(){
    offset = myScale.get_value(1);
}

long MyWeight::GetRawReading(uint8_t samples) {
    return myScale.get_value(samples);
}

float MyWeight::CalculateScaleFactor(long raw_empty, long raw_weight, long known_weight) {
    float scale_factor = 0;
    long raw_delta = raw_weight - raw_empty;
    if (raw_delta > 0) {
        scale_factor = (known_weight * 1000.0) / raw_delta; // Convert to mg for accuracy
        myScale.set_scale(scale_factor);
    }
    return scale_factor;
}