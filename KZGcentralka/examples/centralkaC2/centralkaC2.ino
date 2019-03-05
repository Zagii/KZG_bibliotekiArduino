/* *******************************************************
** Przykład użycia biblioteki, 
 *
** ******************************************************* */

/******
 * 
 * Each of the 54 digital pins on the Mega can be used as an input or output,
 * using pinMode(), digitalWrite(), and digitalRead() functions. 
 * They operate at 5 volts. 
 * Each pin can provide or receive a maximum of 40 mA and has an internal pull-up resistor (disconnected by default) of 20-50 kOhms.
 * In addition, some pins have specialized functions:  
 * ** Serial: 0 (RX) and 1 (TX); 
 * ** Serial 1: 19 (RX) and 18 (TX); 
 * ** Serial 2: 17 (RX) and 16 (TX); 
 * ** Serial 3: 15 (RX) and 14 (TX).
 * Used to receive (RX) and transmit (TX) TTL serial data. 
 * Pins 0 and 1 are also connected to the corresponding pins of the ATmega16U2 USB-to-TTL Serial chip.
 * External Interrupts: 
 * ** 2 (interrupt 0), 
 * ** 3 (interrupt 1), 
 * ** 18 (interrupt 5), 
 * ** 19 (interrupt 4), 
 * ** 20 (interrupt 3), 
 * ** and 21 (interrupt 2). 
 * These pins can be configured to trigger an interrupt on a low value, 
 * a rising or falling edge, or a change in value. 
 * See the attachInterrupt() function for details.  
 * ** PWM: 2 to 13 + 45 + 46 . Provide 8-bit PWM output with the analogWrite() function.  
 * ** SPI: 50 (MISO), 51 (MOSI), 52 (SCK), 53 (SS). These pins support SPI communication using the SPI library. 
 * The SPI pins are also broken out on the ICSP header, which is physically compatible with the Uno, Duemilanove and Diecimila.
 * ** LED: 13. There is a built-in LED connected to digital pin 13. 
 * **** When the pin is HIGH value, the LED is on, when the pin is LOW, it's off.
 * ** TWI: 20 (SDA) and 21 (SCL). Support TWI communication using the Wire library. 
 * Note that these pins are not in the same location as the TWI pins on the Duemilanove or Diecimila.  
 * The Mega2560 has 16 analog inputs, each of which provide 10 bits of resolution (i.e. 1024 different values). 
 * By default they measure from ground to 5 volts, though is it possible to change the upper end of their range using the AREF pin and analogReference() function.  
 * There are a couple of other pins on the board:  
 * ** AREF. Reference voltage for the analog inputs. Used with analogReference().  
 * ** Reset. Bring this line LOW to reset the microcontroller. Typically used to add a reset button to shields which block the one on the board. 
 * ***/

#include <KZGcentralka.h>


/////////////////s14 to ledy w lazience na dole nie zaznaczone na projekcie
                                   ///    wyjsciaPWM              wyj 12V                                                 wyj 220V
//const char wyjPIN_OPIS[] PROGMEM = {"PWM1","PWM2","PWM3","OUT1","OUT2","OUT3","OUT4","OUT_220_1","OUT_220_2","OUT_220_3","OUT_220_4","OUT_220_5","OUT_220_6","OUT_220_7","OUT_220_8"};
enum wyjPIN_TXT                    { s1,   s2,   s3,   s4,   s5,  s6,  s7,   s8,  s9,  s10,   s11,  s12,    s13,  s14,   s15,    s16,  s17,  s18 };
//enum wyjPIN_ENUM                    {PWM1   ,PWM2,   PWM3, OUT1,   OUT2,   OUT3,  OUT4, OUT_220_1,OUT_220_2, OUT_220_3,  OUT_220_4,  OUT_220_5,  OUT_220_6,  OUT_220_7,  OUT_220_8};
                                      /*10 to cs eth, pin=4 to od SD nie uzywac z w5100*/    
const byte wyjPIN_pin[] PROGMEM   ={ 30,   31,   2,    32,   33,  34,  35,   3,   13,     6,   36,    7,     8,      9,    11,    12,     45,  46 };
static const  char*  wyjItem[]    ={"s1", "s2", "s3", "s4", "s5","s6","s7", "s8","s9", "s10", "s11","s12", "s13", "s14", "s15", "s16", "s17","s18"};
const bool wyjPWM[]               ={  0,    0,    1,    0,    0,   0,   0,    1,  1,     1,     0,    1,    1,      1,    1,      1,      1,   1};
const byte ile_wyjsc=18;

//const char wejPIN_OPIS[] PROGMEM = {"INP1","INP2","INP3","INP4","INP5","INP6","INP7","INP8","INP9","INP10","INP13","INP14","INP15","INP16"};//"INP11","INP12" -Serial
enum wejPIN_TXT                    {  w1  ,    w2 ,   w3  ,  w4,     w5  ,  w6,      w7  ,  w8,  w9,   w10,  w11,  w12,  w13,  w14,  p1,  p2,  p3};
//enum wejPIN_ENUM                 {  INP1, INP2  ,INP3 ,   INP4 , INP5 , INP6 ,   INP7 , INP8 ,  INP9 ,   INP10 ,INP11,  INP12, INP13 , INP14 , INP15 , INP16 };//"INP11","INP12"
const byte wejPIN_pin[] PROGMEM =  {  A0,     A1,     A2,    A3,     25,    A5,      A4,    A8,  A9,   A10,  A11,  A12,  A13,  A14,  22,  23,  24};   //  0,      1
static const  char* wejItem[]  =   {  "w1",   "w2",  "w3",  "w4",   "w5",  "w6" ,   "w7",  "w8","w9", "w10","w11","w12","w13","w14","p1","p2","p3"};
                                      /// brakuje 2=1wire,4,5, 10, 12
const byte ile_wejsc=16;


KZGcentralka c;

void setup()
{
    Serial.begin(115200);
    byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
    IPAddress mqttHost(192, 168, 0, 3);

    c.begin("c2",mac,mqttHost,"","",1883);
    c.addInput(0,"i0",KZGinput_STAN_RELEASED,true);
    c.addInput(1,"i1",KZGinput_STAN_RELEASED,true);
    c.addInput(2,"i2",KZGinput_STAN_RELEASED,true);

    c.addOutput("o0",10,255,0,0);
    c.addOutput("o1",11,255,0,0);
    c.addOutput("o2",12,255,0,0);
     

    for(uint8_t j=0;j<ile_wejsc;j++)
    {
//      c.addInput(wejPIN_pin[j],wejItem[j],KZGinput_STAN_RELEASED,true);
    }

    for(uint8_t i=0;i<ile_wyjsc;i++)
    {
//      c.addOutput(wyjItem[i],wyjPIN_pin,255,0,0);
    }

    Serial.println("Koniec Setup"); 
}


void loop()
{
    c.loop();
}
