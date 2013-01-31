//
// MouseMovement.h
// - The move functions for the mouse
//
//==========================================================

#ifndef _MMovement_
#define _MMovement_

#include "Arduino.h"
#include "../MouseMotor/MouseMotor.h"
#include "../MouseIR/MouseIR.h"

using namespace std;

#define one_space 10 //clicks

//----------------- Function Declarations -------------------
void correctDir( char dir = 'f' ); // Corrects direction when moving forward or back
bool forward( char dir = 'f' ); // Moves one space forward by default ( or back )
													//   returns 'false' if unable to move forward



//-----------------------------------------------------------



// move one space forward, while avoiding walls
bool forward( char dir )
{
	//if( dir == 'b' ) // if moving back, swap left and right motors / IRs
	if( frontIR.wall() ) return false;

	// move both motors n clicks forward
	Motor::syncOn( leftM, rightM

	return true;
}


void correctDir( char dir )
{
	if( leftIR.dist()
}









#endif
