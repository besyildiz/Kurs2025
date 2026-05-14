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
int stepDuration = 80;  // Adımı daha iyi hissetmek için 80ms yaptım
int stepWait = 1500;    
int baseSpeed = 110;    
// ---------------------

float Kp = 0.05;    
int previousError = 0;

void setup() {
  Serial.begin(9600); 
  qtr.setTypeAnalog();
  qtr.setSensorPins((const byte[]){A0, A1, A2, A3, A4, A5, A6, A7}, 8);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  Serial.println("KALIBRASYON YAPILIYOR...");
  for (int i = 0; i < 300; i++) {
    qtr.calibrate();
    delay(5);
  }
  Serial.println("Poz\tL_Guc\tR_Guc\tDurum");
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

  int error = 3500 - position;
  float P = error;
  float D = error - previousError;
  int motorSpeed = (Kp * P) + (0.3 * D);
  previousError = error;

  int leftMotorSpeed = 0;
  int rightMotorSpeed = 0;
  String durum = "";

  if (!cizgiVarMi) {
    durum = "CIZGI YOK";
    leftMotorSpeed = 0;
    rightMotorSpeed = 0;
  } 
  else if (position >= 2000 && position <= 5000) {
    durum = "MERKEZDE";
    leftMotorSpeed = baseSpeed - motorSpeed;
    rightMotorSpeed = baseSpeed + motorSpeed;
    leftMotorSpeed = constrain(leftMotorSpeed, 75, 160); 
    rightMotorSpeed = constrain(rightMotorSpeed, 75, 160);
  } 
  else if (position < 2000) {
    durum = "SOL KENAR";
    leftMotorSpeed = 0;    // Sol dursun
    rightMotorSpeed = 150; // Sağ dönsün
  } 
  else {
    durum = "SAG KENAR";
    leftMotorSpeed = 150;  // Sol dönsün
    rightMotorSpeed = 0;   // Sağ dursun
  }

  // Seri Monitör Çıktısı
  Serial.print(position); Serial.print("\t");
  Serial.print(leftMotorSpeed); Serial.print("\t");
  Serial.print(rightMotorSpeed); Serial.print("\t");
  Serial.println(durum);

  // --- HAREKET KOMUTLARI (DÜZELTİLDİ) ---
  if (cizgiVarMi) {
    // Sol Motor Kontrolü
    if (leftMotorSpeed > 0) {
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, leftMotorSpeed);
    } else {
      digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); analogWrite(ENA, 0);
    }

    // Sağ Motor Kontrolü
    if (rightMotorSpeed > 0) {
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, rightMotorSpeed);
    } else {
      digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); analogWrite(ENB, 0);
    }
    
    delay(stepDuration); 
  }
  
  motorsStop(); // Adım bittiğinde her ikisini de kapat
  delay(stepWait); 
}

void motorsStop() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}