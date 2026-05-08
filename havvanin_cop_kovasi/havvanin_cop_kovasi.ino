#include <Servo.h>

// Pin Tanımlamaları
const int trigPin = 7;
const int echoPin = 6;
const int servoPin = 9;

Servo motor;

// --- AYARLAR ---
int engelMesafesi = 20;   // Kaç cm'de kapak açılsın?

// 180 derece için yaklaşık süreler (Mekanizmana göre bu değerleri değiştir!)
int acilmaSuresi = 900;   // Kapak açılırken (yerçekimine karşı) gereken süre
int kapanmaSuresi = 850;  // Kapak kapanırken gereken süre (genelde daha kısa sürer)

bool kapakAcikMi = false;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  motor.attach(servoPin);
  
  motor.write(90); // Başlangıçta motoru durdur
  Serial.begin(9600);
  Serial.println("Sistem Hazir...");
}

void loop() {
  long sure, mesafe;

  // Mesafe ölçümü
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  sure = pulseIn(echoPin, HIGH, 30000); 
  mesafe = (sure / 2) / 29.1;

  // Seri ekrandan izlemek için
  if (mesafe > 0) {
    Serial.print("Mesafe: ");
    Serial.print(mesafe);
    Serial.println(" cm");
  }

  // ENGEL ALGILANDIĞINDA (KAPAK AÇILIYOR)
  if (mesafe < engelMesafesi && mesafe > 0 && !kapakAcikMi) {
    Serial.println("Engel algilandi! Kapak 180 derece aciliyor...");
    motor.write(120);      // Biraz daha hızlı açılması için 120 (90'dan uzaklaştıkça hızlanır)
    delay(acilmaSuresi);   // 180 derece süresi
    motor.write(90);       // Dur
    kapakAcikMi = true;
    delay(2000);           // Kapak açıldıktan sonra 2 saniye beklesin
  } 
  
  // ENGEL KALKTIĞINDA (KAPAK KAPANIYOR)
  else if ((mesafe >= engelMesafesi || mesafe == 0) && kapakAcikMi) {
    Serial.println("Yol temiz, kapak kapaniyor...");
    motor.write(60);       // Kapanma hızı
    delay(kapanmaSuresi);  // Kapanış süresi
    motor.write(90);       // Dur
    kapakAcikMi = false;
  }

  delay(100); 
}