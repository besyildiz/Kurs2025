#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int butonPin = A0;

String menuItems[] = {"1. Saat", "2. Kronometre", "3. Hesap Mak.", "4. Oyun", "5. Ayarlar"};
int menuIndex = 0;

// BUTON OKUMA FONKSİYONU - EN BAŞA TANIMLA
int butonOku() {
  int deger = analogRead(butonPin);
  if (deger < 50) return 1;      // RIGHT
  else if (deger < 200) return 2;  // UP
  else if (deger < 400) return 3;  // DOWN
  else if (deger < 600) return 4;  // LEFT
  else if (deger < 800) return 5;  // SELECT
  else return 0;                   // NONE
}

void setup() {
  lcd.begin(16, 2);
  lcd.print("  ANA MENU");
  delay(1000);
  lcd.clear();
}

void loop() {
  int buton = butonOku();
  
  if (buton == 2) { // UP
    menuIndex--;
    if (menuIndex < 0) menuIndex = 4;
  }
  else if (buton == 3) { // DOWN
    menuIndex++;
    if (menuIndex > 4) menuIndex = 0;
  }
  else if (buton == 5) { // SELECT
    seciliMenuCalistir();
    return;
  }
  
  ekraniGuncelle();
  delay(200);
}

void ekraniGuncelle() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">");
  lcd.print(menuItems[menuIndex].substring(0, 15));
  
  if (menuIndex < 4) {
    lcd.setCursor(1, 1);
    lcd.print(menuItems[menuIndex + 1].substring(0, 15));
  }
}

void seciliMenuCalistir() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Secilen: ");
  lcd.print(menuIndex + 1);
  delay(2000);
  
  switch(menuIndex) {
    case 0: saatModu(); break;
    case 1: kronometreModu(); break;
    case 2: hesapMakinesi(); break;
    case 3: oyunModu(); break;
    case 4: ayarlarModu(); break;
  }
}

// MOD FONKSİYONLARI (Basit versiyonları)
void saatModu() {
  lcd.clear();
  lcd.print("Saat Modu");
  lcd.setCursor(0, 1);
  lcd.print("Gelistirilecek");
  delay(2000);
}

void kronometreModu() {
  lcd.clear();
  lcd.print("Kronometre");
  lcd.setCursor(0, 1);
  lcd.print("Gelistirilecek");
  delay(2000);
}

void hesapMakinesi() {
  lcd.clear();
  lcd.print("Hesap Makinesi");
  lcd.setCursor(0, 1);
  lcd.print("Gelistirilecek");
  delay(2000);
}

void oyunModu() {
  lcd.clear();
  lcd.print("Oyun Modu");
  lcd.setCursor(0, 1);
  lcd.print("Gelistirilecek");
  delay(2000);
}

void ayarlarModu() {
  lcd.clear();
  lcd.print("Ayarlar");
  lcd.setCursor(0, 1);
  lcd.print("Gelistirilecek");
  delay(2000);
}