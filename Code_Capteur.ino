#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"

int Gaz=34;

//TODO :
//  sensor calibration (dynamic value) check w/ api with history

static const char* ssid = SSID;
static const char* password = PASSWORD;

String host = Host;
String location = Location;

unsigned long lastTime = 0;
unsigned long sendInterval = 600000;

//smooting variables
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

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

  //SendData(host, gazsensor, location);
  delay(100);
}

// send data to API
void SendData(String _host, int _value, String _location){

  if((millis() - lastTime) > sendInterval){
    Serial.println("Sending data to API...");
    if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    String serverPath = _host + "?location=" + _location + "?reading=" + _value;

    http.begin(serverPath.c_str());

    int httpResponceCode = http.GET();

    if(httpResponceCode>0){
      Serial.print("HTTP Responce code: ");
      Serial.println(httpResponceCode);
      String payload = http.getString();
      Serial.println(httpResponceCode);
    }
    else {
      Serial.print("[Error] Http error code : ");
      Serial.println(httpResponceCode);
    }
    http.end();
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