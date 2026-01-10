#include <LiquidCrystal.h>

// LCD pin bağlantıları (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Ultrasonik sensör pinleri
const int trigPin = 6;
const int echoPin = 7;

void setup() {
  // Seri haberleşmeyi başlat
  Serial.begin(9600);
  
  // Ultrasonik sensör pinlerini ayarla
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // LCD'yi başlat (16 karakter x 2 satır)
  lcd.begin(16, 2);
  
  // Başlangıç mesajı
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MESAFE OLCME");
  lcd.setCursor(0, 1);
  lcd.print("HC-SR04 + LCD");
  
  delay(2000); // 2 saniye bekle
  
  // Temizle ve hazır mesajı göster
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Olculuyor...");
  
  Serial.println("Sistem hazir!");
  Serial.println("HC-SR04 + LCD 1602A");
  Serial.println("===================");
}

void loop() {
  // Mesafeyi ölç
  float mesafe = mesafeOlc();
  
  // Seri monitöre yaz
  Serial.print("Olculen Mesafe = ");
  Serial.print(mesafe);
  Serial.println(" cm");
  
  // LCD ekranı güncelle
  lcdEkraniGuncelle(mesafe);
  
  delay(500); // 0.5 saniye bekle
}

float mesafeOlc() {
  // Trig pinine 10μs HIGH sinyali gönder
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Echo pininden gelen sinyalin süresini ölç
  long sure = pulseIn(echoPin, HIGH);
  
  // Mesafeyi hesapla (ses hızı = 340 m/s = 0.034 cm/μs)
  // Mesafe = (Süre * Ses Hızı) / 2
  float mesafe = sure * 0.034 / 2;
  
  return mesafe;
}

void lcdEkraniGuncelle(float mesafe) {
  lcd.clear();
  
  // Üst satır: Başlık
  lcd.setCursor(0, 0);
  lcd.print("MESAFE:");
  
  // Alt satır: Mesafe değeri
  lcd.setCursor(0, 1);
  
  // Değeri formatla
  if (mesafe >= 400 || mesafe <= 2) {
    lcd.print("OLCULEMEDI");
  } else if (mesafe < 10) {
    // Tek haneli sayılar için
    lcd.print(" ");
    lcd.print(mesafe, 1); // 1 ondalık basamak
    lcd.print(" cm      "); // Boşluklar için
  } else if (mesafe < 100) {
    // Çift haneli sayılar için
    lcd.print(mesafe, 1); // 1 ondalık basamak
    lcd.print(" cm     "); // Boşluklar için
  } else {
    // Üç haneli sayılar için
    lcd.print(mesafe, 0); // Ondalık yok
    lcd.print(" cm    "); // Boşluklar için
  }
}