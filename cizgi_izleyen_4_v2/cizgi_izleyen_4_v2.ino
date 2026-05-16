#include <QTRSensors.h>

QTRSensors qtr;
unsigned int sensorValues[8];

// Motor Pin Tanımlamaları
#define ENA 5
#define IN1 7
#define IN2 6
#define ENB 3
#define IN3 8
#define IN4 9

// --- ADIM VE GÖZLEM AYARLARI ---
int stepDuration = 80;  
int stepWait = 2000;    // 2 saniye bekleme (Durumu rahat oku diye)
int baseSpeed = 115;    

float Kp = 0.12;        
int previousError = 0;

// --- SENSÖR EŞİK DEĞERİ ---
// Kalibrasyon bittikten sonra burayı monitördeki verilere göre güncelleyebiliriz.
int beyazEsik = 530; 

void setup() {
  Serial.begin(9600); 
  qtr.setTypeAnalog();
  // Pin sırasını senin donanımına göre ters bıraktık
  qtr.setSensorPins((const byte[]){A7, A6, A5, A4, A3, A2, A1, A0}, 8);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  Serial.println("--- KALIBRASYON BASLADI ---");
  Serial.println("Sensorleri beyaz cizgi uzerinde saga sola GENISCE gezdirin...");
  delay(1000);
  
  for (int i = 0; i < 200; i++) {
    qtr.calibrate();
    
    // --- YENİ: KALİBRASYON VERİLERİNİ CANLI İZLEME ---
    if (i % 20 == 0) {
      qtr.read(sensorValues); // O andaki anlık değerleri oku
      Serial.print("Kalibrasyon Adimi "); Serial.print(i); Serial.print("/200 -> ");
      // Sadece 0. (Sol) ve 7. (Sağ) ve 4. (Orta) sensörleri yazdıralım ki ekran dolmasın
      Serial.print("S Sol(A7): "); Serial.print(sensorValues[0]);
      Serial.print("\tS Orta(A4): "); Serial.print(sensorValues[4]);
      Serial.print("\tS Sag(A0): "); Serial.println(sensorValues[7]);
    }
    delay(10);
  }
  
  Serial.println("--- KALIBRASYON BITTI! SISTEM HAZIR ---");
  delay(1500);
}

void loop() {
  // 1. Sensörleri Oku
  qtr.read(sensorValues);
  unsigned int position = qtr.readLineWhite(sensorValues);
  
  // 2. Çizgi Kilit Mekanizması (Gürültü Engelleme)
  bool cizgiVarMi = false;
  int enDusukOkuma = 1023;

  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] < enDusukOkuma) {
      enDusukOkuma = sensorValues[i];
    }
  }

  // En parlak sensörümüz eşikten küçükse çizgi var kabul et
  if (enDusukOkuma < beyazEsik) {
    cizgiVarMi = true;
  }

  // 3. PID Hesaplama
  int error = 3500 - position;
  int motorSpeed = (Kp * error) + (0.3 * (error - previousError));
  previousError = error;

  int leftMotorSpeed = 0;
  int rightMotorSpeed = 0;
  String durum = "";

  // 4. Konum Durumunu Belirleme
  if (!cizgiVarMi) {
    durum = "CIZGI YOK (Siyah Zemin)";
    leftMotorSpeed = 0;
    rightMotorSpeed = 0;
  } 
  else if (position < 1200) {
    durum = "EN SOLDA";
    leftMotorSpeed = 0;    
    rightMotorSpeed = 170; 
  }
  else if (position >= 1200 && position < 3300) {
    durum = "MERKEZ SOL";
    leftMotorSpeed = baseSpeed - motorSpeed; 
    rightMotorSpeed = baseSpeed + motorSpeed;
  }
  else if (position >= 3300 && position <= 3700) {
    durum = "TAM MERKEZ";
    leftMotorSpeed = baseSpeed; 
    rightMotorSpeed = baseSpeed;
  }
  else if (position > 3700 && position <= 5800) {
    durum = "MERKEZ SAG";
    leftMotorSpeed = baseSpeed - motorSpeed; 
    rightMotorSpeed = baseSpeed + motorSpeed;
  }
  else {
    durum = "EN SAGDA";
    leftMotorSpeed = 170;  
    rightMotorSpeed = 0;    
  }

  // Hız Sınırlandırma
  if (cizgiVarMi && durum.startsWith("MERKEZ")) {
    leftMotorSpeed = constrain(leftMotorSpeed, 60, 190); 
    rightMotorSpeed = constrain(rightMotorSpeed, 60, 190);
  }

  // 5. Seri Monitöre Yazdır
  Serial.print("Poz: "); Serial.print(position);
  Serial.print("\tEnDusuk: "); Serial.print(enDusukOkuma);
  Serial.print("\tL: "); Serial.print(leftMotorSpeed);
  Serial.print("\tR: "); Serial.print(rightMotorSpeed);
  Serial.print("\tDurum: "); Serial.println(durum);

  // 6. Hareketi Uygula
  if (cizgiVarMi) {
    // Sol Motor
    if (leftMotorSpeed > 20) {
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
      analogWrite(ENA, leftMotorSpeed);
    } else {
      digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
      analogWrite(ENA, 0);
    }

    // Sağ Motor
    if (rightMotorSpeed > 20) {
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
      analogWrite(ENB, rightMotorSpeed);
    } else {
      digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
      analogWrite(ENB, 0);
    }
    
    delay(stepDuration); 
  }
  
  // 7. Motorları Durdur ve Bekle
  motorsStop(); 
  delay(stepWait); 
}

void motorsStop() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}