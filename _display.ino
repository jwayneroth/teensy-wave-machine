#define OLED_DC 2 //11
#define OLED_CS 1 //12
#define OLED_CLK 4 //10
#define OLED_MOSI 3 //9
#define OLED_RESET 0 //13

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define PI 3.141592654

#define VPX 64
#define VPY 16
#define FOCAL_LENGTH 150

#define BASE_HEIGHT 8
#define BASE_WIDTH 120
#define BASE_DEPTH 100

#define BASE_CENTER ( BASE_DEPTH / 2 )
#define BASE_TOP 39
#define BASE_BOTTOM ( BASE_TOP + BASE_HEIGHT )
#define BASE_LEFT ( 0 - BASE_WIDTH /2 )
#define BASE_RIGHT ( BASE_WIDTH / 2 )
#define BASE_FRONT ( 0 - BASE_DEPTH / 2 )
#define BASE_BACK ( BASE_DEPTH / 2 )

#define BASE_LINES_TOTAL 16

#define WAVE_DIV 1
#define WAVE_POINTS_TOTAL ( BASE_WIDTH / WAVE_DIV + 1 ) * 6

#define WAVE_RES_MIN 12
#define WAVE_RES_MAX 1

#define WAVE_HEIGHT_MIN_BOTTOM 1
#define WAVE_HEIGHT_MIN_TOP 12
#define WAVE_HEIGHT_MAX_BOTTOM 18
#define WAVE_HEIGHT_MAX_TOP 40
#define WAVE_HEIGHT_MAX 40
#define WAVE_WIDTH_MULT_MIN 1
#define WAVE_WIDTH_MULT_MAX 4
#define WAVE_WIDTH_MIN 20

#define WAVE_Y BASE_TOP

uint8_t wave_min_width =  WAVE_WIDTH_MIN;

uint8_t wave_min_height = WAVE_HEIGHT_MIN_BOTTOM;
uint8_t wave_max_height = WAVE_HEIGHT_MAX_BOTTOM;

int wave_resolution = 2;
int wave_height = wave_max_height;

int wave_width = WAVE_WIDTH_MIN;
int wave_width_mult = WAVE_WIDTH_MULT_MIN;

int wave_x = BASE_LEFT;
int wave_center_offset = 0;
int wave_center_offset_max = 0 - wave_width / 2;
int wave_points[WAVE_POINTS_TOTAL];
int base_lines[BASE_LINES_TOTAL];

char * source_names[] = { "PINK", "WHITE", "SINE", "SWEEP" };

float wave_height_range = 1.00;

/*
 * setupDisplay
*/
void setupDisplay() {

	display.begin( SSD1306_SWITCHCAPVCC );
	
	display.clearDisplay();
	
	display.setTextSize(1);
  
  display.setTextColor(WHITE);
  
	initBase();
	
	initWave();
	
	display.display();
	
}

/*
 * loopDisplay
*/
void loopDisplay() {
	
	display.clearDisplay();
	
	displayMenu();
	
	renderWave();
	
	display.display();
	
}

/*
 * displayMenu 
*/
void displayMenu() {
	
	if(menu_mode == 4) {
		
		if(save_flag) {
			display.setCursor(0, 57);
			display.print("save?");
		}
		if(reset_flag) {
			display.setCursor(32, 57);
			display.print("reset?");
		}
		if(clear_flag) {
			display.setCursor(70, 57);
			display.print("clear?");
		}
		
	}else {
		
		display.setCursor( 0, 57 );
		display.print('V');
		display.print((menu_mode + 1));
		
		display.setCursor(32, 57);
		display.print('M');
		display.print((sub_menu_mode + 1));
		
		switch( sub_menu_mode ) {
			
			case 0 :
		
			display.setCursor(62, 57);
			display.print( source_names[ drones[menu_mode]->getSource() ] );
		
			display.setCursor(104, 57);
			if( drones[menu_mode]->getSynced() ) {
			
				display.setTextColor(BLACK, WHITE);
				display.print("SYNC");
				display.setTextColor(WHITE);
		
			}else {
		
				display.print("SYNC");
		
			}
			
			break;
			
			case 1:
			
			display.setCursor(64, 57);
			display.print('R');
			display.print( drones[menu_mode]->getResonance() );
		
			display.setCursor(114, 57);
			display.print('C');
			display.print( drones[menu_mode]->getCurve() );

			break;
				
			case 2:
			
			display.setCursor(64, 57);
			display.print('D');
			display.print(( drones[menu_mode]->getDisplayMode() + 1 ));
			
			if( drones[menu_mode]->getSource() == 3 ) {
				display.setCursor(90, 57);
				display.print("SM");
				display.print(( drones[menu_mode]->getSweepFrequencyMax() ));
			}

			break;

		}
	}
}

/*
 * setWaveHeight
*/
void setWaveHeight( float range_pct ) {
	
	wave_height_range = range_pct;
	
	wave_min_height = WAVE_HEIGHT_MIN_BOTTOM + range_pct * (WAVE_HEIGHT_MIN_TOP - WAVE_HEIGHT_MIN_BOTTOM);
	
	wave_max_height = WAVE_HEIGHT_MAX_BOTTOM + range_pct * (WAVE_HEIGHT_MAX_TOP - WAVE_HEIGHT_MAX_BOTTOM);
	
	//wave_min_height = map( amp_range, AMP_RANGE_MIN, AMP_RANGE_MAX, WAVE_HEIGHT_MIN_BOTTOM, WAVE_HEIGHT_MIN_TOP );
	//wave_max_height = map( amp_range, AMP_RANGE_MIN, AMP_RANGE_MAX, WAVE_HEIGHT_MAX_BOTTOM, WAVE_HEIGHT_MAX_TOP );
	
	//Serial.print("wave_min_height: ");Serial.println(wave_min_height);
	//Serial.print("wave_max_height: ");Serial.println(wave_max_height);
		
}

/*
 * setWaveResolution
*/
void setWaveResolution( int16_t val ) {
	
	wave_resolution = map( val, 0, 1023, WAVE_RES_MIN, WAVE_RES_MAX);
	
	Serial.print("wave_resolution: ");Serial.println(wave_resolution);
	
}

/*
 * setWaveWidth
*/
void setWaveWidth( float freq ) {
	
	wave_width_mult = WAVE_WIDTH_MULT_MIN + (1 - freq) * (WAVE_WIDTH_MULT_MAX - WAVE_WIDTH_MULT_MIN);
	
	Serial.print("setWaveWidth:freq: ");Serial.print(freq);
	Serial.print("\t wave_width_mult: ");Serial.println(wave_width_mult);
	
}

 /*
 * initBase
*/
void initBase() {
	
	int i,pax,pay,paz,pbx,pby,pbz,ax,ay,bx,by;
	float scaleA, scaleB;
	
	/*int base_points[] = {
		BASE_LEFT,  BASE_TOP,    BASE_FRONT,    //pt0 0,1,2
		BASE_RIGHT, BASE_TOP,    BASE_FRONT,    //pt1 3,4,5
		BASE_RIGHT, BASE_BOTTOM, BASE_FRONT,    //pt2 6,7,8
		BASE_LEFT,  BASE_BOTTOM, BASE_FRONT,    //pt3 9,10,11
		BASE_LEFT,  BASE_TOP,    BASE_BACK,     //pt4 12,13,14
		BASE_RIGHT, BASE_TOP,    BASE_BACK      //pt5 15,16,17
	};*/

	int base_lines_src[] = {
		//BASE_LEFT,   BASE_TOP,     BASE_FRONT
		//,BASE_RIGHT,  BASE_TOP,     BASE_FRONT
		
		BASE_RIGHT,  BASE_TOP,     BASE_FRONT,
		BASE_RIGHT,  BASE_BOTTOM,  BASE_FRONT,
		
		BASE_RIGHT,  BASE_BOTTOM,  BASE_FRONT,
		BASE_LEFT,   BASE_BOTTOM,  BASE_FRONT,
		
		BASE_LEFT,   BASE_BOTTOM, BASE_FRONT,
		BASE_LEFT,   BASE_TOP,    BASE_FRONT,
		
		//,BASE_LEFT,   BASE_TOP,    BASE_FRONT
		//,BASE_LEFT,   BASE_TOP,    BASE_BACK
		
		//,BASE_LEFT,   BASE_TOP,    BASE_BACK
		//,BASE_RIGHT,  BASE_TOP,    BASE_BACK
		
		BASE_RIGHT,  BASE_TOP,    BASE_BACK,
		BASE_RIGHT,  BASE_TOP,    BASE_FRONT
	};
	
	for(i = 0; i < 4; i++) {
	
		pax = base_lines_src[i * 6 + 0];
		pay = base_lines_src[i * 6 + 1];
		paz = base_lines_src[i * 6 + 2];
		
		pbx = base_lines_src[i * 6 + 3];
		pby = base_lines_src[i * 6 + 4];
		pbz = base_lines_src[i * 6 + 5];
		
		scaleA = (float)FOCAL_LENGTH / (float)( FOCAL_LENGTH + paz + BASE_CENTER );
		scaleB = (float)FOCAL_LENGTH / (float)( FOCAL_LENGTH + pbz + BASE_CENTER );
		
		ax = VPX + pax * scaleA;
		ay =  VPY + pay * scaleA;
		
		bx = VPX + pbx * scaleB;
		by =  VPY + pby * scaleB;
		
		base_lines[i * 4 + 0] = ax;
		base_lines[i * 4 + 1] = ay;
		base_lines[i * 4 + 2] = bx;
		base_lines[i * 4 + 3] = by;
		
	}
	
}

/*
 * renderBase
*/
void renderBase() {

	int i = 0;
	
	for(i = 0; i < BASE_LINES_TOTAL / 4; i++) {
		
		display.drawLine(base_lines[i * 4 + 0], base_lines[i * 4 + 1], base_lines[i * 4 + 2], base_lines[i * 4 + 3], WHITE);
		
	}
	
}

/*
 * initWave
*/
void initWave() {
	
	int i, line_x, idx;
	
	for( i = 0; i < WAVE_POINTS_TOTAL/6; i++) {
		
		line_x = BASE_LEFT + i * WAVE_DIV;
		
		wave_points[i * 6 + 0] = line_x;
		wave_points[i * 6 + 1] = WAVE_Y;
		wave_points[i * 6 + 2] = BASE_FRONT;
		    
		wave_points[i * 6 + 3] = line_x;
		wave_points[i * 6 + 4] = WAVE_Y;
		wave_points[i * 6 + 5] = BASE_BACK;
		
		//Serial.print("line: ");Serial.print(i);Serial.print("\t x: ");Serial.println(line_x);
		
	}
	
}

/*
 * renderWave
*/
void renderWave() {
	
	if(menu_mode == 5) {
		renderSignals();
		return;
	}
	
	if( drones[menu_mode]->getDisplayMode() == 1 ) {
		
		//renderBase();
		computeWave();
		renderThreeD();
		
	}else {
		
		renderSignals();
		
		//renderTwoDimensions();
	
	}

}


/*
 * renderSignals
*/
void renderSignals() {
	
	int i, j, py, sy, sh, wave_width, wave_x, wave_height;

	float cpp, cps, curve_alpha = 2.0;
	
	for(i = 0; i < 4; i++) {
	
		wave_height = map( drones[i]->getAmp(), 0, 200, 0, 52 );
	
		if(wave_height > 0) {
	
			wave_width = map( drones[i]->getFrequency(), 100, 2000, 40, 10 );
			
			wave_x = map( *lfo_vals[i], 128, -128, 128 - wave_width, 0 );
			
			//cps = (float)map( *lfo_vals[i], -128, 128, 0, 100 ) / 100.0;
	
			for( j = wave_x; j < wave_x + wave_width; j++) {
			
				cpp =  (float)( wave_x - j ) / (float)( wave_x - ( wave_x + wave_width ) );
			
				py = 52 - pow( 4, curve_alpha ) * pow( ( cpp * ( 1.00 - cpp )), curve_alpha ) * wave_height;
			
				if( i == menu_mode ) {
					display.drawLine( j, py, j, 52, WHITE );
				}else {
					display.drawPixel( j, py, WHITE );
				}
				
			}
		}
	}
	
}

/*
 * renderThreeD
 * render the first half left to right
 * and the second right to left
 * so that the black rectangles cover any backface or rear lines
*/
void renderThreeD() {

	int i,pax,pay,paz,pbx,pby,pbz,ax,ay,bx,by;
	float scaleA, scaleB;
	
	for( i = 0; i < WAVE_POINTS_TOTAL / 12; i += wave_resolution ) {
	
		pax = wave_points[i * 6 + 0];
		pay = wave_points[i * 6 + 1];
		paz = wave_points[i * 6 + 2];
		
		pbx = wave_points[i * 6 + 3];
		pby = wave_points[i * 6 + 4];
		pbz = wave_points[i * 6 + 5];
		
		scaleA = (float)FOCAL_LENGTH / (float)( FOCAL_LENGTH + paz + BASE_CENTER );
		scaleB = (float)FOCAL_LENGTH / (float)( FOCAL_LENGTH + pbz + BASE_CENTER );
		
		ax = VPX + pax * scaleA;
		ay =  VPY + pay * scaleA;
		
		bx = VPX + pbx * scaleB;
		by =  VPY + pby * scaleB;
		
		display.drawLine( ax, ay, bx, by, WHITE );
			
		display.fillRect( ax, ay + 1, WAVE_DIV * wave_resolution, base_lines[1] - ( ay - 1 ), BLACK );
	
	}
	
	for( i = WAVE_POINTS_TOTAL / 6 - wave_resolution; i > WAVE_POINTS_TOTAL / 12; i -= wave_resolution ) {
	
		pax = wave_points[i * 6 + 0];
		pay = wave_points[i * 6 + 1];
		paz = wave_points[i * 6 + 2];
		
		pbx = wave_points[i * 6 + 3];
		pby = wave_points[i * 6 + 4];
		pbz = wave_points[i * 6 + 5];
		
		scaleA = (float)FOCAL_LENGTH / (float)( FOCAL_LENGTH + paz + BASE_CENTER );
		scaleB = (float)FOCAL_LENGTH / (float)( FOCAL_LENGTH + pbz + BASE_CENTER );
		
		ax = VPX + pax * scaleA;
		ay =  VPY + pay * scaleA;
		
		bx = VPX + pbx * scaleB;
		by =  VPY + pby * scaleB;
		
		display.fillRect( ax - WAVE_DIV * wave_resolution, ay + 1, WAVE_DIV * wave_resolution, base_lines[1] - ( ay - 1 ), BLACK );
			
		display.drawLine( ax, ay, bx, by, WHITE );
		
	}
	
}

/*
 * computeWave
*/
void computeWave() {
	
	int i, point_y, line_x, width_max;
	float curve_alpha, curve_pct;
	
	wave_resolution = map( (drones[menu_mode]->getResonance() * 100), 0, 500, WAVE_RES_MIN, WAVE_RES_MAX);
	
	width_max = map( drones[menu_mode]->getFrequency(), 100, 2000, 80, 20 );
	
	wave_height = map( drones[menu_mode]->getAmp(), 0, 200, WAVE_HEIGHT_MAX, 0 );
	//wave_height = map( *lfo_vals[ menu_mode ], -128, 128, wave_max_height, wave_min_height );
	
	wave_width = map( *lfo_vals[ menu_mode ], -128, 128, width_max, WAVE_WIDTH_MIN  );
	
	wave_x = map( *lfo_vals[ menu_mode ], 128, -128, BASE_RIGHT - wave_width, BASE_LEFT );//wave_x = map( lfo_one_val, 127, -127, 128 - wave_width, 0 );
	
	//wave_center_offset = map( lfo_one_val, 127, -127, ( wave_width / 5), 0);
	
	curve_alpha = map( wave_x, BASE_LEFT, BASE_RIGHT - wave_width, 5.0, 2.0);

	for( i = 0; i < WAVE_POINTS_TOTAL / 6; i++) {
		
		line_x = wave_points[i * 6 + 0];
		
		//compute y if on curve
		if( line_x >= wave_x && line_x <= wave_x + wave_width) {
		
			curve_pct =  (float)( wave_x - line_x ) / (float)( wave_x - ( wave_x + wave_width ) );
			
			point_y = WAVE_Y - pow( 4, curve_alpha ) * pow( ( curve_pct * ( 1.00 - curve_pct )), curve_alpha ) * wave_height;
			
			wave_points[i * 6 + 1] = point_y;
			wave_points[i * 6 + 4] = point_y;
			
		}else {
		
			wave_points[i * 6 + 1] = WAVE_Y;
			wave_points[i * 6 + 4] = WAVE_Y;
		
		}
		
	}
	
}

/*
 * isBackFace
*/
byte isBackFace(int asx, int asy, int bsx, int bsy, int csx, int csy) {
	
	// see http://www.jurjans.lv/flash/shape.html
	
	int cax = csx - asx;
	int cay = csy - asy;
	
	int bcx = bsx - csx;
	int bcy = bsy - csy;
	
	return cax * bcy > cay * bcx;

}
