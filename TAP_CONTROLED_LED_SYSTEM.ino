#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define touchPin 2
#define redPin 3
#define greenPin 4
#define bluePin 5
#define buzzerPin 6

LiquidCrystal_I2C lcd(0x27, 16, 2); // Use 0x3F if 0x27 doesn't work

int touchCount = 0;
unsigned long lastTouchTime = 0;
const unsigned long waitTime = 1000; // 1 second

bool lastTouchState = LOW;

void setup() {
  pinMode(touchPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);

  // Step 1: Show startup message
  lcd.setCursor(0, 0);
  lcd.print("TAP BASED SYSTEM");
  lcd.setCursor(0, 1);
  lcd.print("   STARTING...  ");
  Serial.println("System Starting...");
  delay(2000);

  // Step 2: Show initial instruction
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tap To Toggle");
  lcd.setCursor(0, 1);
  lcd.print("  The LED     ");
}

void loop() {
  bool touchState = digitalRead(touchPin);

  // Rising edge detection
  if (touchState == HIGH && lastTouchState == LOW) {
    touchCount++;
    lastTouchTime = millis();
    Serial.print("Touch Count: ");
    Serial.println(touchCount);
  }

  lastTouchState = touchState;

  if (touchCount > 0 && (millis() - lastTouchTime > waitTime)) {
    performAction(touchCount);
    touchCount = 0;
  }
}

void performAction(int count) {
  // Turn off all LEDs
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
  lcd.clear();

  switch (count) {
    case 1:
      digitalWrite(redPin, HIGH);
      lcd.print("Single Tap: RED");
      Serial.println("🔴 RED LED ON");
      break;
    case 2:
      digitalWrite(greenPin, HIGH);
      lcd.print("Double Tap: GREEN");
      Serial.println("🟢 GREEN LED ON");
      break;
    case 3:
      digitalWrite(bluePin, HIGH);
      lcd.print("Triple Tap: BLUE");
      Serial.println("🔵 BLUE LED ON");
      break;
    default:
      lcd.print("Invalid Taps");
      Serial.println("Invalid Tap Count");
      return;
  }

  // Buzzer beep twice
  for (int i = 0; i < 2; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(150);
    digitalWrite(buzzerPin, LOW);
    delay(150);
  }
}
