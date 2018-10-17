#ifndef KZGWIFI_h
#define KZGWIFI_h

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <KZGconfigFile.h>

//#include "Defy.h"

/*
typedef enum {
    WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
    WL_IDLE_STATUS      = 0,
    WL_NO_SSID_AVAIL    = 1,
    WL_SCAN_COMPLETED   = 2,
    WL_CONNECTED        = 3,
    WL_CONNECT_FAILED   = 4,
    WL_CONNECTION_LOST  = 5,
    WL_DISCONNECTED     = 6
} wl_status_t;

*/


#undef DEBUG_WIFI   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG_WIFI    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

#define INFO_WIFI
#ifdef INFO_WIFI    //Macros are usually in all capital letters.
  #define IPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define IPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define IPRINT(...)     //now defines a blank line
  #define IPRINTLN(...)   //now defines a blank line
#endif

#define MAX_SSID 8
#define CO_ILE_RECONNECT 5000 //czas odstępu reconnectów w ms
#define ILE_PROB_RECONNECT 3  // ile razy probowac reconnect zanim powinna nastapic zmiana ssid


class KZGwifi
{
String _confFileStr="/KZGwifi.json";
KZGconfigFile _kzgConfigFile;
//// wifi udp  ntp
WiFiUDP ntpUDP;
// By default 'time.nist.gov' is used with 60 seconds update interval and
// no offset
NTPClient *timeClient;
// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

String ntp_server;
unsigned long ntp_offset_h=2;

String wifi_tryb;
  
//unsigned long loopMillis=0;
  
//ESP8266WiFiMulti *wifiMulti;
//WiFiClient espClient;

String ssidTab[MAX_SSID];
String pwdTab[MAX_SSID];
uint8_t ileSSID = 0;                              // liczba zapisanych ssid
uint8_t idSSID = 0;                               // ostatnio użyty ssid
uint8_t nrProbyReconnect = 0;                     // licznik prób reconnectu

//WiFiClient clientAP;
String apName="apName";
String apPwd="apPwd";

uint8_t lastConnectedStatus;

unsigned long probaReconnectMS = 0; // zmienna przechowuje czas ostatniego reconnecta
  
public:
  KZGwifi(){};
  void begin(String conffile="/KZGwifi.json");
  void loop();
  String loadConfigFile();
  bool wifiConnected();
  String getWifiStatusString();
  void initAP(String ssid,String pwd);
  bool getWifiStatusBuf(char *b);
  uint8_t saveConfigFile(String confStr);
  bool importFromFile();
  
  //int getConStat(){return conStat;};
  String getTimeString(){return timeClient->getFormattedTime();};
  unsigned long getEpochTime(){return timeClient->getEpochTime();};
  void wifiScanNetworks();
  void wifiReconnect(bool force);
  void setNTP(String host,unsigned long offset_h);
  String getNTPjsonStr(){ return  String("{\"host\":\""+ntp_server+"\",\"offset\":"+String(ntp_offset_h)+"}");};
 // String getWifijsonStr(){ return String("{\"ssid\":\""+wifi_ssid+"\",\"pwd\":\""+wifi_pwd+"\",\"ip\":\""+wifi_ip+"\",\"tryb\":\""+wifi_tryb+"\"}");};
  void dodajAP(String ssid,String pwd, bool dodajNaKoniec=true);
  void dodajAP(String jsonString);  
  void usunAP(String ssid);
  String getConfigStr();
  void parseConfigStr(String confStr);
  void clearAPList();
};
#endif
