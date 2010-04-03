//================================================//
//==					main.h					==//
//================================================//
// Date: 21.01.2010
// Author  : brunql
//================================================//

#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "led_driver.h"

extern unsigned char flags;
extern unsigned char atomic_temp;

extern uint16_t show_me_1;
extern uint16_t show_me_2;

#define ZERO 0     
#define	DELAY_SHOW_SPLASH 1000


//#define ANIMATION_SWITCH_MENU_ITEMS
//#define LCD_IMAGES
//#define USING_RU_FONTS

#define	UPDATE_DISPLAY_FLAG		_BV(0)
#define ENCODER_BTN_CLICK_FLAG	_BV(1)
#define	ANIMATION_NEXT_FLAG		_BV(2)
//#define ENC_INT_NEED_TO_ENABLE_FLAG _BV(2)
#define	ANIMATION_PREV_FLAG		_BV(3)
//#define ENC_RIGHT_UP_FLAG		_BV(3)
#define	ADC_RESULT_FLAG			_BV(4)
#define	ADC_CALIBRATE_FLAG		_BV(5)
#define ADC_SET_ZERO			_BV(6)
 

#define		IF_FLAG_ON(x)		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { atomic_temp = flags; } if(atomic_temp & (x))
#define 	FLAGS_SWITCH_ON(x)	{ ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { flags |= (x); } }
#define 	FLAGS_SWITCH_OFF(x)	{ ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { flags &= (unsigned char)~(x); } }


#define 	ADC_CONVERT_IN_PROGRESS()	( ADCSRA & _BV(ADSC) )

//================================================//
//== 			 Encoder defines				==//
#define    	ENC_LEFT_PA      		_BV(PD4)
#define 	ENC_BUTTON_PE			_BV(PD3) /* INT1 */
#define 	ENC_RIGHT_PB			_BV(PD2) /* INT0 */
#define 	ENC_LEFT_OR_RIGHT		(PIND & ENC_LEFT_PA)
// Using this defines in anti brrrrzzzrrr contacts.
#define 	ENC_LEFT_UP				(PIND & ENC_LEFT_PA)
#define 	ENC_RIGHT_UP			(PIND & ENC_RIGHT_PB)
#define 	ENC_BUTTON_UP			(PIND & ENC_BUTTON_PE)
//==     	ENC_PB      PD2 == EXT_INT0 		==//	
//==  		ENC_PC  	connected to GND		==//
//==  		ENC_PD  	connected to GND		==//
//==     	ENC_PE      PD3 == EXT_INT1 		==//

//== 	ON/OFF interrupts from encoder			==//
#define ENC_INT_TURN		INT0
#define ENC_INT_BUTTON		INT1
#define ENC_INT_ENABLE() 	{ ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { GIFR = _BV(INTF0); GICR |= _BV(ENC_INT_TURN) | _BV(ENC_INT_BUTTON); } }
#define ENC_INT_DISABLE()	{ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { GICR &=  (unsigned char)~(_BV(ENC_INT_TURN) | _BV(ENC_INT_BUTTON)); }};
//
//#define ENC_INT_BUTTON_ENABLE() {ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { GICR |= _BV(ENC_INT_BUTTON); }};
//#define ENC_INT_BUTTON_DISABLE() {ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { GICR &=  (unsigned char)~(_BV(ENC_INT_BUTTON)); }};
//
//#define ENC_INT_TURN_ENABLE() {ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { GICR |= _BV(ENC_INT_TURN); }};
//#define ENC_INT_TURN_DISABLE() {ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { GICR &=  (unsigned char)~(_BV(ENC_INT_TURN)); }};
//#define ENC_INT_CHECK()		( GICR & (_BV(INT0) | _BV(INT1)) )


//================================================//

#define ENABLE 1
#define DISABLE 0
#ifndef NULL
#	define NULL 0
#endif

#define TIM2_INIT() 					{ TCCR2 = (_BV(CS22) /*| _BV(CS21)*/ /*| _BV(CS20)*/ ); } /* clk / 1024 */
#define TIM2_CLR_COUNTER_AND_OVF_ON()	{ TCNT2 = 0x00; TIMSK |= _BV(TOIE2); }		/* tim2 interrupt enable */
#define TIM2_OVF_OFF() 					{ TIMSK &= (unsigned char)~_BV(TOIE2); }	/* tim2 interrupt disable */

#define ABS(x)		(((int16_t)(x)<0)?(-((int16_t)x)):(x))
#define DIFF(x,y) 	ABS((int16_t)(x) - (int16_t)(y))
//#define CONCAT(A, B)	A##B

#define DELAY_UPDATE_DISPLAY 0x4


//#define TEST_PIN_DDR_INIT()	{ DDRD |= _BV(PD0); }
//#define TEST_PIN_UP()		{ PORTD |= _BV(PD0); }
//#define TEST_PIN_DOWN()		{ PORTD &= (uint8_t)~_BV(PD0); }
