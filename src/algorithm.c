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
double coefs[2][3] = {
//		 R  G  B
		{0, 0, 0},
		{0, 0, 0}
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

volatile int16_t adc_res_first = 0x00;
volatile int16_t adc_res_second = 0x00;


PGM_P pstr_complete = "Done!";
PGM_P pstr_please_wait = "Please wait...";


void hex2dec_result(uint16_t hex, uint8_t result_index, uint8_t result_color)
{
	uint8_t res2, res1, res0;
	res0 =  hex % 10;
	hex /= 10;
	res1 = hex % 10;
	hex /= 10;
	res2 = hex % 10;

	uint16_t dec_result = (res2 << 8) | (res1 << 4)| res0;

	result[result_index][result_color] = dec_result;
}


void StartADC(FirstOrSecond first)
{
	if (first){
		ADMUX &= (unsigned char) ~ _BV(MUX0);
	}else{ // second
		ADMUX |= _BV(MUX0);
	}

	// delay for stabilisation on ADC pin (charge intro reference capacitor)
	// TODO: NEED TEST!
	_delay_us(50);

	ADCSRA |= _BV( ADSC ); // start conversation
	while( ADC_CONVERT_IN_PROGRESS() ) {  };
	//result in adc_data
}

void GetValuesFromADC(void) // with simple averaging
{
	uint16_t temp_val = adc_res_first;
	StartADC( FIRST );
	if(temp_val){
		adc_res_first = (adc_data + temp_val) / 2;
	}else{
		adc_res_first = adc_data;
	}

	temp_val = adc_res_second;
	StartADC( SECOND );
	if(temp_val){
		adc_res_second = (adc_data + temp_val) / 2;
	}else{
		adc_res_second = adc_data;
	}
}


void ADC_255_Times(void)
{
	//===================================//
	adc_res_first = 0x00;
	adc_res_second = 0x00;
	for(uint8_t i=0; i < 0xff;  i++){
		GetValuesFromADC();
	}
	//===================================//
}


void CalibrationAlgorithm(uint8_t color)
{
#ifdef EVP_ALGORITM
	// divide results by 4
//	uint8_t adc_res_first_8 = (uint8_t)((uint16_t)adc_res_first >> 2);
//	uint8_t adc_res_second_8 = (uint8_t)((uint16_t)adc_res_second >> 2);
//	coefs[FIRST][color] = (double) EVP_ALG_PERCENT * ADC_MAX_VALUE / adc_res_first_8;
//	coefs[SECOND][color] = (double) EVP_ALG_PERCENT * ADC_MAX_VALUE / adc_res_second_8;
//
	coefs[FIRST][color] = (double) EVP_ALG_PERCENT * ADC_MAX_VALUE / adc_res_first;
	coefs[SECOND][color] = (double) EVP_ALG_PERCENT * ADC_MAX_VALUE / adc_res_second;

#else
	coefs[FIRST][color] = 1;
	coefs[SECOND][color] = (double)adc_res_first / adc_res_second;
#endif
}

void SetZeroAlgorithm(uint8_t color)
{
//	zero[FIRST] = adc_res_first;
//	zero[SECOND] = adc_res_second;

	max_diff = ADC_MAX_VALUE;// -  ((adc_res_first < adc_res_second) ? adc_res_first : adc_res_second);
}

void SaveResultsAlgorithm(uint8_t color)
{

	// divide results by 4
//	uint8_t adc_res_first_8 = (uint8_t)((uint16_t)adc_res_first >> 2);
//	uint8_t adc_res_second_8 = (uint8_t)((uint16_t)adc_res_second >> 2);
//
//	int16_t result1 = (int16_t)((double)(((double)adc_res_first_8) * coefs[FIRST][color]));
//	int16_t result2 = (int16_t)((double)(((double)adc_res_second_8) * coefs[SECOND][color]));

	int16_t result1 = (int16_t)((double)(((double)adc_res_first) * coefs[FIRST][color]));
	int16_t result2 = (int16_t)((double)(((double)adc_res_second) * coefs[SECOND][color]));
	uint16_t diff_percent = DIFF(result1, result2) * 100 / max_diff;

	hex2dec_result(result1, FIRST, color); // result[FIRST][color] = result1;
	hex2dec_result(result2, SECOND, color); //result[SECOND][color] = result2;
	hex2dec_result(diff_percent, PERCENT, color); //result[PERCENT][color] = DIFF(result1, result2);// * 100 / max_diff;
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
	ENC_INT_DISABLE();

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
		Lcd3310_Char('#', BLACK_TEXT_ON_WHITE);
		if(evalMe == SetZeroAlgorithm){
			LedDriver_SwitchLeds( 0x00 );
		}else{
			LedDriver_SwitchLeds( led_show_codes[color] );
		}
		_delay_ms( DELAY_BEFORE_START_ADC );
		Lcd3310_Char('#', BLACK_TEXT_ON_WHITE);
		ADC_255_Times();
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
	ENC_INT_ENABLE();
}