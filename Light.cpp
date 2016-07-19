#include "Light.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <math.h>

// Helper function.
// Just like regular map, only it works on doubles.
double doubleMap(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Constructor
Light::Light(pin_t pin, int idle_min, int idle_max) {
  _pin       = pin;
  _value     = 0;
  _direction = 1;
  _count_to  = 200;
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

// Our lights follow a sin(x^n) curve with power. This
// is the 'n' in that curve. Higher values cause a slower
// initial increase in power, which compensates for the lights
// having greater sensitivity at lower power levels.
const double smoothing_power = 3;

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
void Light::set_power(int value) {

  // This is our linear value. It just constrains us to 0-maximum power.
  // Absolute minimum power is enforced later in this function.
  int linear_value = constrain(value, 0, absolute_max_pwr);

  // Now map this onto a sin(x^n) wave. Full power is reached at x = (pi/2)^(1/n),
  // so we calculate that here.
  double max_trig_value = pow(M_PI_2, 1/smoothing_power);

  // Map our linear value onto an input suitable for handing to sin(x^n).
  double smoothed_x = doubleMap(linear_value,0,absolute_max_pwr,0,max_trig_value);

  // Pass that value through the smoothing function.
  double smoothed_value = sin( pow(smoothed_x, smoothing_power) );

  // Finally, map that output to our lights power.
  analogWrite(_pin, doubleMap( smoothed_value, 0, 1, absolute_min_pwr, absolute_max_pwr ) );
}

// On and off set the light on and off as if it were a digital line.
void Light::on()  { set_power(255); }
void Light::off() { set_power(0);   }

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

