//
// MouseMotor.h
// - Motor class for the micromouse
//
//==================================================================

#ifndef _MOUSEMOTOR_
#define _MOUSEMOTOR_

#include "Arduino.h"
#include "../ooPinChangeInt/ooPinChangeInt.h"
#define LIBCALL_OOPINCHANGEINT

typedef unsigned char byte;

class Motor: public CallBackInterface
{
	private:
		uint8_t powPinA, powPinB, encA, encB;
		
	public:
		volatile int max; // Holds max number of steps before turning off
		volatile bool isOn;
		volatile int count;
		int speed;

		Motor( uint8_t ppA = 0, uint8_t ppB = 0,
					uint8_t encoA = 0, uint8_t encoB = 0 );
		// Turn on, or for n steps
		void on( int = 0, char dir = 'f', byte pow = 255 );
		void off();
		void forward( int = 0 );
		void back( int = 0 );
		virtual void cbmethod();
		volatile bool isDone();
		void waitFor( Motor& otherM, char dir = 'f' );
		static void syncOn( Motor& m1, Motor& m2, int steps = 0, char dir = 'f' );
};


Motor::Motor( uint8_t ppA, uint8_t ppB,
							uint8_t encoA, uint8_t encoB )
: powPinA(ppA), powPinB(ppB), encA(encoA), encB(encoB), count(0),
	speed(255)
{
	pinMode( powPinA, OUTPUT );
	pinMode( powPinB, OUTPUT );
	digitalWrite( powPinA, LOW );
	digitalWrite( powPinB, LOW );

	pinMode( encA, INPUT ); digitalWrite( encA, HIGH );
	if( encB != 0 ) pinMode( encB, INPUT ); digitalWrite( encB, HIGH );
	PCintPort::attachInterrupt( encA, this, CHANGE );
	if( encB != 0 ) PCintPort::attachInterrupt( encB, this, CHANGE );
}


void Motor::on( int steps, char dir, byte pow )
{
	speed = pow;
	max = steps;

	//digitalWrite( powPinA, HIGH );
	//digitalWrite( powPinB, LOW );
	if( dir == 'f' ) // Turn one or the other on
	{
		analogWrite( powPinA, speed );
		analogWrite( powPinB, LOW );
	}
	else
	{
		analogWrite( powPinA, LOW );
		analogWrite( powPinB, speed );
	}
	//Serial.print("On");// Serial.println(steps);
	isOn = true;
}

void Motor::off()
{
	digitalWrite( powPinA, LOW ); // Turn both off
	digitalWrite( powPinB, LOW );
	//Serial.println("Motor off.");
	isOn = false;
}

void Motor::forward( int steps )
{
	on( steps, 'f' );
}

void Motor::back( int steps )
{
	on( steps, 'b' );
}

void Motor::cbmethod()
{
	count++;
	if( max != 0 && count >= max ) off();
}

volatile bool Motor::isDone()
{
	return !isOn && (count > max || max == 0);
}


// 'sync' does require continuous attention, but only till sync'd up
void Motor::waitFor( Motor& otherM, char dir )
{
	while( otherM.count < count && !otherM.isDone() ) off();
	on( max, dir );
}

// syncOn requires continuous attention until finished. 
void Motor::syncOn( Motor& m1, Motor& m2, int steps, char dir )
{
	m1.count = 0;
	m2.count = 0;

	while( m1.count < steps || m2.count < steps )
	{
		m1.on( steps, dir );
		m2.on( steps, dir );
		if( m1.count > m2.count ) m1.off();
		if( m2.count > m1.count ) m2.off();
	}

	m1.off();
	m2.off();
}























#endif
