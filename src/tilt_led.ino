//Tilt sensor activates state change and updates
//server over WiFi

//Wifi/Server
#include "wifi.h"
#include "PubSubclient.h"

//OTA
// #include "ota.h"

//Convert
#include "gpioconvert.h"

//Settings   
#define tilt_wait   500
#define LISTEN_PORT  80

const uint8_t tiltPin = d8;
const uint8_t ledPin  = d7;   

//Status Variables   
uint8_t prev_state;
long last_msg = 0;

//REST Variables
int tilt_state;

const char* mqtt_server = "192.168.1.19";
const char* mqtt_topic = "mailbox";
const char* payload;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  
  Serial.begin(9600);
  
  //wifi
  init_wifi();

  //MQTT Server
  client.setServer(mqtt_server, 1883);

  //Tilt
  pinMode(tiltPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
}

void mqttConnect(){
  while(!client.connected()){
    Serial.println("Connecting MQTT");
    Serial.print(".");

    if(client.connect("ESP8266_Mailbox")){
      Serial.println("MTQQ connected.");
    }else{
      Serial.print("Failed, clientstate=");
      Serial.println(client.state());
      delay(1000);
    }
  }
}

void loop() {

  //Wait for tilt status check
  long now = millis();
  if((now - last_msg) > tilt_wait){
    tilt_state = !digitalRead(tiltPin);
    last_msg = millis();

    //check for new tilt status
    if (tilt_state != prev_state) {

      //pubsub here
      if(!client.connected()){
          mqttConnect();
      }
    
      //led
      digitalWrite(ledPin, tilt_state);
      
      //Serial
      if(tilt_state){
        client.publish(mqtt_topic, "Door Open");
        Serial.println("Door Open");
        client.disconnect();
      }else{
        client.publish(mqtt_topic, "Door Closed");
        Serial.println("Door Closed");
        client.disconnect();
        // ESP.deepSleep(3*1000000);
      }

      prev_state = tilt_state;

      
    }
    // ESP.deepSleep(3*1000000);
  }
  // ArduinoOTA.handle();
}
