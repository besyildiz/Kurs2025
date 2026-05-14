#include <QTRSensors.h>

QTRSensors qtr;
unsigned int sensorValues[8];

#define ENA 5
#define IN1 7
#define IN2 6
#define ENB 3
#define IN3 8
#define IN4 9

// --- ADIM AYARLARI ---
int stepDuration = 80;  
int stepWait = 1500;    
int baseSpeed = 110;    

float Kp = 0.05;    
int previousError = 0;

void setup() {
  Serial.begin(9600); 
  qtr.setTypeAnalog();
  qtr.setSensorPins((const byte[]){A0, A1, A2, A3, A4, A5, A6, A7}, 8);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  Serial.println("--- KALIBRASYON BASLADI ---");
  for (int i = 0; i < 200; i++) {
    qtr.calibrate();
    if (i % 20 == 0) {
      qtr.read(sensorValues);
      Serial.print("Kalibrasyon... S4 Degeri: "); Serial.println(sensorValues[3]);
    }
    delay(10);
  }
  Serial.println("--- HAZIR ---\nPoz\tL_Guc\tR_Guc\tKonum Durumu");
  delay(1000);
}

void loop() {
  unsigned int position = qtr.readLineBlack(sensorValues);
  
  bool cizgiVarMi = false;
  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] > 400) { 
      cizgiVarMi = true;
      break;
    }
  }

 // --- HESAPLAMA VE DAVRANIS IYILESDIRME ---
  int error = 3500 - position;
  float P = error;
  float D = error - previousError;
  
  // Kp değerini 0.05'ten 0.07'ye çekerek tepkiyi biraz sertleştirdim (Merkeze daha istekli gelir)
  int motorSpeed = (0.07 * P) + (0.3 * D); 
  previousError = error;

  int leftMotorSpeed = 0;
  int rightMotorSpeed = 0;
  String durum = "";

  if (!cizgiVarMi) {
    durum = "CIZGI YOK";
    leftMotorSpeed = 0; rightMotorSpeed = 0;
  } 
  else if (position < 1000) {
    durum = "EN SOLDA";
    leftMotorSpeed = 150; rightMotorSpeed =  0; 
  }
  else if (position >= 1000 && position < 3000) {
    durum = "MERKEZ SOL";
    // MERKEZ SOLDA: Sag motorun (right) daha baskın olması lazım
    // Isaretleri senin motor yonune gore guncelledim:
    leftMotorSpeed = baseSpeed - motorSpeed; 
    rightMotorSpeed = baseSpeed + motorSpeed;
  }
  else if (position >= 3000 && position <= 4000) {
    durum = "TAM MERKEZ";
    leftMotorSpeed = baseSpeed - motorSpeed;
    rightMotorSpeed = baseSpeed + motorSpeed;
  }
  else if (position > 4000 && position <= 6000) {
    durum = "MERKEZ SAG";
    // MERKEZ SAGDA: Sol motorun (left) daha baskın olması lazım
    leftMotorSpeed = baseSpeed - motorSpeed; 
    rightMotorSpeed = baseSpeed + motorSpeed;
  }
  else {
    durum = "EN SAGDA";
    leftMotorSpeed = 0; rightMotorSpeed = 150;
  }
  // Hız kısıtlamaları (Sadece çizgi varken ve merkez bölgelerindeyse uygula)
  if (cizgiVarMi && durum.startsWith("MERKEZ")) {
    leftMotorSpeed = constrain(leftMotorSpeed, 75, 160); 
    rightMotorSpeed = constrain(rightMotorSpeed, 75, 160);
  }

  // Seri Monitör Çıktısı
  Serial.print(position); Serial.print("\t");
  Serial.print(leftMotorSpeed); Serial.print("\t");
  Serial.print(rightMotorSpeed); Serial.print("\t");
  Serial.println(durum);

  // --- HAREKET UYGULAMA ---
  if (cizgiVarMi) {
    if (leftMotorSpeed > 0) {
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, leftMotorSpeed);
    } else {
      digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); analogWrite(ENA, 0);
    }

    if (rightMotorSpeed > 0) {
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, rightMotorSpeed);
    } else {
      digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); analogWrite(ENB, 0);
    }
    delay(stepDuration); 
  }
  
  motorsStop(); 
  delay(stepWait); 
}

void motorsStop() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}