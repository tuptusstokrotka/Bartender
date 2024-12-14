#include "globals.h"
#include "GlassConfig.h"
#include "Bartender.h"

GlassConfig GLASS_PINS[GLASSES] = {
  {MICROSWITCH_2_PIN_COM, 1, 2, 0},
  {MICROSWITCH_3_PIN_COM, 1, 2, 30},
  {MICROSWITCH_4_PIN_COM, 1, 2, 60},
  {MICROSWITCH_5_PIN_COM, 1, 2, 90},
  {MICROSWITCH_6_PIN_COM, 1, 2, 120},
  {MICROSWITCH_7_PIN_COM, 1, 2, 150}
};

Bartender ShotDoser(GLASSES, GLASS_PINS);

void setup() {
  /* SERIAL COM INIT */
  Serial.begin(115200);
  // /* Get data from EEPROM */
  // unsigned int volume = EEPROM_Get();
  // /* Set init volume */
  // ShotDoser.SetVolume(volume);
}

void loop() {
  /* Update bartender */
  // ShotDoser.Update();
  // delay(10);

  ShotDoser.Test();
}