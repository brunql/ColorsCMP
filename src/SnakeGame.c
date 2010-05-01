//================================================//
//==			SNAKE MAIN GAME FILE			==//
//==											==//
//==	File: SnakeGame.c						==//
//==	Author: brunql							==//
//==	Date: 29.01.10							==//
//================================================//
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>		// for rand()

#include "main.h"
#include "lcd/lcd_nokia_menu.h"
#include "lcd/lcd_nokia_3310_frm_brunql.h"

#include "SnakeGame.h"

#define SNAKE_LEN 			10
#define SNAKE_START_LEN  	6

enum DIRECTION {
	// Each point has direction, there she will go.
	UP 		= 1,
	RIGHT 	= 2,
	DOWN 	= 3,
	LEFT	= 4
};

typedef struct {
	// Coordinates of a point from the upper left corner
	uint8_t	x;	// mb: 1..83
	uint8_t	y;	// mb: 8..47
	enum DIRECTION direction;
} point;

PGM_P snake_str = "--Snake game--";

PGM_P uwin_str = "You win!";
uint8_t uwin_times = 0x00; 	// how many times you win?
PGM_P score_str = "Score: 0x";
uint8_t score = 0x00;		// 0xX0, X = uwin_times; 0x0X, X = snake_add_index
PGM_P level_str = "Level: #";

PGM_P got_fun_str = "Got fun?";
PGM_P go_work_str = "Go work!";
PGM_P brunql_str = " brunql";


uint8_t step_speed = SPEED_STEP;
uint8_t get_present_speed = SPEED_TICKS_TO_GET_PRESENT;

uint8_t addme_x = 70;
uint8_t addme_y = 20;

// New point add to snake[ snake_add_index ], also this is length of snake
uint8_t snake_add_index = 0x00;

// Let me introduce myself I'm a snaaake!!! }:->
point snake[ SNAKE_LEN ] =
{
	{1, 8, DOWN},	// 0 	this is my head
	{1, 8, DOWN},	// 1	this is my body
	{1, 8, DOWN},	// 2	and this
	{1, 8, DOWN},	// 3	this is my ass... snake_add_index here
	{1, 8, DOWN},	// 4
	{1, 8, DOWN},	// 5
	{1, 8, DOWN},	// 6
	{1, 8, DOWN},	// 7
	{1, 8, DOWN},	// 8
	{1, 8, DOWN}	// 9
};



//================================================//
// Local header(s):
void snakegame_DirectionStep(point *p_snake);
//void snakegame_AddSnakePoint(void);
void snakegame_UpdateDisplay(void);
//================================================//


void snakegame_InitSnake(void)
{
	snake_add_index = SNAKE_START_LEN; // start length of snake
	score = (uwin_times<<4) | (snake_add_index - SNAKE_START_LEN);
	for(uint8_t i=0; i<SNAKE_LEN; i++){
		snake[i].y = 20;
		snake[i].x = 40 - i;
		snake[i].direction = RIGHT;
	}
}


void SnakeGame_Start(void)
{
	SnakeGame_AddmeUpdateXY();
	snakegame_InitSnake();

	Lcd3310_GotoXY(0, 0);
	Lcd3310_String_P(snake_str, WHITE_TEXT_ON_BLACK);
	snakegame_UpdateDisplay();
}

void SnakeGame_Stop(void)
{
	// show results and go to main menu
}

void SnakeGame_TakeNextStep(void)
{

	// Lets go somethere:
	for(uint8_t i=0; i < SNAKE_LEN; i++)
	{
		snakegame_DirectionStep(&snake[i]);
	}

	// Update direction of each point from last to first
	for(uint8_t i = SNAKE_LEN-1; i != 0 && i < SNAKE_LEN; i--)
	{
		snake[i].direction = snake[i-1].direction;
	}

	// Check if we come to addme point
	if(snake[0].x == addme_x && snake[0].y == addme_y){
		SnakeGame_AddmeUpdateXY();
		snake_add_index++; // add snake point
		score = (uwin_times<<4) | (snake_add_index - SNAKE_START_LEN);
		if(snake_add_index >= SNAKE_LEN){
			Lcd3310_ClearCenter();
			uwin_times++;
			if(uwin_times >= 3){
				uwin_times = 0x00;
				Lcd3310_GotoXY(3, 2);
				Lcd3310_String_P(got_fun_str, BLACK_TEXT_ON_WHITE);
				Lcd3310_GotoXY(3, 3);
				Lcd3310_String_P(go_work_str, BLACK_TEXT_ON_WHITE);
				Lcd3310_GotoXY(0, 5);
				Lcd3310_String_P(brunql_str, WHITE_TEXT_ON_BLACK);
				FLAGS_SWITCH_ON( JOYSTICK_CENTER_CLICK_FLAG ); // stop game! go to main menu.

				get_present_speed = SPEED_TICKS_TO_GET_PRESENT;
				step_speed = SPEED_STEP;
			}else{
				Lcd3310_GotoXY(3, 2);
				Lcd3310_String_P(uwin_str, BLACK_TEXT_ON_WHITE);
				Lcd3310_GotoXY(2, 3);
				Lcd3310_String_P(level_str, BLACK_TEXT_ON_WHITE);
				Lcd3310_UInt8AsText(uwin_times, BLACK_TEXT_ON_WHITE);
				Lcd3310_GotoXY(0, 5);
				Lcd3310_String_P(score_str, WHITE_TEXT_ON_BLACK);
				Lcd3310_UInt8AsText(score, WHITE_TEXT_ON_BLACK);
				Lcd3310_Char(' ', WHITE_TEXT_ON_BLACK);

				get_present_speed /= 1.5;
				step_speed -= 3 * uwin_times;
			}
			while(JOYSTICK_INT_CHECK());
			_delay_ms(2);
			GIFR = _BV(INTF1);


			timer_ticks_to_get_present = get_present_speed;
			timer_delay = step_speed;

			score = (uwin_times<<4);
			SnakeGame_Start();
		}
	}

	snakegame_UpdateDisplay();
}


void SnakeGame_TurnLeft(void)
{
	// directions mb: UP = 1, RIGHT = 2, DOWN = 3, LEFT = 4
	if( --snake[0].direction == 0){
		snake[0].direction = LEFT;
	}
}

void SnakeGame_TurnRight(void)
{
	if( ++snake[0].direction == 5){
		snake[0].direction = UP;
	}
}


void SnakeGame_AddmeUpdateXY(void)
{
	addme_x = (rand() % 79) + 2; 	// 2..81
	addme_y = (rand() % 28) + 10; // 10..38
}




void snakegame_UpdateDisplay(void)
{
	Lcd3310_ClearCenter();

	Lcd3310_GotoXY(0, 5);
	Lcd3310_String_P(score_str, WHITE_TEXT_ON_BLACK);
	Lcd3310_UInt8AsText(score, WHITE_TEXT_ON_BLACK);
	//Lcd3310_Char(' ', WHITE_TEXT_ON_BLACK);
	//Lcd3310_UInt8AsText(snake[0].x, WHITE_TEXT_ON_BLACK);
	//Lcd3310_Char(' ', WHITE_TEXT_ON_BLACK);
	//Lcd3310_UInt8AsText(snake[0].y, WHITE_TEXT_ON_BLACK);

	uint16_t lcd_index = 0x00;

	// eval values for addme pixel show
	uint8_t addme_line = (uint8_t)addme_y / 8;
	uint8_t addme_display_byte = 1 << ((uint8_t)addme_y % 8);

	// Lets draw each point in snake to our lcd.
	for(uint8_t i=0; i < snake_add_index; i++)
	{
		// GotoXY
		// Eval number of line (mb: 1..5) in display for display_byte
		uint8_t line = (uint8_t) (snake[i].y) / 8;
		lcd_index = (uint16_t) (snake[i].x) + (uint16_t)line * 84;
		Lcd3310_Send( 0x80 | ((uint16_t)lcd_index % LCD_X_RES), LCD_CMD );
		Lcd3310_Send( 0x40 | ((uint16_t)lcd_index / LCD_X_RES), LCD_CMD );

		// Make sendme byte with all point in this display byte
		// Init with snake[i] point
		uint8_t display_byte = 1 << ((uint8_t)snake[i].y % 8);
		// Find all point near here (in this display byte)
		for(uint8_t j=0; j<snake_add_index; j++)
		{
			uint8_t line_find = (uint8_t)snake[j].y / 8;
			if(snake[i].x == snake[j].x && line == line_find){
				display_byte |= 1 << ((uint8_t)snake[j].y % 8);
			}
		}

		// check that snake[i] and addme points are in same display_byte
		if(addme_x == snake[i].x && addme_line == line){
			display_byte |= (addme_display_byte & 0x7f);
			addme_display_byte |= 0x80; // say that we already draw this point
		}

		Lcd3310_Send( display_byte, LCD_DATA );
	}


	// if we didn't draw it before lets do it now
	if(!(addme_display_byte & 0x80)){
		lcd_index = (uint16_t)addme_x + (uint16_t)addme_line * 84;
		Lcd3310_Send( 0x80 | (lcd_index % LCD_X_RES), LCD_CMD );
		Lcd3310_Send( 0x40 | (lcd_index / LCD_X_RES), LCD_CMD );
		Lcd3310_Send( addme_display_byte, LCD_DATA );
	}
}


//void snakegame_AddSnakePoint(void)
//{
//	if(snake_add_index >= 5){
//		Lcd3310_ClearCenter();
//		Lcd3310_GotoXY(4, 3);
//		Lcd3310_String_P(uwin, WHITE_TEXT_ON_BLACK);
//		_delay_ms(1000);
//		FLAGS_SWITCH_ON( SNAKE_STOP_GAME_FLAG );
//	}
//
//	// TODO: Need test!
//	// Set for new point values of previous
//	snake[snake_add_index].x = snake[snake_add_index -1].x;
//	snake[snake_add_index].y = snake[snake_add_index -1].y;
//	snake[snake_add_index].direction = snake[snake_add_index -1].direction;
//
//	// After that do one step backward!
//	// turn back
//	if(snake[snake_add_index].direction == UP || snake[snake_add_index].direction == RIGHT){
//		snake[snake_add_index].direction += 2;
//	}else{
//		snake[snake_add_index].direction -= 2;
//	}
//
//	// we are now can go back
//	snakegame_DirectionStep(&snake[snake_add_index]);
//
//	// restore direction
//	snake[snake_add_index].direction = snake[snake_add_index -1].direction;
//
//	snake_add_index++; // don't forget to update index
//}

void snakegame_DirectionStep(point *p_snake)
{
	switch(p_snake->direction)
	{
		case UP:
			if(--p_snake->y < 9) 	p_snake->y = 38;
			break;
		case DOWN:
			if(++p_snake->y > 38) 	p_snake->y = 9;
			break;
		case LEFT:
			if(--p_snake->x < 2) 	p_snake->x = 82;
			break;
		case RIGHT:
			if(++p_snake->x > 81) 	p_snake->x = 2;
			break;
		default: // error
			p_snake->x = 0;
			p_snake->y = 17;
			p_snake->direction = RIGHT;
			break;
	}
}
