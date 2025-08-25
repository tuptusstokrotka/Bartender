#ifndef MYOLED_H
#define MYOLED_H
#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <u8x8lib.h>

#define SCREEN_WIDTH 		128
#define SCREEN_HEIGHT 		64

void DisplayInit();
void DisplayClear();
void DisplayClearLine(uint8_t line);

uint8_t CenterText(const char* string, uint8_t font_width = 8);

void DrawText(const char* string, uint8_t line, bool clear = true);
void DrawBigText(const char* string, uint8_t line, bool clear = true);

void DrawProgressBar(uint8_t percent);

void DrawVolume(int32_t volume);

void DrawGlassCounter(uint8_t count, uint8_t glasses);

void DrawFinished(void);

#endif