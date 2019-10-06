/* *******************************************************
** Przykład użycia biblioteki, 
** przy pierwszym starcie tworzy plik konfiguracyjny
** po każdym restarcie dodaje do pliku symbol *
** ******************************************************* */

#include <KZGwifi.h>
#include <KZGmqtt.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <KZGoneWireDev.h>

KZGwifi wifi;
KZGmqtt mqtt;
KZGoneWireDev oneW;

void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
  DPRINTLN("MQTTcallback");
  char* p = (char*)malloc(length+1);
  memcpy(p,payload,length);
  p[length]='\0';
  
  if(topic[strlen(topic)-1]=='/')
    {topic[strlen(topic)-1] = '\0';}
  String topicStr=topic;
  String msgStr=p;
  free(p);
  DPRINT("Debug: callback topic=");  DPRINT(topicStr);
  DPRINT(" msg=");    DPRINTLN(msgStr);    DPRINTLN(p);
  mqttCallbackStr(topic,p);
}

void mqttCallbackStr(String topic, String msg)
{
  DPRINTLN("mqttCallback= topic: "+topic+", msg: "+msg);

}

void setup()
{
    Serial.begin(115200);
    wifi.begin("MqttTestAP");
    
    mqtt.begin();
    mqtt.setCallbackOryginal(MQTTcallback);
    mqtt.setMqtt("qnap",1883,"ESP","","KZGmqttTestIN","KZGmqttTestOUT","KZGmqttDebug");
    oneW.begin(D4,12,20000); // pin, precyzja, co ile mierzyc
    Serial.println("Koniec Setup"); 
}

unsigned long m=0;
void loop()
{
    wifi.loop();
    mqtt.loop();
    oneW.loop();
    if(oneW.get18b20ready())
    {
      oneW.set18b20notReady();
      DPRINT(" ************* oneWire Temperatures ready liczba: ");DPRINTLN(oneW.getDevicesCount());
      for(int i=0;i<oneW.getDevicesCount();i++)
      {
        DPRINT(" ** i= ");DPRINT(i);DPRINT(", addr: ");DPRINT(oneW.getDeviceAddresStr(i));DPRINT(", T=");DPRINT(oneW.getDeviceTemperature(i));DPRINTLN(" C");
      }
      DPRINTLN(" ************************ koniec ***************************** ");
      
    }
    if(millis()-m>15000)
    {
        Serial.print("## ");Serial.print(wifi.getWifiStatusString());Serial.println(" ##");
        Serial.print("#### ");Serial.print(wifi.getTimeString());Serial.println(" ####");
        mqtt.mqttPub("KZGmqttTestIN",wifi.getTimeString());
        m=millis();
    }
}
