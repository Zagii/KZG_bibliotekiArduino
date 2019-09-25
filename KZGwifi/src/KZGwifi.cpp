#include "KZGwifi.h"


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



void KZGwifi::begin()
{
  DPRINTLN("Debug KZGwifi::begin start"); 
  ///////////////// wifiManager start /////////////////
  pinMode(PIN_LED, OUTPUT);
  Serial.println("\n Starting");
  WiFi.printDiag(Serial); //Remove this line if you do not want to see WiFi password printed
  if (WiFi.SSID()==""){
    Serial.println("We haven't got any access point credentials, so get them now");   
    initialConfig = true;
  }
  if (drd->detectDoubleReset()) {
    Serial.println("Double Reset Detected");
    initialConfig = true;
  }
  if (initialConfig) {
    Serial.println("Starting configuration portal.");
    digitalWrite(PIN_LED, LOW); // turn the LED on by making the voltage LOW to tell us we are in configuration mode.
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    //sets timeout in seconds until configuration portal gets turned off.
    //If not specified device will remain in configuration mode until
    //switched off via webserver or device is restarted.
    //wifiManager.setConfigPortalTimeout(600);

    //it starts an access point 
    //and goes into a blocking loop awaiting configuration
    if (!wifiManager.startConfigPortal()) {
      Serial.println("Not connected to WiFi but continuing anyway.");
    } else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
    }
    digitalWrite(PIN_LED, HIGH); // Turn led off as we are not in configuration mode.
    ESP.reset(); // This is a bit crude. For some unknown reason webserver can only be started once per boot up 
    // so resetting the device allows to go back into config mode again when it reboots.
    delay(5000);
  }

  digitalWrite(PIN_LED, HIGH); // Turn led off as we are not in configuration mode.
  WiFi.mode(WIFI_STA); // Force to station mode because if device was switched off while in access point mode it will start up next time in access point mode.
  unsigned long startedAt = millis();
  Serial.print("After waiting ");
  int connRes = WiFi.waitForConnectResult();
  float waited = (millis()- startedAt);
  Serial.print(waited/1000);
  Serial.print(" secs in setup() connection result is ");
  Serial.println(connRes);
  if (WiFi.status()!=WL_CONNECTED){
    Serial.println("failed to connect, finishing setup anyway");
  } else{
    Serial.print("local ip: ");
    Serial.println(WiFi.localIP());
}
  //////////////wifi Manager koniec ///////////
  setNTP("europe.pool.ntp.org",ntp_offset_h);
  DPRINTLN("Debug KZGwifi::begin end"); 
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
    wynik = "Wifi Connection Error. status= "+WiFi.status();
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
    sprintf(b,"Wifi Connection Error. status= %d",WiFi.status());
    return false; 
  }
}

 
bool KZGwifi::wifiConnected()
{
 if() WiFi.status()==WL_CONNECTED)
 {
   if(lastConnectedStatus!=WL_CONNECTED)  //wylapanie momentu zestawienia polaczenia
   {
     lastConnectedStatus=WL_CONNECTED;
     char buf[100];
     getWifiStatusBuf(buf);
     DPRINTLN(buf);
   }
   return true; 
 }
 else 
 {
   if(lastConnectedStatus==WL_CONNECTED)
   {
     DPRINTLN(F("KZGwifi->Disconnected!!"));
   }
   lastConnectedStatus=WL_IDLE_STATUS;
   return false;
 }
}


void KZGwifi::loop()
{
  timeClient->update();   
  drd->loop();
  delay(10); // czas dla nodeMCU na obsluge watkow wifi
}
