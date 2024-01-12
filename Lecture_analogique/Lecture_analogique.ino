
const int analogPin = A0; //Broche analogique à utiliser pour la lecture

void setup() 
{

  Serial.begin(9600); // Démarre la communication série à 9600 bits/s
  pinMode(D4, OUTPUT); //Configure la broche D4 en sortie

}

void loop() 
{

  int value = analogRead(analogPin); // Lit la valeur brute du convertisseur analogique

  Serial.print("Valeur analogique : ");
  Serial.println(value); // Affiche la valeur lue
  
  delay(100);

  if(value >= 700)
   {
    digitalWrite(D4, LOW);
   }
   else {
   digitalWrite(D4, HIGH);
    
   }

}













