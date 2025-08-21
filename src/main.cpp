#include "pin_config.h"
#include "bartender/bartender.h"

Bartender* ShotDoser = nullptr;

int freeMemory() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup() {
  /* SERIAL COM INIT */
  // Serial.begin(115200);
  // Serial.println("hello world");
  // Serial.print("Initial Free RAM: "); Serial.println(freeMemory()); //DEBUG

  ShotDoser = new Bartender();

  ShotDoser->AddGlass(KZ4_DOUT, KZ4_SCK, KZ4_ANGLE);
  ShotDoser->AddGlass(KZ5_DOUT, KZ5_SCK, KZ5_ANGLE);
  ShotDoser->AddGlass(KZ2_DOUT, KZ2_SCK, KZ2_ANGLE);
  ShotDoser->AddGlass(KZ1_DOUT, KZ1_SCK, KZ1_ANGLE);
  ShotDoser->AddGlass(KZ6_DOUT, KZ6_SCK, KZ6_ANGLE);
  ShotDoser->AddGlass(KZ3_DOUT, KZ3_SCK, KZ3_ANGLE);

  ShotDoser->Init();

  // Serial.print("Free RAM: "); Serial.println(freeMemory()); //DEBUG
}
void loop() {
  ShotDoser->Update();
  // Serial.print("Free RAM: "); Serial.println(freeMemory()); //DEBUG
}
