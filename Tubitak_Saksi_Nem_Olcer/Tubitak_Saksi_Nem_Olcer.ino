#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- WiFi Ayarları ---
const char* ssid = "Ekrem'in Iphonu";
const char* password = "12345...";

// --- MQTT Ayarları ---
const char* mqtt_server = "broker.hivemq.com";
const char* soil_topic = "burak37/toprak_nem"; // Yeni konu başlığı

// --- Donanım Pinleri ---
#define SOIL_PIN A0
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

// Toprak nemi kalibrasyon değerleri (Havadayken ve Su içindeyken ölçülen değerler)
const int KuruDeger = 1023; // Sensör boşta (Kuru)
const int IslakDeger = 400; // Sensör tamamen ıslak

void setup_wifi() {
  delay(10);
  Serial.println("\nWiFi'a baglaniliyor...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi baglandi! IP: " + WiFi.localIP().toString());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT baglantisi deneniyor...");
    String clientId = "ESP8266-SoilClient-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("baglandi!");
    } else {
      Serial.print("hata, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("OLED hatası!"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(10, 25);
  display.println("TOPRAK NEM TAKIP...");
  display.display();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 3000) { // 3 saniyede bir ölçüm yap
    lastMsg = now;

    int analogVeri = analogRead(SOIL_PIN);
    
    // Analog değeri yüzdeye çevir (%0 kuru, %100 çok ıslak)
    int nemYuzdesi = map(analogVeri, KuruDeger, IslakDeger, 0, 100);
    
    // Sınırları koru
    nemYuzdesi = constrain(nemYuzdesi, 0, 100);

    // 1. Seri Port
    Serial.printf("Analog: %d | Nem: %%%d\n", analogVeri, nemYuzdesi);

    // 2. MQTT Yayınlama
    client.publish(soil_topic, String(nemYuzdesi).c_str());

    // 3. OLED Görüntüleme
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Sistem: "); display.print("Online");

    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print("NEM: %"); display.print(nemYuzdesi);

    // Alt kısma durum çubuğu veya uyarı eklenebilir
    display.setTextSize(1);
    display.setCursor(0, 50);
    if(nemYuzdesi < 30) display.print("DURUM: COK KURU!");
    else if(nemYuzdesi < 70) display.print("DURUM: IDEAL");
    else display.print("DURUM: ISLAK");

    display.display();
  }
}