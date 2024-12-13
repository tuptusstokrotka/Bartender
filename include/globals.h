#ifndef GLOBALS_H
#define GLOBALS_H

#pragma once

#define MICROSWITCH_1_PIN_COM 4
#define MICROSWITCH_2_PIN_COM 6
#define MICROSWITCH_3_PIN_COM 7
#define MICROSWITCH_4_PIN_COM 8
#define MICROSWITCH_5_PIN_COM 9
#define MICROSWITCH_6_PIN_COM 10
#define MICROSWITCH_7_PIN_COM 11
#define MICROSWITCH_8_PIN_COM 12

#define GLASSES 7

struct GlassConfig{
  unsigned int pin;
  unsigned int dout;
  unsigned int sck;
  float angle;
};

GlassConfig GLASS_PINS[GLASSES] = {
  {MICROSWITCH_2_PIN_COM, 1, 2, 0},
  {MICROSWITCH_3_PIN_COM, 1, 2, 30},
  {MICROSWITCH_4_PIN_COM, 1, 2, 60},
  {MICROSWITCH_5_PIN_COM, 1, 2, 90},
  {MICROSWITCH_6_PIN_COM, 1, 2, 120},
  {MICROSWITCH_7_PIN_COM, 1, 2, 150}
};

#endif