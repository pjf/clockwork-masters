#include "Dip.h"

// Dip switches are like sensors. They'll pulled LOW when switched 'on'.
// The difference is we don't consider them to be faulty if they start switched on.

// Constructor
Dip::Dip(pin_t pin) {
  _pin = pin;

  // Our lines are pulled high so if the switch is disconnected or absent we
  // don't end up with a floating line.
  pinMode(_pin, INPUT_PULLUP);
}

// Returns true if the switch is on.
// Returns false if the switch is off or absent.

bool Dip::activated() {
  // Switches are pulled low when active.
  return ( digitalRead(_pin) == LOW );
}
