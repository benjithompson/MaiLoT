#include "wifi.h"

const char* ssid = "NETGEAR21";
const char* pw = "exoticchair828";

void init_wifi()
{
    WiFi.begin(ssid, pw);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connected");
}