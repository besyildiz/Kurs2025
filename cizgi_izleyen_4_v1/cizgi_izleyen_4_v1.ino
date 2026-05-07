#include <QTRSensors.h>

QTRSensors qtr;
unsigned int sensorValues[8];

// MOTOR PINS
#define ENA 5
#define IN1 7
#define IN2 6
#define ENB 3
#define IN3 8
#define IN4 9
#define LED 13

// PID
float Kp = 0.06;
float Kd = 0.48;
int previousError = 0;
int baseSpeed = 120;

void setup()
{
  Serial.begin(9600); // Seri haberleşmeyi başlattık

  qtr.setTypeAnalog();
  qtr.setSensorPins((const byte[]){A0, A1, A2, A3, A4, A5, A6, A7}, 8);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(LED, OUTPUT);

  delay(2000);
  digitalWrite(LED, HIGH);

  // KALİBRASYON
  for (int i = 0; i < 300; i++)
  {
    qtr.calibrate();
    delay(5);
  }
  digitalWrite(LED, LOW);
  
  Serial.println("Kalibrasyon Tamamlandi!");
}

void loop()
{
  // readLineWhite: Beyaz çizgi üzerinde siyah zemin için
  // readLineBlack: Siyah çizgi üzerinde beyaz zemin için
  unsigned int position = qtr.readLineWhite(sensorValues);

  // --- SERİ PORTA YAZDIRMA KISMI ---
  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t'); // Değerler arasına boşluk bırakır
  }
  Serial.print(" Pozisyon: ");
  Serial.println(position);
  // ---------------------------------

  int error = 3500 - position;

  float P = error;
  float D = error - previousError;

  int motorSpeed = (Kp * P) + (Kd * D);
  previousError = error;

  int leftMotorSpeed = baseSpeed - motorSpeed;
  int rightMotorSpeed = baseSpeed + motorSpeed;

  leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
  rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);

  leftMotorForward(leftMotorSpeed);
  rightMotorForward(rightMotorSpeed);
}

// MOTOR FONKSİYONLARI (Değişmediği için özet geçilmiştir)
void leftMotorForward(int speedValue) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, speedValue);
}
void rightMotorForward(int speedValue) {
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, speedValue);
}
// ... Diğer geri ve durma fonksiyonların kodunda kalabilir.