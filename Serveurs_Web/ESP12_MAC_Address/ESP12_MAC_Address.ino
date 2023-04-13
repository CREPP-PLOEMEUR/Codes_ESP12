/*
 * This program is free software; you can redistribute it and/or
 * modify without any restriction
 * 
 * This file get the MAC address of ESP12 NodeMCU device
 */

/*
 *   Libraries
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