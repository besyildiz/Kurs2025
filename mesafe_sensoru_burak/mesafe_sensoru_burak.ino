#include <LiquidCrystal.h>

// LCD pinlerini tanımlıyoruz (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int trigPin = 7;
const int echoPin = 6;
const int buzzerPin = 8;

long sure;
int mesafe;

void setup() {
  lcd.begin(16, 2); // LCD'yi başlat
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  lcd.print("Mesafe Olcer");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Mesafeyi ölçüyoruz
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  sure = pulseIn(echoPin, HIGH);
  mesafe = sure * 0.034 / 2; // Ses hızı denklemi

  // LCD'ye yazdırıyoruz
  lcd.setCursor(0, 0);
  lcd.print("Mesafe: ");
  lcd.print(mesafe);
  lcd.print(" cm  "); // Sonundaki boşluklar eski karakterleri temizler

  // Buzzer Kontrolü (Mesafe yaklaştıkça ses hızlanır)
  if (mesafe < 30 && mesafe > 0) {
    digitalWrite(buzzerPin, HIGH);
    delay(mesafe * 10); // Mesafe azaldıkça bekleme süresi düşer
    digitalWrite(buzzerPin, LOW);
    delay(mesafe * 10);
  } else {
    digitalWrite(buzzerPin, LOW);
  }
  
  delay(100);
}