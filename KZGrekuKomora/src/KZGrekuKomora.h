#ifndef KZGrekuKomora_h
#define KZGrekuKomora_h
#include "arduino.h"


#include <OneWire.h>
#include <DallasTemperature.h>

//#define PIN_ONEWIRE D7
#define TEMPERATURE_PRECISION 9//12//9

//static OneWire oneWire(PIN_ONEWIRE);  //statyczne?
//static DallasTemperature sensors(&oneWire); //statyczne?
static bool czyBylTempBegin=false;

/*static DeviceAddress termometrAddr[]={  { 0x28, 0x52, 0x96, 0x23, 0x06, 0x00, 0x00, 0x5C },
                                        { 0x28, 0x04, 0x1E, 0x22, 0x06, 0x00, 0x00, 0xBA },
                                        { 0x28, 0xC0, 0x24, 0x23, 0x06, 0x00, 0x00, 0x4F },
                                        { 0x28, 0x67, 0xD4, 0x22, 0x06, 0x00, 0x00, 0xC7 }};
*/

/*
 * uint8_t pin23[][8] = {
  {
Device Address: 28C024230600004F Temp C: 27.50 Temp F: 81.50
Device Address: 28041E22060000BA Temp C: 26.50 Temp F: 79.70
Device Address: 285296230600005C Temp C: 25.50 Temp F: 77.90
Device Address: 28BEDA21060000C7 Temp C: 25.50 Temp F: 77.90
 */

#define DEBUG_KZGKOMORA
//#undef DEBUG_KZGKOMORA   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG_KZGKOMORA    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

#define INFO_KZGKOMORA
#ifdef INFO_KZGKOMORA    //Macros are usually in all capital letters.
  #define IPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define IPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define IPRINT(...)     //now defines a blank line
  #define IPRINTLN(...)   //now defines a blank line
#endif

class KZGrekuKomora
{
    unsigned long _tempMillis;  // licznik przechowujący millis dla termometru
    uint8_t _id;                // unikalny id komory
    double _temp=0;             // ostatnio odczytana temperatura
    double _cisnienie=0;        // nic nie robi  
    uint8_t _wilgotnosc=0;      // nic nie robi
    //uint8_t co2=0;
      
    OneWire* _oneWire;
    uint8_t* _termAddr;   // adres termometru na 1w magistrali
    DallasTemperature *_dsTermometr; //obiekt terometru
    void pomiarTemp();
    void pomiarCisnienia();
    void pomiarWilgotnosci();
    //void pomiarCO2();
    char addressStr[20];
	String _addresString;
  //  unsigned long _ms=0; //czas ms
    bool _connected=false;
    
	public:
    KZGrekuKomora(uint8_t id, uint8_t* termAddr);
    void printAddress(uint8_t* deviceAddress,char* buf);
	void printAddressToStr(uint8_t* deviceAddress);
    char *getTempAddress(){return addressStr;}
    void begin(OneWire* oneWire);
    void loop();
    double dajTemp(){return _temp;}
    double dajCisnienie(){ return _cisnienie;}
  	uint8_t dajWilgotnosc(){return _wilgotnosc;}
    String getStatusString();
};



#endif