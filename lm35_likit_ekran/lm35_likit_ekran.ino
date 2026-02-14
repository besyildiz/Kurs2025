#include <Wire.h>
#include "DFRobot_RGBLCD1602.h"

DFRobot_RGBLCD1602 lcd(16, 2);

const int lm35Pin = A0;

void setup() {
  Serial.begin(9600);
  Serial.println("LM35 + LCD Test Basladi");

  lcd.init();
  lcd.setRGB(0, 255, 0);   // Yesil arka isik
  lcd.setCursor(0, 0);
  lcd.print("Sicaklik:");
}

void loop() {

  int analogValue = analogRead(lm35Pin);

 float voltage = analogValue * (2.1 / 1023.0);
float temperature = voltage * 100.0;

  // ----- SERIAL MONITOR -----
  Serial.print("Analog: ");
  Serial.print(analogValue);
  Serial.print(" | Volt: ");
  Serial.print(voltage, 3);
  Serial.print(" V | Sicaklik: ");
  Serial.print(temperature, 1);
  Serial.println(" C");

  // ----- LCD -----
  lcd.setCursor(0, 1);
  lcd.print(temperature, 1);
  lcd.print(" C   ");   // eski rakamlari temizlemek icin

  // Renkli geri bildirim (ders icin guzel)
  if (temperature >= 30) {
    lcd.setRGB(255, 0, 0);   // Kirmizi
  } else {
    lcd.setRGB(0, 255, 0);   // Yesil
  }

  delay(1000);
}
