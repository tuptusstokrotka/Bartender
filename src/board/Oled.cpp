#include "board/Oled.h"

// Initialize U8x8 for the SSD1306 128x64 display using software I2C
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

void DisplayInit(){
    u8x8.begin();
}
void DisplayClear(){
    u8x8.clearDisplay();
}

void DisplayClearLine(uint8_t line){
    u8x8.clearLine(line);
}

uint8_t CenterText(const char* string, uint8_t font_width) {
    // Get string length excluding null terminator
    uint8_t used_space = strlen(string);

    // 16px font is made out of 2 x 8px
    if(font_width == 16)
        used_space *= 2;

    // Calculate remaining space on either side
    uint8_t free_space = (SCREEN_WIDTH / 8) - used_space;

    // Return centered X
    return free_space / 2;
}

void DrawText(const char* string, uint8_t line, bool clear) {
    /* Set 8px font */
    u8x8.setFont(u8x8_font_chroma48medium8_r);

    /* Clear line before drawing */
    if(clear){
        u8x8.clearLine(line);
    }

    uint8_t x = CenterText(string);                 // Get the centered x position
    u8x8.drawString(x, line, string);               // Draw the string at the calculated x and given y
}

void DrawBigText(const char* string, uint8_t line, bool clear) {
    /* Set 16px font */
    u8x8.setFont(u8x8_font_px437wyse700a_2x2_r);

    /* Clear double line before drawing */
    if(clear){
        u8x8.clearLine(line);
        u8x8.clearLine(line+1);
    }

    uint8_t x = CenterText(string, 16);            // Get the centered x position
    u8x8.drawString(x, line, string);              // Draw the string at the calculated x and given y
}

void DrawProgressBar(uint8_t percent) {
    uint8_t blocks = map(percent, 0, 100, 0, 10);  // Scale to 16 characters
    blocks = constrain(blocks,0,10);               // Limit progress bar to 0-100%

    char progressBar[] = "[          ]";           // Empty bar
    for (uint8_t i = 0; i < blocks; ++i) {
        progressBar[1+i] = '#';                    // Fill with progress
    }

    DrawText(progressBar, 5, false);
}

void DrawVolume(int32_t volume){
    static long last_value = 0; // Monitor the width of the last volume text

    int last_digits = last_value > 0 ? (int)log10(last_value) + 1 : 1;
    int current_digits = volume > 0 ? (int)log10(volume) + 1 : 1;

    String milliliters = String(volume) + " ml";

    bool clear_screen = last_digits != current_digits;
    DrawBigText(milliliters.c_str(), 3, clear_screen);

    last_value = volume;
}

void DrawGlassCounter(uint8_t count, uint8_t glasses) {
    char glass[2 * glasses + 1];  // 2 chars per glass + 1 for the null terminator
    glass[0] = '\0';  // Initialize as an empty string

    for(uint8_t i = 0; i < glasses; i++) {
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
    DrawText(glass, 7, false);
}

void DrawFinished(void){
    char success[] = "SUCCESS";
    DrawText(success, 5);
}