//================================================//
//==			lcd_nokia_menu.c				==//
//================================================//
// Date: 21.01.2010
// Author  : brunql
//================================================//

#include <avr/pgmspace.h>
#include "main.h"
#include "lcd/lcd_nokia_menu.h"
#include "led_driver.h"
#include "algorithm.h"


double callibrate = 0x00; // == adc_res_first / adc_res_second;

uint8_t menu_before_now = 0x80; // because
struct menu_item * menu_now;

enum {
	BACK_CMD,
	ENTER_MENU_CMD,
	go_cmd,
	results_cmd,
	calibration_cmd,
	set_zero_cmd
};

#ifdef LCD_FIRST_STR
	PGM_P lcd_first_str = LCD_FIRST_STR;
#endif


struct menu_item *menu_now_atomic;

//					name = 	{ CMD, 		main_menu, 	sub_menu, 	next, 	previous, string }; 
struct menu_item go = {go_cmd, NULL, &results, &setup, &readme, 								"// Start    //"};
	struct menu_item results = {results_cmd, &go, NULL, &results, &results, 					"// Results: //"};
struct menu_item setup = {ENTER_MENU_CMD, NULL, &calibration, &readme, &go, 					"// Setup    //"};
	struct menu_item calibration = {calibration_cmd, &setup, NULL, &set_zero, &back_setup, 		"//Callibrate//"};
	struct menu_item set_zero = {set_zero_cmd, &setup, NULL, &back_setup, &calibration, 		"// Set zero //"};
	struct menu_item back_setup = {BACK_CMD, &setup, NULL, &calibration, &set_zero, 			"// Back     //"};
struct menu_item readme = {ENTER_MENU_CMD, NULL, &text, &go, &setup, 							"// Readme   //"};
	struct menu_item text = {BACK_CMD, &readme, NULL, &text, &text, "[ Programmer:][   brunql   ][    (at)    ][  gmail.com ]"};

void Menu_EnterClick(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { menu_now_atomic = menu_now; }
	switch(menu_now_atomic->cmd){
		default:
			break;
		case ENTER_MENU_CMD:
		enter_menu_label:
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { menu_now = menu_now->sub_menu; }
			break;

		case BACK_CMD:
		back_menu_label:
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { menu_now = menu_now->main_menu; }
			break;
		
		case results_cmd:
			LedDriver_SwitchLeds( 0x0000 );
			goto back_menu_label;
	
		case go_cmd:
			FLAGS_SWITCH_ON( ADC_RESULT_FLAG );
			goto enter_menu_label;
		
		case calibration_cmd:
			FLAGS_SWITCH_ON( ADC_CALIBRATE_FLAG );
			break;
		case set_zero_cmd:
			FLAGS_SWITCH_ON( ADC_SET_ZERO );
			break;
	}
}



void init_menu_now(void)
{
	menu_now = &go;
}


//void Lcd3310_Message_P(double ms, PGM_P str)
//{
//	Lcd3310_ClearCenter();
//    Lcd3310_GotoXY(0, 1);
//    Lcd3310_String_P( str, 0 );
//    _delay_ms(ms);
//}


//==============================================//
void Lcd3310_InitializeDisplay(uint16_t show_image_delay_ms)
{
	init_menu_now();

    Lcd3310_Init(show_image_delay_ms);
	//Lcd3310_LcdImageUpperBottom_Fill();
    Lcd3310_UpdateDisplayInfo();
}
//==============================================//

#ifdef ANIMATION_SWITCH_MENU_ITEMS
void Lcd3310_AnimationSwitchMenuItems(char is_turn_left)
{
	uint8_t number_of_elements_in_menu_now = 1;
	struct menu_item *menu_temp;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { 
		menu_now_atomic = menu_now; 
	}
	
	menu_temp = menu_now_atomic->next;
	while( menu_temp->str != menu_now_atomic->str ){
		number_of_elements_in_menu_now++;
		menu_temp = menu_temp->next;
	}
	if(number_of_elements_in_menu_now > 4){
		number_of_elements_in_menu_now = 4;
	}

	Lcd3310_ClearCenter();
	if(number_of_elements_in_menu_now < 3){
		menu_temp = menu_now_atomic;
	}else{
		menu_temp = menu_now_atomic->prev;
	}

	for(uint8_t j=0; j<8; j++){
		for(uint8_t i=0; i < number_of_elements_in_menu_now; i++){
#			ifdef (LCD_IMAGES)
				Lcd3310_GotoXY(0, i + 1);// show menu from (i+1) line because first line filled with '_COLORS_'
#			else
				Lcd3310_GotoXY(0, i); // no images at top
#			endif
			Lcd3310_String_P_anime(
							(is_turn_left) ? menu_temp->prev->str : menu_temp->next->str,
							menu_temp->str,
							(menu_now_atomic == menu_temp), // WhiteOrBlackText
							j, // num_of_cadr
							is_turn_left // is_next
							);
			menu_temp = menu_temp->next;
		}
		if(number_of_elements_in_menu_now < 3){
			menu_temp = menu_now_atomic;
		}else{
			menu_temp = menu_now_atomic->prev;
		}
		_delay_ms(50);
	}
}
#endif


void Lcd3310_UpdateDisplayInfo(void)
{
    uint8_t number_of_elements_in_menu_now = 1;
    
	Lcd3310_ClearCenter();
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { menu_now_atomic = menu_now; }

	struct menu_item *menu_temp = menu_now_atomic->next;
	while( menu_temp != menu_now_atomic ){
		number_of_elements_in_menu_now++;
		menu_temp = menu_temp->next;
	}
	if(number_of_elements_in_menu_now > 4){
		number_of_elements_in_menu_now = 4;
	}
	 
#ifdef ANIMATION_SWITCH_MENU_ITEMS
	if(number_of_elements_in_menu_now < 3){
		menu_temp = menu_now_atomic;
	}else{
		menu_temp = menu_now_atomic->prev;
	}	
#else
	if(menu_before_now >= 0x80 + number_of_elements_in_menu_now){
		menu_before_now = 0x80 + number_of_elements_in_menu_now - 1;
	}else if (menu_before_now < 0x80){
		menu_before_now = 0x80;
	}

	menu_temp = menu_now_atomic;
	for(uint8_t i=1; i <= menu_before_now - 0x80; i++){
		menu_temp = menu_temp->prev;
	}
#endif

#	ifdef LCD_FIRST_STR
	Lcd3310_GotoXY(0, 0);
	Lcd3310_String_P(lcd_first_str, WHITE_TEXT_ON_BLACK);
#	endif

	for(uint8_t i=0; i < number_of_elements_in_menu_now; i++){
#		ifdef LCD_IMAGES
			Lcd3310_GotoXY(0, i + 1);
#		else
#			ifdef LCD_FIRST_STR
				Lcd3310_GotoXY(0, i + 2);// show menu from (i+1) line because first line filled with '_COLORS_'
#			else
				Lcd3310_GotoXY(0, i); // no images at top
#			endif
#		endif
		if(menu_temp->str == results.str){
			Lcd3310_GotoXY(0, i + 1);
		}
		Lcd3310_String_P(menu_temp->str, (menu_now_atomic == menu_temp));
		menu_temp = menu_temp->next;
	}


	if( menu_now_atomic == &results ){
		/*
		 * Result window:
		 *
		 * |    Results:  |
		 * |   R   G   B  |
		 * |1 *** *** *** |
		 * |2 *** *** *** |
		 * |% xx% xx% xx% |
		 * |  xxxxxxxxxxx |
		 *
		 */

		// |   R   G   B  |
		Lcd3310_GotoXY(3, 2);
		Lcd3310_Char('R', BLACK_TEXT_ON_WHITE);
		Lcd3310_GotoXY(7, 2);
		Lcd3310_Char('G', BLACK_TEXT_ON_WHITE);
		Lcd3310_GotoXY(11, 2);
		Lcd3310_Char('B', BLACK_TEXT_ON_WHITE);

		// |1 *** *** *** |
		Lcd3310_GotoXY(0, 3);
		Lcd3310_Char('1', BLACK_TEXT_ON_WHITE);
		Lcd3310_GotoXY(2, 3);
		Lcd3310_UInt16AsText(result[FIRST][RED], BLACK_TEXT_ON_WHITE);
		Lcd3310_UInt16AsText(result[FIRST][GREEN], BLACK_TEXT_ON_WHITE);
		Lcd3310_UInt16AsText(result[FIRST][BLUE], BLACK_TEXT_ON_WHITE);

		// |2 *** *** *** |
		Lcd3310_GotoXY(0, 4);
		Lcd3310_Char('2', BLACK_TEXT_ON_WHITE);
		Lcd3310_GotoXY(2, 4);
		Lcd3310_UInt16AsText(result[SECOND][RED], BLACK_TEXT_ON_WHITE);
		Lcd3310_UInt16AsText(result[SECOND][GREEN], BLACK_TEXT_ON_WHITE);
		Lcd3310_UInt16AsText(result[SECOND][BLUE], BLACK_TEXT_ON_WHITE);

		// |% **% **% **% |
		Lcd3310_GotoXY(0, 5);
		Lcd3310_Char('%', BLACK_TEXT_ON_WHITE);
		Lcd3310_GotoXY(2, 5);
		Lcd3310_UInt16AsText(result[PERCENT][RED], BLACK_TEXT_ON_WHITE);
		Lcd3310_UInt16AsText(result[PERCENT][GREEN], BLACK_TEXT_ON_WHITE);
		Lcd3310_UInt16AsText(result[PERCENT][BLUE], BLACK_TEXT_ON_WHITE);
	}
}


//void SwitchLedsAndADC(FirstOrSecond first, uint16_t leds)
//{
//	ENC_INT_DISABLE(); // turn on
//
//	LedDriver_PWM(PWM, leds);
//	if (first){
//		ADMUX &= (unsigned char) ~ _BV(MUX0);
//	}else{ // second
//		ADMUX |= _BV(MUX0);
//	}
//	//_delay_ms(10);
//	ADCSRA |= _BV( ADSC ); // start conversation
//	while( ADC_CONVERT_IN_PROGRESS() ) { LedDriver_PWM(PWM, leds); };
//
//	ENC_INT_ENABLE();
//	//result in adc_data
//}

