/* *******************************************************
** Przykład użycia biblioteki, 
** przy pierwszym starcie tworzy plik konfiguracyjny
** po każdym restarcie dodaje do pliku symbol *
** ******************************************************* */

#include <KZGwifi.h>

KZGwifi wifi;
String confFile="test.txt";

String defConfTxt="Test string => ";

void setup()
{
    Serial.begin(115200);
    wifi.begin();
    wifi.dodajAP("abc","cde");
    wifi.dodajAP("abc","123");
    wifi.dodajAP("test","aaa");
    wifi.dodajAP("UPC Wi-Free","");


}


void loop()
{
    wifi.loop();
    Serial.println(wifi.getConfigStr());
    delay(15000);
}
