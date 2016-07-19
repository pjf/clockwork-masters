// Clockwork Butterfly controller code.
// Paul '@pjf' Fenwick, 2016
// You can use and/or distribute this code under the GPLv2 license.

// Sleep time in between each run. Having this reduces flicker, or at least it did when running
// on the LeoStick. Altering this can change the timing of everything else (lights, accelerometer,
// quickness to react to sensor changes) so use with care.
const int SLEEP = 10;

// Digital inputs lines. In our design these are hooked to hall-effect sensors.
// We expect these to be pulled LOW when activated. If they're not connected,
// then that's cool, we use the internal pull-up resistors to stop them floating.
const int DIGITAL_INPUTS = 4;

// Sensors array is initialised later.
#include "Sensor.h"
Sensor *Sensors[DIGITAL_INPUTS];

#include "Accelerometer.h"
Accelerometer *accelerometer;

#include "Board.h"
Board *board;

// Which HES switches do what
#define HFE_QUELL 0 // Turn lights off.
#define HFE_SHINE 1 // Brighten all lights.
#define HFE_PULSE 2 // Activate and increase pulsing.

// The setup routine runs once when our board is powered on.
void setup() {

  // Board init comes first.
  board = new Board();

  // These are now private to our setup, since everytyhing else
  // should be using our objects.
  
  const int DIGITAL[DIGITAL_INPUTS] = { 7, 10, 11, 12 };
  const int ANALOG_INPUTS = 3;
  pin_t ANALOG_PINS[ANALOG_INPUTS] = { 17, 18, 19 };

  // Accelerometer initialisation
  accelerometer = new Accelerometer(ANALOG_PINS);

  // And the sensor pins
  for (int i = 0; i < DIGITAL_INPUTS; i++) {
    // Sensors are objects, they do their own init.
    Sensors[i] = new Sensor(DIGITAL[i]);
  }

  // Init our serial monitor, so if a debugger is running it can see what we're doing.
  Serial.begin(9600);
  Serial.print("Init complete");

  // Half a second after power on don't do anything, so
  // our humans can look at the line which may be faulty.
  board->power_led(LOW);
  delay(500);

  // Pulse our power light to show we're running.
  // Also pulse our lines to test they're working.
  for (int i=0; i < LIGHTS; i++) {
    board->power_led(HIGH);
    board->Lights[i]->on();
    delay(1000);
    board->power_led(LOW);
    board->Lights[i]->off();
    delay(250);
  }

}

// Our main loop runs forever. Here's where all the fancy things happen.
void loop() {
  int i;

  // Update our behaviour based upon dip switches, if they exist on the board.
  board->update_dips();

  // Nudge our accelerometer.
  accelerometer->update();
  
  // Walk through our outputs, and adjust according to our sensors.
  for (i=0; i < LIGHTS; i++) {

    Light *light = board->Lights[i];

    // Shine increases the min power to make everythng more bright.
    // If the shine sensor is active, OR if the pulse sensor is activated
    // but pulse is disabled, then we'll add to shine. Otherwise
    // we'll signal it to deactivate.

    if (
      Sensors[HFE_SHINE]->activated() ||
      ( ( ! board->Pulse_Enabled ) && Sensors[HFE_PULSE]->activated() )
    ) {
      light->brighten( true );
    }
    else {
      light->brighten( false );
    }

    // Quell drops down the max power until it reaches zero,
    // and allows us to have a completely dark suit.
    light->quell( Sensors[HFE_QUELL]->activated() );

    // If Pulse is enabled, then we'll run pulse mode, which simply
    // removes the idle sections between pulses.
    if (board->Pulse_Enabled) {
      light->pulse( Sensors[HFE_PULSE]->activated() );
    }
  }

  // Now do our pulsing. This actually sets our light values.
  // If we're in photography mode, we just max them all out.
  for (i = 0; i < LIGHTS; i++) {
    if (board->Photography) {
        board->Lights[i]->on();
    }
    else {
        board->Lights[i]->update(accelerometer->sparkle());
    }
  }
 
  delay(SLEEP);
}
