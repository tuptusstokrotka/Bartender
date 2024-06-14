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

void MyOled::Init(){
    if (!begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
        Serial.println(F("Error: init SSD1306 failed."));
        while(1);
    }
    setTextSize(3);
    setTextColor(SSD1306_WHITE);
}

void MyOled::Splash(){
    // CLEAR DISPLAY
    clearDisplay();

    // DRAW BITMAP TO THE SCREEN
    drawBitmap(0, 0, splash, SCREEN_WIDTH, SCREEN_HEIGHT, 1);

    // DISPLAY TO SCREEN
    display();

    // LET THEM WATCH YOUR BEAUTIFUL LOGO
    // #define MASOCHIST // DO IT - UNCOMMENT THIS LINE
    #ifdef MASOCHIST
        delay(20000);
    #else
        delay(2000);
    #endif
}

void MyOled::Volume(uint8_t volume){
    // CONCAT STRING
    String valueString = String(volume) + " ml";

    // SEND BUFFER STRING TO THE OLED
    // NOTE: experimental
    // ADD LITTLE OFFSET SO THAT PROGRESS BAR CAN FIT INTO THE SCREEN
    PrintText(valueString, 4);

    // DISPLAY TO SCREEN
    display();
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
}

void MyOled::Complete(){
    // CLEAR DISPLAY
    Clear();

    String valueString = "FINISHED!";

    // SEND BUFFER STRING TO THE OLED
    PrintText(valueString);

    // DISPLAY TO SCREEN
    display();

    // LET THEM WATCH
    delay(3000);
}

void MyOled::Pouring(unsigned long start_time, uint8_t volume){
    /*
    //TODO
    count remining volume to pour from variables
    instead of parsing time as %
    parse volume vs total volume
    update volume remining on the screen - decrement
    */

    // CONCAT STRING
    String valueString = String(volume) + " ml";
    // SEND BUFFER STRING TO THE OLED
    PrintText(valueString, 4);

    // COUNT PROCESS COMPLETION PERCENTAGE
    // TIME WHEN IT WILL STOP POURING
    unsigned long stop_time = start_time + (volume / ML_PER_SECOND * 1000);
    // REMINING TIME TILL STOP
    long a = (stop_time-millis());
    // TOTAL TIME OF POURING
    long b = (stop_time-start_time);
    // CONVERT MS TO %
    float percentage = int(100-float(a)/float(b)*100);

    // DRAW PROGRESS BAR
    ProgressBar(percentage);

    // DISPLAY TO SCREEN
    display();
}

void MyOled::Clear(){ clearDisplay(); }

void MyOled::Display(){ display(); }
