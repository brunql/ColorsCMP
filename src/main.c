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

#include "debug.h"	// define here DEBUG to 1 for enable debug mode

uint8_t flags = 0x00;
uint8_t atomic_temp = 0x00;

volatile int16_t adc_data = 0x00;





//================================================//
//== 			Encoder turning					==//
ISR(INT0_vect)
{
	uint8_t encoder_state = ENC_LEFT_OR_RIGHT;

	_delay_ms(2);	// anti brrrrzzzzrr

	GIFR = _BV(INTF0); // ,kz t,fysq dhjn!!!

	if(ENC_RIGHT_UP){ return; } // else make some stuff

	// after brrrzzzzrrrrrr, lets check where should i move menu?
    if (encoder_state){
		menu_now = menu_now->prev;

		#ifdef ANIMATION_SWITCH_MENU_ITEMS
			FLAGS_SWITCH_ON( ANIMATION_PREV_FLAG );
		#else
			menu_before_now--;
			FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );
		#endif
	}else{
		menu_now = menu_now->next;

		#ifdef ANIMATION_SWITCH_MENU_ITEMS
			FLAGS_SWITCH_ON( ANIMATION_NEXT_FLAG );
		#else
			menu_before_now++;
			FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );
		#endif
	}
}


//================================================//
//== 			Encoder button click			==//
ISR(INT1_vect)
{
	_delay_ms(4);	// anti brrrrzzzzrr

	GIFR = _BV(INTF1); // ,kz t,fysq dhjn!!!

	if(ENC_BUTTON_UP){ return; } // else make some stuff

	#ifndef ANIMATION_SWITCH_MENU_ITEMS
		menu_before_now = 0x80;
	#endif
	FLAGS_SWITCH_ON(ENCODER_BTN_CLICK_FLAG);
}

//ISR( TIMER2_OVF_vect )
//{
//}

//================================================//
//==					ADC						==//
ISR(ADC_vect)
{
	adc_data = ADC;
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

    PORTD=0x00; //_BV(PD2) | _BV(PD3) | _BV(PD4); // pull up encoder
    DDRD=0x00;

    //TEST_PIN_DDR_INIT();

    ACSR = _BV(ACD); // turn off analog comparer, this is turns ON after RESET... delurium, but the fact ;)

    // External Interrupt(s) initialization
    MCUCR= _BV(ISC11) | _BV(ISC01); // fall
    //// --Any logical change on INT1 and INT0 generates an interrupt request.--
    GICR = _BV(INT0) | _BV(INT1); // int1 int0 enable interrupts

//    ASSR = 0x00;
//    TCCR0 = 0x00;
//    TCCR1A = 0x00;
//    TCCR1B = 0x00;
//    TCCR2 = 0x00;
//    TIMSK = 0x00;

	//TIM2_INIT(); // see defines.h for details

    // Init ADC
	ADMUX = 0x00; //_BV(ADLAR); // ADC0, result in ADC
	ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0) ; // XTAL / 16;


	LedDriver_Init();
	Lcd3310_InitializeDisplay(DELAY_SHOW_SPLASH);

	DEBUG_PRINT_CHAR( 0x02 );


	sei();
	DEBUG_PRINT_CHAR( 0x03 );

	DEBUG_PRINT_CHAR( 0x00 );

   	for(;;){

		IF_FLAG_ON( UPDATE_DISPLAY_FLAG ){
			FLAGS_SWITCH_OFF( UPDATE_DISPLAY_FLAG );
			Lcd3310_UpdateDisplayInfo();
		}

		IF_FLAG_ON( ENCODER_BTN_CLICK_FLAG ){
			FLAGS_SWITCH_OFF( ENCODER_BTN_CLICK_FLAG );

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
			Lcd3310_AnimationSwitchMenuItems(1);
			FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );
		}
		IF_FLAG_ON( ANIMATION_PREV_FLAG ){
			FLAGS_SWITCH_OFF( ANIMATION_PREV_FLAG );
			Lcd3310_AnimationSwitchMenuItems(0);
			FLAGS_SWITCH_ON( UPDATE_DISPLAY_FLAG );
		}
	#endif
	}

	return ZERO;
}
