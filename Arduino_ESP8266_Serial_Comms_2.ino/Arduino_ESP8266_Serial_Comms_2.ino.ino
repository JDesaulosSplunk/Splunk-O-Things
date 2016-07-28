// Basic serial communication with ESP8266
// Uses serial monitor for communication with ESP8266
//
//  Pins
//  Arduino pin 2 (RX) to ESP8266 TX
//  Arduino pin 3 to voltage divider then to ESP8266 RX
//  Connect GND from the Arduiono to GND on the ESP8266
//  Pull ESP8266 CH_PD HIGH
//
// When a command is entered in to the serial monitor on the computer 
// the Arduino will relay it to the ESP8266
//
//http://www.martyncurrey.com/arduino-to-esp8266-serial-commincation/
#include <SoftwareSerial.h>
#include "DHT.h"
#define DHTPIN 9 
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE); 
#include <SoftwareSerial.h>
SoftwareSerial ESPserial(2, 3); // RX | TX
float f;
char buffer[6];

 
void setup() 
{
    Serial.begin(9600);     // communication with the host computer
    //while (!Serial)   { ; }
 
    // Start the software serial for communication with the ESP8266
    ESPserial.begin(9600);  
 
    Serial.println("");
    Serial.println("Remember to to set Both NL & CR in the serial monitor.");
    Serial.println("Ready");
    Serial.println("");    
}
 
void loop() 
{
    f = dht.readTemperature(true);
    String tf = dtostrf(f, 4, 1, buffer);
  
    // listen for communication from the ESP8266 and then write it to the serial monitor
    if ( ESPserial.available() )   {  Serial.write( ESPserial.read() );  }
 
    // listen for user input and send it to the ESP8266
    if ( Serial.available() )
      {
        //ESPserial.print(tf); 
        ESPserial.write(tf);  
        //delay(15000);
      }
}
