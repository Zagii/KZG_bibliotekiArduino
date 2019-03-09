#ifndef KZGlan_mqtt_h
#define KZGlan_mqtt_h

#include <Ethernet.h>
#include <PubSubClient.h>


#define DEBUG_LAN_MQTT   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG_LAN_MQTT    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

typedef void (*KZGlan_mqttCallback) (String topic, String msg);


class KZGlan_mqtt
{
  byte _mac[6];//    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
  EthernetClient _ethClient;
  PubSubClient _mqttClient;    
  unsigned long _ostatniOdczytanyPingMqtt=0;    // mqtt callback ustawia odczyt pingu
  unsigned long _lastPingMillis=0;              // zapisuje kiedy ostatnio wysłał ping
  unsigned long _lastEthReconnectAttempt = 0;   // timer for auto eth reconnect
  unsigned long _lastMqttReconnectAttempt = 0;  // timer for auto mqtt reconnect
  bool _isEthNewConnecion=false;                // flag - new connection estabilished
  bool _isEthNewDisconnecion=false;             // flag - Eth dc occured
  
  String _mqttHostDNS = "m20.cloudmqtt.com";  // dns - mqtt host
  IPAddress _mqttHostIP;                      // ip - mqtt  host
  uint16_t _mqttPort=11379;
  String _mqttUsr="";
  String _mqttPwd="";
  
  String _name="Cdef";                         // human name of device
  String _mojTopicIDSubscribe="/Sub/";         //topic z komunikatami z serwera
  String _mojTopicIDSubscribeConfig="/Config/";   //topic z komunikatami z serwera
  String _mojTopicIDPublish="C1/Pub/";         // topic z komunikatami do serwera
  String _mojTopicIDPublishDebug="DebugTopic/C1/"; // topic z komunikatami do serwera
  String _mojTopicIDPing="C1/Ping/";           //topic z komukatami keepalive

  void callback(char* topic, uint8_t* payload, unsigned int length);
  void ethReconnect();
  void mqttReconnect();

  KZGlan_mqttCallback _mqttCallback;
 //MQTT_CALLBACK_SIGNATURE _mqttCallback;
public:
  KZGlan_mqtt(){};
  void begin(String name,byte * mac, IPAddress mqttHostIP, String mqttHost,String mqttUser,String mqttPwd, uint16_t mqttPort, KZGlan_mqttCallback mqttCallback);
  void loop();
  String getEthStatusString();
  void publish(String topic, String msg);
  void publishPrefix(String topic, String msg);
  PubSubClient getMqttClient(){return _mqttClient;}
};
#endif
