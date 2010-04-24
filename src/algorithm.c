/*
 * algorithm.c
 *
 *  Created on: 15.02.2010
 *      Author: brunql
 */
#include <avr/pgmspace.h>
#include "main.h"
#include "algorithm.h"
#include "lcd/lcd_nokia_menu.h"


// calibration coefficients
double coefs[3] = {
//		R  G  B
		1, 1, 1
};

// ADC values then leds off, NOT USED NOW
//uint16_t zero[2] = { 0x0000, 0x0000 };

// this value eval then "set zero" and use for eval percent(%) of difference between sensors
uint16_t max_diff = ADC_MAX_VALUE;

// Algorithm results, show this in menu
uint16_t result[3][3] = {
//		 R  G  B
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
};

uint16_t led_show_codes[3] = {
//		 	R  		 G  		 B
		RED_LEDS, GREEN_LEDS, BLUE_LEDS
};

volatile uint16_t adc_data = 0x0000;


PGM_P pstr_complete = "Complete!";
PGM_P pstr_please_wait = "Please wait...";


uint16_t hex2dec_result(uint16_t hex)
{
	uint8_t res2, res1, res0;
	res0 =  hex % 10;
	hex /= 10;
	res1 = hex % 10;
	hex /= 10;
	res2 = hex % 10;

	return (res2 << 8) | (res1 << 4)| res0;
}


//void StartADC(void)
//{
//	// delay for stabilisation on ADC pin (charge intro reference capacitor)
//	// TODO: NEED TEST!
//	ADCSRA |= _BV( ADSC ); // start conversation
//	while( ADC_CONVERT_IN_PROGRESS() ) {  };
//	adc_data = ADC;
//	//result in adc_data
//}

//void GetValuesFromADC(void) // with simple averaging
//{
//	// 1:
////	uint16_t temp_val = adc_res_first;
////	StartADC( FIRST );
////	if(temp_val){
////		adc_res_first = (adc_data + temp_val) / 2;
////	}else{
////		adc_res_first = adc_data;
////	}
////
////	temp_val = adc_res_second;
////	StartADC( SECOND );
////	if(temp_val){
////		adc_res_second = (adc_data + temp_val) / 2;
////	}else{
////		adc_res_second = adc_data;
////	}
//
//	// 2,3:
////	StartADC( FIRST );
////	adc_res_first = adc_data;
//
//	StartADC( );
//	adc_res_second = adc_data;
//}


void ADC_N_Times(void)
{
	//===================================//
	// 1: 256 ADCs without summing and dividing
//	adc_res_first = 0x0000;
//	adc_res_second = 0x0000;
//	for(uint8_t i=0; i < 0xff;  i++){
//		GetValuesFromADC();
//	}





//	// 2: 64 ADCs
//	uint16_t adc_64_first = 0x0000;
//	uint16_t adc_64_second = 0x0000;
//	adc_res_first = 0x0000;
//	adc_res_second = 0x0000;
//	for(uint8_t i=0; i < 0x40;  i++){
//		GetValuesFromADC();
//		adc_64_first += (uint16_t) adc_res_first;
//		adc_64_second += (uint16_t) adc_res_second;
//	}
//	adc_64_first >>= 6; // divide 64
//	adc_64_second >>= 6; // divide 64
//
//	adc_res_first = (uint16_t) adc_64_first;
//	adc_res_second = (uint16_t) adc_64_second;

	// 3: 256 ADCs
//	uint32_t adc_256_first = 0x00000000;
//	uint32_t adc_256_second = 0x00000000;
//	adc_res_first = 0x0000;
//	adc_res_second = 0x0000;
//	PORTC |= _BV(PC3);
//	DDRC  |= _BV(PC3);
//	for(uint8_t i=0; i < 0xff;  i++){
//		GetValuesFromADC();
//		adc_256_first += (uint32_t) adc_res_first;
//		adc_256_second += (uint32_t) adc_res_second;
//	}
//	PORTC &= (uint8_t)~_BV(PC3);
//	GetValuesFromADC();
//	adc_256_first += (uint32_t) adc_res_first;
//	adc_256_second += (uint32_t) adc_res_second;
//	adc_256_first >>= 8; // divide 256
//	adc_256_second >>= 8; // divide 256
//	adc_res_first = (uint16_t) adc_256_first;
//	adc_res_second = (uint16_t) adc_256_second;



	// 4: diff ADC inputs
	uint32_t adc_256_times = 0x00000000;
	adc_data = 0x0000;

	PORTC |= _BV(PC3);
	DDRC  |= _BV(PC3);	// test pin up

	uint8_t i=0;
	do{
		ADCSRA |= _BV( ADSC ); // start conversation
		while( ADC_CONVERT_IN_PROGRESS() ) {  };
		adc_data = ADC;
		if((uint16_t)adc_data & (uint16_t)0x0200){
			adc_data = (uint16_t)0x03ff - adc_data + 1;
		}

		adc_256_times += (uint32_t) adc_data;
	} while( ++i != 245 ); // loop 245 times, 7 brrrzzzzzzzzzzz

	PORTC &= (uint8_t)~_BV(PC3); // test pin down

	adc_256_times >>= 8; // divide 256
	adc_data = (uint16_t) adc_256_times;
	//===================================//
}


void SetZeroAlgorithm(uint8_t color)
{
	max_diff = ADC_MAX_VALUE;// -  ((adc_res_first < adc_res_second) ? adc_res_first : adc_res_second);
	Lcd3310_Char('K', BLACK_TEXT_ON_WHITE);
	coefs[0] = 1;
	coefs[1] = 1;
	coefs[2] = 1;
}

void CalibrationAlgorithm(uint8_t color)
{
	result[CALIBRATE_INDX][color] = hex2dec_result( adc_data );
}

void SaveResultsAlgorithm(uint8_t color)
{
	result[MEASURE_INDX][color] = hex2dec_result( adc_data );
	result[DIFF_INDX][color] = hex2dec_result( DIFF(adc_data, result[CALIBRATE_INDX][color]) );
}

void ADC_LoadingAndEvalIt(ptrEvalMe evalMe)
{
	/*
	 * Please wait...
	 *
	 * [############]
	 *
	 * 	 Complete!
	 */
	JOYSTICK_INT_DISABLE();

	Lcd3310_ClearCenter();
	Lcd3310_GotoXY(0, 1);
	Lcd3310_String_P( pstr_please_wait, BLACK_TEXT_ON_WHITE );
	if(evalMe == SetZeroAlgorithm){
		Lcd3310_GotoXY(5, 3);
	}else{
		Lcd3310_GotoXY(1, 3);
	}
	Lcd3310_Char('[', BLACK_TEXT_ON_WHITE);

	for(uint8_t color=0; color < 3; color++){
		LedDriver_SwitchLeds( RED_LEDS | GREEN_LEDS | BLUE_LEDS );
		_delay_ms( DELAY_BEFORE_START_ADC );
		Lcd3310_Char('#', BLACK_TEXT_ON_WHITE);
		if(evalMe == SetZeroAlgorithm){
			LedDriver_SwitchLeds( 0x00 );
		}else{
			LedDriver_SwitchLeds( led_show_codes[color] );
		}
		_delay_ms( DELAY_BEFORE_START_ADC );
		Lcd3310_Char('#', BLACK_TEXT_ON_WHITE);

		ADC_N_Times();
		Lcd3310_Char('#', BLACK_TEXT_ON_WHITE);

		// Start Algorithm
		(evalMe)(color);

		if(evalMe == SetZeroAlgorithm){
			break; // not need to switch off leds 3 times :)
		}
	}
	LedDriver_SwitchLeds( 0x00 );
	Lcd3310_Char(']', BLACK_TEXT_ON_WHITE);
	Lcd3310_GotoXY(2, 5);
	Lcd3310_String_P( pstr_complete, BLACK_TEXT_ON_WHITE );
	_delay_ms(500); // show complete and wait 0.5 sec
	JOYSTICK_INT_ENABLE();
//	LedDriver_SwitchLeds(GREEN_LEDS);
//	(evalMe)(0);
}
