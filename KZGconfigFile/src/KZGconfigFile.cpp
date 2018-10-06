#include "KZGconfigFile.h"

void KZGconfigFile::begin(String nazwaPliku)
{
    _nazwaPliku = nazwaPliku;
    DPRINTLN(F("KZGconfigFile::begin montowanie FS..."));
    if (!SPIFFS.begin()) 
    {
        DPRINTLN(F("Blad montowania fileSystemu"));
        return;
    }
}

String KZGconfigFile::loadConfigFile()
{
    DPRINT(F("KZGconfigFile::loadConfigFile = "));DPRINTLN(_nazwaPliku);
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

uint8_t KZGconfigFile::saveConfigFile(String plikStr) 
{
    DPRINT(F(" KZGconfig::saveConfigFile: ")); DPRINT(plikStr); DPRINT(F(" / ")); DPRINTLN(_nazwaPliku);
    File configFile = SPIFFS.open(_nazwaPliku, "w");
    if (!configFile) 
    {
        DPRINT(F("Blad zapisu pliku "));DPRINTLN(_nazwaPliku);
        return KZGCONFFILE_SAVE_ERR;
    }
    configFile.println(plikStr);
    configFile.close();
    DPRINTLN(F("zapis [OK]"));
    return KZGCONFFILE_SAVE_OK;
}