#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>
 
ESP8266WebServer Webserver(80);

// Replace with your network credentials
const char* ssid = "logitec-000000";
const char* password = "misakamikoto";
 
String HTMLpage = "";
IRsend irsend(4);  // An IR LED is controlled by GPIO pin 4 (D2)

uint16_t  acon[243] = {3300,1650,450 ,400 ,450 ,400 ,450 ,1250,450 ,400 ,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,1250,450 ,1250,450 ,1250,450 ,1250,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,1250,450 ,400 ,450 ,400 ,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,400 ,450 ,400 ,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,1250,450 ,1250,450 ,400 ,450 ,1250,450 ,400 ,450 ,1250,450};

uint16_t  acoff[99] = {3350,1650,450 ,400 ,450 ,400 ,450 ,1250,450 ,400 ,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,1250,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450};

void airconON() {
  Serial.println("power on");
  irsend.sendRaw(acon, 243, 38);
}

void airconOFF() {
  Serial.println("power off");
  irsend.sendRaw(acoff, 99, 38);
}

void setup(void){
  HTMLpage += "<head><title>esp8266</title></head><meta name=\"viewport\" content=\"width=300\"><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><h3>air conditioner controler</h3><p>air-con <a href=\"acON\"><button>16℃自動</button></a>&nbsp;<a href=\"acOFF\"><button>OFF</button></a></p>";
 
  Serial.begin(115200);
  irsend.begin();
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  if (MDNS.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
 
  Webserver.on("/", [](){
    Webserver.send(200, "text/html", HTMLpage);
  });
  Webserver.on("/acON", [](){
    Webserver.send(200, "text/html", HTMLpage+"<p>air-con is ON</p>");
    airconON();
    delay(1000);
  });
  Webserver.on("/acOFF", [](){
    Webserver.send(200, "text/html", HTMLpage+"<p>air-con is OFF</p>");
    airconOFF();
    delay(1000); 
  });
 
  Webserver.begin();
  Serial.println("HTTP Webserver started");
}
 
void loop(void){
  Webserver.handleClient();
}
