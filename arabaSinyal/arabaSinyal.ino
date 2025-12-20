 int l1=10, l2=11,l3=12,l4=13;
 int r1=9, r2=8,r3=7,r4=6;

void setup()
{
pinMode(l1,OUTPUT);  pinMode(l2,OUTPUT);
  pinMode(l3,OUTPUT);  pinMode(l4,OUTPUT);
  
  pinMode(r1,OUTPUT);  pinMode(r2,OUTPUT);
  pinMode(r3,OUTPUT);  pinMode(r4,OUTPUT);
}


void loop()
{
digitalWrite(r1,1);delay(50);
  digitalWrite(r2,1);delay(50);
  digitalWrite(r3,1);delay(50);
  digitalWrite(r4,1);delay(50);
    digitalWrite(r1,0);
  digitalWrite(r2,0); 
  digitalWrite(r3,0);
  digitalWrite(r4,0);
  delay(1000);
  
  
  digitalWrite(l1,1);delay(50);
  digitalWrite(l2,1);delay(50);
  digitalWrite(l3,1);delay(50);
  digitalWrite(l4,1);delay(50);
    digitalWrite(l1,0);
  digitalWrite(l2,0); 
  digitalWrite(l3,0);
  digitalWrite(l4,0);
  delay(1000);
  
}