#include <WiFi.h>
#include <PubSubClient.h>

// WiFi Ayarları
const char* ssid = "i6";
const char* password = "12345678";

// MQTT Ayarları
const char* mqtt_server = "broker.hivemq.com";

// Pin ve Konu Ayarları
#define LED_PIN 2
const char* led_topic = "burak37"; // LED kontrol konusu
const char* status_topic = "konu"; // Durum bildirme konusu

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.println("========================================");
  Serial.println("ESP32 WiFi ve MQTT Kontrol Sistemi");
  Serial.println("========================================");
  Serial.println();
  Serial.print("WiFi ağına bağlanıyor: ");
  Serial.println(ssid);
  Serial.println("Bağlanıyor...");

  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    
    if (attempts > 40) {
      Serial.println();
      Serial.println("WiFi bağlantısı zaman aşımı!");
      return;
    }
  }

  Serial.println();
  Serial.println("✅ WiFi'ye BAĞLANDI!");
  Serial.print("📡 IP adresi: ");
  Serial.println(WiFi.localIP());
  Serial.print("📶 Sinyal gücü: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  Serial.println();
}

// MQTT mesajı geldiğinde çalışacak fonksiyon
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("📨 Mesaj geldi [");
  Serial.print(topic);
  Serial.print("]: ");
  
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // LED kontrol mesajı
  if (String(topic) == led_topic) {
    messageTemp.toUpperCase(); // Büyük harfe çevir
    
    if(messageTemp == "ON" || messageTemp == "1"){
      Serial.println("💡 LED AÇIK komutu");
      digitalWrite(LED_PIN, HIGH);
      client.publish(status_topic, "LED AÇIK");
      Serial.println("✅ LED yakıldı");
    }
    else if(messageTemp == "OFF" || messageTemp == "0"){
      Serial.println("💡 LED KAPALI komutu");
      digitalWrite(LED_PIN, LOW);
      client.publish(status_topic, "LED KAPALI");
      Serial.println("✅ LED söndürüldü");
    }
    else {
      Serial.println("❌ Geçersiz komut! Sadece ON veya OFF gönderin.");
      client.publish(status_topic, "Geçersiz komut");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println();
    Serial.print("🔗 MQTT sunucusuna bağlanılıyor...");
    
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("✅ MQTT'ye BAĞLANDI!");
      
      // LED kontrol konusuna abone ol
      client.subscribe(led_topic);
      Serial.print("👂 Abone olundu: ");
      Serial.println(led_topic);
      
      // Başlangıç durumunu bildir
      client.publish(status_topic, "ESP32 Hazır. LED: KAPALI");
      Serial.println("📤 Hazır mesajı gönderildi");
      
    } else {
      Serial.print("❌ MQTT bağlantısı başarısız, yeniden denenecek...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // BAŞLANGIÇTA LED KAPALI
  
  Serial.println();
  Serial.println("🚀 ESP32 Başlatılıyor...");
  Serial.println("💡 LED Pin 2 - Başlangıç: KAPALI");
  
  setup_wifi();
  
  // WiFi bağlı değilse bekle
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ WiFi bağlantısı yok!");
    while(1) {
      // WiFi yoksa LED yanıp sönsün
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(900);
    }
  }
  
  // MQTT broker ayarları
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  // WiFi bağlantısını kontrol et
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi bağlantısı kesildi!");
    digitalWrite(LED_PIN, LOW); // WiFi kesilirse LED'i söndür
    delay(1000);
    return;
  }
  
  // MQTT bağlantısını kontrol et
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();
}