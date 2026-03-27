#include "pin_config.h"
#include "debug.h"
#include "bartender/bartender.h"

Bartender* ShotDoser = nullptr;

void setup() {
  /* SERIAL COM INIT */
  DEBUG_SERIAL;
  DEBUG_PRINTLN("hello world");

  ShotDoser = new Bartender();

  ShotDoser->AddGlass(KZ4_DOUT, KZ4_SCK, KZ4_ANGLE);
  ShotDoser->AddGlass(KZ5_DOUT, KZ5_SCK, KZ5_ANGLE);
  ShotDoser->AddGlass(KZ2_DOUT, KZ2_SCK, KZ2_ANGLE);
  ShotDoser->AddGlass(KZ1_DOUT, KZ1_SCK, KZ1_ANGLE);
  ShotDoser->AddGlass(KZ6_DOUT, KZ6_SCK, KZ6_ANGLE);
  ShotDoser->AddGlass(KZ3_DOUT, KZ3_SCK, KZ3_ANGLE);

  ShotDoser->Init();
}
void loop() {
  ShotDoser->Update();
}
