#define VOL_MIN 0
#define VOL_MAX 70

#define SPEAKER_VOL_MIN 31
#define SPEAKER_VOL_MAX 13

#define RES_MIN 0.70
#define RES_MAX 5.0
#define RES_INC 0.30
#define FILTER_TWO_RES_MULT 0.50

#define FILTER_FREQ_MIN 100
#define FILTER_FREQ_MAX 2000
#define FILTER_FREQ_INC 100

#define SUB_EXP_MIN 1
#define SUB_EXP_MAX 5

#define SUB_FACTOR_MIN 30
#define SUB_FACTOR_MAX 100

#define AMP_MIN 0
#define AMP_MAX 100

#define AMP_RANGE_MIN 5
#define AMP_RANGE_MAX 100

AudioSynthNoisePink      pink;          
AudioSynthNoiseWhite     white;
AudioSynthWaveform       sine1;          
AudioSynthWaveform       sine2;
AudioSynthWaveform       sine3;          
AudioSynthWaveform       sine4;
AudioSynthSineSweep       sweep1;          
AudioSynthSineSweep       sweep2;
AudioSynthSineSweep       sweep3;          
AudioSynthSineSweep       sweep4;

AudioFilterStateVariable filter1;
AudioFilterStateVariable filter2;
AudioFilterStateVariable filter3;
AudioFilterStateVariable filter4;
AudioFilterStateVariable filter5;
AudioFilterStateVariable filter6;
AudioFilterStateVariable filter7;
AudioFilterStateVariable filter8;
AudioFilterStateVariable filter9;
AudioFilterStateVariable filter10;
AudioFilterStateVariable filter11;
AudioFilterStateVariable filter12;
AudioFilterStateVariable filter13;
AudioFilterStateVariable filter14;
AudioFilterStateVariable filter15;
AudioFilterStateVariable filter16;

AudioMixer4              mixer1;
AudioMixer4              mixer2;
AudioMixer4              mixer3;
AudioMixer4              mixer4;
AudioMixer4              mixer5;
AudioMixer4              mixer6;
AudioMixer4              mixer7;
AudioMixer4              mixer8;
AudioMixer4              mixer9;
AudioMixer4              mixer10;
AudioMixer4              mixer11;
AudioMixer4              mixer12;
AudioMixer4              mixer13;
AudioMixer4              mixer14;
AudioMixer4              mixer15;
AudioMixer4              mixer16;
AudioMixer4              mixer_signal1;
AudioMixer4              mixer_signal2;
AudioMixer4              mixer_signal3;
AudioMixer4              mixer_signal4;

AudioMixer4              mixer_last;
AudioOutputI2S           i2s1;

//pink source
AudioConnection          patchCord1(pink, 0, filter1, 0);
AudioConnection          patchCord2(pink, 0, filter2, 0);
AudioConnection          patchCord3(pink, 0, filter3, 0);
AudioConnection          patchCord4(pink, 0, filter4, 0);
AudioConnection          patchCord5(filter1, 0, mixer1, 0);
AudioConnection          patchCord6(filter1, 1, mixer1, 1);
//AudioConnection          patchCordHigh1(filter1, 2, mixer1, 1);
AudioConnection          patchCord7(filter2, 0, mixer2, 0);
AudioConnection          patchCord8(filter2, 1, mixer2, 1);
//AudioConnection          patchCordHigh2(filter2, 2, mixer2, 1);
AudioConnection          patchCord9(filter3, 0, mixer3, 0);
AudioConnection          patchCord10(filter3, 1, mixer3, 1);
//AudioConnection          patchCordHigh3(filter3, 2, mixer3, 1);
AudioConnection          patchCord11(filter4, 0, mixer4, 0);
AudioConnection          patchCord12(filter4, 1, mixer4, 1);
//AudioConnection          patchCordHigh4(filter4, 2, mixer4, 1);
AudioConnection          patchCord13(mixer1, 0, mixer_signal1, 0);
AudioConnection          patchCord14(mixer2, 0, mixer_signal2, 0);
AudioConnection          patchCord15(mixer3, 0, mixer_signal3, 0);
AudioConnection          patchCord16(mixer4, 0, mixer_signal4, 0);

//white source
AudioConnection          patchCord17(white, 0, filter5, 0);
AudioConnection          patchCord18(white, 0, filter6, 0);
AudioConnection          patchCord19(white, 0, filter7, 0);
AudioConnection          patchCord20(white, 0, filter8, 0);
AudioConnection          patchCord21(filter5, 0, mixer5, 0);
AudioConnection          patchCord22(filter5, 1, mixer5, 1);
AudioConnection          patchCord23(filter6, 0, mixer6, 0);
AudioConnection          patchCord24(filter6, 1, mixer6, 1);
AudioConnection          patchCord25(filter7, 0, mixer7, 0);
AudioConnection          patchCord26(filter7, 1, mixer7, 1);
AudioConnection          patchCord27(filter8, 0, mixer8, 0);
AudioConnection          patchCord28(filter8, 1, mixer8, 1);
AudioConnection          patchCord29(mixer5, 0, mixer_signal1, 1);
AudioConnection          patchCord30(mixer6, 0, mixer_signal2, 1);
AudioConnection          patchCord31(mixer7, 0, mixer_signal3, 1);
AudioConnection          patchCord32(mixer8, 0, mixer_signal4, 1);

//sine 1 source
AudioConnection          patchCord33(sine1, 0, filter9, 0);
//AudioConnection          patchCord34(filter9, 0, mixer9, 0);
AudioConnection          patchCord35(filter9, 1, mixer9, 1);
AudioConnection          patchCord36(mixer9, 0, mixer_signal1, 2);

//sine 2 source
AudioConnection          patchCord37(sine2, 0, filter10, 0);
//AudioConnection          patchCord38(filter10, 0, mixer10, 0);
AudioConnection          patchCord39(filter10, 1, mixer10, 1);
AudioConnection          patchCord40(mixer10, 0, mixer_signal2, 2);

//sine 3 source
AudioConnection          patchCord41(sine3, 0, filter11, 0);
//AudioConnection          patchCord42(filter11, 0, mixer11, 0);
AudioConnection          patchCord43(filter11, 1, mixer11, 1);
AudioConnection          patchCord44(mixer11, 0, mixer_signal3, 2);

//sine 4 source
AudioConnection          patchCord45(sine4, 0, filter12, 0);
//AudioConnection          patchCord46(filter12, 0, mixer12, 0);
AudioConnection          patchCord47(filter12, 1, mixer12, 1);
AudioConnection          patchCord48(mixer12, 0, mixer_signal4, 2);

//sweep 1 source
AudioConnection          patchCord49(sweep1, 0, filter13, 0);
//AudioConnection          patchCord50(filter13, 0, mixer13, 0);
AudioConnection          patchCord51(filter13, 1, mixer13, 1);
AudioConnection          patchCord52(mixer13, 0, mixer_signal1, 3);

//sweep 2 source
AudioConnection          patchCord53(sweep2, 0, filter14, 0);
//AudioConnection          patchCord54(filter14, 0, mixer14, 0);
AudioConnection          patchCord55(filter14, 1, mixer14, 1);
AudioConnection          patchCord56(mixer14, 0, mixer_signal2, 3);

//sweep 3 source
AudioConnection          patchCord57(sweep3, 0, filter15, 0);
//AudioConnection          patchCord58(filter15, 0, mixer15, 0);
AudioConnection          patchCord59(filter15, 1, mixer15, 1);
AudioConnection          patchCord60(mixer15, 0, mixer_signal3, 3);

//sweep 4 source
AudioConnection          patchCord61(sweep4, 0, filter16, 0);
//AudioConnection          patchCord62(filter16, 0, mixer16, 0);
AudioConnection          patchCord63(filter16, 1, mixer16, 1);
AudioConnection          patchCord64(mixer16, 0, mixer_signal4, 3);

AudioConnection          patchCordLast1(mixer_signal1, 0, mixer_last, 0);
AudioConnection          patchCordLast2(mixer_signal2, 0, mixer_last, 1);
AudioConnection          patchCordLast3(mixer_signal3, 0, mixer_last, 2);
AudioConnection          patchCordLast4(mixer_signal4, 0, mixer_last, 3);

AudioConnection          patchCordLast5(mixer_last, 0, i2s1, 0);
AudioConnection          patchCordLast6(mixer_last, 0, i2s1, 1);

AudioControlSGTL5000     audioShield;

DroneVoice drone1( &pink, &white, &sine1, &sweep1, &filter1, &filter5, &filter9, &filter13, &mixer_signal1 );
DroneVoice drone2( &pink, &white, &sine2, &sweep2, &filter2, &filter6, &filter10, &filter14, &mixer_signal2 );
DroneVoice drone3( &pink, &white, &sine3, &sweep3, &filter3, &filter7, &filter11, &filter15, &mixer_signal3 );
DroneVoice drone4( &pink, &white, &sine4, &sweep4, &filter4, &filter8, &filter12, &filter16, &mixer_signal4 );

DroneVoice *  drones[] = { &drone1, &drone2, &drone3, &drone4 };

float vol = 0.2;
float speaker_vol = 29;

/*
 * setupAudio
*/
void setupAudio() {
	
	AudioMemory(80);
	
	audioShield.enable();
	
	audioShield.volume( vol );
	
	audioShield.lineOutLevel( speaker_vol );
	
	audioShield.enhanceBassEnable();
	
	drone1.setup();
	drone2.setup();
	drone3.setup();
	drone4.setup();
	
} 

/*
 * setVolume
*/
void setVolume( int16_t val ) {
	
	float v = (float)map( val, 0, 1023, VOL_MIN, VOL_MAX ) / 100.0;
	
	audioShield.volume( v );
	
	audioShield.lineOutLevel( map(val, 0, 1023, SPEAKER_VOL_MIN, SPEAKER_VOL_MAX ) );
	
}

/*
 * volumeDown
*/
void volumeDown() {
	vol -= 0.1;
	if( vol < VOL_MIN) vol = VOL_MIN;
	audioShield.volume( vol );
	speaker_vol += 3;
	if(speaker_vol > SPEAKER_VOL_MIN) speaker_vol = SPEAKER_VOL_MIN;
	audioShield.lineOutLevel(speaker_vol);
}

/*
 * volumeUp
*/
void volumeUp() {
	vol += 0.1;
	if( vol > VOL_MAX) vol = VOL_MAX;
	audioShield.volume( vol );
	speaker_vol -= 3;
	if(speaker_vol < SPEAKER_VOL_MAX) speaker_vol = SPEAKER_VOL_MAX;
	audioShield.lineOutLevel(speaker_vol);
}

/*
 * loopAudio
*/
void loopAudio() {
	
	mixer_last.gain(0, drone1.computeNoise( *lfo_vals[0] ) );
	mixer_last.gain(1, drone2.computeNoise( *lfo_vals[1] ) );
	mixer_last.gain(2, drone3.computeNoise( *lfo_vals[2] ) );
	mixer_last.gain(3, drone4.computeNoise( *lfo_vals[3] ) );
	
	/*
	Serial.print("CPU: ");
	Serial.print("sweep1=");
	Serial.print(sweep1.processorUsage());
	Serial.print(", ");
	Serial.print(sweep1.processorUsageMax());
	Serial.print(" all=");
	Serial.print(AudioProcessorUsage());
	Serial.print(",");
	Serial.print(AudioProcessorUsageMax());
	Serial.print("\tMemory: ");
	Serial.print(AudioMemoryUsage());
	Serial.print(",");
	Serial.print(AudioMemoryUsageMax());
	Serial.println();
	*/
	
}
