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
void DisplayClearLine(unsigned int line);

unsigned int CenterText(const char* string, unsigned int font_width = 8);

void Test();

void DrawText(const char* string, unsigned int line, bool clear = true);
void DrawBigText(const char* string, unsigned int line, bool clear = true);

void DrawProgressBar(unsigned int percent);

void DisplayVolume(long volume);

void DrawGlassCounter(int count, unsigned int glasses);

void DrawFinished(void);

#endif