#define BUZZER 6

// Notalar (Hz)
#define DO  262
#define RE  294
#define MI  330
#define FA  349
#define SOL 392
#define LA  440
#define SI  494
#define DO2 523

int melody[] = {
  SOL, SOL, LA, SOL, FA, MI,
  SOL, SOL, LA, SOL, FA, MI,
  SOL, SOL, LA, SOL, FA, MI,
  MI, FA, SOL, LA, SOL
};

int durations[] = {
  400, 400, 400, 400, 400, 600,
  400, 400, 400, 400, 400, 600,
  400, 400, 400, 400, 400, 600,
  400, 400, 400, 600, 800
};

void setup() {
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  int noteCount = sizeof(melody) / sizeof(int);

  for (int i = 0; i < noteCount; i++) {
    tone(BUZZER, melody[i], durations[i]);
    delay(durations[i]);
    noTone(BUZZER);
    delay(50);
  }

  delay(3000); // tekrar etmeden önce bekleme
}
