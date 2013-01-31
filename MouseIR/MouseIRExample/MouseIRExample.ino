#include "Arduino.h"
#define wallDist 50
#include "MouseIR.h"

#define IRPowerPin 13
#define frontIRPin 14
#define leftIRPin 15
#define rightIRPin 16
#define backIRPin 17

IRSensor frontIR( frontIRPin, IRPowerPin );
IRSensor leftIR( leftIRPin, IRPowerPin );
IRSensor rightIR( rightIRPin, IRPowerPin );
IRSensor backIR( backIRPin, IRPowerPin );

void setup()
{
  //pinMode( 13, OUTPUT );
  //digitalWrite( 13, HIGH );
  Serial.begin(9600);
}

void loop()
{
  Serial.print("Front Dist: "); Serial.println( frontIR.dist() );
  Serial.print("   Wall: "); Serial.println( frontIR.wall() );
  
  Serial.print("Left Dist: "); Serial.println( leftIR.dist() );
  Serial.print("   Wall: "); Serial.println( leftIR.wall() );
  
  Serial.print("Right Dist: "); Serial.println( rightIR.dist() );
  Serial.print("   Wall: "); Serial.println( rightIR.wall() );
  
  Serial.print("Back Dist: "); Serial.println( backIR.dist() );
  Serial.print("   Wall: "); Serial.println( backIR.wall() );
  delay(1000);
}

