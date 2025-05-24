#include <LiquidCrystal.h>
#include <Servo.h>

// LCD RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// PIR input pins for availability toggle
const int pirSlot1Pin = 7;
const int pirSlot2Pin = 8;

// PIR input pins for servo control (opposite as before)
const int pirServo1Pin = 13;  // Controls servo1 (slot 1 gate)
const int pirServo2Pin = 6;   // Controls servo2 (slot 2 gate)

// Servo objects and pins
Servo servo1;
Servo servo2;
const int servo1Pin = 9;
const int servo2Pin = 10;

// Slot availability flags
bool slot1Free = true;
bool slot2Free = true;

// Previous PIR states for edge detection (to detect new motion)
bool lastPirSlot1State = LOW;
bool lastPirSlot2State = LOW;

void setup() {
  // LCD
  lcd.begin(16, 2);

  // Welcome message centered
  lcd.clear();
  lcd.setCursor(1, 0); lcd.print("Smart Parking");
  lcd.setCursor(5, 1); lcd.print("System");
  delay(2000);

  // Scroll the project title
  String msg = "[Project by Saptarshi Bose] ";
  
  // Append 16 spaces properly
  for (int i = 0; i < 16; i++) {
    msg += ' ';
  }

  for (int i = 0; i < msg.length() - 15; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(msg.substring(i, i + 16));
    delay(250);
  }

  // Initial slot display
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Slot 1: A");
  lcd.setCursor(0, 1); lcd.print("Slot 2: A");

  // Setup PIR pins
  pinMode(pirSlot1Pin, INPUT);
  pinMode(pirSlot2Pin, INPUT);
  pinMode(pirServo1Pin, INPUT);
  pinMode(pirServo2Pin, INPUT);

  // Attach servos
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Set initial servo position (closed)
  servo1.write(0);
  servo2.write(0);
}

void loop() {
  // Read current PIR states for availability toggling
  bool currentPirSlot1State = digitalRead(pirSlot1Pin);
  bool currentPirSlot2State = digitalRead(pirSlot2Pin);

  // Detect rising edge on PIR slot 1 pin to toggle slot1Free
  if (currentPirSlot1State == HIGH && lastPirSlot1State == LOW) {
    slot1Free = !slot1Free; // toggle slot 1 availability
  }
  lastPirSlot1State = currentPirSlot1State;

  // Detect rising edge on PIR slot 2 pin to toggle slot2Free
  if (currentPirSlot2State == HIGH && lastPirSlot2State == LOW) {
    slot2Free = !slot2Free; // toggle slot 2 availability
  }
  lastPirSlot2State = currentPirSlot2State;

  // Servo1 responds to PIR on pin 13 (open if HIGH, else closed)
  if (digitalRead(pirServo1Pin) == HIGH) {
    servo1.write(90); // Open gate for Slot 1
  } else {
    servo1.write(0);  // Close gate when no detection
  }

  // Servo2 responds to PIR on pin 6, but only if at least one slot is free
  if (digitalRead(pirServo2Pin) == HIGH && (slot1Free || slot2Free)) {
    servo2.write(90); // Open gate for Slot 2
  } else {
    servo2.write(0);  // Close gate when no detection or both slots filled
  }

  // Update LCD text with current slot availability
  lcd.setCursor(8, 0);
  lcd.print(slot1Free ? "A " : "NA");
  lcd.setCursor(8, 1);
  lcd.print(slot2Free ? "A " : "NA");

  delay(200);
}