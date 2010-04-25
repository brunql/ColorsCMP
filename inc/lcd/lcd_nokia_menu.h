//================================================//
//==			lcd_nokia_menu.h				==//
//================================================//
// Date: 21.01.2010
// Author  : brunql
//================================================//

#ifndef _LCD_NOKIA_MENU_INCLUDED_
#define _LCD_NOKIA_MENU_INCLUDED_ 1

#include <avr/pgmspace.h> // for PGM_P

//================================================//
//==				#defines					==//
#ifndef NULL
    #define NULL 0
#endif

#define CHARS_IN_LINE   14 
#define LCD_CONTRAST    30

#define LCD_FIRST_STR  ("--Colors CMP--")

//================================================//



//================================================//
//==				Variables					==//

struct menu_item {
	unsigned char cmd;
	
	struct menu_item * main_menu;
	struct menu_item * sub_menu;

	struct menu_item * next;
	struct menu_item * prev;

	PGM_P str;
};



extern uint8_t menu_before_now; // number of elements, that will be shown before menu_now element

extern struct menu_item * menu_now;

extern struct menu_item go;
	extern struct menu_item results;
extern struct menu_item snake_start;
	extern struct menu_item snake_results;
extern struct menu_item setup;
	extern struct menu_item calibration;
	extern struct menu_item set_measure_delay;
	extern struct menu_item back_setup;
extern struct menu_item readme;
	extern struct menu_item text;

//================================================//



//================================================//
//==			Functions prototypes			==//
extern void Lcd3310_InitializeDisplay(uint16_t);
extern void Lcd3310_UpdateDisplayInfo(void);
extern void Lcd3310_Message_P(double, PGM_P str);
extern void Menu_EnterClick(void);
extern void Lcd3310_AnimationSwitchMenuItems(char is_turn_left);
extern void GetValuesFromADC(void);
//================================================//

#endif
