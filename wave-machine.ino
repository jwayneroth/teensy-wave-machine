#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "sin8192_int8.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include "multi_pot.h"
#include "drone_voice.h"
#include "synth_sinesweep.h"

volatile int lfo_one_val;
volatile int lfo_two_val;
volatile int lfo_three_val;
volatile int lfo_four_val;

volatile int * lfo_vals[] = { &lfo_one_val, &lfo_two_val, &lfo_three_val, &lfo_four_val };
volatile int * lfo_vals_init[] = { &lfo_one_val, &lfo_two_val, &lfo_three_val, &lfo_four_val };

int8_t menu_mode = 0;
int8_t sub_menu_mode = 0;

void setup()   {                
  
	Serial.begin(9600);
	
	//while(!Serial){}
	
	setupDisplay();
	
	setupTimer();
	
	setupAudio();
	
	setupUI();
	
}

void loop() {
	
	loopUI();
	
	loopTimer();
	
	loopAudio();
	
	loopDisplay();
  
}





