#ifndef CCONFIGFILE_h
#define CCONFIGFILE_h
#include "Arduino.h"
#include "FS.h"

#define DEBUG   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

#define CCONFFILE_SAVE_OK 0
#define CCONFFILE_SAVE_ERR 1

class CConfigFile
{
    String _nazwaPliku;

    public:
      CConfigFile(){};
      void begin(String nazwaPliku);
      String loadConfigFile();
      uint8_t saveConfigFile(String plikStr);
};

#endif