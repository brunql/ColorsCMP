//================================================//
//==		lcd_nokia_3310_frm_brunql.c			==//
//================================================//
// Date: 21.01.2010
// Author  : brunql
//================================================//

#include "main.h" // #ifdef compilation
#include "lcd/lcd_nokia_3310_frm_brunql.h" // include's io.h, pgmspace.h and delay.h

#include "lcd/lcd_nokia_3310_font_lookup.h"
#include "lcd/lcd_nokia_3310_images.h"

#include "uint8_16_to_string.h"


//#include <stdlib.h>


uint16_t Lcd3310_Index = 0x00;



void Lcd3310_InitFill (void);



void Lcd3310_Init(uint16_t show_init_data_delay_ms)
{
	//  Pull-up on reset pin.
    PORTB |= LCD_RST_PIN;

    //  Set output bits on port B.
    DDRB |= LCD_RST_PIN | LCD_DC_PIN | LCD_CE_PIN | SPI_MOSI_PIN | SPI_CLK_PIN;

    _delay_ms(50);

    //  Toggle display reset pin.
    PORTB &= (unsigned char)~LCD_RST_PIN;
    _delay_ms(1);
    PORTB |= LCD_RST_PIN;

    //  Enable SPI port: No interrupt, MSBit first, Master mode, CPOL->0, CPHA->0, Clk/4
    SPCR = 0x50;

    //  Disable LCD controller
    PORTB |= LCD_CE_PIN;

    Lcd3310_Send( 0x21, LCD_CMD );  // LCD Extended Commands.
    Lcd3310_Send( 0xC8, LCD_CMD );  // Set LCD Vop (Contrast).
    Lcd3310_Send( 0x06, LCD_CMD );  // Set Temp coefficent.
    Lcd3310_Send( 0x13, LCD_CMD );  // LCD bias mode 1:48.
    Lcd3310_Send( 0x20, LCD_CMD );  // LCD Standard Commands, Horizontal addressing mode.
    Lcd3310_Send( 0x0C, LCD_CMD );  // LCD in normal mode.

	Lcd3310_InitFill();

    //  LCD Extended Commands.
    Lcd3310_Send( 0x21, LCD_CMD );

    // Set LCD Vop (Contrast).
    Lcd3310_Send( 0x80 | LCD_CONTRAST, LCD_CMD );

    //  LCD Standard Commands, horizontal addressing mode.
    Lcd3310_Send( 0x20, LCD_CMD );

	_delay_ms(show_init_data_delay_ms); // show image from LcdCache initial value
}

void Lcd3310_Char(unsigned char ch, WhiteOrBlackText is_invert_colors)
{
	//  Convert to a printable character.
    if ( (ch < 0x20) ){
        ch = 92;
    }
	
	Lcd3310_Send( 0x80 | (Lcd3310_Index % LCD_X_RES), LCD_CMD );
    Lcd3310_Send( 0x40 | (Lcd3310_Index / LCD_X_RES), LCD_CMD );
	
	for (char i = 0; i < 5; i++ ){		
		char temp;
		PGM_GET_BYTE_FROM_FONTLOOKUP(temp, ch, i );
		if(is_invert_colors) temp = (unsigned char) ~temp;
	    Lcd3310_Send( temp, LCD_DATA );
    }
	if(is_invert_colors) Lcd3310_Send( 0xff, LCD_DATA );
	else Lcd3310_Send( 0x00, LCD_DATA );
	Lcd3310_Index += 6;
}

void Lcd3310_String_P(PGM_P str_ptr, WhiteOrBlackText is_invert_colors)
{
	char count = 0;
    while ( *str_ptr ){
		count++;
        Lcd3310_Char( *(str_ptr++), is_invert_colors );
    }
	if(is_invert_colors && count > 0 && count < 14){
		for(count = (14 - count) * 6; count > 0; count--){
			Lcd3310_Send( 0xff, LCD_DATA );
		}
	}
}

#ifdef ANIMATION_SWITCH_MENU_ITEMS
void Lcd3310_String_P_anime(PGM_P str_ptr, PGM_P str_ptr_next, WhiteOrBlackText is_invert_colors, char num_of_cadr, char is_next)
{
	unsigned char ch1 = 1, ch2 = 1, display, temp1, temp2, count = 0;

	Lcd3310_Send( 0x80 | (Lcd3310_Index % LCD_X_RES), LCD_CMD );
    Lcd3310_Send( 0x40 | (Lcd3310_Index / LCD_X_RES), LCD_CMD );
	
	while ( ch1 || ch2 ){
		count++;
		if(*str_ptr) ch1 = *(str_ptr++);
		else ch1 = 0x00;
		
		if(*str_ptr_next) ch2 = *(str_ptr_next++);
		else ch2 = 0x00;

		for (char i = 0; i < 5; i++ ){		
			if(ch1) PGM_GET_BYTE_FROM_FONTLOOKUP(temp1, ch1, i )
			else temp1 = 0x00;

			if(ch2) PGM_GET_BYTE_FROM_FONTLOOKUP(temp2, ch2, i )
			else temp2 = 0x00;

			if(is_next) display = (temp1 >> num_of_cadr) | ( (temp2 << (8 - num_of_cadr)) );
			else 		display = (temp1 << num_of_cadr) | ( (temp2 >> (8 - num_of_cadr)) );
			if(is_invert_colors) display = (unsigned char) ~display;
		    Lcd3310_Send( display, LCD_DATA );
	    }
		if(is_invert_colors) Lcd3310_Send( 0xff, LCD_DATA );
		else Lcd3310_Send( 0x00, LCD_DATA );
	}
	if(is_invert_colors && (count < 14)){
		for(count = (14 - count) * 6; count > 0; count--){
			Lcd3310_Send( 0xff, LCD_DATA );
		}
	}
}
#endif

void Lcd3310_GotoXY(uint16_t x, uint16_t y)
{
    Lcd3310_Index = (uint16_t)x * 6 + (uint16_t)y * 84;
}

void Lcd3310_ClearCenter(void)
{
#ifdef LCD_IMAGES //  clear only center lines
	Lcd3310_Send( 0x80 | (84 % LCD_X_RES), LCD_CMD );
    Lcd3310_Send( 0x40 | (84 / LCD_X_RES), LCD_CMD );
	for(uint16_t i=0; i < LCD_CENTER_RES; i++){
		Lcd3310_Send( 0x00, LCD_DATA );
	}
#else // clear all...
	Lcd3310_Send( 0x80, LCD_CMD );
	Lcd3310_Send( 0x40, LCD_CMD );
	for(uint16_t i=0; i < LCD_XY_RES; i++){
		Lcd3310_Send( 0x00, LCD_DATA );
	}
#endif
}





void Lcd3310_UInt8AsText(uint8_t ch, WhiteOrBlackText is_invert_colors)
{
	UInt8ToString( ch );
	for(uint8_t count=0; count < 2; count++){
		Lcd3310_Char( to_string_result[count], is_invert_colors );
	}
}

void Lcd3310_UInt16AsText(uint16_t i, WhiteOrBlackText is_invert_colors)
{
	UInt16ToString( i );
	// show three numbers and space
	for(uint8_t count=1; count < 4; count++){ // 0x03ff - max value, show only '3ff'
			Lcd3310_Char( to_string_result[count], is_invert_colors );
	}
	Lcd3310_Char( ' ', is_invert_colors );
}


void Lcd3310_InitFill(void)
{
#ifdef LCD_IMAGES
	// Goto (0, 0) of display
	Lcd3310_Send( 0x80, LCD_CMD );
    Lcd3310_Send( 0x40, LCD_CMD );

	for (int i = 0; i < 84; i++ ){
        Lcd3310_Send( pgm_read_byte( LcdImage_Upper + i ), LCD_DATA );
    }	
	for (uint16_t i=0; i < LCD_CENTER_RES; i++ ){
        Lcd3310_Send( pgm_read_byte( LcdImage_Center + i), LCD_DATA );
	}
    for (int i = 0; i < 84; i++ ){
        Lcd3310_Send( pgm_read_byte( LcdImage_brunql_gmail_com + i), LCD_DATA );
    }
#else
    Lcd3310_ClearCenter();
#endif
}


void Lcd3310_Send (char data, LcdCmdData cd)
{
	//  Enable display controller (active low).
    PORTB &= (unsigned char)~LCD_CE_PIN;

    if ( cd == LCD_DATA )
    {
        PORTB |= (unsigned char)LCD_DC_PIN;
    }
    else
    {
        PORTB &= (unsigned char)~LCD_DC_PIN;
    }

    //  Send data to display controller.
    SPDR = data;

    //  Wait until Tx register empty.
    while ( (SPSR & _BV(SPIE)) == 0 );

    //  Disable display controller.
    PORTB |= (unsigned char)LCD_CE_PIN;
}
