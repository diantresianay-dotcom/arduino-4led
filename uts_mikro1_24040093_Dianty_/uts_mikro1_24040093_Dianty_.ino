#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo ventServo;

#define ledAuto 4
#define ledClosed 6
#define ledLamp 5
#define btnPower 2
#define btnMode  3
#define potPin   A1

bool manualMode = false, systemOn = false, valveOpen = false;

int lastBtnMode = HIGH, lastBtnPower = HIGH;
unsigned long lastDebMode = 0, lastDebPower = 0, debounceDelay = 120;
unsigned long lastAuto = 0, autoDelay = 1500;

unsigned long lastPot = 0;
unsigned long potDelay = 200;

void setup() {
  Serial.begin(9600);

  pinMode(ledAuto, OUTPUT);
  pinMode(ledClosed, OUTPUT);
  pinMode(ledLamp, OUTPUT);
  pinMode(btnPower, INPUT_PULLUP);
  pinMode(btnMode, INPUT_PULLUP);

  ventServo.attach(9);
  ventServo.write(0);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print(" Smart AirFlow ");
  lcd.setCursor(0,1);
  lcd.print("   Starting...");
  delay(1200);

  // TAMPILAN MODE AWAL
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("[AUTO MODE]  ");
  lcd.setCursor(0,1);
  lcd.print("AUTO RUNNING..");
}


void loop() {

  unsigned long now = millis();

  int pwr  = digitalRead(btnPower);
  int mode = digitalRead(btnMode);

  // ======================
  // TOMBOL POWER
  // ======================
  if (pwr != lastBtnPower && now - lastDebPower > debounceDelay) {
    lastDebPower = now;

    if (pwr == LOW) {
      systemOn = !systemOn;
      Serial.println(systemOn ? "SYSTEM ON" : "SYSTEM OFF");

      lcd.clear();
      if (systemOn) {
        lcd.setCursor(0,0);
        lcd.print(manualMode ? "[MANUAL MODE]" : "[AUTO MODE]  ");
        lcd.setCursor(0,1);
        lcd.print(manualMode ? "Servo: ---    " : "AUTO RUNNING..");
      }
    }
  }
  lastBtnPower = pwr;

  // ======================
  // JIKA SISTEM MATI
  // ======================
  if (!systemOn) {
    digitalWrite(ledAuto, LOW);
    digitalWrite(ledClosed, LOW);
    analogWrite(ledLamp, 0);
    ventServo.write(0);

    lcd.setCursor(0,0);
    lcd.print("   SYSTEM OFF   ");
    lcd.setCursor(0,1);
    lcd.print("Press Power Btn ");
    return;
  }


  // ======================
  // TOMBOL MODE
  // ======================
  if (mode != lastBtnMode && now - lastDebMode > debounceDelay) {
    lastDebMode = now;

    if (mode == LOW) {
      manualMode = !manualMode;
      Serial.println(manualMode ? "MANUAL" : "AUTO");

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(manualMode ? "[MANUAL MODE]" : "[AUTO MODE]  ");

      lcd.setCursor(0,1);
      lcd.print(manualMode ? "Servo: ---    " : "AUTO RUNNING..");
    }
  }
  lastBtnMode = mode;
  int pot = analogRead(potPin);
  // ======================
  // MODE MANUAL
  // ======================
  if (manualMode) {
    digitalWrite(ledAuto, LOW);
    digitalWrite(ledClosed, LOW);

    int lampPWM = map(pot, 0, 1023, 0, 255);
    int pos     = map(pot, 0, 1023, 0, 180);

    analogWrite(ledLamp, lampPWM);
    ventServo.write(pos);

    lcd.setCursor(0,1);
    lcd.print("Servo:");
    lcd.print(pos);
    lcd.print("   ");

    if (now - lastPot >= potDelay) {
      lastPot = now;
      Serial.print("[MANUAL] Pot="); Serial.print(pot);
      Serial.print(" | Servo="); Serial.print(pos);
      Serial.print(" | PWM="); Serial.println(lampPWM);
    }
  }

  // ======================
  // MODE AUTO
  // ======================
  else {
    analogWrite(ledLamp, 0);

    if (now - lastAuto >= autoDelay) {
      lastAuto = now;
      valveOpen = !valveOpen;

      if (valveOpen) {
        ventServo.write(90);
        digitalWrite(ledAuto, 1);
        digitalWrite(ledClosed, 0);

        lcd.setCursor(0,1);
        lcd.print("Valve: OPEN   ");

        Serial.println("[AUTO] VALVE OPEN");
      }
      else {
        ventServo.write(0);
        digitalWrite(ledAuto, 0);
        digitalWrite(ledClosed, 1);

        lcd.setCursor(0,1);
        lcd.print("Valve: CLOSED ");

        Serial.println("[AUTO] VALVE CLOSED");
      }
    }
  }
}
