/* https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiClientBasic/WiFiClientBasic.ino
 *  This sketch sends a message to a TCP server
 *
 */
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "DHT.h"
#define DHTPIN 2 
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE); 
float f;
char buffer[6];

RTC_PCF8523 rtc;
ESP8266WiFiMulti WiFiMulti;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider

void setup() {


    #ifndef ESP8266
      while (!Serial); // for Leonardo/Micro/Zero
    #endif
    
    Serial.begin(115200);
    delay(10);
      
    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");  
    while (1);
    }

    if (! rtc.initialized()) {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // This line sets the RTC with an explicit date & time, for example to set
      // January 21, 2014 at 3am you would call:
      // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }

    // We start by connecting to a WiFi network
    WiFiMulti.addAP("SSID", "PASS");

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
    DateTime now = rtc.now();

    photocellReading = analogRead(photocellPin);

    f = dht.readTemperature(true);
    String tf = dtostrf(f, 4, 1, buffer);

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
    

    // This will send the request to the server
    //Serial.print("1: " + tf);
    delay(1000);
    long time2 = 147032524;
    String time3  = (String)now.unixtime(); 
    //String temporary = " " + tf;
    client.println(tf + " " + photocellReading + " " + time3.substring(0,4) + time3.substring(5));

    delay(3000);
    
    Serial.println(tf + " " + photocellReading + " " + time3);
    //client.print("time " + time);
    Serial.println(sizeof(tf));
    //read back one line from server
    //String line = client.readStringUntil('\r');
    //client.println("temp : " + tf);

    Serial.println("closing connection");
    client.stop();
    
    Serial.println("wait 5 sec...");
    delay(1000);
}

