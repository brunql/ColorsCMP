/*
 * uint8_16_to_string.c
 *
 *  Created on: 11.02.2010
 *      Author: brunql
 */

#include <avr/io.h>
#include "uint8_16_to_string.h"

char to_string_result[] = {0, 0, 0, 0};

char HalfByteAsASCII(char c)
{
    if (c >= 0 && c <= 9){
        c += 0x30;
    }else if (c >= 0xA && c <= 0x0F){
        c += 0x37;
    }else{
        c = '?';
    }
    return c;
}

void UInt8ToString(uint8_t ch)
{
	to_string_result[0] = HalfByteAsASCII( (ch & 0xf0) >>4 );
	to_string_result[1] = HalfByteAsASCII( ch & 0x0f );
	//to_string_result[2] = 0x00; // end of result string here
	// result in result_str
}

void UInt16ToString(uint16_t i)
{
	//to_string_result[0] = HalfByteAsASCII( (i & 0xf000) >> 12 );
	to_string_result[1] = HalfByteAsASCII( (i & 0x0f00) >> 8  );
	to_string_result[2] = HalfByteAsASCII( (i & 0x00f0) >> 4  );
	to_string_result[3] = HalfByteAsASCII(  i & 0x000f		);
	// result in result_str
}
