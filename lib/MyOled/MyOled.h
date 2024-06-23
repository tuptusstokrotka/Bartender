#ifndef MYOLED_H
#define MYOLED_H
#pragma once

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 		128
#define SCREEN_HEIGHT 		64
#define OLED_RESET 			A0
#define OLED_I2C_ADDRESS 	0x3C

// #define ML_PER_SECOND 		(44.5 / 5.0)	// THIS HAS TO BE IN BRACES!
#include "globals.h"

#define SSD1306_NO_SPLASH	// THIS IS NOT WORKING, has to change inside display class - f*ck it

enum PAGE{
	AUTO = 0,				// AUTOMATIC
	CALIBRATE_START,		// SERVICE MODE - CALIBRATE VOLUME
	CALIBRATE_STOP,			// SERVICE MODE - CALIBRATE VOLUME
};

// SPLASH SCREEN IMAGE DEFINITION
const uint8_t PROGMEM splash[] = {
	0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x06, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x20, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x07, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x60, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x07, 0x8f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0xe0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x07, 0xcf, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf1, 0xe0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfc, 0x00, 0x00, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x0f, 0xf0, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf8, 0xff, 0xff, 0x3f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf3, 0xff, 0xff, 0x8f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe7, 0xff, 0xff, 0xe7, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xcf, 0xff, 0xff, 0xf3, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x9f, 0xff, 0xff, 0xf9, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xbf, 0xff, 0xff, 0xfd, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x3f, 0xfe, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x0f, 0xf0, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x07, 0xe0, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x07, 0xe0, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x07, 0xe0, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x0f, 0xf0, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x3f, 0xfc, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0xff, 0xff, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf9, 0xff, 0xff, 0x9f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfd, 0xff, 0xff, 0xbf, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x7f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x7f, 0xff, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7f, 0xfe, 0x7f, 0xfe, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x7f, 0xfd, 0x3f, 0xfe, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0xff, 0xbf, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x3f, 0xff, 0xff, 0xf9, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x9f, 0xff, 0xff, 0xf3, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xcf, 0xff, 0xff, 0xe7, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xcf, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xc7, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc3, 0xf7, 0xef, 0xc3, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0x83, 0xf7, 0xef, 0xc1, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x07, 0xcf, 0xff, 0x01, 0xf7, 0xcf, 0x80, 0xff, 0xf3, 0xe0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x07, 0x8f, 0xfe, 0x00, 0xc1, 0x83, 0x00, 0x7f, 0xe1, 0xe0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x07, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x60, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x06, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x60, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00
};

class MyOled : private Adafruit_SSD1306 {
private:
	/**
	 * @brief Helper function. Based on the string length draws string centered on a screen.
	 * @note Overflow possible
	 * @param text String to display
	 * @param offset Vertical offset to move string UP
	 */
	void PrintText(String text, uint8_t offset=0);

	/**
	 * @brief Same as PrintText but small font
	 * @param text String to display
	 * @param offset Vertical offset to move string UP
	 */
	void PrintSmallText(String text, uint8_t y_pos);
public:
	PAGE page = AUTO;

    MyOled();
    ~MyOled();

	/**
	 * @brief Initialize SSD1306. Set text size and color.
	 */
    void Init();

	/**
	 * @brief Draw splash screen defined in array
	 * @note Clears and Displays OLED
	 */
    void Splash();

	/**
	 * @brief Draw a number + ml on a screen
	 * @note Displays OLED
	 * @param volume Current volume desired to pour in milliliters
	 */
    void Volume(uint8_t volume);
	/**
	 * @brief Draw a number + ml/s on a screen
	 * @note Displays OLED
	 * @param volume Current flowrate desired to pour in milliliters per second
	 */
    void Flow(uint8_t volume);

	void Seconds(uint8_t volume);
	// Drawing Pouring progress bar
	/**
	 * @brief Draw a frame and progress bar of pouring process
	 * @param percentage Pouring process completion %
	 */
    void ProgressBar(float percentage);

	/**
	 * @brief Draw SUCCESS text on a screen
	 * @note Clears and Displays OLED
	 */
    void Complete();

	/**
	 * @brief Draws page text on top of a screen
	 * @note Displays OLED
	 */
    void PageUpdate(uint8_t volume);

	// Combine All screen elements
	/**
	 * @brief Print all element required while pouring.
	 * Volume in milliliters to pour. Progress bar per glass.
	 * Progress bar will smoothly update if handled correctly
	 *
	 * @param start_time Start pouring time to measure completion %
	 * @param volume Current volume desired to pour in milliliters
	 *
	 * @note Displays OLED
	 */
    void Pouring(unsigned long start_time, uint8_t volume);

    /**
     * @brief Clears display
     */
    void Clear();

	/**
     * @brief Display buffer on display
     */
    void Display();

	void SwitchPage();
};

#endif