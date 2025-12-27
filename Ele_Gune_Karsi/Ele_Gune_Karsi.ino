// MFÖ - Ele Güne Karşı (Çok Basit)
// Sadece 3 nota kullan

const int b1 = 6;
const int b2 = 7;
const int b3 = 8;

#define DO  262
#define MI  330
#define SOL 392

int basitMelodi[] = {
  MI, MI, DO, DO, MI, MI,
  DO, DO, DO, DO, MI, DO,
  MI, MI, DO, DO, MI, MI,
  DO, DO, DO, DO, DO,
  
  SOL, SOL, MI, MI, SOL, SOL,
  MI, MI, MI, MI, SOL, MI,
  SOL, SOL, MI, MI, SOL, SOL,
  MI, MI, MI, MI, MI
};

int basitSure[] = {
  800, 800, 800, 800, 800, 800,
  800, 800, 800, 800, 800, 800,
  800, 800, 800, 800, 800, 800,
  800, 800, 800, 800, 1600,
  
  800, 800, 800, 800, 800, 800,
  800, 800, 800, 800, 800, 800,
  800, 800, 800, 800, 800, 800,
  800, 800, 800, 800, 1600
};

void setup() {
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);
}

void loop() {
  // Tüm buzzer'lar birlikte çal (güçlü ses)
  for (int i = 0; i < sizeof(basitMelodi)/sizeof(basitMelodi[0]); i++) {
    tone(b1, basitMelodi[i], basitSure[i]);
    tone(b2, basitMelodi[i] + 20, basitSure[i]);
    tone(b3, basitMelodi[i] - 20, basitSure[i]);
    
    delay(basitSure[i]);
    
    noTone(b1);
    noTone(b2);
    noTone(b3);
    
    delay(100);
  }
  
  delay(5000);
}