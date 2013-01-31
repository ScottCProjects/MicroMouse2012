#include "MemoryFree.h"
#include <Wire.h>
#include "MouseBrain.h"
//#include "Maze.h"
//Mouse mouse;
void setup()
{
  Serial.begin(9600);
  out.on('r');
  button.pause();
  
  initMouse();
  out.off('r');
  out.on('g');
  Serial.print("In Setup: ");
  Serial.println(freeMemory());
  printIntInfo();
  frontIR.setWallDist( 408 );
  leftIR.setWallDist( 393 );
  rightIR.setWallDist( 514 );
  backIR.setWallDist( 470 );
  leftM.count = 0;
  rightM.count = 0;
  
  //initSensors();
  //button.pause();
}


void loop()
{
  Serial.println( facingStraight() );
  delay(1000);
}
