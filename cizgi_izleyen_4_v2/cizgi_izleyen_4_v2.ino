#include <QTRSensors.h>

QTRSensors qtr;
unsigned int sensorValues[8];

// MOTOR PİNLERİ
#define ENA 5
#define IN1 7
#define IN2 6
#define ENB 3
#define IN3 8
#define IN4 9
#define LED 13

float Kp = 0.05;    
float Kd = 0.4;     
int baseSpeed = 70; // Adım atarken biraz güç lazım, 70 iyidir
int previousError = 0;

void setup()
{
  Serial.begin(9600); 

  qtr.setTypeAnalog();
  qtr.setSensorPins((const byte[]){A0, A1, A2, A3, A4, A5, A6, A7}, 8);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(LED, OUTPUT);

  delay(2000);
  digitalWrite(LED, HIGH);
  Serial.println("--- DENEYSEL ADIM MODU ---");
  
  for (int i = 0; i < 300; i++) {
    qtr.calibrate();
    delay(5);
  }
  
  digitalWrite(LED, LOW);
  Serial.println("Kalibrasyon Bitti! Her satir bir adimdir.");
  Serial.println("Poz\tHata\tSol_H\tSag_H");
  delay(1000);
}

void loop()
{
  // 1. ADIM: Sensörleri oku ve hesapla
  unsigned int position = qtr.readLineBlack(sensorValues);
  int error = 3500 - position;
  float P = error;
  float D = error - previousError;
  int motorSpeed = (Kp * P) + (Kd * D);
  previousError = error;

  int leftMotorSpeed = baseSpeed - motorSpeed;
  int rightMotorSpeed = baseSpeed + motorSpeed;

  leftMotorSpeed = constrain(leftMotorSpeed, 0, 120); 
  rightMotorSpeed = constrain(rightMotorSpeed, 0, 120);

  // 2. ADIM: Bilgiyi ekrana yaz
  Serial.print(position);
  Serial.print("\t");
  Serial.print(error);
  Serial.print("\tL:");
  Serial.print(leftMotorSpeed);
  Serial.print("\tR:");
  Serial.println(rightMotorSpeed);

  // 3. ADIM: Motorları çok kısa bir süre çalıştır (Adım Atma)
  leftMotorForward(leftMotorSpeed);
  rightMotorForward(rightMotorSpeed);
  delay(100); // 0.1 saniye boyunca hareket et

  // 4. ADIM: Motorları durdur ve bekle
  motorsStop();
  delay(1000); // 1 saniye bekle (Senin okuman ve gözlemlemen için)
}

// YENİ FONKSİYON: Motorları Durdurma
void motorsStop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void leftMotorForward(int speedValue) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, speedValue);
}

void rightMotorForward(int speedValue) {
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, speedValue);
}