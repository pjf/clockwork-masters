#ifndef BOARD_H
#define BOARD_H

#include "Types.h"
#include "Dip.h"
#include "Light.h"

// I wish this wasn't hard-coded.
#define DIP_SWITCHES 4
#define LIGHTS 3

class Board {
    private:
        unsigned int  _version;  // V1, V2, V3, etc.
        pin_t _pwr;
        Dip *_dips[DIP_SWITCHES];

    public:
        Light *Lights[LIGHTS];
        bool Photography = false; // Are we in photogrpahy mode?

        Board();
        void power_led(int state);
        void update_dips();
};

#endif