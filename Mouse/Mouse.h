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
#include "../Compass/Compass.h"

typedef unsigned char byte;

// -------------- Motors, Sensors and Pins ------------------

#define leftMPinA 6
#define leftMPinB 5
#define leftMEncA 2
#define leftMEncB 0
#define rightMPinA 10
#define rightMPinB 11
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

#define defaultFrontDist 408.0
#define defaultRightDist 514.0
#define defaultBackDist 470.0
#define defaultLeftDist 393.0
#define defaultMaxDist 850.0

#define compTurnSpeed 70
#define degreeError 300 // n/10 degrees
#define tooClose_corr 25
#define one_space 235
#define avoidSubtNum 8
#define avoidAddNum 4
#define turn_f 220
#define turn_b 35
#define space_aft_turn 125
#define irERROR 10

#define NOCOMPASS
#define DEFAULTSVALS
//#define NOWALLAUTOTURN

// -----------------------------------------------------------


Motor leftM( leftMPinA, leftMPinB, leftMEncA, leftMEncB );
Motor rightM( rightMPinA, rightMPinB, rightMEncA, rightMEncB );
IRSensor frontIR( frontIRPin, IRPowerPin );
IRSensor leftIR( leftIRPin, IRPowerPin );
IRSensor rightIR( rightIRPin, IRPowerPin );
IRSensor backIR( backIRPin, IRPowerPin );

Output out( redPin, greenPin, bluePin );
Pushbutton button(buttonPin);
Compass compass;
int trueNorth;

#ifdef NOWALLAUTOTURN
bool noPrevWall = false;
#endif

//-------------------- Functions -------------------------

void initMouse();
int within3600( int num );
bool facingStraight();
void compassCorrect( int heading );
void compassTurnTo( int newHeading );

void exCommand( char comm = '!', uint8_t spaces = 1 );
void exGetCommand();
void exRandCommand( bool pause = 0 );
void printIRInfo();
void printIntInfo();

void initSensors();
bool avoidWalls( Motor& lM, Motor& rM, IRSensor& front,
					IRSensor& left, IRSensor& right, char dir );
bool move( char dir = 'f', uint8_t spaces = 1 );
bool turn( char dir );
bool executeTurn( Motor& m1, Motor& m2,
		IRSensor& IR1, IRSensor& IR2 );

//--------------------------------------------------------



// Add into IR class a way to standardize distance
//   and ambient IR
void initMouse()
{
	Serial.println("Initializing mouse.");
	out.off();
	#ifndef NOCOMPASS
	Serial.println("Initializing compass.");
	out.on('b');
	compass.calibrate();
	out.blink('b', 2);
	out.on('b');
	button.pause();
	compass.setTrueNorth();
	while( compass.req_read() < 3500 && compass.req_read() > 100 )
	{
		//Serial.print( compass.heading );
		out.ERROR("trueNorth incorrectly set");
		compass.setTrueNorth();
	}
	out.blink('g');
	out.on('g');
	#endif
	Serial.println("Initializing sensors.");
	initSensors();
	delay(10);
	Serial.println("** Initialization done.");
}

int within3600( int num )
{
	if( num > 3600 )
		return num - 3600;
	if( num < 0 )
		return num + 3600;
	return num;
}

bool facingStraight()
{
	#ifdef NOCOMPASS
	return true;
	#endif
	int heading = compass.req_read();
	Serial.println(heading);
	return (heading > 3600-degreeError || heading < degreeError) ||
		(heading > 900-degreeError && heading < 900+degreeError) ||
		(heading > 1800-degreeError && heading < 1800+degreeError) ||
		(heading > 2700-degreeError && heading < 2700+degreeError);
}

void compassCorrect( int heading )
{
	if((heading > 3150 && heading <= 3600) ||
		(heading > 0 && heading <= 450)) compassTurnTo(0);
	else if(heading > 450 && heading <= 1350) compassTurnTo(900);
	else if(heading > 1350 && heading <= 2250) compassTurnTo(1800);
	else if(heading > 2250 && heading <= 3150) compassTurnTo(2700);
}

void compassTurnTo( int newHeading )
{
	int realNorth = compass.trueNorth;
	compass.trueNorth = newHeading;
	compass.req_read();
	if( compass.heading < 180 )
		while( !facingStraight() /*compass.req_read() > 20 && compass.heading < 180*/ )
		{
			leftM.on( 10, 'b', compTurnSpeed );
			rightM.on( 10, 'f', compTurnSpeed );
			delay(10);
		}
	else
		while( !facingStraight() /*compass.req_read() < 340 && compass.heading > 180*/ )
		{
			leftM.on( 10, 'f', compTurnSpeed );
			rightM.on( 10, 'b', compTurnSpeed );
			delay(10);
		}
	leftM.off();
	rightM.off();
	out.blink('b');
	out.blink('r');
	out.blink('b');
	compass.trueNorth = realNorth;
}
		

void initSensors()
{
	button.count = 0;
	#ifndef DEFAULTSVALS
	out.put("Press to measure ambient IR\n", 'a', 0);
	while( !button.isPushed() ) out.blink( 'b' );
	
	double percent = ((double)backIR.rawDist() / (double)defaultBackDist
			+ (double)leftIR.rawDist() / (double)defaultLeftDist )/2.0;
	Serial.print("percent = ");
	if( percent > 1.17 ) percent = 1.17;
	Serial.println(percent);
	#endif
	while( !button.isPushed() ) out.blink( 'b' );

	frontIR.setWallDist( defaultFrontDist/**percent,
							defaultMaxDist+percent*10*/ );
	rightIR.setWallDist( defaultRightDist/**percent,
							defaultMaxDist+percent*10*/ );
	backIR.setWallDist( defaultBackDist/**percent,
							defaultMaxDist*percent*10*/ );
	leftIR.setWallDist( defaultLeftDist/**percent,
							defaultMaxDist*percent*10*/ );

}

void exCommand( char comm, uint8_t spaces )
{
	switch( comm )
	{
		case 'l':
		case 'r':
			turn( comm );
			break;
		case 'p':
			char c;
			if( Serial.available() ) c = Serial.read();
			//switch( c )
			{
			//	case 'i':
					printIRInfo();
			//		break;
			//	case 'm':
					printIntInfo();
			//		break;
			}
			break;
		default:
			move( comm, spaces );
	}
}

void exGetCommand()
{
	while( !Serial.available() );
	exCommand( Serial.read() );
}

void exRandCommand( bool pause )
{
	char c;
	do
	{
		c = (char)random( 98, 114 );
	} while( c != 'f' && c != 'b' && c != 'l' && c != 'r' );
	if( pause ) button.pause();
	exCommand( c, 1 );
}


bool move( char dir, uint8_t spaces ) ///// must fix to handle backwards
{
	if( dir == 'l' || dir == 'r' )
	{
		return turn( dir );
	}
	Motor* leftMptr,* rightMptr;
	IRSensor* frontIRptr, * leftIRptr, * rightIRptr;

//	leftIRptr = &leftIR;
//	rightIRptr = &rightIR;
	switch( dir )
	{
		case 'f':
			leftMptr = &leftM;
			rightMptr = &rightM;
			frontIRptr = &frontIR;
			leftIRptr = &leftIR;
			rightIRptr = &rightIR;
			break;
		case 'b':
			leftMptr = &rightM;
			rightMptr = &leftM;
			frontIRptr = &backIR;
			leftIRptr = &rightIR;
			rightIRptr = &leftIR;
			break;
		default:
			out.ERROR( "Invalid move direction", 5 );
			return false;
	}
//	Serial.println("In Move");	
	for( ; spaces > 0; --spaces )
	{
		if( frontIRptr->wall() )
		{
			out.ERROR( "Cannot move forward", 3 );
			return false;
		}
		
		leftMptr->count = 0;
		rightMptr->count = 0;
		leftMptr->on( one_space, dir );
		rightMptr->on( one_space, dir );
			
		
		// keep moving forward while avoiding walls
		if( !avoidWalls( *leftMptr, *rightMptr, *frontIRptr,
					*leftIRptr, *rightIRptr, dir ) )
			return false;
		#ifdef NOWALLAUTOTURN
		if( !left.wall() && !right.wall() )
			noPrevWall = true;
		#endif

	}
	return true;
}

bool avoidWalls( Motor& lM, Motor& rM, IRSensor& front,
					IRSensor& left, IRSensor& right, char dir )
{
	out.off();
	bool moved = true;
	int initHeading = compass.req_read();
	while( !front.tooClose() && !lM.isDone() && !rM.isDone() )
	{
		lM.waitFor( rM, dir );
		rM.waitFor( lM, dir );
		if( (right.wall() && left.dist() +
				(dir == 'b' ? 1 : irERROR) < right.dist() ||
			left.tooClose()) /*|| right.tooFar()*/ /*&& facingStraight()*/ )
		//	lM.count -= 1;
		//else if( left.tooClose() )
		{
			//lM.speed -= 10;
			lM.count -= avoidSubtNum;
			rM.waitFor( lM, dir );
			lM.count += avoidAddNum;
			lM.waitFor( rM, dir );
		}

		if( (left.wall() && right.dist() +
				(dir == 'b' ? 1 : irERROR) < left.dist() ||
			right.tooClose())/* || left.tooFar()*/ /*&& facingStraight()*/ )
		//	rM.count -= 1;
		//else if( right.tooClose() )
		{
			//rM.speed -= 10;
			rM.count -= avoidSubtNum;
			lM.waitFor( rM, dir );
			rM.count += avoidAddNum;
			rM.waitFor( lM, dir );
		}
		
		int lmax = lM.max;
		int rmax = rM.max;
		int lcount = lM.count;
		int rcount = rM.count;
		if( !facingStraight() )
		{
			out.blink( 'r', 2 );
			lM.count = 0;
			rM.count = 0;
			switch( dir )
			{
				case 'f':
					lM.back( tooClose_corr );
					rM.back( tooClose_corr );
					break;
				default:
					lM.forward( tooClose_corr );
					rM.forward( tooClose_corr );
			}
			while( !lM.isDone() || !rM.isDone() );
			lM.count = lcount;
			rM.count = rcount;
			lM.on( lmax, dir );
			rM.on( rmax, dir );
			compassCorrect( initHeading );
		}

	}
	lM.off();
	rM.off();
	
	if( front.tooClose() )
	{
		out.blink( 'b', 1 );
		if( lM.count < lM.max/2 || lM.count < lM.max/2 )
			moved = false;
		lM.count = 0;
		rM.count = 0;
		if( dir == 'f' )
		{
			lM.on( tooClose_corr, 'b' );
			rM.on( tooClose_corr, 'b' );
		}
		else
		{
			lM.forward( tooClose_corr );
			rM.forward( tooClose_corr );
		}
		//lM.off();
		//rM.off();
	}

	return moved;
}
	
bool turn( char dir )
{
	switch( dir )
	{
		case 'l':
			if( !leftIR.wall() )
				return executeTurn( leftM, rightM, leftIR, rightIR );
			else
			{
				out.ERROR( "Cannot turn left.\n", 2 );
				return false;
			}
			break;
		case 'r':
			if( !rightIR.wall() )
				return executeTurn( rightM, leftM, rightIR, leftIR );
			else
			{
				out.ERROR( "Cannot turn right.\n", 2 );
				return false;
			}
			break;

		default:
			out.ERROR( "Invalid Turn Direction", 4 );
			return false;
	}
}

bool executeTurn( Motor& m1, Motor& m2,
		IRSensor& IR1, IRSensor& IR2 )
{
	m1.count = 0;
	m2.count = 0;
	m1.back( turn_b );
	while( !m1.isDone() );
	m2.forward( turn_f );
	while( !m2.isDone() );
	m1.count = 0;
	m2.count = 0;
	m1.on( space_aft_turn, 'f' );
	m2.on( space_aft_turn, 'f' );
	avoidWalls( leftM, rightM, frontIR, leftIR, rightIR, 'f' );
	return true;
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
		Serial.print( "  Raw: " );
		Serial.println( sPtr[i]->rawDist() );
		Serial.print( " Offs: " );
		Serial.println( sPtr[i]->offset );
		Serial.print( "  Max: " );
		Serial.println( sPtr[i]->max );
		Serial.print( "\t\twDist: " );
		Serial.println( sPtr[i]->wDist );
		Serial.print( "\t\ttooClose: " );
		Serial.println( sPtr[i]->tooClose() );
	}
	Serial.println( "---------------------" );
}

void printIntInfo()
{
	Serial.print("Left Motor: "); Serial.println(leftM.count);
	Serial.print("Right Motor: "); Serial.println(rightM.count);
	Serial.print("Pushbutton: "); Serial.println(button.count);
}






#undef one_space
#undef irERROR
#endif
