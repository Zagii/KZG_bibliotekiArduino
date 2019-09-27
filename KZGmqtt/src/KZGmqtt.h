#ifndef KZGMQTT_h
#define KZGMQTT_h

#include <ArduinoJson.h>
#include <KZGconfigFile.h>
#include <WiFiClient.h>
#include <PubSubClient.h>



#define DEBUG_MQTT   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG_MQTT    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

#define MAX_TOPIC_LENGHT 100

typedef void (*KZG_MQTT_CALLBACK)(String,String);
typedef void (*MQTT_CALLBACK)(char*, uint8_t*, unsigned int);

class KZGmqtt
{
String _confFileStr="/KZGmqtt.json";
KZGconfigFile _kzgConfigFile;

unsigned long lastMQTTReconnectAttempt;

WiFiClient _espClient;
PubSubClient _clientMqtt;
String _mqttServer="";
String _mqttUser="";
String _mqttPwd="";
uint16_t _mqttPort=1883;

String _inTopic="";
String _outTopic="";
String _debugTopic="";

//wskaznik na funkcje callback mqtt 
KZG_MQTT_CALLBACK _mqttCallBackFunc;
void mqttCallBackOryginal(char*,byte *,unsigned int);

public:
  KZGmqtt(){};
  void begin(String confFileStr = "/KZGmqtt.json");
  void loop();
  bool reconnectMQTT();
  void setMqtt(String mqttServer,uint16_t mqttPort, String mqttUser, String mqttPwd, String inTopic, String outTopic, String debugTopic);
 
  void setCallbackOryginal(MQTT_CALLBACK callback);
  void setCallback(KZG_MQTT_CALLBACK callback);

  uint8_t saveConfigFile(String confStr);
  String loadConfigFile();
  String getConfigStr();
  void parseConfigStr(String confStr);
  void mqttPub(String topic, String msq, bool cisza=false);

 // PubSubClient* getMqttObj(){return &_clientMqtt;};
};
#endif
