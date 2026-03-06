int sensorPins[6] = {A0, A1, A2, A3, A4, A5};
int threshold = 0;  // otomatik hesaplanacak
int calibrationTime = 3000; // 3 saniye kalibrasyon süresi

void setup() {
  Serial.begin(9600);
  Serial.println("CIZGI TEST BASLADI");

  // Otomatik kalibrasyon
  calibrateSensors();
}

void loop() {
  int values[6];
  int sum = 0;
  int weightedSum = 0;

  Serial.print("Ham: ");

  for (int i = 0; i < 6; i++) {
    values[i] = analogRead(sensorPins[i]);
    Serial.print(values[i]);
    Serial.print("\t");

    // Siyah algılama (düşük değer siyah varsayımı)
    if (values[i] < threshold) {
      sum++;
      weightedSum += i * 1000;  
    }
  }

  Serial.print(" | ");

  if (sum > 0) {
    int position = weightedSum / sum;
    Serial.print("Pozisyon: ");
    Serial.print(position);
  } else {
    Serial.print("Cizgi Yok");
  }

  Serial.println();
  delay(150);
}

// ------------------------
// Sensörleri otomatik kalibre eden fonksiyon
// ------------------------
void calibrateSensors() {
  long whiteSum[6] = {0,0,0,0,0,0};
  long blackSum[6] = {0,0,0,0,0,0};
  int samples = 50;

  Serial.println("Beyaz zemin üzerinde bırakın...");
  delay(2000);

  // Beyaz zemin ölçümü
  for (int s = 0; s < samples; s++) {
    for (int i = 0; i < 6; i++) {
      whiteSum[i] += analogRead(sensorPins[i]);
    }
    delay(20);
  }

  Serial.println("Şimdi sensörü siyah çizgi üzerine yerleştirin...");
  delay(2000);

  // Siyah zemin ölçümü
  for (int s = 0; s < samples; s++) {
    for (int i = 0; i < 6; i++) {
      blackSum[i] += analogRead(sensorPins[i]);
    }
    delay(20);
  }

  // Ortalama hesapla
  long whiteAvg = 0;
  long blackAvg = 0;
  for (int i = 0; i < 6; i++) {
    whiteAvg += whiteSum[i] / samples;
    blackAvg += blackSum[i] / samples;
  }
  whiteAvg /= 6;
  blackAvg /= 6;

  // Threshold = beyaz ve siyah ortalaması
  threshold = (whiteAvg + blackAvg) / 2;

  Serial.print("Kalibrasyon tamamlandı. Threshold = ");
  Serial.println(threshold);
}