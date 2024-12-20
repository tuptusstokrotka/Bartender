#pragma once

struct GlassConfig{
  unsigned int dout;
  unsigned int sck;
  float angle;
};

#define GLASS_THRESHOLD 10        // Minimal weight to trigger glass