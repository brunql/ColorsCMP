//================================================//
//==			SNAKE MAIN GAME FILE			==//
//==											==//
//==	File: SnakeGame.h						==//
//================================================//

// How many _timer_ ticks i should leave before take next step?
extern uint8_t step_speed; // uses in timer interrupts
extern uint8_t get_present_speed;
// How many _speed_ ticks i should leave before call SnakeGame_AddmeUpdateXY()?
#define SPEED_STEP						30
#define SPEED_TICKS_TO_GET_PRESENT 		200

void SnakeGame_Start(void);
void SnakeGame_Stop(void);
void SnakeGame_TakeNextStep(void);
void SnakeGame_TurnLeft(void);
void SnakeGame_TurnRight(void);

// addme - is a point that i add to snake if you stepped on it
// set rand() numbers to x and y of addme
void SnakeGame_AddmeUpdateXY(void);
