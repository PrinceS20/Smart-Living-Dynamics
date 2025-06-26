#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// Pin assignments
const int entrySensorPin = 2; // Pin for entry sensor
const int exitSensorPin = 3;  // Pin for exit sensor
const int relayPin = 4;       // Pin for controlling the relay


// Variables to store counts and sensor states
int peopleCount = 0;
bool entryState = false;
bool exitState = false;


// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change address if needed


void setup() {
  // Initialize serial communication
  Serial.begin(9600);
 
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
 
  // Set pin modes
  pinMode(entrySensorPin, INPUT_PULLUP);
  pinMode(exitSensorPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
 
  // Initially turn off the bulb
  digitalWrite(relayPin, LOW);
 
  // Display initial count on LCD
  updateLCD();
}


void loop() {
  // Read the state of entry and exit sensors
  bool entryReading = digitalRead(entrySensorPin);
  bool exitReading = digitalRead(exitSensorPin);
 
  // Debouncing and hysteresis for entry sensor
  if (entryReading != entryState) {
    delay(10); // Debounce delay
    entryReading = digitalRead(entrySensorPin);
    if (entryReading != entryState) {
      entryState = entryReading;
      if (entryState == LOW) {
        // Increment people count
        peopleCount++;
        Serial.println("Person entered. Count: " + String(peopleCount));
        updateLCD();
      }
    }
  }
 
  // Debouncing and hysteresis for exit sensor
  if (exitReading != exitState) {
    delay(10); // Debounce delay
    exitReading = digitalRead(exitSensorPin);
    if (exitReading != exitState) {
      exitState = exitReading;
      if (exitState == LOW) {
        // Decrement people count
        peopleCount--;
        // Make sure people count doesn't go negative
        if (peopleCount < 0) {
          peopleCount = 0;
        }
        Serial.println("Person left. Count: " + String(peopleCount));
        updateLCD();
      }
    }
  }
 
  // Control the relay based on people count
  if (peopleCount > 0) {
    digitalWrite(relayPin, HIGH); // Turn on the bulb
  } else {
    digitalWrite(relayPin, LOW);  // Turn off the bulb
  }
}


// Function to update LCD with current people count
void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("People Count: ");
  lcd.print(peopleCount);
}
