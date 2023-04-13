/*
 * This program is free software; you can redistribute it and/or
 * modify without any restriction
 * 
 * This file create a web server based on ESP12 NodeMCU.
 * You can load a page and turn-on and turn off the internal LED
 */

/*
 *   Libraries
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

/*
 *   Macros
 */
#define PORT 80 //Default port
#define LED D4  //LED Pin

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

const String fullPageContent = "<html>\
  <head>\
    <title>CREPP Web Server</title>\
    <meta charset=\"utf-8\"/> \
    <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">\
  </head>\
  <body style=\"margin-left:5%;\">\
    <h1>Led on ESP8266</h1><br>\
    <h3>Led control <span class=\"badge badge-secondary\">D4</span> pin</h3><br>\
      <a href=\"/?LED=ON\"><button class=\"btn btn-success\">Turn-on</button></a>\
      <a href=\"/?LED=OFF\"><button class=\"btn btn-danger\">Turn-off</button></a>\
      <a href=\"/?BTN=OK\"><button class=\"btn btn-warning\">Message on Serial port</button></a>\
  </body>\
</html>";

/*
 *   Objects
 */
ESP8266WebServer server(PORT);


void setup() {
  
  pinMode(LED, OUTPUT);       //LED ouput
  digitalWrite(LED, LOW);     //Turn-on LED
  Serial.begin(115200);       //Baudrate 
  WiFi.begin(ssid, password); //Connexion
  Serial.println("");         //New line

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println(">>> Network is not available...Retry...");
  }
  
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
  
}

void loop() 
{
  server.handleClient(); //Clients handler
}


void mainPage() 
{ 

 if(server.arg("LED")=="ON")        //Reading 'LED' argument value
 {
      digitalWrite(LED, LOW);       //Turn-on LED
 }
 else if(server.arg("LED")=="OFF")
 {
     digitalWrite(LED, HIGH);       //Turn-off LED
 }
  else if(server.arg("BTN")=="OK")
 {
     Serial.println(">>> Coucou");
 }
 else {
  //nothing
 }

 server.send(200, "text/html", fullPageContent); //Sending main page
  
}

void notFoundPage()  //Bad URL handler
{
  server.send(404, "text/plain", "Page not found !\n\n");
}
