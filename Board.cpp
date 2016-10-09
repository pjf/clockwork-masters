#include "Board.h"
#include "Dip.h"
#include "Light.h"

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

// Boards before this version don't have dip switches
const int Min_Version_For_Switches = 3;

const int Light_Pins[LIGHTS] = { 3, 6, 9 };

// I wish I could use a vector here, but using <vector> causes the code to
// not compile for the uno. Note that the Dip-pins are in "reverse" order,
// because that means they correspond to the labelling (1-4) on the board itself.
const int Dip_Pins[DIP_SWITCHES] = { 23, 22, 21, 20 };

enum Dip_Function_t {
    Dip_Unassigned,         // Does nothing!
    Dip_Power_Led,          // Controls power LED; useful for testing
    Dip_Photography,        // Sets photography mode
    Dip_Pulse_Enable,       // Sets sensor 3 to "pulse" rather than "shine"
    Dip_POST_Disable        // Skip the Power-On Self-Test
};

// What do each of the switches do?
const int Dip_Function[] = {
    Dip_Power_Led,
    Dip_Photography,
    Dip_Pulse_Enable,
    Dip_POST_Disable
};

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

    // Set up our dip switches

    for (int i = 0; i < DIP_SWITCHES; i++) {
        _dips[i] = new Dip(Dip_Pins[i]);
    }

    // Set up our lights

    for (int i = 0; i < LIGHTS; i++) {
        Lights[i] = new Light(Light_Pins[i]);
    }

    // Read our dip switches
    update_dips();

}

// Sets the power LED on or off.
void Board::power_led(int state) {
    digitalWrite(_pwr, state);
}

// Updates state based upon various dip switches.
// In most cases this sets state (eg: photography mode) rather than
// directly changing things. The Power LED is an exception.
void Board::update_dips() {

    // If our board doesn't have dip switches, then do nothing.
    if (_version < Min_Version_For_Switches) {
        return;
    }

    // Flip all our states to false. If our switches are
    // set, they'll be flipped back on.
    Pulse_Enabled = false;
    Photography   = false;
    POST_Disable  = false;

    for (int i = 0; i < DIP_SWITCHES; i++) {
        switch (Dip_Function[i]) {
            case Dip_Unassigned:
                break;
            case Dip_Power_Led:
                power_led( _dips[i]->activated() );
                break;
            case Dip_Photography:
                Photography = _dips[i]->activated();
                break;
            case Dip_Pulse_Enable:
                Pulse_Enabled = _dips[i]->activated();
                break;
            case Dip_POST_Disable:
                POST_Disable = _dips[i]->activated();
                break;
            ;
        }
    }
}
