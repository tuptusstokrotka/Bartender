#include "MyOled.h"

// Initialize U8x8 for the SSD1306 128x64 display using software I2C
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
//U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);//CHECK

void DisplayInit(){
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
}

unsigned int CenterText(const char* string) {
    // Width of each character is 8 pixels for this font
    unsigned int char_width = 8;
    // Width of the display is 128 pixels
    unsigned int screen_width = 128 / char_width;
    // Calculate the x position to center the text
    unsigned int x_position = (screen_width - strlen(string)) / 2;

    return x_position;
}

void DisplayClear(){
    u8x8.clearDisplay();
    DrawGlassCounter(0, 6);
}

void DrawText(const char* string, unsigned int line) {
    unsigned int x = CenterText(string);        // Get the centered x position
    u8x8.drawString(x, line, string);           // Draw the string at the calculated x and given y
}

void DrawProgress(unsigned int percent) {
    unsigned int blocks = map(percent, 0, 100, 0, 10);  // Scale to 16 characters
    blocks = constrain(blocks,0,10);                    // Limit progress bar to 0-100%
    char progressBar[] = "[          ]";                // Empty bar

    for (unsigned int i = 0; i < blocks; ++i) {
        progressBar[1+i] = '#';                         // Fill with progress
    }

    unsigned int x = CenterText(progressBar);           // Get the centered x position
    u8x8.drawString(x, 4, progressBar);                 // Show progress bar
}

void DisplayVolume(long volume){
    /* Set bigger font */
    u8x8.setFont(u8x8_font_pcsenior_f);

    /* Display volume */
    String milliliters = "  " + String(volume) + " ml  ";
    DrawText(milliliters.c_str(), 3);

    /* Restore font */
    u8x8.setFont(u8x8_font_chroma48medium8_r);
}

void DrawGlassCounter(int count, unsigned int glasses) {
    static int last_count = 0;

    if(last_count == count)
        return;  // No need to update if the count hasn't changed

    char glass[2 * glasses + 1];  // 2 chars per glass + 1 for the null terminator
    glass[0] = '\0';  // Initialize as an empty string

    for(unsigned int i = 0; i < glasses; i++) {
        // Check if the ith glass is set in the count
        if(count & (1 << i)) {
            // Add the number of the glass followed by a space
            snprintf(glass + strlen(glass), sizeof(glass) - strlen(glass), "%d ", i + 1);
        }
        else {
            // If the glass is not set, add a space
            snprintf(glass + strlen(glass), sizeof(glass) - strlen(glass), "  ");
        }
    }

    // Draw the string with spaces between numbers
    DrawText(glass, 7);

    last_count = count;  // Store the last count to prevent redundant updates
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