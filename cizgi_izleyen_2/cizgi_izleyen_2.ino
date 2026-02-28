int sensorPins[6] = {A0, A1, A2, A3, A4, A5};

int threshold = 500;   // İlk başta 500, sonra ayarlayacağız

void setup() {
  Serial.begin(9600);
  Serial.println("CIZGI TEST BASLADI");
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