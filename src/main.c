//================================================//
//==			ColorsCMP main file				==//
//================================================//
// Version : 0.7_eclipse_edition
// Date    : 21.01.2010
// Author  : brunql
//
// Chip type           : ATmega8
// Clock frequency     : 16,000000 MHz
//================================================//

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "main.h"
#include "led_driver.h"
#include "lcd/lcd_nokia_menu.h"
#include "algorithm.h" // define extern variables for main problem
#include "SnakeGame.h"
#include "debug.h"	// define here DEBUG to 1 for enable debug mode

uint16_t flags = 0x0000;
uint16_t atomic_flags = 0x0000;
uint8_t atomic_temp = 0x00;


uint8_t timer_delay = 0x00;
uint8_t timer_ticks_to_get_present = SPEED_TICKS_TO_GET_PRESENT;

uint8_t count = 3;
double coefs_2[10];
uint8_t data = ALL_LS;
uint16_t time = 0;//(30000) / (count + 1);


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





//================================================//
//== 				Joystick click				==//
ISR(INT1_vect)
{
#ifndef ANIMATION_SWITCH_MENU_ITEMS
	menu_before_now = 0x80;
#endif
	_delay_ms(2);	// anti brrrrzzzzrr

	GIFR = _BV(INTF1); // kill int1 flag

	uint8_t button_clicked = J_CENTER;

//	Lcd3310_ClearCenter();
//	Lcd3310_GotoXY(0, 0);

	while(1){
		uint8_t jpin_h = (JPIN & 0xf0);

		if( jpin_h == (uint8_t)(J_LU | J_RU) ){
			if( jpin_h == (uint8_t)(J_LU | J_RU) ){
				if( jpin_h == (uint8_t)(J_LU | J_RU) ){
					button_clicked = J_DOWN;
//					Lcd3310_Char('d', BLACK_TEXT_ON_WHITE);
					break;
				}
			}
		}

		if( jpin_h == (uint8_t)(J_LD | J_RD) ){
			if( jpin_h == (uint8_t)(J_LD | J_RD) ){
				if( jpin_h == (uint8_t)(J_LD | J_RD) ){
					button_clicked = J_UP;
//					Lcd3310_Char('u', BLACK_TEXT_ON_WHITE);
					break;
				}
			}
		}

		if( jpin_h == (uint8_t)(J_LU | J_LD) ){
			if( jpin_h == (uint8_t)(J_LU | J_LD) ){
				if( jpin_h == (uint8_t)(J_LU | J_LD) ){
					button_clicked = J_RIGHT;
//					Lcd3310_Char('r', BLACK_TEXT_ON_WHITE);
					break;
				}
			}

		}
		if( jpin_h == (uint8_t)(J_RU | J_RD) ){
			if( jpin_h == (uint8_t)(J_RU | J_RD) ){
				if( jpin_h == (uint8_t)(J_RU | J_RD) ){
					button_clicked = J_LEFT;
//					Lcd3310_Char('l', BLACK_TEXT_ON_WHITE);
					break;
				}
			}

		}
		if( jpin_h == (uint8_t)(J_RU | J_LU | J_RD | J_LD) ){
			if( jpin_h == (uint8_t)(J_RU | J_LU | J_RD | J_LD) ){
				if( jpin_h == (uint8_t)(J_RU | J_LU | J_RD | J_LD) ){
					button_clicked = J_CENTER;
					//Lcd3310_Char('c', BLACK_TEXT_ON_WHITE);
					break;
				}
			}
		}
	}

	if (JOYSTICK_INT_CHECK()) { return; } // interrupt on brrzzzz then pulling up joystick, so not need to do smth

	if (button_clicked == J_CENTER){
		FLAGS_SWITCH_ON(JOYSTICK_CENTER_CLICK_FLAG);

//		ALG_Start();

	}else if(button_clicked == J_UP){
		menu_now = menu_now->prev;

#ifdef ANIMATION_SWITCH_MENU_ITEMS
		FLAGS_SWITCH_ON( ANIMATION_PREV_FLAG );
#else
		menu_before_now--;
		FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );
#endif
	}else if(button_clicked == J_DOWN){
		menu_now = menu_now->next;

#ifdef ANIMATION_SWITCH_MENU_ITEMS
		FLAGS_SWITCH_ON( ANIMATION_NEXT_FLAG );
#else
		menu_before_now++;
		FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );
#endif
	}else if(button_clicked == J_LEFT){
		IF_FLAG_ON( SNAKE_PLAYING_NOW_FLAG ){
			SnakeGame_TurnLeft();
		}

		//LedDriver_SwitchLeds(RED_LEDS);

		if(menu_now == &results){
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
			FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );
		}

	}else if(button_clicked == J_RIGHT){
		IF_FLAG_ON( SNAKE_PLAYING_NOW_FLAG ){
			SnakeGame_TurnRight();
		}

		//LedDriver_SwitchLeds(0x0000);

		if(menu_now == &calibration){
			menu_now = &go;
		}else if(menu_now == &go){
			menu_now = &calibration;
		}
		FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );

	}

}

//================================================//
//== 			Game timer ticks				==//
ISR(TIMER2_OVF_vect)
{
	if(!timer_delay--){
		if(!timer_ticks_to_get_present--){
			timer_ticks_to_get_present = get_present_speed;
			SnakeGame_AddmeUpdateXY();
		}
		SnakeGame_TakeNextStep();
		timer_delay = step_speed;
	}
}






int main(void)
{

//	DEBUG_INIT(); // Initialize uart if DEBUG != 0
	DEBUG_PRINT_CHAR( 0x01 );

	// Input/Output Ports initialization
    PORTB=0x00;
    DDRB=0x00;

    PORTC=0x00;
    DDRC=0x00;

    PORTD= _BV(PD3); // pull up joystick center button
    DDRD=0x00;


    JPORT = (uint8_t)(J_RU | J_LU | J_RD | J_LD);
    JDDR &= (uint8_t)~(J_RU | J_LU | J_RD | J_LD);

    //TEST_PIN_DDR_INIT();

    ACSR = _BV(ACD); // turn off analog comparer, this is turns ON after RESET... delurium, but the fact ;)

    // External Interrupt(s) initialization
    MCUCR= _BV(ISC11) | _BV(ISC01); // fall
    GICR = _BV(INT0) | _BV(INT1); // int1 int0 enable interrupts

//    ASSR = 0x00;
//    TCCR0 = 0x00;
//    TCCR1A = 0x00;
//    TCCR1B = 0x00;
//    TCCR2 = 0x00;
//    TIMSK = 0x00;

	TIM2_INIT(); // see defines.h for details

    // Init ADC
	//ADMUX = 0x00; //_BV(ADLAR); // ADC0, result in ADC
	ADMUX =  _BV(MUX3) | _BV(MUX0); // diff * 10; result in ADC
	ADCSRA = _BV(ADEN) /*| _BV(ADIE)*/ | _BV(ADPS2)  | _BV(ADPS1) | _BV(ADPS0); // XTAL / 16;

	ADCSRA |= _BV( ADSC ); // start conversation first times
	while(ADC_CONVERT_IN_PROGRESS()){}
	adc_data = ADC;

	LedDriver_Init();
	Lcd3310_InitializeDisplay(DELAY_SHOW_SPLASH);



	DEBUG_PRINT_CHAR( 0x02 );


	sei();
	DEBUG_PRINT_CHAR( 0x03 );

	DEBUG_PRINT_CHAR( 0x00 );


	LedDriver_SwitchLeds(0x0000);


	DDRC  |= _BV(PC3);
	ADMUX = _BV(MUX3) | _BV(MUX0); //  ADC = 10 x |ADC0 - ADC1|;  0x000 == 0; 0x3FF = -1;  0x1FF = 512; 0x200 = -512;
//	ADMUX = _BV(MUX4);
//
//
//	for(;;){
//
//	}


   	for(;;){

		IF_FLAG_ON( UPDATE_DISPLAY_FLAG ){
			FLAGS_SWITCH_OFF( UPDATE_DISPLAY_FLAG );
			IF_FLAG_OFF( SNAKE_PLAYING_NOW_FLAG ){
				Lcd3310_UpdateDisplayInfo();
			}
		}

		IF_FLAG_ON( JOYSTICK_CENTER_CLICK_FLAG ){
			FLAGS_SWITCH_OFF( JOYSTICK_CENTER_CLICK_FLAG );

			Menu_EnterClick();

			FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );
		}



		IF_FLAG_ON( ADC_RESULT_FLAG ){ // in menu adc result

			ADC_LoadingAndEvalIt(SaveResultsAlgorithm);

			FLAGS_SWITCH_OFF( ADC_RESULT_FLAG );
			FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );
		}

		IF_FLAG_ON( ADC_CALIBRATE_FLAG ){

			ADC_LoadingAndEvalIt(CalibrationAlgorithm);

			FLAGS_SWITCH_OFF( ADC_CALIBRATE_FLAG );
		}

		IF_FLAG_ON( ADC_SET_ZERO ){

			ADC_LoadingAndEvalIt(SetZeroAlgorithm);

			FLAGS_SWITCH_OFF( ADC_SET_ZERO );
		}

	#ifdef ANIMATION_SWITCH_MENU_ITEMS
		IF_FLAG_ON( ANIMATION_NEXT_FLAG ){
			FLAGS_SWITCH_OFF( ANIMATION_NEXT_FLAG );
			if(menu_now != &results && menu_now != &snake_results){
				Lcd3310_AnimationSwitchMenuItems(1);
			}
			FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );
		}
		IF_FLAG_ON( ANIMATION_PREV_FLAG ){
			FLAGS_SWITCH_OFF( ANIMATION_PREV_FLAG );
			if(menu_now != &results && menu_now != &snake_results){
				Lcd3310_AnimationSwitchMenuItems(0);
			}
			FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );
		}
	#endif

		IF_FLAG_ON( SNAKE_START_GAME_FLAG ){
			FLAGS_SWITCH_OFF( SNAKE_START_GAME_FLAG );
			SnakeGame_Start();
			TIM2_CLR_COUNTER_AND_OVF_ON();
			FLAGS_SWITCH_ON( SNAKE_PLAYING_NOW_FLAG );
		}
		IF_FLAG_ON( SNAKE_STOP_GAME_FLAG ){
			FLAGS_SWITCH_OFF( SNAKE_STOP_GAME_FLAG );
			TIM2_OVF_OFF();
			SnakeGame_Stop();
			FLAGS_SWITCH_OFF( SNAKE_PLAYING_NOW_FLAG );
		}

	}

	return ZERO;
}
