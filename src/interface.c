
#include "interface.h"

uint8_t tetris_malloc(uint8_t player) {
	unsigned int k = 0;
	for(k = 0; k < MAXI; k++) {
		tetris[player][k] = malloc(3*sizeof(char));
		if (tetris[player][k] == NULL) return ERROR;
	}
	return OK;
}

void tetris_set(uint8_t player, uint8_t i, uint8_t j) {
	if(i < MAXI && j < MAXJ) {
		if(j < 8) tetris[player][i][0] |= 1 << j;
		else if( j < 16) tetris[player][i][1] |= 1 << (j-8);
		else if( j < 24) tetris[player][i][2] |= 1 << (j-16); 
		lcd_block(player, i, j);
	}
}
void tetris_unset(uint8_t player, uint8_t i, uint8_t j) {
	if(i < MAXI && j < MAXJ) {
		if(j < 8) tetris[player][i][0] &= ~(1 << j);
		else if( j < 16) tetris[player][i][1] &= ~(1 << (j-8));
		else if( j < 24) tetris[player][i][2] &= ~(1 << (j-16));
	}
	lcd_block(player, i, j);
}
uint8_t tetris_is_set(uint8_t player, uint8_t i, uint8_t j) {
	if(i < MAXI && j < MAXJ) {
		if(j < 8 && (tetris[player][i][0] & 1 << j) ) return IS_SET;
		else if( j < 16 && (tetris[player][i][1] & 1 << (j-8) ) ) return IS_SET;
		else if( j < 24 && (tetris[player][i][2] & 1 << (j-16) ) ) return IS_SET;
	}
	return NOT_SET;
}
		
uint8_t tetris_get(uint8_t player, uint8_t i, uint8_t j) {
	if(i < MAXI && j < MAXJ) {
		if(j < 8) return (tetris[player][i][0] & (1 << j));
		else if( j < 16) return (tetris[player][i][1] & (1 << (j-8)));
		else if( j < 24) return (tetris[player][i][2] & (1 << (j-16)));
	}
	return 0x00;
}

void tetris_clear(uint8_t player) {
	uint8_t i;
	for(i = 0; i < MAXI; i++) {
		tetris[player][i][0] = 0x00;
		tetris[player][i][1] = 0x00;
		tetris[player][i][2] = 0x00;
	}
	// TODO!
	lcd_set_call(0x00, CHIP1) ;
	lcd_set_call(0x00, CHIP2) ;
}

uint8_t tetris_cage_get(uint8_t player, uint8_t i, uint8_t j) {
	if(i < 4 && j < 4) { 
		return (cage[player][j] & (1 << i));
	}
	return 0x00;
}
void tetris_cage_clear( uint8_t player ) {
	uint8_t j;
	for(j = 0; j < MLEN; j++) 
		cage[player][j] &= 0b11110000;	
}

unsigned char * tetris_get_cagepointer(uint8_t player) {
//	if(player == PLAYER1) cagepointer = &cage[0];
//	else if(player == PLAYER2) cagepointer = &cage2[0];
	return (unsigned char *) &cage[player][0];
}
	
// next model's cage
void tetris_next_cage_preview(uint8_t player) {
	uint8_t i,j, colour;
	uint8_t shift = 0;
	if(player == PLAYER2) 
		shift = 118;
	for(i = 0; i < MLEN; i ++)	{	
		for(j = 0; j < MLEN; j++) { // TODO!!!
			if( tetris_cage_get(player, i, j) & (1 << i) )  colour = BLACK;
			else colour = WHITE;
			//lcd_set_block(3,i,j+1, colour);
			lcd_set_pixel(i*2+2 + shift,j*2+4, colour);
			lcd_set_pixel(i*2+2+1 + shift,j*2+4, colour);
			lcd_set_pixel(i*2+2 + shift,j*2+4+1, colour);
			lcd_set_pixel(i*2+2+1 + shift,j*2+4+1, colour);
	}	}	
}

// test cage 
void tetris_test_cage_store( uint8_t player ) {
	uint8_t j;
	for(j = 0; j < MLEN; j++) { cage_test[player][j] = cage[player][j]; }
}

void tetris_test_cage_restore( uint8_t player ) {
	uint8_t j;
	for(j = 0; j < MLEN; j++) {
		cage_test[player][j] &= 0b00001111; 
		cage[player][j] &= 0b11110000;
		cage[player][j] |= cage_test[player][j];
	}
}


// draw a block of pixels
void lcd_block(uint8_t player, uint8_t i, uint8_t j) {
	if(num_players == 1) {
		if(tetris_get(player, i,j) > 0) 
			 lcd_set_block(FAKTOR, i, MAXJ-1-j, BLACK);
		else lcd_set_block(FAKTOR, i, MAXJ-1-j, WHITE);
	} else if(num_players == 2) {
		switch(player) {
			case PLAYER1:
					if(tetris_get(player, i,j) > 0) 
						 lcd_set_block(FAKTOR/2, MAXJ-1-j, PLAYER1_X/FAKTOR + i, BLACK);
					else lcd_set_block(FAKTOR/2, MAXJ-1-j, PLAYER1_X/FAKTOR + i,  WHITE);
					break;
			case PLAYER2:
					if(tetris_get(PLAYER2, i,j) > 0) 
						 lcd_set_block(FAKTOR/2, MAXJ-1-j, PLAYER2_X/FAKTOR+i, BLACK);
					else lcd_set_block(FAKTOR/2, MAXJ-1-j, PLAYER2_X/FAKTOR+i, WHITE);
					break;
		}
	}
	return;
}

void lcd_frame() {
	uint8_t i,j;
	if(num_players == 1) {
		for(i = 0; i < 132; i ++)	{	
			lcd_set_pixel(i,0, BLACK); 
			lcd_set_pixel(i,1, BLACK); 
			lcd_set_pixel(i,63, BLACK); 
			lcd_set_pixel(i,62, BLACK); 
		} 
	} else {
		for(i = 0; i < 132; i ++)	{	
			lcd_set_pixel(i,0, BLACK); 
			lcd_set_pixel(i,1, BLACK); 
			lcd_set_pixel(i,63, BLACK); 
			lcd_set_pixel(i,62, BLACK); 
		} 
		for(i = 0; i < 64; i ++)	{	
			lcd_set_block(FAKTOR/2, i, (PLAYER1_X-FAKTOR)/FAKTOR + 0, BLACK);
			lcd_set_block(FAKTOR/2, i, (PLAYER1_X)/FAKTOR + MAXI, BLACK);
			lcd_set_block(FAKTOR/2, i, (PLAYER2_X-FAKTOR)/FAKTOR + 0, BLACK);
			lcd_set_block(FAKTOR/2, i, (PLAYER2_X)/FAKTOR + MAXI, BLACK);
		} 	
	}
}

// print out tetris array
void lcd_tetris(uint8_t player) {
	if((num_players == 2 && player == PLAYER2) || (num_players == 1 && player == PLAYER1) ) lcd_frame();
	uint8_t i,j;
	for(i = 0; i < MAXI; i ++)	{	
		for(j = 0; j < MAXJ; j++) {
			lcd_block(player,i,j);
	}	}	
}
