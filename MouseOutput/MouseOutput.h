//
// MouseOutput.h
// - Serial and LED output
//
//=====================================================

#ifndef _MOUSEOUTPUT_H_
#define _MOUSEOUTPUT_H_

#include "Arduino.h"

#define bl_time 400

class LED
{
	private:
		int pin, bringLow;
	
	public:
		LED( int p = 0, bool bl = 0 );
		void on();
		void off();
		void blink( int times = 1 );
};

class pRGB // p meaning "piranha" RGB, the name of it
{
	private:
		LED blue, green, red;
	
	public:
		pRGB( int r = 0, int g = 0, int b = 0 );
		void on( char color = 'a' ); // 'a' for all colors
		void off( char color = 'a' );
		void blink( char color = 'a', int times = 1 );
};

class Output
{
	private:
		pRGB rgb;
	
	public:
		Output( int r = 0, int g = 0, int b = 0 );
		void put( char* str = NULL, char color = 'g', int bTimes = 2 );
		void blink( char color = 'a', int bTimes = 1 );
		void on( char color = 'a' );
		void off( char color = 'a' );
		void ERROR( char* str = NULL, int bTimes = 3 );
};





LED::LED( int p, bool bl )
: pin(p), bringLow(bl)
{
	if( p == 0 ) return;
	pinMode( pin, OUTPUT );
	if( bringLow ) digitalWrite( pin, HIGH );
	else digitalWrite( pin, LOW );
}

void LED::on()
{
	if( bringLow ) digitalWrite( pin, LOW );
	else digitalWrite( pin, HIGH );
}
void LED::off()
{
	if( bringLow ) digitalWrite( pin, HIGH );
	else digitalWrite( pin, LOW );
}

void LED::blink( int times )
{
	for( ; times > 0; --times )
	{
		on();
		delay(bl_time);
		off();
		delay(bl_time);
	}
}
	

pRGB::pRGB( int r, int g, int b )
: blue( LED( b, true ) ), green( LED( g, true ) ), red( LED( r, true ) )
{}


void pRGB::on( char color )
{
	switch( color )
	{
		case 'r':
			red.on();
			break;
		case 'b':
			blue.on();
			break;
		case 'g':
			green.on();
			break;

		default: // case 'a'
			red.on();
			green.on();
			blue.on();
	}
}

void pRGB::off( char color )
{
	switch( color )
	{
		case 'r':
			red.off();
			break;
		case 'b':
			blue.off();
			break;
		case 'g':
			green.off();
			break;

		default: // case 'a'
			red.off();
			green.off();
			blue.off();
	}
}


void pRGB::blink( char color, int times )
{
	switch( color )
	{
		case 'r':
			blue.off();
			green.off();
			red.blink( times );
			break;
		case 'b':
			red.off();
			green.off();
			blue.blink( times );
			break;
		case 'g':
			red.off();
			blue.off();
			green.blink( times );
			break;

		default:
			for( ; times > 0; --times )
			{
				red.blink();
				blue.blink();
				green.blink();
			}
	}
}




Output::Output( int r, int g, int b )
: rgb( r, g, b )
{
//	Serial.begin( 9600 );
}

void Output::put( char* str, char color, int bTimes )
{
	if( str != NULL ) Serial.print( str );
	if( bTimes == 0 ) rgb.on( color );
	else rgb.blink( color, bTimes );
}

void Output::blink( char color, int bTimes )
{
	rgb.blink( color, bTimes );
}

void Output::on( char color )
{
	rgb.on( color );
}

void Output::off( char color )
{
	rgb.off( color );
}

void Output::ERROR( char* str, int bTimes )
{
	Serial.print( "*ERROR*" );
	if( str != NULL )
	{
		Serial.print( ": " );
		Serial.print( str );
	}
	
	Serial.print("\n");

	rgb.blink( 'r', bTimes );
}











#undef bl_time
#undef br
#endif
