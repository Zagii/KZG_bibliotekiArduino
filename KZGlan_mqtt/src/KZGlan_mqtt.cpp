#include "KZGlan_mqtt.h"

void KZGlan_mqtt::begin(String name,byte [] mac, IPAddress mqttHostIP, String mqttHost,String mqttUser,String mqttPwd, uint16_t mqttPort, KZGlan_mqttCallback mqttCallback)
{
  DPRINTLN("Debug KZGlan_mqtt::begin start"); 
  String _name=name;
  _mac[0]=mac[0];_mac[1]=mac[1];_mac[2]=mac[2];
  _mac[3]=mac[3];_mac[4]=mac[4];_mac[5]=mac[5];
  String _mojTopicIDSubscribe=name+"/Sub/"; //topic z komunikatami z serwera
  String _mojTopicIDSubscribeConfig=name+"/ConfigSub/"; //topic z komunikatami z serwera
  String _mojTopicIDPublish=name+"/Pub/"; // topic z komunikatami do serwera
  String _mojTopicIDPublishDebug="DebugTopic/"+name+"/"; // topic z komunikatami do serwera
  String _mojTopicIDPing=name+"/Ping/"; //topic z komukatami keepalive
  _mqttUsr=mqttUser;
  _mqttPwd=mqttPwd;
  _mqttPort=mqttPort;
  if(mqttHost=="")
  {
    _mqttHostDNS="";
    _mqttHostIP[0]=mqttHostIP[0];_mqttHostIP[1]=mqttHostIP[1];
    _mqttHostIP[2]=mqttHostIP[2];_mqttHostIP[3]=mqttHostIP[3];
    _mqttClient.setServer(_mqttHostIP, _mqttPort);
  }else
  {
    _mqttHostDNS=mqttHost;
    _mqttClient.setServer(_mqttHostDNS, _mqttPort);
  }
  //_mqttClient.setCallback(MQTTcallback); <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  _mqttCallback = mqttCalback;
  _mqttClient.set_callback([this] (char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); }

  _mqttClient.setClient(_ethClient);
  ethReconnect();
  mqttReconnect();
  DPRINTLN("Debug KZGlan_mqtt::begin end"); 
}

void KZGlan_mqtt::ethReconnect()
{
  DPRINTLN(F("KZGlan_mqtt::ethReconnect")); 
  Ethernet.stop();
  Ethernet.begin(_mac);
  //delay(500);
  DPRINTLN(getEthStatusString());
}
void KZGlan_mqtt::mqttReconnect()
{
  DPRINTLN(F("KZGlan_mqtt::mqttReconnect")); 
  if (!_mqttClient.connected()) 
  {
    _mqttClient.disconnect();
    DPRINT(F("Proba polaczenia z MQTT .."));
    bool mqttconnStat=false;
    if(_mqttUsr.length>0)
    {
      mqttconnStat=_mqttClient.connect(_name.c_str(),_usr.c_str(),_pwd.c_str());
    }else
    {
      mqttconnStat=_mqttClient.connect(_name.c_str());
    }
    if(mqttconnStat) 
    {
        _mqttClient.subscribe(_mojTopicIDSubscribe.c_str());
        _mqttClient.subscribe(_mojTopicIDSubscribeConfig.c_str());
        _mqttClient.subscribe(_mojTopicIDPing.c_str());
        _mqttClient.publish(_mojTopicIDPublishDebug.c_str(),"Polaczenie");
        probaReconnect=0;
        DPRINTLN(F(" [ok]"));
      }else
      {
        DPRINT(F("nieudane: "));
        DPRINTLN(client.state());
      }
    }
}



String KZGlan_mqtt::getEthStatusString() 
{
  String statStr=""; 
  if(_ethClient.connected())
  {
    IPAddress ip=_ethClient.localIP();
    statStr="Eth connected: %d.%d.%d.%d\n"+ ip[0]+"."+ip[1]+"."+ip[2]+"."+ip[3];
    
  }else
  {
    statStr="Eth Connection Error. status= "+ _ethClient.connected();
    
  }
  return statStr;
}

void KZGlan_mqtt::loop()
{
  unsigned long now = millis();

  /////////////////////////////// ETH  test connection ////////////////////////////////
  if(!_ethClient.connected())
  {
    if( isEthNewDisconnecion)
    {
      DPRINTLN(F("*** -> Eth disconnected ***"));
      DPRINTLN(getEthStatusString());
      isEthNewDisconnecion=false;
    }
    if (now - lastEthReconnectAttempt > 5000) 
    {
      lastEthReconnectAttempt = now;
      DPRINTLN(F("*** Eth reconnecting.."));
      isEthNewConnecion=true;
      ethReconnect();
    }
  } else
  {
    if(_isEthNewConnecion)
    {
      DPRINTLN(F("*** -> Eth new conn estabilished ***"));
      DPRINTLN(getEthStatusString());
      _isEthNewConnecion=false;
      _isEthNewDisconnecion=true;
    }
    ////////////////////////////////////////////////////////////////////////////
  
  
    if (!_mqttClient.connected())
    {
      if (now - lastMqttReconnectAttempt > 5000) 
      {
        lastMqttReconnectAttempt = now;
        ++probaReconnect;
        DPRINTLN(F("***** MQTT reconnecting.."));
        mqttReconnect();
      }
    } 
    else
    {    // Client connected
      _mqttClient.loop();
      if(now-lastPingMillis>60000)
      {
        lastPingMillis=now;
        lp++;
        
        unsigned long sec = millis() / 1000;
        unsigned long min = sec / 60;
        unsigned long hr = min / 60;
        unsigned long dd = hr /24;

        sec=sec%60;
        min=min%60;
        hr=hr%24;
        
        char tmp[50];
        sprintf ( tmp,"%s: %lu= %02lud%02lu:%02lu:%02lu ",_name,lp,dd,hr, min,sec);
        
        client.publish(_mojTopicIDPing.c_str(),tmp);
      }  
    }
  }


}
void KZGlan_mqtt::callback(char* topic, byte* payload, unsigned int length) 
{
  String topicStr=String(topic);
  DPRINT(F("Topic:"));
  DPRINT(topicStr);

  DPRINT(F(" dlugosc= "));
  DPRINT(length);
 
  DPRINT(F(" MQTT_tresc= "));
  String msgStr="";
  for(int i=0;i<length;i++)
  {
    msgStr[i]+=(char)payload[i];
  }
  msgStr[length]='\0';
  DPRINTLN(msgStr);

    
  if(strcmp(_mojTopicIDPing,topicStr.c_str())==0) //powrocil komunikat ping
  {
    _ostatniOdczytanyPingMqtt=millis();
  }else if(strcmp(_mojTopicIDSubscribeConfig,topicStr.c_str())==0)
  {
    
  }else if(strcmp(_mojTopicIDSubscribe,topicStr.c_str())==0)
  {
    _mqttCallback(topicStr,msgStr);
  }
}
void KZGlan_mqtt::publish(String topic, String msg)
{
  if (_mqttClient.connected())
  {
    _mqttClient.publish(topic.c_str(),msg.c_str());
  }
}