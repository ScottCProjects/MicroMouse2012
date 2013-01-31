//
// MouseMotor.h
// - Motor class for the micromouse
//
//==================================================================

#ifndef _MOUSEMOTOR_
#define _MOUSEMOTOR_

//#include [stuff i need]
#include "Arduino.h"
#include "PinChangeInt.h"

int* defaultBull;

class Motor
{
	private:
		uint8_t powPinA, powPinB, encA, encB;
		
	public:
		//int count;
		int& count;

		Motor( uint8_t ppA = 0, uint8_t ppB = 0,
					uint8_t encA = 0, uint8_t encB = 0, int& = *defaultBull );
		void on( char = 'f', int = 0 ); // Turn on, or for n steps
		void off();

};


Motor::Motor( uint8_t ppA, uint8_t ppB,
							uint8_t encoA, uint8_t encoB, int& ct )
: powPinA(ppA), powPinB(ppB), encA(encoA), encB(encoB), count( ct )
{
	pinMode( powPinA, OUTPUT );
	pinMode( powPinB, OUTPUT );
	digitalWrite( powPinA, LOW );
	digitalWrite( powPinB, LOW );

	if( encA != 0 ) pinMode( encA, INPUT ); digitalWrite( encA, HIGH );
	if( encB != 0 ) pinMode( encB, INPUT ); digitalWrite( encB, HIGH );
	//PCintPort::attachInterrupt( encA, this, CHANGE );
	//PCintPort::attachInterrupt( encB, this, CHANGE );
}

void Motor::on( char dir, int steps )
{
	if( dir == 'f' ) // Turn one or the other on
	{
		digitalWrite( powPinA, HIGH );
		digitalWrite( powPinB, LOW );
	}
	else
	{
		digitalWrite( powPinA, LOW );
		digitalWrite( powPinB, HIGH );
	}

	if( steps != 0 )
	{
		while( count < steps );
		off();
	}
}

void Motor::off()
{
	digitalWrite( powPinA, LOW ); // Turn both off
	digitalWrite( powPinB, LOW );
}




#endif
