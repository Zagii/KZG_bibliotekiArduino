#ifndef KZGMQTT_h
#define KZGMQTT_h

#include <ArduinoJson.h>
#include <KZGconfigFile.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <LinkedList.h>  //https://github.com/ivanseidel/LinkedList

//#define DEBUG_MQTT
#undef DEBUG_MQTT   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG_MQTT    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

#define INFO_MQTT
#ifdef INFO_MQTT    //Macros are usually in all capital letters.
  #define IPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define IPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define IPRINT(...)     //now defines a blank line
  #define IPRINTLN(...)   //now defines a blank line
#endif

#define MAX_TOPIC_LENGHT 100

class KZGmqtt : public PubSubClient, public KZGconfigFile
{
String _confFileStr="/KZGmqtt.json";

LinkedList<String> _subscribeTopicList;

unsigned long lastMQTTReconnectAttempt;
bool lastMQTTconnecionStatus = false;   // zmienna do przechowywania chwili polaczenia i rozlaczenia MQTT

WiFiClient _espClient;

String _mqttServer="";
String _mqttUser="";
String _mqttPwd="";
uint16_t _mqttPort=1883;

String _mojID="KZGmqttClient";
String _debugTopic="";

public:
  KZGmqtt() : PubSubClient(), KZGconfigFile()  {};
  void begin(String confFileStr = "/KZGmqtt.json");
  void loop();
  bool reconnectMQTT();
  void setMqtt(String mqttServer,uint16_t mqttPort,String mojID, String mqttUser, String mqttPwd,  String debugTopic);
  bool importFromFile();
  String getConfigStr();
  void parseConfigStr(String confStr);
  void mqttPub(String topic, String msq, bool cisza = false);
  void addSubscribeTopic(String topic, bool saveConfig = false);
  bool testConfigParams();
};
#endif
