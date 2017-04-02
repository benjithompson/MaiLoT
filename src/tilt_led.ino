//Tilt sensor activates state change and updates
//server over WiFi

//Wifi/Server
#include "wifi.h"

//OTA
#include "ota.h"

//Convert
#include "gpioconvert.h"

//Settings   
#define tilt_wait   500
#define LISTEN_PORT  80

const uint8_t tiltPin = d1;
const uint8_t ledPin  = d7;  
const uint8_t ledPin2 = d6;  

aREST rest = aREST();
WiFiServer server(LISTEN_PORT);

//Status Variables   
uint8_t prev_state;
long last_msg = 0;

//REST Variables
int tilt_state;
int event_count = 0;

void setup() {
  
  Serial.begin(9600);
  
  //wifi
  init_wifi();

  //ArduinoOTA 
  init_android_ota();

  //Server 
  rest.set_id("01");
  rest.set_name("mailbox");
  rest.variable("tilt_state", &tilt_state);
  rest.variable("event_count", &event_count);

  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
  
  //Tilt
  pinMode(tiltPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  WiFi.forceSleepBegin();
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
