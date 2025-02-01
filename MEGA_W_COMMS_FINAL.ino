/*
Originally developed by Jackson McMillen and Garrett Cline
For use by Newton's Attic

Required Libraries to download:
LiquidCrystal I2C library by Marco Schwartz

*/

#include <LiquidCrystal_I2C.h>

// Initialize the LCD displays
LiquidCrystal_I2C lcd1(0x25,20,4);  // LCD 1
LiquidCrystal_I2C lcd2(0x27,20,4);  // LCD 2
LiquidCrystal_I2C lcd3(0x26,20,4);  // LCD 3

// Pin definitions
const int resetTargetButton1 = 36;  //Button 1, 5-1
const int resetTargetButton2 = 35;  //Button 2 , 5-2
const int resetTargetButton3 = 34;
const int resetTargetButton4 = 33;
const int resetTargetButton5 = 32;
const int resetTargetButton6 = 31;

//Button for immediate round over
const int immediateRoundEnd = 28;

// Variables for debounced add points to team 1
const int addPointsT1 = 30; // 7, 1-3
int buttonStateAddPointsT1 = LOW;
int lastButtonStateAddPointsT1 = LOW;
unsigned long lastDebounceTimeAddPointsT1 = 0;
const unsigned long debounceDelayAddPointsT1 = 50; // Adjust the delay as needed

// Variables for debounced remove points from Team 1
const int removePointsT1 = 27; // Pin connected to the remove points button for Team 1
int buttonStateRemovePointsT1 = LOW;
int lastButtonStateRemovePointsT1 = LOW;
unsigned long lastDebounceTimeRemovePointsT1 = 0;
const unsigned long debounceDelayRemovePointsT1 = 50; // Debounce delay in milliseconds

// Variables for debounced add points to Team 2
const int addPointsT2 = 29; // Pin connected to the add points button for Team 2
int buttonStateAddPointsT2 = LOW;
int lastButtonStateAddPointsT2 = LOW;
unsigned long lastDebounceTimeAddPointsT2 = 0;
const unsigned long debounceDelayAddPointsT2 = 50; // Debounce delay in milliseconds

// Variables for debounced remove points from Team 2
const int removePointsT2 = 26; // Pin connected to the remove points button for Team 2
int buttonStateRemovePointsT2 = LOW;
int lastButtonStateRemovePointsT2 = LOW;
unsigned long lastDebounceTimeRemovePointsT2 = 0;
const unsigned long debounceDelayRemovePointsT2 = 50; // Debounce delay in milliseconds

const int resume = 23;  //  14, 1-2

//Variables for debonuced add time
const int incrTime5Button = 25; //12, 3-2
int buttonStateIT5 = LOW;
int lastButtonStateIT5 = LOW;
unsigned long lastDebounceTimeIT5 = 0; // To track the time since the last button press
unsigned long debounceDelayIT5 = 50;   // Debounce delay in milliseconds

//Variables for debounced remove time
const int decrTime5Button = 22; // 15, 3-1
int buttonStateDT5 = LOW;
int lastButtonStateDT5 = LOW;
unsigned long lastDebounceTimeDT5 = 0; // To track the time since the last button press
unsigned long debounceDelayDT5 = 50;   // Debounce delay in milliseconds

const int instantRoundOver = 39;

const int startStopButton = 24; //Button 13, 1 - 1

//Variables used to contain time
unsigned long currentTime = 0;  //Used for timing when pin is knocked down 
unsigned long startTime = 0;
unsigned long state3time = 0;
unsigned long elapsedTime = 0;

//Variables used to contain team scores
int team1Score = 0;
int team2Score = 0;

bool paused = false;                        //Bool for if we are paused
bool firstCheckDisplayState1 = true;        //Used for debugging     
bool needToResetTime = false;               //Needed when a round ends by target elimination

int currentState = 0;                       //Initial state for finite state machine

bool pinState1 = true;                      //State of targets (bowling pins)
bool pinState2 = true;
bool pinState3 = true;
bool pinState4 = true;
bool pinState5 = true;
bool pinState6 = true;

void checkNanoMessages() {
  // Check if data is available from the Nano
  if (Serial3.available() > 0) {
    // Read the incoming string until a newline character
    String message = Serial3.readStringUntil('\n');
    message.trim();  // Remove any whitespace

    if (message.equals("ONE_DOWN")) {
      pinState1 = false;
    }
    if (message.equals("TWO_DOWN")) {
      pinState2 = false;
    }
    if (message.equals("THREE_DOWN")) {
      pinState3 = false;
    }
  }

  if (Serial2.available() > 0) {
    // Read the incoming string until a newline character
    String message = Serial2.readStringUntil('\n');
    message.trim();  // Remove any whitespace

    if (message.equals("FOUR_DOWN")) {
      pinState4 = false;
    }
    if (message.equals("FIVE_DOWN")) {
      pinState5 = false;
    }
    if (message.equals("SIX_DOWN")) {
      pinState6 = false;
    }
  }
}

void increaseTimeBy5(int &countdown){
  
  int reading = digitalRead(incrTime5Button);
  // If the button state has changed
  if (reading != lastButtonStateIT5) {
    lastDebounceTimeIT5 = millis();  // Reset the debounce timer
  }
  // Only trigger the action if the button has been in the same state for the debounce period
  if ((millis() - lastDebounceTimeIT5) > debounceDelayIT5) {
    // If the current reading is HIGH and it was previously LOW, the button was pressed
    if (reading == HIGH && buttonStateIT5 == LOW) {
      countdown = countdown + 5;
    }
    buttonStateIT5 = reading; // Update the current button state
  }
  lastButtonStateIT5 = reading;
}

void decreaseTimeBy5(int &countdown){
    
  int reading = digitalRead(decrTime5Button);
  // If the button state has changed
  if (reading != lastButtonStateDT5) {
    lastDebounceTimeDT5 = millis();  // Reset the debounce timer
  }
  // Only trigger the action if the button has been in the same state for the debounce period
  if ((millis() - lastDebounceTimeDT5) > debounceDelayDT5) {
    // If the current reading is HIGH and it was previously LOW, the button was pressed
    if (reading == HIGH && buttonStateDT5 == LOW) {
      if (countdown > 6){
        countdown = countdown - 5;
      }
    }
    buttonStateDT5 = reading; // Update the current button state
  }
  lastButtonStateDT5 = reading;
}

void increaseScoreT1(int &team1Score){
  int reading = digitalRead(addPointsT1);
  // If the button state has changed
  if (reading != lastButtonStateAddPointsT1) {
    lastDebounceTimeAddPointsT1 = millis();  // Reset the debounce timer
  }
  // Only trigger the action if the button has been in the same state for the debounce period
  if ((millis() - lastDebounceTimeAddPointsT1) > debounceDelayAddPointsT1) {
    // If the current reading is HIGH and it was previously LOW, the button was pressed
    if (reading == HIGH && buttonStateAddPointsT1 == LOW) {
      team1Score++;
      lcd3.setCursor(0, 2);
      lcd3.print("Home score: ");
      lcd3.print(team1Score);
    }
    buttonStateAddPointsT1 = reading; // Update the current button state
  }
  lastButtonStateAddPointsT1 = reading;
}

void decreaseScoreT1(int &team1Score){
  int reading = digitalRead(removePointsT1);
  
  // If the button state has changed
  if (reading != lastButtonStateRemovePointsT1) {
    lastDebounceTimeRemovePointsT1 = millis();  // Reset the debounce timer
  }
  
  // Only trigger the action if the button has been in the same state for the debounce period
  if ((millis() - lastDebounceTimeRemovePointsT1) > debounceDelayRemovePointsT1) {
    // If the current reading is HIGH and it was previously LOW, the button was pressed
    if (reading == HIGH && buttonStateRemovePointsT1 == LOW) {
      if (team1Score > 0) { // Prevent negative scores
        team1Score--;
        lcd3.setCursor(0, 2);
        lcd3.print("Home score: ");
        lcd3.print(team1Score);
      }
    }
    buttonStateRemovePointsT1 = reading; // Update the current button state
  }
  lastButtonStateRemovePointsT1 = reading;
}

void increaseScoreT2(int &team2Score){
  int reading = digitalRead(addPointsT2);
  
  // If the button state has changed
  if (reading != lastButtonStateAddPointsT2) {
    lastDebounceTimeAddPointsT2 = millis();  // Reset the debounce timer
  }
  
  // Only trigger the action if the button has been in the same state for the debounce period
  if ((millis() - lastDebounceTimeAddPointsT2) > debounceDelayAddPointsT2) {
    // If the current reading is HIGH and it was previously LOW, the button was pressed
    if (reading == HIGH && buttonStateAddPointsT2 == LOW) {
      team2Score++;
      lcd3.setCursor(0, 3); // Adjust the cursor position as needed
      lcd3.print("Away score: ");
      lcd3.print(team2Score);
    }
    buttonStateAddPointsT2 = reading; // Update the current button state
  }
  lastButtonStateAddPointsT2 = reading;
}

void decreaseScoreT2(int &team2Score){
  int reading = digitalRead(removePointsT2);
  
  // If the button state has changed
  if (reading != lastButtonStateRemovePointsT2) {
    lastDebounceTimeRemovePointsT2 = millis();  // Reset the debounce timer
  }
  
  // Only trigger the action if the button has been in the same state for the debounce period
  if ((millis() - lastDebounceTimeRemovePointsT2) > debounceDelayRemovePointsT2) {
    // If the current reading is HIGH and it was previously LOW, the button was pressed
    if (reading == HIGH && buttonStateRemovePointsT2 == LOW) {
      if (team2Score > 0) { // Prevent negative scores
        team2Score--;
        lcd3.setCursor(0, 3); // Adjust the cursor position as needed
        lcd3.print("Away score: ");
        lcd3.print(team2Score);
      }
    }
    buttonStateRemovePointsT2 = reading; // Update the current button state
  }
  lastButtonStateRemovePointsT2 = reading;
}


void initialize_displays(){
  lcd1.init();
  lcd1.clear();         
  lcd1.backlight();      // Make sure backlight is on
  lcd2.init();
  lcd2.clear();         
  lcd2.backlight();      // Make sure backlight is on
  lcd3.init();
  lcd3.clear();         
  lcd3.backlight();      // Make sure backlight is on
}

void reset_displays(){
  lcd3.clear();
  lcd1.clear();
  lcd2.clear();
  lcd1.setCursor(0,0);   //Set cursor to character 0 on line 0
  lcd1.print("Home");
  lcd1.setCursor(0,1);   //Set cursor to character 0 on line 0
  lcd1.print("Target 1: UP");
  lcd1.setCursor(0,2);   //Move cursor to character 0 on line 1
  lcd1.print("Target 2: UP");
  lcd1.setCursor(0,3);   //Move cursor to character 0 on line 2
  lcd1.print("Target 3: UP");
  lcd2.setCursor(0,0);   //Set cursor to character 0 on line 0
  lcd2.print("Away");
  lcd2.setCursor(0,1);   //Set cursor to character 0 on line 0
  lcd2.print("Target 4: UP");
  lcd2.setCursor(0,2);   //Move cursor to character 0 on line 1
  lcd2.print("Target 5: UP");
  lcd2.setCursor(0,3);   //Move cursor to character 0 on line 2
  lcd2.print("Target 6: UP");
}

//Setup lcd boards and buttons and attach sensors to digital pins
void setup() {
  Serial.begin(9600);
  Serial3.begin(9600); // Initialize Serial1 for communication with Nano
  Serial2.begin(9600);

  initialize_displays();

  reset_displays();

  // Remove sensorTarget pin modes and interrupts

  pinMode(resetTargetButton1, INPUT);
  pinMode(resetTargetButton2, INPUT);
  pinMode(resetTargetButton3, INPUT);
  pinMode(resetTargetButton4, INPUT);
  pinMode(resetTargetButton5, INPUT);
  pinMode(resetTargetButton6, INPUT);

  pinMode(resume, INPUT);
  pinMode(incrTime5Button, INPUT);
  pinMode(decrTime5Button, INPUT);
  pinMode(instantRoundOver, INPUT);

  pinMode(startStopButton, INPUT);
}

void loop() {
  checkNanoMessages(); // Check for messages from Nano

  switch(currentState) {
    case 0:                                         // Waiting for button input to begin the game
      if (firstCheckDisplayState1){
        firstCheckDisplayState1 = false;
      }
      lcd3.setCursor(0,0);
      lcd3.print("Waiting to begin");
      if (digitalRead(startStopButton) == HIGH) {   // If button is pressed begin round
        
        Serial3.println("START");                   // Send START command to Nano
        Serial2.println("START");
        Serial.println("START");

        lcd3.clear();
        lcd3.print("Beginning round!");
        currentState = 1;  
        startTime = millis();
        delay(1000);
        lcd3.clear();
        lcd3.print("Game on!");
      }
      break;
      
    case 1:  // State 1: Game in progress
      static unsigned long previousMillis = 0;    //Used for second countdown
      static int countdown = 10000;                  //10 seconds is 11
      if(needToResetTime){
        countdown = 10000;
        needToResetTime = false;
      }
      while (countdown > 0 && !paused) {
        if(digitalRead(startStopButton)==HIGH){
          
          Serial3.println("PAUSE");              // Send PAUSE command to Nano
          Serial2.println("PAUSE"); 
          Serial.println("PAUSE"); 

          lcd3.setCursor(0,2);
          lcd3.print("Paused");
          currentState = 2;
          paused = true;
        }

        if(digitalRead(immediateRoundEnd)==HIGH){
          if(pinState1 == false){
            team1Score++;
          }
          if(pinState2 == false){
            team1Score++;
          }
          if(pinState3 == false){
            team1Score++;
          }
          if(pinState4 == false){
            team2Score++;
          }
          if(pinState5 == false){
            team2Score++;
          }
          if(pinState6 == false){
            team2Score++;
          }
          currentState = 3;
          lcd3.setCursor(0,3);
          lcd3.print("Resume to reset");
          paused = true;
          needToResetTime = true;
        }

        checkNanoMessages(); // Check for messages from Nano, inside while loop so need to call again
        
        //Increase time by 5 seconds formula
        increaseTimeBy5(countdown);

        //Decrease time by 5 seconds formula
        decreaseTimeBy5(countdown);

        if(pinState1 == false){
          lcd1.setCursor(0,1);
          lcd1.print("Target 1: DOWN");
        }

        if(pinState2 == false){
          lcd1.setCursor(0,2);
          lcd1.print("Target 2: DOWN");  
        }

        if(pinState3 == false){
          lcd1.setCursor(0,3);
          lcd1.print("Target 3: DOWN");
        }

        if(pinState4 == false){
          lcd2.setCursor(0,1);
          lcd2.print("Target 4: DOWN");
        }

        if(pinState5 == false){
          lcd2.setCursor(0,2);
          lcd2.print("Target 5: DOWN");
        }

        if(pinState6 == false){
          lcd2.setCursor(0,3);
          lcd2.print("Target 6: DOWN");
        }
      

        // Check for team wins
        if(pinState1 == false && pinState2 == false && pinState3 == false){
          currentState = 3;
          state3time = millis();
          countdown = 0;
          needToResetTime = true;
          lcd3.clear();
          lcd3.print("Round over!");
          lcd3.setCursor(0,1);
          lcd3.print("Home wins!!!");
          team1Score = team1Score + 1;
          lcd3.setCursor(0,2);
          lcd3.print(team1Score);
          lcd3.setCursor(0,3);
          lcd3.print("Resume to reset");
          delay(5000);
        }

        if(pinState4 == false && pinState5 == false && pinState6 == false){
          currentState = 3;
          state3time = millis();
          countdown = 0;
          needToResetTime = true;
          lcd3.clear();
          lcd3.print("Round over!");
          lcd3.setCursor(0,1);
          lcd3.print("Away wins!!!");
          team2Score = team2Score + 1;
          lcd3.setCursor(0,2);
          lcd3.print(team2Score);
          lcd3.setCursor(0,3);
          lcd3.print("Resume to reset");
          delay(5000);
        }

        //Display countdown and decrease it
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= 1000 && !paused) { // Check if 1 second has passed
            previousMillis = currentMillis;
            lcd3.clear();
            lcd3.setCursor(0,0);
            lcd3.print("Game on!");
            lcd3.setCursor(0,1);
            lcd3.print(countdown - 1);
            countdown--; // Decrement countdown
        }
      }
      elapsedTime = millis() - startTime; 
      if (elapsedTime >= 10000000 && !paused) {                   // If 10 seconds have passed
          lcd3.clear();
          lcd3.print("Round over!");
          lcd3.setCursor(0,3);
          lcd3.print("Resume to reset");

          if(pinState1 == false){
            team1Score++;
          }
          if(pinState2 == false){
            team1Score++;
          }
          if(pinState3 == false){
            team1Score++;
          }
          if(pinState4 == false){
            team2Score++;
          }
          if(pinState5 == false){
            team2Score++;
          }
          if(pinState6 == false){
            team2Score++;
          }

          currentState = 3;                         // Move to state 3
          state3time = millis();                     // Record the start time for the next state
          countdown = 10000;                             // normally 11
      }
      break;
      
    case 2:

      //Increase time by 5 seconds formula
      increaseTimeBy5(countdown);

      //Decrease time by 5 seconds formula
      decreaseTimeBy5(countdown);

      if(digitalRead(resetTargetButton1)==HIGH){
        Serial3.println("ONE_RESET_IN_PAUSE");                // Send RESET command to Nano
        pinState1 = true;
        lcd1.setCursor(0,1);
        lcd1.print("Target 1: UP  ");
        delay(5000);                             // Wait for Nano to reset
      }

      if(digitalRead(resetTargetButton2)==HIGH){
        Serial3.println("TWO_RESET_IN_PAUSE");                // Send RESET command to Nano
        pinState2 = true;
        lcd1.setCursor(0,2);
        lcd1.print("Target 2: UP  ");
        delay(5000);                             // Wait for Nano to reset
      }

      if(digitalRead(resetTargetButton3)==HIGH){
        Serial3.println("THREE_RESET_IN_PAUSE");                // Send RESET command to Nano
        pinState3 = true;
        lcd1.setCursor(0,3);
        lcd1.print("Target 3: UP  ");
        delay(5000);                             // Wait for Nano to reset
      }
      
      if(digitalRead(resetTargetButton4)==HIGH){
        Serial2.println("FOUR_RESET_IN_PAUSE");                // Send RESET command to Nano
        pinState4 = true;
        lcd2.setCursor(0,1);
        lcd2.print("Target 4: UP  ");
        delay(5000);                             // Wait for Nano to reset
      }

      if(digitalRead(resetTargetButton5)==HIGH){
        Serial2.println("FIVE_RESET_IN_PAUSE");                // Send RESET command to Nano
        pinState5 = true;
        lcd2.setCursor(0,2);
        lcd2.print("Target 5: UP  ");
        delay(5000);                             // Wait for Nano to reset
      }

      if(digitalRead(resetTargetButton6)==HIGH){
        Serial2.println("SIX_RESET_IN_PAUSE");                // Send RESET command to Nano
        pinState6 = true;
        lcd2.setCursor(0,3);
        lcd2.print("Target 6: UP  ");
        delay(5000);                             // Wait for Nano to reset
      }

      if(digitalRead(resume)==HIGH){
        paused = false;
        Serial3.println("START");               // Send START command to Nano
        Serial2.println("START");
        currentState = 1;
        //delay(1000);
      }
      break;

    case 3:

      paused = false;
      
      if(digitalRead(resume)==HIGH){
        lcd3.clear();
        currentState = 4;
        delay(1000);
      }

      break;

    case 4: 
      lcd3.setCursor(0,2);
      lcd3.print("Resetting pins");

      Serial3.println("RESET");                // Send RESET command to Nano 1
      Serial2.println("RESET");                 // Send RESET command to Nano 2 5 seconds
      delay(5000);       

      pinState1 = true;                        // reset pinstate1 to be true after reset
      pinState2 = true;
      pinState3 = true;
      pinState4 = true;
      pinState5 = true;
      pinState6 = true;

      reset_displays();

      lcd3.clear();
      lcd3.setCursor(0,0);
      lcd3.print("Adjust Scores");
      lcd3.setCursor(0,1);
      lcd3.print("Press resume to cont.");
      lcd3.setCursor(0,2);
      lcd3.print("Home score: ");
      lcd3.print(team1Score);
      lcd3.setCursor(0,3);
      lcd3.print("Away score: ");
      lcd3.print(team2Score);

      currentState = 5;
      break;

    case 5:

      int addPointsT1ButtonState = digitalRead(addPointsT1);
      
      if(digitalRead(resume)==HIGH){
        lcd3.clear();
        currentState = 0;
        delay(1000);
      }

      increaseScoreT1(team1Score);
      decreaseScoreT1(team1Score);
      increaseScoreT2(team2Score);
      decreaseScoreT2(team2Score);

      //Code to adjust score, buttons for + and - for each team      
      break;

      

  }
}