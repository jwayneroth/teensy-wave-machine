#define OSCIL_MIN 1
#define OSCIL_MAX 150
#define MSECS 100

IntervalTimer oscilTimer;

volatile uint16_t lfo_one_count = SIN8192_NUM_CELLS / 2;
volatile uint16_t lfo_two_count = SIN8192_NUM_CELLS / 2;
volatile uint16_t lfo_three_count = SIN8192_NUM_CELLS / 2;
volatile uint16_t lfo_four_count = SIN8192_NUM_CELLS / 2;

uint8_t lfo_one_freq;
uint8_t lfo_two_freq;
uint8_t lfo_three_freq;
uint8_t lfo_four_freq;

uint8_t * lfo_frequencies[] = { &lfo_one_freq, &lfo_two_freq, &lfo_three_freq, &lfo_four_freq };

volatile uint8_t t2Count;
volatile char t2Over;


/*
 * setupTimer
*/
void setupTimer() {
	
	lfo_one_freq = OSCIL_MIN;
	lfo_two_freq = OSCIL_MIN;
	lfo_three_freq = OSCIL_MIN;
	lfo_four_freq = OSCIL_MIN;
	
	oscilTimer.begin(oscilFire, 100);
}

/*
 * setLFOFrequency
*/
void setLFOFrequency( int16_t val, uint8_t index ) {
	
	//Serial.println("_timer::setLFOFrequency");
	//Serial.print("val: ");Serial.print(val);
	//Serial.print("\t index: ");Serial.println(index);
	
	*lfo_frequencies[ index ]  =  map( val, 0, 1023, OSCIL_MIN, OSCIL_MAX );
	
	//lfo_one_freq = map( val, 0, 1023, OSCIL_MIN, OSCIL_MAX);
	
	//Serial.print("lfo_one_freq: ");Serial.println(lfo_one_freq);
	
}

/*
 * toggleLFOSync 
*/
void toggleLFOSync( int8_t index ) {
	
	if( drones[index]->getSynced() ) {
		
		lfo_vals[index] = &lfo_one_val;
	
	}else {
	
		lfo_vals[index] = lfo_vals_init[index];
	
	}

}

/*
 * oscilFire
*/
void oscilFire() {
  t2Count++;
  if (t2Count >= MSECS && !t2Over) {
		t2Over = 1;
		t2Count = 0;
		oscilNext();
		t2Over = 0;
	}
}

/*
 * oscilNext
*/
void oscilNext() {

	lfo_one_count += lfo_one_freq;
	if(lfo_one_count >= SIN8192_NUM_CELLS) lfo_one_count = 0;
	
	lfo_two_count += lfo_two_freq;
	if(lfo_two_count >= SIN8192_NUM_CELLS) lfo_two_count = 0;
	
	lfo_three_count += lfo_three_freq;
	if(lfo_three_count >= SIN8192_NUM_CELLS) lfo_three_count = 0;
	
	lfo_four_count += lfo_four_freq;
	if(lfo_four_count >= SIN8192_NUM_CELLS) lfo_four_count = 0;
	
}

/*
 * loopTimer
*/
void loopTimer() {
	
	uint16_t lfo1;
	uint16_t lfo2;
	uint16_t lfo3;
	uint16_t lfo4;

	noInterrupts();
	lfo1 = lfo_one_count;
	lfo2 = lfo_two_count;
	lfo3 = lfo_three_count;
	lfo4 = lfo_four_count;
	interrupts();

	lfo_one_val = SIN8192_DATA[lfo1];
	lfo_two_val = SIN8192_DATA[lfo2];
	lfo_three_val = SIN8192_DATA[lfo3];
	lfo_four_val = SIN8192_DATA[lfo4];
	
	//Serial.print("lfo1: ");
	//Serial.println(lfo1);
	//Serial.print("\t lfo_one_val: ");
	//Serial.println(lfo_one_val);
	
}
