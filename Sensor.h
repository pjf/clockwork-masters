#ifndef SENSOR_H
#define SENSOR_H

#include <core_pins.h>

// What type do our pins (and hence digitalRead) use?
typedef uint8_t pin_t;

class Sensor {
  private:
    pin_t _pin;
    bool  _faulty;
  
  public:
    Sensor(pin_t pin);
    bool activated();
};

#endif
