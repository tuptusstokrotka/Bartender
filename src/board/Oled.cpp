#include "board/Oled.h"

// Initialize U8x8 for the SSD1306 128x64 display using software I2C
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

void DisplayInit(){
    u8x8.begin();
}
void DisplayClear(){
    u8x8.clearDisplay();
}

void DisplayClearLine(unsigned int line){
    u8x8.clearLine(line);
}

unsigned int CenterText(const char* string, unsigned int font_width) {
    // Get string length excluding null terminator
    unsigned int used_space = strlen(string);

    // 16px font is made out of 2 x 8px
    if(font_width == 16)
        used_space *= 2;

    // Calculate remaining space on either side
    unsigned int free_space = (SCREEN_WIDTH / 8) - used_space;

    // Return centered X
    return free_space / 2;
}

void DrawText(const char* string, unsigned int line, bool clear) {
    /* Set 8px font */
    u8x8.setFont(u8x8_font_chroma48medium8_r);

    /* Clear line before drawing */
    if(clear){
        u8x8.clearLine(line);
    }

    unsigned int x = CenterText(string);                // Get the centered x position
    u8x8.drawString(x, line, string);                   // Draw the string at the calculated x and given y
}

void DrawBigText(const char* string, unsigned int line, bool clear) {
    /* Set 16px font */
    u8x8.setFont(u8x8_font_px437wyse700a_2x2_r);

    /* Clear double line before drawing */
    if(clear){
        u8x8.clearLine(line);
        u8x8.clearLine(line+1);
    }

    unsigned int x = CenterText(string, 16);            // Get the centered x position
    u8x8.drawString(x, line, string);                   // Draw the string at the calculated x and given y
}

void DrawProgressBar(unsigned int percent) {
    unsigned int blocks = map(percent, 0, 100, 0, 10);  // Scale to 16 characters
    blocks = constrain(blocks,0,10);                    // Limit progress bar to 0-100%

    char progressBar[] = "[          ]";                // Empty bar
    for (unsigned int i = 0; i < blocks; ++i) {
        progressBar[1+i] = '#';                         // Fill with progress
    }

    DrawText(progressBar, 5, false);
}

void DisplayVolume(long volume){
    static long last_value = 0;

    int last_digits = last_value > 0 ? (int)log10(last_value) + 1 : 1;
    int current_digits = volume > 0 ? (int)log10(volume) + 1 : 1;

    String milliliters = String(volume) + " ml";

    if(last_digits != current_digits)
        // Clear screen
        DrawBigText(milliliters.c_str(), 3);
    else
        // Overwrite screen
        DrawBigText(milliliters.c_str(), 3, false);

    last_value = volume;
}

void DrawGlassCounter(int count, unsigned int glasses) {
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
    DrawText(glass, 7, false);
}

void DrawFinished(void){
    char success[] = "SUCCESS";
    DrawText(success, 5);
    delay(1000);
}