#ifndef GLOBALS_H
#define GLOBALS_H
#pragma once

extern float ML_PER_SECOND;
#define GLASSES 7

#define EEPROM_ADDR_START   0
#define EEPROM_FLOW_RATE    (EEPROM_ADDR_START)
#define EEPROM_VOLUME       (EEPROM_FLOW_RATE + sizeof(float))

struct myEeprom {
  float _flowrate;
  uint8_t _volume;
};

#endif