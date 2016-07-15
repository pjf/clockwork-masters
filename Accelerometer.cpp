#include "Accelerometer.h"
#include <stdlib.h>
#include "WProgram.h"

// Accelerometer sensitivity. We divide all readings by this before adding.
// DO NOT MAKE ZERO OR YOU WILL END THE UNIVERSE!
const int Sensitivity = 8;

// Accelerometer deadzone. This means that small movements and thermal noise won't
// be listened to, but allows larger movements to still be counted. The larger this
// value, the larger a change in acceleration is required to register.
const int Deadzone = 10;

// Each cycle, reduce sparkle by this percentage, in addition to the 1 unit
// we decrease at anyway.
const int Decay = 1;

// Constructor.
Accelerometer::Accelerometer(pin_t input_pins[ACCEL_AXES]) {
  _sparkle = 0;

  // Set the hardware for input and take first values.
  for (int i = 0; i < ACCEL_AXES; i++) {
    _input_pins[i] = input_pins[i];
    pinMode(_input_pins[i], INPUT);
    _last_values[i] = analogRead(_input_pins[i]);
  }
  
}

void Accelerometer::update() {

  // Decay our sparkle from last iteration.
  _sparkle = max(0, ((_sparkle * (100 - Decay) / 100) - 1));
  
  int potential_sparkle = 0;
  
  for (int i=0; i < ACCEL_AXES; i++) {
    int val = analogRead(_input_pins[i]);
    Serial.print(val);
    Serial.print(" (");

    // Add our difference to our potential sparkle.
    potential_sparkle += abs(val - _last_values[i]) / Sensitivity;

    // And save this as the last reading.
    _last_values[i] =val;
  }

  // Add our sparkle iff it's greater than our deadzone.
  if (potential_sparkle > Deadzone) {
    _sparkle += potential_sparkle;
  }
}

int Accelerometer::sparkle() {
  return _sparkle;
}

