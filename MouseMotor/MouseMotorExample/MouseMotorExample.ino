// MouseMotor Test Sketch
// - Using "ooPinChangeInt.h"
//====================================================

#include "Arduino.h"
#include "MouseMotor.h"
#include "ooPinChangeInt.h"

#define NO_PORTB_PINCHANGES
#define NO_PORTC_PINCHANGES

#define leftA 2
#define leftB 0 // <--- 0 meaning not used
#define rightA 3
#define rightB 0
#define lmPinA 6
#define lmPinB 5
#define rmPinA 10
#define rmPinB 11

Motor leftM( lmPinA, lmPinB, leftA, leftB );
Motor rightM( rmPinA, rmPinB, rightA, rightB );

#define num_steps 1000
void setup()
{
  Serial.begin(9600);
  leftM.on(num_steps, 'b');
  rightM.on(num_steps);
}

void loop()
{
  Serial.print("Left Motor: ");
  Serial.println( leftM.ct() );
  Serial.print("Right Motor: ");
  Serial.println( rightM.ct() );
  delay(1000);
  
  if(!leftM.isOn)
  {
    Motor::syncOn( leftM, rightM, 1000 );
    Motor::syncOn( leftM, rightM, 1000, 'b' );
    return;
  }
    
}


