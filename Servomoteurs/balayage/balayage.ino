#include <Servo.h>

#define SERVO_PIN D2 //broche de commande

//Inclusion de la bibliothèque Servo
Servo myServo; // Création d'un objet Servo


void setup() 
{
  myServo.attach(SERVO_PIN);    //Choix de la broche du servo moteur
}
void loop() 
{
  for (int pos = 0; pos <= 180; pos += 1) 
  { 
    myServo.write(pos);  //Actualise la position
    delay(15);

  }//Fin for


  for (int pos = 180; pos >= 0; pos -= 1) 
  {
    myServo.write(pos);  //Actualise la position
    delay(15);

  }//Fin for
}//Fin loop