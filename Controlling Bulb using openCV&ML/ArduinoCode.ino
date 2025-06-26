const int relayPin = 2; // Arduino pin connected to the relay
void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(relayPin, OUTPUT); // Set relay pin as output
  digitalWrite(relayPin, LOW); // Turn off the relay initially
}


void loop() {
  if (Serial.available() > 0) {
    int val = Serial.read();
    if (val == '0') { // If the data is 0, turn off the relay
      digitalWrite(relayPin, LOW);
      Serial.println("Relay OFF");
    } else if (val == '1') { // If the data is 1, turn on the relay
      digitalWrite(relayPin, HIGH);
      Serial.println("Relay ON");
    }
  }
}
