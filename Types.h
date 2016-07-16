#ifndef TYPES_H
#define TYPES_H

// Types for the whole project are set here.

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <core_pins.h>
#endif

// What type do our pins (and hence digitalRead) use?
typedef uint8_t pin_t;

#endif
