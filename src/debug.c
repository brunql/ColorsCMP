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



//void DebugSendAllResultValues(void)
//{
//	ATOMIC_BLOCK( ATOMIC_RESTORESTATE ){
//		DEBUG_PRINT_CHAR( 'C' );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[CALIBRATE_INDX][RED] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[CALIBRATE_INDX][GREEN] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[CALIBRATE_INDX][BLUE] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[CALIBRATE_INDX][RED_GREEN] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[CALIBRATE_INDX][RED_BLUE] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[CALIBRATE_INDX][GREEN_BLUE] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[CALIBRATE_INDX][ALL] ) );
//
//		DEBUG_PRINT_CHAR( 'M' );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[MEASURE_INDX][RED] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[MEASURE_INDX][GREEN] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[MEASURE_INDX][BLUE] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[MEASURE_INDX][RED_GREEN] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[MEASURE_INDX][RED_BLUE] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[MEASURE_INDX][GREEN_BLUE] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[MEASURE_INDX][ALL] ) );
//
//		DEBUG_PRINT_CHAR( 'D' );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[DIFF_INDX][RED] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[DIFF_INDX][GREEN] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[DIFF_INDX][BLUE] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[DIFF_INDX][RED_GREEN] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[DIFF_INDX][RED_BLUE] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[DIFF_INDX][GREEN_BLUE] ) );
//		DEBUG_PRINT_UINT16( hex2dec_result( result[DIFF_INDX][ALL] ) );
//
//		DEBUG_PRINT_CHAR( 'E' ); //  End
//	}
////	DEBUG_PRINT_UINT16( ( result[CALIBRATE_INDX][RED] ) );
////	DEBUG_PRINT_UINT16( ( result[CALIBRATE_INDX][GREEN] ) );
////	DEBUG_PRINT_UINT16( ( result[CALIBRATE_INDX][BLUE] ) );
////
////	DEBUG_PRINT_CHAR( 'M' );
////	DEBUG_PRINT_UINT16( ( result[MEASURE_INDX][RED] ) );
////	DEBUG_PRINT_UINT16( ( result[MEASURE_INDX][GREEN] ) );
////	DEBUG_PRINT_UINT16( ( result[MEASURE_INDX][BLUE] ) );
////
////	DEBUG_PRINT_CHAR( 'D' );
////	DEBUG_PRINT_UINT16( ( result[DIFF_INDX][RED] ) );
////	DEBUG_PRINT_UINT16( ( result[DIFF_INDX][GREEN] ) );
////	DEBUG_PRINT_UINT16( ( result[DIFF_INDX][BLUE] ) );
//}

#endif
