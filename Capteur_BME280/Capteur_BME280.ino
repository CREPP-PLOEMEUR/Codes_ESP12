#include <Wire.h>   //Bibliothèque pour communication I2C

#include <Adafruit_Sensor.h>
#include "Adafruit_BME280.h"


#define SEALEVELPRESSURE_HPA (1013.25)  //Pression au niveau de la mer  

Adafruit_BME280 bme; //Créer un objet bme

void setup() {

  Serial.begin(9600);
  Serial.println("BME28E");

  //Addresse par défaut
  bool status = bme.begin(0x76);  

  if (!status) 
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

}

void loop() { 

  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" °C");
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
  delay(1000);
}
