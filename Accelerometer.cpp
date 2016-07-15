#include "Accelerometer.h"
#include <stdlib.h>

// Constructor.
Accelerometer::Accelerometer(int inputs, pin_t *input_pins) {
  _inputs = inputs;
  _input_pins = input_pins;

  // Prepare for last value readings.
  // TODO: Make sure this succeeds! Also free it in destructor!
  _last_values = (int *) calloc(_inputs, sizeof(int));

  // Set the hardware for input and take first values.
  for (int i = 0; i < _inputs; i++) {
    pinMode(_input_pins[i], INPUT);
    _last_values[i] = analogRead(_input_pins[i]);
  }
  
}

