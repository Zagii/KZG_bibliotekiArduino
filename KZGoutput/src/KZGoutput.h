﻿#ifndef KZGoutput_h
#define KZGoutput_h

#include "Arduino.h"
//#define DEBUG_KZG_OUTPUT   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG_KZG_OUTPUT    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

#define UNDEF 99999
//#define WYJSCIE_NORMALNE 0
//#define WYJSCIE_MCP 1

#define T_PWM 50
#define KZGoutput_autoOFFduration 3000

#include "FaBoPWM_PCA9685.h"

class KZGoutput
{
    uint8_t _pin;        // hardware pin number. 
    String _name;        // human button name
    unsigned long _on;         // 255
    unsigned long _off;        // 0
    unsigned long _max;        // max/min value of _on/_off
    unsigned long _min;
    unsigned long _currentState=0;  // current PWM state during fading
    unsigned long _hardwareState=UNDEF; // state assigned in loop to hardware pin
    unsigned long _aimState=0;    // state when fading should be stopped
    bool _isFading=false;
    unsigned long _fadeSpeed;
    unsigned long _timerPWM=0;
    unsigned long _timeOfHardwareState=0; // przechowuje czas kiedy wyjscie fizyczne zmieniło stan
	unsigned long _cyklPWM; // cykl co ile ma byc zmiana stanu w trakcie fadowania
	
	bool _usePCA9685=false;
	FaBoPWM _faboPWM;
	
	bool _isWaitingForChange=false; // is output waiting for automated change state
	unsigned long _waitingForChangeDuration=0; //millis time when change should happened
	unsigned long _waitingForChangeStartTime=0; //millis of first setting value
	unsigned long _futureState;		// state of future change, turn off or setup def value
	bool _futureChangeFading=false;
	
	void prepareAutoChangeState(unsigned long futureState, unsigned long timeToChangeState,bool fading);
  public:
    KZGoutput(){};
    void begin(String name, uint8_t pin, unsigned long on, unsigned long off, unsigned long initState, bool usePCA9685);
    void setOutput(unsigned long state);
    void setOutputON(){setOutput(_on);}
    void setOutputOFF(){setOutput(_off);}
    void setFadingSpeed(unsigned long aimState, unsigned long speed);
    void setFadingDuration(unsigned long aimState, unsigned long duration);
    bool loop();    // if state has changed
    unsigned long getState(){return _hardwareState;}
    String getJsonStatusStr();
    bool isFading(){return _isFading;}
    unsigned long getTimeOfHardwareState(){return _timeOfHardwareState;}
    String getName(){return _name;}
	void setOutputThenChange(unsigned long state, unsigned long futureState, unsigned long timeToChangeState);
	void setFadingSpeedThenChange(unsigned long aimState, unsigned long speed,unsigned long futureState, unsigned long timeToChangeState);
	void setFadingDurationThenChange(unsigned long aimState, unsigned long duration, unsigned long futureState, unsigned long timeToChangeState);
};
#endif
