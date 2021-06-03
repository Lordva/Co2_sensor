#include <WiFi.h>
#include <AsyncMqttClient.h>
#include "config.h"

int Gaz=34;

//TODO :
//  sensor calibration (dynamic value) check w/ api with history

static const char* ssid = SSID;
static const char* password = PASSWORD;
static const IPAddress MQTTHost = HOST;
static const int MQTTPort = PORT;

String location = Location;

unsigned long lastTime = 0;
unsigned long sendInterval = 60000;

//smooting variables
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

AsyncMqttClient mqttClient;

void setup(){
  //Start Serial
  Serial.begin(9600);
  while (!Serial)
  {
    ; //wait for serial connexion (for debug)
  }
  
  pinMode(Gaz, INPUT);
  //Start WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  initWifi(ssid, password);

  initMQTT();

  // init all readings to 0
  for(int thisReading = 0; thisReading < numReadings; thisReading++){
    readings[thisReading] = 0;
  }
}

void loop(){
  // read sensor value
  int gazsensor = Smoothing(Gaz);

  Serial.print("reading: ");
  Serial.println(gazsensor);

  delay(100);
}

// send data to API
void SendData(String _host, int _value, String _location){

  if((millis() - lastTime) > sendInterval){
    Serial.println("Sending data to MQTT brocker");
    if(WiFi.status() == WL_CONNECTED){
      mqttClient.publish("esp32/airquality", 1, true, String(_value).c_str());
  }
    else{
    Serial.println("Wifi disconnected !");
    }
  }
  lastTime = millis();
}

// connect to wifi
void initWifi(const char* _SSID, const char* _pwd){
  int timer = 1000000 + millis();

  //Scan Wifi at range Used for debug
  //ScanWifi();

  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  Serial.print("Using passord: ");
  Serial.println(_pwd);

  WiFi.mode(WIFI_STA);
  WiFi.begin(_SSID, _pwd);

  Serial.print("Connexion au Wifi ..");

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    if(WiFi.status() == WL_CONNECT_FAILED){
      Serial.print(WiFi.status());
      Serial.println("[ERROR] Connection failed");
    }
    if(millis() > timer){
      Serial.println("[ERROR] Timeout...");
      break;
    }
    delay(1000);
  }
  Serial.print("Connecter au Wifi avec l'addresse : ");
  Serial.println(WiFi.localIP());
}

void initMQTT(){
  Serial.println("Connecting to MQTT...");
  mqttClient.setServer(MQTTHost, MQTTPort);
  mqttClient.connect();
}

void ScanWifi(){
  Serial.println("Scanning for networks...");
  int n = WiFi.scanNetworks();
  if(n == 0){
    Serial.println("[WARN] No network found");
  } else{
    Serial.print(n);
    Serial.print(" networks found");
    for (int i = 0; i < n; ++i){
      Serial.print(i+1);
      Serial.print(": ");
      Serial.println(WiFi.SSID(i));
    }
  }
}

int Smoothing(int _val){
  readIndex = 0;
  total = 0;
  while(readIndex <= numReadings){
    readings[readIndex] = analogRead(_val);
    total = total + readings[readIndex];

    readIndex = readIndex + 1;
    delay(10);
  }
  average = total / numReadings;
  return average;
}
