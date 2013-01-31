// MouseMotor Test Sketch
// - Using "PinChangeInt.h", so unable to put inturrupt
//   inside motor class.
//   Will update when "ooPinChangeInt.h" is fixed
//====================================================

#include "Arduino.h"
#include <PinChangeInt.h>
#include "MouseMotor.h"

//#define NO_PORTB_PINCHANGES
//#define NO_PORTC_PINCHANGES

#define leftA 2
#define leftB 0 // <--- 0 meaning not used
#define rightA 3
#define rightB 0
#define lmPinA 6
#define lmPinB 5
#define rmPinA 10
#define rmPinB 11

uint8_t latest_int_pin;
int int_count[20] = {0};

Motor leftM( lmPinA, lmPinB, leftA, leftB, int_count[leftA] );
Motor rightM( rmPinA, rmPinB, rightA, rightB, int_count[rightA] );

void IntCount()
{
  latest_int_pin = PCintPort::arduinoPin;
  int_count[latest_int_pin]++;
}

void setup()
{
  //PCintPort::attachInterrupt( leftB, &leftMInt, FALLING );
  //PCintPort::attachInterrupt( rightA, &rightMInt, FALLING );
  //PCintPort::attachInterrupt( rightA, &rightMInt, FALLING );
  pinMode(leftA, INPUT); digitalWrite(leftA, HIGH);
  PCintPort::attachInterrupt( leftA, &IntCount, CHANGE );
  pinMode(rightA, INPUT); digitalWrite(rightA, HIGH);
  PCintPort::attachInterrupt( rightA, &IntCount, CHANGE );
  Serial.begin(9600);
}

void loop()
{
  leftM.on();
  rightM.on();
  
  while( int_count[leftA] < 10000 )
  {
    leftM.on();
    rightM.on();
    Serial.print("Left Motor: ");
    Serial.println(int_count[leftA]);
    Serial.print("Right Motor: ");
    Serial.println(int_count[rightA]);
    delay(1000);
    leftM.off();
    rightM.off();
    delay(1000);
  }
  leftM.off();
  rightM.off();
}


