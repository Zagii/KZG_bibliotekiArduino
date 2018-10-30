#ifndef KZGwiatrak_h
#define KZGwiatrak_h
#include "arduino.h"
#define KZG_TACHO_CZAS_SEK 1
#define KZG_TACHO_CZAS_MS (TACHO_CZAS_SEK*1000)

#define MAX_PWM 100

//#define DEBUG_KZGWIATRAK
#undef DEBUG_KZGWIATRAK   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG_KZGWIATRAK    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

#define INFO_KZGWIATRAK
#ifdef INFO_KZGWIATRAK    //Macros are usually in all capital letters.
  #define IPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define IPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define IPRINT(...)     //now defines a blank line
  #define IPRINTLN(...)   //now defines a blank line
#endif

class KZGwiatrak
{
	uint8_t _pin_pwm;
	uint8_t _pin_tacho;
	volatile unsigned long _tachoLicznik=0;
	unsigned long _tachoOstPredkosc=0;	//obr/min
	unsigned long _ms=0; //millisy w loop
	uint8_t _zadanaPredkoscProcent=0;

    public:
        KZGwiatrak(uint8_t pin_pwm,uint8_t pin_tacho)
        {
            _pin_pwm=pin_pwm;
            _pin_tacho=pin_tacho;
        }
        void begin();
        void obslugaTachoISR();
        void loop();
        void ustawPredkosc(uint8_t procent);
        uint8_t dajZadanaPredkoscProcent(){return _zadanaPredkoscProcent;}
        unsigned long dajOstPredkosc(){return _tachoOstPredkosc;}
        uint8_t dajISR() {return _pin_tacho;}
};

#endif