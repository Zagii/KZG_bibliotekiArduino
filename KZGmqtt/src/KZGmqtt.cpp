#include "KZGmqtt.h"

void KZGmqtt::begin(String confFileStr)
{
  setClient(_espClient);
  _confFileStr = confFileStr;
  KZGconfigFile::begin(_confFileStr);
  _subscribeTopicList.clear();
}

void KZGmqtt::setMqtt(String mqttServer,uint16_t mqttPort, String mojID,String mqttUser, String mqttPwd,  String debugTopic)
{
  _mqttServer = mqttServer;  _mqttPort = mqttPort;
  _mqttUser = mqttUser;      _mqttPwd = mqttPwd;
  _mojID=mojID;              _debugTopic=debugTopic;
  
  setServer(_mqttServer.c_str(), _mqttPort);
}

void KZGmqtt::addSubscribeTopic(String topic, bool saveConfig)
{
  DPRINT(F(" KZGmqtt::addSubscribeTopic: ")); DPRINTLN(topic);
  if(topic == "")
  {
    DPRINTLN(F("   blad- nie mozna subscrybowac pustego Topicu.")); 
    return;
  }
  for(uint8_t i = 0; i < _subscribeTopicList.size(); i++)
  {
    if(_subscribeTopicList.get(i) == topic)
    {
         DPRINT(F("   blad- Topic, już istnieje na pozycji: "));DPRINTLN(i);
         return;
    }
  }
  _subscribeTopicList.add(topic);
  subscribe(topic.c_str());
  if(saveConfig)
  {
    saveConfigFile(getConfigStr());
  }
  DPRINT(F(" KZGmqtt::addSubscribeTopic: subskrypcja udana -")); DPRINTLN(_subscribeTopicList.size());
}
bool KZGmqtt::reconnectMQTT()
{
  DPRINTLN(F(" KZGmqtt::reconnectMQTT "));
  bool cliCon;
  if(_mojID == "")
  {
    DPRINTLN(F("  Brak mojID - nie lacze z serwerem MQTT"));
    return false;
  }
  if(_mqttUser == "")
    cliCon = connect(_mojID.c_str());
  else
    cliCon = connect(_mojID.c_str() , _mqttUser.c_str(),_mqttPwd.c_str());

  return connected();
}

void KZGmqtt::mqttPub(String topic,String msg,bool cisza)
{
  if(!cisza){
    DPRINT("Debug String KZGmqtt::mqttPub, topic=");  DPRINT(topic); DPRINT(", msg=");  DPRINTLN(msg);
  }
  if(connected())
  {
    bool w=publish(topic.c_str(),msg.c_str()); 
   
   }else
   {
	   if(!cisza){DPRINTLN(" nie wysylam, brak polaczenia");}
   }
}
bool KZGmqtt::testConfigParams()
{
  if(_mqttServer == "") return false;
  if(_mqttPort == 0) return false;
  if(_mojID == "") return false;

  return true;
}
bool KZGmqtt::importFromFile()
{
    String conf = loadConfigFile();
    if(conf == "" || !testConfigParams())  //brak pliku konfiguracyjnego, lub bledne dane
    {
        conf = getConfigStr();
        if(conf != "")
        {
          uint8_t saveStatus = saveConfigFile(conf);
          DPRINT(F("  Zapis konfiguracji MQTT: "));
          DPRINTLN(saveStatus);
          if(saveStatus == KZGCONFFILE_SAVE_OK)
              return true;
          else
              return false;
        }
        else 
        {
          DPRINTLN(F("  Brak domyslnych konfiguracji MQTT"));
          return false;
        }
    }
    else
    {
        DPRINTLN(F("  Wczytywanie konfiguracji MQTT"));
        parseConfigStr(conf);
        return true;
    }
}
String KZGmqtt::getConfigStr()
{
  DynamicJsonBuffer jb;
  JsonObject &root = jb.createObject();
  JsonObject &mq = root.createNestedObject("MQTT");
  mq["mqttServer"] = _mqttServer;   mq["mqttPort"] = _mqttPort;
  mq["mqttUser"]   = _mqttUser;     mq["mqttPwd"]  = _mqttPwd;
  mq["mojID"] = _mojID;             mq["debugTopic"] = _debugTopic;
  JsonArray &arr = root.createNestedArray("subscribeTopic");
  DPRINT("lista subscr: "); DPRINTLN(_subscribeTopicList.size());
  for(uint8_t i = 0; i< _subscribeTopicList.size(); i++)
  {
     DPRINT(" --> ");DPRINT(i);DPRINT(" - "); DPRINTLN(_subscribeTopicList.get(i));
    arr.add(_subscribeTopicList.get(i));
  }
  String wynik;
  root.printTo(wynik);
  DPRINT(F("KZGmqtt::getConfigStr: "));DPRINTLN(wynik);
  return wynik;
}

void KZGmqtt::parseConfigStr(String confStr)
{
  DPRINT(F("  KZGmqtt::parseConfigStr: ")); DPRINTLN(confStr);

  DynamicJsonBuffer jb;
  JsonObject& root = jb.parseObject(confStr);
  if (!root.success())
  {
     DPRINT(F("    Blad parsowania confStr"));
     return;
  }
  JsonObject& mq=root["MQTT"];
  _mqttServer = mq["mqttServer"].as<char*>();  _mqttPort = mq["mqttPort"];
  _mqttUser   = mq["mqttUser"].as<char*>();    _mqttPwd = mq["mqttPwd"].as<char*>();
  _mojID = mq["mojID"].as<char*>();            _debugTopic = mq["debugTopic"].as<char*>();

  _subscribeTopicList.clear();
  JsonArray& ar=root["subscribeTopic"];
  for (String s: ar) 
  {
    //DPRINT("    parseStr--> ");DPRINTLN(s);
    _subscribeTopicList.add(s);
  } 
  
  DPRINTLN(F("  Koniec parseConfigStr "));
}

void KZGmqtt::loop()
{
  if (millis() - lastMQTTReconnectAttempt > 5000)
  {
    lastMQTTReconnectAttempt = millis();
    if(!connected())
    {
      if (reconnectMQTT())
      {
        lastMQTTconnecionStatus = true;
        IPRINT(F("  Nawiazano polaczenie MQTT, subskrypcja start: "));IPRINTLN(_subscribeTopicList.size());
        for(uint8_t i = 0; i < _subscribeTopicList.size(); i++)
        {
          subscribe(_subscribeTopicList.get(i).c_str());
          String bStr=" --- subskrybcja do: "+_subscribeTopicList.get(i);
          IPRINTLN(bStr);
          mqttPub(_debugTopic,bStr);
        }
      }
    }
    DPRINT(F("MQTT status: "));DPRINTLN(state());
  }
  if(connected())
  {
   PubSubClient::loop();
  }else
  {
    if(lastMQTTconnecionStatus)
    {
      IPRINTLN(F("Utracono połączenie MQTT"));
      lastMQTTconnecionStatus = false;
    }
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
