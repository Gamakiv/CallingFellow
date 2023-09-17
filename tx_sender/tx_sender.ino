#include <RH_ASK.h>
#include <SPI.h>
#include <RGBLed.h>


//button
int switchPin_bol_blue = 2;
int switchPin_ant_yell = 3;

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
  if(digitalRead(switchPin_bol_blue) == HIGH) 
    {
      const char *msg = "b1";
      driver.send((uint8_t *)msg, strlen(msg));
      driver.waitPacketSent();
      delay(100);        
      
      led.brightness(RGBLed::BLUE, 10);
      led.flash(RGBLed::BLUE, 1000);      
    } 
  else 
    {
      led.off();
    }

    if(digitalRead(switchPin_ant_yell) == HIGH) 
    {
      const char *msg = "p2";
      driver.send((uint8_t *)msg, strlen(msg));
      driver.waitPacketSent();
      delay(100);        
      
      led.brightness(RGBLed::YELLOW, 10);
      led.flash(RGBLed::YELLOW, 1000);     
    } 
  else 
    {
      led.off();
    }

    /*
    const char *msg = "hello";
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(500);
    */
}
