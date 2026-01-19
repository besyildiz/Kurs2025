/*
 * SABAH-AKŞAM ALGILAMA SİSTEMİ
 * HC-SR04 Ultrasonik Mesafe Sensörlü
 * Tinkercad Uyumlu
 * 
 * ÇALIŞMA PRENSİBİ:
 * - Sabah (aydınlık): Hareket algılanırsa BUZZER öter
 * - Akşam (karanlık): Hareket algılanırsa LED'ler yanar
 */

// ========== PİN TANIMLAMALARI ==========
// Mesafe Sensörü (HC-SR04)
const int trigPin = 9;
const int echoPin = 10;

// Diğer Sensör ve Aktüatörler
const int ldrPin = A0;          // LDR ışık sensörü
const int buzzerPin = 7;        // Buzzer
const int ledPins[] = {3, 4, 5, 6};  // LED pinleri
const int ledCount = 4;         // LED sayısı

// ========== AYARLANABİLİR PARAMETRELER ==========
int esikDegeri = 500;           // Işık eşiği (LDR) - AYARLAYIN!
int mesafeEsik = 80;            // Algılama mesafesi (cm) - AYARLAYIN!
const unsigned long beklemeSuresi = 5000; // Aktif kalma süresi (ms)

// ========== DEĞİŞKENLER ==========
long mesafe;
int ldrDegeri;
bool hareketAlgilandi = false;
unsigned long hareketZamani = 0;

// ========== SETUP ==========
void setup() {
  // Seri iletişimi başlat
  Serial.begin(9600);
  Serial.println("========================================");
  Serial.println("SABAH-AKŞAM ALGILAMA SİSTEMİ");
  Serial.println("HC-SR04 Ultrasonik Sensör ile");
  Serial.println("========================================");
  
  // Pin modlarını ayarla
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // LED pinlerini çıkış yap ve kapat
  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  
  // Buzzer'ı kapat
  noTone(buzzerPin);
  
  Serial.println("Sistem başlatıldı. Seri monitörden değerleri izleyin.");
  Serial.println("LDR < " + String(esikDegeri) + " : SABAH (Buzzer)");
  Serial.println("LDR >= " + String(esikDegeri) + " : AKŞAM (LED)");
  Serial.println("Mesafe < " + String(mesafeEsik) + " cm : Hareket");
  Serial.println();
}

// ========== ANA DÖNGÜ ==========
void loop() {
  // 1. SENSÖRLERDEN VERİ OKU
  mesafe = mesafeOlc();        // Mesafeyi ölç
  ldrDegeri = analogRead(ldrPin); // Işık şiddetini oku
  
  // 2. SERİ MONİTÖRE YAZDIR (Debug)
  Serial.print("LDR: ");
  Serial.print(ldrDegeri);
  if (ldrDegeri >= esikDegeri) {
    Serial.print(" [KARANLIK]");
  } else {
    Serial.print(" [AYDINLIK]");
  }
  
  Serial.print(" | Mesafe: ");
  if (mesafe <= 0) {
    Serial.print("OKUNAMADI");
  } else {
    Serial.print(mesafe);
    Serial.print(" cm");
  }
  
  Serial.print(" | Durum: ");
  
  // 3. HAREKET KONTROLÜ (2-400cm arası geçerli)
  if (mesafe > 0 && mesafe < mesafeEsik && mesafe < 400) {
    // HAREKET ALGILANDI!
    if (!hareketAlgilandi) {
      hareketAlgilandi = true;
      hareketZamani = millis();
      
      // IŞIK DURUMUNA GÖRE İŞLEM YAP
      if (ldrDegeri <= esikDegeri) {
        // AKŞAM MODU (KARANLIK) -> LED'ler YANAR
        Serial.println("AKŞAM - LED'ler YANIYOR");
        aksamModu();
      } else {
        // SABAH MODU (AYDINLIK) -> BUZZER ÖTER
        Serial.println("SABAH - BUZZER ÇALIYOR");
        sabahModu();
      }
    } else {
      Serial.println("Hareket devam ediyor...");
    }
  } else {
    // HAREKET YOK veya MESAFE ÇOK UZAK
    if (hareketAlgilandi) {
      // Hareket biteli belli süre oldu mu kontrol et
      if (millis() - hareketZamani > beklemeSuresi) {
        sistemSifirla();
        Serial.println("Sistem sıfırlandı");
      } else {
        Serial.println("Beklemede (aktif)");
      }
    } else {
      Serial.println("Beklemede");
    }
  }
  
  delay(100);  // 100ms ölçüm aralığı
}

// ========== FONKSİYONLAR ==========

// MESAFE ÖLÇME FONKSİYONU (HC-SR04 için)
long mesafeOlc() {
  long sure, uzaklik;
  
  // Trig pinini temizle
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // 10 mikrosaniye HIGH sinyal gönder
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Echo pininden gelen sinyalin süresini ölç
  sure = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  
  // Süreyi cm'ye çevir (ses hızı = 340 m/s = 0.034 cm/µs)
  // Gidiş-dönüş olduğu için 2'ye böl
  uzaklik = sure * 0.034 / 2;
  
  // Hatalı okumalar için kontrol
  if (uzaklik <= 0 || uzaklik > 400) {
    return -1;  // Geçersiz okuma
  }
  
  return uzaklik;
}

// AKŞAM MODU - LED'leri yak (efektli)
void aksamModu() {
  Serial.println("  >> Akşam modu aktif: LED'ler yanıyor");
  
  // Önce tüm LED'leri kapat
  for (int i = 0; i < ledCount; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  
  // Buzzer'ı kapat
  noTone(buzzerPin);
  
  // 1. Sırayla yakma efekti
  Serial.println("  >> Sırayla yakılıyor...");
  for (int i = 0; i < ledCount; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(150);  // Efekt süresi
  }
  
  // 2. Hafif yanıp sönme efekti
  Serial.println("  >> Yanıp sönme efekti...");
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < ledCount; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    delay(100);
    for (int i = 0; i < ledCount; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
    delay(100);
  }
  
  // 3. Sabit yanık kal
  Serial.println("  >> Sabit yanık kalacak...");
}

// SABAH MODU - Buzzer çal
void sabahModu() {
  Serial.println("  >> Sabah modu aktif: Buzzer çalıyor");
  
  // LED'leri kapat
  for (int i = 0; i < ledCount; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  
  // Sabah uyanma melodisi
  // 1. Kısa uyarı sesleri
  Serial.println("  >> Sabah melodisi başlıyor...");
  for (int i = 0; i < 3; i++) {
    tone(buzzerPin, 1200, 200);  // Yüksek frekans
    delay(300);
    noTone(buzzerPin);
    delay(100);
  }
  
  // 2. Uzun uyarı
  tone(buzzerPin, 800, 1000);    // Alçak frekans, uzun
  delay(1200);
  
  // 3. Sürekli uyarı (bekleme süresi bitene kadar)
  tone(buzzerPin, 1000);
}

// SİSTEM SIFIRLAMA
void sistemSifirla() {
  Serial.println("  >> Sistem sıfırlanıyor...");
  
  // Tüm LED'leri kapat
  for (int i = 0; i < ledCount; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  
  // Buzzer'ı kapat
  noTone(buzzerPin);
  
  // Bayrağı sıfırla
  hareketAlgilandi = false;
}

// ========== KALİBRASYON ve TEST FONKSİYONLARI ==========

// SİSTEM TESTİ - Tüm bileşenleri test et
void sistemTesti() {
  Serial.println("\n=== SİSTEM TESTİ BAŞLIYOR ===");
  
  // 1. LED TESTİ
  Serial.println("1. LED Testi:");
  for (int i = 0; i < ledCount; i++) {
    Serial.print("   LED");
    Serial.print(i+1);
    Serial.println(" yanıyor...");
    digitalWrite(ledPins[i], HIGH);
    delay(500);
    digitalWrite(ledPins[i], LOW);
    delay(200);
  }
  
  // 2. BUZZER TESTİ
  Serial.println("2. Buzzer Testi:");
  tone(buzzerPin, 1000, 500);
  Serial.println("   Buzzer çalıyor...");
  delay(1000);
  noTone(buzzerPin);
  
  // 3. LDR TESTİ
  Serial.println("3. LDR Testi (5 ölçüm):");
  for (int i = 0; i < 5; i++) {
    ldrDegeri = analogRead(ldrPin);
    Serial.print("   Ölçüm ");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.print(ldrDegeri);
    Serial.print(" - ");
    if (ldrDegeri >= esikDegeri) {
      Serial.println("KARANLIK (Akşam)");
    } else {
      Serial.println("AYDINLIK (Sabah)");
    }
    delay(1000);
  }
  
  // 4. MESAFE SENSÖRÜ TESTİ
  Serial.println("4. Mesafe Sensörü Testi (10 ölçüm):");
  Serial.println("   Sensör önüne elinizi yaklaştırıp uzaklaştırın");
  for (int i = 0; i < 10; i++) {
    mesafe = mesafeOlc();
    Serial.print("   Ölçüm ");
    Serial.print(i+1);
    Serial.print(": ");
    if (mesafe <= 0) {
      Serial.println("OKUNAMADI");
    } else {
      Serial.print(mesafe);
      Serial.print(" cm - ");
      if (mesafe < mesafeEsik) {
        Serial.println("HAREKET ALGILANDI");
      } else {
        Serial.println("hareket yok");
      }
    }
    delay(800);
  }
  
  Serial.println("=== SİSTEM TESTİ TAMAMLANDI ===\n");
  delay(2000);
}

// LDR KALİBRASYONU - Işık eşiğini bul
void ldrKalibrasyon() {
  Serial.println("\n=== LDR KALİBRASYONU ===");
  Serial.println("1. Ortamı AYDINLIK yapın (Sabah modu)");
  Serial.println("   ENTER tuşuna basın...");
  while(!Serial.available());
  Serial.read();
  
  int toplam = 0;
  for (int i = 0; i < 10; i++) {
    toplam += analogRead(ldrPin);
    delay(200);
  }
  int aydinlikOrtalama = toplam / 10;
  Serial.print("   Aydınlık ortalama: ");
  Serial.println(aydinlikOrtalama);
  
  Serial.println("\n2. Ortamı KARANLIK yapın (Akşam modu)");
  Serial.println("   ENTER tuşuna basın...");
  while(!Serial.available());
  Serial.read();
  
  toplam = 0;
  for (int i = 0; i < 10; i++) {
    toplam += analogRead(ldrPin);
    delay(200);
  }
  int karanlikOrtalama = toplam / 10;
  Serial.print("   Karanlık ortalama: ");
  Serial.println(karanlikOrtalama);
  
  // Eşik değerini otomatik hesapla
  esikDegeri = (aydinlikOrtalama + karanlikOrtalama) / 2;
  
  Serial.println("\n=== KALİBRASYON SONUÇLARI ===");
  Serial.print("Aydınlık (Sabah): ");
  Serial.println(aydinlikOrtalama);
  Serial.print("Karanlık (Akşam): ");
  Serial.println(karanlikOrtalama);
  Serial.print("ÖNERİLEN EŞİK DEĞERİ: ");
  Serial.println(esikDegeri);
  Serial.print("Kodda şu satırı güncelleyin: ");
  Serial.print("int esikDegeri = ");
  Serial.print(esikDegeri);
  Serial.println(";\n");
}