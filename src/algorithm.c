/*
 * algorithm.c
 *
 *  Created on: 15.02.2010
 *      Author: brunql
 */
#include <avr/pgmspace.h>
#include "main.h"
#include "debug.h"
#include "algorithm.h"
#include "lcd/lcd_nokia_menu.h"
#include "lcd/lcd_nokia_3310_frm_brunql.h"


// calibration coefficients
double coefs[3] = {
//		R  G  B
		1, 1, 1
};


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

uint8_t measure_delay = MEASURE_DELAY_BEFORE_START_ADC;

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


void ADC_N_Times(void)
{
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

void SaveMeasureResultsToCalibrate(void)
{
	result[CALIBRATE_INDX][RED] = result[MEASURE_INDX][RED];
	result[CALIBRATE_INDX][GREEN] = result[MEASURE_INDX][GREEN];
	result[CALIBRATE_INDX][BLUE] = result[MEASURE_INDX][BLUE];

	result[DIFF_INDX][RED] = 0;
	result[DIFF_INDX][GREEN] = 0;
	result[DIFF_INDX][BLUE] = 0;

	Lcd3310_ClearCenter();
	Lcd3310_GotoXY(5,3);
	Lcd3310_Char('S', BLACK_TEXT_ON_WHITE);
	Lcd3310_Char('a', BLACK_TEXT_ON_WHITE);
	Lcd3310_Char('v', BLACK_TEXT_ON_WHITE);
	Lcd3310_Char('e', BLACK_TEXT_ON_WHITE);
	Lcd3310_Char('d', BLACK_TEXT_ON_WHITE);
	_delay_ms(500);
}

void MeasureAllAlgorithm(uint8_t color)
{
	result[MEASURE_INDX][color] = adc_data;
	int16_t diff = result[CALIBRATE_INDX][color] - result[MEASURE_INDX][color];
	if( diff < 0 ) diff *= -1;
	result[DIFF_INDX][color] = diff;
}

void CalibrationAlgorithm(uint8_t color)
{
	result[CALIBRATE_INDX][color] = adc_data;
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

	Lcd3310_GotoXY(1, 3);
	Lcd3310_Char('[', BLACK_TEXT_ON_WHITE);

	for(uint8_t color=0; color < 3; color++){
		LedDriver_SwitchLeds( RED_LEDS | GREEN_LEDS | BLUE_LEDS );
		_delay_ms((double) measure_delay * 1000 );
		Lcd3310_Char('#', BLACK_TEXT_ON_WHITE);

		LedDriver_SwitchLeds( led_show_codes[color] );
		_delay_ms((double) measure_delay * 1000 );

		Lcd3310_Char('#', BLACK_TEXT_ON_WHITE);

		ADC_N_Times();
		Lcd3310_Char('#', BLACK_TEXT_ON_WHITE);

		// Start Algorithm
		(evalMe)(color);
	}
//	LedDriver_SwitchLeds( 0x00 );
	LedDriver_SwitchLeds( RED_LEDS | GREEN_LEDS | BLUE_LEDS );
	Lcd3310_Char(']', BLACK_TEXT_ON_WHITE);
	Lcd3310_GotoXY(2, 5);
	Lcd3310_String_P( pstr_complete, BLACK_TEXT_ON_WHITE );
	_delay_ms(500); // show complete and wait 0.5 sec

	JOYSTICK_INT_ENABLE();
}








//uint8_t count = 3;
//double coefs_2[10];
//uint8_t data = ALL_LS;
//uint16_t time = 0;//(30000) / (count + 1);


//
//void EvalCoefs_2(void)
//{
//	ADMUX = _BV(ADLAR) | _BV(MUX3) | _BV(MUX0); // 10 x diff
//
//
//	Lcd3310_GotoXY(0, 0);
//	for(uint8_t i=0; i<10; i++){
//		count = i * 10;
//		PORTC |= _BV(PC3);
//		__asm__ volatile ("nop"::);
//		PORTC &= (uint8_t)~_BV(PC3);
//
//
//		time = (uint16_t)(0xFFFF) / (count + 1); // = time_us / (6 * count + 6); 6 * 0xFFFF us
//		time <<= 1;
//		for(uint16_t pwm=0; pwm < time; pwm++)
//		{
//			LedDriver_Switch_3_Leds(data);
//			for(uint8_t down=0; down < count; down++)
//				LedDriver_Switch_3_Leds(0b000);
//		}
//		////		_delay_ms(2);
//		//		if( count++ > 20 ) count = 3;
//
//		//		PORTC |= _BV(PC3);
//		ADCSRA |= _BV( ADSC ); // start conversation
//		while( ADC_CONVERT_IN_PROGRESS() ) {  // 112us
//			LedDriver_Switch_3_Leds(data);
//			for(uint8_t down=0; down < count; down++)
//				LedDriver_Switch_3_Leds(0b000);
//		}
//		adc_data = ADCH;
//		coefs_2[i] = (double) EVP_ALG_PERCENT * ADC_MAX_VALUE / adc_data;
//
//		Lcd3310_UInt8AsText(count, WHITE_TEXT_ON_BLACK);
//		Lcd3310_Char(':', WHITE_TEXT_ON_BLACK);
//		Lcd3310_UInt16AsText_3Chars((uint16_t)adc_data, WHITE_TEXT_ON_BLACK);
//		//Lcd3310_Char(' ', WHITE_TEXT_ON_BLACK);
//		//		PORTC &= (uint8_t)~_BV(PC3);
//		//		if(count == 0xff){
//		//			_delay_ms(2);
//		//		}
//	}
//}
//
//void ALG_Start(void)
//{
//	ADMUX = _BV(ADLAR) | _BV(MUX3) | _BV(MUX0); // 1 x diff
//
//	Lcd3310_GotoXY(0, 0);
//	for(uint8_t i=0; i<10; i++){
//		count = i * 10;
//
//		PORTC |= _BV(PC3);
//		__asm__ volatile ("nop"::);
//		PORTC &= (uint8_t)~_BV(PC3);
//
//
//		time = (uint16_t)(0xFFFF) / (count + 1); // = time_us / (6 * count + 6); time_us = 6 * 0xFFFF us
//		time <<= 1; // *2
//		for(uint16_t pwm=0; pwm < time; pwm++)
//		{
//			LedDriver_Switch_3_Leds(data);
//			for(uint8_t down=0; down < count; down++)
//				LedDriver_Switch_3_Leds(0b000);
//		}
//		////		_delay_ms(2);
//		//		if( count++ > 20 ) count = 3;
//
//		//		PORTC |= _BV(PC3);
//		ADCSRA |= _BV( ADSC ); // start conversation
//		while( ADC_CONVERT_IN_PROGRESS() ) {  // 112us
//			LedDriver_Switch_3_Leds(data);
//			for(uint8_t down=0; down < count; down++)
//				LedDriver_Switch_3_Leds(0b000);
//		}
//		adc_data = ADCH;
//		adc_data = (uint16_t)((double) adc_data * coefs_2[i]);
//
//		Lcd3310_UInt8AsText(count, WHITE_TEXT_ON_BLACK);
//		Lcd3310_Char(':', WHITE_TEXT_ON_BLACK);
//		Lcd3310_UInt16AsText_3Chars((uint16_t)adc_data, WHITE_TEXT_ON_BLACK);
//		//Lcd3310_Char(' ', WHITE_TEXT_ON_BLACK);
//		//		PORTC &= (uint8_t)~_BV(PC3);
//		//		if(count == 0xff){
//		//			_delay_ms(2);
//		//		}
//	}
//}

