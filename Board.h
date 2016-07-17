#ifndef BOARD_H
#define BOARD_H

#include "Types.h"

class Board {
    private:
        unsigned int  _version;  // V1, V2, V3, etc.
        pin_t _pwr;

    public:
        Board();
        void power_led(int state);
};

#endif
