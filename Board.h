#ifndef BOARD_H
#define BOARD_H

#include "Types.h"
#include "Dip.h"

// I wish this wasn't hard-coded.
#define DIP_SWITCHES 4

class Board {
    private:
        unsigned int  _version;  // V1, V2, V3, etc.
        pin_t _pwr;
        Dip *_dips[DIP_SWITCHES];

    public:
        Board();
        void power_led(int state);
        void update_dips();
};

#endif
