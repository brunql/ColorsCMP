//================================================//
//==		lcd_nokia_3310_frm_brunql.h			==//
//================================================//
// Date: 21.01.2010
// Author  : brunql
//================================================//

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

// TQFP mega8 pinout
/* VDD      +3,3V */            /* lcd_1 */
/* SCLK */ #define SPI_CLK_PIN  /* lcd_2 */    _BV(PB7)
/* SDIN */ #define SPI_MOSI_PIN /* lcd_3 */    _BV(PB5)
/* D/C  */ #define LCD_DC_PIN   /* lcd_4 */    _BV(PB4)
/* SCE  */ #define LCD_CE_PIN   /* lcd_5 */    _BV(PB3)
/* GND  */                      /* lcd_6 */
/* VOUT    4,7uF to GND */      /* lcd_7 */
/* RES  */ #define LCD_RST_PIN  /* lcd_8 */    _BV(PB2)

#define LCD_CONTRAST		30
#define LCD_X_RES			84
#define LCD_Y_RES			48
#define LCD_XY_RES			/*(LCD_X_RES * LCD_Y_RES / 8)==*/ 504
//#define LCD_CENTER_RES			/*(LCD_X_RES * LCD_Y_RES / 8) - 84 * 2==*/ 336
#define LCD_CENTER_RES			/*(LCD_X_RES * LCD_Y_RES / 8) - 84==*/ 420 /* without brunql@gmail.com */


#ifdef USING_RU_FONTS
/* Check CHAR and read from FontLoopup_RU or FontLookup */
#define PGM_GET_BYTE_FROM_FONTLOOKUP(OUT, CHAR, INDEX) 			\
	{ 															\
		if ( (CHAR) >= 0xC0){ /* RU */							\
			OUT = pgm_read_byte									\
				(												\
					(uint16_t) *FontLookup_RU + 				\
					(uint16_t)((uint16_t)(CHAR)-0xC0) * 5 + 	\
					(uint16_t)(INDEX)							\
				);												\
		}else{	/* Default */									\
			OUT = pgm_read_byte									\
				(												\
					(uint16_t) *FontLookup + 					\
					(uint16_t)((uint16_t)(CHAR)-0x20) * 5 + 	\
					(uint16_t)(INDEX)							\
				) << 1;											\
		}														\
	}															
/* END PGM_GET_BYTE_FROM_FONTLOOKUP */
#else
#define PGM_GET_BYTE_FROM_FONTLOOKUP(OUT, CHAR, INDEX) 		\
	{ 														\
		OUT = pgm_read_byte									\
			(												\
				(uint16_t) FontLookup + 					\
				(uint16_t)((uint16_t)(CHAR)-0x20) * 5 + 	\
				(uint16_t)(INDEX)							\
			) << 1;											\
	}
#endif

typedef enum { LCD_CMD, LCD_DATA } LcdCmdData;
typedef enum { BLACK_TEXT_ON_WHITE, WHITE_TEXT_ON_BLACK } WhiteOrBlackText;

extern void Lcd3310_Send (char data, LcdCmdData cd);
extern void Lcd3310_Init(uint16_t show_init_data_delay_ms);
extern void Lcd3310_Char(unsigned char ch, WhiteOrBlackText is_invert_colors);
extern void Lcd3310_String_P(PGM_P str_ptr, WhiteOrBlackText is_invert_colors);
extern void Lcd3310_String_P_anime(PGM_P str_ptr, PGM_P str_ptr_next, WhiteOrBlackText is_invert_colors, char num_of_cadr, char is_next);
extern void Lcd3310_GotoXY(uint16_t x, uint16_t y);
extern void Lcd3310_ClearCenter(void);
extern void Lcd3310_UInt8AsText(uint8_t ch, WhiteOrBlackText is_invert_colors);
extern void Lcd3310_UInt16AsText(uint16_t i, WhiteOrBlackText is_invert_colors);
