// Scott Camarena
// IEEE Micromouse 2012
// Maze Class - C++ Implimentation
//===============================================================

#ifndef _MAZE_H_
#define _MAZE_H_

#include "../ByteStack/ByteStack.h"

#define NORTH 1
#define EAST 2
#define SOUTH 4
#define WEST 8
#define VISITED 128

typedef unsigned char byte;

// Holds the maze and everything known of the maze
//		in relation to the mouse.

class Maze
{
public:
	byte walls[256]; // One to hold the walls
	byte dist[256]; // Second to hold distance from center
	byte curr; // Current cell index
	byte facing; // Direction facing
	ByteStack stack; // Holds cell indexes to fill


	Maze();

	byte block( byte current, byte dir ); // Returns the neighbor block
	byte getNESW( char& dir ); // Converts relative dir to absolute
	char getRelDir( byte nesw ); // Converts absolute dir to relative
	void movePos( char dir ); // Moves known current position
	//char moveNext(); // Returns recommended next move
	void updateWalls( bool front, bool right, bool back, bool left );
	void flood();
	void blockUnvisited();
	byte minOpenDir( byte cell );

	void cxxPrint(); // Prints distance array when testing in C++
	void cxxCommand( char comm ); // Executes given command
};

byte Maze::block( byte current, byte dir )
{
	switch( dir )
	{
		case 0:
			return current + 16;
		case 1:
			return current + 1;
		case 2:
			return current - 16;
		case 3:
			return current - 1;
	}
}

Maze::Maze()
	: curr(0), facing(0)
{
	//curr = 0; // Set current cell to (0,0)
	//facing = 0; // direction is north
	for( short i = 255; i >= 0; --i )
		walls[i] = 0;
	// Count toward center row from top and bottom
	for( byte col = 0, col2 = 15, dc = 7; col <= col2;
		++col, --col2, --dc )
		// toward center column
		for( byte i = col, dr = 7; i < 128;
			i += 16, --dr )
		{
			dist[i] = dr + dc;
			dist[240 + col - (i - col)] = dr + dc;
			dist[i + col2-col] = dr + dc;
			dist[255 - col - (i - col)] = dr + dc;
		}
		/*dist[119] = 0;
		dist[120] = 0;
		dist[135] = 0;
		dist[136] = 0;*/
}


byte Maze::getNESW( char& dir )
{
	switch( dir )
	{
		case 'f':
			return facing;
		case 'l':
			return facing > 0 ? facing - 1 : 3;
		case 'r':
			return facing < 3 ? facing + 1 : 0;
		case 'b':
			return facing > 1 ? facing - 2 : facing + 2;
	}
}

char Maze::getRelDir( byte nesw )
{
	switch( facing )
	{
		case 0:
			switch( nesw )
			{
				case 0: return 'f';
				case 1: return 'r';
				case 2: return 'b';
				case 3: return 'l';
			}
		case 1:
			switch( nesw )
			{
				case 0: return 'l';
				case 1: return 'f';
				case 2: return 'r';
				case 3: return 'b';
			}
		case 2:
			switch( nesw )
			{
				case 0: return 'b';
				case 1: return 'l';
				case 2: return 'f';
				case 3: return 'r';
			}
		case 3:
			switch( nesw )
			{
				case 0: return 'r';
				case 1: return 'b';
				case 2: return 'l';
				case 3: return 'f';
			}
	}
}
	

void Maze::movePos( char dir )
{
	byte newDir = getNESW( dir );
	if( dir != 'b' ) facing = newDir;
	switch( newDir )
	{
		case 0:
			curr = curr + 16;
			break;
		case 1:
			curr = curr + 1;
			break;
		case 2:
			curr = curr - 16;
			break;
		case 3:
			curr = curr - 1;
	}
}

void Maze::updateWalls( bool front, bool right, bool back, bool left )
{
	// Make absolute directions
	bool temp;
	switch( facing )
	{
		case 0:
			break;
		case 1:
			temp = front;
			front = left;
			left = back;
			back = right;
			right = temp;
			break;
		case 2:
			temp = left;
			left = right;
			right = temp;
			temp = front;
			front = back;
			back = temp;
			break;
		case 3:
			temp = front;
			front = right;
			right = back;
			back = left;
			left = temp;
	}
	
	// Assuming facing North
	// If there's a wall: OR the current known walls with it
	if( front )
		walls[curr] = walls[curr] | NORTH;
	if( right )
		walls[curr] = walls[curr] | EAST;
	if( back )
		walls[curr] = walls[curr] | SOUTH;
	if( left )
		walls[curr] = walls[curr] | WEST;
		
	walls[curr] = walls[curr] | VISITED;
}

void Maze::flood()
{
	// If stack is empty, push curr
	if( stack.empty() )
	{
		stack.push( curr );
	}
	
	byte cell, minDir;
	// While stack is not empty
	while( !stack.empty() )
	{
		// Pop and test a cell
		cell = stack.pop();
		Serial.print("Cell "); Serial.print( cell );
		Serial.println(" popped");
		minDir = minOpenDir( cell );
		// If the current dist. is NOT 1 + min
		if( dist[cell] != 1 + dist[block( cell, minDir )] )
		{
			//  update this dist value to 1 + min
			dist[cell] = 1 + dist[block( cell, minDir )];
			// push other open neighbors on the stack
			if( !(walls[cell] & NORTH) )
				stack.push( block( cell, 0 ) );
			if( !(walls[cell] & EAST) )
				stack.push( block( cell, 1 ) );
			if( !(walls[cell] & SOUTH) )
				stack.push( block( cell, 2 ) );
			if( !(walls[cell] & WEST) )
				stack.push( block( cell, 3 ) );
		}
	}
}

void Maze::blockUnvisited()
{
	for( short int i = 0; i < 256; ++i )
		if( !(walls[i] & VISITED) )
			dist[i] = 254;
}

// Choose which neighbor cell has lowest dist
//   giving priority to the current facing direction
byte Maze::minOpenDir( byte cell )
{
	byte lowDir = 255;
	byte lowVal = 255;
	bool wallInFront = true;
	// North
	if( !(walls[cell] & NORTH) && dist[block( cell, 0 )] <= lowVal )
	{
		lowVal = dist[block( cell, 0 )];
		lowDir = 0;
		if( facing == 0 ) wallInFront = false;
	}
	// East
	if( !(walls[cell] & EAST) && dist[block( cell, 1 )] <= lowVal )
	{
		lowVal = dist[block( cell, 1 )];
		lowDir = 1;
		if( facing == 1 ) wallInFront = false;
	}
	// South
	if( !(walls[cell] & SOUTH) && dist[block( cell, 2 )] <= lowVal )
	{
		lowVal = dist[block( cell, 2 )];
		lowDir = 2;
		if( facing == 2 ) wallInFront = false;
	}
	// West
	if( !(walls[cell] & WEST) && dist[block( cell, 3 )] <= lowVal )
	{
		lowVal = dist[block( cell, 3 )];
		lowDir = 3;
		if( facing == 3 ) wallInFront = false;
	}
/*
	// If the lowest dir is != facing dir, and lowVal = facing_val change
	if( lowVal == dist[block(cell, facing)]
			&& lowDir != facing && !wallInFront )
		return facing;*/
	return lowDir;
}

#endif
