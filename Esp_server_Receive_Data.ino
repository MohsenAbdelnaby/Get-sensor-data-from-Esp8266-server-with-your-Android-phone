 // ESP8266 as Webserver turning LED on/off and reading TMP36 temperature sensor
// This code is based on SparkFun's ESP8266 sample code
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "Adafruit_BMP085.h"
#include <SPI.h>
#include <Ethernet.h>
Adafruit_BMP085 bmp;

// WiFi Definitions 
const char WiFiSSID[] = "iPhone";
const char WiFiPSK[] = "MohsenBiomedicalEngineer";



// Pin Definitions 
const int LED_PIN =BUILTIN_LED;    // ESP8266 HUZZA onboard RED LED
const int DIGITAL_PIN = 13; // Digital pin to be read
int d;


WiFiServer server(80);

void setup() 
{
   
  Wire.pins(2, 14);
  Wire.begin(2, 14);
  if (!bmp.begin()) {
    
  }
 
  initHardware();
  connectWiFi();
  server.begin();
}

void loop() 
{
  String t = "T= " + String(bmp.readTemperature()) + " F";
  String p = "P= " + String(bmp.readPressure()) + " Pa";
  String a = "A= " + String(bmp.readAltitude(103000)) + " m";// insert pressure at sea level
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
  
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  int val = -1; // We'll use 'val' to keep track of both the
                // request type (read/set) and value if set.
  if (req.indexOf("/led/0") != -1)
    val = 1; // Will write LED high
  else if (req.indexOf("/led/1") != -1)
    val = 0; // Will write LED low
  else if (req.indexOf("/read/Temperature") != -1)
    val = -2; // Will print pin reads
     else if (req.indexOf("/read/Pressure") != -1)
    val = -4; // Will print pin reads
    else if (req.indexOf("/read/Altitude") != -1)
    val = -5; // Will print pin reads
     else if (req.indexOf("/pb/check") != -1)
     val = -3;
  // Otherwise request will be invalid. We'll say as much in HTML

  // Set GPIO5 according to the request
  if (val >= 0)
    digitalWrite(LED_PIN, val);

  client.flush();
//html
  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  //s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  // If we're setting the LED, print out a message saying we did
  if (val >= 0)
  {
    s += "<br>";
    s += "<br>"; 
    s += "LED   is   now ";
    s += (val)?"off":"on";
  }
  else if (val == -2)
  { // If we're reading pins, print out those values:
    //s += "<br>";
    //s += " Temperature C = ";
    s += String(t);
  }
  else if (val == -4)
  { // If we're reading pins, print out those values:
    //s += "<br>";
    //s += " Pressure Pa = ";
    s += String(p);
  }
  else if (val == -5)
  { // If we're reading pins, print out those values:
    //
    //s += "<br>";
    //s += " Altitude m = ";
    s += String(a);
  }
   else if (val == -3)
  {
    s += "<br>";         // Go to the next line.
     
    d=digitalRead(DIGITAL_PIN);
    if(d==1){
      s += "PushButton is release ";
 
    }
    else {s += "PushButton is pressed ------->> DAMA ";}
  }

  else
  {
    s += "Request Successfully. Try /led/1, /led/0,/pb/check, or /read.";
  }
 // s += "</html>\n";  //end oe html langauge

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed

 
  }

 

void connectWiFi()
{
  byte ledStatus = LOW;
  Serial.println();
  Serial.println("Connecting to: " + String(WiFiSSID));
  // Set WiFi mode to station (as opposed to AP or AP_STA)
  WiFi.mode(WIFI_STA);

  // WiFI.begin([ssid], [passkey]) initiates a WiFI connection
  // to the stated [ssid], using the [passkey] as a WPA, WPA2,
  // or WEP passphrase.
  WiFi.begin(WiFiSSID, WiFiPSK); //static ip

  // Use the WiFi.status() function to check if the ESP8266
  // is connected to a WiFi network.
  while (WiFi.status() != WL_CONNECTED)
  {
    // Blink the LED
    digitalWrite(LED_PIN, HIGH); // Write LED high/low
     delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
    
  }
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}





void initHardware()
{
  Serial.begin(115200);
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  // Don't need to set ANALOG_PIN as input, 
  // that's all it can be.
}
