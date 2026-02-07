// RGB LED Pin Tanımlamaları
const int kirmiziPin = 9;   // PWM pin
const int yesilPin = 10;    // PWM pin  
const int maviPin = 11;     // PWM pin

// Renk değişim hızı (ms)
int renkGecisHizi = 20;

void setup() {
  // RGB LED pinlerini çıkış olarak ayarla
  pinMode(kirmiziPin, OUTPUT);
  pinMode(yesilPin, OUTPUT);
  pinMode(maviPin, OUTPUT);
  
  // Başlangıçta LED'i kapat
  rgbYak(0, 0, 0);
  
  Serial.begin(9600);
  Serial.println("RGB LED Renk Gecisi");
  Serial.println("====================");
}

void loop() {
  Serial.println("🔴 KIRMIZI");
  kirmizidanYesile();
  
  Serial.println("🟢 YESIL");
  yesildenMaviye();
  
  Serial.println("🔵 MAVI");
  mavidenKirmiziya();
  
  Serial.println("🌈 TUM RENKLER");
  tumRenkler();
  
  Serial.println("🎨 OZEL RENKLER");
  ozelRenkler();
}

// RGB renk ayarlama fonksiyonu
void rgbYak(int kirmizi, int yesil, int mavi) {
  analogWrite(kirmiziPin, kirmizi);
  analogWrite(yesilPin, yesil);
  analogWrite(maviPin, mavi);
}

// KIRMIZI → YEŞİL geçişi
void kirmizidanYesile() {
  for (int i = 0; i <= 255; i++) {
    rgbYak(255 - i, i, 0);
    delay(renkGecisHizi);
  }
}

// YEŞİL → MAVİ geçişi
void yesildenMaviye() {
  for (int i = 0; i <= 255; i++) {
    rgbYak(0, 255 - i, i);
    delay(renkGecisHizi);
  }
}

// MAVİ → KIRMIZI geçişi
void mavidenKirmiziya() {
  for (int i = 0; i <= 255; i++) {
    rgbYak(i, 0, 255 - i);
    delay(renkGecisHizi);
  }
}

// TÜM RENKLERİN GEÇİŞİ
void tumRenkler() {
  // Kırmızı → Sarı
  for (int i = 0; i <= 255; i++) {
    rgbYak(255, i, 0);
    delay(renkGecisHizi/2);
  }
  
  // Sarı → Yeşil
  for (int i = 255; i >= 0; i--) {
    rgbYak(i, 255, 0);
    delay(renkGecisHizi/2);
  }
  
  // Yeşil → Camgöbeği (Cyan)
  for (int i = 0; i <= 255; i++) {
    rgbYak(0, 255, i);
    delay(renkGecisHizi/2);
  }
  
  // Camgöbeği → Mavi
  for (int i = 255; i >= 0; i--) {
    rgbYak(0, i, 255);
    delay(renkGecisHizi/2);
  }
  
  // Mavi → Mor (Magenta)
  for (int i = 0; i <= 255; i++) {
    rgbYak(i, 0, 255);
    delay(renkGecisHizi/2);
  }
  
  // Mor → Beyaz
  for (int i = 0; i <= 255; i++) {
    rgbYak(255, i, 255);
    delay(renkGecisHizi/2);
  }
  
  // Beyaz → Kırmızı
  for (int i = 255; i >= 0; i--) {
    rgbYak(255, i, i);
    delay(renkGecisHizi/2);
  }
}

// ÖZEL RENKLER
void ozelRenkler() {
  // Ana renkler
  rgbYak(255, 0, 0);   // 🔴 Kırmızı
  delay(1000);
  
  rgbYak(0, 255, 0);   // 🟢 Yeşil
  delay(1000);
  
  rgbYak(0, 0, 255);   // 🔵 Mavi
  delay(1000);
  
  // Ara renkler
  rgbYak(255, 255, 0);   // 🟡 Sarı
  delay(1000);
  
  rgbYak(0, 255, 255);   // 🔷 Camgöbeği (Cyan)
  delay(1000);
  
  rgbYak(255, 0, 255);   // 🟣 Mor (Magenta)
  delay(1000);
  
  rgbYak(255, 255, 255); // ⚪ Beyaz
  delay(1000);
  
  rgbYak(255, 165, 0);   // 🟠 Turuncu
  delay(1000);
  
  rgbYak(128, 0, 128);   // 🟣 Mor (Koyu)
  delay(1000);
  
  rgbYak(0, 128, 128);   // 🔵 Yeşilimsi Mavi
  delay(1000);
  
  // Kapat
  rgbYak(0, 0, 0);
  delay(500);
}