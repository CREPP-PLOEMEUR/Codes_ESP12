/*
 * This program is free software; you can redistribute it and/or
 * modify without any restriction
 * 
 * This file create a web server based on ESP12 NodeMCU.
 * You can load a page and turn-on and turn off the internal LED
 * A dynamic graphic is generated on page
 */

/*
 *   Libraries
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "DHT.h"
#include "circularRing.h"


#define PORT 80 //Default port
#define LED D4  //LED Pin

#define REFRESH_PAGE_DELAY 5 //Timeout before refresh

#define DHTPIN D2         //Digital pin connected to the DHT sensor
#define DHTTYPE DHT11     //Use DHT22 or DHT21

/*
 *   Global variables
 */
const char* ssid     = "creafab_invite";          //SSID
const char* password = "MonTraficEstJournalise";  //Password

int current_index = 0;
float temperature = 20.0;
float humidity = 50.0;

/*
 *   Objects
 */
DHT dht(DHTPIN, DHTTYPE);

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
 
  if(server.arg("LED")=="ON") //Reading 'LED' argument value
  {
      digitalWrite(LED, LOW); //Turn-on the LED
  }
  else if(server.arg("LED")=="OFF")
  {
     digitalWrite(LED, HIGH);   //Turn-off the LED
  }
  else {
    //Nothing
  }

 server.send(200, "text/html", getString()); //Sending main page

 
}

void notFoundPage()  //Bad URL handler
{
  server.send(404, "text/plain", "Page not found !\n\n");
}



String getString()   //Generate main page
{

  temperature = dht.readTemperature();
  humidity  = dht.readHumidity();
  
  updateRings(&current_index, NB_DATA_TEMP, temperature, humidity); //Update of rings
 
  String dataTemperatures = concatenateArray(temperatures, current_index);
  String dataHumidities = concatenateArray(humidities, current_index);
  String dataReferences = concatenateArray(references, current_index); //Array with all values set up to 0

  String dataTime ="[";
  for (int i=current_index;i>0;i--) {
    dataTime += "'"+String(i)+"',";
  }
  dataTime += "]";
  
  
  const String fullPageContent = "<html>\
  <head>\
    <title>CREPP Web Server</title>\
    <meta charset=\"utf-8\"/> \
    <meta http-equiv=\"refresh\" content=\""+String(REFRESH_PAGE_DELAY)+"\">\
    <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">\
    <script src=\"https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js\"></script>  \
    </head>\
  <body style=\"margin-left:5%;\">\
    <h1>Interface ESP12</h1><br>\
    <h3>LED control on <span class=\"badge badge-secondary\">D4</span>Pin </h3> <br>\
      <a href=\"/?LED=ON\"><button class=\"btn btn-success\">Turn-on</button></a>\
      <a href=\"/?LED=OFF\"><button class=\"btn btn-danger\">Turn-off</button></a><br><br>\
    <h3>Temperature and humidity measurements with DHT11 sensor on <span class=\"badge badge-secondary\">D2</span> pin</h3><br>\
      <br>\
      >>> <b>Temperature</b> : "+String(temperature)+" °C<br>\
      >>> <b>Humidity</b> : "+String(humidity)+"%\
      <div style='max-width:40%;'><canvas id=\"myChart\" width=\"600\" height=\"450\"></canvas></div> \
        <script> \
var ctx = document.getElementById('myChart'); \
var temperatures = "+dataTemperatures+";\
var references = "+dataReferences+";\
var humidities = "+dataHumidities+";\
var time = "+dataTime+";\
var myChart = new Chart(ctx, {type: 'line', data: { labels: time, datasets: [{ label: 'Temperature', data: temperatures, borderColor: 'blue',backgroundColor: '', fill: 0}, { label: 'Référence', data: references, borderColor: 'black',backgroundColor: '', fill: 1}, { label: 'Humidité', data: humidities, borderColor: 'green',backgroundColor: '', fill: 0}]}})</script>\
  </body>\
</html>";

  return fullPageContent;

}
