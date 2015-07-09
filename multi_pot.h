#ifndef JWR_MULTI_POT
#define JWR_MULTI_POT

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#define MAX_VALUES 6

class MultiPot { 

	public:
		
		MultiPot( int8_t pot, int8_t led, int8_t values );
		
		void
			setCurrentPot( int8_t pot ),
			read( int8_t pot),
			setPot( int8_t pot, int16_t val );
		
		boolean
			hasNewReading();
			
		int8_t
			getCurrentPot();
		
		int16_t 
			getPot( int8_t pot );
		
	protected:
		
		boolean
			_new_reading,
			_locked;
			
		int8_t
			_pot_pin, 
			_led_pin, 
			_current_pot;
			
		int16_t 
			_values[MAX_VALUES];
		
};
#endif 