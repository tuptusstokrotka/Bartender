#include "MyOled.h"

MyOled::MyOled() : Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

MyOled::~MyOled() {}

void MyOled::PrintText(String text, uint8_t offset){
    // CENTER TEXT
    int16_t _x, _y;
    uint16_t textWidth, textHeight;
    getTextBounds(text, 0, 0, &_x, &_y, &textWidth, &textHeight);

    // SEND BUFFER STRING TO THE OLED
    int16_t x = (SCREEN_WIDTH  - textWidth)  / 2;
    int16_t y = (SCREEN_HEIGHT - textHeight) / 2 - offset;
    setCursor(x, y);
    print(text);
}
void MyOled::PrintSmallText(String text, uint8_t y_pos){
    // SMALL TEXT
    setTextSize(1);

    // CENTER TEXT
    int16_t _x, _y;
    uint16_t textWidth, textHeight;
    getTextBounds(text, 0, 0, &_x, &_y, &textWidth, &textHeight);

    // SEND BUFFER STRING TO THE OLED
    int16_t x = (SCREEN_WIDTH  - textWidth)  / 2;
    setCursor(x, y_pos);
    print(text);

    // RETURN TO DEFAULT SIZE
    setTextSize(3);
}

void MyOled::Init(){
    if (!begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
        Serial.println(F("Error: init SSD1306 failed."));
        while(1);
    }
    setTextSize(3);
    setTextColor(SSD1306_WHITE);

    // PRINT SPLASH SCREEN AFTER THE INIT
    Splash();
}

void MyOled::Splash(){
    // CLEAR DISPLAY
    clearDisplay();

    // DRAW BITMAP TO THE SCREEN
    drawBitmap(0, 0, splash, SCREEN_WIDTH, SCREEN_HEIGHT, 1);

    // DISPLAY TO SCREEN
    display();

    // LET THEM WATCH YOUR BEAUTIFUL LOGO
    delay(2000);

    // CLEAR DISPLAY
    clearDisplay();
}

void MyOled::Volume(uint8_t volume){
    // CONCAT STRING
    String valueString = String(volume) + " ml";

    // SEND BUFFER STRING TO THE OLED
    // NOTE: experimental
    // ADD LITTLE OFFSET SO THAT PROGRESS BAR CAN FIT INTO THE SCREEN
    PrintText(valueString, 2);
}

void MyOled::Flow(uint8_t volume){
    // CONCAT STRING
    String valueString = String(volume) + " ml/s";

    // SEND BUFFER STRING TO THE OLED
    // NOTE: experimental
    // ADD LITTLE OFFSET SO THAT PROGRESS BAR CAN FIT INTO THE SCREEN
    PrintText(valueString, 2);
}

void MyOled::Seconds(uint8_t volume){
    // CONCAT STRING
    String valueString = String(volume) + " sek";

    // SEND BUFFER STRING TO THE OLED
    // NOTE: experimental
    // ADD LITTLE OFFSET SO THAT PROGRESS BAR CAN FIT INTO THE SCREEN
    PrintText(valueString, 2);
}

void MyOled::ProgressBar(float percentage){
    // DISPLAY FRAME
    drawLine(8,55,120,55,1);        // TOP
    drawLine(8,55,8,62,1);          // RIGHT
    drawLine(8,62,120,62,1);        // BOTTOM
    drawLine(120,55,120,62,1);      // LEFT

    // CALCULATE BAR WIDTH
    int width = map(percentage, 0, 100, 10, 118);

    // DISPLAY BAR
    drawLine(10,57,width,57,1);     // PROGRESS
    drawLine(10,58,width,58,1);     // BAR
    drawLine(10,59,width,59,1);     // 4 PX
    drawLine(10,60,width,60,1);     // TALL

    // DISPLAY TO SCREEN
    display();
}

void MyOled::Complete(){
    // CLEAR DISPLAY
    Clear();

    // SEND BUFFER STRING TO THE OLED
    String valueString = "Gotowe!";
    PrintText(valueString);

    // DISPLAY TO SCREEN
    display();

    // LET THEM WATCH
    delay(1000);
    Clear();
}

void MyOled::PageUpdate(uint8_t volume){
    static uint8_t vol = 0;
    static PAGE last = AUTO;

    if(last != page || vol != volume){
        Clear();
        // DISPLAY TO SCREEN
        last = page;
        vol = volume;
    }
    // SET STRING BASED ON PAGE
    switch(page){
        default:
        case AUTO:
            PrintSmallText("Barman",0);
            Volume(volume);
            break;
        case CALIBRATE_START:
            PrintSmallText("Kalibracja",0);
            Seconds(volume);
            break;
        case CALIBRATE_STOP:
            PrintSmallText("Kalibracja",0);
            Volume(volume);
            break;
    }
    display();
}

void MyOled::Pouring(unsigned long start_time, uint8_t volume){
    /*
    //TODO
    count remining volume to pour from variables
    instead of parsing time as %
    parse volume vs total volume
    update volume remining on the screen - decrement
    */

    // COUNT PROCESS COMPLETION PERCENTAGE
    // TIME WHEN IT WILL STOP POURING
    unsigned long stop_time = start_time + (volume / ML_PER_SECOND * 1000);
    // REMINING TIME TILL STOP
    long a = (stop_time-millis());
    // TOTAL TIME OF POURING
    long b = (stop_time-start_time);
    // CONVERT MS TO %
    float percentage = int(100-float(a)/float(b)*100);

    // DRAW DRINK VOLUME
    Volume(volume);
    // DRAW PROGRESS BAR
    ProgressBar(percentage);
}

void MyOled::Clear(){ clearDisplay(); }

void MyOled::Display(){ display(); }

void MyOled::SwitchPage(){
    switch (page) {
        case AUTO:
            page = CALIBRATE_START;
            break;
        case CALIBRATE_START:
            page = CALIBRATE_STOP;
            break;
        case CALIBRATE_STOP:
            page = AUTO;
            break;
    }
}