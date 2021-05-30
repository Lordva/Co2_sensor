#include <WiFi.h>
#include <HTTPClient.h>

#define Gaz 4

const char* ssid = "";
const char* password = "";

String Host = "";

unsigned long lastTime = 0;
unsigned long sendInterval = 600000;

void setup(){
  Serial.begin(9600);
  pinMode(Gaz, INPUT);
  initWifi(ssid, password);
}

void loop(){
  int gazsensor = analogRead(Gaz);
  int threashold;
  Serial.println(gazsensor);

  if(gazsensor > 2000){
    Serial.println("Alert");
    SendData(Host, gazsensor);
  }
  
  delay(100);
}

void SendData(String _host, int _value){

  if((millis() - lastTime) > sendInterval){
    if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    String serverPath = _host + "?reading=" + _value;

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

void initWifi(const char* SSID, const char* pwd){
  int timer = 10000 + millis();

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, pwd);

  Serial.print("Connection au Wifi ..");

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    if(millis() > timer){
      Serial.println("[ERROR] Timeout...");
      break;
      return;
    }
    delay(1000);
  }
  Serial.println("Connecter au Wifi avec l'addresse : " + WiFi.localIP());
}