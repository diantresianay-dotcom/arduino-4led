#define button1 2
#define button2 3
#define potPin A1

#define led1 4
#define led2 5
#define led3 8
#define led4 10

bool led1State = false;           // Status LED1
bool led2State = true;            // Status LED2 (kebalikan dari LED1)
bool led3Blink = true;            // LED3 dalam mode blink
bool lastButton1State = HIGH;     // Status tombol 1 sebelumnya
bool lastButton2State = HIGH;     // Status tombol 2 sebelumnya
bool led3LightState = LOW;        // Status nyala/mati LED3
unsigned long prevMillis = 0;     // Timer untuk kedip LED3
const int blinkInterval = 300;    // Kecepatan kedip LED3

void setup() {
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  Serial.begin(9600);
  Serial.println("Program: 4 LED, 2 tombol toggle, dan 1 potensiometer");
}

void loop() {
  int stateButton1 = digitalRead(button1);
  int stateButton2 = digitalRead(button2);
  int potValue = analogRead(potPin);
  int brightness = potValue / 4; // ubah 0–1023 jadi 0–255

  // ===== Button 1: toggle LED1 dan LED2 =====
  if (stateButton1 == LOW && lastButton1State == HIGH) {
    led1State = !led1State;          // ubah status LED1
    led2State = !led2State;          // ubah status LED2 (kebalikannya)
    Serial.print("Button1 ditekan → LED1: ");
    Serial.print(led1State ? "ON" : "OFF");
    Serial.print(" | LED2: ");
    Serial.println(led2State ? "ON" : "OFF");
    delay(200); // debounce
  }
  lastButton1State = stateButton1;

  digitalWrite(led1, led1State);
  digitalWrite(led2, led2State);

  // ===== Button 2: toggle LED3 (blink mode) =====
  if (stateButton2 == LOW && lastButton2State == HIGH) {
    led3Blink = !led3Blink;
    if (led3Blink) {
      Serial.println("LED3: MODE KEDAP-KEDIP AKTIF ✅");
    } else {
      Serial.println("LED3: MATI ❌");
      digitalWrite(led3, LOW);
    }
    delay(200); // debounce
  }
  lastButton2State = stateButton2;

  // ===== LED3: mode kedip non-blocking =====
  if (led3Blink) {
    unsigned long currentMillis = millis();
    if (currentMillis - prevMillis >= blinkInterval) {
      prevMillis = currentMillis;
      led3LightState = !led3LightState;
      digitalWrite(led3, led3LightState);
    }
  }

  // ===== LED4: dikontrol potensiometer =====
  analogWrite(led4, brightness);

  // ===== Keterangan LED4 =====
  if (brightness < 100) {
    Serial.println("LED4: Terlalu redup");
  } else if (brightness > 200) {
    Serial.println("LED4: Terlalu terang");
  } else {
    Serial.println("LED4: Normal");
  }

  delay(50);
}

