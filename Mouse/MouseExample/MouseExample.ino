#include "MemoryFree.h"
#include <Wire.h>
#include "Mouse.h"
#include "Maze.h"
//Mouse mouse;
Maze maze;
void setup()
{
  
  Serial.begin(9600);
  Serial.println(freeMemory());
  out.on('r');
  button.pause();
  initMouse();
  
  out.off();
  out.on('g');
  Serial.print("In Setup: ");
  Serial.println(freeMemory());
  printIntInfo();
  //initSensors();
  /*frontIR.setWallDist( 408 );
  leftIR.setWallDist( 393 );
  rightIR.setWallDist( 514 );
  backIR.setWallDist( 470 );*/
  //button.pause();
  //move();
  //turn('r');
  leftM.count = 0;
  rightM.count = 0;
  //button.pause();
}


void loop()
{
  exGetCommand();
  printIRInfo();
}
