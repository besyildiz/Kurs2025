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

// PID VE HIZ AYARLARI
float Kp = 0.05;    
float Kd = 0.4;     
int baseSpeed = 60; // Takip için yavaş hız
int previousError = 0;
int debugCounter = 0;

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
  Serial.println("--- IZLEME MODU AKTIF ---");
  Serial.println("Kalibrasyon yapiliyor...");

  for (int i = 0; i < 300; i++)
  {
    qtr.calibrate();
    delay(5);
  }
  
  digitalWrite(LED, LOW);
  Serial.println("Kalibrasyon Bitti!");
  Serial.println("Poz\tHata\tSol_Hiz\tSag_Hiz"); // Baslik eklendi
  delay(1000);
}

void loop()
{
  unsigned int position = qtr.readLineBlack(sensorValues);

  int error = 3500 - position;
  float P = error;
  float D = error - previousError;
  int motorSpeed = (Kp * P) + (Kd * D);
  previousError = error;

  // MOTOR HIZ HESABI 
  // Sola donmek icin (error pozitifken) Sag motor (R) hizlanmali
  int leftMotorSpeed = baseSpeed - motorSpeed;
  int rightMotorSpeed = baseSpeed + motorSpeed;

  // Hız sınırları (Alt sınır motorun dönmesini sağlar)
  leftMotorSpeed = constrain(leftMotorSpeed, 35, 100); 
  rightMotorSpeed = constrain(rightMotorSpeed, 35, 100);

  // SERİ PORT ÇIKTISI (Geliştirilmiş İzleme)
  debugCounter++;
  if (debugCounter >= 150) { 
    Serial.print(position);
    Serial.print("\t");
    Serial.print(error);
    Serial.print("\tL: ");
    Serial.print(leftMotorSpeed);
    Serial.print("\tR: ");
    Serial.println(rightMotorSpeed);
    debugCounter = 0; 
  }

  leftMotorForward(leftMotorSpeed);
  rightMotorForward(rightMotorSpeed);
}

void leftMotorForward(int speedValue) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, speedValue);
}

void rightMotorForward(int speedValue) {
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, speedValue);
}