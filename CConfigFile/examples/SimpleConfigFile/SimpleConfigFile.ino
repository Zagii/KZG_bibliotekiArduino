/* *******************************************************
** Przykład użycia biblioteki, 
** przy pierwszym starcie tworzy plik konfiguracyjny
** po każdym restarcie dodaje do pliku symbol *
** ******************************************************* */

#include <CConfigFile.h>

CConfigFile config;
String confFile="test.txt";

String defConfTxt="Test string => ";

void setup()
{
    Serial.begin(115200);
    config.begin(confFile);
    // odczytaj plik
    String loadedStr=config.loadConfigFile();
    Serial.print("Odczytano: ");
    Serial.println(loadedStr);
    if(loadedStr=="")
    {
        //pusty plik
        Serial.println("pusty plik, zapisuje def");
        config.saveConfigFile(defConfTxt);      
    }else
    {
        // przy każdym restarcie dodaje * do pliku konfiguracyjnego
        Serial.println(" dopisuje *");
        loadedStr+="*";
        config.saveConfigFile(loadedStr);
    }
}


void loop()
{
    Serial.println("Restart ESP8266 by dodać * do pliku");
    delay(15000);
}
