int sensorPins[6] = {A0, A1, A2, A3, A4, A5};

void setup() {
  Serial.begin(9600);
  Serial.println("QTR-8A UNO TEST");
}

void loop() {

  for (int i = 0; i < 6; i++) {
    int value = analogRead(sensorPins[i]);
    Serial.print(value);
    Serial.print("\t");
  }

  Serial.println();
  delay(150);
}