//================================================//
//==					debug.c					==//
//================================================//
// Date: 21.01.2010
// Author  : brunql
//================================================//

#include "debug.h"

#if DEBUG > 0

void uart_initialize(void)
{
	UCSRA = UCSRA_VALUE;
    UCSRB = UCSRB_VALUE;
    UCSRC = UCSRC_VALUE;
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
}


void uart_putchar(uint8_t c)
{
	if(c == '\n'){
		uart_putchar('\r');
	}
	loop_until_bit_is_set( UCSRA, UDRE );
	UDR = c;
} 

void uart_putstring_P(const char *strPtr)
{
	uint8_t temp;
	while ( (temp = pgm_read_byte ( strPtr++ )) ){
		uart_putchar( temp );
	}
}

#endif
