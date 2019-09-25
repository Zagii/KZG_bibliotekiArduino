#ifndef KZGWIFI_h
#define KZGWIFI_h

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager

#include <DoubleResetDetector.h> //https://github.com/datacute/DoubleResetDetector
#include <KZGconfigFile.h>
#include <NTPClient.h>

// Number of seconds after reset during which a 
// subseqent reset will be considered a double reset.
#define DRD_TIMEOUT 10

// RTC Memory Address for the DoubleResetDetector to use
#define DRD_ADDRESS 0

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


//#define DEBUG_WIFI   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG_WIFI    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

#define MAX_SSID 8



class KZGwifi
{
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


DoubleResetDetector drd;
// Onboard LED I/O pin on NodeMCU board
const int PIN_LED = 2; // D4 on NodeMCU and WeMos. Controls the onboard LED.

// Indicates whether ESP has WiFi credentials saved from previous session, or double reset detected
bool initialConfig = false;
  
unsigned long lastWIFIReconnectAttempt = 0;
  
unsigned long loopMillis=0;
  
uint8_t lastConnectedStatus;
  
public:
  KZGwifi(){ drd= new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);};
  void setNTP(String host,unsigned long offset_h);
  void begin();
  String getWifiStatusString();
  bool getWifiStatusBuf(char *b);
  bool wifiConnected();
 
  void loop();
  String getTimeString(){return timeClient->getFormattedTime();};
  unsigned long getEpochTime(){return timeClient->getEpochTime();};
  
  String getNTPjsonStr(){ return  String("{\"host\":\""+ntp_server+"\",\"offset\":"+String(ntp_offset_h)+"}");};
 // String getWifijsonStr(){ return String("{\"ssid\":\""+wifi_ssid+"\",\"pwd\":\""+wifi_pwd+"\",\"ip\":\""+wifi_ip+"\",\"tryb\":\""+wifi_tryb+"\"}");};
  
};
#endif
