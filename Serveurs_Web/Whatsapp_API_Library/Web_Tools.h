#include "IPAddress.h"

#ifndef WEB_TOOLS
#define WEB_TOOLS 

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>


typedef enum
{
	Whatsapp_OK = 200,
	Whatsapp_FAIl = 400

} WhatsApp_Response;

class Web_Tools

{

public:

	Web_Tools();

	//Whatsapp
	void initWhatsapp_API(const String phoneNumber, const String apiKey);
	uint16_t sendMessage(String message);
	
	String ipAddress2String(IPAddress ip);

/*
#  private methods and attributs
*/
private:

	String m_phoneNumber_whatsapp;
	String m_apiKey_whatsapp;
	String m_url_whatsapp;
	//
	WiFiClient m_WifiClient;    
	HTTPClient m_httpClient;

};


#endif
