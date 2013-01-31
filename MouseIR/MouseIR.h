//
// MouseIR.h
// - IRSensor class for the micromouse
//
//==============================================================

#ifndef _MouseIR_
#define _MouseIR_

#include "Arduino.h"
//#include "../MouseOutput/MouseOutput.h"


#define ERROR 30

class IRSensor
{
	private:
		int pin, powPin;

	public:
		IRSensor( int sPin = 0, int pPin = 0, int w = 200, int m = 900 );
		
		int wDist; // offset
		int offset;
		int max;

		int rawDist();
		int dist();
		bool wall(); // Returns whether or not there is a wall
		bool tooClose(); // A failsafe, test to make sure not too close
		bool tooFar();
		void setWallDist( double d = -1, double m = 850 );
};




IRSensor::IRSensor( int sPin, int pPin, int w, int m )
: pin( sPin ), powPin( pPin ), offset(0), wDist(w)
{
	max = m; //Had weird problem initializing in init list
	if(powPin)
	{
		pinMode( powPin, OUTPUT );
		digitalWrite(powPin, HIGH);
	}
}

int IRSensor::rawDist()
{
	return analogRead(pin);
}

int IRSensor::dist()
{
	return rawDist() + offset;
}

bool IRSensor::wall()
{
	return dist() - ERROR < max;
}

bool IRSensor::tooClose()
{
	return rawDist() + ERROR < wDist;
}

bool IRSensor::tooFar()
{
	if( wall() ) return false;
	return rawDist() - ERROR > wDist;
}

void IRSensor::setWallDist( double d, double m )
{
	if( d == -1 ) wDist = rawDist();
	else wDist = (int)d;
	offset = 500 - wDist;
	//if( offset > 0 ) max = 700 + (150 * offset/90);
	//else max = 700 - 150 * offset/900;
	max = (int)m;// - offset/2;
	//max = wDist + 100;
}




















#undef ERROR
#endif
