#define sensorPin A0  // Sensörün bağlı olduğu analog pini tanımlıyoruz

void setup() {
  Serial.begin(9600);  // Seri haberleşmeyi 9600 baud hızında başlat
  pinMode(sensorPin, INPUT); // Sensor pinini giriş olarak ayarla (isteğe bağlı)
}

void loop() {
  // 1. Sensörden analog değeri oku (0 - 1023 arası)
  int reading = analogRead(sensorPin);

  // 2. Okunan değeri voltaja çevir.
  // Arduino 5V ile çalıştığı için referans voltajımız 5V.
  // 1023 birimi 5V'a eşittir.
  float voltage = (reading / 1023.0) * 5.0;

  // 3. Voltajı sıcaklığa çevir (10mV = 1°C)
  float temperatureC = voltage * 100.0;

  // 4. İstersek Fahrenheit'e de çevirebiliriz
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

  // 5. Sonuçları seri porta yazdır
  Serial.print("Voltaj: ");
  Serial.print(voltage);
  Serial.print(" V,  Sıcaklık: ");
  Serial.print(temperatureC);
  Serial.print(" °C,  ");
  Serial.print(temperatureF);
  Serial.println(" °F");

  delay(1000); // 1 saniye bekle
}