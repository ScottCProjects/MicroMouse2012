
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

