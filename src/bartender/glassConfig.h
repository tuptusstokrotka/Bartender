#pragma once
#include "Arduino.h"        // uint8_t

struct GlassConfig{
  uint8_t dout;             // Beam dout pin
  uint8_t sck;              // Beam sck pin
  int angle;                // Servo angle to set
  int8_t led_index;         // LED index to set
};

#define GLASS_THRESHOLD 10  // Minimal weight to trigger glass