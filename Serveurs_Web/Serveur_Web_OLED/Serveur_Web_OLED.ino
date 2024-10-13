/*
 * This program is free software; you can redistribute it and/or
 * modify without any restriction
 * 
 * This file create a web server based on ESP12 NodeMCU.
 * You can load a page and turn-on and turn off the internal LED
 * Major informations about connection are displayed on OLED screen
 */

/*
 *   Libraries
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*
 *   Macros
 */
#define PORT 80 //Default port
#define LED D4  //LED Pin

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 //reset pin
#define SCREEN_ADDRESS 0x3C ///Sometimes 0x3D ou 0x3F


/*
 *   Global variables
 */
const char* ssid     = "creafab_invite";          //SSID
const char* password = "MonTraficEstJournalise";  //Password

const String minimalPageContent = "<html>\
  <head>\
    <title>CREPP Web Server</title>\
    <meta charset=\"utf-8\"/> \
    </head>\
  <body>\
    <h1>Led ESP8266</h1><br>\
    <h3>LED on D4 Pin</h3><br>\
      <a href=\"/?LED=ON\"><button >Turn-on</button></a>\
      <a href=\"/?LED=OFF\"><button >Turn-off</button></a>\
  </body>\
</html>";

/*
 *   Objects
 */
ESP8266WebServer server(PORT);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void setup() {
  
  pinMode(LED, OUTPUT);       //LED ouput
  digitalWrite(LED, LOW);     //Turn-on LED
  Serial.begin(115200);       //Baudrate 
  WiFi.begin(ssid, password); //Connexion
  Serial.println("");         //New line

  display.display(); //Display Adafruit symbol
  delay(500); 
  display.clearDisplay(); //Clear screen

  display.setTextSize(1);               //Size factor
  //display.setTextColor(SSD1306_WHITE);  //White text
  display.setCursor(0, 0);              //Set cursor to (0,0)
  
  display.print("WAITING NETWORK...");

  
  /*while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println(">>> Network is not available...Retry...");
  }*/
  
  Serial.print(">>> Connected to ");
  Serial.println(ssid);
  Serial.print("with this IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("and this MAC address : ");
  Serial.println(WiFi.macAddress());

  if (MDNS.begin("esp8266")) //Multicast DNS 
  {   
    Serial.println(">>> Serveur MDNS : ON");
  }

  server.on("/", mainPage);           //Display main page
  server.onNotFound(notFoundPage);    //Display error page

  server.begin();                     //Starting server
  Serial.println(">>> Starting server");


  /*if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Echec OLED"));
  }*/

  display.display(); //Display Adafruit symbol
  delay(500); 
  display.clearDisplay(); //Clear screen

  display.setTextSize(1);               //Size factor
  //display.setTextColor(SSD1306_WHITE);  //White text
  display.setCursor(0, 0);              //Set cursor to (0,0)
  
  display.print(">>> Connected to ");
  display.println(ssid);
  display.print("with this IP address: ");
  display.println(WiFi.localIP());
  display.print("and this MAC address : ");
  display.println(WiFi.macAddress());
  
}

void loop() 
{
  server.handleClient(); //Clients handler
}


void mainPage() 
{ 

 if(server.arg("LED")=="ON") //Reading 'LED' argument value
 {
      digitalWrite(LED, LOW); //Turn-on LED
      display.clearDisplay();
      display.setCursor(0, 0);  
      display.println("LED : 1");
      display.display();
      
 }
 else if(server.arg("LED")=="OFF")
 {
    digitalWrite(LED, HIGH);   //Turn-off LED
    display.clearDisplay();
    display.setCursor(0, 0);  
    display.println("LED : 0");
    display.display();
 }
 else 
 {
  //nothing
 }

 server.send(200, "text/html", minimalPageContent); //Sending main page
  
}


void notFoundPage()  //Bad URL handler
{
  server.send(404, "text/plain", "Page not found !\n\n");
}
