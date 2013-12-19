#ifndef __TETRIS_MODULES_H
#define __TETRIS_MODULES_H

#include "definitions.h"
#include "interface.h"

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */


uint8_t startx, starty;
int posx [MAX_PLAYERS];
int posy [MAX_PLAYERS]; // IMPORTANT! NEEDS TO BE INT!
uint8_t rotation[MAX_PLAYERS];
uint8_t signifier[MAX_PLAYERS];
uint8_t signifier_next[MAX_PLAYERS];

static volatile uint32_t rand_generator = 0;

uint8_t  tetris_init(uint8_t  player);

uint8_t  tetris_insert(uint8_t player);
uint8_t  tetris_move(uint8_t player, uint8_t  direction);
void tetris_rotate(uint8_t player, uint8_t  direction);
int  tetris_move_down(uint8_t player);

void tetris_get_model(uint8_t player, uint8_t signif, uint8_t rot);

uint8_t  tetris_test_position(uint8_t player, int k, int l);
void tetris_clear_position(uint8_t player, int  k, int  l);
void tetris_set_position(uint8_t player, int  k, int  l);
void tetris_next_model(uint8_t player);

uint8_t  tetris_row_remove(uint8_t player);
void tetris_game_over();

//void tetris_move_or_exit(uint8_t player, uint8_t  key);

#endif // __TETRIS_MODULES_H
