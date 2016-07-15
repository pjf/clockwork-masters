#ifndef LIGHT_H
#define LIGHT_H

#include "Types.h"

class Light {
  private:
    pin_t _pin;
    int   _value;
    int   _direction;
    int   _count_to;
    int   _min_power;
    int   _max_power;
    int   _curr_max_power;
    int   _curr_min_power;
    int   _idle_min;
    int   _idle_max;
    int   _idle;
    void  _cycle();
  
  public:
    Light(pin_t pin, int idle_min = 50, int idle_max = 200);
    void update(int bias);
    void set_power(int power);
};

#endif
