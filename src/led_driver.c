//================================================//
//==				led_driver.c				==//
//================================================//
// Date: 21.01.2010
// Author  : brunql
//================================================//

#include "led_driver.h"


void LedDriver_SwitchLeds(uint16_t data)
{
	char i;
	for(i=0; i<16; i++){
		CLK_DOWN;
		if(data & _BV(i)){
			LED_DR_PORT |= DATA_PIN;
		}else{
			LED_DR_PORT &= (unsigned char)~DATA_PIN;
		}
		CLK_UP;
	}
	LATCH_PULSE;
}

void LedDriver_PWM(uint8_t pwm, uint16_t leds)
{
	uint8_t temp = pwm;
	LedDriver_SwitchLeds(leds); // switch on leds
	while(temp--){__asm__ __volatile__("nop");}
	temp = (uint8_t)~pwm;
	LedDriver_SwitchLeds(0x00); // switch off all leds
	while(temp--){__asm__ __volatile__("nop");}
	LedDriver_SwitchLeds(0x00);
}

void LedDriver_Init(void)
{
	LATCH_DOWN;
	CLK_DOWN;
	LED_DR_DDR |= DATA_PIN | CLK_PIN | LATCH_PIN;
	LedDriver_SwitchLeds(0x00);
}

void LedDriver_Switch_3_Leds(uint8_t data)
{
	// data mb: RED_LS, GREEN_LS, BLUE_LS

	for(uint8_t i=0; i < 3; i++){
		CLK_DOWN;
		if(data & _BV(i)){
			LED_DR_PORT |= DATA_PIN;
		}else{
			LED_DR_PORT &= (uint8_t)~DATA_PIN;
		}
		CLK_UP;
	}
	LATCH_PULSE;
}
