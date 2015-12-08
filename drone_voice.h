#ifndef JWR_DRONE_SIGNAL
#define JWR_DRONE_SIGNAL

#include <Audio.h>
#include "synth_sinesweep.h"

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

class DroneVoice { 

	public:
		
		DroneVoice( 
		 AudioSynthNoisePink * pink, AudioSynthNoiseWhite * white, AudioSynthWaveform * sine, AudioSynthSineSweep * sweep,
		 AudioFilterStateVariable * pink_filter, AudioFilterStateVariable * white_filter, AudioFilterStateVariable * sine_filter,  AudioFilterStateVariable * sweep_filter,
		 AudioMixer4 * mixer );
		
		void
			setup(),
			setFrequency( uint16_t val ),
			setAmpMin( uint16_t val ),
			incrementSource(),
			setSource( int16_t source ),
			incrementDisplayMode(),
			setDisplayMode( int16_t mode ),
			incrementResonance(),
			setResonance( float val ),
			setAmpFactor( uint16_t val ),
			incrementCurve(),
			setCurve( int16_t curve ),
			incrementSweepFrequencyMax(),
			setSweepFrequencyMax(uint16_t freq),
			toggleSynced(),
			setSynced( boolean synced);
			
		uint16_t
			getAmp(),
			getFrequency(),
			getSweepFrequencyMax();
		
		int16_t
			getSource(),
			getCurve(),
			getDisplayMode();
			
		float
			getResonance(),
			computeNoise( int16_t lfo_val );
		
		boolean
		 getSynced();
		 
	private:
	
		float 
			_resonance,
			_amp_factor,
			_amp;
			
			
		uint16_t
			_filter_freq,
			_sweep_freq_max,
			_amp_min;
			
		int16_t
			_curve_exp,
			_source,
			_display_mode;
		
		boolean
			_synced;
			
		AudioSynthNoisePink*       _pink;
		AudioSynthNoiseWhite*      _white;
		AudioSynthWaveform*        _sine;
		AudioSynthSineSweep*        _sweep;
		AudioFilterStateVariable*  _pink_filter;
		AudioFilterStateVariable*  _white_filter;
		AudioFilterStateVariable*  _sine_filter;
		AudioFilterStateVariable*  _sweep_filter;
		AudioMixer4*               _mixer;
		           
};

#endif 