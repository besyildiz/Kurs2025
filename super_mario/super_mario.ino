// Sadece Mario Ses Efektleri
const int sound1 = 6;
const int sound2 = 7;
const int sound3 = 8;

void setup() {
  pinMode(sound1, OUTPUT);
  pinMode(sound2, OUTPUT);
  pinMode(sound3, OUTPUT);
  Serial.begin(9600);
  Serial.println("MARIO SOUND EFFECTS");
  Serial.println("1: Coin");
  Serial.println("2: Jump");
  Serial.println("3: Power-up");
  Serial.println("4: Pipe");
  Serial.println("5: Fireball");
}

void loop() {
  // 1. COIN
  Serial.println("1. COIN SOUND");
  for (int i = 800; i < 1200; i += 30) {
    tone(sound1, i, 20);
    delay(25);
  }
  noTone(sound1);
  delay(1000);
  
  // 2. JUMP
  Serial.println("2. JUMP SOUND");
  tone(sound1, 400, 100);
  tone(sound2, 450, 100);
  delay(120);
  tone(sound1, 600, 80);
  tone(sound2, 650, 80);
  delay(100);
  noTone(sound1);
  noTone(sound2);
  delay(1000);
  
  // 3. POWER-UP
  Serial.println("3. POWER-UP");
  for (int i = 300; i < 900; i += 60) {
    tone(sound1, i, 60);
    tone(sound2, i+30, 60);
    tone(sound3, i+60, 60);
    delay(70);
  }
  noTone(sound1);
  noTone(sound2);
  noTone(sound3);
  delay(1000);
  
  // 4. PIPE
  Serial.println("4. PIPE SOUND");
  for (int i = 400; i > 100; i -= 20) {
    tone(sound2, i, 40);
    delay(45);
  }
  noTone(sound2);
  delay(1000);
  
  // 5. FIREBALL
  Serial.println("5. FIREBALL");
  tone(sound3, 800, 50);
  delay(60);
  tone(sound3, 600, 50);
  delay(60);
  tone(sound3, 400, 50);
  delay(60);
  noTone(sound3);
  
  delay(3000);
}