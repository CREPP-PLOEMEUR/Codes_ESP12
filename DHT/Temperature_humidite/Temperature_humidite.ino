/*

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * Programme pour la lecture d'un capteur de température DHT11/22
 * par Club de Robotique et d’électronique Programmable de Ploemeur
 * Autorisation de redistribuer et modifier le code sous les termes de la Licence GNU-GPL 

 */
#include "DHT.h"
#define DHTPIN D2     //Broche du capteur

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22 
//#define DHTTYPE DHT21   // DHT 21 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  
  Serial.begin(115200);
  dht.begin();          //Initialisation du capteur
}

void loop() {

  float h = dht.readHumidity();       //Récupère la température
  float t = dht.readTemperature();    //récupère l'humidité

  Serial.print("Humidite: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.println("°C ");
  
  delay(2000);

}
