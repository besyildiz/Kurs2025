// İSTİKLAL MARŞI - Pasif Buzzer İçin
// Sadece ilk bölüm - Korkma sönmez...

const int buzzer = 6;  // Pasif buzzer pin 6'da

// Nota frekansları (Hz)
#define SOL 392
#define LA 440
#define SI 494
#define DO2 523
#define RE2 587
#define MI2 659

void setup() {
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  Serial.println("İstiklal Marşı - Pasif Buzzer");
}

void loop() {
  // === 1. SATIR: "Korkma sönmez bu şafaklarda yüzen al sancak" ===
  
  // "Kork-ma" (SOL - SOL)
  tone(buzzer, SOL, 400); delay(450);
  tone(buzzer, SOL, 400); delay(450);
  
  // "sön-mez" (LA - SOL)
  tone(buzzer, LA, 350); delay(400);
  tone(buzzer, SOL, 600); delay(650);
  
  // "bu şa-fak-lar-da" (DO2 - SI - LA - SOL)
  tone(buzzer, DO2, 300); delay(350);
  tone(buzzer, SI, 300); delay(350);
  tone(buzzer, LA, 300); delay(350);
  tone(buzzer, SOL, 600); delay(650);
  
  // "yü-zen" (LA - LA)
  tone(buzzer, LA, 300); delay(350);
  tone(buzzer, LA, 300); delay(350);
  
  // "al" (SI)
  tone(buzzer, SI, 300); delay(350);
  
  // "san-cak" (LA - RE2 - DO2)
  tone(buzzer, LA, 300); delay(350);
  tone(buzzer, RE2, 350); delay(400);
  tone(buzzer, DO2, 800); delay(850);
  
  // Biraz bekle ve tekrar et
  noTone(buzzer);
  delay(2000);
}