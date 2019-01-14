#include "KZGrekuKomora.h"

KZGrekuKomora::KZGrekuKomora(uint8_t id, uint8_t* termAddr)
{
    _id=id;
    _termAddr=termAddr;
	printAddress(_termAddr,addressStr);
	printAddressToStr(termAddr);
}
void KZGrekuKomora::begin(OneWire* oneWire)
{
  DPRINT(__func__);DPRINT(" komoraID=");DPRINTLN(_id);
	DPRINT("-- adres DS:");DPRINT(_addresString);DPRINT("-");DPRINTLN(addressStr);
    _oneWire=oneWire;
	_dsTermometr=new DallasTemperature(_oneWire);
	_dsTermometr->begin();
    // locate devices on the bus
  DPRINT("Szukam urzadzen 1-wire...");
  DPRINT("Znaleziono ");
  DPRINT(_dsTermometr->getDeviceCount(), DEC);
  DPRINTLN(" urzadzen.");

  // report parasite power requirements
  DPRINT("Parasite power is: "); 
  if (_dsTermometr->isParasitePowerMode()) DPRINTLN("ON");
  else DPRINTLN("OFF");

  /*if(!czyBylTempBegin)
  {
    czyBylTempBegin=true;
    sensors.begin();  
  }*/
  //if (!sensors.getAddress(termometrAddr[_id], _id))
  _tempMillis=millis();
  if(!_dsTermometr->isConnected(_termAddr))
  {
     DPRINT(__func__);DPRINT(" Komora:begin - problem z termometrem id=");DPRINTLN(_id);
     _connected=false; 
  }else
  {
	DPRINT(__func__);DPRINT(" znaleziono termometr id=");DPRINT(_id);DPRINT(" adres: ");DPRINTLN(addressStr);
	_dsTermometr->setResolution(_termAddr, TEMPERATURE_PRECISION);
	_connected=true;
  }
  
  DPRINT("Koniec KZGrekuKomora::begin");
  
}
void KZGrekuKomora::pomiarTemp()
{
  if(millis()-_tempMillis>5000)
  {
  DPRINTLN(" aaaaaaaaaaaaaaaaaa");
    _tempMillis=millis();
	if(_dsTermometr->isConnected(_termAddr))
	{
	DPRINTLN(" bbbbbbbbbbbbbbbbbbbbbb");
		if(!_connected) // pilnowanie by po podlaczeniu poprawnie zainicjowac termometr
		{
		DPRINTLN(" ccccccccccccccccccccc");
			_dsTermometr->setResolution(_termAddr, TEMPERATURE_PRECISION);
			_connected=true;
		}DPRINTLN(" ddddddddddddddddddd");
		_dsTermometr->requestTemperaturesByAddress(_termAddr);
		DPRINT(" eeeeeeeeeeeeeeeeeeeeee");
		_temp = _dsTermometr->getTempC(_termAddr);
   
		DPRINT(__func__);DPRINT(" komoraID=");DPRINT(_id);DPRINT(" temp=");DPRINTLN(_temp);
	}else
	{
		DPRINT(__func__);DPRINT(" - problem z termometrem id=");DPRINT(_id);DPRINT(" adres: ");DPRINTLN(addressStr);
		_connected=false;
	}
  }

}
void KZGrekuKomora::pomiarCisnienia(){}
void KZGrekuKomora::pomiarWilgotnosci(){}

// function to print a device address
void KZGrekuKomora::printAddress(uint8_t* deviceAddress,char * buf)
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
void KZGrekuKomora::printAddressToStr(uint8_t* deviceAddress)
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
String KZGrekuKomora::getStatusString()
{
  String r="{ \"komoraid\":\""+String(_id)+"\", \"ds_addr\":\""+String(addressStr)+"\",";
  r+="\"temp\":\""+String(_temp)+"\"}";
  return r;
}

void KZGrekuKomora::loop()
{
	pomiarTemp();
	pomiarCisnienia();
	pomiarWilgotnosci();
	
}