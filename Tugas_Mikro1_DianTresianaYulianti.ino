#define button1 2       // Tombol 1
#define button2 3       // Tombol 2
#define potPin A1       // Potensiometer

#define led1 4          // LED 1
#define led2 5          // LED 2
#define led3 8          // LED 3 (kedip jika tombol 2 ditekan)
#define led4 10         // LED 4 (atur kecerahan via potensiometer, gunakan pin PWM)

void setup() {
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  Serial.begin(9600);
  Serial.println("Program: 4 LED, 2 Push Button, 1 Potensiometer");
}

void loop() {
  int stateButton1 = digitalRead(button1);
  int stateButton2 = digitalRead(button2);
  int potValue = analogRead(potPin);
  int brightness = potValue / 4;  // ubah range dari 0–1023 menjadi 0–255

  // --- Tombol 1: menyalakan LED1 dan LED2 ---
  if (stateButton1 == LOW) {  // tombol ditekan (aktif LOW)
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    delay(50);
  } else {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    delay(50);
  }

  // --- Tombol 2: membuat LED3 berkedip ---
  if (stateButton2 == LOW) {
    digitalWrite(led3, HIGH);
    delay(200);
    digitalWrite(led3, LOW);
    delay(200);
  } else {
    digitalWrite(led3, LOW);
  }

  // --- LED4 dikontrol oleh potensiometer ---
  analogWrite(led4, brightness);

  // --- Tampilkan pesan jika LED4 terlalu redup ---
   if (brightness < 100) {
    Serial.println("LED4 terlalu redup");
  } 
  else if (brightness > 200) {
    Serial.println("LED4 terlalu terang");
  } 
  else {
    Serial.println("LED4 dalam kondisi normal");
  }

  delay(100); // jeda agar pesan di Serial Monitor tidak terlalu cepat
}
