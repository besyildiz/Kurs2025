#include <Servo.h>

// Pin Tanımlamaları
const int trigPin = 7;
const int echoPin = 6;
const int servoPin = 9;

Servo motor;

// Ayarlar
int engelMesafesi = 20; // Kaç cm'de engel algılansın?
// 45 derece için 200ms ise, 90 derece için yaklaşık 400ms-450ms gerekir.
int donmeSuresi = 450;  // Bu değeri tam 90 dereceyi bulana kadar artırıp azaltabilirsin.
bool engelVarMi = false;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  motor.attach(servoPin);
  
  motor.write(90); // Başlangıçta motoru durdur
  Serial.begin(9600);
}

void loop() {
  long sure, mesafe;

  // Mesafe ölçümü yapılıyor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5); // Daha stabil okuma için 5ms yaptık
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // pulseIn fonksiyonuna 30000ms timeout ekledik, sensör kilitlenmesin diye
  sure = pulseIn(echoPin, HIGH, 30000); 
  mesafe = (sure / 2) / 29.1;

  Serial.print("Mesafe: ");
  Serial.print(mesafe);
  Serial.println(" cm");

  // ENGEL ALGILANDIĞINDA
  if (mesafe < engelMesafesi && mesafe > 0 && !engelVarMi) {
    Serial.println("Engel algilandi! 90 derece donuyor...");
    motor.write(110);      // Bir yöne dönüş hızı
    delay(donmeSuresi);    // 90 dereceye tekabül eden süre
    motor.write(90);       // Dur
    engelVarMi = true;     
  } 
  
  // ENGEL KALKTIĞINDA
  else if ((mesafe >= engelMesafesi || mesafe == 0) && engelVarMi) {
    // mesafe == 0 durumu bazen sensörün menzil dışı kalmasıdır, hata olmasın diye ekledik
    Serial.println("Engel kalkti! Eski konumuna donuyor...");
    motor.write(70);       // Ters yöne dönüş hızı
    delay(donmeSuresi);    // Aynı süre kadar geri dön
    motor.write(90);       // Dur
    engelVarMi = false;    
  }

  delay(200); // Sensörün ve seri portun rahatlaması için süreyi biraz artırdık
}