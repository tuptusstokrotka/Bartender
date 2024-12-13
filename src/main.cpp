#include "globals.h"
#include "Bartender.h"

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
  ShotDoser.Update();
  // delay(10);

  // ShotDoser.SetVolume(100);
  // ShotDoser.ServeDrinks();
}