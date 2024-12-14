#include "MyWeight.h"

MyWeight::MyWeight(unsigned int dout, unsigned int sck){
    this->dout = dout;
    this->sck = sck;
}

MyWeight::~MyWeight(){}

void MyWeight::Init(){
    // Scale begin
    myScale.begin(dout, sck);
    // Scale calibrate
    Calibrate();
}

void MyWeight::Calibrate(){
    //TODO
    // dummy hardcode test value
    myScale.set_scale(0.802);

    // Set offset on init
    offset = myScale.get_value(5);
}

int MyWeight::Measure(){
    /* Get fresh reading */
    adc_reading = myScale.get_value(2);

    /* PARSING, CONVERTING, OFFSETING */
    adc_reading = constrain(adc_reading - offset, -1000000, 1200000); // This should limit in range [-1250, 1500]g

    /* TEST IF VALUE HAS CHANGED */
    if(converted != adc_reading / myScale.get_scale()){
        converted = adc_reading / myScale.get_scale();          // This should be grams or whatever
    }

    return converted;
}

void MyWeight::Zero(){
    offset = myScale.get_value(1);
}


void MyWeight::Showcase(){
    /* SET STRING */
    String temp="";
    if(converted/1000 > 1000 || converted/1000 < -1000){    // Below -1000g and Above 1000g
        temp = String(converted/1000000);
        temp+= "Kg";
    }
    else{
        if(converted/1000 < 1 && converted/1000 > -1)       // Prevent -0 displaying, resolution limited to grams only
            temp = "0";
        else
            temp = String(converted/1000);
        temp.remove(temp.length()-3);
        temp+= "g";
    }
}