#include "MyWeight.h"

MyWeight::MyWeight(unsigned int dout, unsigned int sck){
    this->dout = dout;
    this->sck = sck;
    // Scale begin
    myScale.begin(dout, sck);
}

MyWeight::~MyWeight(){}

void MyWeight::Calibrate(){
    /*  //TODO
        1. Get 1st raw reading
        // remove weight //
        get raw reading

        2. Get 2nd raw reading
        // place weight //
        place known weight - e.g. 100g
        get raw reading

        3. Calculate scale factor
        (float) scale factor = (expected delta) / (raw delta)
        (float) scale factor = 100 * 1000 / (2nd_raw - 1st_raw); // x1000 cause the adc returns miligrams

        4. Tare the weight
        // remove weight //
        set offset
    */

    // dummy hardcode test value
    myScale.set_scale(0.802);

    // Set offset on init
    offset = myScale.get_value(5);
}

long MyWeight::Measure(unsigned int samples){
    /* Get fresh reading */
    adc_reading = myScale.get_value(samples);

    /* PARSING, CONVERTING, OFFSETING */
    adc_reading = constrain(adc_reading - offset, -1000000, 1200000); // This should limit in range [-1250, 1500]g

    /* TEST IF VALUE HAS CHANGED */
    if(converted != adc_reading / myScale.get_scale()){
        converted = adc_reading / myScale.get_scale();          // This should be grams or whatever
    }

    /* Converted to grams */
    return converted / 1000;
}

void MyWeight::Zero(){
    offset = myScale.get_value(1);
}