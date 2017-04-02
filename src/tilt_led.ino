//Tilt sensor activates state change and updates
//server over WiFi

//Wifi/Server
#include "ESP8266WiFi.h"
#include <aREST.h>

//OTA
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//Convert
#include "gpioconvert.h"

//Settings   
#define tilt_wait   500
#define LISTEN_PORT  80

const uint8_t tiltPin = d1;
const uint8_t ledPin  = d7;  
const uint8_t ledPin2 = d6;  

const char* ssid = "NETGEAR21";
const char* pw = "exoticchair828";

aREST rest = aREST();
WiFiServer server(LISTEN_PORT);

//Status Variables   
short prev_state;
long last_msg = 0;

//REST Variables
int tilt_state;
int event_count = 0;

void setup() {
  
  Serial.begin(9600);
  
  //wifi
  WiFi.begin(ssid, pw);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");

  //ArduinoOTA 
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA Ready");

  //Server 
  rest.variable("tilt_state", &tilt_state);
  rest.variable("event_count", &event_count);
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
  
  //Tilt
  pinMode(tiltPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
}

void loop() {

  //Wait for tilt status check
  long now = millis();
  if((now - last_msg) > tilt_wait){
    
    tilt_state = !digitalRead(tiltPin);
    last_msg = millis();

    //check for new tilt status
    if (tilt_state != prev_state) {

      digitalWrite(ledPin, tilt_state);
      
      if(tilt_state){
        Serial.println("Door Open");
        event_count++;
      }else{
        Serial.println("Door Closed");
      }

      if((event_count % 2) == 0){
        //digitalWrite(ledPin2
      }
      prev_state = tilt_state;
    }
    
  }
  
  //server
  WiFiClient client = server.available();
  if(!client){
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);

  ArduinoOTA.handle();
}
