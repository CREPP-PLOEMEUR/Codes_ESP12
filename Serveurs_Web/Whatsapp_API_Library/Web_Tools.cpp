#include "Web_Tools.h"


Web_Tools::Web_Tools() {
}

void Web_Tools::initWhatsapp_API(const String phoneNumber, const String apiKey)
{
    m_phoneNumber_whatsapp = phoneNumber;
    m_apiKey_whatsapp = apiKey;

}

uint16_t Web_Tools::sendMessage(String message)
{
    m_url_whatsapp = "http://api.callmebot.com/whatsapp.php?phone=" + m_phoneNumber_whatsapp + "&apikey=" + m_apiKey_whatsapp + "&text=" + urlEncode(message);
    m_httpClient.begin(m_WifiClient, m_url_whatsapp);
    m_httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
    return  m_httpClient.POST(m_url_whatsapp);
}

String Web_Tools::ipAddress2String(IPAddress ip)
{
  return String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") +  String(ip[3]); 
}