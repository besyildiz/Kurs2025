// RGB LED - HIZLI DİSK GEÇİŞ EFEKTİ
// Pinler: 9=Kırmızı, 10=Yeşil, 11=Mavi

const int R = 9;    // Kırmızı pin
const int G = 10;   // Yeşil pin
const int B = 11;   // Mavi pin

// Geçiş hızı (ms) - DAHA HIZLI
int hiz = 5;  // 5ms = ÇOK HIZLI

void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  
  // Başlangıçta LED'i kapat
  analogWrite(R, 0);
  analogWrite(G, 0);
  analogWrite(B, 0);
  
  Serial.begin(9600);
  Serial.println("RGB LED - Disk Gecisi");
  Serial.println("HIZ: 5ms | EFEKT: Disk");
}

void loop() {
  // 1. HIZLI KIRMIZI PULSE
  Serial.println("🔴 HIZLI KIRMIZI DALGA");
  for (int i = 0; i <= 255; i += 5) {
    analogWrite(R, i);
    analogWrite(G, 0);
    analogWrite(B, 0);
    delay(hiz);
  }
  for (int i = 255; i >= 0; i -= 5) {
    analogWrite(R, i);
    delay(hiz);
  }
  
  // 2. YEŞİL DİSK EFEKTİ
  Serial.println("🟢 YEŞİL DİSK DÖNÜŞÜ");
  for (int i = 0; i <= 255; i += 3) {
    analogWrite(R, 0);
    analogWrite(G, i);
    analogWrite(B, 0);
    delay(hiz);
  }
  // Ani kesme efekti
  analogWrite(G, 0);
  delay(10);
  
  // 3. MAVİ SPİRAL
  Serial.println("🔵 MAVİ SPİRAL");
  for (int i = 0; i <= 255; i += 4) {
    analogWrite(R, 0);
    analogWrite(G, 0);
    analogWrite(B, i);
    delay(hiz/2);
  }
  for (int i = 255; i >= 0; i -= 8) {
    analogWrite(B, i);
    delay(hiz/2);
  }
  
  // 4. SARI DİSK (Kırmızı+Yeşil)
  Serial.println("🟡 SARI DİSK");
  for (int i = 0; i <= 255; i += 4) {
    analogWrite(R, i);
    analogWrite(G, i);
    analogWrite(B, 0);
    delay(hiz);
  }
  
  // 5. MOR STROBE (Kırmızı+Mavi)
  Serial.println("🟣 MOR STROBE");
  for (int flash = 0; flash < 10; flash++) {
    analogWrite(R, 255);
    analogWrite(G, 0);
    analogWrite(B, 255);
    delay(30);
    analogWrite(R, 0);
    analogWrite(B, 0);
    delay(30);
  }
  
  // 6. BEYAZ DÖNÜŞ
  Serial.println("⚪ BEYAZ DÖNER DİSK");
  for (int i = 0; i <= 255; i += 3) {
    analogWrite(R, i);
    analogWrite(G, i);
    analogWrite(B, i);
    delay(hiz);
  }
  for (int i = 255; i >= 0; i -= 6) {
    analogWrite(R, i);
    analogWrite(G, i);
    analogWrite(B, i);
    delay(hiz/2);
  }
  
  // 7. GÖKKUŞAĞI DİSKİ (EN HIZLI)
  Serial.println("🌈 GÖKKUŞAĞI DİSK");
  gokkusagiDisk();
  
  // 8. RASTGELE DİSK PATLAMALARI
  Serial.println("💥 RASTGELE PATLAMALAR");
  for (int patlama = 0; patlama < 15; patlama++) {
    int r = random(50, 255);
    int g = random(50, 255);
    int b = random(50, 255);
    
    analogWrite(R, r);
    analogWrite(G, g);
    analogWrite(B, b);
    delay(50);
    
    analogWrite(R, 0);
    analogWrite(G, 0);
    analogWrite(B, 0);
    delay(50);
  }
  
  // 9. SOLAR SYSTEM (Gezegen efekti)
  Serial.println("🪐 GEZEGEN DÖNÜŞÜ");
  gezegenEfekti();
}

// GÖKKUŞAĞI DİSK EFEKTİ
void gokkusagiDisk() {
  // Kırmızı → Sarı
  for (int i = 0; i <= 255; i += 4) {
    analogWrite(R, 255);
    analogWrite(G, i);
    analogWrite(B, 0);
    delay(hiz/2);
  }
  
  // Sarı → Yeşil
  for (int i = 255; i >= 0; i -= 4) {
    analogWrite(R, i);
    analogWrite(G, 255);
    analogWrite(B, 0);
    delay(hiz/2);
  }
  
  // Yeşil → Camgöbeği
  for (int i = 0; i <= 255; i += 4) {
    analogWrite(R, 0);
    analogWrite(G, 255);
    analogWrite(B, i);
    delay(hiz/2);
  }
  
  // Camgöbeği → Mavi
  for (int i = 255; i >= 0; i -= 4) {
    analogWrite(R, 0);
    analogWrite(G, i);
    analogWrite(B, 255);
    delay(hiz/2);
  }
  
  // Mavi → Mor
  for (int i = 0; i <= 255; i += 4) {
    analogWrite(R, i);
    analogWrite(G, 0);
    analogWrite(B, 255);
    delay(hiz/2);
  }
  
  // Mor → Beyaz
  for (int i = 0; i <= 255; i += 4) {
    analogWrite(R, 255);
    analogWrite(G, i);
    analogWrite(B, 255);
    delay(hiz/2);
  }
}

// GEZEGEN DÖNÜŞ EFEKTİ
void gezegenEfekti() {
  // Kırmızı gezegen
  for (int i = 0; i < 3; i++) {
    for (int parlaklik = 0; parlaklik <= 255; parlaklik += 10) {
      analogWrite(R, parlaklik);
      analogWrite(G, parlaklik/3);
      analogWrite(B, 0);
      delay(10);
    }
    delay(100);
    analogWrite(R, 0);
    delay(100);
  }
  
  // Mavi gezegen
  for (int i = 0; i < 3; i++) {
    for (int parlaklik = 0; parlaklik <= 255; parlaklik += 10) {
      analogWrite(R, 0);
      analogWrite(G, parlaklik/4);
      analogWrite(B, parlaklik);
      delay(10);
    }
    delay(100);
    analogWrite(B, 0);
    delay(100);
  }
  
  // Yeşil gezegen
  for (int i = 0; i < 3; i++) {
    for (int parlaklik = 0; parlaklik <= 255; parlaklik += 10) {
      analogWrite(R, parlaklik/5);
      analogWrite(G, parlaklik);
      analogWrite(B, parlaklik/5);
      delay(10);
    }
    delay(100);
    analogWrite(G, 0);
    delay(100);
  }
}