#ifndef ACCELEROMETER_H
#define ACCELLEROMETER_H

#include "Types.h"

#define ACCEL_AXES 3

class Accelerometer {
  private:
    pin_t _input_pins[ACCEL_AXES];
    int   _last_values[ACCEL_AXES];
    int   _sparkle;
  
  public:
    Accelerometer(pin_t *input_pins);
    void update();
    int sparkle();
};

#endif
