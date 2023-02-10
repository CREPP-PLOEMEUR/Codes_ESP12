/*

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * Programme pour la mise en place d'un serveur Web simple avec un écran OLED pour afficher l'adresse IP
 * par Club de Robotique et d'Électronique Programmable de Ploemeur
 * Autorisation de redistribuer et modifier le code sous les termes de la Licence GNU-GPL 

 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define PORT 80 //Port par défaut
#define LED D4  //Broche de la LED

ESP8266WebServer server(PORT);

const char* ssid     = "creafab_invite";//Nom du routeur sur le réseau 
const char* password = "MonTraficEstJournalise"; //Mot de passe du routeur


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 //Broche reset
#define SCREEN_ADDRESS 0x3C ///Parfois 0x3D ou 0x3F
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
  Serial.begin(115200);       //Communication à 115200 bits/s
  WiFi.begin(ssid, password); //Connexion
  Serial.println("");         //Retour à la ligne

  delay(100);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println(">>> Impossible de se connecter au réseau...");
  }
  

  if (MDNS.begin("esp8266")) {   //Multicast DNS 
    Serial.println(">>> Serveur MDNS activé");
  }

  server.on("/", mainPage);           //Affichage de la page principale si requête sur '/' -> saisir IP dans le navigateur
  server.onNotFound(notFoundPage);    //Affichage de la page d'erreur si adresse non valide

  server.begin();                     //Initialisation du serveur
  Serial.println(">>> démarrage du serveur : OK");

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Echec OLED"));
    for(;;); 
  }

  display.display(); //Affiche le logo Adafruit
  delay(500); 
  display.clearDisplay(); //efface l'écran

  display.setTextSize(1);               //Facteur d'échelle
  display.setTextColor(SSD1306_WHITE);  //texte blanc
  display.setCursor(0, 0);              //Place le curseur en (0,0)
  
  display.println("Connexion au reseau ");
  display.println(ssid);
  display.println("avec l'adresse IP : ");
  display.println(WiFi.localIP());
  
  display.display();   //met à jour le contenu
  
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
      display.clearDisplay();
      display.setCursor(0, 0);  
      display.println("LED : 1");
      display.display();
      
 }//Fin if
 else if(server.arg("LED")=="OFF")
 {
    digitalWrite(LED, HIGH);   //On eteint la led
    display.clearDisplay();
    display.setCursor(0, 0);  
    display.println("LED : 0");
    display.display();
 }//Fin else if
 else {
  //nothing
 }

 server.send(200, "text/html", minimalPageContent); //On envoie la page principale
  
}//FIn mainPage


void notFoundPage()  //Gestion si mauvaise URL
{
  server.send(404, "text/plain", "Page introuvable !\n\n");
  
}
