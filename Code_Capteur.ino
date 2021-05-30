#include <WiFi.h>
#define Gaz 4

const char* ssid = "";
const char* password = "";

void setup(){
  Serial.begin(9600);
  pinMode(Gaz, INPUT);
  WiFi.mode(WIFI_STA);
}

void loop(){
  int gazsensor = analogRead(Gaz);
  int threashold;
  Serial.println(gazsensor);

  //if(gazsensor > 1000){
    //Serial.println("Alert");
  //}
  delay(100);
}

void initWifi(){
  WiFi.begin(ssid, password);
  Serial.print("Connection au Wifi ..");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connécter au Wifi avec l'addresse : " + WiFi.localIP())
}