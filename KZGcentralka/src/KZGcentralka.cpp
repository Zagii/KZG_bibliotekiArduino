#include "KZGcentralka.h"

void KZGcentralka::begin(String name,byte [] mac, IPAddress mqttHostIP, String mqttHost,String mqttUser,String mqttPwd, uint16_t mqttPort,uint8_t 1w_pin,uint8_t ds18b20precision, unsigned long tempFreq)
{
  DPRINTLN("Debug KZGcentralka::begin start"); 
  String _name=name;
  
  _ethMqtt.begin(name,mac,mqttHost,mqttUser,mqttPwd,mqttPort,[this] (String topic, String msg) { this->callback(String topic, String msg); }));
  _input_num=0;
  _output_num=0;

  _1w_pin=1w_pin;
  _ds18b20precision=ds18b20precision;
  _tempFreq=tempFreq;
  
  _oneWire(_1w_pin);
  _sensors(&_oneWire);
 
  checkNewTermometers();

  DPRINTLN("Debug KZGcentralka::begin end"); 
}
String KZGcentralka::printAddressToStr(DeviceAddress deviceAddress)
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
void KZGcentralka::checkNewTermometers()
{
  _sensors.begin();
  _sensors.setResolution(_ds18b20precision);
  _sensors.setWaitForConversion(false);
  _numberOfDevices = _sensors.getDeviceCount();
  if(_numberOfDevices>MAX_TERMOMETERS)_numberOfDevices=MAX_TERMOMETERS;
  
  // Loop through each device, print out address
  for(int i=0;i<_numberOfDevices; i++)
  {
    // Search the wire for address
    if(_sensors.getAddress(_tempDeviceAddress[i], i))
	  {
      DPRINT("Found device ");
      DPRINT(i, DEC);
      DPRINT(" with address: ");
      DPRINTLN(printAddressToStr(tempDeviceAddress));
      
    }else{
      DPRINT("Found ghost device at ");
      DPRINT(i, DEC);
      DPRINT(" but could not detect address. Check power and cabling");
      DPRINTLN();
      }
    }
  

}
void KZGcentralka::addInput(uint8_t pin,String nazwa, uint8_t initState, bool activeLow)
{
  _inputs[_input_num].begin(pin,nazwa,initState,activeLow);
  _input_num++;
}
void KZGcentralka::addOutput(String name, uint8_t pin, uint16_t on, uint16_t off, uint16_t initState)
{
  _outputs[_output_num].begin(name,pin,on,off,initState);
  _output_num++;
}

void KZGcentralka::loop()
{
  if(millis()-_lastTempMeasure>_tempFreq)
  {
    sensors.requestTemperatures();
    _termometerWaiting=true;
    _lastTempMeasure=millis();
  }

  if(millis()-_lastTempMeasure>_sensor.millisToWaitForConversion(_ds18b20precision) && _termometerWaiting)
  {
    _termometerWaiting=false;

    for(int i=0;i<_numberOfDevices; i++)
    {
      float tempC= _sensors.getTempC(_tempDeviceAddress[i]);
      if(tempC == DEVICE_DISCONNECTED_C) 
      {
        DPRINTLN("Error: Could not read temperature data");
        return;
      }
      String s="{\"name\":\""+_name+"\", \"termometrAddr\": \""+printAddressToStr(_tempDeviceAddress[i])+"\"";
      s+=", \"value\": \""+String(tempC)+"\"}";
      _ethMqtt.publishPrefix("tepmeratura",s);
    }

  }

  
      
  
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
  for(uint8_t i=0;i<_input_num;i++)
  {
    if(_inputs[i].loop())
    {
      String s=_ipnuts[i].getStatusString();
     DPRINT("Btn event, new status: ");
      DPRINT(s);
      _ethMqtt.publishPrefix(String("input"),s);
    }
  }
  for(uint8_t j=0;j<_output_num;j++)
  {
    if(_outputs[j].loop())
    {
        String so= _outputs[j].getJsonStatusStr();
       DPRINT("@@@ analog output fading finished: ");
       DPRINTLN(so);
    }
    if(_outputs[j].isFading())
    {
        if(millis()%300==0)
        {
            String so= _outputs[j].getJsonStatusStr();
            DPRINT("@@@@@@@@ analog output is fading: ");
            DPRINTLN(so);
        }
    }
  }

}

void KZGcentralka::callback(String topic, String msg)
{
    DPRINT("### mqttCallback -> topic: ");
    DPRINT(topic);
    DPRINT(", msg: ");
    DPRINTLN(msg);

    if(topic.indexOf("output")>0)
    {
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, msg);
      if (error)return;
      const char* name = doc["name"];
      String nameStr=String(name);

      uint8_t id=255;
      //////// szukanie odpowiedniego wyjscia
      for(uint8_t i=0;i<_output_num;i++)
      {
        if(_outputs[i].getName()==nameStr)
        {
          id=i;
          break;
        }
      }
      if(id==255){
        DPRINT("!!!! unknown name: ");
        DPRINTLN(nameStr);
        return;
        }
      uint16_t value = doc["value"];
      if(doc.containsKey("duration"))
      {
        uint16_t duration =doc["duration"];
        _outputs[id].setFadingDuration(value,duration);
      } else
      if(doc.containsKey("speed"))
      {
        uint16_t speed=doc["speed"];
        _outputs[id].setFadingSpeed(value,duration);
      } else
      {
        _outputs[id].setOutput(value);
      }
      

    }
}