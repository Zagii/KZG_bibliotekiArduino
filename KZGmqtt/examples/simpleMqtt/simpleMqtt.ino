/* *******************************************************
** Przykład użycia biblioteki, 
** przy pierwszym starcie tworzy plik konfiguracyjny
** po każdym restarcie dodaje do pliku symbol *
** ******************************************************* */

#include <KZGwifi.h>
#include <KZGmqtt.h>

KZGwifi wifi;
KZGmqtt mqtt;

void callback(char* topic, byte* payload, unsigned int length) 
{
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
    wifi.begin();
    String conf=wifi.loadConfigFile();
    if(conf == "")  //brak pliku konfiguracyjnego
    {
        Serial.print("Przygotuj domyslny config");
        wifi.dodajAP("DOrangeFreeDom","KZagaw01_ruter_key");
        wifi.initAP("TestWifiAP","qwerty");
        conf = wifi.getConfigStr();
        Serial.println("Zapis konfiguracji: "+wifi.saveConfigFile(conf));
    }else
    {
        Serial.print("Wczytywanie konfiguracji");
        wifi.parseConfigStr(conf);
    }
    mqtt.begin();
    mqtt.setCallback(callback);
    String confMqtt=mqtt.loadConfigFile();
    if(confMqtt == "")
    {
        Serial.print("Przygotuj domyslny config MQTT");
        mqtt.setMqtt("broker.hivemq.com",1883,"mqttUser","");
        confMqtt=mqtt.getConfigStr();
        Serial.println("Zapis konfiguracji MQTT: "+ mqtt.saveConfigFile(confMqtt));
    }else
    {
        Serial.print("Wczytywanie konfiguracji MQTT");
        mqtt.parseConfigStr(confMqtt);
    }
    Serial.println("Koniec Setup"); 
}

unsigned long m=0;
void loop()
{
    wifi.loop();
    mqtt.loop();
    if(millis()-m>15000)
    {
        Serial.print("## ");Serial.print(wifi.getWifiStatusString());Serial.println(" ##");
        Serial.print("#### ");Serial.print(wifi.getTimeString());Serial.println(" ####");
        m=millis();
    }
}
