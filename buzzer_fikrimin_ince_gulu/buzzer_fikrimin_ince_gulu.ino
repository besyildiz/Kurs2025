// FİKRİMİN İNCE GÜLÜ - Pasif Buzzer İçin
// Arduino D8 pinine bağlı buzzer ile çalınır

const int buzzer = 8;  // Buzzer pin bağlantısı

// Nota frekansları (Hz)
#define SUS 0      // Sessiz (nota yok)
#define DO 262
#define RE 294
#define MI 330
#define FA 349
#define SOL 392
#define LA 440
#define SI 494
#define DO2 523
#define RE2 587

// Melodi notaları - "Fikrimin İnce Gülü"
int melodi[] = {
  // 1. kısım - "Fikrimin ince gülü"
  RE, MI, FA, MI,
  RE, MI, FA, MI,
  MI, RE, DO, SI, LA, SOL, LA, SI,
  SOL, LA, SOL, FA, FA, MI, MI,
  
  // 2. kısım - "Gönlümün şen bülbülü"
  RE, MI, MI, MI,
  RE, MI, FA, MI,
  RE, MI, FA, FA, MI, RE, RE, DO, DO, SI, DO,
  
  // 3. kısım - "O gün ki gördüm seni"
  SI, DO, DO, SI, LA,
  LA, SOL, LA, SI, LA, SOL, FA, MI, RE,
  FA, FA, FA, MI, RE, SOL, FA, FA, MI, MI
};

// Nota süreleri (ms)
int sure[] = {
  // 1. kısım süreleri
  400, 400, 800, 400,
  400, 400, 800, 400,
  400, 400, 400, 400, 400, 400, 400, 800,
  400, 400, 400, 400, 400, 800,
  
  // 2. kısım süreleri
  400, 400, 400, 400,
  400, 400, 800, 400,
  400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 800,
  
  // 3. kısım süreleri
  400, 400, 400, 400, 800,
  400, 400, 400, 400, 400, 400, 400, 400, 800,
  400, 400, 400, 400, 400, 400, 400, 400, 400, 800
};

void setup() {
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  Serial.println("Fikrimin Ince Gulu - Basliyor...");
}

void loop() {
  int notaSayisi = sizeof(melodi) / sizeof(melodi[0]);
  
  for (int i = 0; i < notaSayisi; i++) {
    if (melodi[i] == SUS) {
      noTone(buzzer);
      delay(sure[i]);
    } else {
      tone(buzzer, melodi[i], sure[i]);
      delay(sure[i] + 20);  // Notalar arası 20ms boşluk
      noTone(buzzer);
    }
  }
  
  Serial.println("Sarki bitti, 3 saniye sonra tekrar...");
  delay(3000);  // Şarkı bitince 3 saniye bekle
}