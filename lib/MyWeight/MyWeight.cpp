#include "MyWeight.h"

MyWeight::MyWeight(unsigned int dout, unsigned int sck){
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
void MyWeight::Calibrate(unsigned int step){
    switch(step){
        case 0:{ // Get 1st raw reading
            // remove weight //
            // get raw reading for no weight
            myScale.get_value(5);
            break;
        }
        case 1:{ // Get 2nd raw reading
            // place known weight //
            // get raw reading for known weight
            myScale.get_value(5);
            break;
        }
        case 2:{ // Calculate scale factor
            // (float) scale_factor = (expected delta) / (raw delta)
            // (float) scale_factor = 100 * 1000 / (2nd_raw - 1st_raw); // x1000 cause the adc returns miligrams
            myScale.set_scale(12345);
            break;
        }
        case 3:{ // Tare the weight
            // remove weight //
            offset = myScale.get_value(5);
            break;
        }
    }
}

long MyWeight::Measure(unsigned int samples){
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