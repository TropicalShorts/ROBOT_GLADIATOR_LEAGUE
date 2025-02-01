#include <Adafruit_NeoPixel.h>

// Neopixel information 
//////////////////////////////////////////////////////////////////////////////////////////////////
// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    5

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 60

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//////////////////////////////////////////////////////////////////////////////////////////////

// Proximity sensor variables and debounce logic
////////////////////////////////////////////////////////////////////////////////////////////////////
const int proximitySensorPin1 = 6;    // Digital input pin connected to the proximity sensor

unsigned long lastDebounceTime1 = 0;
unsigned long debounceDelay = 1000000;  // 1 second debounce time in microseconds
int lastSensorState1 = LOW;

bool pinState1 = true;    // State of the proximity sensor
bool pin1DownFirst = false;
//////////////////////////////////////////////////////////////////////////////////////////////

// Relay and limit switch variables
///////////////////////////////////////////////////////////////////////////////////////////////
const int relayPin1 = 8;           // Relay control pin 1, Tightens
const int relayPin2 = 7;           // Relay control pin 2, Loosens 

const int limitSwitch1 = 3;        // Limit switch for tightening
const int limitSwitch2 = 2;        // Limit switch for loosening

const long RESET_TIME_ALLOWED = 5000; // 5 seconds allowed for reset
//////////////////////////////////////////////////////////////////////////////////////////////

// State machine variables
///////////////////////////////////////////////////////////////////////////////////////////////
int currentState = 0;

///////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // Initialize serial communication with Mega via hardware Serial
  Serial.begin(9600);

  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);

  // Initialize limit switch pins as inputs
  pinMode(limitSwitch1, INPUT);
  pinMode(limitSwitch2, INPUT);

  // Initialize the proximity sensor pin as an input
  pinMode(proximitySensorPin1, INPUT);

  // Initialize NeoPixel strip
  strip.begin();           // Initialize NeoPixel strip object
  strip.show();            // Turn off all pixels
  strip.setBrightness(255); // Set brightness to maximum

  strip.clear();
  colorWipe(strip.Color(  0, 255,   0), 255); // Green
  strip.show();
}

void loop() {
  checkMegaMessages(); // Check for commands from Mega

  switch(currentState) {
    case 0:
      // Idle state
      break;

    case 1:
      // Monitoring the proximity sensor
      debounceProxReading();
      break;

    case 2:
      // Paused state
      strip.clear();
      colorWipe(strip.Color(0, 0, 255), 255); // Blue color to indicate paused
      strip.show();
      break;

    case 3:
      // Reset state
      reset_pin();
      currentState = 0;
      break;

    case 4:
      // Reset in pause state, difference is to return to pause state after
      reset_pin();
      currentState = 2;
      break;
  }
}

void checkMegaMessages() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equals("START")) {
      currentState = 1;
    } else if (command.equals("PAUSE")) {
      currentState = 2;
    } else if (command.equals("RESET")) {
      currentState = 3;
    } else if (command.equals("RESET_IN_PAUSE")){
      currentState = 4;
    }
  }
}

void debounceProxReading(){
  // Get the current time
  unsigned long currentTime = micros();

  // Set NeoPixels to green
  strip.clear();
  colorWipe(strip.Color(0, 255, 0), 255); // Green color
  strip.show();

  // Read the value from the proximity sensor
  int sensorValue1 = digitalRead(proximitySensorPin1);

  // Check if the sensor state has changed and debounce
  if (sensorValue1 != lastSensorState1) {
    // Reset the debounce timer
    lastDebounceTime1 = currentTime;
  }

  // Only update if debounce time has passed
  if ((currentTime - lastDebounceTime1) > debounceDelay) {
    if (sensorValue1 == LOW) {
      // Set NeoPixels to red
      strip.clear();
      colorWipe(strip.Color(255, 0, 0), 255); // Red color
      strip.show();

      pin1DownFirst = true;
    } else {
      pin1DownFirst = false;
    }
  }

  if (pin1DownFirst == true){
    // Send "DOWN" to the Mega
    Serial.println("DOWN");
    currentState = 0; // Return to idle state
  }

  // Save the current sensor state
  lastSensorState1 = sensorValue1;
}

// Function to reset the pin
void reset_pin() {
  static unsigned long previousMillis1 = 0;
  static unsigned long previousMillis2 = 0;

  strip.clear();
  colorWipe(strip.Color(255, 255, 0), 255); // Yellow color
  strip.show();

  delay(2000);

  // Handle relayPin1 and limitSwitch1
  while (digitalRead(limitSwitch1) == LOW) {        // Tighten cable to set pin upright
    unsigned long currentMillisPinReset = millis();

    if (previousMillis1 == 0) {
      previousMillis1 = currentMillisPinReset;      // Start timing
    }
    if (currentMillisPinReset - previousMillis1 <= RESET_TIME_ALLOWED) {
      digitalWrite(relayPin2, LOW);
      digitalWrite(relayPin1, HIGH);                // Activate relay
    } else {
      digitalWrite(relayPin1, LOW);                 // Deactivate relay
    }
  } 
  digitalWrite(relayPin1, LOW);

  delay(3000);                                      //Give bowling pin time to settle and lose vibrations
  
  while (digitalRead(limitSwitch2) == LOW) {        // Loosen cable to allow pin to be knocked over
    unsigned long currentMillisPinReset = millis();
    
    if (previousMillis2 == 0) {
      previousMillis2 = currentMillisPinReset;       // Start timing
    }
    if (currentMillisPinReset - previousMillis2 <= RESET_TIME_ALLOWED) {
      digitalWrite(relayPin1, LOW);
      digitalWrite(relayPin2, HIGH);                // Activate relay
    } else {
      digitalWrite(relayPin2, LOW);                 // Deactivate relay
    }
  }
  digitalWrite(relayPin1, LOW);                     // Ensure relay is off
  previousMillis1 = 0;                              // Reset timing
  digitalWrite(relayPin2, LOW);                     // Ensure relay is off
  previousMillis2 = 0;                              // Reset timing

  strip.clear();
  colorWipe(strip.Color(0, 255, 0), 255);           // Green color
  strip.show();
  currentState = 0;
  pin1DownFirst = false;                            // Reset the flag
}

void colorWipe(uint32_t color, int wait) {
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    // No need to show and delay here; we update all at once
  }
}
