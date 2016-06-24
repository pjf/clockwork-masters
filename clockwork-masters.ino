
// How many output pins do we have?
#define OUTPUTS 3

// Pins which we can do PWM on
int LINE[OUTPUTS]       = { 3, 6, 9 };

// Starting values (brightness) and directions for each line.
int VALUE[OUTPUTS]      = { 20, 60, 120  };
int DIRECTION[OUTPUTS]  = { 1, 1, 1 };

// Maximum value to count to per line.
// Higher values = slower pulses
int COUNT_TO[OUTPUTS]  = { 200, 50, 50 };

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
int RAND_DELAY_MIN[OUTPUTS]  = { 36,  36,  36 };
int RAND_DELAY_MAX[OUTPUTS]  = { 600, 200, 800 };

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
#define DIP_SWITCHES 4
int DIP[DIP_SWITCHES] = { 20, 21, 22, 23 };

// Working digital pins.
// On at least one board, there's a faulty cable that causes sensors to start *on*.
// This diagnoses those and removes them from the circuit.
int DIGITAL_FAULTY[DIGITAL_INPUTS] = { 0, 0, 0, 0};

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

  Serial.begin(9600);
  Serial.print("Init complete");

  delay(1000);
}

// Low and high readings that we see at rest on the accelerometer.
// These are for the freetronics version
// #define ACCEL_LOW 255
// #define ACCEL_HIGH 755

// These are zero scaling versions
#define ACCEL_LOW 0
#define ACCEL_HIGH 1023

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

  for (i=0; i < DIP_SWITCHES; i++) {
    int dip = digitalRead(DIP[i]);
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

    // Use our accel input to set the delay between each pulse.
    RAND_DELAY_MAX[i] = max(RAND_DELAY_MIN[i], mapped);

    Serial.print(mapped);
    Serial.print(") ");

  }

  Serial.print(" || PWR: ");

  // For digital lines, if we see any active, that *increases* lights.
  for (i=0; i < OUTPUTS; i++) {

    // Skip faulty lines.
    if (DIGITAL_FAULTY[i]) {
      Serial.print("FAULT ");
      continue;
    }
    
    if (! digitalRead(DIGITAL[i])) {
      CURR_MIN_POWER[i] = min(MAX_POWER[i], CURR_MIN_POWER[i]+1);
    }
    else {
      CURR_MIN_POWER[i] = max(MIN_POWER[i], CURR_MIN_POWER[i]-1);
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
        // if our rand functions say we should.
        if (VALUE[i] <= 0) {
          RAND_DELAY_IDLE[i] = random(RAND_DELAY_MIN[i], RAND_DELAY_MAX[i]);
        }
      }
      // Set the pin value.
      analogWrite(LINE[i], map(VALUE[i], 0, COUNT_TO[i], CURR_MIN_POWER[i], CURR_MAX_POWER[i]));
    }
  }
  delay(SLEEP);
}
