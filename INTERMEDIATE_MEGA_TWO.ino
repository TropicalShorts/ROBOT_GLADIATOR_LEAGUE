// Intermediate Mega Code


void setup() {
  Serial.begin(9600);    // Communication with Central Mega
  Serial1.begin(9600);   // Communication with Nano 4
  Serial2.begin(9600);   // Communication with Nano 5
  Serial3.begin(9600);   // Communication with Nano 6
}

void loop() {
  // Forward messages from Central Mega to Nano
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    // Forward this command to Nano

    if (command.equals("START")){
      Serial1.println(command);
      Serial2.println(command);
      Serial3.println(command);
    }

    if (command.equals("PAUSE")){
      Serial1.println(command);
      Serial2.println(command);
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

    if (command.equals("FOUR_RESET_IN_PAUSE")){
      Serial1.println("RESET_IN_PAUSE");
      delay(5000);
    }

    if (command.equals("FIVE_RESET_IN_PAUSE")){
      Serial2.println("RESET_IN_PAUSE");
      delay(5000);
    }

    if (command.equals("SIX_RESET_IN_PAUSE")){
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
      Serial.println("FOUR_DOWN");
    }
  }

  if (Serial2.available() > 0) {
    String message = Serial2.readStringUntil('\n');
    message.trim();
    // Forward this message to Central Mega
    if (message.equals("DOWN")){
      Serial.println("FIVE_DOWN");
    }
  }

  if (Serial3.available() > 0) {
    String message = Serial3.readStringUntil('\n');
    message.trim();
    // Forward this message to Central Mega
    if (message.equals("DOWN")){
      Serial.println("SIX_DOWN");
    }
  }
}
