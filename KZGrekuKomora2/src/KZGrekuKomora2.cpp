#include "KZGrekuKomora2.h"

KZGrekuKomora2::KZGrekuKomora2(uint8_t id, uint8_t* termAddr)
{
    _id=id;
    _termAddr=termAddr;
	printAddress(_termAddr,addressStr);
	printAddressToStr(termAddr);
}
void KZGrekuKomora2::begin(KZGoneWireDev* oneWire)
{
  DPRINT(__func__);DPRINT(" komoraID=");DPRINTLN(_id);
	DPRINT("-- adres DS:");DPRINT(_addresString);DPRINT("-");DPRINTLN(addressStr);
    _oneWire=oneWire;
  
  DPRINT("Koniec KZGrekuKomora2::begin");
  
}
void KZGrekuKomora2::pomiarTemp()
{
  if(millis()-_tempMillis>5000)
  {
 
    _tempMillis=millis();

  }

} 
void KZGrekuKomora2::pomiarCisnienia(){}
void KZGrekuKomora2::pomiarWilgotnosci(){}

// function to print a device address
void KZGrekuKomora2::printAddress(uint8_t* deviceAddress,char * buf)
{
  strcpy(buf,"");
  char tmp[4];
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) strcat(buf,"0");

    sprintf(tmp,"%X",deviceAddress[i]);
    strcat(buf,tmp);
  }
}
void KZGrekuKomora2::printAddressToStr(uint8_t* deviceAddress)
{
  _addresString="";
  //strcpy(buf,"");
  char tmp[4];
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) _addresString+="0";

    sprintf(tmp,"%X",deviceAddress[i]);
    _addresString+=tmp;
  }
}
String KZGrekuKomora2::getStatusString()
{
  String r="{ \"komoraid\":\""+String(_id)+"\", \"ds_addr\":\""+String(addressStr)+"\",";
  r+="\"temp\":\""+String(_temp)+"\"}";
  return r;
}

void KZGrekuKomora2::loop()
{
	pomiarTemp();
	pomiarCisnienia();
	pomiarWilgotnosci();
	
}