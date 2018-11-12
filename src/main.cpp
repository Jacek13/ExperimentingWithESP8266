#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// wifi connection variables
const char* ssid = "CERN";
const char* password = "";
boolean wifiConnected = false;

// UDP variables
unsigned int localPort = 9997;
//const char* hostIP = "bb-rpi";
IPAddress hostIP (194,12,185,103);
WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back

boolean connectWifi();

//for DS
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


void setup() 
{

  Serial.begin(115200);

  sensors.begin();
  delay(0);

  wifiConnected = connectWifi();

  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
}

long lastTime=25555555;

void loop() {
    // check if the WiFi and UDP connections were successful
    if(wifiConnected)
    {
        while (millis()-lastTime<20000/*||millis()<lastTime*/){delay(0);}

        sensors.requestTemperatures();
        float temp = sensors.getTempCByIndex(0);
        Serial.println(temp);
        delay(0);

        if (UDP.beginPacket(hostIP, localPort))
        {
            UDP.printf("%f", temp);
            UDP.endPacket();
            Serial.println("Done sending packet!");
        }
    }
    else
    {
      wifiConnected = connectWifi();
    }

    lastTime=millis();
    delay(10);
}


// connect to wifi – returns true if successful or false if not
boolean connectWifi()
{
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
    if (i > 10)
    {
      Serial.println("");
      Serial.println("Connection failed.");
      return false;
    }
    i++; 
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  return true;
}
