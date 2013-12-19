// ##################################################################################################
// #																								#
// #   Name: lcd.h																				#
// #   Author: Andre Fabricius  (mailto:master.andre@web.de)										#
// #   Bug-fixxes: Roland Halbig (halbig.roland@freenet.de)
// #   Date: 19.12.13 11:17																		    #
// #   Description: Header for GLCD routines for KS0108 or compatibel controllers				    #
// #																								#
// ##################################################################################################
// #																								#
// # Copyright (C) 2008  Andre Fabricius															#
// #																								#
// # This program is free software; you can redistribute it and/or modify it under the terms of 	#
// # the GNU General Public License as published by the Free Software Foundation; either version 3 	#
// # of the License, or (at your option) any later version.											#
// #																								#
// # This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 		#				
// # without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 		#
// # See the GNU General Public License for more details.											#
// # You should have received a copy of the GNU General Public License along with this program; 	#
// # if not, see <http://www.gnu.org/licenses/>.													#
// #																								#
// ##################################################################################################

// ##################################################################################################
// #																								#
// #	  Test-Configuration:	ATMEGA32 @ 16MHz													#
// #							TODO:PowerTip PG12864K (PG12864WRF-KNN-H)								#
// #							TODO:WinAVR-20070525														#
// #							TODO:AVR Studio V4.13 Build 571											#
// #																								#
// ##################################################################################################
	
	#ifndef	MYLCD_H
	#define MYLCD_H
	
	#include "definitions.h"
	
	#include <avr/pgmspace.h>
	#include <avr/io.h>
	#include <util/delay.h>
	
	#ifndef LCD_LATCH
	#define LCD_LATCH \
			LCD_CMD_PORT |= (1<<EN); \
			_delay_us(2); \
			LCD_CMD_PORT &= ~(1<<EN);
	#endif		
	
	//###############################################################################################
	//# 																							#
	//# User Configurations																			#
	//#																								#
	//###############################################################################################
	
	#define LCD_DATA_PIN		PINC		// Data Input Register
	#define LCD_DATA_PORT		PORTC		// Data Output Register
	#define LCD_DATA_DDR		DDRC		// Data Direction Register for Data Port
	
	// Ports
	#define LCD_CMD_PORT		PORTA		// Command Output Register
	#define LCD_CMD_DDR			DDRA		// Data Direction Register for Command Port

	// Command Port Bits
	#define CD					0x07		// Data/Control (RS) PinNumber
	#define RW					0x06		// R/W PinNumber
	#define EN					0x05		// EN  PinNumber
	#define CS1					0x04		// CS1 PinNumber
	#define CS2					0x03		// CS2 PinNumber	
	//#define LCD_NO_RES					// uncomment this line if you have an pullup on reset
	
	#ifndef LCD_NO_RES
		#define RES				0x02		// Reset Pin
	#endif



	//###############################################################################################
	//# 																							#
	//# Extra configuration																			#
	//#																								#
	//###############################################################################################

	#define FASTTEXT 			// comment this if you get bad text output
								// draws Text much faster (writes whole pages without using set_pixel)	
								// only recommended to use if font has an height of 8 (or multipel)
								// using y-positions,0,8,16,24... lcd_set_line(uint8_t line)	

	//#define NO_BMPS			// if you do not need bitmaps
	//#define NO_GRAPHICS		// if you do not need lines/rects/circels
	//#define NO_BRESENHAM		// if you only use horizontal or vertical lines (or rects) (~264 Bytes)
	//#define NO_TEXT			// if you do not need text output

	//###############################################################################################
	//# 																							#
	//# Defines																						#
	//#																								#
	//###############################################################################################

	// Chips
	#define CHIP1				0x01
	#define CHIP2				0x02

	// Commands
	#define LCD_ON				0b00111111
	#define LCD_OFF				0b00111110
	#define LCD_SET_ADD			0b01000000
	#define LCD_SET_PAGE		0b10111000
	#define LCD_DISP_START		0b11000000
	#define LCD_BUSY			0b10000000
	#define LCD_RESET			0b00010000

	// Pixel Colors
	#define BLACK				0x01
	#define WHITE				0x00
	
	// font lines
	#define LINE0				0x00
	#define LINE1				0x08
	#define LINE2				0x10
	#define LINE3				0x18
	#define LINE4				0x20
	#define LINE5				0x28
	#define LINE6				0x30
	#define LINE7				0x38

	//###############################################################################################
	//# 																							#
	//# Prototyes																					#
	//#																								#
	//###############################################################################################

	#define max(a, b) ((a)>(b)?(a):(b)) 
	#define min(a, b) ((a)>(b)?(b):(a))
	
	
	// low level lcd	
	void lcd_chip_select(uint8_t);
	void lcd_write_cmd(uint8_t,uint8_t);
	void lcd_write_data(uint8_t,uint8_t);
	void lcd_write(uint8_t,uint8_t);
	uint8_t lcd_read_data(uint8_t);
	uint8_t lcd_get_data(uint8_t, uint8_t, uint8_t);
	//void wait_while_chip_is_busy();
	
	// lcd
	void lcd_init();
	void lcd_set_all(uint8_t);
	void lcd_set_call(uint8_t, uint8_t);
	void lcd_clear();
	void lcd_fill();
	void lcd_blink( void ); 
	void lcd_set_pixel(uint8_t,uint8_t,uint8_t);
	
	void lcd_set_block(uint8_t range, uint8_t i, uint8_t j, uint8_t colour);
	
	void lcd_invert();
	void lcd_invert_chip(uint8_t chip);
	#ifndef NO_BMP
	void lcd_draw_fullscreen_bmp(uint8_t*);
	#endif

	#endif
