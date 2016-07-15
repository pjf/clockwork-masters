// Clockwork Butterfly controller code.
// Paul '@pjf' Fenwick, 2016
// You can use and/or distribute this code under the GPLv2 license.

// How many output pins do we have?
#define OUTPUTS 3

// Which dip switches should do what?
#define DIP_SHOW_STATE 0
#define DIP_ACCEL_TO_SPARKLE 3

// Pins which we can do PWM on
int LINE[OUTPUTS]       = { 3, 6, 9 };

// Starting values (brightness) and directions for each line.
int VALUE[OUTPUTS]      = { 5, 5, 5  };
int DIRECTION[OUTPUTS]  = { 1, 1, 1 };

// Maximum value to count to per line.
// Higher values = slower pulses
int COUNT_TO[OUTPUTS]  = { 50, 50, 50 };

// Min/Maximum power for each output.
// 0 is entirely off, 255 is full power.
int MIN_POWER[OUTPUTS]      = {  5,  5,  5  };
int MAX_POWER[OUTPUTS]      = {  255, 255, 255 };

// Current min/max power allows us the freedom to manipulate
// power short-term.
int CURR_MAX_POWER[OUTPUTS];
int CURR_MIN_POWER[OUTPUTS];

// Minimum and maximum delays between random pulses.
// Zero means no delay, both the same means a fixed
// delay.
int RAND_DELAY_MIN[OUTPUTS]  = {  50,  50,  50 };
int RAND_DELAY_MAX[OUTPUTS]  = { 200, 200, 200 };

// If anything in here is non-zero, we decrement it
// rather than incrementing or decrementing our pulse.
// This gets set by our code.
int RAND_DELAY_IDLE[OUTPUTS] = { 0, 0, 0 };

// Sleep time in between each run. Having this reduces flicker, or at least it did when running
// on the LeoStick. Altering this can change the timing of everything else (lights, accelerometer,
// quickness to react to sensor changes) so use with care.
int SLEEP = 10;

// This just shows we're on. On most boards LED 13 is on the controller itself.
int PWR_LED = 13;

// Analog input lines. In our design these are hooked to an accelerometer, but
// any analog input will do.
#define ANALOG_INPUTS 3
int ANALOG[ANALOG_INPUTS] = { 17, 18, 19 };

// Digital inputs lines. In our design these are hooked to hall-effect sensors.
// We expect these to be pulled LOW when activated. If they're not connected,
// then that's cool, we use the internal pull-up resistors to stop them floating.
#define DIGITAL_INPUTS 4

// Sensors array is initialised later.
#include "Sensor.h"
Sensor *Sensors[DIGITAL_INPUTS];

// DIP switches
// These are both physical switches and lines that might be soldered to ground to
// indicate particular configurations. In our case 23-20 are physical switches
// (listed in reverse order because the "first" sitch on our board is on pin 23, and
// the "last" on pin 20), and pins 0 and 1 are hooked to ground on our two boards that
// are missing DIP switches entirely. Again, these can be unconnected; we use pull-ups
// to make sure they don't float.
#define DIP_SWITCHES 6

// DIP 1-4, 0 = Big board, 1 = Small board.
int DIP[DIP_SWITCHES] = { 23, 22, 21, 20, 0, 1 };

// Accelerometer readings. These are used to keep state; changes in readings make us glow more.
int ACCEL_LAST[3] = { 0, 0, 0 };

// Accelerometer sensitivity. We divide all readings by this before adding.
// DO NOT MAKE ZERO OR YOU WILL END THE UNIVERSE!
#define ACCEL_SENSIT 8

// Accelerometer deadzone. This means that small movements and thermal noise won't
// be listened to, but allows larger movements to still be counted. The larger this
// value, the larger a change in acceleration is required to register.
#define ACCEL_DEADZONE 10

// Sparkle added by accelerometer
int Added_Sparkle = 0;

// Each cycle, reduce sparkle by this percentage.
#define SPARKLE_DECAY 1

// Low and high readings that we see at rest on the accelerometer.
// These don't really get used in our code anymore, and will be removed shortly.
//
// TODO: Remove these and the code that uses them.
//
// These are for the freetronics version
// #define ACCEL_LOW 255
// #define ACCEL_HIGH 755

// These are zero scaling versions
#define ACCEL_LOW 0
#define ACCEL_HIGH 1023

// Which HES switches do what
#define HFE_QUELL 0 // Turn lights off.
#define HFE_SHINE 1 // Brighten all lights.
#define HFE_PULSE 2 // Activate and increase pulsing.

// The setup routine runs once when our board is powered on.
void setup() {

  // These are now private to our setup, since everytyhing else
  // should be using our sensor array.
  int DIGITAL[DIGITAL_INPUTS] = { 7, 10, 11, 12 };

  // Set the power LED, so we can see we're running.
  pinMode(PWR_LED, OUTPUT);
  digitalWrite(PWR_LED, HIGH);
  
  // Initialise the lighting pins as PWM output.
  int i;
  for (i=0; i < OUTPUTS; i++) {    
    pinMode(LINE[i], OUTPUT);     
    analogWrite(LINE[i],0);
  }

  // Initialise the accelerometer pins
  for (i = 0; i < ANALOG_INPUTS; i++) {
    pinMode(ANALOG[i], INPUT);
  }

  // And the sensor pins
  for (i = 0; i < DIGITAL_INPUTS; i++) {
    
    // Sensors are objects, they do their own init.
    Sensors[i] = new Sensor(DIGITAL[i]);
  }

  // And the dip-switches
  for (i = 0; i < DIP_SWITCHES; i++) {
    pinMode(DIP[i], INPUT_PULLUP);
  }

  // Initalise our power levels.
  for (i = 0; i < OUTPUTS; i++) {
    CURR_MAX_POWER[i] = MAX_POWER[i];
    CURR_MIN_POWER[i] = MIN_POWER[i];
  }

  // Read starting values into our accelerometer tracker. These get scaled and constrainted to
  // a 0-1023 range.
  for (i = 0; i < ANALOG_INPUTS; i++) {
    ACCEL_LAST[i] = constrain(map(analogRead(ANALOG[i]),ACCEL_LOW,ACCEL_HIGH,0,1023),0,1023);
  }

  // Init our serial monitor, so if a debugger is running it can see what we're doing.
  Serial.begin(9600);
  Serial.print("Init complete");

  // TODO: Skip all this if a DIP-switch tells us to.

  // Half a second after power on don't do anything, so
  // our humans can look at the line which may be faulty.
  digitalWrite(PWR_LED, LOW);
  delay(500);

  // Pulse our power light to show we're running.
  // Also pulse our lines to test they're working.
  for (i=0; i < OUTPUTS; i++) {
    digitalWrite(PWR_LED, HIGH);
    analogWrite(LINE[i], 255);
    delay(1000);
    digitalWrite(PWR_LED, LOW);
    analogWrite(LINE[i], 0);
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

  Serial.print("DIP: ");

  // NB, this FLIPS the bits as they come in. Our switches are connected
  // to GND when they're on, but our logic here exports them as "True" when on.

  // TODO: If we're reading these, we should be saving them too, rather than having direct
  // reads as part of the main loop.
  for (i=0; i < DIP_SWITCHES; i++) {
    int dip = ! digitalRead(DIP[i]);
    Serial.print(dip);
    Serial.print(" ");
  }

  // The state DIP switch is used for debugging. We show our power light
  // based upon its value. We *do not* flip the logic here, as boards
  // without dip switches (using the pull-up only) should have their light set
  // on.
  digitalWrite(PWR_LED, digitalRead(DIP[DIP_SHOW_STATE]));

  Serial.print("|| ");  

  // Walk through our analog lines.
  // TODO: Count based on actual inputs, not 'OUTPUTS'! x_x
  int potential_sparkle = 0;
  
  for (i=0; i < OUTPUTS; i++) {
    int val = analogRead(ANALOG[i]);
    Serial.print(val);
    Serial.print(" (");

    // Munge our accel input to the full range of outputs.
    int mapped = constrain(map(analogRead(ANALOG[i]),ACCEL_LOW,ACCEL_HIGH,0,1023),0,1023);

    Serial.print(mapped);
    Serial.print(") ");

    // TODO: Just have the accelerometers add to sparkle.
    // TODO: Have the switch DISABLE the accelerometer.

    // Based upon the status of the 4th switch (3rd from zero), we either
    // base our delay between pulses based on the accelerometer input (original
    // testing program), or we have accelerometer values increase brightness of
    // our lines.

    if (DIP[DIP_ACCEL_TO_SPARKLE]) {
      // Add our difference to our potential sparkle.
      potential_sparkle += abs(mapped - ACCEL_LAST[i]) / ACCEL_SENSIT;

      // And save this as the last reading.
      ACCEL_LAST[i] = mapped;
    }

    // Otherwise our accelerometer does nothing.
  }

  // Add our sparkle iff it's greater than our deadzone.
  if (potential_sparkle > ACCEL_DEADZONE) {
    Added_Sparkle += potential_sparkle;
  }

  Serial.print(" || PWR: ");

  // Walk through our outputs. For each one, we'll be modifying based upon sensor readings, if needed.
  // TODO: Invert this loop, so we check each sensor, and then walk through output lines if active?
  //       It feels cleaner and easier to work with.
  for (i=0; i < OUTPUTS; i++) {

    // We no longer need to skip faulty lines, because our sensors simply return
    // false if they show faults.

    // Shine increases the min power to make everythng more bright.
    if (Sensors[HFE_SHINE]->activated()) {
      // Power-up!
      CURR_MIN_POWER[i] = min(MAX_POWER[i], CURR_MIN_POWER[i]+1);
    }
    else {
      // Power-down
      CURR_MIN_POWER[i] = max(MIN_POWER[i], CURR_MIN_POWER[i]-1);
    }

    // Quell drops down the max power until it reaches zero,
    // and allows us to have a completely dark suit.
    if (Sensors[HFE_QUELL]->activated()) {
      // Quell
      CURR_MAX_POWER[i] = max(0, CURR_MAX_POWER[i]-1);
      CURR_MIN_POWER[i] = 0; // Quell lets us go completely dark.
    }
    else {
      // Restore max power back upwards.
      // TODO: Sunrise effect?
      CURR_MAX_POWER[i] = min(MAX_POWER[i], CURR_MAX_POWER[i]+1);

      // If our line was underpowered (because we went dark) then
      // bring it back towards its specified minimum.
      if (CURR_MIN_POWER[i] < MIN_POWER[i]) {
        CURR_MIN_POWER[i]++;
      }
    }

    // Pulse just causes any inactive lines to activate, and sets
    // everything to increase in brightness.
    // TODO: Increase the speed of pulses while these are active, maybe by bumping direction values.
    if (Sensors[HFE_PULSE]->activated()) {
      // Flip direction if we were decaying.
      if (DIRECTION[i] < 0) {
        DIRECTION[i] = -DIRECTION[i];
      }

      // Activate line if not already active.
      RAND_DELAY_IDLE[i] = 0;
    }
    
    Serial.print(CURR_MIN_POWER[i]);
    Serial.print("-");
    Serial.print(CURR_MAX_POWER[i]);
    Serial.print(" ");
  }  

  Serial.print("\n");

  // Now do our pulsing. This actually sets our light values.
  for (i = 0; i < OUTPUTS; i++) {

    // If we're idling, then simply decrement that counter and skip
    // this line for now.
    if (RAND_DELAY_IDLE[i] > 0) {
      RAND_DELAY_IDLE[i]--;
    }
    
    // Otherwise we're pulsing.
    else {
      VALUE[i] += DIRECTION[i];

      // Constrain our value in, since other code can cause us to
      // over or underrun.
      VALUE[i] = constrain(VALUE[i], 0, COUNT_TO[i]);

      // If we bump against either end, then flip direction.
      if (VALUE[i] >= COUNT_TO[i] || VALUE[i] <= 0) {
        DIRECTION[i] = -DIRECTION[i];
        
        // If we've just hit zero, then sleep
        // this line for a random amount of time.
        if (VALUE[i] <= 0) {
          RAND_DELAY_IDLE[i] = random(RAND_DELAY_MIN[i], RAND_DELAY_MAX[i]);
        }
      }
    }

    // Either way, set the pin values even if we're sleeping, as
    // sensor lines may have altered our values. We constrain this to 0-255
    // as sparkle may overflow our max line values.

    analogWrite(LINE[i], constrain(map(VALUE[i], 0, COUNT_TO[i], CURR_MIN_POWER[i], CURR_MAX_POWER[i])+Added_Sparkle,0,255));

  }

  // Decay our sparkle.
  Added_Sparkle = max(0, ((Added_Sparkle * (100 - SPARKLE_DECAY))/100) - 1);
 
  delay(SLEEP);
}
