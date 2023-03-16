/*

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * Programme pour le contrôle d'un serveur Web qui récupère l'adresse MAC de l'ESP
 * par Club de Robotique et d'Électronique Programmable de Ploemeur

 */

#include <ESP8266WiFi.h>

void setup()
{
  Serial.begin(115200);
}

void loop()
{
    Serial.print("ESP12 MAC Address:  ");
    Serial.println(WiFi.macAddress());
    delay(1000);
}