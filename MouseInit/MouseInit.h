#ifndef _MOUSEINIT_H_
#define _MOUSEINIT_H_

#include "../MouseMovement/MouseMovement.h"
#include "../MouseIR/MouseIR.h"
#include "../MouseMotor/MouseMotor.h"

// -------------- Motors, Sensors and Pins ------------------

#define leftMPinA 6
#define leftMPinB 5
#define leftMEncA 1
#define leftMEncB 2
#define rightMPinA 11
#define rightMPinB 10
#define rightMEncA 3
#define rightMEncB 4

Motor leftM( leftMPinA, leftMPinB,
						leftMEncA, leftMEncB );
Motor rightM( rightMPinA, rightMPinB,
							rightMEncA, rightMEncB );

#define leftIRPin A1
#define rightIRPin A2
#define frontIRPin A0
#define backIRPin A3
#define IRPowerPin 13
IRSensor leftIR( leftIRPin, IRPowerPin );
IRSensor rightIR( rightIRPin, IRPowerPin );
IRSensor frontIR( frontIRPin, IRPowerPin );
IRSensor backIR( backIRPin, IRPowerPin );

















#endif
