#ifndef ACCELEROMETER_H
#define ACCELLEROMETER_H

#include "Types.h"

class Accelerometer {
  private:
    int   _inputs;
    pin_t *_input_pins;
    int   *_last_values;
  
  public:
    Accelerometer(int inputs, pin_t *input_pins);
};

#endif
