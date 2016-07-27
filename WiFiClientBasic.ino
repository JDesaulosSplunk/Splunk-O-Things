  /*
 *  This sketch sends a message to a TCP server
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;
String temp;
void setup() {
    Serial.begin(9600);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP("Splunk-Guest-PL", "legacyplace");

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);
}


void loop() {
    const uint16_t port = 2319;
    const char * host = "192.168.10.82"; // ip or dns

    
    
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
    }
    delay(10000);
    temp = Serial.read();
    delay(1000);
    // This will send the request to the server
    client.print(temp);
    Serial.println("3");
    Serial.println(temp);
    delay(2000);

    //read back one line from server
    String line = client.readStringUntil('\r');
    client.println(line);

    Serial.println("closing connection");
    client.stop();
    
    Serial.println("wait 5 sec...");
    delay(5000);
}

