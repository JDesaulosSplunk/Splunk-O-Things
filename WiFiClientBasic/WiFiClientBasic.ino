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

#include <SPI.h>
#include <SD.h>

#define DHTPIN 2 
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE); 
float f;
float h;
int pR;
char buffer[6];
// Use WiFiClient class to create TCP connections
WiFiClient client;

RTC_PCF8523 rtc;
ESP8266WiFiMulti WiFiMulti;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
TimeSpan offset = 19200;

int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider
const int chipSelect = 15;

const uint16_t port = 2319;
const char * host = "192.168.10.82"; // ip or dns

void setup() {


    #ifndef ESP8266
      while (!Serial); // for Leonardo/Micro/Zero
    #endif
    
    Serial.begin(115200);
    delay(10);

    
    Serial.print("Initializing SD card...");
  
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
      Serial.println("Card failed, or not present");
      // don't do anything more:
      return;
    }
    Serial.println("card initialized.");
      
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
    WiFiMulti.addAP("Splunk-Guest-PL","legacyplace");

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

//    const uint16_t port = 2319;
//    const char * host = "192.168.10.82"; // ip or dns

    Serial.print("connecting to ");
    Serial.println(host);

    

    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
    }
    
}



void loop() {

    
    f = dht.readTemperature(true);
    h = dht.readHumidity();
    
    String tf = dtostrf(f, 4, 1, buffer);
    float hif = dht.computeHeatIndex(f, h);
    pR = analogRead(photocellPin);

//    delay(10000);
    DateTime now = rtc.now() + offset;
//  
//    String timeYear  = (String)(now.year()); 
//    String timeMonth  = (String)(now.month());
//    String timeDay  = (String)(now.day());  
//    String timeHour  = (String)(now.hour()); 
//    String timeMinute  = (String)(now.minute()); 
//    String timeSecond  = (String)(now.second()); 
//
//    String timeOut/*put*/ = timeYear + " " + timeMonth + " " + timeDay;
//    String timeOut2 = timeHour + ":" + timeMinute + ":" + timeSecond;


    String dataString = "{\"Time\":\"" + (String)now.unixtime() + "\",\"Temperature\":\"" + tf + "\",\"Humidity\":\"" + h + "\",\"Light\":\"" + pR + "\",\"Heat_Index\":\"" + hif + "\"}";

//    client.stop(); // remove this line in production!!!!!!!!
    
    if (client.connected()){
      client.println(dataString);
      Serial.print("Data sent over TCP: ");
      Serial.println(dataString);
    }
    else{
      Serial.println("Data NOT SENT OVER TCP! \n\t... but it is written to file. hopefully.");
      Serial.println("Reconnecting to tcp port 2319");
      
      if (!client.connect(host, port)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
      }
      else{
        Serial.println("REconnected!! WHoop!");
      }
      client.println(dataString);
      Serial.print("Data sent over TCP: ");
      Serial.println(dataString);
    }
  
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
//      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
//    Serial.println(timeOut);
//    Serial.println(timeOut2);
//    Serial.println(sizeof(tf));

//    Serial.println("closing connection");
//    client.stop();
    
    Serial.println("wait 1 sec...");
    delay(3000);
}

