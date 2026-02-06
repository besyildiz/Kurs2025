#define LED_PIN 2

void setup() {
  Serial.begin(115200);         // Seri haberleşmeyi başlat
  pinMode(LED_PIN, OUTPUT);
  Serial.println("ESP32 LED Test Basladi!");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED ON");
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED OFF");
  delay(1000);
}