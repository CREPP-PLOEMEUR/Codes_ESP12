/*

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * Programme pour la lecture d'un capteur de présence PIR
 * par Club de Robotique et d'Electronique Programmable de Ploemeur
 * Autorisation de redistribuer et modifier le code sous les termes de la Licence GNU-GPL 
 */

#define LED D2    //Broche de la LED
#define OUT D7     //Broche du capteur PIR

int status = LOW;     //Statut du mouvement : LOW : pas de mouvement détecté
bool outValue = 0;    //Valeur du capteur
long beginTime = 0;   //instant du déclenchement lors de la ldétection d'un mouvement

void setup() 
{
  
	pinMode(LED, OUTPUT); //LED en sortie
	pinMode(OUT, INPUT);  //Broche du capteur en entrée
	Serial.begin(9600);   //Vitesse de communication à 9600 bauds
 
}//End setup

void loop(){

	outValue = digitalRead(OUT);                            //Lire létat du capteur
	
	if (outValue == HIGH)                                   //Détection d'un mouvement
	{
		digitalWrite(LED, HIGH);                              //Allume la LED

		if (status == LOW) 
		{
			Serial.println(">>> Mouvement ");
			status = HIGH;                                    //Mise à jour du statut du capteur
			beginTime = millis();                             //Sauvegarde du temps                   

		}//End if

	} 
	else                                                  //Aucun mouvement
	{
	  
		digitalWrite(LED, LOW);                             //Eteint la LED
		
		if(status == HIGH)                                  //Fin de détection
		{ 
			Serial.print(">>> Fin du mouvement");
			status = LOW;                                     //Mise à jour du statut du capteur
			unsigned long duree = millis() - beginTime;       //Calcul de la durée du mouvement
			Serial.print(">>> Duree = ");
			Serial.print(duree);
      		Serial.println(" ms");

		}//End if
	}//End else
}//End loop
