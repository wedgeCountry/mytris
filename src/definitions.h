#ifndef __DEFINES_H
#define __DEFINES_H


#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h> 
#include <util/delay.h>

#include "lcd.h"
#include "font_small.h"


/*///////////////////////////////////////////////////
// FOR PIN CONFIGURATIONS CHANGE THE FILE "MYLCD.H"//
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

// TETRIS
#define MODELS 	7
#define MLEN 	4
#define MAXI	10
#define MAXJ	20
	 
#define ERROR 	1
#define OK 		0
#define LOCK 	1
#define UNLOCK 	0


#define TETRIS_T 0x00
#define TETRIS_o 0x01
#define TETRIS_z 0x02
#define TETRIS_s 0x03
#define TETRIS_L 0x04
#define TETRIS_J 0x05
#define TETRIS_I 0x06

// which character will be displayed?
#define SYMBOL_T 'T'
#define SYMBOL_o 'o'
#define SYMBOL_z 'z'
#define SYMBOL_s 's'
#define SYMBOL_L 'L'
#define SYMBOL_J 'J'
#define SYMBOL_I 'I'

// LCD Configuration
#define FAKTOR 6
#define LCD_MAX 128
#define LCD_MAY 64

// CAREFUL! USED IN MODULES FOR ARRAY ACCESS!!
#define MAX_PLAYERS 2
#define PLAYER1 0x00 
#define PLAYER2 0x01

#define POS_PLAYER1 12
#define POS_PLAYER2 90

#define PLAYER1_X 42
#define PLAYER2_X 132

volatile uint8_t num_players;

#define WAIT_S _delay_ms(150);

#define DEBOUNCE _delay_ms(2)

#define CONTINUE 0x00
#define EXIT_GAME 0x01
#define RESUME_GAME 0x02
#define GAME_OVER 0x03

/*////////////////////////////////////
// MENU 
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#define MENU_WELCOME 0x00
#define MENU_START 0x01
#define MENU_OPTIONS 0x02
#define MENU_PLAY 0x03
#define MENU_RETURN 0x04
#define MENU_SOUND 0x05
#define MENU_GAME_OVER 0x06

///////////////////////////////////////
// MOVE
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
#define MOVE_LEFT 0x00
#define MOVE_RIGHT 0x01
#define MOVE_DOWN 0xFF
#define MOVE_UP 0x03

#define IS_SET 0x01
#define NOT_SET 0x00





#endif // __DEFINES_H
