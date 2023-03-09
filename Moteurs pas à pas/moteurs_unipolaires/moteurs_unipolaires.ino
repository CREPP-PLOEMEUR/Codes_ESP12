/*

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * Programme pour le contrôle d'un moteur pas à pas
 * par Club de Robotique et d’électronique Programmable de Ploemeur
 * Autorisation de redistribuer et modifier le code sous les termes de la Licence GNU-GPL 

 */
  #include <Stepper.h> //Inclusion de la bibliothèque Stepper
  
  int nbPas = 2048; //Nombre de pas pour le moteur 28BYJ-48
  
  #define IN1 D1  //Broche IN1
  #define IN2 D5  //Broche IN2
  #define IN3 D2  //Broche IN3
  #define IN4 D6  //Broche IN4
  
  Stepper moteur(nbPas, IN1, IN3, IN2, IN4); //Création de l'objet moteur
     
    void setup() {
  
      moteur.setSpeed(10); //On définit la vitesse à 10 tr/min
  
    }//Fin setup
     
    void loop() {
  
      moteur.step(2048);   //On avance de nbPas pas, c'est à dire un tour complet (sens horaire)
      delay(1000);          //pause de 2s
      moteur.step(-2048);  //On avance de -nbPas pas, c'est à dire un tour complet (sens anti-horaire)
      delay(1000);          //pause de 2s
  
    }//Fin loop
