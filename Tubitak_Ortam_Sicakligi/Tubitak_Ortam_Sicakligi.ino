#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- WiFi Ayarları ---
const char* ssid = "Ekrem'in Iphonu";
const char* password = "12345...";

// --- MQTT Ayarları ---
const char* mqtt_server = "broker.hivemq.com"; // Ücretsiz genel broker
const char* temp_topic = "burak37/sicaklik";
const char* hum_topic = "burak37/nem";

// --- Donanım Pinleri ---
#define DHTPIN D4
#define DHTTYPE DHT22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Nesne Tanımlamaları
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

// Wi-Fi Bağlantı Fonksiyonu
void setup_wifi() {
  delay(10);
  Serial.println("\n---------------------------------");
  Serial.print(ssid);
  Serial.println(" agina baglaniliyor...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi baglandi!");
  Serial.print("IP Adresi: ");
  Serial.println(WiFi.localIP());
}

// MQTT Yeniden Bağlanma Fonksiyonu
void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT baglantisi deneniyor...");
    // Benzersiz bir ID oluşturma
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("baglandi!");
    } else {
      Serial.print("hata, rc=");
      Serial.print(client.state());
      Serial.println(" 5 saniye sonra tekrar denenecek");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // OLED Başlatma
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("OLED baslatilamadi"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Sistem Basliyor...");
  display.display();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  // MQTT Bağlantı Kontrolü
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  // 5 saniyede bir veri oku ve yayınla
  if (now - lastMsg > 5000) {
    lastMsg = now;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Sensorden veri okunamadi!");
      return;
    }

    // --- 1. Seri Port Çıktısı ---
    Serial.print("Sicaklik: "); Serial.print(t);
    Serial.print(" C | Nem: %"); Serial.println(h);

    // --- 2. MQTT Yayınlama (Publish) ---
    client.publish(temp_topic, String(t).c_str());
    client.publish(hum_topic, String(h).c_str());

    // --- 3. OLED Ekranda Görüntüleme ---
    display.clearDisplay();
    
    // IP ve Durum Satırı
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("IP: "); display.print(WiFi.localIP());

    // Sıcaklık
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print("ISI: "); display.print(t, 1); display.print("C");

    // Nem
    display.setCursor(0, 45);
    display.print("NEM: %"); display.print(h, 0);

    display.display();
  }
}