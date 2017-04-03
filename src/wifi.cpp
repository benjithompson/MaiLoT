#include "wifi.h"


String ssid = "NETGEAR21";
String pw = "exoticchair828";
String host = "http://192.168.1.19";
String uri = "/";
String data;

SoftwareSerial esp(6,7);

const int httpPort = 3000;

HttpClient http;

esp.begin(9600);

void init_wifi()
{
    IPAddress ip(192, 168, 1, 201);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(ip, gateway, subnet);

    WiFi.begin(ssid, pw);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connected");
}



void POST(char state){
    Serial.print("Connecting to ");
    Serial.println(host);

    // if(!client.connect(host, httpPort)){
    //     Serial.println("Connection to server failed");
    //     for(int i = 0; i < 5; i++){
    //         digitalWrite(13, 0);
    //         delay(250);
    //         digitalWrite(13, 1);
    //         delay(250);
    //     }
    //     return;
    // }

    // http.begin(host);
    // http.addHeader("Content-Type", "application/urlencoded");
    // http.POST("{\"value\": 20 }");
    // http.writeToStream(&Serial);
    // http.end();

    // String content = "param1=value1&param2=value2";
    // Serial.println("POST Request sending...");
    // client.println("POST HTTP/1.1");
    // client.println("Host: http://192.168.1.19:3000");
    // client.println("Accept: */*");
    // client.println("Content-Length: " + content.length());
    // client.println("Content-Type: application/x-www-form-urlencoded");
    // client.println();
    // client.println(content);

    data = (String)state;
    esp.println("AT+CIPSTART=\"TCP\",\"" + host + "\",3000");

    if(esp.find("OK")){
        Serial.println("TCP connection ready");
    }
    delay(1000);
    String postRequest = "POST " + uri + " HTTP/1.0\r\n" +
            "Host: " + host + "\r\n" +
            "Accept: *" + "/" + "*\r\n" +
            "Content-Length: " + data.length() + "\r\n" +
            "Content-Type: application/x-www-form-urlencoded\r\n" +
            "\r\n" + data; 

    String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
    esp.print(sendCmd);
    esp.println(postRequest.length() );
    delay(500);

    if(esp.find(">")) { 
        Serial.println("Sending.."); esp.print(postRequest);

        if( esp.find("SEND OK")) { 
            Serial.println("Packet sent");

            while (esp.available()) {

                String tmpResp = esp.readString();
                Serial.println(tmpResp);
            }
            // close the connection
            esp.println("AT+CIPCLOSE");
        }
    }
}