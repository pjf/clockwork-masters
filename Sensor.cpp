#include "Sensor.h"

// Constructor
Sensor::Sensor(pin_t pin) {
  _pin = pin;

  // Our lines are pulled high so if the sensor is disconnected we're still good.
  // This also lets us use bare sensors, rather than just the freetronics boards.
  pinMode(_pin, INPUT_PULLUP);

  // Test for fault. The pin should be HIGH from our pull-up. If it's low, then
  // there's a short in the line and we should ignore it in the future.
  _faulty = (digitalRead(_pin) == HIGH ? false : true );
}

// Returns true if the sensor is active
// Returns false if the sensor is inactive or faulty.

bool Sensor::activated() {
  if (_faulty) {
    return false;
  }

  // Sensors are pulled low when active.
  return ( digitalRead(_pin) == LOW );
}
