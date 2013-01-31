//
// Mouse.h
// - The complete mouse itself,
//   using motors and IRs
//=================================================================


#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "Arduino.h"
#include "../MouseIR/MouseIR.h"
#include "../MouseMotor/MouseMotor.h"
#include "../MouseOutput/MouseOutput.h"
#include "../PushButton/PushButton.h"
#include "../MemoryFree/MemoryFree.h"

// -------------- Motors, Sensors and Pins ------------------

#define leftMPinA 6
#define leftMPinB 5
#define leftMEncA 2
#define leftMEncB 0
#define rightMPinA 11
#define rightMPinB 10
#define rightMEncA 3
#define rightMEncB 0

#define leftIRPin A1
#define rightIRPin A2
#define frontIRPin A0
#define backIRPin A3
#define IRPowerPin 13

#define redPin 8
#define greenPin 7
#define bluePin 9

#define buttonPin 4

#define one_space 240
#define irERROR 100

// -----------------------------------------------------------


Motor leftM( leftMPinA, leftMPinB, leftMEncA, leftMEncB );
Motor rightM( rightMPinA, rightMPinB, rightMEncA, rightMEncB );
IRSensor frontIR( frontIRPin, IRPowerPin );
IRSensor leftIR( leftIRPin, IRPowerPin );
IRSensor rightIR( rightIRPin, IRPowerPin );
IRSensor backIR( backIRPin, IRPowerPin );
Output out( redPin, greenPin, bluePin );
Pushbutton button(buttonPin);

//-------------------- Functions -------------------------

void initSensors();
bool move( uint8_t spaces = 1, char dir = 'f' );
bool turn( char dir );
void executeTurn( Motor& m1, Motor& m2 );
void printIRInfo();
void printIntInfo();

//--------------------------------------------------------



// Add into IR class a way to standardize distance
//   and ambient IR
void initSensors()
{
	button.count = 0;
	out.put("Press to measure ambient IR\n", 'a', 0);
	while( !button.isPushed() ) out.blink( 'b' );
	
	rightIR.setWallDist();
	frontIR.setWallDist();

	while( !button.isPushed() ) out.blink( 'g' );
	leftIR.setWallDist();
	backIR.setWallDist();
}



bool move( uint8_t spaces, char dir )
{
	for( ; spaces > 0; --spaces )
	{
		/*if( frontIR.wall() )
		{
			out.ERROR( "Cannot move forward", 10 );
			return false;
		}*/
		
		leftM.count = 0;
		rightM.count = 0;
		leftM.on( one_space, dir );
		rightM.on( one_space, dir );
		
		// keep moving forward while avoiding walls
		while(/* !frontIR.tooClose() &&*/ !leftM.isDone() )
		{
			leftM.sync( rightM );
			rightM.sync( leftM );

			if( leftIR.dist() + irERROR < rightIR.dist() )
				leftM.count -= 10;
			if( rightIR.dist() + irERROR < rightIR.dist() )
				rightM.count -= 10;
		}
	}
	return true;
}

bool turn( char dir )
{
	switch( dir )
	{
		case 'l':
			executeTurn( leftM, rightM );
		case 'r':
			executeTurn( rightM, leftM );

		default:
			out.ERROR( "Invalid Turn Direction", 4 );
	}
}

void executeTurn( Motor& m1, Motor& m2 )
{
	
}


void printIRInfo()
{
	IRSensor* sPtr[4] = {&frontIR, &leftIR, &rightIR, &backIR};
	Serial.println( "------ IR Info ------" );
	for( int i = 0; i < 4; ++i )
	{
		switch(i)
		{
			case 0:
				Serial.print( "Front: " );
				break;
			case 1:
				Serial.print( "Left: " );
				break;
			case 2:
				Serial.print( "Right: " );
				break;
			case 3:
				Serial.print( "Back: " );
		}
		Serial.print( sPtr[i]->dist() );
		Serial.print( "\tW: " );
		Serial.println( sPtr[i]->wall() );
		Serial.print( "\t\twDist: " );
		Serial.println( sPtr[i]->wDist );
		Serial.print( "\t\ttooClose: " );
		Serial.println( sPtr[i]->tooClose() );
	}
	Serial.println( "---------------------" );
}

//-----------------------------------------------------------
// NOT USING THIS CLASS - because interrupts have issues
/*
class Mouse : public CallBackInterface//: public Motor : public Pushbutton
{
	//private:
	public:
		Motor leftM, rightM;
		IRSensor frontIR, leftIR, rightIR, backIR;
		Output out;
		Pushbutton button;
	
	//public:
		Mouse( Motor lm = Motor( leftMPinA, leftMPinB, leftMEncA, leftMEncB ),
					Motor rm = Motor( rightMPinA, rightMPinB, rightMEncA, rightMEncB ),
					IRSensor fir = IRSensor( frontIRPin, IRPowerPin ),
					IRSensor lir = IRSensor( leftIRPin, IRPowerPin ),
					IRSensor rir = IRSensor( rightIRPin, IRPowerPin ),
					IRSensor bir = IRSensor( backIRPin, IRPowerPin ),
					Output o = Output( redPin, greenPin, bluePin ),
					Pushbutton b = Pushbutton(buttonPin) );
		

		void initSensors();
		bool forward( uint8_t spaces = 1, char dir = 'f' );
		virtual void cbmethod();
};


Mouse::Mouse( Motor lm, Motor rm, IRSensor fir, IRSensor lir,
							IRSensor rir, IRSensor bir, Output o, Pushbutton b )
: leftM(lm), rightM(rm), frontIR(fir), leftIR(lir),
	rightIR(rir), backIR(bir), out(o), button(b)
{}


void Mouse::initSensors()
{
	button.count = 0;
	out.put("Press to measure ambient IR\n", 'g', 0);
	while( true )
	{
		//Serial.println( button.count );
		//delay(1000);
	}
	out.off();
	Serial.println( 5 );
	// <----------------------------------------------------- Add values here
	

	//frontIR.setMinMax( 70, 900 );
	//leftIR.setMinMax( 70, 800 );
	//rightIR.setMinMax( 50, 850 );
	//backIR.setMinMax( 70, 900 );
}

bool Mouse::forward( uint8_t spaces, char dir )
{
	for( ; spaces > 0; --spaces )
	{
		if( frontIR.wall() )
		{
			out.ERROR( "Cannot move forward", 10 );
			return false;
		}
		
		leftM.count = 0;
		rightM.count = 0;
		leftM.on( one_space, dir );
		rightM.on( one_space, dir );
		
		// keep moving forward while avoiding walls
		while( !frontIR.tooClose() && !leftM.isDone() )
		{
			leftM.sync( rightM );
			rightM.sync( leftM );

			if( leftIR.dist() + irERROR < rightIR.dist() )
				leftM.count -= 10;
			if( rightIR.dist() + irERROR < rightIR.dist() )
				rightM.count -= 10;
		}
	}
	return true;
}*/
















#undef one_space
#undef irERROR
#endif
