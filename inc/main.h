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

extern uint16_t flags;
extern uint16_t atomic_flags;
extern uint8_t atomic_temp;

extern uint16_t show_me_1;
extern uint16_t show_me_2;

extern uint8_t timer_delay;
extern uint8_t timer_ticks_to_get_present;

#define ZERO 0     
#define	DELAY_SHOW_SPLASH 1000


#define ANIMATION_SWITCH_MENU_ITEMS
#define LCD_IMAGES
//#define USING_RU_FONTS

#define	UPDATE_DISPLAY_FLAG					_BV(0)
#define JOYSTICK_CENTER_CLICK_FLAG			_BV(1)
#define	ANIMATION_NEXT_FLAG					_BV(2)
#define	ANIMATION_PREV_FLAG					_BV(3)
#define	ADC_RESULT_FLAG						_BV(4)
#define	ADC_CALIBRATE_FLAG					_BV(5)
#define SNAKE_PLAYING_NOW_FLAG 				_BV(6)
#define SNAKE_START_GAME_FLAG 				_BV(7)
#define SNAKE_STOP_GAME_FLAG 				_BV(8)
#define SAVE_MEASURED_AS_CALIBRATE_FLAG 	_BV(9)

 

#define		IF_FLAG_ON(x)		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { atomic_flags = flags; } if(atomic_flags & ((uint16_t)(x)))
#define		IF_FLAG_OFF(x)		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { atomic_flags = flags; } if(!(atomic_flags & ((uint16_t)(x))))
#define 	FLAGS_SWITCH_ON(x)	{ ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { flags |= (uint16_t)(x); } }
#define 	FLAGS_SWITCH_OFF(x)	{ ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { flags &= (uint16_t)~(x); } }


#define 	ADC_CONVERT_IN_PROGRESS()	( ADCSRA & _BV(ADSC) )

#define LED_PORT	PORTC
#define LED_DDR		DDRC
#define LED_P		_BV(PC3)

//================================================//
// 				Joystick defines
#define JDDR	DDRC
#define JPORT	PORTC
#define JPIN	PINC
#define J_C		_BV(PD3) /* CENTER - PD3 (INT0) */
#define J_RU	_BV(PC4) /* RIGHT UP */
#define J_LU	_BV(PC7) /* LEFT UP */
#define J_RD	_BV(PC5) /* RIGHT DOWN */
#define J_LD	_BV(PC6) /* LEFT DOWN */

#define J_CENTER 	0x00
#define J_UP		0x01
#define J_DOWN		0x02
#define J_LEFT		0x03
#define J_RIGHT		0x04

#define JOYSTICK_INT	INT1
#define JOYSTICK_INT_ENABLE() 	{ ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { GIFR = _BV(INTF0); GICR |= _BV(JOYSTICK_INT); 	}}
#define JOYSTICK_INT_DISABLE() 	{ ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { GICR &=  (uint8_t)~(_BV(JOYSTICK_INT)); 	}}
#define JOYSTICK_INT_CHECK()	( PIND & _BV(PD3) )
//================================================//

//#define ENABLE 1
//#define DISABLE 0
#ifndef NULL
#	define NULL 0
#endif

#define TIM2_INIT() 					{ TCCR2 = (_BV(CS22) | _BV(CS21) /*| _BV(CS20)*/ ); } /* clk / 1024 */
#define TIM2_CLR_COUNTER_AND_OVF_ON()	{ TCNT2 = 0x00; TIMSK |= _BV(TOIE2); }		/* tim2 interrupt enable */
#define TIM2_OVF_OFF() 					{ TIMSK &= (uint8_t)~_BV(TOIE2); }	/* tim2 interrupt disable */

//#define ABS(x)		(((int16_t)(x)<0)?(-((int16_t)x)):(x))
//#define DIFF(x,y) 	ABS((int16_t)(x) - (int16_t)(y))
//#define CONCAT(A, B)	A##B


//#define TEST_PIN_DDR_INIT()	{ DDRD |= _BV(PD0); }
//#define TEST_PIN_UP()		{ PORTD |= _BV(PD0); }
//#define TEST_PIN_DOWN()		{ PORTD &= (uint8_t)~_BV(PD0); }
