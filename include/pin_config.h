#pragma once
#include <Arduino.h>

// pump
#define PUMP_PIN_COIL1  1

// servo
#define SERVO_PIN_SIG   A0

// encoder
#define ENC_PIN_CLK     3   // ISR
#define ENC_PIN_DT      2   // ISR
#define ENC_PIN_SW      A6

// leds
#define LEDS_PIN        A1

// glass 0
#define KZ4_DOUT        8
#define KZ4_SCK         9
#define KZ4_ANGLE       0   // Degrees

#define KZ5_DOUT        11
#define KZ5_SCK         10
#define KZ5_ANGLE       35  // Degrees

#define KZ2_DOUT        5
#define KZ2_SCK         4
#define KZ2_ANGLE       70  // Degrees

#define KZ1_DOUT        A2
#define KZ1_SCK         A3
#define KZ1_ANGLE       105 // Degrees

#define KZ6_DOUT        12
#define KZ6_SCK         13
#define KZ6_ANGLE       140 // Degrees

// glass 5
#define KZ3_DOUT        7
#define KZ3_SCK         6
#define KZ3_ANGLE       178 // Degrees