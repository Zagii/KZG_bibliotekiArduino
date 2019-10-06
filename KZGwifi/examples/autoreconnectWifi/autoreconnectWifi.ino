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
    // ustawienie domyślnych AP
    Serial.print("Przygotuj domyslny config");
    wifi.dodajAP("abc","cde");
    wifi.dodajAP("DOrangeFreeDom","KZagaw01_ruter_key"); 
    wifi.initAP("TestWifiAP","qwerty");
    
    //wczytanie konfiguracji i ew zastapienie domyślnych ustawien
    wifi.importFromFile();  
    
    Serial.println("Koniec Setup"); 
}

unsigned long m=0;
void loop()
{
    wifi.loop();
    if(millis()-m>15000)
    {
        Serial.print("## ");Serial.print(wifi.getWifiStatusString());
        Serial.print(" #### ");Serial.print(wifi.getTimeString());Serial.println(" ####");
        m=millis();
    }
}
