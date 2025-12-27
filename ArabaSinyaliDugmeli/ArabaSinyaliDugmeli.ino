 
const int l1 = 10, l2 = 11, l3 = 12, l4 = 13;   
const int r1 = 9, r2 = 8, r3 = 7, r4 = 6;      

 
const int sagButon = 2;    // Sağ  
const int resetButon = 3;  // Reset 
const int solButon = 4;    // Sol   


 
bool solSinyalAktif = false;
bool sagSinyalAktif = false;

 
unsigned long oncekiZaman = 0;
const unsigned long sinyalAraligi = 500;  
bool sinyalDurumu = false;  

void setup() {
  
  pinMode(l1, OUTPUT); pinMode(l2, OUTPUT);
  pinMode(l3, OUTPUT); pinMode(l4, OUTPUT);
  pinMode(r1, OUTPUT); pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT); pinMode(r4, OUTPUT);
  
  
  pinMode(sagButon, INPUT_PULLUP);
  pinMode(solButon, INPUT_PULLUP);
  pinMode(resetButon, INPUT_PULLUP);
  
  
  tumLEDleriKapat();
  
  Serial.begin(9600);  
  Serial.println("Araba Sinyal Sistemi Hazir");
  Serial.println("Sol buton: Sol sinyal");
  Serial.println("Sag buton: Sag sinyal");
  Serial.println("Reset buton: Sinyalleri kapat");
}

void loop() {
 
  bool sagButonBasili = !digitalRead(sagButon);
  bool solButonBasili = !digitalRead(solButon);
  bool resetBasili = !digitalRead(resetButon);
  
  
  if (resetBasili) {
    tumLEDleriKapat();
    solSinyalAktif = false;
    sagSinyalAktif = false;
    Serial.println("Reset: Tum sinyaller kapandi");
    delay(300);  
    return;
  }
  
   
  if (sagButonBasili) {
    if (sagSinyalAktif) {
      // Sağ açıksa kapat
      sagSinyalAktif = false;
      sagLEDleriKapat();
      Serial.println("Sag sinyal: KAPALI");
    } else {
      // Sağ kapalıysa aç
      sagSinyalAktif = true;
      solSinyalAktif = false; // Sol kapat
      solLEDleriKapat();
      Serial.println("Sag sinyal: ACIK");
    }
    delay(300);   // debounce
  }
  
  // Sol   kontrolü - basıldığında sol   aç/kapat
  if (solButonBasili) {
    if (solSinyalAktif) {
      // Sol   zaten açıksa kapat
      solSinyalAktif = false;
      solLEDleriKapat();
      Serial.println("Sol sinyal: KAPALI");
    } else {
      // Sol   kapalıysa aç
      solSinyalAktif = true;
      sagSinyalAktif = false; // Sağ   kapat
      sagLEDleriKapat();
      Serial.println("Sol sinyal: ACIK");
    }
    delay(300); // debounce
  }
  
  // Sinyal yanıp sönme kontrolü
  unsigned long suankiZaman = millis();
  if (suankiZaman - oncekiZaman >= sinyalAraligi) {
    oncekiZaman = suankiZaman;
    
    // tersine çevir
    sinyalDurumu = !sinyalDurumu;
    
    // Sağ  aktifse
    if (sagSinyalAktif) {
      if (sinyalDurumu) {
     
        digitalWrite(r1, HIGH); delay(50);
        digitalWrite(r2, HIGH); delay(50);
        digitalWrite(r3, HIGH); delay(50);
        digitalWrite(r4, HIGH);
      } else {
     
        digitalWrite(r4, LOW); delay(50);
        digitalWrite(r3, LOW); delay(50);
        digitalWrite(r2, LOW); delay(50);
        digitalWrite(r1, LOW);
      }
    }
    
    // Sol  aktifse
    if (solSinyalAktif) {
      if (sinyalDurumu) {
   
        digitalWrite(l1, HIGH); delay(50);
        digitalWrite(l2, HIGH); delay(50);
        digitalWrite(l3, HIGH); delay(50);
        digitalWrite(l4, HIGH);
      } else {
      
        digitalWrite(l4, LOW); delay(50);
        digitalWrite(l3, LOW); delay(50);
        digitalWrite(l2, LOW); delay(50);
        digitalWrite(l1, LOW);
      }
    }
  }
}

// Tüm LED'leri kapatma fonksiyonu
void tumLEDleriKapat() {
  digitalWrite(r1, 0);
  digitalWrite(r2, 0);
  digitalWrite(r3, 0);
  digitalWrite(r4, 0);
  digitalWrite(l1, 0);
  digitalWrite(l2, 0);
  digitalWrite(l3, 0);
  digitalWrite(l4, 0);
}

// Sağ kapatma 
void sagLEDleriKapat() {
  digitalWrite(r1, 0);
  digitalWrite(r2, 0);
  digitalWrite(r3, 0);
  digitalWrite(r4, 0);
}

// Sol kapatma 
void solLEDleriKapat() {
  digitalWrite(l1, 0);
  digitalWrite(l2, 0);
  digitalWrite(l3, 0);
  digitalWrite(l4, 0);
}