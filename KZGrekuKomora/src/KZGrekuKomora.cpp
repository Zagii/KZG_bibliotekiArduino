#include "KZGrekuKomora.h"

KZGrekuKomora::KZGrekuKomora(uint8_t id, DeviceAddress* termAddr)
{
    _id=id;
    _termAddr=termAddr;
}
void KZGrekuKomora::begin(OneWire* oneWire)
{
  DPRINT(__func__);DPRINT(" komoraID=");DPRINTLN(komoraID);
	
    _oneWire=oneWire;
    _dsTermometr.setOneWire(_oneWire);
    _dsTermometr.begin();

  /*if(!czyBylTempBegin)
  {
    czyBylTempBegin=true;
    sensors.begin();  
  }*/
  //if (!sensors.getAddress(termometrAddr[_id], _id))
  if(!_dsTermometr.isConnected(*_termAddr))
  {
     DPRINT(__func__);DPRINT(" problem z termometrem id=");DPRINTLN(_id);
      
  }
  printAddress(*_termAddr,addressStr);
  _dsTermometr.setResolution(*_termAddr, TEMPERATURE_PRECISION);
  _tempMillis=millis();
}
void KZGrekuKomora::pomiarTemp()
{
  if(millis()-_tempMillis>5000)
  {
    _tempMillis=millis();
    _dsTermometr.requestTemperaturesByAddress(*_termAddr);
    _temp = _dsTermometr.getTempC(*_termAddr);
   
    DPRINT(__func__);DPRINT(" komoraID=");DPRINT(_id);DPRINT(" temp=");DPRINTLN(_temp);
  }

}
void KZGrekuKomora::pomiarCisnienia(){}
void KZGrekuKomora::pomiarWilgotnosci(){}

// function to print a device address
void KZGrekuKomora::printAddress(DeviceAddress deviceAddress,char * buf)
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