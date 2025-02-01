// Intermediate Mega Code


void setup() {
  Serial.begin(9600);    // Communication with Central Mega
  Serial1.begin(9600);   // Communication with Nano 1
  Serial2.begin(9600);   // Communication with Nano 2
  Serial3.begin(9600);   // Communication with Nano 3
}

void loop() {
  // Forward messages from Central Mega to Nano
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    // Forward this command to Nano
    //Serial.println(command);

    if (command.equals("START")){
      //Serial1.println(command);
      //Serial2.println(command);
      //Serial3.println(command);
      //delay(50);
      Serial1.println(command);
      //delay(50);
      Serial2.println(command);
      //delay(50);
      Serial3.println(command);
    }

    if (command.equals("PAUSE")){
      //delay(50);
      Serial1.println(command);
      //delay(50);
      Serial2.println(command);
      //delay(50);
      Serial3.println(command);
    }

    if (command.equals("RESET")){
      Serial1.println(command);
      delay(5000);
      Serial2.println(command);
      delay(5000);
      Serial3.println(command);
      delay(5000);
    }

    if (command.equals("ONE_RESET_IN_PAUSE")){
      Serial1.println("RESET_IN_PAUSE");
      delay(5000);
    }

    if (command.equals("TWO_RESET_IN_PAUSE")){
      Serial2.println("RESET_IN_PAUSE");
      delay(5000);
    }

    if (command.equals("THREE_RESET_IN_PAUSE")){
      Serial3.println("RESET_IN_PAUSE");
      delay(5000);
    }
  }

  // Forward messages from Nano 1 to Central Mega
  if (Serial1.available() > 0) {
    String message = Serial1.readStringUntil('\n');
    message.trim();
    // Forward this message to Central Mega
    if (message.equals("DOWN")){
      Serial.println("ONE_DOWN");
    }
  }

  if (Serial2.available() > 0) {
    String message = Serial2.readStringUntil('\n');
    message.trim();
    // Forward this message to Central Mega
    if (message.equals("DOWN")){
      Serial.println("TWO_DOWN");
    }
  }

  if (Serial3.available() > 0) {
    String message = Serial3.readStringUntil('\n');
    message.trim();
    // Forward this message to Central Mega
    if (message.equals("DOWN")){
      Serial.println("THREE_DOWN");
    }
  }
}
