#include "KZGwifi.h"



void KZGwifi::wifiReconnect(bool force=false)
{
   DPRINT(F("KZGwifi::wifiReconnect, ileSSID=")); DPRINTLN(ileSSID);

  if(force)
  {
    DPRINTLN(F("  Force disconnect WIFI"));
    WiFi.disconnect();
    delay(30);
  }
  if(status == WL_CONNECTED) return;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssidTab[idSSID],pwdTab[idSSID]);
 
  if(wifiMulti) delete wifiMulti;
  lastConnectedStatus=WL_IDLE_STATUS;
  wifiMulti=new ESP8266WiFiMulti();
  for(uint8_t i=0; i<ileSSID; i++)
  {
    wifiMulti->addAP(ssidTab[i].c_str(), pwdTab[i].c_str());
  }
  delay(1000);
}
void KZGwifi::setNTP(String host,unsigned long offset_h)
{
  ntp_server=host;
  if(timeClient)
  {
    timeClient->end();
    delete timeClient; 
  }
  ntp_offset_h=offset_h;
  timeClient=new NTPClient(ntpUDP, ntp_server.c_str(), ntp_offset_h*3600, 60000);
  timeClient->begin();
}

void KZGwifi::dodajAP(String jsonString)
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& js= jsonBuffer.parse(jsonString);
  if (!js.success()) 
  {
    DPRINTLN("Blad parsowania zmianyAP");
    return;
  }
  if(js.containsKey("ssid"))
  {
    String s=js["ssid"];
    if(js.containsKey("pwd"))
    {
      String p=js["pwd"];
      dodajAP(s,p);
    }
  }
}
void KZGwifi::dodajAP(String ssid,String pwd, bool dodajNaKoniec)
{
  DPRINT(F("KZGwifi::dodajAP: ")); DPRINT(ssid);
  DPRINT(F(" pwd: ")); DPRINTLN(pwd);
  
  // przed dodaniej ssid usuwamy wszystkie ssid o tej nazwie
  // by nie bylo przypadku ze jedno ssid ma wiecej niz jedno hasło
  usunAP(ssid);
  if(ileSSID>=MAX_SSID) 
  {
    DPRINTLN(F("Za duzo SSID nie moge dodac nowego"));
    return;
  }
  // przesuwamy tablice o jeden by nowy ssid wpadl na poczatek
  for(uint8_t i=ileSSID; i > 0; i--)
  {
    ssidTab[i]=ssidTab[i-1];
    pwdTab[i]=pwdTab[i-1];
  }
  if(dodajNaKoniec)
  {
    ssidTab[0]=ssid;
    pwdTab[0]=pwd;
  }else
  {
    ssidTab[ileSSID]=ssid;
    pwdTab[ileSSID]=pwd;
  }
  ileSSID++;
  wifiReconnect();
  DPRINT(F("Koniec dodawania, ileSSID = ")); DPRINTLN(ileSSID); 
}
/* usuwa wszystkie AP o zadanym SSID */
void KZGwifi::usunAP(String ssid)
{
  DPRINT(F("KZGwifi::usunAP: ")); DPRINT(ssid);
  DPRINT(F(" ileSSID = ")); DPRINTLN(ileSSID);
  if(ileSSID == 0) return;
 
  for(int i=0; i < ileSSID; i++)
  {
    if(ssidTab[i] == ssid)
    {
       for(int y = i; y < ileSSID-1; y++)
       {  
          ssidTab[y] = ssidTab[y+1];
          pwdTab[y] = pwdTab[y+1];
       }
       ssidTab[ileSSID]="";
       pwdTab[ileSSID]="";
       ileSSID--;
    }
  }
  DPRINT(F("Koniec usuwania, ileSSID = ")); DPRINTLN(ileSSID);
}
void KZGwifi::clearAPList()
{
  DPRINTLN(F("KZGwifi::clearAPList"));
  while(ileSSID>0)
  {
    usunAP(ssidTab[0]);
  }
  DPRINT(F("Koniec clearAPList, ileSSID = ")); DPRINTLN(ileSSID);

}

void KZGwifi::begin(String confFile)
{
  DPRINTLN("Debug KZGwifi::begin start"); 
  _confFileStr=confFile;
  _kzgConfigFile.begin(_confFileStr);
  //WiFi.mode(WIFI_AP_STA);
  WiFi.mode(WIFI_STA);
  //wifiReconnect();
  setNTP("europe.pool.ntp.org",ntp_offset_h);
  DPRINTLN("Debug KZGwifi::begin end"); 
}
String KZGwifi::loadConfigFile()
{
  return _kzgConfigFile.loadConfigFile();
}
uint8_t KZGwifi::saveConfigFile(String confStr)
{
  return _kzgConfigFile.saveConfigFile(confStr);
}
String KZGwifi::getWifiStatusString()
{
  String wynik="";
  if(wifiConnected())
  {
    IPAddress ip=WiFi.localIP();
    wynik = "WiFi connected: "+WiFi.SSID()+", IP: "+ ip[0]+"."+ip[1]+"."+ip[2]+"."+ip[3];
  }else
  {
    wynik = "Wifi Connection Error. status= "+wifiMulti->run();
  }
  return wynik;
}
bool KZGwifi::getWifiStatusBuf(char *b) 
{ 
  if(wifiConnected())
  {
    IPAddress ip=WiFi.localIP();
    sprintf(b,"WiFi connected: %s ,%d.%d.%d.%d\n", WiFi.SSID().c_str(), ip[0],ip[1],ip[2],ip[3]);
    return true;
  }else
  {
    sprintf(b,"Wifi Connection Error. status= %d",wifiMulti->run());
    return false; 
  }
}

 void KZGwifi::wifiScanNetworks()
{
  DPRINT("scaning.. ");
  int n = WiFi.scanNetworks(false,false);
  if(n>=0) DPRINT(".. done ");
  if (n == 0)
    DPRINTLN(" no networks found");
  else
  {
    DPRINT(n);
    DPRINTLN(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
     DPRINT(i + 1);
     DPRINT(": ");
     DPRINT(WiFi.SSID(i));
     DPRINT(" (");
     DPRINT(WiFi.RSSI(i));
     DPRINT(")");
     DPRINTLN((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
    DPRINTLN("");
  }
  
}

String KZGwifi::getConfigStr()
{
  String wynik="{\"STA\":[";

  for(int i=0; i < ileSSID; i++)
  {
    wynik+="{\"ssid\":\""+ssidTab[i]+"\",\"pwd\":\""+pwdTab[i]+"\"}";
    if(i+1<ileSSID)wynik+=",";
  }
  wynik+="],\"AP\":{\"ssid\":\""+apName+"\",\"pwd\":\""+apPwd+"\"}}";

  return wynik;
}
void KZGwifi::parseConfigStr(String confStr)
{
  DPRINT(F("KZGwifi::parseConfigStr: ")); DPRINTLN(confStr);

  DynamicJsonBuffer jb;
  JsonObject& root = jb.parseObject(confStr);
  if (!root.success())
  {
     DPRINT(F("Blad parsowania confStr"));
     return;
  }
  JsonArray& staArr=root["STA"];
  // Loop through the element of the array
  clearAPList();
  for (JsonObject& js : staArr) 
  {
    dodajAP(js["ssid"], js["pwd"],true);  
  } 
  initAP(root["AP"]["ssid"].as<char*>(), root["AP"]["pwd"].as<char*>());
  DPRINT(F("Koniec parseConfigStr, ileSSID = ")); DPRINTLN(ileSSID);
}
void KZGwifi::initAP(String ssid,String pwd)
{
  DPRINT(F("KZGwifi::initAP "));DPRINTLN(ssid);
  apName = ssid;
  apPwd = pwd;

  /////////////////TODO sofAP
  //WiFi.softAPdisconnect();
  //delay(2000);

  //IPAddress apIP(192,168,1,1);               // The IP address of the access point
 // WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  //WiFi.softAP(apName.c_str(), apPwd.c_str());
  DPRINT(F("KZGwifi::initAP -koniec "));DPRINTLN(apName);
}
bool KZGwifi::wifiConnected()
{
 uint8_t status = WiFi.status();
 if (status == WL_CONNECTED)
 {
   if(lastConnectedStatus != WL_CONNECTED)  //wylapanie momentu zestawienia polaczenia
   {
     lastConnectedStatus = WL_CONNECTED;
     char buf[100];
     getWifiStatusBuf(buf);
     DPRINTLN(buf);
   }
   return true; 
 }
 else 
 {
   if(lastConnectedStatus == WL_CONNECTED)
   {
     DPRINTLN(F("KZGwifi->Disconnected!!"));
   }
   lastConnectedStatus = status;
   return false;
 }
}


void KZGwifi::loop()
{
 /*  loopMillis=millis();  
   if(loopMillis%5000==0)
   {
      if(!getWifiStatusString(bb))
      {
         Serial.println(bb);
         wifiScanNetworks();
         DPRINT("Free heap: ");DPRINTLN(ESP.getFreeHeap());
      }
           
  }*/
 
 if(!wifiConnected())
 {
    wifiReconnect();
 }else
 {   
    timeClient->update();
 } 
 delay(10); // czas dla nodeMCU na obsluge watkow wifi
}
