#define trigPin 6
#define echoPin 7

#define led1 8
#define led2 9

long sure;
int mesafe;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  // Mesafe ölç
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  sure = pulseIn(echoPin, HIGH, 25000); // timeout eklendi
  mesafe = sure * 0.034 / 2;

Serial.print("Olculen Mesafe=");
Serial.println(mesafe);
 
 
if(mesafe<=15 && mesafe>0)
{digitalWrite(led1,1);digitalWrite(led2,1);}
else
if(mesafe>15 && mesafe<30)
{digitalWrite(led1,1);digitalWrite(led2,0);}
else if(mesafe>30 || mesafe==0)
{digitalWrite(led1,0);digitalWrite(led2,0);}

 
  delay(10);
}


