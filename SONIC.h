#include <inttypes.h>

#define portOfPin(P)\
  (((P)>=0&&(P)<8)?&PORTD:(((P)>7&&(P)<14)?&PORTB:&PORTC))
#define ddrOfPin(P)\
  (((P)>=0&&(P)<8)?&DDRD:(((P)>7&&(P)<14)?&DDRB:&DDRC))
#define pinOfPin(P)\
  (((P)>=0&&(P)<8)?&PIND:(((P)>7&&(P)<14)?&PINB:&PINC))
#define pinIndex(P)((uint8_t)(P>13?P-14:P&7))
#define pinMask(P)((uint8_t)(1<<pinIndex(P)))

#define pinAsInput(P) *(ddrOfPin(P))&=~pinMask(P)
#define pinAsInputPullUp(P) *(ddrOfPin(P))&=~pinMask(P);digitalHigh(P)
#define pinAsOutput(P) *(ddrOfPin(P))|=pinMask(P)
#define digitalLow(P) *(portOfPin(P))&=~pinMask(P)
#define digitalHigh(P) *(portOfPin(P))|=pinMask(P)
#define isHigh(P)((*(pinOfPin(P))& pinMask(P))>0)
#define isLow(P)((*(pinOfPin(P))& pinMask(P))==0)
#define digitalState(P)((uint8_t)isHigh(P))

class ISSensor {
  public:
    virtual void reset();
    virtual void iteration(byte data, byte data2);
    virtual boolean isReady();
    virtual int getResult();
};
class SonicSensor: public ISSensor {
    boolean resetMode = true;
    boolean measuremnetWaiting;
    unsigned long time;
    int result = 0;
    int pin;
    boolean bReady = false;
  public:
    SonicSensor(int pin) {
      this -> pin = pin;
      time=micros();
    };
    void reset() {
      measuremnetWaiting = false;
      resetMode = true;
      bReady = false;
      time = micros();
    }
   void iteration(byte data, byte data2) {
      //reset mode with delay
      if (resetMode) {
        if (micros() - time > 15000) {
          pinAsOutput(pin);
          digitalWrite(pin, HIGH);
          delayMicroseconds(9);
          digitalWrite(pin, LOW);
          pinAsInput(pin);
          time = micros();
          resetMode = false;
        }
        return;
      }
      if (micros() - time >13000) {
        result = 257;
        bReady = true;
        return;
      }
      if (measuremnetWaiting) {
        if (LOW == digitalState(pin)) {
          result = ((micros() - time)*  34) / 2000 - 14;
          if (result < 0) {
            result = 0;
          }
          bReady = true;
        }
      }
      else {
        if (digitalState(pin) == HIGH) {
          measuremnetWaiting = true;
        }
      }
 }
 boolean isReady() {
      return bReady;
 }
 int getResult() {
      return result;
 }
};
int get_Sonic(byte a);

