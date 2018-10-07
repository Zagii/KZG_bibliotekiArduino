#include "KZGwifi.h"

void KZGwifi::wifiReconnect()
{
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
/* usuwa wszystkie AP o zadanym SSID */
void KZGwifi::usunAP(String ssid)
{
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
}
void KZGwifi::dodajAP(String ssid,String pwd)
{
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
  ssidTab[0]=ssid;
  pwdTab[0]=pwd;
  ileSSID++;
  wifiReconnect();  
}
void KZGwifi::begin()
{
  DPRINTLN("Debug KZGwifi::begin start"); 
  WiFi.mode(WIFI_STA);
  wifiReconnect();
  setNTP("europe.pool.ntp.org",ntp_offset_h);
  DPRINTLN("Debug KZGwifi::begin end"); 
}

bool KZGwifi::getWifiStatusString(char *b) 
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
  String wynik="{\"STA\":{[";

  for(int i=0; i < ileSSID; i++)
  {
    wynik+="{\"ssid\":\""+ssidTab[i]+"\",\"pwd\":\""+pwdTab[i]+"\"}";
    if(i+1<ileSSID)wynik+=",";
  }
  wynik+="]},\"AP\":{\"ssid\":\""+String("espTestWifi")+"\",\"pwd\":\""+String("espPass")+"\"}}";

  return wynik;

}

bool KZGwifi::wifiConnected()
{
 if (wifiMulti->run()==WL_CONNECTED)
 {
   if(lastConnectedStatus!=WL_CONNECTED)  //wylapanie momentu zestawienia polaczenia
   {
     lastConnectedStatus=WL_CONNECTED;
     char buf[100];
     getWifiStatusString(buf);
     DPRINTLN(buf);
   }
   return true; 
 }
 else 
 {
   lastConnectedStatus=WL_IDLE_STATUS;
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
