// c libraries
#include <stdio.h>
#include <stdlib.h>

// needs to stand before delay
#include "definitions.h"
#include "lcd.h"
#include "ctrl.h"
#include "menu.h"
#include "game_mechanics.h"

// avr libraries
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h> 
#include <util/delay.h>


int main() {

	lcd_init();
	ctrl_init();
	
	lcd_menu_init();
	
	tetris_init(PLAYER1);
	tetris_init(PLAYER2);
	
	lcd_clear(); 
	
	lcd_menu_ctrl(MENU_START); 
	
	TETRIS_START();
	
}

