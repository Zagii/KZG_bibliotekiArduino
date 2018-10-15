/* *******************************************************
** Przykład użycia biblioteki, 
** przy pierwszym starcie tworzy plik konfiguracyjny
** po każdym restarcie dodaje do pliku symbol *
** ******************************************************* */

#include <KZGwifi.h>
#include <KZGmqtt.h>

KZGwifi wifi;
KZGmqtt mqtt;

void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
    Serial.println("callback srodek");
    return;
  char* p = (char*)malloc(length+1);
  memcpy(p,payload,length);
  p[length]='\0';
  /*if(strstr(topic,"watchdog"))
  {
    DPRINT("Watchdog msg=");
    DPRINT(p);
    DPRINT(" teraz=");
   
    if(isNumber(p))
      wifi.setWDmillis(strtoul (p, NULL, 0));
    DPRINTLN(wifi.getWDmillis());
    

  }*/
    if(topic[strlen(topic)-1]=='/')
        {topic[strlen(topic)-1] = '\0';}
    DPRINT("Debug: callback topic=");
    DPRINT(topic);
    DPRINT(" msg=");
    DPRINTLN(p);
    //parsujRozkaz(topic,p);
  free(p);
}

void setup()
{
    Serial.begin(115200);
    //////////////////////////////// wifi /////////////////////////////
    wifi.begin();
    // ustawienie domyślnych AP
    Serial.print("Przygotuj domyslny config");
    wifi.dodajAP("abc","cde");
    wifi.dodajAP("DOrangeFreeDom","KZagaw01_ruter_key"); 
    wifi.initAP("TestWifiAP","qwerty");
    
    //wczytanie konfiguracji i ew zastapienie domyślnych ustawien
    wifi.importFromFile();
    ////////////////////////////////////////////////////////////////////

    //////////////////////////// mqtt /////////////////////////////////
    mqtt.begin();
    mqtt.setCallback(MQTTcallback);
    // domyślna konfiguracja
    Serial.print("Przygotuj domyslny config MQTT");
    mqtt.setMqtt("broker.hivemq.com",1883,"ESP","","KZGmqttTestIN","KZGmqttTestOUT","KZGmqttDebug");
    mqtt.importFromFile();
    ////////////////////////////////////////////////////////////////////
    Serial.println("Koniec Setup"); 
}

unsigned long m=0;
void loop()
{
    wifi.loop();
    mqtt.loop();
    if(millis()-m>15000)
    {
        Serial.print("## ");Serial.print(wifi.getWifiStatusString());
        Serial.print(" #### ");Serial.print(wifi.getTimeString());Serial.println(" ####");

        mqtt.mqttPub("KZGmqttTestIN",wifi.getTimeString());
        m=millis();
    }
}
