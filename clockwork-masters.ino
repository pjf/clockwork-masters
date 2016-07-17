// Clockwork Butterfly controller code.
// Paul '@pjf' Fenwick, 2016
// You can use and/or distribute this code under the GPLv2 license.

// How many output pins do we have?
const int OUTPUTS = 3;

// Which dip switches should do what?
const int DIP_SHOW_STATE = 0;

// Pins which we can do PWM on
int LINE[OUTPUTS] = { 3, 6, 9 };

// Lights are initialised later.
#include "Light.h"
Light *Lights[OUTPUTS];

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

  // Initialise the lighting pins as PWM output.
  int i;
  for (i=0; i < OUTPUTS; i++) {
    Lights[i] = new Light(LINE[i]);
  }

  // Accelerometer initialisation
  
  accelerometer = new Accelerometer(ANALOG_PINS);

  // And the sensor pins
  for (i = 0; i < DIGITAL_INPUTS; i++) {
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
  for (i=0; i < OUTPUTS; i++) {
    board->power_led(HIGH);
    Lights[i]->on();
    delay(1000);
    board->power_led(LOW);
    Lights[i]->off();
    delay(250);
  }

}

// Our main loop runs forever. Here's where all the fancy things happen.
void loop() {
  int i;

  // Debug sensor lines.
  Serial.print("Sensor read is ");

  for (i=0; i < DIGITAL_INPUTS; i++) {
    bool sensor = Sensors[i]->activated();
    Serial.print(sensor);
    Serial.print(" ");
  }

  Serial.print("|| ");  

  // Update our behaviour based upon dip switches, if they exist on the board.
  board->update_dips();

  // Nudge our accelerometer.
  accelerometer->update();
  
  // Walk through our outputs, and adjust according to our sensors.
  for (i=0; i < OUTPUTS; i++) {

    Light *light = Lights[i];

    // Shine increases the min power to make everythng more bright.
    light->brighten( Sensors[HFE_SHINE]->activated() );

    // Quell drops down the max power until it reaches zero,
    // and allows us to have a completely dark suit.
    light->quell( Sensors[HFE_QUELL]->activated() );

    // Pulse just causes any inactive lines to activate
    light->pulse( Sensors[HFE_PULSE]->activated() );
  }  

  // Now do our pulsing. This actually sets our light values.
  for (i = 0; i < OUTPUTS; i++) {
    Lights[i]->update(accelerometer->sparkle());
  }
 
  delay(SLEEP);
}
