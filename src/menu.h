#ifndef __AZ_MENU_H
#define __AZ_MENU_H

#include "definitions.h"
#include "ctrl.h"
//#include "blocks_game.h"
#include "metagame.h"

////////////////////////////////////////////////////
//////////////MENU
////////////////////////////////////////////////////

static uint8_t cursor;
static uint8_t current_menu;
static uint8_t previous_menu;
static uint8_t sound;

#define ALIGN_DOT 15
#define ALIGN_TXT 22
#define ALIGN_NBR 85


lcd_menu_init() {
	
	num_players = 2;
	cursor = 0;
	current_menu = 0x01;
	previous_menu = 0x01;
	sound = 0;

}


uint8_t wait_for_signal() {
	uint8_t signal = PIN_CTRL_1;
	if( !(signal & CTRL_L1) || !(signal & CTRL_R1) || !(signal & CTRL_U1) || !(signal & CTRL_D1) )
		 return EXIT_GAME;
	else return CONTINUE;	
}

uint8_t wait_and_check() {
	WAIT_S;
	return wait_for_signal();
}

void lcd_menu_entry(entry, line, alignment) {
	lcd_set_cursor(alignment, line);	// new start position
	if(alignment == ALIGN_DOT) {
		lcd_puts(small_font,"o  "); 
		lcd_puts(small_font, entry);
	} else {
		lcd_puts(small_font, entry);
	}
}

lcd_menu_3(const char * str1, const char * str2, const char * str3) {
	lcd_clear();
	lcd_set_cursor(50,LINE2);					// new start position
	lcd_puts(small_font,"TETRIS");
	if(cursor == 0) {
		lcd_menu_entry(str1, LINE5, ALIGN_DOT);
		lcd_menu_entry(str2, LINE6, ALIGN_TXT);
		lcd_menu_entry(str3, LINE7, ALIGN_TXT);
	} else if(cursor == 1) {
		lcd_menu_entry(str1, LINE5, ALIGN_TXT);
		lcd_menu_entry(str2, LINE6, ALIGN_DOT);
		lcd_menu_entry(str3, LINE7, ALIGN_TXT);
	} else {
		lcd_menu_entry(str1, LINE5, ALIGN_TXT);
		lcd_menu_entry(str2, LINE6, ALIGN_TXT);
		lcd_menu_entry(str3, LINE7, ALIGN_DOT);
	} 
}

lcd_menu_2(const char * str1, const char * str2) 
{	
	lcd_menu_3(str1, str2, "");
}

lcd_menu_values(uint8_t v1, uint8_t v2) {
	char str[32];
	itoa(v1, str, 10); 
	lcd_menu_entry(str, LINE5, ALIGN_NBR);
	itoa(v2, str, 10); 
	lcd_menu_entry(str, LINE6, ALIGN_NBR);
}
lcd_menu_value(uint16_t v1) {
	char str[32];
	itoa(v1, str, 10); 
	lcd_menu_entry(str, LINE5, ALIGN_NBR);
}

// MENU_WELCOME -> MENU_START --> INTRO
//							   --> MENU_PLAY  
//								
//
//

lcd_menu(uint8_t menu) {
	previous_menu = current_menu;
	switch(menu) {
		case MENU_WELCOME: 
			current_menu = MENU_WELCOME;
			lcd_display_txt(20, LINE4, " DIES IST DER AZ 1   ");
			WAIT_S;
			lcd_clear();
			lcd_display_txt(50,LINE2,"TETRIS");
			WAIT_S;
			lcd_blink();
			break;
		case MENU_START:
			current_menu = MENU_START;
			lcd_menu_2("SPIEL", "INTRO"); // TODO: , "OPTIONEN"
			break;
		case MENU_OPTIONS: 
			current_menu = MENU_OPTIONS;
			lcd_menu_2("SOUND", "SCHWIERIGKEIT");
			lcd_menu_values(sound, TETRIS_GET_DIFFICULTY() );
			break;
		case MENU_PLAY: 
			current_menu = MENU_PLAY;
			lcd_menu_3("SCHWIERIGKEIT", "SPIELER", "LOS!");
			lcd_menu_values( TETRIS_GET_DIFFICULTY(), num_players );
			break;
		case MENU_RETURN: 
			current_menu = MENU_RETURN;
			lcd_menu_2("FORTFAHREN", "SPIEL VERLASSEN"); // TODO: , "SOUND"
			break;
		case MENU_SOUND: 
			current_menu = MENU_SOUND;
			lcd_menu_2("SOUND", "");
			lcd_menu_value(sound);
		case MENU_GAME_OVER:
			WAIT_S;
			lcd_blink();
			lcd_display_txt( 40, LINE3, "GAME OVER" );
			WAIT_S;
			
			lcd_set_cursor(ALIGN_TXT,LINE5);		
			lcd_puts(small_font, "SCORE");
			lcd_menu_value(TETRIS_GET_SCORE());
	
			// loop until restart
			while(wait_for_signal() == CONTINUE);	
			break;
	}
}

void lcd_bday( void );

/*//////////////////////////////
// navigate the menu
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
uint8_t lcd_menu_ctrl(uint8_t menu) {
	lcd_menu(menu);
	while(1) {
		uint8_t signal;
		// player 1 controls the menu
		signal = PIN_CTRL_1;
		// UP
		if( !(signal & CTRL_U1) && cursor > 0 ) {
			cursor --;
			lcd_menu(menu);
		}
		// DOWN
		else if( !(signal & CTRL_D1) ) {
			if(cursor < 1) 
				cursor++;
			else if( menu == MENU_PLAY && cursor < 2) 
				cursor ++;
			//else if( (menu == MENU_RETURN || menu == MENU_START) && cursor < 2) 
			//	cursor ++;
			lcd_menu(menu);
		}
		// LEFT
		if( !(signal & CTRL_L1) ) {
			if(menu == MENU_SOUND) 
				return CONTINUE;
			else if(menu != MENU_START) {
				cursor = 0;
				return CONTINUE;
			}
		} 
		// RIGHT
		else if( !(signal & CTRL_R1) ) {
			switch(menu) {
				case MENU_START: 
					switch(cursor) {
						case 0: cursor = 1;
								lcd_menu_ctrl(MENU_PLAY); 
								break;
						case 1: lcd_clear();
								lcd_bday();
								break;
					}
					break;
			/*	case MENU_OPTIONS:
					switch(cursor) {
						case 0: sound = (++ sound)%10; 
								break;
						case 1:	TETRIS_SET_DIFFICULTY( TETRIS_GET_DIFFICULTY()+1);
								break;
					}
					break;
				case MENU_SOUND: 
					sound = (++ sound)%10;
					break;
			*/	case MENU_PLAY:
					switch(cursor) { 
						case 0:
							TETRIS_SET_DIFFICULTY( TETRIS_GET_DIFFICULTY()+1);
							break;
						case 1:
							num_players = 1+ (num_players + 2 ) % 2;
							break;
						case 2:
							lcd_clear();
							uint8_t status;
							cursor = 0;
							
							status = TETRIS_START();
							
							while(status == RESUME_GAME) {
								status = lcd_menu_ctrl(MENU_RETURN);
								lcd_clear();
								if(status == CONTINUE) status = TETRIS_RESUME();
								else break;
							}
							if(status == GAME_OVER) {
								lcd_clear();
								lcd_menu(MENU_GAME_OVER);
							}
							lcd_clear();
							cursor = 0;
							lcd_menu(MENU_START);
							return CONTINUE;
							
					}
					break;
				case MENU_RETURN:
					switch(cursor) {
						case 0: return CONTINUE;
						case 1: return EXIT_GAME;
						/*case 2: cursor = 0;
								lcd_menu_ctrl(MENU_SOUND); 
								cursor = 2;*/
					}
			} 
			lcd_menu(menu);
		} 
		// SIMULATE FALLING EDGE
		while(1) {
			if( (signal & CTRL_L1) && (signal & CTRL_R1) && (signal & CTRL_U1) && (signal & CTRL_D1) ) break; 
			signal = PIN_CTRL_1;
		}		
		DEBOUNCE;
	}
	return CONTINUE;
}



void lcd_bday( void ) {
	previous_menu = current_menu;
	current_menu = MENU_WELCOME;

	lcd_display_txt(20,LINE2, "HEJ  ANJA,");
	if(wait_and_check() == EXIT_GAME) return;
	lcd_clear();
	lcd_display_txt(20,LINE2, "DIES  IST  DER");
	lcd_display_txt(20,LINE4, "    AZ 1   ");
	if(wait_and_check() == EXIT_GAME) return;
	lcd_clear();
	lcd_display_txt(10,LINE1, "TETRIS  KANNST  DU  DAMIT");
	lcd_display_txt(20,LINE3, "ZWAR  NOCH  NICHT  SPIELEN");
	lcd_display_txt(30,LINE5, "ABER  DAS  MACHT  NIX ;)");
	if(wait_and_check() == EXIT_GAME) return;
	lcd_clear();
	lcd_display_txt(10,LINE1, "DAFUER  HAST  DU  JA  MICH");
	lcd_display_txt(20,LINE3, "UND  ICH  DICH");
	lcd_display_txt(1,LINE5,  "UND  WIR  UNS  HIER  MIR  DIR  WIR");
	if(wait_and_check() == EXIT_GAME) return;
	lcd_display_txt(120, LINE5, "?");
	if(wait_and_check() == EXIT_GAME) return;
	lcd_display_txt(1,LINE6, "... ABER  DAS  GEHOERT ");
	lcd_display_txt(1,LINE7, "(BEILAEUFIG)  NICHT  HIER  HER!");	
	if(wait_and_check() == EXIT_GAME) return;
	if(wait_and_check() == EXIT_GAME) return;
	lcd_clear();
	lcd_display_txt(20,LINE4, " KUSS! ");
	if(wait_and_check() == EXIT_GAME) return;
	lcd_display_txt(20,LINE6, " ...");
	if(wait_and_check() == EXIT_GAME) return;
	lcd_display_txt(40, LINE6, "PS: ich liebe dich ");
	if(wait_and_check() == EXIT_GAME) return;
	lcd_clear();
	
	lcd_display_txt(10,LINE2, "ALLES  GUTE"); 
	if(wait_and_check() == EXIT_GAME) return;
	
	lcd_display_txt(20,LINE4,"ZUM  GEBURTSTAG!!!");
	if(wait_and_check() == EXIT_GAME) return;
	
	lcd_clear();
	lcd_set_cursor(50,LINE2);
	lcd_puts(small_font,"TETRIS");
	if(wait_and_check() == EXIT_GAME) return;
	lcd_blink();
}



#endif // __AZ_MENU_H
