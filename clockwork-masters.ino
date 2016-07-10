
// How many output pins do we have?
#define OUTPUTS 3

// Which dip switch says our accelerometer should add sparkle?
#define DIP_ACCEL_TO_SPARKLE 3

// Pins which we can do PWM on
int LINE[OUTPUTS]       = { 3, 6, 9 };

// Starting values (brightness) and directions for each line.
int VALUE[OUTPUTS]      = { 20, 60, 120  };
int DIRECTION[OUTPUTS]  = { 1, 1, 1 };

// Maximum value to count to per line.
// Higher values = slower pulses
int COUNT_TO[OUTPUTS]  = { 1000, 800, 600 };

// Min/Maximum power for each output.
int MIN_POWER[OUTPUTS]      = {  5,  5,  5  };
int MAX_POWER[OUTPUTS]      = {  255, 255, 255 };

// Current min/max power allows us the freedom to manipulate
// power short-term.
int CURR_MAX_POWER[OUTPUTS];
int CURR_MIN_POWER[OUTPUTS];

// Minimum and maximum delays between random pulses.
// Zero means no delay, both the same means a fixed
// delay.
int RAND_DELAY_MIN[OUTPUTS]  = { 200, 200, 200 };
int RAND_DELAY_MAX[OUTPUTS]  = { 6000, 2000, 4000 };

// If anything in here is non-zero, we decrement it
// rather than incrementing or decrementing our pulse.
// This gets set by our code.
int RAND_DELAY_IDLE[OUTPUTS] = { 0, 0, 0 };

// Sleep time in between each run. Having this reduces flicker.
int SLEEP = 10;

// This just shows we're on.
int PWR_LED = 13;

// Analog inputs
#define ANALOG_INPUTS 3
int ANALOG[ANALOG_INPUTS] = { 17, 18, 19 };

// Digital inputs
#define DIGITAL_INPUTS 4
int DIGITAL[DIGITAL_INPUTS] = { 7, 10, 11, 12 };

// DIP switches
#define DIP_SWITCHES 6

// DIP 1-4, 0 = Big board, 1 = Small board.
int DIP[DIP_SWITCHES] = { 23, 22, 21, 20, 0, 1 };

// Accelerometer readings
int ACCEL_LAST[3] = { 0, 0, 0 };

// Accelerometer sensitivity. We divide all readings by this before adding.
// DO NOT MAKE ZERO YOU WILL END THE UNIVERSE!
#define ACCEL_SENSIT 5

// Sparkle added by accelerometer
int Added_Sparkle = 0;

// Each cycle, reduce sparkle by this percentage.
#define SPARKLE_DECAY 1

// Working digital pins.
// On at least one board, there's a faulty cable that causes sensors to start *on*.
// This diagnoses those and removes them from the circuit.
int DIGITAL_FAULTY[DIGITAL_INPUTS] = { 0, 0, 0, 0};

// Low and high readings that we see at rest on the accelerometer.
// These are for the freetronics version
// #define ACCEL_LOW 255
// #define ACCEL_HIGH 755

// These are zero scaling versions
#define ACCEL_LOW 0
#define ACCEL_HIGH 1023

// Which HFE switches do what
#define HFE_QUELL 0
#define HFE_SHINE 1
#define HFE_PULSE 2

// the setup routine runs once when you press reset:
void setup() {

  // Power LED
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
    // HFEs are high for no magnets nearby. I'm *so glad*
    // the Teensy has internal pull-ups that can be enabled.
    pinMode(DIGITAL[i], INPUT_PULLUP);
  }

  // And the dip-switches
  for (i = 0; i < DIP_SWITCHES; i++) {
    pinMode(DIP[i], INPUT_PULLUP);
  }

  // Test for faulty sensors. Any digital line that is LOW
  // has a short and should not be used.
  for (i = 0; i < DIGITAL_INPUTS; i++) {
    DIGITAL_FAULTY[i] = ! digitalRead(DIGITAL[i]);
  }

  // Set up our power levels
  for (i = 0; i < OUTPUTS; i++) {
    CURR_MAX_POWER[i] = MAX_POWER[i];
    CURR_MIN_POWER[i] = MIN_POWER[i];
  }

  // Read starting values into our accelerometer tracker
  for (i = 0; i < ANALOG_INPUTS; i++) {
    ACCEL_LAST[i] = constrain(map(analogRead(ANALOG[i]),ACCEL_LOW,ACCEL_HIGH,0,1023),0,1023);
  }

  Serial.begin(9600);
  Serial.print("Init complete");

  delay(1000);
}

// the loop routine runs over and over again forever:
void loop() {
  int i;

  // Debug sensor lines.
  Serial.print("Sensor read is ");

  for (i=0; i < DIGITAL_INPUTS; i++) {
    int sensor = digitalRead(DIGITAL[i]);
    Serial.print(sensor);
    Serial.print(" ");
  }

  Serial.print("DIP: ");

  // NB, this FLIPS the bits as they come in. Our switches are connected
  // to GND when theyre on, but our logic here exports them as "True" when on.
  for (i=0; i < DIP_SWITCHES; i++) {
    int dip = ! digitalRead(DIP[i]);
    Serial.print(dip);
    Serial.print(" ");
  }

  Serial.print("|| ");  

  // For now, we're going to have our max light delay determined by the accelrometer inputs.
  for (i=0; i < OUTPUTS; i++) {
    int val = analogRead(ANALOG[i]);
    Serial.print(val);
    Serial.print(" (");

    // Munge our accel input to the full range of outputs.
    int mapped = constrain(map(analogRead(ANALOG[i]),ACCEL_LOW,ACCEL_HIGH,0,1023),0,1023);

    Serial.print(mapped);
    Serial.print(") ");

    // Based upon the status of the 4th switch (3rd from zero), we either
    // base our delay between pulses based on the accelerometer input (original
    // testing program), or we have accelerometer values increase brightness of
    // our lines.

    if (DIP[DIP_ACCEL_TO_SPARKLE]) {
      // Add our difference to our sparkle.
      // We may want to add a dead zone later.

      // TODO: Add a deadzone. #2
      
      Added_Sparkle += abs(mapped - ACCEL_LAST[i]) / 5;

      // And save this as the last reading.
      ACCEL_LAST[i] = mapped;
    }
    else {
      // Use our accel input to set the delay between each pulse.
      RAND_DELAY_MAX[i] = max(RAND_DELAY_MIN[i], mapped);
    }

  }

  Serial.print(" || PWR: ");

  // Walk through our outputs, modified by each HFE if we see them.
  for (i=0; i < OUTPUTS; i++) {

    // Skip faulty lines.
    if (DIGITAL_FAULTY[i]) {
      Serial.print("FAULT ");
      continue;
    }

    // Shine increases the min power to make everythng more bright.
    if (!digitalRead(DIGITAL[HFE_SHINE])) {
      CURR_MIN_POWER[i] = min(MAX_POWER[i], CURR_MIN_POWER[i]+1);
    }
    else {
      CURR_MIN_POWER[i] = max(MIN_POWER[i], CURR_MIN_POWER[i]-1);
    }

    // Quell drops down the max power until it reaches zero,
    // and allows us to have a completely dark suit.
    if (!digitalRead(DIGITAL[HFE_QUELL])) {
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
    if (!digitalRead(DIGITAL[HFE_PULSE])) {

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

  // Now do our pulsing.
  for (i = 0; i < OUTPUTS; i++) {

    // If we're idling, then simply decrement that counter and skip
    // this line for now.
    if (RAND_DELAY_IDLE[i]) {
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
