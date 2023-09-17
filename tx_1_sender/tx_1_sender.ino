#include <RH_ASK.h>
#include <SPI.h>
#include <RGBLed.h>


//button
int switchPin_kat_gr = 3;

//433
RH_ASK driver;

//led
RGBLed led(5, 7, 6, RGBLed::COMMON_CATHODE);
int redpin = 5; //pin red LED
int bluepin = 6; //pin blue LED
int greenpin = 7;//pin green LED



void setup()
{
  //433
  driver.init();

   //RGB HW-479
  pinMode(redpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  
  led.flash(RGBLed::WHITE, 1000); //
 
}

void loop()
{
   // включаем всетодиод при нажатой кнопке
  if(digitalRead(switchPin_kat_gr) == HIGH) 
    {
      const char *msg = "k1";
      driver.send((uint8_t *)msg, strlen(msg));
      driver.waitPacketSent();
      delay(100);        
      
      led.brightness(RGBLed::GREEN, 10);
      led.flash(RGBLed::GREEN, 1000);      
    } 
  else 
    {
      led.off();
    }

}
