int sensorPins[6] = {A0, A1, A2, A3, A4, A5};
int threshold = 500;

void setup() {
  Serial.begin(9600);
}

void loop() {

  for (int i = 0; i < 6; i++) {

    int value = analogRead(sensorPins[i]);

    if (value < threshold) {
      Serial.print("1 ");
    } else {
      Serial.print("0 ");
    }

  }

  Serial.println();
  delay(200);
}