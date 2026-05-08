#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- WiFi Ayarları ---
const char* ssid = "Ekrem'in Iphonu";
const char* password = "12345...";

// --- MQTT Ayarları ---
const char* mqtt_server = "broker.hivemq.com";
const char* temp_topic = "burak37/sicaklik";
const char* hum_topic = "burak37/nem";

// --- Donanım Pinleri ---
#define DHTPIN D4
#define DHTTYPE DHT22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

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
  Serial.println("\nWiFi baglandi! IP: " + WiFi.localIP().toString());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT baglantisi deneniyor...");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
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
  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("OLED hatası!"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(20, 25);
  display.println("SISTEM ACILIYOR...");
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
  if (now - lastMsg > 5000) { // 5 saniyede bir güncelle
    lastMsg = now;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Sensorden veri okunamadi! Baglantiyi kontrol et.");
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("SENSOR HATASI!");
      display.display();
      return;
    }

    // 1. Seri Port Çıktısı
    Serial.printf("Sicaklik: %.1f C | Nem: %%%.0f\n", t, h);

    // 2. MQTT Yayınlama
    client.publish(temp_topic, String(t).c_str());
    client.publish(hum_topic, String(h).c_str());

    // 3. OLED Görüntüleme
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("WiFi: "); display.print(ssid);

    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print("ISI: "); display.print(t, 1); display.print("C");

    display.setCursor(0, 45);
    display.print("NEM: %"); display.print(h, 0);
    display.display();
  }
}