#include <WiFi.h>
#include <AsyncMqttClient.h>
#include "config.h"


int Gaz=34;

long lastTime = 0;
unsigned long sendInterval = 60000;
//unsigned long sendInterval = 100000;

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
  initWifi(SSID, PASSWORD);

  initMQTT();

  // init all readings to 0
  for(int thisReading = 0; thisReading < numReadings; thisReading++){
    readings[thisReading] = 0;
  }
}

void loop(){
  // read sensor value
  int gazsensor = Smoothing(Gaz);
  SendData(gazsensor);
  
  delay(10000);
}

// send data to MQTT broker
void SendData(int _value){
  if(WiFi.status() == WL_CONNECTED){
    mqttClient.publish("esp32/airquality", 1, true, String(_value).c_str());
    Serial.println("Sending MQTT packet");
  }
  else{
  Serial.println("Wifi disconnected !");
  }
}

// connect to wifi
void initWifi(const char* _SSID, const char* _pwd){
  int timer = 1000000 + millis();

  // Scan Wifi at range Used for debug
  //ScanWifi();

  Serial.print("Connecting to: ");
  Serial.println(_SSID);

  // Set wifi mode to station
  WiFi.mode(WIFI_STA);
  // Begin connection
  WiFi.begin(_SSID, _pwd);

  Serial.print("Connexion au Wifi ..");

  // wait for connection to happen
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
  // Print the local IP address
  Serial.print(" Connected to Wifi with: ");
  Serial.println(WiFi.localIP());
}

//Start Connection to MQTT broker
void initMQTT(){
  int timer = 1000000 + millis();

  Serial.print("Connecting to MQTT on ");
  Serial.print(MQTT_HOST);
  Serial.print(":");
  Serial.println(MQTT_PORT);

  if(IS_MQTT_PROTECTED){
    mqttClient.setCredentials(MQTT_USER, MQTT_PASSWORD);
    Serial.println("MQTT connection is protected");
  }
  
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.connect();

  while(mqttClient.connected()){
    Serial.print(".");
    if(millis() > timer){
      Serial.println("[ERROR] timed out");
      break;
    }
  }
}

//Scan for wifi at range (for debuging)
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

// smooth the analogs values
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
