#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// -------- LCD --------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// -------- PINS --------
const int entrySensor = 3;
const int exitSensor  = 4;
const int servoPin    = 9;
const int buzzerPin   = 6;

// -------- SYSTEM --------
int slots = 4;
const int maxSlots = 4;

// -------- STATES --------
bool systemBusy = false;
unsigned long triggerTime = 0;

// timing
const unsigned long gateOpenTime = 2000;
const unsigned long cooldownTime = 6000;

// message system
String msg1 = "";
String msg2 = "";
bool showingMessage = false;
unsigned long msgStart = 0;
const unsigned long msgDuration = 1500;

// servo
Servo gate;

// -------- SETUP --------
void setup() {
  pinMode(entrySensor, INPUT);
  pinMode(exitSensor, INPUT);
  pinMode(buzzerPin, OUTPUT);

  gate.attach(servoPin);
  gate.write(0);

  Wire.begin();
  lcd.begin(16, 2);
  lcd.backlight();

  // welcome screen
  lcd.setCursor(1, 0);
  lcd.print("HI PRAJWAL");
  lcd.setCursor(2, 1);
  lcd.print("SUNNAGAR");
  delay(2000);

  updateLCD();
}

// -------- LOOP --------
void loop() {
  unsigned long now = millis();

  // message timing
  if (showingMessage && now - msgStart > msgDuration) {
    showingMessage = false;
    updateLCD();
  }

  renderMessage();

  // gate control
  if (systemBusy) {
    if (now - triggerTime >= gateOpenTime && now - triggerTime < cooldownTime) {
      gate.write(0);
    }

    if (now - triggerTime >= cooldownTime) {
      systemBusy = false;
      updateLCD();
    }

    return;
  }

  // sensor read
  bool entry = digitalRead(entrySensor);
  bool exit  = digitalRead(exitSensor);

  // ENTRY
  if (entry == LOW && slots > 0) {
    slots--;

    // 🔊 short beep
    tone(buzzerPin, 1000);
    delay(200);
    noTone(buzzerPin);

    showMessage("WELCOME", "");
    activateGate(now);
  }

  // EXIT
  else if (exit == LOW && slots < maxSlots) {
    slots++;

    // 🔊 small beep
    tone(buzzerPin, 1200);
    delay(150);
    noTone(buzzerPin);

    showMessage("THANK YOU", "");
    activateGate(now);
  }

  // FULL
  else if (entry == LOW && slots == 0) {

    // 🔊 long beep
    tone(buzzerPin, 500);
    delay(1000);
    noTone(buzzerPin);

    showMessage("SORRY", "PARKING FULL");
  }
}

// -------- GATE --------
void activateGate(unsigned long now) {
  gate.write(90);
  systemBusy = true;
  triggerTime = now;
}

// -------- DISPLAY --------

// show message
void showMessage(String line1, String line2) {
  msg1 = line1;
  msg2 = line2;
  msgStart = millis();
  showingMessage = true;
}

// render message (NO FLICKER)
void renderMessage() {
  if (!showingMessage) return;

  lcd.setCursor(0, 0);
  lcd.print("                ");

  lcd.setCursor(0, 1);
  lcd.print("                ");

  int pos1 = (16 - msg1.length()) / 2;
  int pos2 = (16 - msg2.length()) / 2;

  lcd.setCursor(pos1, 0);
  lcd.print(msg1);

  lcd.setCursor(pos2, 1);
  lcd.print(msg2);
}

// normal display (STABLE)
void updateLCD() {

  lcd.setCursor(0, 0);
  lcd.print("                ");

  lcd.setCursor(0, 1);
  lcd.print("                ");

  String line1 = "SLOTS=" + String(slots);
  int pos1 = (16 - line1.length()) / 2;

  lcd.setCursor(pos1, 0);
  lcd.print(line1);

  if (slots == 0) {
    lcd.setCursor(6, 1);
    lcd.print("FULL");
  } else {
    lcd.setCursor(3, 1);
    lcd.print("Available");
  }
}