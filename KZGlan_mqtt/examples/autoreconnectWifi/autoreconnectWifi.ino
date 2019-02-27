/* *******************************************************
** Przykład użycia biblioteki, 
** przy pierwszym starcie tworzy plik konfiguracyjny
** po każdym restarcie dodaje do pliku symbol *
** ******************************************************* */

#include <KZGwifi.h>

KZGwifi wifi;

void setup()
{
    Serial.begin(115200);
    wifi.begin();
    String conf=wifi.loadConfigFile();
    if(conf == "")  //brak pliku konfiguracyjnego
    {
        Serial.print("Przygotuj domyslny config");
        wifi.dodajAP("abc","cde");
        wifi.dodajAP("abc","123");
        wifi.dodajAP("test","aaa");
        wifi.dodajAP("UPC Wi-Free","");
        wifi.dodajAP("DOrangeFreeDom","KZagaw01_ruter_key");
        wifi.initAP("TestWifiAP","qwerty");
        //save config
        conf = wifi.getConfigStr();
        Serial.println("Zapis konfiguracji: "+wifi.saveConfigFile(conf));
    }else
    {
        Serial.print("Wczytywanie konfiguracji");
        wifi.parseConfigStr(conf);
    }
    
    Serial.println("Koniec Setup"); 
}

unsigned long m=0;
void loop()
{
    wifi.loop();
    if(millis()-m>15000)
    {
        Serial.print("## ");Serial.print(wifi.getWifiStatusString());Serial.println(" ##");
        Serial.print("#### ");Serial.print(wifi.getTimeString());Serial.println(" ####");
        m=millis();
    }
}
