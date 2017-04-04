#include "wifi.h"

const char* ssid = "NETGEAR21";
const char* pw = "exoticchair828";



void init_wifi()
{
    int ledstate = 0;
    //static IP setup
    IPAddress ip(192, 168, 1, 201);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(ip, gateway, subnet);

    WiFi.begin(ssid, pw);
    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(13, !ledstate);
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi Connected");
    digitalWrite(13, 0);
}