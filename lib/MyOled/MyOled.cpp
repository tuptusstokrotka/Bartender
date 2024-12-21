#include "MyOled.h"
#include <U8g2lib.h>

// U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED
// U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C

// U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED
// U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(/* clock=*/ 21, /* data=*/ 20, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather M0 Basic Proto + FeatherWing OLED
U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

#ifdef U8X8_HAVE_HW_SPI
// #include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


void DrawBegin(){
    u8g2.begin();
    u8g2.firstPage();
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0,20,"Hello World!");
}


void DrawText(char* string){
    u8g2.clear();
    u8g2.clearDisplay();

    u8g2.setFont(u8g2_font_ncenB24_tr);
    u8g2.drawStr(0, 0, string);
}
void DrawSmallText(char* string, unsigned int y){
    u8g2.clear();
    u8g2.clearDisplay();

    u8g2.setFont(u8g2_font_ncenB10_tr);
    // y / 10 == 1 ? u8x8.setInverseFont(1) : u8x8.setInverseFont(0);
    u8g2.drawStr(0, y, string);
}

void DrawLine(){
    u8g2.clear();
    u8g2.clearDisplay();

    delay(1000);
    u8g2.drawHLine(0,0,10);
    delay(1000);
    u8g2.drawHLine(0,31,10);
}
void DrawProgress(unsigned int percent){
    u8g2.clear();
    u8g2.clearDisplay();

    u8g2.drawHLine(8,  55, 114);    // TOP
    u8g2.drawVLine(8,  55, 8);      // LEFT

    u8g2.drawHLine(8,  62, 114);    // BOTTOM
    u8g2.drawVLine(120,55, 8);      // RIGHT

    unsigned int width = map(percent, 0,100, 0, 114);
    u8g2.drawHLine(10,  57, width); // PROGRESS
    u8g2.drawHLine(10,  58, width); // BAR
    u8g2.drawHLine(10,  59, width); // 4 PX
    u8g2.drawHLine(10,  60, width); // TALL
}

void DrawBitmap(){
    u8g2.clear();
    u8g2.clearDisplay();

    u8g2.drawBitmap(0,0,1024,32,splash);
}


// MyOled::MyOled() : Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

// MyOled::~MyOled() {}

// void MyOled::Clear(){ clearDisplay(); }
// void MyOled::Display(){ display(); }

// void MyOled::PrintText(String text, uint8_t offset){
//     // CENTER TEXT
//     int16_t _x, _y;
//     uint16_t textWidth, textHeight;
//     getTextBounds(text, 0, 0, &_x, &_y, &textWidth, &textHeight);

//     // SEND BUFFER STRING TO THE OLED
//     int16_t x = (SCREEN_WIDTH  - textWidth)  / 2;
//     int16_t y = (SCREEN_HEIGHT - textHeight) / 2 - offset;
//     setCursor(x, y);
//     print(text);
// }
// void MyOled::PrintSmallText(String text, uint8_t y_pos){
//     // SMALL TEXT
//     setTextSize(1);

//     // CENTER TEXT
//     int16_t _x, _y;
//     uint16_t textWidth, textHeight;
//     getTextBounds(text, 0, 0, &_x, &_y, &textWidth, &textHeight);

//     // SEND BUFFER STRING TO THE OLED
//     int16_t x = (SCREEN_WIDTH  - textWidth)  / 2;
//     setCursor(x, y_pos);
//     print(text);

//     // RETURN TO DEFAULT SIZE
//     setTextSize(3);
// }


// void MyOled::Init(){
//     if (!begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS, true, true)) {
//         Serial.println(F("Error: init SSD1306 failed."));
//         while(1);
//     }
//     setTextSize(3);
//     setTextColor(SSD1306_WHITE);

//     // PRINT SPLASH SCREEN AFTER THE INIT
//     Splash();
// }
// void MyOled::Splash(){
//     // CLEAR DISPLAY
//     clearDisplay();

//     // DRAW BITMAP TO THE SCREEN
//     drawBitmap(0, 0, splash, SCREEN_WIDTH, SCREEN_HEIGHT, 1);

//     // DISPLAY TO SCREEN
//     display();

//     // LET THEM WATCH YOUR BEAUTIFUL LOGO
//     delay(2000);

//     // CLEAR DISPLAY
//     clearDisplay();
// }


// void MyOled::Volume(uint8_t volume){
//     // CONCAT STRING
//     String valueString = String(volume) + " ml";

//     // SEND BUFFER STRING TO THE OLED
//     // NOTE: experimental
//     // ADD LITTLE OFFSET SO THAT PROGRESS BAR CAN FIT INTO THE SCREEN
//     PrintText(valueString, 2);
// }
// void MyOled::ProgressBar(float percentage){
//     // DISPLAY FRAME
//     drawLine(8,  55, 120, 55, 1);   // TOP
//     drawLine(8,  55, 8,   62, 1);   // RIGHT
//     drawLine(8,  62, 120, 62, 1);   // BOTTOM
//     drawLine(120,55, 120, 62, 1);   // LEFT

//     // CALCULATE BAR WIDTH
//     int width = map(percentage, 0, 100, 10, 120); //was 118

//     // DISPLAY BAR
//     drawLine(10,57,width,57,1);     // PROGRESS
//     drawLine(10,58,width,58,1);     // BAR
//     drawLine(10,59,width,59,1);     // 4 PX
//     drawLine(10,60,width,60,1);     // TALL

//     // DISPLAY TO SCREEN
//     display();
// }
// void MyOled::Complete(){
//     // CLEAR DISPLAY
//     Clear();

//     // SEND BUFFER STRING TO THE OLED
//     String valueString = "Gotowe!";
//     PrintText(valueString);

//     // DISPLAY TO SCREEN
//     display();

//     // LET THEM WATCH
//     delay(1000);
//     Clear();
// }

// void MyOled::PageUpdate(uint8_t volume){
//     static uint8_t vol = 0;
//     static PAGE last = AUTO;

//     if(last == page || vol == volume)
//         return;

//     Clear();
//     last = page;
//     vol = volume;

//     // SET STRING BASED ON PAGE
//     switch(page){
//         default:
//         case AUTO:
//             PrintSmallText("Barman",0);
//             Volume(volume);
//             break;
//         case CALIBRATE_START:
//             PrintSmallText("Kalibracja",0);
//             break;
//         case CALIBRATE_STOP:
//             PrintSmallText("Kalibracja 2",0);
//             break;
//     }
//     display();
// }

// void MyOled::Pouring(unsigned int glass, unsigned int volume){
//     // DRAW DRINK VOLUME
//     Volume(volume);
//     // DRAW PROGRESS BAR
//     ProgressBar((glass/volume*100));
// }

// void MyOled::SwitchPage(){
//     switch (page) {
//         case AUTO:
//             page = CALIBRATE_START;
//             break;
//         case CALIBRATE_START:
//             page = CALIBRATE_STOP;
//             break;
//         case CALIBRATE_STOP:
//             page = AUTO;
//             break;
//     }
// }