#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "SSD1306.h"
SSD1306  display(0x3C, 4, 5);
MDNSResponder mdns;
ESP8266WebServer server(80);
int p=0;
void setup() {
  pinMode(2,INPUT);
  pinMode(12,INPUT);
  pinMode(14,INPUT);
  pinMode(13,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2),Count,FALLING);
   Serial.begin(115200);
  display.init();//Serial connection
 // display.flipScreenVertically();
 
  display.setColor(WHITE);
  display.clear();
 // pinMode(2,INPUT);
//  digitalWrite(0,HIGH);
  WiFi.begin("Pompie", "YashSoni13495");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println(".");
   // String payload="";
 //StaticJsonBuffer<200> jsonBuffer;
  }
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
   Serial.println("WiFi Connected....");
   for (int16_t i=0; i<DISPLAY_HEIGHT; i+=4) {
    display.drawLine(DISPLAY_WIDTH-1, 0, 0, i);
    display.display();
    delay(10);
  }
  for (int16_t i=0; i<DISPLAY_WIDTH; i+=4) {
    display.drawLine(DISPLAY_WIDTH-1, 0, i, DISPLAY_HEIGHT-1);
    display.display();
    delay(10);
  }
  delay(1000);
   display.clear();
   display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);
    display.drawString(15,15,"Welcome");
    display.display();
}
else{
  Serial.println("Wifi Not Connected!");
}
 if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
 
    server.begin();
}

void loop() {
   
  server.handleClient();
  getMsg();
  delay(2000);
  
}
void Count(){
  p++;
 /* digitalWrite(13,HIGH);
  delay(800);
  digitalWrite(13,LOW);*/
}
void getMsg(void){
  display.clear();
   HTTPClient http;    //Declare object of class HTTPClient
   DynamicJsonBuffer jsonBuffer;
   http.begin("http://api.thingspeak.com/channels/289052/feeds.json?api_key=DSQ3GHJIZ93QC5W3&results=5");
   http.addHeader("Content-Type", "application/json");  //Specify content-type header
   delay(2000);
   int httpCode = http.GET(); //Send the request
   String payload = http.getString();   
   Serial.println(httpCode);   //Print HTTP return code 200 correct data 401 invalid data  404 Error
   Serial.println(payload);    //Print request response payload
   JsonObject& root = jsonBuffer.parseObject(payload);
   
  if(!root.success()){
    Serial.println("Nope");
  }
  
  else{
   
    JsonArray& trains = root["feeds"];
    JsonObject& Last = root["channel"];
    String Lasts = Last["last_entry_id"];
    JsonObject& train = trains[p];
    String pos = train["field1"];    
    String TIME = train["created_at"];
    String ID = train["entry_id"];
    Serial.println(pos);
    
        display.display();
    
    for(int i=0;i>=-50;i--){
      display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString    (0,30,TIME);

    display.setFont(ArialMT_Plain_10);
    display.drawString(0,0,"Msg No: ");
    display.drawString(40,0,ID); 
    display.drawString(80,0,"Total: ");
    display.drawString(110,0,Lasts);
display.setFont(ArialMT_Plain_16);
    display.drawString(i,47,pos);  
       display.display();
       delay(10);
       display.clear();
           delay(50);
      }
  

    Serial.println(pos);
  
}
}

