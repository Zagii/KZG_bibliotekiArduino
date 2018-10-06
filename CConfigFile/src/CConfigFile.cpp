#include "CConfigFile.h"

void CConfigFile::begin(String nazwaPliku)
{
    _nazwaPliku = nazwaPliku;
    DPRINTLN(F("Montowanie FS..."));
    if (!SPIFFS.begin()) 
    {
        DPRINTLN(F("Blad montowania fileSystemu"));
        return;
    }
}

String CConfigFile::loadConfigFile()
{
    DPRINT(F("CConfigFile::LoadConfigFile = "));DPRINTLN(_nazwaPliku);
    String s="";
    File configFile = SPIFFS.open(_nazwaPliku, "r");
    if (!configFile)
    {
        DPRINT(F("Blad odczytu pliku: "));DPRINTLN(_nazwaPliku);
        return s;
    }
    s=configFile.readString();
    configFile.close();
    DPRINTLN(F("## zawartosc pliku ##"));
    DPRINTLN(s.c_str());
    DPRINTLN(F("## koniec pliku ##"));
    return s;  
}

uint8_t CConfigFile::saveConfigFile(String plikStr) 
{
    DPRINT(F(" CConfig::saveConfigFile: ")); DPRINT(plikStr); DPRINT(F(" / ")); DPRINTLN(_nazwaPliku);
    File configFile = SPIFFS.open(_nazwaPliku, "w");
    if (!configFile) 
    {
        DPRINT(F("Blad zapisu pliku "));DPRINTLN(_nazwaPliku);
        return CCONFFILE_SAVE_ERR;
    }
    configFile.println(plikStr);
    configFile.close();
    DPRINTLN(F("zapis [OK]"));
    return CCONFFILE_SAVE_OK;
}