/* *******************************************************
** Przykład użycia biblioteki, 
 *
** ******************************************************* */

#include <KZGoutput.h>

KZGoutput o_digital;
KZGoutput o_analog;


void setup()
{
    Serial.begin(115200);
    randomSeed(analogRead(0));
    o_digital.begin("outDigital",5,255,0,0);
    o_digital.begin("outAnalog",6,255,0,0);
    Serial.println("Koniec Setup"); 
}

unsigned long md=0;

long rand=0;
void loop()
{
    /////////////////////// digital pin output /////////////////////////
    if(o_digital.loop())
    {
        Serial.print("### digital output change: ");
        Serial.println(o_digital.getJsonStatusStr());
    }
    if(millis()-md>rand)
    {
        md=millis();
        rand=random(1000,10000);
        if(o_digital.getState()>0) o_digital.setOutputOFF(); else o_digital.setOutputON();
    }
    ////////////////////////////////////////////////////////////////////
    
    ////////////////////// analog output //////////////////////////////////
    if(o_analog.loop())
    {
        Serial.print("@@@ analog output fading finished: ");
        Serial.println(o_analog.getJsonStatusStr());
    }
    if(o_analog.isFading())
    {
        if(millis()%300==0)
        {
            Serial.print("@@@@@@@@ analog output is fading: ");
            Serial.println(o_analog.getJsonStatusStr());
        }
    }else
    {
        // new random fading
        long randValue = random(0, 255);
        long randDur =random(0,10000);
        String s="@@@@@ new random fading: value= "+String(randValue)+", duration= "+String(randDur);
        Serial.println(s);
        o_analog.setFadingDuration(randValue,randDur);
    }
    

}
