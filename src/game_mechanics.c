
#include "game_mechanics.h"	

// TODO: Fallunterscheidung Player in Funktionen	

uint8_t tetris_init(uint8_t player) {
	
	uint8_t i, j, k;
	
	uint8_t state = OK;
	state = tetris_malloc(player);
	if(state == ERROR) return state;
	tetris_clear(player);
	tetris_cage_clear(player); 
	
	startx = MAXI/2-1;
	starty = MAXJ-2-1;
	posx[player] = startx;
	posy[player] = starty;
	signifier[player] = TETRIS_T;
	tetris_next_model(player);
	// rotation 0 = no turn, 1 = 1 turn left, 2 = two turns left, 3 = 1 turn right
	rotation[player] = 0;
	tetris_get_model(player, signifier[player], rotation[player]);
	// runtime configuration
	tetris_set_position(player, posx[player], posy[player]);
	
}

void tetris_clear_position(uint8_t player, int k, int l)
{
	uint8_t i, j;
	for(j = 0; j < MLEN; j++) {
		for(i = 0; i < MLEN; i++) {
			if( !(tetris_cage_get(player, i,j) & (1 << i)) ) continue;
			if(l+j >= MAXJ) continue; 
			if( !(k+i<MAXI && l+j<MAXJ) ) continue;
			tetris_unset(player, k+i, l+j);
	}	}
	return;
}

uint8_t tetris_test_position(uint8_t player, int k, int l)
{
	rand_generator += 41;
	uint8_t i, j;
	for(j = 0; j < MLEN; j++) {
		for(i = 0; i < MLEN; i++) {
			if( !(tetris_cage_get(player, i,j) & (1 << i)) ) continue;
			if( k+i >= MAXI || k+i < 0 || l + j < 0 )
				return ERROR;
			if( (tetris_is_set(player,k+i,l+j)  == IS_SET) ) 
				return ERROR;
	} 	}
	return OK;
}

void tetris_set_position(uint8_t player, int k, int l)
{
	uint8_t i, j;
	for(j = 0; j < MLEN; j++) {
		for(i = 0; i < MLEN; i++) {		
			if(l+j >= MAXJ) continue; 
			if( tetris_cage_get(player, i,j) & (1 << i) ) 
				 tetris_set(player, k+i, l+j);
	}	}
	return;
}

uint8_t  tetris_move(uint8_t  player, uint8_t  direction) 
{
	rand_generator += 5;
	int  check_position;
	int x, y;
	x = posx[player]; y = posy[player];
	if(direction == MOVE_RIGHT) x = posx[player] + 1; // # right
	else if(direction == MOVE_LEFT) x = posx[player] -1; // # left
	else if(direction == MOVE_UP) y = posy[player] + 1; // # right
	else if(direction == MOVE_DOWN) y = posy[player] -1; // # left
	else return UNLOCK;
	
	tetris_clear_position(player, posx[player], posy[player]); // clear old position
	check_position = tetris_test_position(player, x, y); // check new one
	if(check_position == ERROR) { // set old position
		tetris_set_position(player, posx[player], posy[player]);
		if(direction == MOVE_DOWN) {
			return LOCK;
		}
	} else { // set new position
		tetris_set_position(player, x, y);
		posx[player] = x; posy[player] = y;
	}
	return UNLOCK;
}

void tetris_rotate(uint8_t player, uint8_t direction)
{
	uint8_t rot;
	rand_generator += 15;
	// # new rotation
	if(direction > 0) // right
		rot = (4 + rotation[player] - 1)%4;
	else if(direction < 0) // left
		rot = (rotation[player] + 1)%4;
		
	tetris_clear_position(player, posx[player], posy[player]); // clear old position
	
	// make temporary copy
	tetris_test_cage_store(); 
	
	tetris_get_model(player, signifier[player], rot); // what would happen if I rotated?
	uint8_t check_position = tetris_test_position(player, posx[player], posy[player]); // check new position
	
	if(check_position == ERROR) { // # restore old state
		tetris_test_cage_restore();
		tetris_set_position(player, posx[player], posy[player]);
		return;
	} else {
		rotation[player] = rot;
		tetris_set_position(player, posx[player], posy[player]);
	}
	
	return;
}

void tetris_next_model(uint8_t player)
{
	rand_generator += 150;
	// TODO: srand(time(NULL));
	// TODO: srand( rand() % 2048);
	srand(1903*rand_generator);
	srand( rand() % (rand_generator + 200) );
	
	signifier_next[player] = (rand()) % 7;
	//signifier_next = (signifier_next+1) % 7;
	
	tetris_test_cage_store(player); // make temporary copy
	tetris_get_model(player, signifier_next[player], 0); // what would happen if I rotated?
	tetris_next_cage_preview(player);
	tetris_test_cage_restore(player);
	
}

uint8_t tetris_insert(uint8_t player) 
{
	rand_generator += 211;
	signifier[player] = signifier_next[player];
	rotation[player] = 0;
	tetris_get_model(player, signifier[player], rotation[player]);
	tetris_next_model(player);
	
	uint8_t check_position = tetris_test_position(player, startx, starty) ;
	if(check_position == ERROR) 
		return ERROR;
	else {
		posx[player] = startx;
		posy[player] = starty;
		tetris_set_position(player, posx[player], posy[player]);
		return OK;
	}
}

// delete all rows that are full
uint8_t tetris_row_remove(uint8_t player) 
{
	uint8_t scored = 0;
	uint8_t ymax = MAXJ-1;
	uint8_t y, i, j;
	uint8_t row_complete = 1;
	
	for(y = 0; y < ymax; y ++) {
		row_complete = 1;
		for(i = 0; i < MAXI; i++) 
			if( tetris_is_set(player,i,y) == NOT_SET ) { 
				row_complete = 0;
				rand_generator += 1;
				break;
			}
		if(row_complete == 0) continue;
		
		rand_generator += 11;
		scored = scored + 1;
		
		// copy down
		for(j = y; j < MAXJ-1; j++) {
			for(i = 0; i < MAXI; i++) {
				 tetris_is_set(player,i,j+1) ? tetris_set(player,i,j) : tetris_unset(player,i,j);
				 tetris_unset(player,i,j+1);
		}	}
		
		// clear top row
		for(i = 0; i < MAXI; i++) tetris_unset(player,i,ymax);
		ymax = ymax - 1;
		// start at the same row again:
		y = y - 1; 
			
	}
	// TODO: copy up
	// TODO: TOP ROW
	/*
	if(num_players != 2) return scored;
	tetris_unset(PLAYER2-player, posx[PLAYER2-player], posy(PLAYER2-player]);
	for(y = 1; num_players == 2 && y < scored+1; y++) {
		for(j = MAXJ-2; j > scored - 1; j--) {
			for(i = 0; i < MAXI; i++) {
				 tetris_is_set(PLAYER2-player,i,j) ? tetris_set(PLAYER2-player,i,j+1) : tetris_unset(PLAYER2-player,i,j+1);
				 tetris_unset(PLAYER2-player,i,j);
		}	}
		for(i = 0; i < MAXI; i++) {
				tetris_set(PLAYER2-player,i,0);
		}	
	}
	tetris_set(PLAYER2-player, posx[PLAYER2-player], posy(PLAYER2-player]);
	*/
	return scored;
}

//###############################################
//################################ Models	
//###############################################
	
void tetris_get_model(uint8_t player, uint8_t signif, uint8_t rot) 
{
	volatile unsigned char * cagepointer;
	cagepointer = tetris_get_cagepointer(player);
	// clear
	uint8_t j;
	for(j = 0; j < MLEN; j++) {
		cagepointer[j] = 0x00;
	}
	switch(signif) {
		case TETRIS_T:
			switch(rot) { 
			case 2:	cagepointer[0] |= 0b00000111;
					cagepointer[1] |= 0b00000010;	
					break;
			case 3:	cagepointer[0] |= 0b00000100;
					cagepointer[1] |= 0b00000110;
					cagepointer[2] |= 0b00000100;
					break;
			case 0:	cagepointer[1] |= 0b00000010;
					cagepointer[2] |= 0b00000111;
					break;
			case 1: cagepointer[0] |= 0b00000001;
					cagepointer[1] |= 0b00000011;
					cagepointer[2] |= 0b00000001;
					break;
			}; break;
		case TETRIS_o:
			cagepointer[0] |= 0b00000011;
			cagepointer[1] |= 0b00000011; 
		break;
		case TETRIS_z:
			if( rot == 0 || rot == 2) {
				cagepointer[0] |= 0b00000110 ;
				cagepointer[1] |= 0b00000011;
			} else if( rot == 1 || rot == 3) {
				cagepointer[0] |= 0b00000001;
				cagepointer[1] |= 0b00000011;
				cagepointer[2] |= 0b00000010;
			}; break;
		case TETRIS_s:
			if( rot == 0 || rot == 2) {
				cagepointer[1] |= 0b00000011;
				cagepointer[2] |= 0b00000110;
			} else if( rot == 1 || rot == 3) {
				cagepointer[1] |= 0b00000100;
				cagepointer[2] |= 0b00000110;
				cagepointer[3] |= 0b00000010;
			}; break;
		case TETRIS_L:
			switch(rot) {
				case 1:	cagepointer[0] |= 0b00000100 ;
						cagepointer[1] |= 0b00000100;  
						cagepointer[2] |= 0b00000110 ;
						break;
				case 2:	cagepointer[1] |= 0b00000001; 
						cagepointer[2] |= 0b00000111 ;
						break;
				case 3:	cagepointer[0] |= 0b00000011 ;
						cagepointer[1] |= 0b00000001;
						cagepointer[2] |= 0b00000001 ;
						break;
				case 0:	cagepointer[0] |= 0b00000111;
						cagepointer[1] |= 0b00000100 ;
						break;
			}; break;
		case TETRIS_J:
			switch(rot) {
				case 2:	cagepointer[0] |= 0b00000110 ;
						cagepointer[1] |= 0b00000100;  
						cagepointer[2] |= 0b00000100 ;
						break;
				case 1:	cagepointer[1] |= 0b00000100; 
						cagepointer[2] |= 0b00000111 ;
						break;
				case 0:	cagepointer[0] |= 0b00000001 ;
						cagepointer[1] |= 0b00000001;
						cagepointer[2] |= 0b00000011 ;
						break;
				case 3:	cagepointer[0] |= 0b00000111;
						cagepointer[1] |= 0b00000001 ;
						break;
			}; break;
		case TETRIS_I:
			if(rot == 1 || rot == 3) {
				cagepointer[2] |= 0b00001111;//
			} else if(rot == 0 || rot == 2) {
				cagepointer[0] |= 0b00000010;// I
				cagepointer[1] |= 0b00000010;// I
				cagepointer[2] |= 0b00000010;// I
				cagepointer[3] |= 0b00000010;// I
			}; break;

	} // switch(signifier)

}

