#include "Board.h"

// Board-dependent behaviour and sensing. In particular:
// V1 boards have no dip switches, and pin 0 tied to ground.
// V2 boards have no dip switches, and pin 1 tied to ground.
// V3 boards have dip switches, and pins 0 and 1 are floating.

// We also configure the high-level behaviour of our dip-switches here.
// This means we have one place to check things like "how should my sensors
// work?" or "should I be in photography mode right now?"

const int Pwr_Led = 13;
const int V1_Board_Pin = 0;
const int V2_Board_Pin = 1;

// Constructor
Board::Board() {

    // Pull up our version pins, then check if any stay low to sense
    // the board version.
    
    pinMode(V1_Board_Pin, INPUT_PULLUP);
    pinMode(V2_Board_Pin, INPUT_PULLUP);

    if (digitalRead(V1_Board_Pin) == LOW) {
        _version = 1;
    }
    else if (digitalRead(V2_Board_Pin) == LOW) {
        _version = 2;
    }
    else {
        _version = 3;
    }

    // All boards have the PWR LED on the same pin.

    _pwr = Pwr_Led;

    pinMode(_pwr, OUTPUT);
    power_led(HIGH);
}

// Sets the power LED on or off.
void Board::power_led(int state) {
    digitalWrite(_pwr, state);
}

