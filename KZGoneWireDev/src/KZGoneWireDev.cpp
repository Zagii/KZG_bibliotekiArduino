#include "KZGoneWireDev.h"

void KZGoneWireDev::begin(uint8_t x1w_pin,uint8_t ds18b20precision, unsigned long tempFreq)
{
  DPRINTLN("Debug KZGoneWireDev::begin start"); 
  _1w_pin=x1w_pin;
  _ds18b20precision=ds18b20precision;
  _tempFreq=tempFreq;
  _18b20ready=false;
  _termometerWaiting=false;
  
  _oneWire=new OneWire(_1w_pin);
  _sensors= new DallasTemperature(_oneWire);
 
  if(_sensors->millisToWaitForConversion(_ds18b20precision)<_tempFreq)
  {
    DPRINT("UWAGA: zabyt czeste probkowanie!! precyzja: ");DPRINT(_ds18b20precision);
    DPRINT(", min czas: ");DPRINT(_sensors->millisToWaitForConversion(_ds18b20precision));
    DPRINT(", oczekiwana czestotliwosc"); DPRINTLN(_tempFreq);
  }

  checkNewTermometers();
  DPRINTLN("Debug KZGoneWireDev::begin end"); 
}


void KZGoneWireDev::loop()
{

  ///////////////////// DS18b20 -start ///////////////////////
  //cyklicznie inicjuj pomiar temperatury
  if(millis()-_lastTempMeasure>_tempFreq)
  {
    _sensors->requestTemperatures();
    _termometerWaiting=true;
    _lastTempMeasure=millis();
  }

  // gdy minie czas na konwersje odczytaj temperatury
  if((millis()-_lastTempMeasure>_sensors->millisToWaitForConversion(_ds18b20precision)) && _termometerWaiting)
  {
    _termometerWaiting=false;

    for(int i=0;i<_numberOfDevices; i++)
    {
      _temperaturaC[i]= _sensors->getTempC(_tempDeviceAddress[i]);
      if( _temperaturaC[i] == DEVICE_DISCONNECTED_C) 
      {
        DPRINT("Err: problem z termometrem i=");DPRINT(i);DPRINT(", addr:");DPRINTLN(printAddressToStr(_tempDeviceAddress[i]));
       
      }     
    }
    _18b20ready=true;
  }

  ///////////////////// DS18b20 -end ///////////////////////
      
  
    //////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////// 
}


String KZGoneWireDev::printAddressToStr(DeviceAddress deviceAddress)
{
  String s="";
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) s+="0";
    s+=String(deviceAddress[i], HEX);
  }
  return s;
}

void KZGoneWireDev::checkNewTermometers()
{
  _sensors->begin();
  _sensors->setResolution(_ds18b20precision);
  _sensors->setWaitForConversion(false);
  _numberOfDevices = _sensors->getDeviceCount();
  if(_numberOfDevices>MAX_TERMOMETERS)_numberOfDevices=MAX_TERMOMETERS;
  
  // Loop through each device, print out address
  for(int i=0;i<_numberOfDevices; i++)
  {
    // Search the wire for address
    if(_sensors->getAddress(_tempDeviceAddress[i], i))
	  {
      DPRINT("Found device ");
      DPRINT(i, DEC);
      DPRINT(" with address: ");
      DPRINTLN(printAddressToStr(_tempDeviceAddress[i]));
      
    }else{
      DPRINT("Found ghost device at ");
      DPRINT(i, DEC);
      DPRINT(" but could not detect address. Check power and cabling");
      DPRINTLN();
      }
    }
  

}