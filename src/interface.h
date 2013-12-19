#ifndef __INTERFACE2_H
#define __INTERFACE2_H

#include "definitions.h"
#include "lcd.h"

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
//#include <malloc.h>

volatile unsigned char * tetris[MAX_PLAYERS][MAXI];
volatile unsigned char cage[MAX_PLAYERS][MLEN]; // first 4 bits: cage_next, last 4 bits: cage
volatile unsigned char cage_test[MAX_PLAYERS][MLEN];

uint8_t tetris_malloc(uint8_t player);

// tetris - this is the main array manipulation
// works block wise

void tetris_set(uint8_t player, uint8_t i, uint8_t j);
uint8_t tetris_is_set(uint8_t player, uint8_t i, uint8_t j);
void tetris_unset(uint8_t player, uint8_t i, uint8_t j);
uint8_t tetris_get(uint8_t player, uint8_t i, uint8_t j);
void tetris_clear(uint8_t player);

void tetris_cage_set(uint8_t player, uint8_t i, uint8_t j);
void tetris_cage_unset(uint8_t player, uint8_t i, uint8_t j);
uint8_t tetris_cage_get(uint8_t player, uint8_t i, uint8_t j);
unsigned char * tetris_get_cagepointer(uint8_t player);
	

void tetris_next_cage_set(uint8_t player, uint8_t i, uint8_t j);
void tetris_next_cage_unset(uint8_t player, uint8_t i, uint8_t j);
uint8_t tetris_next_cage_get(uint8_t player, uint8_t i, uint8_t j);

void lcd_block(uint8_t player, uint8_t i, uint8_t j);
void lcd_tetris(uint8_t player);

#endif // __INTERFACE2_H
