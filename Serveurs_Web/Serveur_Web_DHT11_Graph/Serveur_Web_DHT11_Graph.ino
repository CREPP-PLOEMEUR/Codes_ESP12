/*

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * Programme pour le contrôle d'un serveur Web qui affiche la température et l'humidité sur une page Web avec un graphique
 * par Club de Robotique et d'Électronique Programmable de Ploemeur

 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define NB_DATA_TEMP 60 //Nombre de valeur sur le graphique

#include "DHT.h"
#include "circularRing.h"


//########################### Début Paramètres 
#define PORT 80 //Port par défaut
#define LED D4  //Broche de la LED

#define REFRESH_PAGE_DELAY 5 //Temps de reafraichissement de la page en s

#define DHTPIN D2     //Broche du DHT11/22

#define DHTTYPE DHT11   
//#define DHTTYPE DHT22  
//#define DHTTYPE DHT21 


const char* ssid     = "creafab_invite";//Nom du routeur sur le réseau (par Exemple FReebox-44F45)
const char* password = "MonTraficEstJournalise"; //Mot de passe du routeur


//###########################Fin paramètres

DHT dht(DHTPIN, DHTTYPE);


int current_index = 0;

ESP8266WebServer server(PORT);

float temperature = 20.0;
float humidity = 50.0;

void setup() {
  
  pinMode(LED, OUTPUT);       //LED en sortie
  digitalWrite(LED, LOW);     //LED éteinte
  Serial.begin(115200);       //Communication à 115200 bits/s
  WiFi.begin(ssid, password); //Connexion
  Serial.println("");         //Retour à la ligne
  dht.begin();
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println(">>> Impossible de se connecter au réseau...");
  }
  
  Serial.print(">>> Connexion au réseau ");
  Serial.println(ssid);
  Serial.print("avec l'adresse IP : ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {   //Multicast DNS 
    Serial.println(">>> Serveur MDNS activé");
  }

  server.on("/", mainPage);           //Affichage de la page principale si requête sur '/' -> saisir IP dans le navigateur
  server.onNotFound(notFoundPage);    //Affichage de la page d'erreur si adresse non valide

  server.begin();                     //Initialisation du serveur
  Serial.println(">>> démarrage du serveur");
  
}//Fin setup

void loop() 
{
  
  server.handleClient(); //Gestion des clients sur le serveur
  
}//Fin loop


void mainPage() //Page principale
{ 
 
  if(server.arg("LED")=="ON") //Lecture de l'argument 'LED'
  {
      digitalWrite(LED, LOW); //On allume la led
  }//Fin if
  else if(server.arg("LED")=="OFF")
  {
     digitalWrite(LED, HIGH);   //On eteint la led
  }//Fin else if

 server.send(200, "text/html", getString()); //On envoie la page principale

 
}//FIn mainPage


void notFoundPage()  //Gestion si mauvaise URL
{
  server.send(404, "text/plain", "Page introuvable !\n\n");
  
}


String getString() {//Récupère la page

  temperature = dht.readTemperature();
  humidity  = dht.readHumidity();
  
  updateRings(&current_index, NB_DATA_TEMP, temperature, humidity); //Mise à jour des tableaux
 
  String dataTemperatures = concatenateArray(temperatures, current_index);
  String dataHumidities = concatenateArray(humidities, current_index);
  String dataReferences = concatenateArray(references, current_index); //Tableau contenant toutes les valeurs à 0, pour afficher la référence sur le graphique
  
  String dataTime ="[";
  for (int i=current_index;i>0;i--) {
    dataTime += "'"+String(i)+"',";
  }
  dataTime += "]";
  
  
  const String fullPageContent = "<html>\
  <head>\
    <title>Serveur Web CREPP</title>\
    <meta charset=\"utf-8\"/> \
    <meta http-equiv=\"refresh\" content=\""+String(REFRESH_PAGE_DELAY)+"\">\
    <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">\
    <script src=\"https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js\"></script>  \
    </head>\
  <body style=\"margin-left:5%;\">\
    <h1>Interface ESP12</h1><br>\
    <h3>Contrôle de la LED sur la broche <span class=\"badge badge-secondary\">D4</span></h3><br>\
      <a href=\"/?LED=ON\"><button class=\"btn btn-success\">Allumer</button></a>\
      <a href=\"/?LED=OFF\"><button class=\"btn btn-danger\">Eteindre</button></a><br><br>\
    <h3>Mesure de la température et humidité avec le module DHT11 sur la broche <span class=\"badge badge-secondary\">D2</span></h3><br>\
      <br>\
      >>> <b>Température</b> : "+String(temperature)+" °C<br>\
      >>> <b>Humidité</b> : "+String(humidity)+"%\
      <div style='max-width:60%;'><canvas id=\"myChart\" width=\"600\" height=\"450\"></canvas></div> \
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
