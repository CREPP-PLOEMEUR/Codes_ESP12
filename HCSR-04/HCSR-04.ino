/*

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * Programme pour l'utilisation d'un capteur de distance ultrason
 * par Club de Robotique et d'Electronique Programmable de Ploemeur
 * Autorisation de redistribuer et modifier le code sous les termes de la Licence GNU-GPL 

 */
#define TRIGGER_PIN D5  //Broche Trigger
#define ECHO_PIN D6     //Broche Echo

#define SOUND_VELOCITY 0.034 //Vitesse en cm/us

float distance = 0.0;

void setup() {

  Serial.begin(115200);         //Communication à 9600 bauds
  pinMode(TRIGGER_PIN, OUTPUT); //Broche Trigger en sortie
  pinMode(ECHO_PIN, INPUT);     //Broche Echo en entrée

  digitalWrite(TRIGGER_PIN, LOW);

}//End setup

void loop() {

  digitalWrite(TRIGGER_PIN, HIGH); //Envoie une impulsion de 10µs
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  int duration = pulseIn(ECHO_PIN, HIGH); //Récupère le temps à l'état Haut de la broche ECHO

  distance = duration * SOUND_VELOCITY/2; //Distance en cm

  Serial.print("Distance (cm) = ");
  Serial.println(distance);
  
  delay(1000);
}