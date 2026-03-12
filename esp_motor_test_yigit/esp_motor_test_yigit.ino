//Bu kod her 3 saniyede bir motor yönünü değiştirir.


#define IN1 14
#define IN2 15
#define IN3 12
#define IN4 13

void setup()
{
Serial.begin(115200);

pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);

Serial.println("PIN TEST BASLADI");
}

void loop()
{

Serial.println("FORWARD");

digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);

digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);

delay(3000);

Serial.println("BACKWARD");

digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);

digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);

delay(3000);

Serial.println("STOP");

digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);

digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);

delay(3000);

}