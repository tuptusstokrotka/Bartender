#include "GlassConfig.h"
#include "Bartender.h"

#define GLASSES 6

// Order based on the location of the strain gauge on the PCB
GlassConfig GLASS_PINS[GLASSES] = {
  {8,  9,  4},    // KZ4
  {11, 10, 38},   // KZ5
  {5,  4,  76},   // KZ2
  {A2, A3, 114},  // KZ1
  {12, 13, 148},  // KZ6
  {7,  6,  212},  // KZ3
};

Bartender* ShotDoser = nullptr;

void setup() {
  /* SERIAL COM INIT */
  // Serial.begin(115200);
  // Serial.println("hello world");

  ShotDoser = new Bartender(GLASSES, GLASS_PINS);
}

void loop() {
  /* Update bartender */
  ShotDoser->Update();
}