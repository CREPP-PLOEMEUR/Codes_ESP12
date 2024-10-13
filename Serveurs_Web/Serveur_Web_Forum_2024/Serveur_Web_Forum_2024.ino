/*
 * This program is free software; you can redistribute it and/or
 * modify without any restriction
 * 
 * This file create a web server based on ESP12 NodeMCU.
 * You can load a page and turn-on and turn off the internal LED
 * Major informations about connection are displayed on OLED screen and serveur Web
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

#include <Adafruit_Sensor.h>
#include "Adafruit_BME280.h"


#define SEALEVELPRESSURE_HPA (1023.25)  //Pression au niveau de la mer  

Adafruit_BME280 bme; //Créer un objet bme

/*
 *   Macros
 */

#define REFRESH_PAGE_DELAY 600 //Timeout before refresh


#define PORT 80 //Default port
#define LED D4  //LED Pin

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 //reset pin
#define SCREEN_ADDRESS 0x3C ///Sometimes 0x3D ou 0x3F

#define GREEN_LED D0
#define YELLOW_LED D3
#define RED_LED D5
/*
 *   Global variables
 */
const char* ssid     = "Pixel_Nico";//"creafab_invite";          //SSID
const char* password = "biosdu56";//"MonTraficEstJournalise";  //Password

float temperature = 0.0;
float humidity = 0.0;
float pressure = 0.0;
float altitude = 0.0;



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
  
  pinMode(GREEN_LED, OUTPUT);       //LED ouput
  pinMode(YELLOW_LED, OUTPUT);       //LED ouput
  pinMode(RED_LED, OUTPUT);       //LED ouput


  for (uint8_t i = 0; i < 5; i++)
  {
    digitalWrite(GREEN_LED, HIGH);     //Turn-on LED
    digitalWrite(YELLOW_LED, HIGH);     //Turn-on LED
    digitalWrite(RED_LED, HIGH);     //Turn-on LED
    delay(100);
    digitalWrite(GREEN_LED, LOW);     //Turn-on LED
    digitalWrite(YELLOW_LED, LOW);     //Turn-on LED
    digitalWrite(RED_LED, LOW);     //Turn-on LED
    delay(100);
  }

  Serial.begin(115200);       //Baudrate 

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Echec OLED"));

  }
  //display.display(); //Display Adafruit symbol
  digitalWrite(YELLOW_LED, HIGH);     //Turn-on LED
  delay(500); 
  display.clearDisplay(); //Clear screen
  display.setRotation(2);
  display.setTextSize(1);               //Size factor
  display.setTextColor(SSD1306_WHITE);  //White text
  display.setCursor(0, 0);              //Set cursor to (0,0)
  
  display.print("WAITING NETWORK...");
  display.display();


  WiFi.begin(ssid, password); //Connexion
  Serial.println("");         //New line

  
  while (WiFi.status() != WL_CONNECTED) 
  {
    
    delay(1000);
    Serial.println(">>> Network is not available...Retry...");
  }
  digitalWrite(YELLOW_LED, LOW);     //Turn-on LED
  
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

   bool status = bme.begin(0x76);  

  if (!status) 
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    digitalWrite(RED_LED, HIGH);     //Turn-on LED
    //while (1);
  }

  server.on("/", mainPage);           //Display main page
  server.onNotFound(notFoundPage);    //Display error page

  server.begin();                     //Starting server
  Serial.println(">>> Starting server");



  delay(500); 
  display.clearDisplay(); //Clear screen

  display.setTextSize(1);               //Size factor
  display.setTextColor(SSD1306_WHITE);  //White text
  display.setCursor(0, 0);              //Set cursor to (0,0)
  
  display.println(ssid);
  display.println(WiFi.localIP());
  display.println(WiFi.macAddress());
  display.display();

  delay(5000);
  
}

void loop() 
{
  server.handleClient(); //Clients handler
}


void mainPage() 
{ 

 if(server.arg("LED")=="ON") //Reading 'LED' argument value
 {
      digitalWrite(GREEN_LED, HIGH); //Turn-on LED
      
 }
 else if(server.arg("LED")=="OFF")
 {
    digitalWrite(GREEN_LED, LOW);   //Turn-off LED
 }
 else if(server.arg("UPDATE")=="1")
 {

    digitalWrite(YELLOW_LED, HIGH);   //Turn-off LED
    delay(100);
    digitalWrite(YELLOW_LED, LOW);   //Turn-off LED


 }
 else 
 {
  //nothing
 }

  temperature = bme.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  pressure = bme.readPressure() / 100.0F;
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print("Approx. Altitude = ");
  Serial.print(altitude);
  Serial.println(" m");

  humidity = bme.readHumidity();
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");

  display.clearDisplay();
  display.setCursor(0, 0); 
  display.println("Temperature:"+String(temperature)+" °C");
  display.println("Humudity:"+String(humidity)+ " %");
  display.println("Pressure:"+String(pressure)+" hPa");
  display.println("Altitude:"+String(altitude)+" m");
  display.display();


  const String fullPageContent = "<html>\
  <head>\
    <title>CREPP Web Server</title>\
    <meta charset=\"utf-8\"/> \
    <meta http-equiv=\"refresh\" content=\""+String(REFRESH_PAGE_DELAY)+"\">\
    <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">\
  </head>\
  <body style=\"margin-left:5%;\">\
    <h1>Weather Station</h1><br>\
    <h3>Led control on <span class=\"badge badge-secondary\">D0</span> pin</h3><br>\
      <a href=\"/?LED=ON\"><button class=\"btn btn-success\">Turn-on</button></a>\
      <a href=\"/?LED=OFF\"><button class=\"btn btn-danger\">Turn-off</button></a><br><br>\
    <h3>Temperature, humidity and pressure measurements with BME280 sensor on I2C Bus</h3><br>\
      <a href=\"/?UPDATE=1\"><button class=\"btn btn-primary\">Update</button></a><br><br>\
      <b>Temperature</b> : "+String(temperature)+" C<br>\
      <b>Humidity</b> : "+String(humidity)+"%<br>\
      <b>Pressure</b> : "+String(pressure)+"  hPa<br>\
      <b>Altitude</b> : "+String(altitude)+" m<br>\
  </body>\
</html>";

const String fullPageContent2 = "<html>\
  <head>\
    <title>CREPP Web Server</title>\
    <meta charset=\"utf-8\"/>\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">\
    <meta http-equiv=\"refresh\" content=\"" + String(REFRESH_PAGE_DELAY) + "\">\
    <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">\
    <style>\
      body {\
        margin: 0;\
        padding: 0;\
        font-family: Arial, sans-serif;\
        background-color: #f8f9fa;\
        color: #333;\
        display: flex;\
        justify-content: center;\
        align-items: center;\
        height: 100vh;\
        text-align: center;\
      }\
      .container {\
        max-width: 400px;\
        background: white;\
        padding: 20px;\
        border-radius: 8px;\
        box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);\
      }\
      .btn {\
        width: 100%;\
        margin-bottom: 10px;\
      }\
      h1 {\
        font-size: 24px;\
        margin-bottom: 20px;\
      }\
      h3 {\
        font-size: 18px;\
        margin-bottom: 15px;\
      }\
      b {\
        font-weight: 600;\
      }\
    </style>\
  </head>\
  <body>\
    <div class=\"container\">\
      <h1>Weather Station</h1><br>\
      <h3>Led control on <span class=\"badge badge-secondary\">D0</span> pin</h3><br>\
      <a href=\"/?LED=ON\"><button class=\"btn btn-success\">Turn on</button></a>\
      <a href=\"/?LED=OFF\"><button class=\"btn btn-danger\">Turn off</button></a><br><br>\
      <h3>Temperature, Humidity, and Pressure Measurements with BME280 Sensor on I2C Bus</h3><br>\
      <a href=\"/?UPDATE=1\"><button class=\"btn btn-primary\">Update</button></a><br><br>\
      <p><b>Temperature</b>: " + String(temperature) + " °C</p>\
      <p><b>Humidity</b>: " + String(humidity) + "%</p>\
      <p><b>Pressure</b>: " + String(pressure) + " hPa</p>\
      <p><b>Altitude</b>: " + String(altitude) + " m</p>\
    </div>\
  </body>\
</html>";

 server.send(200, "text/html", fullPageContent2); //Sending main page
  
}


void notFoundPage()  //Bad URL handler
{
  server.send(404, "text/plain", "Page not found !\n\n");
}

