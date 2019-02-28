/* *******************************************************
** Przykład użycia biblioteki, 
 *
** ******************************************************* */

#include <KZGlan_mqtt.h>

KZGlan_mqtt ethMqtt;

void mqttCallback(String topic, String msg)
{
    Serial.print("### mqttCallback -> topic: ");
    Serial.print(topic);
    Serial.print(", msg: ");
    Serial.println(msg);
}


void setup()
{
    Serial.begin(115200);
    byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
    IPAddress mqttHost(192, 168, 0, 3);

    ethMqtt.begin("sample1",mac,mqttHost,"","","",1883,mqttCallback);
     
    Serial.println("Koniec Setup"); 
}

unsigned long m=0;
void loop()
{
    ethMqtt.loop();
    if(millis()-m>15000)
    {
        Serial.print("## ");Serial.print(ethMqtt.getEthStatusString());
        m=millis();
        ethMqtt.publish(String("sample1/Sub/testFullPath/"),String(m));
        ethMqtt.publishPrefix(String("testPrefix"),String(m));

    }
}
