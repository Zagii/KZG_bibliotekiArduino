#include "KZGoutput.h"

    
void KZGoutput::begin(String name, uint8_t pin, unsigned long on, unsigned long off, unsigned long initState)
{
  _name=name;
  _pin=pin;
  pinMode(_pin,OUTPUT);
  _on=on;
  _off=off;
  if(_on>_off)
  {
    _max=_on;
    _min=_off;
  }else
  {
    _max=_off;
    _min=_on;
  }
  
  setOutput(initState);
  loop();
}

void KZGoutput::setOutput(unsigned long state)
{
  DPRINT("KZGoutput::setOutput = ");DPRINTLN(state);
  _currentState=state;
  _isFading=false;
}
    
/*******************************
 * aimState - docelowy stan PWM
 * speed - predkosc zmian PWM w krokach/s
 *****************************/
void KZGoutput::setFadingSpeed(unsigned long aimState, unsigned long speed)
{
  DPRINT("KZGoutput::setFadingSpeed = ");DPRINT(aimState);DPRINT(" speed=");DPRINTLN(speed);
  if(_currentState==aimState)return;
  _fadeSpeed=speed;
  _aimState=aimState;
  _isFading=true;
  _timerPWM=millis();
  _cyklPWM=T_PWM;
}

/******************************
 * aimState - docelowy PWM
 * duration - czas trwania fadingu w ms
 * *************************/
void KZGoutput::setFadingDuration(unsigned long aimState, unsigned long duration)
{
	if(_currentState==aimState)return;
    unsigned long speed,droga;
	if(aimState>_currentState)droga=aimState-_currentState; else droga = _currentState-aimState;
	_cyklPWM=duration/droga;
	_fadeSpeed=1;
	_aimState=aimState;
	_isFading=true;
	_timerPWM=millis();
	DPRINT("KZGoutput::setFadingDuration = ");DPRINT(aimState);DPRINT(", cykl=");DPRINT(_cyklPWM);DPRINT(" duration=");DPRINTLN(duration);
	DPRINT(" startMillis=");DPRINTLN(_timerPWM);

}


/*************************
 * return true if 
 * fading reach the aim (for analogWrite) 
 * or switched on/off (for digitalWrite)
 * 
 * *************************/
bool KZGoutput::loop()    // if state has changed
{
DPRINT("HardwareSate:");DPRINT(_hardwareState);DPRINT(" isFading:");DPRINT(_isFading);DPRINT(" currentState:");DPRINTLN(_currentState);
  if(_isFading)
  {
    if(millis() - _timerPWM>_cyklPWM)
    {
      _timerPWM=millis();
      if(_currentState>_aimState)
      {
        if(_currentState>_aimState+_fadeSpeed) 
        {
          _currentState-=_fadeSpeed;
		  DPRINT("KZGoutput::loop A");DPRINTLN(_currentState);
        }
        else
        {
		    _currentState=_aimState;
			DPRINT("KZGoutput::loop B");DPRINTLN(_currentState);
        }
      }else
      {
        if(_currentState+_fadeSpeed>_aimState)
        {
          _currentState=_aimState;
		  DPRINT("KZGoutput::loop C");DPRINTLN(_currentState);
        } else
        {
          _currentState+=_fadeSpeed;
		  DPRINT("KZGoutput::loop D");DPRINTLN(_currentState);
        } 
      }
      if(_currentState>_max)_currentState=_max;
      if(_currentState<_min)_currentState=_min;
    }
  }

  /////////////////////////// hardware state change //////////////////////
  if(_hardwareState!=_currentState)
  {
    DPRINT("KZGoutput::loop hs");DPRINTLN(_currentState);
    _hardwareState=_currentState;
    _timeOfHardwareState=millis();
    if(_hardwareState==_on || _hardwareState == _off)
    {
	  DPRINT("KZGoutput::loop digitalChange");DPRINTLN(_hardwareState);
      digitalWrite(_pin,_hardwareState);
	  DPRINT("d stopMillis=");DPRINTLN(millis());
      _isFading=false;
      return true;
    } else
    {
	  DPRINT("KZGoutput::loop analogChange");DPRINTLN(_hardwareState);
      analogWrite(_pin,_hardwareState);
      if(_aimState==_hardwareState)
      {
	  	 DPRINT("KZGoutput::loop stopFading");DPRINTLN(_hardwareState);
		 DPRINT("a stopMillis=");DPRINTLN(millis());
        _isFading=false;
        return true;
      }
    }
  }else 
  {
	if(_isFading)
	{	
		//_isFading=false;
		//return true;
	}
  }
  return false;
}

String KZGoutput::getJsonStatusStr()
{
  String f="0";
  if(isFading())f="1";
  String w="{\"pin\":\""+String(_pin)+"\", \"name\":\""+String(_name)+"\", \"value\":\""+String(_hardwareState)+"\"";
  w+=", \"isFading\":\""+f +"\", \"aimValue\":\""+String(_aimState)+"\"";
  return w+="}";
}