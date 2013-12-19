#ifndef __TETRIS_H
#define __TETRIS_H

#include "definitions.h"
#include "ctrl.h"
#include "interface.h"
#include "game_mechanics.h"

#ifndef DEBOUNCE_COUNT
#define DEBOUNCE_COUNT 400
#endif
#ifndef MAX_SPEED
#define MAX_SPEED 5000
#endif
uint8_t TETRIS_CTRL(  );

static volatile uint16_t lol;
static volatile uint16_t tetris_speed [10] = {0, 500, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 4700};
static volatile uint16_t tetris_level [10] = {10, 60, 120, 200, 300, 410, 540, 700, 950, 1000};
static volatile uint8_t difficulty = 0;
static volatile uint16_t score;
static volatile uint16_t debounce_l[MAX_PLAYERS],  debounce_r[MAX_PLAYERS],  debounce_u[MAX_PLAYERS],  debounce_d[MAX_PLAYERS];

uint8_t TETRIS_GET_DIFFICULTY() { 
	return difficulty; 
}
void TETRIS_SET_DIFFICULTY(uint8_t setval) {
	difficulty = setval%10; 
}
uint16_t TETRIS_GET_SCORE() { return score; }

uint8_t TETRIS_START() {
	debounce_l[PLAYER1] = debounce_r[PLAYER1] = debounce_u[PLAYER1] = debounce_d[PLAYER1] = 0;
	lol = 0;
	score = 0;
	tetris_init(PLAYER1);
	lcd_tetris(PLAYER1);
	if(num_players == 2) {
		tetris_init(PLAYER2);
		lcd_tetris(PLAYER2);
		debounce_l[PLAYER2] = debounce_r[PLAYER2] = debounce_u[PLAYER2] = debounce_d[PLAYER2] = 0;
	}
	uint8_t status = TETRIS_CTRL(PLAYER1);
	while( status == CONTINUE) status = TETRIS_CTRL();
	return status;
}

uint8_t TETRIS_RESUME( void ) {
	debounce_l[PLAYER1] = debounce_r[PLAYER1] = debounce_u[PLAYER1] = debounce_d[PLAYER1] = 0;
	lcd_tetris(PLAYER1);
	if(num_players == 2) {
		lcd_tetris(PLAYER2);
		debounce_l[PLAYER2] = debounce_r[PLAYER2] = debounce_u[PLAYER2] = debounce_d[PLAYER2] = 0;
	}
	uint8_t status = TETRIS_CTRL();
	while( status == CONTINUE) status = TETRIS_CTRL();
	return status;
}
// check if row is complete, clear rows, score
uint8_t TETRIS_EVALUATE( uint8_t player ) {
	if( tetris_move(player, MOVE_DOWN) == LOCK ) {
		uint8_t result = tetris_row_remove(player);
		// score. Maximal koennen nur 4 Spalten gleichzeitig abgebaut werden!
		score += result*result;
		// welcome to the next level!!
		if(score > tetris_level[difficulty])
			difficulty ++;
		return tetris_insert(player);
	} else return CONTINUE;
}
uint8_t TETRIS_CTRL( ) {

	uint8_t signal;
	uint8_t result, j, i;	
	
	for(i = 0; i < num_players; i++) {
		if(debounce_l[i] > 0) debounce_l[i]--;
		if(debounce_r[i] > 0) debounce_r[i]--;
		if(debounce_u[i] > 0) debounce_u[i]--;
		if(debounce_d[i] > 0) debounce_d[i]--;
	}
	lol ++;
	
	// simulated timer
	if(lol == (MAX_SPEED - tetris_speed[difficulty])) {
		lol = 0;
		result = TETRIS_EVALUATE(PLAYER1);
		if( result == ERROR ) 
			return GAME_OVER;
		if(num_players > 1) {
			result = TETRIS_EVALUATE(PLAYER2);
			if( result == ERROR ) 
				return GAME_OVER;
		}
	}
	
	// PLAYER1
	signal = PIN_CTRL_1;
	uint8_t ctrl_l1, ctrl_r1;
	if(num_players > 1) {
		ctrl_l1 = CTRL_R1;
		ctrl_r1 = CTRL_L1;	
	} else {
		
	}
	// check menu
	if( !(signal & CTRL_L1) && !(signal & CTRL_R1) && !(signal & CTRL_U1) && !(signal & CTRL_D1) ) {
		return RESUME_GAME;
	}	

	if( !(signal & CTRL_D1) ) {
		result = TETRIS_EVALUATE(PLAYER1);
		if( result == ERROR ) return GAME_OVER;
	} 
	else if( debounce_u[PLAYER1] == 0 && !(signal &  CTRL_U1)  ) {
		tetris_rotate(PLAYER1, 1);
		debounce_u[PLAYER1] = DEBOUNCE_COUNT*2;
	}
	else if( num_players == 1 && debounce_r[PLAYER1] == 0 && !(signal & CTRL_R1) ) {
		tetris_move(PLAYER1, MOVE_LEFT) ;
		debounce_r[PLAYER1] = DEBOUNCE_COUNT;
	} 
	else if( num_players == 2 && debounce_r[PLAYER1] == 0 && !(signal & CTRL_L1) ) {
		tetris_move(PLAYER1, MOVE_LEFT) ;
		debounce_r[PLAYER1] = DEBOUNCE_COUNT;
	} 
	else if( num_players == 1 && debounce_l[PLAYER1] == 0 && !(signal &  CTRL_L1) ) {
		tetris_move(PLAYER1, MOVE_RIGHT) ;
		debounce_l[PLAYER1] = DEBOUNCE_COUNT;
	}
	else if( num_players == 2 && debounce_l[PLAYER1] == 0 && !(signal &  CTRL_R1) ) {
		tetris_move(PLAYER1, MOVE_RIGHT) ;
		debounce_l[PLAYER1] = DEBOUNCE_COUNT;
	}
	
	if(num_players < 2) return CONTINUE;
	// PLAYER2
	signal = PIN_CTRL_2;
	if( !(signal & CTRL_D2) ) {
		result = TETRIS_EVALUATE(PLAYER2);
		if( result == ERROR ) return GAME_OVER;
	} 
	else if( debounce_u[PLAYER2] == 0 && !(signal &  CTRL_U2)  ) {
		tetris_rotate(PLAYER2, 1);
		debounce_u[PLAYER2] = DEBOUNCE_COUNT*2;
	}
	else if( debounce_r[PLAYER2] == 0 && !(signal & CTRL_R2) ) {
		tetris_move(PLAYER2, MOVE_LEFT) ;
		debounce_r[PLAYER2] = DEBOUNCE_COUNT;
	} 
	else if( debounce_l[PLAYER2] == 0 && !(signal &  CTRL_L2) ) {
		tetris_move(PLAYER2, MOVE_RIGHT) ;
		debounce_l[PLAYER2] = DEBOUNCE_COUNT;
	}
	// SIMULATE FALLING EDGE
	
	return CONTINUE;	

}

#endif // __TETRIS_H
