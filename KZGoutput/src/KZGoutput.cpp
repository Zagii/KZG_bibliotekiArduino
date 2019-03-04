#include "KZGoutput.h"

    
void KZGoutput::begin(String name, uint8_t pin, uint16_t on, uint16_t off, uint16_t initState)
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
}

void KZGoutput::setOutput(uint16_t state)
{
  _currentState=state;
  _isFading=false;
}
    
/*******************************
 * aimState - docelowy stan PWM
 * speed - predkosc zmian PWM w krokach/s
 *****************************/
void KZGoutput::setFadingSpeed(uint16_t aimState, uint16_t speed)
{
  _fadeSpeed=speed;
  _aimState=aimState;
  _isFading=true;
  _timerPWM=millis();
}

/******************************
 * aimState - docelowy PWM
 * duration - czas trwania fadingu w ms
 * *************************/
void KZGoutput::setFadingDuration(uint16_t aimState, unsigned long duration)
{
    uint16_t speed= duration/T_PWM;
    setFadingSpeed(aimState,speed);
}


/*************************
 * return true if 
 * fading reach the aim (for analogWrite) 
 * or switched on/off (for digitalWrite)
 * 
 * *************************/
bool KZGoutput::loop()    // if state has changed
{
  if(isFading)
  {
    if(millis() - _timerPWM>T_PWM)
    {
      timerPWM=millis();
      if(_currentState>_aimState)
      {
        if(_currentState>_aimState+_fadeSpeed) 
        {
          _currentState=_aimState;
        }
        else
        {
          _currentState-=_fadeSpeed;
        }
      }else
      {
        if(_currentState+_fadeSpeed>_aimState)
        {
          _currentState=_aimState;
        } else
        {
          _currentState+=_fadeSpeed;
        } 
      }
      if(_currentState>_max)_currentState=_max;
      if(_currentState<_min)_currentState=_min;
    }
  }

  /////////////////////////// hardware state change //////////////////////
  if(_hardwareState!=_currentState)
  {
    _hardwareState=_currentState;
    _timeOfHardwareState=millis();
    if(_hardwareState==_on || _hardwareState == _off)
    {
      digitalWrite(_pin,_hardwareState);
      _isFading=false;
      return true;
    } else
    {
      analogWrite(_pin,_hardwareState);
      if(_aimState==_hardwareState)
      {
        _isFading=false;
        return true;
      }
    }
  }
  return false;
}

String KZGoutput::getJsonStatusStr()
{
  String w="{\"pin\":\""+String(_pin)+"\", \"name\":\""+_name+"\", \"value\":\""+String(_hardwareState)+"\"";
  w+=", \"isFading\":\""+isFading? String(1) : String(0) +"\", \"aimValue\":\""+String(_aimState)+"\"";
  return w+="}";
}