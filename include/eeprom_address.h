#pragma once

#define EEPROM_ADDR_START   0
#define EEPROM_FLOW_RATE    (EEPROM_ADDR_START)                 // float
#define EEPROM_VOLUME       (EEPROM_FLOW_RATE + sizeof(float))  // int32_t
#define EEPROM_FACTOR       (EEPROM_VOLUME + sizeof(int32_t))   // float