#include "Light.h"
#include "WProgram.h"

// Constructor
Light::Light(pin_t pin, int idle_min = 50, int idle_max = 200) {
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
}

const int absolute_min_pwr = 0;
const int absolute_max_pwr = 255;

// Updates our light. Intended to be called once per cycle of main code.
// Bias is added to all lines if provided.
void Light::update(int bias) {

  // Update our light cycle.
  _cycle();

  // Map our cycle to a brightness, plus our bias.
  analogWrite(
    _pin,
    constrain(
      map(_value, 0, _count_to, _curr_min_power, _curr_max_power) + bias,
      absolute_min_pwr, absolute_max_pwr
    )
  );
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
