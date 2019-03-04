#ifndef KZGoutput_h
#define KZGoutput_h


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


class KZGoutput
{
    uint8_t _pin;        // hardware pin number. 
    String _name;        // human button name
    uint16_t _on;         // 255
    uint16_t _off;        // 0
    uint16_t _max;        // max/min value of _on/_off
    uint16_t _min;
    uint16_t _currentState=0;  // current PWM state during fading
    uint16_t _hardwareState=UNDEF; // state assigned in loop to hardware pin
    uint16_t _aimState=0;    // state when fading should be stopped
    bool _isFading=false;
    uint16_t _fadeSpeed;
    unsigned long _timerPWM=0;
    unsigned long _timeOfHardwareState=0; // przechowuje czas kiedy wyjscie fizyczne zmieniło stan
  
  public:
    KZGoutput(){};
    void begin(String name, uint8_t pin, uint16_t on, uint16_t off, uint16_t initState);
    void setOutput(uint16_t state);
    void setOutputON(){setOutput(_on);}
    void setOutputOFF(){setOutput(_off);}
    void setFadingSpeed(uint16_t aimState, unsigned long speed);
    void setFadingDuration(uint16_t aimState, unsigned long duration);
    bool loop();    // if state has changed
    uint16_t getState(){return _hardwareState;}
    String getJsonStatusStr();
    bool isFading(){return _isFading;}
    unsigned long getTimeOfHardwareState(){return _timeOfHardwareState;}
};
#endif
