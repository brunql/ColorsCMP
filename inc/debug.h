//================================================//
//==					debug.h					==//
//================================================//
// Date: 21.01.2010
// Author  : brunql
//================================================//

#define DEBUG	1

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
//#include <stdio.h>

#define BAUD 9600

#include <util/setbaud.h>

#define DEBUG_INIT() \
	do{ if(DEBUG) {	\
			uart_initialize();	\
			_delay_ms(500);	\
		}	\
	}while(0)

#define DEBUG_PRINT_MESSAGE_P( MESSAGE ) \
	do{ if(DEBUG) uart_putstring_P( PSTR( MESSAGE ) ); }while(0)

#define DEBUG_PRINT_CHAR( CHAR ) \
	do{ if(DEBUG) uart_putchar( CHAR ); }while(0)

#define DEBUG_PRINT_UINT16( UINT16 ) \
	do{ \
		if(DEBUG){ \
			uart_putchar( (uint8_t) ((UINT16 >> 8) & 0x00ff) );	\
			uart_putchar( (uint8_t) (UINT16 & 0x00ff) ); \
		} \
	}while(0)

// USART Transmitter: 8 Data, 1 Stop, No Parity, Asynchronous, 9600
#define UCSRA_VALUE		(0x00)
#define UCSRB_VALUE 	(_BV(TXEN))
#define UCSRC_VALUE 	(_BV(URSEL) | _BV(USBS) | _BV(UCSZ1) | _BV(UCSZ0))

extern void uart_initialize(void);
extern void uart_putchar(uint8_t c); //, FILE *stream);
extern void uart_putstring_P(const char * strPtr); //NOTE: use only with PSTR("string");
