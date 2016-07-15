#include "Sensor.h"

// Constructor
Sensor::Sensor(pin_t pin) {
  _pin = pin;

  // Test for fault. The pin should be HIGH when we're created.
  _faulty = ! digitalRead(_pin);
}

// Returns true if the sensor is active
// Returns false if the sensor is inactive or faulty.
bool Sensor::activated() {
  if (_faulty) {
    return false;
  }

  return ! digitalRead(_pin);
}
