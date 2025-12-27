// SUPER MARIO - KALDIĞIN YERDEN DEVAM
const int buzzer1 = 6;
const int buzzer2 = 7;
const int buzzer3 = 8;
const int button = 3;

bool isPlaying = false;
bool wasPlaying = false;
int currentNote = 0;
unsigned long noteStartTime = 0;
int noteDuration = 0;

#define MI  330
#define SOL 392
#define DO2 523

int melody[] = {MI, MI, MI, DO2, MI, SOL, SOL, DO2};
int durations[] = {500, 500, 500, 500, 500, 800, 800, 800};

void setup() {
  pinMode(buzzer1, OUTPUT);
  pinMode(buzzer2, OUTPUT);
  pinMode(buzzer3, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Mario - Kaldığın Yerden Devam");
}

void loop() {
  // Buton kontrolü
  if (digitalRead(button) == LOW) {
    delay(300); // Debounce
    isPlaying = !isPlaying;
    
    if (isPlaying) {
      Serial.println("▶️  Çalıyor...");
      noteStartTime = millis(); // Nota başlangıç zamanı
    } else {
      Serial.println("⏸️  Duraklatıldı");
      allNotesOff();
    }
  }
  
  // Müzik çalma mantığı
  if (isPlaying) {
    if (currentNote < 8) {
      noteDuration = durations[currentNote];
      
      // Nota süresi doldu mu?
      if (millis() - noteStartTime >= noteDuration) {
        // Sonraki nota
        currentNote++;
        noteStartTime = millis();
        
        if (currentNote >= 8) {
          currentNote = 0; // Başa sar
        }
      }
      
      // Notayı çal
      playCurrentNote();
    }
  }
  
  delay(10);
}

void playCurrentNote() {
  tone(buzzer1, melody[currentNote]);
  
  // Efektler
  if (currentNote % 2 == 0) {
    tone(buzzer2, melody[currentNote] + 100);
  }
  
  if (currentNote == 4 || currentNote == 6) {
    tone(buzzer3, 800);
  }
}

void allNotesOff() {
  noTone(buzzer1);
  noTone(buzzer2);
  noTone(buzzer3);
}