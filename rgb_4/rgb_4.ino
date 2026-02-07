// RGB LED - SÜPER HIZLI DİSK EFEKTİ
const int R = 9;
const int G = 10;
const int B = 11;

// ÇOK HIZLI geçiş
int ultraHiz = 2; // 2ms = SÜPER HIZLI

void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  Serial.begin(9600);
  Serial.println("⚡ SÜPER HIZLI DİSK EFEKTİ ⚡");
}

void loop() {
  // 1. NEON DİSK - Hızlı geçişler
  Serial.println("💫 NEON DİSK");
  neonDisk();
  
  // 2. STROBE DİSK - Flaş efekti
  Serial.println("⚡ STROBE DİSK");
  strobeDisk();
  
  // 3. PULSE DİSK - Nabız efekti
  Serial.println("💓 PULSE DİSK");
  pulseDisk();
  
  // 4. SPİRAL DİSK - Dönen spiral
  Serial.println("🌀 SPİRAL DİSK");
  spiralDisk();
}

void neonDisk() {
  // Hızlı neon renk değişimleri
  int neonRenkler[6][3] = {
    {255, 0, 0},     // Kırmızı
    {255, 255, 0},   // Sarı
    {0, 255, 0},     // Yeşil
    {0, 255, 255},   // Camgöbeği
    {0, 0, 255},     // Mavi
    {255, 0, 255}    // Mor
  };
  
  for (int tur = 0; tur < 5; tur++) {
    for (int renk = 0; renk < 6; renk++) {
      analogWrite(R, neonRenkler[renk][0]);
      analogWrite(G, neonRenkler[renk][1]);
      analogWrite(B, neonRenkler[renk][2]);
      delay(ultraHiz * 10);
    }
  }
}

void strobeDisk() {
  // Flaş efekti - çok hızlı yanıp sönme
  for (int flash = 0; flash < 20; flash++) {
    // Beyaz flaş
    analogWrite(R, 255);
    analogWrite(G, 255);
    analogWrite(B, 255);
    delay(ultraHiz * 5);
    
    // Karanlık
    analogWrite(R, 0);
    analogWrite(G, 0);
    analogWrite(B, 0);
    delay(ultraHiz * 5);
    
    // Renkli flaş
    analogWrite(R, random(100, 255));
    analogWrite(G, random(100, 255));
    analogWrite(B, random(100, 255));
    delay(ultraHiz * 3);
    
    analogWrite(R, 0);
    analogWrite(G, 0);
    analogWrite(B, 0);
    delay(ultraHiz * 3);
  }
}

void pulseDisk() {
  // Nabız efekti - genişleyip daralan
  for (int pulse = 0; pulse < 3; pulse++) {
    // Kırmızı nabız
    for (int i = 0; i <= 255; i += 8) {
      analogWrite(R, i);
      analogWrite(G, i/4);
      delay(ultraHiz);
    }
    for (int i = 255; i >= 0; i -= 12) {
      analogWrite(R, i);
      analogWrite(G, i/4);
      delay(ultraHiz);
    }
    
    // Mavi nabız
    for (int i = 0; i <= 255; i += 8) {
      analogWrite(R, i/4);
      analogWrite(B, i);
      delay(ultraHiz);
    }
    for (int i = 255; i >= 0; i -= 12) {
      analogWrite(R, i/4);
      analogWrite(B, i);
      delay(ultraHiz);
    }
    
    // Yeşil nabız
    for (int i = 0; i <= 255; i += 8) {
      analogWrite(G, i);
      analogWrite(B, i/3);
      delay(ultraHiz);
    }
    for (int i = 255; i >= 0; i -= 12) {
      analogWrite(G, i);
      analogWrite(B, i/3);
      delay(ultraHiz);
    }
  }
}

void spiralDisk() {
  // Spiral dönüş efekti
  for (int spiral = 0; spiral < 3; spiral++) {
    // İçten dışa spiral
    for (int i = 0; i <= 255; i += 5) {
      analogWrite(R, i);
      analogWrite(G, 255 - i);
      analogWrite(B, (i + 128) % 255);
      delay(ultraHiz * 2);
    }
    
    // Dıştan içe spiral
    for (int i = 255; i >= 0; i -= 5) {
      analogWrite(R, i);
      analogWrite(G, 255 - i);
      analogWrite(B, (i + 128) % 255);
      delay(ultraHiz * 2);
    }
  }
}