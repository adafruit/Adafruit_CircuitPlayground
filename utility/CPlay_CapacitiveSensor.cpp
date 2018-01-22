/*
 CapacitiveSense.h v.04 - Capacitive Sensing Library for 'duino / Wiring
 https://github.com/PaulStoffregen/CapacitiveSensor
 http://www.pjrc.com/teensy/td_libs_CapacitiveSensor.html
 http://playground.arduino.cc/Main/CapacitiveSensor
 Copyright (c) 2009 Paul Bagder  All right reserved.
 Version 05 by Paul Stoffregen - Support non-AVR board: Teensy 3.x, Arduino Due
 Version 04 by Paul Stoffregen - Arduino 1.0 compatibility, issue 146 fix
 vim: set ts=4:
 */

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif

#include "CPlay_CapacitiveSensor.h"

/**************************************************************************/
/*! 
    @brief  Function that handles the creation and setup of instances
    @param sendPin send pin for the sensor
    @param receivePin the receiving pin for the sensor
*/
/**************************************************************************/
CPlay_CapacitiveSensor::CPlay_CapacitiveSensor(uint8_t sendPin, uint8_t receivePin)
{
	// initialize this instance's variables
	// Serial.begin(9600);		// for debugging
	error = 1;
	loopTimingFactor = 300;		// determined empirically -  a hack

	CS_Timeout_Millis = (2000 * (float)loopTimingFactor * (float)F_CPU) / 16000000;
	CS_AutocaL_Millis = 20000;

	// Serial.print("timwOut =  ");
	// Serial.println(CS_Timeout_Millis);
	// get pin mapping and port for send Pin - from PinMode function in core

#ifdef NUM_DIGITAL_PINS
	if (sendPin >= NUM_DIGITAL_PINS) error = -1;
	if (receivePin >= NUM_DIGITAL_PINS) error = -2;
#endif
	
	// sendpin to OUTPUT
#ifdef ARDUINO_TEEONARDU_FLORA
	// terrible hack!
	DDRD |= _BV(5);
#else
	pinMode(sendPin, OUTPUT);
#endif

	pinMode(receivePin, INPUT);						// receivePin to INPUT

#ifdef ARDUINO_TEEONARDU_FLORA
	// terrible hack!
	PORTD &= ~_BV(5);
#else
	digitalWrite(sendPin, LOW);
#endif

	sBit = PIN_TO_BITMASK(sendPin);					// get send pin's ports and bitmask
	sReg = PIN_TO_BASEREG(sendPin);					// get pointer to output register

	rBit = PIN_TO_BITMASK(receivePin);				// get receive pin's ports and bitmask
	rReg = PIN_TO_BASEREG(receivePin);

	// get pin mapping and port for receive Pin - from digital pin functions in Wiring.c
	leastTotal = 0x0FFFFFFFL;   // input large value for autocalibrate begin
	lastCal = millis();         // set millis for start
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries


/**************************************************************************/
/*! 
    @brief  get a capacitive sensor reading
    @param samples number of samples to take
    @return the sensor reading
*/
/**************************************************************************/
long CPlay_CapacitiveSensor::capacitiveSensor(uint8_t samples)
{
	total = 0;
	if (samples == 0) return 0;
	if (error < 0) return -1;            // bad pin


	for (uint8_t i = 0; i < samples; i++) {    // loop for samples parameter - simple lowpass filter
		if (SenseOneCycle() < 0)  return -2;   // variable over timeout
}

		// only calibrate if time is greater than CS_AutocaL_Millis and total is less than 10% of baseline
		// this is an attempt to keep from calibrating when the sensor is seeing a "touched" signal

		if ( (millis() - lastCal > CS_AutocaL_Millis) && abs(total  - leastTotal) < (int)(.10 * (float)leastTotal) ) {

			// Serial.println();               // debugging
			// Serial.println("auto-calibrate");
			// Serial.println();
			// delay(2000); */

			leastTotal = 0x0FFFFFFFL;          // reset for "autocalibrate"
			lastCal = millis();
		}
		/*else{                                // debugging
			Serial.print("  total =  ");
			Serial.print(total);

			Serial.print("   leastTotal  =  ");
			Serial.println(leastTotal);

			Serial.print("total - leastTotal =  ");
			x = total - leastTotal ;
			Serial.print(x);
			Serial.print("     .1 * leastTotal = ");
			x = (int)(.1 * (float)leastTotal);
			Serial.println(x);
		} */

	// routine to subtract baseline (non-sensed capacitance) from sensor return
	if (total < leastTotal) leastTotal = total;                 // set floor value to subtract from sensed value
	return(total - leastTotal);

}

/**************************************************************************/
/*! 
    @brief  get a raw sensor reading
    @param samples the number of samples to take
    @return -1 for error, -2 for timeout, other values are a raw sensor reading
*/
/**************************************************************************/
long CPlay_CapacitiveSensor::capacitiveSensorRaw(uint8_t samples)
{
	total = 0;
	if (samples == 0) return 0;
	if (error < 0) return -1;                  // bad pin - this appears not to work

	for (uint8_t i = 0; i < samples; i++) {    // loop for samples parameter - simple lowpass filter
		if (SenseOneCycle() < 0)  return -2;   // variable over timeout
	}

	return total;
}

/**************************************************************************/
/*! 
    @brief  reset the auto calibration
*/
/**************************************************************************/
void CPlay_CapacitiveSensor::reset_CS_AutoCal(void){
	leastTotal = 0x0FFFFFFFL;
}

/**************************************************************************/
/*! 
    @brief set the auto-calibration time
    @param autoCal_millis the desired calibration time in milliseconds
*/
/**************************************************************************/
void CPlay_CapacitiveSensor::set_CS_AutocaL_Millis(unsigned long autoCal_millis){
	CS_AutocaL_Millis = autoCal_millis;
}

/**************************************************************************/
/*! 
    @brief  set the sensor timeout
    @param timeout_millis the number of milliseconds to set the timeout to
*/
/**************************************************************************/
void CPlay_CapacitiveSensor::set_CS_Timeout_Millis(unsigned long timeout_millis){
	CS_Timeout_Millis = (timeout_millis * (float)loopTimingFactor * (float)F_CPU) / 16000000;  // floats to deal with large numbers
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

/**************************************************************************/
/*! 
    @brief  sense a single sicle
    @return the reading
*/
/**************************************************************************/
int CPlay_CapacitiveSensor::SenseOneCycle(void)
{
    noInterrupts();
	DIRECT_WRITE_LOW(sReg, sBit);	// sendPin Register low
	DIRECT_MODE_INPUT(rReg, rBit);	// receivePin to input (pullups are off)
	DIRECT_MODE_OUTPUT(rReg, rBit); // receivePin to OUTPUT
	DIRECT_WRITE_LOW(rReg, rBit);	// pin is now LOW AND OUTPUT
	delayMicroseconds(10);
	DIRECT_MODE_INPUT(rReg, rBit);	// receivePin to input (pullups are off)
	DIRECT_WRITE_HIGH(sReg, sBit);	// sendPin High
    interrupts();

	while ( !DIRECT_READ(rReg, rBit) && (total < CS_Timeout_Millis) ) {  // while receive pin is LOW AND total is positive value
		total++;
	}
	//Serial.print("SenseOneCycle(1): "); Serial.println(total);

	if (total > CS_Timeout_Millis) {
		return -2;         //  total variable over timeout
	}

	// set receive pin HIGH briefly to charge up fully - because the while loop above will exit when pin is ~ 2.5V
    noInterrupts();
	DIRECT_WRITE_HIGH(rReg, rBit);
	DIRECT_MODE_OUTPUT(rReg, rBit);  // receivePin to OUTPUT - pin is now HIGH AND OUTPUT
	DIRECT_WRITE_HIGH(rReg, rBit);
	DIRECT_MODE_INPUT(rReg, rBit);	// receivePin to INPUT (pullup is off)
	DIRECT_WRITE_LOW(sReg, sBit);	// sendPin LOW
    interrupts();

#ifdef FIVE_VOLT_TOLERANCE_WORKAROUND
	DIRECT_MODE_OUTPUT(rReg, rBit);
	DIRECT_WRITE_LOW(rReg, rBit);
	delayMicroseconds(10);
	DIRECT_MODE_INPUT(rReg, rBit);	// receivePin to INPUT (pullup is off)
#else
	while ( DIRECT_READ(rReg, rBit) && (total < CS_Timeout_Millis) ) {  // while receive pin is HIGH  AND total is less than timeout
		total++;
	}
#endif
	//Serial.print("SenseOneCycle(2): ");
	//Serial.println(total);

	if (total >= CS_Timeout_Millis) {
		return -2;     // total variable over timeout
	} else {
		return 1;
	}
}
