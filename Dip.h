#ifndef DIP_H
#define DIP_H

#include "Types.h"

class Dip {
  private:
    pin_t _pin;
  
  public:
    Dip(pin_t pin);
    bool activated();
};

#endif
