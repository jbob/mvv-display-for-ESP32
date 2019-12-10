/*
	Esp32 Websockets Client
	By Gil Maimon
	https://github.com/gilmaimon/ArduinoWebsockets

*/

#include <ArduinoWebsockets.h>
#include <WiFi.h>

const char* ssid = "XXX"; //Enter SSID
const char* password = "XXX"; //Enter Password


using namespace websockets;

WebsocketsClient client;
void setup() {
    Serial.begin(115200);
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Check if connected to wifi
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!");
        return;
    }

    Serial.println("Connected to Wifi, Connecting to server.");
    // try to connect to Websockets server
    //bool connected = client.connect(websockets_server_host, websockets_server_port, websockets_server_uri);
    bool connected = client.connect("wss://tralis.sbahnm.geops.de:443/ws");
    if(connected) {
        Serial.println("Connecetd!");
        client.send("SUB timetable_8000261"); //Subscribe for Departures at Hauptbahnhof
    } else {
        Serial.println("Not Connected!");
    }
    
    // run callback when messages are received
    client.onMessage([&](WebsocketsMessage message){
        Serial.print("Got Message: ");
        Serial.println(message.data());
    });
}

void loop() {
    // let the websockets client check for incoming messages
    if(client.available()) {
        client.poll();
    }
    delay(500);
}
