#include <ESP8266WiFiMulti.h>


#define TIMEOUT 5000
ESP8266WiFiMulti wifiHandler;


void setup()
{
  //Mise en mode Station
  WiFi.mode(WIFI_STA);

  //Enregistrement des reseaux Wifi
  wifiHandler.addAP("creafab_invite", "MonTraficEstJournalise");
  wifiHandler.addAP("YourSSID2", "YourPassword2");
  wifiHandler.addAP("YourSSID3", "YourPassword23");
}

void loop()
{
  // Maintain WiFi connection
  if (wifiHandler.run(TIMEOUT) == WL_CONNECTED) {
      print("OK")
  }
}