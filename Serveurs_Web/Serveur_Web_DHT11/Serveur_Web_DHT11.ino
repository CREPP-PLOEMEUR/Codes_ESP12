/*

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * Programme pour le contrôle d'un serveur Web qui affiche la température et l'humidité sur une page Web
 * par Club de Robotique et d'Électronique Programmable de Ploemeur

 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#include "DHT.h"
#define PORT 80 //Port par défaut
#define LED D4  //Broche de la LED

#define REFRESH_PAGE_DELAY 5 //Temps de reafraichissement de la page en s

#define DHTPIN D2     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   
//#define DHTTYPE DHT22  
//#define DHTTYPE DHT21 

DHT dht(DHTPIN, DHTTYPE);


ESP8266WebServer server(PORT);
/*
const char* ssid     = "creafab_invite";//Nom du routeur sur le réseau (par Exemple FReebox-44F45)
const char* password = "MonTraficEstJournalise"; //Mot de passe du routeur
*/

float temperature = 20.0;
float humidity = 50.0;

const char* ssid     = "creafab_invite";//Nom du routeur sur le réseau (par Exemple FReebox-44F45)
const char* password = "MonTraficEstJournalise"; //Mot de passe du routeur


//Page principale
const String minimalPageContent = "<html>\
  <head>\
    <title>Serveur Web CREPP</title>\
    <meta charset=\"utf-8\"/> \
    </head>\
  <body>\
    <h1>Led ESP8266</h1><br>\
    <h3>Contrôle de la LED sur la broche D4</h3><br>\
      <a href=\"/?LED=ON\"><button >Allumer</button></a>\
      <a href=\"/?LED=OFF\"><button >Eteindre</button></a>\
  </body>\
</html>";


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
 else if(server.arg("UPDATE")=="1")
 {
  server.send(200, "text/html", getString()); //On envoie la page principale
  }
 else {
  //nothing
 }

 server.send(200, "text/html", getString()); //On envoie la page principale
  
}//FIn mainPage


void notFoundPage()  //Gestion si mauvaise URL
{
  server.send(404, "text/plain", "Page introuvable !\n\n");
  
}


String getString() {//Récupère la page

  temperature = dht.readTemperature();
  humidity  = dht.readHumidity();
  
  Serial.println(">>> Température = "+String(temperature));
  Serial.println(">>> Humidité = "+String(temperature));
  
  const String fullPageContent = "<html>\
  <head>\
    <title>Serveur Web CREPP</title>\
    <meta charset=\"utf-8\"/> \
    <meta http-equiv=\"refresh\" content=\""+String(REFRESH_PAGE_DELAY)+"\">\
    <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">\
  </head>\
  <body style=\"margin-left:5%;\">\
    <h1>Led ESP8266</h1><br>\
    <h3>Contrôle de la LED sur la broche <span class=\"badge badge-secondary\">D4</span></h3><br>\
      <a href=\"/?LED=ON\"><button class=\"btn btn-success\">Allumer</button></a>\
      <a href=\"/?LED=OFF\"><button class=\"btn btn-danger\">Eteindre</button></a><br><br>\
    <h3>Mesure de la température et humidité avec le module DHT11 sur la broche <span class=\"badge badge-secondary\">D2</span></h3><br>\
      <a href=\"/?UPDATE=1\"><button class=\"btn btn-primary\">Mettre à jour</button></a><br><br>\
      >>> <b>Température</b> : "+String(temperature)+" °C<br>\
      >>> <b>Humidité</b> : "+String(humidity)+"%\
  </body>\
</html>";

return fullPageContent;

}
