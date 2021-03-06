#include "KZGmqtt.h"

void KZGmqtt::begin(String confFileStr)
{
  _clientMqtt.setClient(_espClient);
  _confFileStr = confFileStr;
  _kzgConfigFile.begin(_confFileStr);
}

void KZGmqtt::setMqtt(String mqttServer,uint16_t mqttPort, String mqttUser, String mqttPwd, String inTopic, String outTopic, String debugTopic)
{
  _mqttServer = mqttServer;  _mqttPort = mqttPort;
  _mqttUser = mqttUser;      _mqttPwd = mqttPwd;
  _inTopic=inTopic;          _outTopic=outTopic;
  _debugTopic=debugTopic;
  _clientMqtt.setServer(_mqttServer.c_str(), _mqttPort);
}
void KZGmqtt::setCallback(MQTT_CALLBACK_SIGNATURE) {
    _clientMqtt.setCallback(callback);
}
bool KZGmqtt::reconnectMQTT()
{
  
  bool cliCon;
  if(_mqttUser == "") return false;
  if(_mqttPwd == "")
    cliCon = _clientMqtt.connect(/*nodeMCUid*/ _mqttUser.c_str());
  else
    cliCon = _clientMqtt.connect(/*nodeMCUid*/ _mqttUser.c_str() , _mqttUser.c_str(),_mqttPwd.c_str());

  if (cliCon) 
  {
    char s[MAX_TOPIC_LENGHT];
    strcpy(s,_inTopic.c_str());
    strcat(s,"\/#");  
    _clientMqtt.subscribe(s);
    String bStr="reconnectMQTT, subskrybcja do: "+String(s);
    mqttPub(_debugTopic,bStr);
  }
  return _clientMqtt.connected();
}

void KZGmqtt::mqttPub(String topic,String msg,bool cisza)
{
  if(!cisza){
    DPRINT("Debug String KZGmqtt::mqttPub, topic=");  DPRINT(topic); DPRINT(", msg=");  DPRINTLN(msg);
  }
  if(_clientMqtt.connected())
  {
	  _clientMqtt.publish(topic.c_str(),msg.c_str());
   
   }else
   {
	   if(!cisza){DPRINTLN(" nie wysylam, brak polaczenia");}
   }
}

String KZGmqtt::loadConfigFile()
{
  return _kzgConfigFile.loadConfigFile();
}

uint8_t KZGmqtt::saveConfigFile(String confStr)
{
  return _kzgConfigFile.saveConfigFile(confStr);
}

String KZGmqtt::getConfigStr()
{
  DynamicJsonBuffer jb;
  JsonObject &root = jb.createObject();
  JsonObject &mq = root.createNestedObject("MQTT");
  mq["mqttServer"] = _mqttServer;   mq["mqttPort"] = _mqttPort;
  mq["mqttUser"]   = _mqttUser;     mq["mqttPwd"]  = _mqttPwd;
  mq["inTopic"]    = _inTopic;      mq["outTopic"] = _outTopic;
  mq["debugTopic"] = _debugTopic;
  String wynik;
  root.printTo(wynik);
  DPRINT(F("KZGmqtt::getConfigStr: "));DPRINTLN(wynik);
  return wynik;
}

void KZGmqtt::parseConfigStr(String confStr)
{
  DPRINT(F("KZGmqtt::parseConfigStr: ")); DPRINTLN(confStr);

  DynamicJsonBuffer jb;
  JsonObject& root = jb.parseObject(confStr);
  if (!root.success())
  {
     DPRINT(F("Blad parsowania confStr"));
     return;
  }
  JsonObject& mq=root["MQTT"];
  _mqttServer = mq["mqttServer"].as<char*>();  _mqttPort = mq["mqttPort"];
  _mqttUser   = mq["mqttUser"].as<char*>();    _mqttPwd = mq["mqttPwd"].as<char*>();
  _inTopic    = mq["inTopic"].as<char*>();     _outTopic = mq["outTopic"].as<char*>();
  _debugTopic = mq["debugTopic"].as<char*>();
  
  DPRINTLN(F("Koniec parseConfigStr "));
}

void KZGmqtt::loop()
{
  if (millis() - lastMQTTReconnectAttempt > 5000)
  {
    lastMQTTReconnectAttempt = millis();
    if(!_clientMqtt.connected())
    {
      if (reconnectMQTT())
      {
      }
    }
    DPRINT(_mqttUser);DPRINT(F(" / "));DPRINTLN(_mqttPwd); DPRINT(F("MQTT status: "));DPRINTLN(_clientMqtt.state());
  }
  /*if(loopMillis%600000==0) //10 min wysyłaj pingi watchdoga cyklicznie 
           {
            char m[MAX_MSG_LENGHT];
            sprintf(m,"%ld",loopMillis);
            char m2[MAX_TOPIC_LENGHT];
            sprintf(m2,"%s/watchdog",inTopic);
            RSpisz(m2,m);
          }
          if(loopMillis%600000==0)//10 min sprawdza czy wrocil ping watchdoga
          {
            String str="czas od restartu= "+(String) TimeToString(loopMillis/1000);
            DPRINTLN(str);
            DPRINT("Watchdog czas ");
            DPRINTLN(loopMillis-WDmillis);
            if(loopMillis-WDmillis>600000)
            {
                RSpisz(debugTopic,"Watchdog restart");
                delay(3000);
                //ESP.restart();
              //  ESP.reset();  // hard reset
              //  resetFunc();
            }
          }
          */
}
