#ifndef KZGCONFIGFILE_h
#define KZGCONFIGFILE_h
#include "Arduino.h"
#include "FS.h"

#define DEBUG_CONFIGFILE   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG_CONFIGFILE    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

#define KZGCONFFILE_SAVE_OK 0
#define KZGCONFFILE_SAVE_ERR 1

class KZGconfigFile
{
    String _nazwaPliku;

    public:
      KZGconfigFile(){};
      void begin(String nazwaPliku);
      String loadConfigFile();
      uint8_t saveConfigFile(String plikStr);
};

#endif