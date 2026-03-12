#include "esp_camera.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <iostream>
#include <sstream>

struct MOTOR_PINS
{
  int pinEn;
  int pinIN1;
  int pinIN2;
};

std::vector<MOTOR_PINS> motorPins =
{
  {2,14,15},   // RIGHT MOTOR
  {4,12,13}    // LEFT MOTOR
};

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define STOP 0

#define RIGHT_MOTOR 0
#define LEFT_MOTOR 1

#define FORWARD 1
#define BACKWARD -1

const int PWMFreq = 1000;
const int PWMResolution = 8;

const int RIGHT_MOTOR_CHANNEL = 0;
const int LEFT_MOTOR_CHANNEL  = 1;

//Camera pins
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

const char* ssid     = "i5i";
const char* password = "12345...";

AsyncWebServer server(80);
AsyncWebSocket wsCamera("/Camera");
AsyncWebSocket wsCarInput("/CarInput");

uint32_t cameraClientId = 0;

const char* htmlHomePage PROGMEM = R"HTML(
<!DOCTYPE html>
<html>
<h2>ESP32 Camera Car</h2>
<body align=center>

<img id="cameraImage" style="width:400px">

<br><br>

<button onclick='send("MoveCar","1")'>Forward</button><br><br>
<button onclick='send("MoveCar","3")'>Left</button>
<button onclick='send("MoveCar","4")'>Right</button><br><br>
<button onclick='send("MoveCar","2")'>Back</button><br><br>
<button onclick='send("MoveCar","0")'>Stop</button>

<br><br>

Speed:
<input type="range" min="0" max="255" value="150"
oninput='send("Speed",value)'>

<script>

var wsCam = new WebSocket("ws://" + location.hostname + "/Camera");
wsCam.binaryType = "blob";

wsCam.onmessage = function(event)
{
document.getElementById("cameraImage").src =
URL.createObjectURL(event.data);
}

var wsCar = new WebSocket("ws://" + location.hostname + "/CarInput");

function send(key,value)
{
wsCar.send(key + "," + value);
}

</script>
</body>
</html>
)HTML";

void rotateMotor(int motorNumber, int motorDirection)
{

if (motorDirection == FORWARD)
{
digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
digitalWrite(motorPins[motorNumber].pinIN2, LOW);
}

else if (motorDirection == BACKWARD)
{
digitalWrite(motorPins[motorNumber].pinIN1, LOW);
digitalWrite(motorPins[motorNumber].pinIN2, HIGH);
}

else
{
digitalWrite(motorPins[motorNumber].pinIN1, LOW);
digitalWrite(motorPins[motorNumber].pinIN2, LOW);
}

}

void moveCar(int inputValue)
{

switch(inputValue)
{

case UP:
rotateMotor(RIGHT_MOTOR, FORWARD);
rotateMotor(LEFT_MOTOR, FORWARD);
break;

case DOWN:
rotateMotor(RIGHT_MOTOR, BACKWARD);
rotateMotor(LEFT_MOTOR, BACKWARD);
break;

case LEFT:
rotateMotor(RIGHT_MOTOR, FORWARD);
rotateMotor(LEFT_MOTOR, BACKWARD);
break;

case RIGHT:
rotateMotor(RIGHT_MOTOR, BACKWARD);
rotateMotor(LEFT_MOTOR, FORWARD);
break;

default:
rotateMotor(RIGHT_MOTOR, STOP);
rotateMotor(LEFT_MOTOR, STOP);
break;

}

}

void handleRoot(AsyncWebServerRequest *request)
{
request->send_P(200, "text/html", htmlHomePage);
}

void onCarInputWebSocketEvent(
AsyncWebSocket *server,
AsyncWebSocketClient *client,
AwsEventType type,
void *arg,
uint8_t *data,
size_t len)
{

if(type == WS_EVT_DATA)
{

std::string msg = "";
msg.assign((char*)data,len);

std::istringstream ss(msg);

std::string key;
std::string value;

getline(ss,key,',');
getline(ss,value,',');

int val = atoi(value.c_str());

if(key=="MoveCar")
moveCar(val);

if(key=="Speed")
{
ledcWrite(RIGHT_MOTOR_CHANNEL,val);
ledcWrite(LEFT_MOTOR_CHANNEL,val);
}

}

}

void onCameraWebSocketEvent(
AsyncWebSocket *server,
AsyncWebSocketClient *client,
AwsEventType type,
void *arg,
uint8_t *data,
size_t len)
{

if(type == WS_EVT_CONNECT)
cameraClientId = client->id();

if(type == WS_EVT_DISCONNECT)
cameraClientId = 0;

}

void setupCamera()
{

camera_config_t config;

config.ledc_channel = LEDC_CHANNEL_0;
config.ledc_timer = LEDC_TIMER_0;

config.pin_d0 = Y2_GPIO_NUM;
config.pin_d1 = Y3_GPIO_NUM;
config.pin_d2 = Y4_GPIO_NUM;
config.pin_d3 = Y5_GPIO_NUM;
config.pin_d4 = Y6_GPIO_NUM;
config.pin_d5 = Y7_GPIO_NUM;
config.pin_d6 = Y8_GPIO_NUM;
config.pin_d7 = Y9_GPIO_NUM;

config.pin_xclk = XCLK_GPIO_NUM;
config.pin_pclk = PCLK_GPIO_NUM;
config.pin_vsync = VSYNC_GPIO_NUM;
config.pin_href = HREF_GPIO_NUM;

config.pin_sscb_sda = SIOD_GPIO_NUM;
config.pin_sscb_scl = SIOC_GPIO_NUM;

config.pin_pwdn = PWDN_GPIO_NUM;
config.pin_reset = RESET_GPIO_NUM;

config.xclk_freq_hz = 20000000;
config.pixel_format = PIXFORMAT_JPEG;

config.frame_size = FRAMESIZE_QVGA;
config.jpeg_quality = 12;
config.fb_count = 1;

esp_camera_init(&config);

}

void sendCameraPicture()
{

if(cameraClientId==0)
return;

camera_fb_t * fb = esp_camera_fb_get();

if(!fb)
return;

wsCamera.binary(cameraClientId,fb->buf,fb->len);

esp_camera_fb_return(fb);

}

void setupMotorPins()
{

ledcSetup(RIGHT_MOTOR_CHANNEL,PWMFreq,PWMResolution);
ledcSetup(LEFT_MOTOR_CHANNEL,PWMFreq,PWMResolution);

pinMode(14,OUTPUT);
pinMode(15,OUTPUT);
pinMode(12,OUTPUT);
pinMode(13,OUTPUT);

ledcAttachPin(2,RIGHT_MOTOR_CHANNEL);
ledcAttachPin(4,LEFT_MOTOR_CHANNEL);

moveCar(STOP);

}

void setup()
{

Serial.begin(115200);

setupMotorPins();

WiFi.softAP(ssid,password);

Serial.println(WiFi.softAPIP());

server.on("/",HTTP_GET,handleRoot);

wsCamera.onEvent(onCameraWebSocketEvent);
server.addHandler(&wsCamera);

wsCarInput.onEvent(onCarInputWebSocketEvent);
server.addHandler(&wsCarInput);

server.begin();

setupCamera();

}

void loop()
{

wsCamera.cleanupClients();
wsCarInput.cleanupClients();

sendCameraPicture();

}