#include "drone_voice.h"

#ifdef __AVR__
 #include <avr/pgmspace.h>
#else
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#define RES_MIN 0.70
#define RES_MAX 5.0
#define RES_INC 0.25

#define FILTER_FREQ_MIN 100
#define FILTER_FREQ_MAX 2000

#define SWEEP_FREQ_MIN_TOP 400
#define SWEEP_FREQ_MAX_BOTTOM 500

#define CURVE_EXP_MIN 1
#define CURVE_EXP_MAX 6

#define AMP_FACTOR_MIN 0
#define AMP_FACTOR_MAX 100

#define AMP_MIN_BOTTOM 0
#define AMP_MIN_TOP 150

#define AMP_MAX 200

#define SWEEP_FREQUENCY_MAX_BOTTOM 400
#define SWEEP_FREQUENCY_MAX_TOP 1600

#define SWEEP_INCREMENT_MIN 1
#define SWEEP_INCREMENT_MAX 200

/*
 * DroneVoice
*/
DroneVoice::DroneVoice( 
 AudioSynthNoisePink  * pink, 
 AudioSynthNoiseWhite * white, 
 AudioSynthWaveform   * sine, 
 AudioSynthSineSweep  * sweep,
 AudioFilterStateVariable * pink_filter, 
 AudioFilterStateVariable * white_filter, 
 AudioFilterStateVariable * sine_filter, 
 AudioFilterStateVariable * sweep_filter,
 AudioMixer4 * mixer ) { 
	
	_source = 0;
	_synced = false;
	_display_mode = 0;
	
	_pink = pink;
	_white = white;
	_sine = sine;
	_sweep = sweep;
	
	_pink_filter = pink_filter;
	_white_filter = white_filter;
	_sine_filter = sine_filter;
	_sweep_filter = sweep_filter;
	
	_mixer = mixer;
	
	_filter_freq = 1000;
	_resonance = 4.0;
	_curve_exp = CURVE_EXP_MIN;
	_amp_factor = AMP_FACTOR_MIN;
	_amp_min = AMP_MIN_BOTTOM;
	
	_mixer->gain(0, 1.0);
	_mixer->gain(1, 0);
	_mixer->gain(2, 0);
	_mixer->gain(3, 0);

	_pink_filter->frequency(_filter_freq);
	_white_filter->frequency(_filter_freq);
	_sine->frequency(_filter_freq);
	_sine_filter->frequency(_filter_freq);
	//_sweep->frequency(_filter_freq);
	//_sweep_filter->frequency(_filter_freq);
	
	_pink_filter->resonance(_resonance);
	_white_filter->resonance(_resonance);
	_sine_filter->resonance(_resonance);
	_sweep_filter->resonance(_resonance);
	
	_sweep_freq_max = 800;
	
}

/*
 * setup
*/
void DroneVoice::setup() {
	
	_sine->begin(WAVEFORM_SINE);
	_sweep->begin(WAVEFORM_SINE);//(WAVEFORM_TRIANGLE);
	
	_sweep->sweepFrequencyMax( _sweep_freq_max );
	
	//_pink->amplitude(.01);
	//_white->amplitude(.01);
	_sine->amplitude(0);
	_sweep->amplitude(0);
	
}

/*
 * setResonance
*/
/*void DroneVoice::setResonance( int16_t val ) {

	_resonance =  (float)map( val, 0, 1023, RES_MIN, RES_MAX );
	
	Serial.println("DroneVoice::setResonance");
	Serial.print("\t val: ");Serial.println(val);
	Serial.print("\t resonance: ");Serial.println(_resonance);
	
	_filter->resonance( _resonance );

}*/

/*
 * incrementResonance
*/
void DroneVoice::incrementResonance() {
	
	_resonance += RES_INC;
	
	if( _resonance > RES_MAX ) _resonance = RES_MIN;
	
	Serial.println("DroneVoice::incrementResonance");
	Serial.print("\t resonance: ");Serial.println(_resonance);
	
	_pink_filter->resonance( _resonance );
	_white_filter->resonance( _resonance );
	_sine_filter->resonance( _resonance );
	_sweep_filter->resonance( _resonance );
	
}

/*
 * incrementSource
*/
void DroneVoice::incrementSource() {
	
	_mixer->gain(_source, 0);
	
	_source++;
	
	if( _source > 3 ) _source = 0;
	
	if( _source == 2 ) {
	
		_sine->amplitude(.01);
		_sweep->amplitude(0);
	
	}else if( _source == 3 ) {
	
		_sine->amplitude(0);
		_sweep->amplitude(.01);
	
	}else {
	
		_sine->amplitude(0);
		_sweep->amplitude(0);
	
	}
	
	Serial.println("DroneVoice::incrementSource");
	Serial.print("\t _source: ");Serial.println(_source);
	
	_mixer->gain(_source, 1.0);
	
}

/*
 * setFrequency
*/
void DroneVoice::setFrequency( int16_t val ) {
	
	_filter_freq = map( val, 0, 1023, FILTER_FREQ_MIN, FILTER_FREQ_MAX );
	
	Serial.println("DroneVoice::setFrequency");
	Serial.print("\t val: ");Serial.println(val);
	Serial.print("\t freq: ");Serial.println(_filter_freq);
	
	_pink_filter->frequency( _filter_freq );
	_white_filter->frequency( _filter_freq );
	
	_sine_filter->frequency( _filter_freq );
	_sine->frequency( _filter_freq );
	
	//_sweep_filter->frequency( _filter_freq );
	//_sweep->frequency( _filter_freq );
	
	if( _source == 3 ) _sweep->sweepIncrement( map( val, 0, 1023, SWEEP_INCREMENT_MIN, SWEEP_INCREMENT_MAX ));

	
}

/*
 * setAmpFactor
*/
void DroneVoice::setAmpFactor( int16_t val ) {
	
	_amp_factor = (float)( map( val, 0, 1023, AMP_FACTOR_MIN, AMP_FACTOR_MAX ) / 100.00 );
	
	Serial.println("DroneVoice::setAmpFactor");
	Serial.print("\t val: ");Serial.println(val);
	Serial.print("\t amp_factor: ");Serial.println(_amp_factor);

}

/*
 * setAmpMin
*/
void DroneVoice::setAmpMin( int16_t val ) {
	
	_amp_min = map( val, 0, 1023, AMP_MIN_BOTTOM, AMP_MIN_TOP );
	
	Serial.println("DroneVoice::setAmpMin");
	Serial.print("\t val: ");Serial.println(val);
	Serial.print("\t amp_min: ");Serial.println(_amp_min);
	
}

/*
 * getAmpMax
*/
uint16_t DroneVoice::getAmp() {
	
	return _amp;//_amp_min + ( AMP_MAX - _amp_min ) * _amp_factor;
	
}

/*
 * getSource
*/
int16_t DroneVoice::getSource() {
	
	return _source;
	
}

/*
 * getResonance
*/
float DroneVoice::getResonance() {
	
	return _resonance;
	
}

/*
 * getCurve
*/
int16_t DroneVoice::getCurve() {
	
	return _curve_exp;
	
}

/*
 * getDisplayMode
*/
int16_t DroneVoice::getDisplayMode() {
	
	return _display_mode;
	
}

/*
 * getFrequency
*/
uint16_t DroneVoice::getFrequency() {
	
	return _filter_freq;
	
}

/*
 * setCurve
*/
/*void DroneVoice::setCurve( int16_t val ) {
	
	_curve_exp = map( val, 0, 1023, CURVE_EXP_MIN, CURVE_EXP_MAX );
	
	Serial.println("DroneVoice::setCurve");
	Serial.print("\t val: ");Serial.println(val);
	Serial.print("\t curve: ");Serial.println(_curve_exp);
	
}*/

/*
 * incrementCurve
*/
void DroneVoice::incrementCurve() {
	
	_curve_exp++;
	
	if( _curve_exp > CURVE_EXP_MAX ) _curve_exp = CURVE_EXP_MIN;
	
	Serial.println("DroneVoice::incrementCurve");
	Serial.print("\t curve: ");Serial.println(_curve_exp);
	
}

/*
 * toggleSynced
*/
void DroneVoice::toggleSynced() {

	_synced = !_synced;

}

/*
 * getSynced
*/
boolean DroneVoice::getSynced() {

	return _synced;

}

/*
 * incrementDisplayMode
*/
void DroneVoice::incrementDisplayMode() {

	_display_mode++;
	
	if(_display_mode > 1) _display_mode = 0;

}

/*
 * setSweepFrequencyMax
*/
void DroneVoice::incrementSweepFrequencyMax() {
	
	_sweep_freq_max += 200;
	if( _sweep_freq_max > SWEEP_FREQUENCY_MAX_TOP ) _sweep_freq_max = SWEEP_FREQUENCY_MAX_BOTTOM;
	
	_sweep->sweepFrequencyMax( _sweep_freq_max );
	
}

/*
 * getSweepFrequencyMax
*/
uint16_t DroneVoice::getSweepFrequencyMax() {
	
	return _sweep_freq_max;
	
}

/*
 * computeNoise
*/
float DroneVoice::computeNoise( int16_t lfo_val ) {
	
	float curve_pct = (float)map( lfo_val, -128, 128, 0, 100 ) / 100.0; 
	
	_amp = _amp_min + ( pow(curve_pct, _curve_exp) ) * ( AMP_MAX - _amp_min ) * _amp_factor;
	
	//Serial.println("DroneVoice::computeNoise");
	//Serial.print("\t lfo: ");Serial.println(lfo_val);
	//Serial.print("\t _resonance: ");Serial.println(_resonance);
	//Serial.print("\t _amp_min: ");Serial.println(_amp_min);
	//Serial.print("\t _amp_factor: ");Serial.println(_amp_factor);
	//Serial.print("\t amp: ");Serial.println(amp);
	
	//if( _source == 3 ) computeSweep( lfo_val );
	if( _source == 3 ) _sweep->updateSweep();
	
	return _amp;

}
