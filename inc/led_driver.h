//================================================//
//==				led_driver.h				==//
//================================================//
// Date: 21.01.2010
// Author  : brunql
//================================================//

#ifndef _LED_DRIVER_INCLUDED_
#define _LED_DRIVER_INCLUDED_ 1

#include <avr/io.h>
#include "main.h"

#define LED_DR_PORT 	(PORTA)
#define LED_DR_DDR 		(DDRA)

#define DATA_PIN 	_BV(PA2)
#define CLK_PIN 	_BV(PA3)
#define LATCH_PIN	_BV(PA4)

#define LATCH_UP			{ LED_DR_PORT |= LATCH_PIN;	}
#define LATCH_DOWN			{ LED_DR_PORT &= (uint8_t) ~LATCH_PIN;	}
#define LATCH_PULSE			{ LATCH_UP; __asm__ __volatile__("nop"::); LATCH_DOWN; }

#define CLK_DOWN		{ LED_DR_PORT &= (uint8_t) ~CLK_PIN; 	}
#define CLK_UP			{ LED_DR_PORT |= CLK_PIN; 	}

#define RED_LEDS		0x0090
#define GREEN_LEDS		0x0048
#define BLUE_LEDS		0x0024


#define RED_LS  	(0b001)
#define GREEN_LS  	(0b100)
#define BLUE_LS  	(0b010)
#define ALL_LS		(RED_LS | GREEN_LS | BLUE_LS)

#define LED_FIRST_RED		((uint16_t)1<<8)
#define LED_FIRST_GREEN		((uint16_t)1<<9)
#define LED_FIRST_BLUE		((uint16_t)1<<10)

#define LED_SECOND_RED		((uint16_t)1<<11)
#define LED_SECOND_GREEN	((uint16_t)1<<12)
#define LED_SECOND_BLUE		((uint16_t)1<<13)

#define PWM 0x80

extern void LedDriver_Init(void);
extern void LedDriver_SwitchLeds(uint16_t);
extern void LedDriver_PWM(uint8_t pwm, uint16_t leds);
extern void LedDriver_Switch_3_Leds(uint8_t data);


#endif
