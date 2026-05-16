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

// --- ULTRA DİKKATLİ VE SIK VERİ KONTROL AYARLARI ---
int baseSpeed = 85;     // Temel hızı daha da düşürdük (Motorların bayılmayacağı en alt sınır)
float Kp = 0.22;        // Virajı kaçırmasın diye tepki sertliğini artırdık (Çok kritik!)
float Kd = 0.50;        // Ani tepkide masada savrulmayı önleyen sert fren gücü
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

  Serial.println("--- ULTRA DIKKATLI MOD BASLADI ---");
  Serial.println("Sensorleri beyaz cizgi uzerinde saga sola GENISCE gezdirin...");
  
  for (int i = 0; i < 200; i++) {
    qtr.calibrate();
    delay(10);
  }
  
  Serial.println("--- SENSORLER HAZIR (KESINTISIZ GOZLEM) ---");
  delay(1000);
}

void loop() {
  // 1. Kesintisiz Sensör Oku (Gecikme sıfıra indirildi)
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

  // 3. PID Hesaplama (Yüksek hassasiyetli katsayılar)
  int error = 3500 - position;
  int motorSpeed = (Kp * error) + (Kd * (error - previousError));
  previousError = error;

  int leftMotorSpeed = 0;
  int rightMotorSpeed = 0;
  String durum = "";

  // 4. Hızlı Karar - Yavaş Uygulama Mekanizması
  if (!cizgiVarMi) {
    durum = "CIZGI YOK - ANINDA GERI ARA";
    // DELAY YOK! Çizgi bittiği salise motorlar anında geri vitese geçer.
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    analogWrite(ENA, 75);    analogWrite(ENB, 75);
    return; 
  } 
  else if (position < 1000) {
    durum = "EN SOLDA";
    leftMotorSpeed = 0;    
    rightMotorSpeed = 130; // Virajı yakaladığı an içeri fırlatacak güvenli güç
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
    leftMotorSpeed = 130;  
    rightMotorSpeed = 0;    
  }

  // 5. Hız Sınırlandırma (Düşük hız bariyeri)
  if (durum.startsWith("MERKEZ")) {
    leftMotorSpeed = constrain(leftMotorSpeed, 35, 130); 
    rightMotorSpeed = constrain(rightMotorSpeed, 35, 130);
  }

  // 6. Motor Komutları
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

  // Büyük delay() kaldırıldı. Sadece işlemcinin nefes alması için 1 milisaniye mola.
  // Araba saniyede binlerce kez durum kontrolü yapacak.
  delay(1); 
}