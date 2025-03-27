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
const int proximitySensorPin1 = 9;    // Digital input pin connected to the proximity sensor
const int proximitySensorPin2 = 6;

unsigned long lastDebounceTime1 = 0;
unsigned long debounceDelay = 1000000;  // 1 second debounce time in microseconds
int lastSensorState1 = LOW;

bool pinState1 = true;    // State of proximity sensor 1
bool pin1DownFirst = false;

unsigned long lastDebounceTime2 = 0;
int lastSensorState2 = LOW;

bool pinState2 = true;    // State of proximity sensor 2
bool pin2DownFirst = false;
//////////////////////////////////////////////////////////////////////////////////////////////

// Relay and limit switch variables
///////////////////////////////////////////////////////////////////////////////////////////////
const int relayPin1 = 8;           // Relay control pin 1, Direction
const int relayPin2 = 7;           // Relay control pin 2, on/off

const int limitSwitch1 = 3;        // Limit switch for tightening
const int limitSwitch2 = 2;        // Limit switch for loosening

const long RESET_TIME_ALLOWED = 5000; // 5 seconds allowed for reset
//////////////////////////////////////////////////////////////////////////////////////////////



void setup() {
    Serial.begin(9600);  // Start serial communication

    pinMode(relayPin1, OUTPUT);
    pinMode(relayPin2, OUTPUT);

    // Initialize limit switch pins as inputs
    pinMode(limitSwitch1, INPUT);
    pinMode(limitSwitch2, INPUT);

    // Initialize the proximity sensor pin as an input
    pinMode(proximitySensorPin1, INPUT);
    pinMode(proximitySensorPin2, INPUT);

     // Set A0 to A7 as digital outputs
    for (int pin = A0; pin <= A7; pin++) {
        pinMode(pin, OUTPUT);
    }

    // Set D10 to D13 as digital outputs
    for (int pin = 10; pin <= 13; pin++) {
        pinMode(pin, OUTPUT);
    }


    Serial.println("Enter a number to select");
    Serial.println("1 - Relay 1 test");
    Serial.println("2 - Relay 2 test");
    Serial.println("3 - Limit switch 1 (data B) test");
    Serial.println("4 - Limit switch 2 (data A) test");
    Serial.println("5 - Proximity switch 1 (data B) test");
    Serial.println("6 - Proximity switch 2 (data A) test");
    Serial.println("7 - Neopixel (D5)");
    Serial.println("8 - Temp Sensor");
    Serial.println("9 - Expansion bus");

    int userInput = 0;
    
    // Wait for valid user input
    while (userInput == 0) {
        if (Serial.available() > 0) { 
            userInput = Serial.parseInt();  // Read input as an integer

            switch (userInput) {
                case 1:
                    Serial.println("Case 1 selected");
                      while(1){
                        digitalWrite(relayPin1, HIGH); 
                        delay(1000);
                        digitalWrite(relayPin1, LOW);
                        delay(1000); 
                      }
                    break;
                case 2:
                    Serial.println("Case 2 selected");
                      while(1){
                        digitalWrite(relayPin2, HIGH); 
                        delay(1000);
                        digitalWrite(relayPin2, LOW);
                        delay(1000); 
                      }
                    break;
                case 3:
                    Serial.println("Case 3 selected");
                      while(1){
                        int A = digitalRead(limitSwitch1);
                        Serial.println(A);
                      }
                    break;
                case 4:
                    Serial.println("Case 4 selected");
                      while(1){
                        int A = digitalRead(limitSwitch2);
                        Serial.println(A);
                      }
                    break;
                case 5:
                    Serial.println("Case 5 selected");
                      while(1){
                        int A = digitalRead(proximitySensorPin1);
                        Serial.println(A);
                      }
                    break;
                case 6:
                    Serial.println("Case 6 selected");
                      while(1){
                        int A = digitalRead(proximitySensorPin2);
                        Serial.println(A);
                      }
                    break;
                case 7:
                    Serial.println("Case 7 selected");
                    while(1){
                      digitalWrite(5, HIGH);
                    }
                    break;
                case 8:
                    Serial.println("Case 8 selected");
                    // Add case 5 code here
                    break;
                case 9:
                    Serial.println("Case 9 selected");
                    while(1){
                        digitalWrite(A0, HIGH);
                        digitalWrite(A1, HIGH);
                        digitalWrite(A2, HIGH);
                        digitalWrite(A3, HIGH);
                        digitalWrite(A4, HIGH);
                        digitalWrite(A5, HIGH);
                        digitalWrite(A6, HIGH);
                        digitalWrite(A7, HIGH);

                        digitalWrite(10, HIGH);
                        digitalWrite(11, HIGH);
                        digitalWrite(12, HIGH);
                        digitalWrite(13, HIGH);
                    }
                    break;
                default:
                    Serial.println("Invalid input. Enter a number between 1 and 5.");
                    userInput = 0;  // Reset input to keep waiting
                    break;
            }
        }
    }
}

void loop() {
    // Nothing needed here; the program starts based on the input in setup()
}
