/* *******************************************************
** Przykład użycia biblioteki, 
 *
** ******************************************************* */

#include <KZGoutput.h>

KZGoutput o_digital;
KZGoutput o_analog;

KZGoutput oPwm[15];


void setup()
{
    Serial.begin(115200);
    randomSeed(analogRead(0));
    //o_digital.begin("outDigital",0,0xFFF,0,0,true);
    //o_analog.begin("outAnalog",1,0xFFF,0,0,true);
    for(int i=0;i<15;i++)
    {
      String n="pwm"+String(i+1);
      oPwm[i].begin(n,i+1,0xFFF,0,0,true);
    }
   
    Serial.println("Koniec Setup"); 
}

unsigned long md=0;

long rand1=0;
void loop()
{
   for(int i=0;i<15;i++)
    {
      oPwm[i].loop();
      if(!oPwm[i].isFading())
      {
         long randValue = random(0, 5)*50;
          long randDur =random(1000,8000);
          String s="@@@@@ new random fading: value= "+String(randValue)+", duration= "+String(randDur);
          Serial.println(s);
      
        oPwm[i].setFadingDuration(randValue,randDur);
      }
    }
}
