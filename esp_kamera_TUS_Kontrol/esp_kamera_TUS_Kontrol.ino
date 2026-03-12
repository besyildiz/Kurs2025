#include "esp_camera.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "i5i";
const char* password = "12345...";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

#define RIGHT_EN 2
#define RIGHT_IN1 14
#define RIGHT_IN2 15

#define LEFT_EN 4
#define LEFT_IN1 12
#define LEFT_IN2 13

#define PWM_FREQ 1000
#define PWM_RES 8

#define CH_RIGHT 0
#define CH_LEFT 1

uint32_t speedValue = 150;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<body align=center>

<h2>ESP32 Camera Car TEST</h2>

<img src="" id="cam" width="400"><br><br>

<button onclick="send('F')">Forward</button><br><br>

<button onclick="send('L')">Left</button>
<button onclick="send('R')">Right</button><br><br>

<button onclick="send('B')">Back</button><br><br>

<button onclick="send('S')">Stop</button>

<br><br>

Speed
<input type="range" min="0" max="255" value="150"
oninput="sendSpeed(this.value)">

<script>

var ws = new WebSocket("ws://" + location.hostname + "/ws");

function send(x)
{
ws.send(x);
}

function sendSpeed(v)
{
ws.send("V"+v);
}

</script>

</body>
</html>
)rawliteral";

void printMotorPins()
{

Serial.print("RIGHT: ");
Serial.print(digitalRead(RIGHT_IN1));
Serial.print(" ");
Serial.print(digitalRead(RIGHT_IN2));

Serial.print(" | LEFT: ");
Serial.print(digitalRead(LEFT_IN1));
Serial.print(" ");
Serial.println(digitalRead(LEFT_IN2));

}

void stopMotor()
{

digitalWrite(RIGHT_IN1,LOW);
digitalWrite(RIGHT_IN2,LOW);

digitalWrite(LEFT_IN1,LOW);
digitalWrite(LEFT_IN2,LOW);

Serial.println("STOP");
printMotorPins();

}

void forwardMotor()
{

digitalWrite(RIGHT_IN1,HIGH);
digitalWrite(RIGHT_IN2,LOW);

digitalWrite(LEFT_IN1,HIGH);
digitalWrite(LEFT_IN2,LOW);

Serial.println("FORWARD");
printMotorPins();

}

void backMotor()
{

digitalWrite(RIGHT_IN1,LOW);
digitalWrite(RIGHT_IN2,HIGH);

digitalWrite(LEFT_IN1,LOW);
digitalWrite(LEFT_IN2,HIGH);

Serial.println("BACKWARD");
printMotorPins();

}

void leftMotor()
{

digitalWrite(RIGHT_IN1,HIGH);
digitalWrite(RIGHT_IN2,LOW);

digitalWrite(LEFT_IN1,LOW);
digitalWrite(LEFT_IN2,HIGH);

Serial.println("LEFT");
printMotorPins();

}

void rightMotor()
{

digitalWrite(RIGHT_IN1,LOW);
digitalWrite(RIGHT_IN2,HIGH);

digitalWrite(LEFT_IN1,HIGH);
digitalWrite(LEFT_IN2,LOW);

Serial.println("RIGHT");
printMotorPins();

}

void handleWS(void *arg,uint8_t *data,size_t len)
{

String msg="";

for(int i=0;i<len;i++)
msg+=(char)data[i];

if(msg=="F") forwardMotor();
else if(msg=="B") backMotor();
else if(msg=="L") leftMotor();
else if(msg=="R") rightMotor();
else if(msg=="S") stopMotor();

else if(msg.startsWith("V"))
{

speedValue = msg.substring(1).toInt();

ledcWrite(CH_RIGHT,speedValue);
ledcWrite(CH_LEFT,speedValue);

Serial.print("Speed = ");
Serial.println(speedValue);

}

}

void onEvent(AsyncWebSocket *server,
AsyncWebSocketClient *client,
AwsEventType type,
void *arg,
uint8_t *data,
size_t len)
{

if(type==WS_EVT_DATA)
handleWS(arg,data,len);

}

void setupCamera()
{

camera_config_t config;

config.ledc_channel = LEDC_CHANNEL_0;
config.ledc_timer = LEDC_TIMER_0;

config.pin_d0 = 5;
config.pin_d1 = 18;
config.pin_d2 = 19;
config.pin_d3 = 21;
config.pin_d4 = 36;
config.pin_d5 = 39;
config.pin_d6 = 34;
config.pin_d7 = 35;

config.pin_xclk = 0;
config.pin_pclk = 22;
config.pin_vsync = 25;
config.pin_href = 23;

config.pin_sscb_sda = 26;
config.pin_sscb_scl = 27;

config.pin_pwdn = 32;
config.pin_reset = -1;

config.xclk_freq_hz = 20000000;
config.pixel_format = PIXFORMAT_JPEG;

config.frame_size = FRAMESIZE_QVGA;
config.jpeg_quality = 12;
config.fb_count = 1;

esp_camera_init(&config);

}

void setup()
{

Serial.begin(115200);

pinMode(RIGHT_IN1,OUTPUT);
pinMode(RIGHT_IN2,OUTPUT);
pinMode(LEFT_IN1,OUTPUT);
pinMode(LEFT_IN2,OUTPUT);

ledcSetup(CH_RIGHT,PWM_FREQ,PWM_RES);
ledcSetup(CH_LEFT,PWM_FREQ,PWM_RES);

ledcAttachPin(RIGHT_EN,CH_RIGHT);
ledcAttachPin(LEFT_EN,CH_LEFT);

WiFi.softAP(ssid,password);

Serial.print("IP: ");
Serial.println(WiFi.softAPIP());

ws.onEvent(onEvent);
server.addHandler(&ws);

server.on("/",HTTP_GET,[](AsyncWebServerRequest *request){
request->send_P(200,"text/html",index_html);
});

server.begin();

setupCamera();

Serial.println("SYSTEM READY");

}

void loop()
{
}