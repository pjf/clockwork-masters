#include "Light.h"
#include "WProgram.h"

// Constructor
Light::Light(pin_t pin, int idle_min, int idle_max) {
  _pin       = pin;
  _value     = 0;
  _direction = 1;
  _count_to  = 50;
  _min_power = 5;
  _max_power = 255;
  _curr_max_power = _max_power;
  _curr_min_power = _min_power;
  _idle_min = idle_min;
  _idle_max = idle_max;
  _idle = 0;

  // Initialise the hardware itself.
  pinMode(_pin, OUTPUT);     
  analogWrite(_pin,0);
}

const int absolute_min_pwr = 0;
const int absolute_max_pwr = 255;

// Updates our light. Intended to be called once per cycle of main code.
// Bias is added to all lines if provided.
void Light::update(int bias) {

  // Update our light cycle.
  _cycle();

  set_power( map(_value, 0, _count_to, _curr_min_power, _curr_max_power) + bias );
}

// Cycles our lights. Intended to be called by update.
void Light::_cycle() {
  
  // If we're idling, then keep doing that.
  if (_idle > 0) {
    _idle--;
    return;
  }

  // Move our value in the direction we're going.
  _value += _direction;

  // Constrain our value, since directions larger than '1' can
  // cause us to over or under-run.
  _value = constrain(_value, 0, _count_to);

  // If we've hit the bottom of our cycle, then idle.
  if (_value == 0) {
    _idle = random(_idle_min, _idle_max);
  }

  // Likewise if we've hit either end of our cycle, mark ourselves
  // as heading the other way.
  if (_value == 0 || _value == _count_to) {
    _direction = -_direction;
  }
}

// Sets power directly. Will not change internal state, so
// this will be overwritten to the next call to update().
//
// This auto-constrains to 0-255, but I'm not sure if we really need this.
void Light::set_power(int power) {
  analogWrite(_pin, constrain( power, absolute_min_pwr, absolute_max_pwr ) );
}

// If passed a true argument, makes everything brighter by boosting the minimum
// brightness. If passed a false argument, decays the minimum brihgtness towards
// its original value.
void Light::brighten(bool brighten) {
  if (brighten) {
    _curr_min_power = min(_max_power, _curr_min_power+1);
  }
  else {
    _curr_min_power = max(_min_power, _curr_min_power-1);
  }
}

// If passed a true argument, drop the max power down until it reaches zero.
// This allows our suit to go competely dark. Otherwise restore power values
// back to normal.
void Light::quell(bool quell) {
  if (quell) {
    _curr_max_power = max(0, _curr_max_power-1);
    _curr_min_power = 0;
  }
  else {
    _curr_max_power = min(_max_power, _curr_max_power+1);

    if (_curr_min_power < _min_power) {
      _curr_min_power++;
    }
  }
}

// Causes lights to start a pulse if not already pulsing.
// On a false argument, does nothing.
// TODO: Have this *speed* pulses, too.
void Light::pulse(bool pulse) {
  if (pulse) {
    _idle = 0;
  }
}

