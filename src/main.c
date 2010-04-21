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

volatile int16_t adc_data = 0x0000;
uint8_t timer_delay = 0x00;
uint8_t timer_ticks_to_get_present = SPEED_TICKS_TO_GET_PRESENT;





//================================================//
//== 				Joystick click				==//
ISR(INT1_vect)
{
#ifndef ANIMATION_SWITCH_MENU_ITEMS
	menu_before_now = 0x80;
#endif
	_delay_ms(2);	// anti brrrrzzzzrr

	GIFR = _BV(INTF1); // kill int1 flag

	uint8_t result = J_CENTER;

//	Lcd3310_ClearCenter();
//	Lcd3310_GotoXY(0, 0);

	while(1){
		uint8_t jpin_h = (JPIN & 0xf0);

		if( jpin_h == (uint8_t)(J_LU | J_RU) ){
			if( jpin_h == (uint8_t)(J_LU | J_RU) ){
				if( jpin_h == (uint8_t)(J_LU | J_RU) ){
					result = J_DOWN;
//					Lcd3310_Char('d', BLACK_TEXT_ON_WHITE);
					break;
				}
			}
		}

		if( jpin_h == (uint8_t)(J_LD | J_RD) ){
			if( jpin_h == (uint8_t)(J_LD | J_RD) ){
				if( jpin_h == (uint8_t)(J_LD | J_RD) ){
					result = J_UP;
//					Lcd3310_Char('u', BLACK_TEXT_ON_WHITE);
					break;
				}
			}
		}

		if( jpin_h == (uint8_t)(J_LU | J_LD) ){
			if( jpin_h == (uint8_t)(J_LU | J_LD) ){
				if( jpin_h == (uint8_t)(J_LU | J_LD) ){
					result = J_RIGHT;
//					Lcd3310_Char('r', BLACK_TEXT_ON_WHITE);
					break;
				}
			}

		}
		if( jpin_h == (uint8_t)(J_RU | J_RD) ){
			if( jpin_h == (uint8_t)(J_RU | J_RD) ){
				if( jpin_h == (uint8_t)(J_RU | J_RD) ){
					result = J_LEFT;
//					Lcd3310_Char('l', BLACK_TEXT_ON_WHITE);
					break;
				}
			}

		}
		if( jpin_h == (uint8_t)(J_RU | J_LU | J_RD | J_LD) ){
			if( jpin_h == (uint8_t)(J_RU | J_LU | J_RD | J_LD) ){
				if( jpin_h == (uint8_t)(J_RU | J_LU | J_RD | J_LD) ){
					result = J_CENTER;
					//Lcd3310_Char('c', BLACK_TEXT_ON_WHITE);
					break;
				}
			}
		}
	}

	if (JOYSTICK_INT_CHECK()) { return; } // interrupt on brrzzzz then pulling up joystick, so not need to do smth

	if (result == J_CENTER){
		FLAGS_SWITCH_ON(JOYSTICK_CENTER_CLICK_FLAG);
	}else if(result == J_UP){
		menu_now = menu_now->prev;

#ifdef ANIMATION_SWITCH_MENU_ITEMS
		FLAGS_SWITCH_ON( ANIMATION_PREV_FLAG );
#else
		menu_before_now--;
		FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );
#endif
	}else if(result == J_DOWN){
		menu_now = menu_now->next;

#ifdef ANIMATION_SWITCH_MENU_ITEMS
		FLAGS_SWITCH_ON( ANIMATION_NEXT_FLAG );
#else
		menu_before_now++;
		FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );
#endif
	}else if(result == J_LEFT){
		IF_FLAG_ON( SNAKE_PLAYING_NOW_FLAG ){
			SnakeGame_TurnLeft();
		}
	}else if(result == J_RIGHT){
		IF_FLAG_ON( SNAKE_PLAYING_NOW_FLAG ){
			SnakeGame_TurnRight();
		}
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
	ADMUX = 0x00; //_BV(ADLAR); // ADC0, result in ADC
	ADCSRA = _BV(ADEN) /*| _BV(ADIE)*/ | _BV(ADPS2)  | _BV(ADPS1) | _BV(ADPS0); // XTAL / 16;


	LedDriver_Init();
	Lcd3310_InitializeDisplay(DELAY_SHOW_SPLASH);



	DEBUG_PRINT_CHAR( 0x02 );


	sei();
	DEBUG_PRINT_CHAR( 0x03 );

	DEBUG_PRINT_CHAR( 0x00 );

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
