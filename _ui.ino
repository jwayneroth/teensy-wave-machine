#define POT_ONE 2    //main frequency
#define POT_TWO 3    //range
#define POT_THREE 6  //resonance //volume
#define POT_FOUR 7   //filter frequency

#define BTN_ONE 12
#define BTN_TWO 5
#define BTN_THREE 7 
#define BTN_FOUR 14

#define LED_ONE 10
#define LED_TWO 8
#define LED_THREE 6
#define LED_FOUR 15

#define MENU_MODE_TOTAL 5

MultiPot pot_one( POT_ONE, LED_ONE, 6 );
MultiPot pot_two( POT_TWO, LED_TWO, 6 );
MultiPot pot_three( POT_THREE, LED_THREE, 6 );
MultiPot pot_four( POT_FOUR, LED_FOUR, 6 );

MultiPot * multi_pots[] = { &pot_one, &pot_two, &pot_three, &pot_four };

boolean button_one_pressed = 0;
boolean button_two_pressed = 0;
boolean button_three_pressed = 0;
boolean button_four_pressed = 0;

/*
 * setupUI
*/
void setupUI() {
	
	pinMode(BTN_ONE, INPUT);
	pinMode(BTN_TWO, INPUT);
	pinMode(BTN_THREE, INPUT);
	pinMode(BTN_FOUR, INPUT);
	
	pinMode(LED_ONE, OUTPUT);
	pinMode(LED_TWO, OUTPUT);
	pinMode(LED_THREE, OUTPUT);
	pinMode(LED_FOUR, OUTPUT);
	
	loadUIState();
	
	pot_one.setCurrentPot(menu_mode);
	pot_two.setCurrentPot(menu_mode);
	pot_three.setCurrentPot(menu_mode);
	pot_four.setCurrentPot(menu_mode);
	
}

/*
 * loopUI
*/
void loopUI() {
	
	checkButtons();
	
	checkPots();
	
}


/*
 * checkButtons
*/
void checkButtons() {
	
	if( digitalRead(BTN_ONE) == HIGH ) { button_one_pressed = 1; }
	else {
		if(button_one_pressed) {
			
			Serial.println("one pressed: ");
			
			incrementMenuMode();
			
			button_one_pressed = 0;
		
		}
	}
	
	if(menu_mode < 4) {
		
		checkSignalMenu();
	
	}else {
	
		checkMenuButtons();
	
	}

}

/*
 * checkSignalMenu
*/
void checkSignalMenu() {

	if( digitalRead(BTN_TWO) == HIGH ) { button_two_pressed = 1; }
	else {
		if(button_two_pressed) {
		
			Serial.println("two pressed: ");
			
			incrementSubMenuMode();
			
			button_two_pressed = 0;
		
		}
	}
	
	if(sub_menu_mode == 0) {
	
		signalMenuOne();
	
	}else if(sub_menu_mode == 1) {
		
		signalMenuTwo();
		
	}else {
		
		signalMenuThree();
		
	}
	
}

/*
 * signalMenuOne
*/
void signalMenuOne() {

	if( digitalRead(BTN_THREE) == HIGH ) { button_three_pressed = 1; }
	else {
		if(button_three_pressed) {

			Serial.println("three pressed: ");
			
			drones[menu_mode]->incrementSource();
			
			button_three_pressed = 0;
		
		}
	}
	
	if( digitalRead(BTN_FOUR) == HIGH ) { button_four_pressed = 1; }
	else {
		if(button_four_pressed) {
			
			Serial.println("four pressed: ");
			
			drones[menu_mode]->toggleSynced();
			
			toggleLFOSync( menu_mode );
			
			button_four_pressed = 0;
		
		}

	}
	
}

/*
 * signalMenuTwo
*/
void signalMenuTwo() {

	if( digitalRead(BTN_THREE) == HIGH ) { button_three_pressed = 1; }
	else {
		if(button_three_pressed) {

			Serial.println("three pressed: ");
			
			drones[menu_mode]->incrementResonance();
			
			button_three_pressed = 0;
		
		}
	}
	
	if( digitalRead(BTN_FOUR) == HIGH ) { button_four_pressed = 1; }
	else {
		if(button_four_pressed) {
			
			Serial.println("four pressed: ");
			
			drones[menu_mode]->incrementCurve();
			
			button_four_pressed = 0;
		
		}

	}
	
}

/*
 * signalMenuThree
*/
void signalMenuThree() {

	if( digitalRead(BTN_THREE) == HIGH ) { button_three_pressed = 1; }
	else {
		if(button_three_pressed) {

			Serial.println("three pressed: ");
			
			drones[menu_mode]->incrementDisplayMode();
			
			button_three_pressed = 0;
		
		}
	}
	
	if( digitalRead(BTN_FOUR) == HIGH ) { button_four_pressed = 1; }
	else {
		if(button_four_pressed) {
			
			if( drones[menu_mode]->getSource() == 3 ) drones[menu_mode]->incrementSweepFrequencyMax();
			
			Serial.println("four pressed: ");
			
			button_four_pressed = 0;
		
		}

	}
	
}

/*
 * checkMenuButtons
*/
void checkMenuButtons() {

	if( digitalRead(BTN_TWO) == HIGH ) { button_two_pressed = 1; }
	else {
		if(button_two_pressed) {
		
			Serial.println("two pressed: ");
			
			saveUIState();
			
			button_two_pressed = 0;
		
		}
	}
	/*
	if( digitalRead(BTN_THREE) == HIGH ) { button_three_pressed = 1; }
	else {
		if(button_three_pressed) {

			Serial.println("three pressed: ");
			
			
			button_three_pressed = 0;
		
		}
	}
	
	if( digitalRead(BTN_FOUR) == HIGH ) { button_four_pressed = 1; }
	else {
		if(button_four_pressed) {
			
			Serial.println("four pressed: ");
		
			button_four_pressed = 0;
		
		}
	}
	*/
}

/*
 * checkPots
*/
void checkPots() {
	
	pot_one.read( menu_mode );
	pot_two.read( menu_mode );
	pot_three.read( menu_mode );
	pot_four.read( menu_mode );
	
	if(menu_mode < 4) {
		
		droneSignalSettings();
		
	}else if(menu_mode == 4) {
	
		menuSettings();
	
	}
	
}

/*
 * droneSignalSettings
*/
void droneSignalSettings() {
	
	if ( pot_one.hasNewReading() == 1 ) {
		
		setLFOFrequency( pot_one.getPot(menu_mode), menu_mode );
		
	}
	
	if ( pot_two.hasNewReading() == 1  ) {
		
		drones[menu_mode]->setAmpMin( pot_two.getPot(menu_mode) );
		
	}
	
	if ( pot_three.hasNewReading() == 1  ) {
		
		drones[menu_mode]->setAmpFactor( pot_three.getPot(menu_mode) );
		
	}
	
	if ( pot_four.hasNewReading() == 1  ) {
		
		drones[menu_mode]->setFrequency( pot_four.getPot(menu_mode) );
	
	}
	
}


/*
 * saveUIState
*/
void saveUIState() {
	
	Serial.println("saveUIState");
	
	uint16_t i, e1, e2, e3, e4;
	uint16_t e5, e6, e7, e8, e9, e10;
	
	// save vals for all drone voices
	for( i=0; i<4; i++) {
		
		// pot vals
		e1 = pot_one.getPot(i);
		e2 = pot_two.getPot(i);
		e3 = pot_three.getPot(i);
		e4 = pot_four.getPot(i);
		
		EEPROM.write(( i * 4 + 0 ), e1/4 );
		EEPROM.write(( i * 4 + 1 ), e2/4 );
		EEPROM.write(( i * 4 + 2 ), e3/4 );
		EEPROM.write(( i * 4 + 3 ), e4/4 );
		
		Serial.print("\t voice ");Serial.print(i);Serial.print(" [");
		Serial.print(e1);Serial.print(" , ");
		Serial.print(e2);Serial.print(" , ");
		Serial.print(e3);Serial.print(" , ");
		Serial.print(e4);Serial.println(" ]");
		
		// TODO:: button vals: source, res, curve, display mode, sync, sweep max, 
		/*
		e5 = drones[i].getSource();
		e6 = drones[i].getResonance();
		e7 = drones[i].getCurve();
		e8 = drones[i].getDisplayMode();
		e9 = drones[i].getSync();
		e10 = drones[i].getSweepFrequencyMax();
		
		EEPROM.write(( i * 4 + 0 ), e5/4 );
		EEPROM.write(( i * 4 + 1 ), e6/4 );
		EEPROM.write(( i * 4 + 2 ), e7/4 );
		EEPROM.write(( i * 4 + 3 ), e8/4 );
		EEPROM.write(( i * 4 + 3 ), e9/4 );
		EEPROM.write(( i * 4 + 3 ), e10/4 );
		*/
	}
}

/*
 * loadUIState
 * loop over saved pot values
*/
void loadUIState() {
	
	Serial.println("loadUIState");
	
	uint16_t i, e1, e2, e3, e4;
	
	//load pot vals for all drone voices
	for( i=0; i<4; i++) {
		
		e1 = EEPROM.read( i * 4 + 0 ) * 4;
		e2 = EEPROM.read( i * 4 + 1 ) * 4;
		e3 = EEPROM.read( i * 4 + 2 ) * 4;
		e4 = EEPROM.read( i * 4 + 3 ) * 4;
		
		pot_one.setPot( i, e1 );
		pot_two.setPot( i, e2 );
		pot_three.setPot( i, e3 );
		pot_four.setPot( i, e4 );
		
		///*
		Serial.print("\t voice ");Serial.print(i);Serial.print(" [");
		Serial.print(e1);Serial.print(" , ");
		Serial.print(e2);Serial.print(" , ");
		Serial.print(e3);Serial.print(" , ");
		Serial.print(e4);Serial.println(" ]");
		//*/
		
		setLFOFrequency( e1, ( i ));
		drones[ i ]->setAmpMin( e2 );
		drones[ i ]->setAmpFactor( e3 );
		drones[ i ]->setFrequency( e4 );
	
	}
	
}

/*
 * incrementMenuMode
*/
void incrementMenuMode() {
	
	menu_mode++;
	
	if( menu_mode >= MENU_MODE_TOTAL ) menu_mode = 0;
	
	pot_one.setCurrentPot(menu_mode);
	pot_two.setCurrentPot(menu_mode);
	pot_three.setCurrentPot(menu_mode);
	pot_four.setCurrentPot(menu_mode);
	
	sub_menu_mode = 0;
	
	Serial.print("incrementMenuMode: ");Serial.println(menu_mode);

}

/*
 * incrementSubMenuMode
*/
void incrementSubMenuMode() {
	
	sub_menu_mode++;
	
	if( sub_menu_mode > 2 ) sub_menu_mode = 0;
	
	Serial.print("incrementSubMenuMode: ");Serial.println(sub_menu_mode);

}

/*
 * menuSettings
*/
void menuSettings() {
	
	if ( pot_one.hasNewReading() == 1 ) {
		
		setVolume( pot_one.getPot( menu_mode ) );
		
	}
	/*
	if ( pot_two.hasNewReading() == 1  ) {
		
		
	}
	
	if ( pot_three.hasNewReading() == 1  ) {
		
		
	}
	
	if ( pot_four.hasNewReading() == 1  ) {
		
		
	}
	*/
}