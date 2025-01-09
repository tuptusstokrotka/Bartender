#pragma once

struct GlassConfig{
  unsigned int dout;              // Beam dout pin
  unsigned int sck;               // Beam sck pin
  float angle;                    // Servo angle to set
};

#define GLASS_THRESHOLD 10        // Minimal weight to trigger glass