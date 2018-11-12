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
boolean connectUDP();

//for DS
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


void setup() {
// Initialise Serial connection
Serial.begin(115200);

sensors.begin();
delay(0);
// Initialise wifi connection
wifiConnected = connectWifi();
// only proceed if wifi connection successful
/*if(wifiConnected){
    udpConnected = connectUDP();
}
Serial.print(0);
if (udpConnected){
// initialise pins
//pinMode(5,OUTPUT);
}*/

}
long lastTime=25555555;
void loop() {
    // check if the WiFi and UDP connections were successful
    if(wifiConnected){
        //if(udpConnected){

        // if there’s data available, read a packet
        /*
        int packetSize = UDP.parsePacket();
        if(packetSize)
        {
        Serial.println("");
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remote = UDP.remoteIP();
        for (int i =0; i < 4; i++)
        {
        Serial.print(remote[i], DEC);
        if (i < 3)
        {
        Serial.print(".");
        }
        }
        Serial.print(", port ");
        Serial.println(UDP.remotePort());

        // read the packet into packetBufffer
        UDP.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
        Serial.println("Contents:");
        int value = packetBuffer[0]*256 + packetBuffer[1];
        Serial.println(value);
        */
        // send a reply, to the IP address and port that sent us the packet we received
        //UDP.beginPacket(UDP.remoteIP(), UDP.remotePort()); //original version
        //Serial.print("1");
        while(millis()-lastTime<20000/*||millis()<lastTime*/){delay(0);}
        sensors.requestTemperatures();
        float temp = sensors.getTempCByIndex(0);
        Serial.println(temp);
        delay(0);
        if(UDP.beginPacket(hostIP, localPort)){
            UDP.printf("%f", temp);
            UDP.endPacket();
        }
        // turn LED on or off depending on value recieved
        //digitalWrite(5,value);
        }
        lastTime=millis();
        delay(10);

    //}

}

//}

// connect to UDP – returns true if successful or false if not
boolean connectUDP()
{
    boolean state = false;

    Serial.println("");
    Serial.println("Connecting to UDP");

    if(UDP.begin(localPort) == 1){
        Serial.println("Connection successful");
        state = true;
    }
    else{
        Serial.println("Connection failed");
    }

    return state;
}
// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
boolean state = true;
int i = 0;
WiFi.begin(ssid, password);
Serial.println("");
Serial.println("Connecting to WiFi");

// Wait for connection
Serial.print("Connecting");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
if (i > 10){
state = false;
break;
}
i++;
}
if (state){
Serial.println("");
Serial.print("Connected to ");
Serial.println(ssid);
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
}
else {
Serial.println("");
Serial.println("Connection failed.");
}
return state;
}

