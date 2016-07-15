#ifndef SENSOR_H
#define SENSOR_H

#include "Types.h"

class Sensor {
  private:
    pin_t _pin;
    bool  _faulty;
  
  public:
    Sensor(pin_t pin);
    bool activated();
};

#endif
