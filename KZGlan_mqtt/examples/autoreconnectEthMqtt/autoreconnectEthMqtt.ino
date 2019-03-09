/* *******************************************************
** Przykład użycia biblioteki, 
 *
** ******************************************************* */

#include <KZGlan_mqtt.h>

KZGlan_mqtt ethMqtt;

void mqttCallback(char* topic, uint8_t* payload, unsigned int length) 
{
  DPRINT(F("> Mqtt callback: "));
  String topicStr=String(topic);
  DPRINT(F("Topic:"));
  DPRINT(topicStr);

  DPRINT(F(" dlugosc= "));
  DPRINT(length);
 
  DPRINT(F(" MQTT_tresc= "));
  String msgStr="";
  for(int i=0;i<length;i++)
  {
    msgStr+=(char)payload[i];
  }
    DPRINTLN(msgStr);
  
}

void setup()
{
    Serial.begin(115200);
    byte mac[]    = {  0xCE, 0x00, 0x00, 0x00, 0x00, 0x01 };
    IPAddress mqttHost(192, 168, 1, 3);

    ethMqtt.begin(String("sample1"),mac,mqttHost,String(""),String(""),String(""),1883,mqttCallback);
     
    Serial.println("Koniec Setup"); 
}

unsigned long m=0;
void loop()
{
    ethMqtt.loop();
    if(millis()-m>15000)
    {
        Serial.print("## ");Serial.println(ethMqtt.getEthStatusString());
        m=millis();
        ethMqtt.publish(String("sample1/Sub/testFullPath/"),String(m));
        ethMqtt.publishPrefix(String("testPrefix"),String(m));

    }
}
