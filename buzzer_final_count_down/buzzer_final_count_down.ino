// Europe - The Final Countdown
// Optimize 3 Buzzer Versiyonu

const int lead = 6;    // Lead synth (ana melodi)
const int bass = 7;    // Bass line
const int chord = 8;   // Chord/efekt

// Notalar
#define DO  262
#define RE  294
#define MI  330
#define FA  349
#define SOL 392
#define LA  440
#define SI  494
#define DO2 523
#define RE2 587
#define MI2 659

void setup() {
  pinMode(lead, OUTPUT);
  pinMode(bass, OUTPUT);
  pinMode(chord, OUTPUT);
  Serial.begin(9600);
  Serial.println("THE FINAL COUNTDOWN - 3 BUZZER");
}

void playNote(int note, int duration) {
  tone(lead, note, duration);
  
  // Bass: root note'un 1 oktav pesi
  tone(bass, note/2, duration/2);
  
  // Chord: 5'li aralık
  if (duration > 300) {
    tone(chord, note * 1.5, duration/3);
  }
  
  delay(duration + 80);
  
  noTone(lead);
  noTone(bass);
  noTone(chord);
  
  delay(40);
}

void loop() {
  Serial.println("=== INTRO ===");
  
  // İkonik intro
  playNote(DO2, 350);
  playNote(SI, 350);
  playNote(LA, 350);
  playNote(SOL, 350);
  playNote(FA, 350);
  playNote(MI, 350);
  playNote(RE, 350);
  playNote(DO, 700);
  
  Serial.println("=== REPEAT ===");
  playNote(DO2, 350);
  playNote(SI, 350);
  playNote(LA, 350);
  playNote(SOL, 350);
  playNote(FA, 350);
  playNote(MI, 350);
  playNote(RE, 350);
  playNote(DO, 700);
  
  Serial.println("=== BUILD UP ===");
  playNote(DO2, 350);
  playNote(SI, 350);
  playNote(LA, 350);
  playNote(SOL, 350);
  playNote(FA, 350);
  playNote(MI, 350);
  playNote(RE, 350);
  playNote(DO, 700);
  
  Serial.println("=== ASCENDING ===");
  playNote(MI, 300);
  playNote(FA, 300);
  playNote(SOL, 300);
  playNote(LA, 300);
  playNote(SI, 300);
  playNote(DO2, 300);
  playNote(RE2, 300);
  playNote(MI2, 1200);
  
  Serial.println("=== OUTRO ===");
  // Son akor
  tone(lead, MI2, 500);
  tone(bass, MI, 500);
  tone(chord, SI, 500);
  delay(800);
  
  noTone(lead);
  noTone(bass);
  noTone(chord);
  
  Serial.println("=== FINISHED ===");
  delay(8000); // 8 saniye bekle
}