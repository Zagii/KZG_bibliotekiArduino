
#include "KZGwiatrak.h"

void KZGwiatrak::begin()
{   
	pinMode(_pin_pwm, OUTPUT);
	analogWrite(_pin_pwm,0);
    analogWriteRange(100);
    pinMode(_pin_tacho, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(_pin_tacho),(void*)obslugaTachoISR, RISING );
    //attachInterrupt(digitalPinToInterrupt(_pin_tacho), isrIN, RISING );
    //attachInterrupt(digitalPinToInterrupt( _pin_tacho), isrOUT, RISING );
}

 void KZGwiatrak::obslugaTachoISR()
{
	_tachoLicznik++;
 //   DPRINT(__func__);DPRINT(" _pin_tacho ");DPRINT(_pin_tacho);DPRINT(" _tachoLicznik ");DPRINTLN(_tachoLicznik);
}

void KZGwiatrak::loop()
{
	if(millis()-_ms>=KZG_TACHO_CZAS_MS)
	{
		_ms=millis();
		_tachoOstPredkosc=(_tachoLicznik/KZG_TACHO_CZAS_SEK)*60;
		_tachoLicznik=0;
        DPRINT(__func__);DPRINT(" _pin_tacho ");DPRINT(_pin_tacho);DPRINT(" _tachoOstPredkosc ");DPRINTLN(_tachoOstPredkosc);
	}
}
String KZGwiatrak::getStatusString()
{
  String r="{ \"wiatrakid\":\""+String(_id)+"\", \"setProcent\":\""+dajZadanaPredkoscProcent()+"\",";
  r+="\"rpm\":\""+dajOstPredkosc()+"\"}";
  return r;
}
void KZGwiatrak::ustawPredkosc(uint8_t procent)
{
  if(procent==_zadanaPredkoscProcent)return;
	//uint16_t pwm=map(procent, 0, 100, 0,1023);
	//if(pwm>MAX_PWM)pwm=MAX_PWM;
	if(procent>MAX_PWM)procent=MAX_PWM;
  
	analogWrite(_pin_pwm,procent);
	_zadanaPredkoscProcent=procent;
    DPRINT("KZGwiatrak - Zadana predkosc wiatrak ");DPRINT(_pin_pwm);DPRINT(" ");DPRINTLN(procent);
}