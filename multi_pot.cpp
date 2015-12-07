#include "multi_pot.h"

#ifdef __AVR__
 #include <avr/pgmspace.h>
#else
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#define HYSTERESIS 10

/*
 * MultiPot
 * costructor
*/
MultiPot::MultiPot( int8_t pin, int8_t led, int8_t values ) {
	
	int8_t i;
	
	_pot_pin = pin;
	_led_pin = led;

	for(i=0; i<values;i++) {
		_values[i] =  0;
	}
	
	_current_pot = -1;
	_new_reading = 0;
	_locked = 0;
	
}

/*
 * read
 * if outside HYSTERESIS range
 *  if not locked save new reading
 * if val within HYSTERESIS range of last
 *  do nothing if not locked
 *  if locked, unlock
*/
void MultiPot::read( int8_t pot ) {
	
	if ( _current_pot != pot ) return;
	
	int16_t val = analogRead( _pot_pin );
	
	if (((val - _values[pot]) > HYSTERESIS) || ((_values[pot] - val) > HYSTERESIS)) {
		
		if( !_locked ) {
		
			_new_reading = 1;
		
			_values[pot] = val;
	
		}
		
	}else {
		
		_new_reading = 0;
		
		if( _locked ) {
			
			_locked = !_locked;
			
			digitalWrite( _led_pin, LOW );
		
		}
	
	}
	
}

/*
 * setPot
*/
void MultiPot::setPot( int8_t pot, int16_t val ) {
	if( _current_pot != pot ) {
		_values[pot] = val;
	}
}

/*
 * setCurrentPot
*/
void MultiPot::setCurrentPot( int8_t pot ) {
	
	Serial.println("MultiPot::setCurrentPot");
	
	if( _current_pot == pot) return;
	
	if(_locked) {
		_locked = 0;
		digitalWrite( _led_pin, LOW);
	}
	
	int16_t val = analogRead( _pot_pin );
	
	Serial.print("\t val: ");Serial.println(val);
	
	_new_reading = 0;
		
	Serial.print("\t last: ");Serial.println(_values[pot]);

	_current_pot = pot;
		
	if (((val - _values[pot]) > HYSTERESIS) || ((_values[pot] - val) > HYSTERESIS)) {
		
		Serial.println("\tlock");
		
		_locked = 1;
		
		digitalWrite( _led_pin, HIGH );
	
	}

}

/*
 * getCurrentPot
*/
int8_t MultiPot::getCurrentPot() {
	return _current_pot;
}

/*
 * hasNewReading
*/
boolean MultiPot::hasNewReading() {
	return _new_reading;
}

/*
 * getPot
*/
int16_t MultiPot::getPot( int8_t pot ) {
	return _values[pot];
}