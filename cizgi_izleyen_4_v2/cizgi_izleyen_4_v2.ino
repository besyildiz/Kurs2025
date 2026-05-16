#include <QTRSensors.h>

QTRSensors qtr;
unsigned int sensorValues[8];

// --- MOTOR PIN TANIMLAMALARI ---
#define ENA 3  // Sol motor hız pini
#define IN1 8  // Sol motor yön pinleri
#define IN2 9  

#define ENB 5  // Sağ motor hız pini
#define IN3 7  // Sağ motor yön pinleri
#define IN4 6  

// --- SÜPER YAVAŞ VE GÜVENLİ SÜRÜŞ PARAMETRELERİ ---
int baseSpeed = 20;    // Çok sakin, masa üstü için ideal güvenli temel hız 105
float Kp = 0.12;        // Yavaş sürüşe uygun, yumuşak dönüş tepkisi 0.12
float Kd = 0.35;        // Titremeyi önleyen sönümleme gücü 0.35
int previousError = 0; 

// --- SENSÖR EŞİK DEĞERİ ---
int beyazEsik = 530; 

void setup() {
  Serial.begin(9600); 
  qtr.setTypeAnalog();
  qtr.setSensorPins((const byte[]){A7, A6, A5, A4, A3, A2, A1, A0}, 8);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  Serial.println("--- SÜPER YAVAŞ MOD: KALIBRASYON BASLADI ---");
  Serial.println("Sensorleri beyaz cizgi uzerinde saga sola GENISCE gezdirin...");
  
  for (int i = 0; i < 200; i++) {
    qtr.calibrate();
    delay(10);
  }
  
  Serial.println("--- SÜPER GÜVENLİ SÜRÜŞ BAŞLADI ---");
  delay(1000);
}

void loop() {
  // 1. Sensörleri Oku
  qtr.read(sensorValues);
  unsigned int position = qtr.readLineWhite(sensorValues);
  
  // 2. Çizgi Kontrolü
  bool cizgiVarMi = false;
  int enDusukOkuma = 1023;

  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] < enDusukOkuma) {
      enDusukOkuma = sensorValues[i];
    }
  }

  if (enDusukOkuma < beyazEsik) {
    cizgiVarMi = true;
  }

  // 3. PID Hesaplama
  int error = 3500 - position;
  int motorSpeed = (Kp * error) + (Kd * (error - previousError));
  previousError = error;

  int leftMotorSpeed = 0;
  int rightMotorSpeed = 0;
  String durum = "";

  // 4. Yavaşlatılmış Sürüş ve Dönüş Kararları
  if (!cizgiVarMi) {
    durum = "GERI GELEREK CIZGI ARANIYOR";
    // Masadan düşme riskine karşı geri aramayı çok yavaş (70 hızında) yapıyoruz
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    analogWrite(ENA, 70);    analogWrite(ENB, 70);
    delay(50); // Çok kısa bir an geri gel
    return;
  } 
  else if (position < 1000) {
    durum = "EN SOLDA";
    leftMotorSpeed = 0;    
    rightMotorSpeed = 135; // Keskin dönüş hızı da tamamen yavaşlatıldı
  }
  else if (position >= 1000 && position < 3300) {
    durum = "MERKEZ SOL";
    leftMotorSpeed = baseSpeed - motorSpeed; 
    rightMotorSpeed = baseSpeed + motorSpeed;
  }
  else if (position >= 3300 && position <= 3700) {
    durum = "TAM MERKEZ";
    leftMotorSpeed = baseSpeed; 
    rightMotorSpeed = baseSpeed;
  }
  else if (position > 3700 && position <= 6000) {
    durum = "MERKEZ SAG";
    leftMotorSpeed = baseSpeed - motorSpeed; 
    rightMotorSpeed = baseSpeed + motorSpeed;
  }
  else { // position > 6000
    durum = "EN SAGDA";
    leftMotorSpeed = 135;  // Keskin dönüş hızı tamamen yavaşlatıldı
    rightMotorSpeed = 0;    
  }

  // 5. Hız Sınırlandırma (Masa koruması için üst sınırı 150'ye çektik)
  if (durum.startsWith("MERKEZ")) {
    leftMotorSpeed = constrain(leftMotorSpeed, 40, 150); 
    rightMotorSpeed = constrain(rightMotorSpeed, 40, 150);
  }

  // 6. Motorları Çalıştır
  if (leftMotorSpeed > 20) {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    analogWrite(ENA, leftMotorSpeed);
  } else {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }

  if (rightMotorSpeed > 20) {
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENB, rightMotorSpeed);
  } else {
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  }

  // Robotun hızını yapay olarak dizginleyen ana mola (50ms)
  delay(50); 
}