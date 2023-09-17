#include <RH_ASK.h>
#include <SPI.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <LedControl.h>

void ShowInfo();
void ShowCalls();


//433
RH_ASK driver;
char receive[32];

//disp
LiquidCrystal_I2C lcd(0x27, 16, 2);


//EEPROM
byte senoval; 
byte petr; 
byte katya; 

//sensor btn
int button_reset = 3;
int button_accept = 5;
bool accept_flag;


//duble click
int bounceTime = 10;          // задержка для подавления дребезга
int doubleTime = 500;         // время, в течение которого нажатия можно считать двойным
int i = 0;
boolean lastReading = false;  // флаг предыдущего состояния кнопки
boolean buttonSingle = false; // флаг состояния "краткое нажатие"
boolean buttonMulti = false; // флаг состояния "двойное нажатие"
long onTime = 0;              // переменная обработки временного интервала
long lastSwitchTime = 0;      // переменная времени предыдущего переключения состояния


//led_matrix
//DataIn, LOAD, CLK, Count LED
LedControl lc = LedControl(6, 8, 7, 1);
unsigned long delaytime = 50;
bool cancal_flag = false;


void setup()
{  
  Serial.begin(9600);
  driver.init();

  //read eeprom
  lcd.init();           // инициализация
  lcd.backlight();      // включить подсветку
  Serial.println("Start");

  //btn
  pinMode(button_accept, INPUT);
  pinMode(button_reset, INPUT);

  //ledpannel
  lc.shutdown(0, false);  
  lc.setIntensity(0, 15);  
  lc.clearDisplay(0);
  
  ShowInfo();  
  lcd.setCursor(4, 1);  // столбец 4 строка 1
  lcd.print("No calls ");  
}


void loop() 
{
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  //boolean reading = digitalRead(button_accept);
  boolean reading = digitalRead(button_reset);


  if (driver.recv(buf, &buflen))
    {
      memset(receive, 0, sizeof(receive));
      for (int i = 0; i < buflen; i++) 
        {
          receive[i] = buf[i];
        }
      if (strcmp(receive, "b1") == 0) 
        {
          Serial.println("Senoval Calls!"); 
          EEPROM.update(0, ++senoval);          
          ShowInfo();
          ShowCalls("Senoval!");  
          single();
        } 
        
      else if (strcmp(receive, "p2") == 0) 
        {
          Serial.println("Pert calls!");
          EEPROM.update(1, ++petr);
          ShowInfo();
          ShowCalls("Petr!");
          single();
        }

      else if (strcmp(receive, "k1") == 0) 
        {
          Serial.println("Katya calls!");          
          EEPROM.update(2, ++katya);
          //ShowInfo();         
          //ShowCalls("Katya!   ");
          single();
        }
      }

//--------------------------------------------

  if (digitalRead(button_accept) == HIGH) 
    { 
      
      lcd.setCursor(2, 1);  // столбец 4 строка 1
      lcd.print("Accept call!  ");  
      delay(3000);        
      lcd.setCursor(2, 1);  // столбец 4 строка 1     
      lcd.print("  No calls ");  
      
      Serial.println("button_accept press");          
      cancal_flag = false;
      lc.clearDisplay(0);
    }
    
/*

    if (digitalRead(button_reset) == HIGH) 
    { 
      accept_flag = true;
      lcd.setCursor(2, 1);  // столбец 4 строка 1
      lcd.print("Clear EEPROM");
      ClearCont();
      delay(3000);
      lcd.clear();      
      ShowInfo();
      lcd.setCursor(4, 1);  // столбец 4 строка 1
      lcd.print("No calls ");  
    }
*/
//----------------- NEW KEY ------------------
// проверка первичного нажатия
  if (reading && !lastReading)
    {
      onTime = millis();
    }
  
  if (!reading && lastReading)
    {
      if (((millis() - onTime) > bounceTime))
        {
          if ((millis() - lastSwitchTime) >= doubleTime)
            {
              lastSwitchTime = millis();
              buttonSingle = true;
              i=1;
            }       
          else 
            {
              i++;
              lastSwitchTime = millis();
              buttonSingle = false;
              buttonMulti = true;
            }
          }
    }
  
  lastReading = reading;
    
  if (buttonMulti && (millis() - lastSwitchTime) > doubleTime){
    if (i == 3) 
      {
          accept_flag = true;
          lcd.setCursor(2, 1);  // столбец 4 строка 1
          lcd.print("Clear EEPROM");
          ClearCont();
          delay(3000);
          lcd.clear();      
          ShowInfo();
          lcd.setCursor(4, 1);  // столбец 4 строка 1
          lcd.print("No calls ");  
          Serial.println("button_erase press");
          buttonSingle = false;
          buttonMulti = false;
      }    
//--------------------------------------------

  }
}


void ShowInfo() {
  EEPROM.get(0, senoval);
  EEPROM.get(1, petr);
  EEPROM.get(2, katya);
 
  lcd.setCursor(1, 0);  // столбец 1 строка 0
  lcd.print("S="+ String(senoval));

  lcd.setCursor(6, 0);  // столбец 1 строка 0
  lcd.print("P="+ String(petr));

  lcd.setCursor(11, 0);  // столбец 1 строка 0
  lcd.print("K="+ String(katya));  
 

  Serial.println("call ShowInfo fucn");
  
}

void ShowCalls(String name) 
  {
   
    lcd.setCursor(2, 1);  // столбец 4 строка 1  
    lcd.print("                ");  
    lcd.setCursor(2, 1);  // столбец 4 строка 1  
    lcd.print("Calls " + name);  
   
    Serial.println("LCD update - ShowCalls");
  }


void ClearCont() {
  EEPROM.update(0, 0);
  EEPROM.update(1, 0);
  EEPROM.update(2, 0);
}


void yield() {
    if (digitalRead(button_accept) == HIGH) 
    { 
      cancal_flag = true;    
      Serial.println("yield!");
    }
}


void single() {
  ShowInfo();         
  ShowCalls("Katya!   ");
  Serial.println("LCD update - single");

  for (int row = 0; row < 8; row++)        
      {        
        Serial.println("cancal_flag =" + String(cancal_flag));
        for (int col = 0; col < 8; col++) 
          {
          if (cancal_flag == false) 
        {
          delay(delaytime);
          lc.setLed(0, row, col, true);
          delay(delaytime);
        }
      else
        break;
            
          for (int i = 0; i < col; i++) 
            {
              if (cancal_flag == false) 
              {
                lc.setLed(0, row, col, false);
                delay(delaytime);
                lc.setLed(0, row, col, true);
                delay(delaytime);
             }
      else
        break;
      }
    }
  }
}
