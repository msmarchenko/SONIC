extern "C" {
  // AVR LibC Includes
  #include <inttypes.h>
  #include <avr/interrupt.h>
}
#include <Arduino.h>
#include "SONIC.h"

#define DIGITAL_PIN_1 4
#define DIGITAL_PIN_2 7
#define DIGITAL_PIN_3 8
#define DIGITAL_PIN_4 11
#define DIGITAL_PIN_5 12

byte Cal_Sonic=0;
ISSensor* sensors[6];
int get_Sonic (byte a)
{  
  if(!Cal_Sonic){
    sensors[1]  = new SonicSensor(DIGITAL_PIN_1);
    sensors[2]  = new SonicSensor(DIGITAL_PIN_2);
    sensors[3]  = new SonicSensor(DIGITAL_PIN_3);
    sensors[4]  = new SonicSensor(DIGITAL_PIN_4);
    sensors[5]  = new SonicSensor(DIGITAL_PIN_5);
    Cal_Sonic=1;
  }
  if(a==DIGITAL_PIN_1)
  a=1;
  else if(a==DIGITAL_PIN_2)
  a=2;
  else if(a==DIGITAL_PIN_3)
  a=3;
  else if(a==DIGITAL_PIN_4)
  a=4;
  else if(a==DIGITAL_PIN_5)
  a=5;
  else
  return -1;
  
      if (sensors[a] -> isReady()) {
        sensors[a] -> reset();
      }
      while (!(sensors[a] -> isReady())) 
      {
        sensors[a] -> iteration(0, 0);
      }
  int result = sensors[a] -> getResult();
  return(result);
}

