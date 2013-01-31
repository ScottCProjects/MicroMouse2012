// Scott Camarena
// IEEE Micromouse 2012
// Mouse Brain - Top Level Abstracted Mouse Functions
//============================================================

#ifndef _MOUSEBRAIN_H_
#define _MOUSEBRAIN_H_

#include "Arduino.h"
#include "../Mouse/Mouse.h"
#include "../Maze/Maze.h"


Maze maze;

//------------------------- Functions --------------------------

void firstRun();
void speedRuns();

void senseWalls(); // Senses and stores walls
void moveMinDir();
void moveBackToStart();

void printWalls();

//--------------------------------------------------------------

void firstRun()
{
	Serial.println("Beginning First Run.");
	while( maze.dist[maze.curr] != 0 && !button.isPushed() )
	{
		button.count = 0;
		senseWalls();
		printWalls();
		maze.flood();

		moveMinDir();
		while( !leftM.isDone() && !rightM.isDone() );
		delay( 250 );
		//button.pause();
	}

	out.put( "CHEESE FOUND", 'a' );
	
	//maze.storeRoute();
	//moveBackToStart();
}

void speedRuns()
{
	maze.blockUnvisited();
	while( true )
	{
		button.count = 0;
		while( !button.isPushed() ) out.blink('a');
		while( maze.dist[maze.curr] != 0 )
			moveMinDir();
	}
}


void senseWalls()
{
	maze.updateWalls( frontIR.wall(), rightIR.wall(),
						backIR.wall(), leftIR.wall() );
}

void moveMinDir()
{
	char dir = maze.getRelDir( maze.minOpenDir( maze.curr ) );
	Serial.print( "Moving: " );
	Serial.println( dir );
	// If it successfully moved forward, update position in maze
	if( move( dir, 1 ) ) 
		maze.movePos( dir );

}

/*void moveBackToStart()
{
	while( maze.curr != 0 )
	{
		byte moveTo = maze.walls[maze.curr] >> 4;
		if( NORTH
		move( maze.getRelDir( 
}
*/


void printWalls()
{
	Serial.print( "\n " );
	Serial.println( maze.walls[maze.curr] & 1 );
	
	Serial.print( maze.walls[maze.curr] & 8 );
	Serial.print( "   " );
	Serial.println( maze.walls[maze.curr] & 2 );
	Serial.print( " " );
	Serial.println( maze.walls[maze.curr] & 4 );
}




#endif
