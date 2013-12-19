

#include "lcd.h"

volatile uint8_t cursor_x,cursor_y,inverted=0; // cursor position


// ----------------------
// Controller selection
// ----------------------
void lcd_chip_select(uint8_t chip)
{
	LCD_CMD_PORT &=	~((1<<CS1)|(1<<CS2));
	if(chip & CHIP1) {	LCD_CMD_PORT |= (1<<CS1); }
	if(chip & CHIP2) {	LCD_CMD_PORT |= (1<<CS2); }
}
void lcd_write_cmd(uint8_t cmd, uint8_t chip)
{	
	lcd_chip_select(chip);
	LCD_CMD_PORT &= ~((1<<CD)|(1<<RW));		// command mode
	lcd_write(cmd,chip);
}
void lcd_write_data(uint8_t data, uint8_t chip)
{
	if(inverted) data = ~data;
	lcd_chip_select(chip);
	LCD_CMD_PORT |= (1<<CD);	// Set Data mode, and write data
	LCD_CMD_PORT &= ~(1<<RW);	// we're in write mode
	lcd_write(data,chip);
}
void lcd_write(uint8_t val, uint8_t chip)
{
	LCD_DATA_DDR = 0xff; // set all as output
	LCD_DATA_PORT = val; // set PORT

	lcd_chip_select(chip);	
	LCD_LATCH;

	_delay_us(2);		// wait
	LCD_DATA_PORT=0x00;	// set all off	
}

void wait_while_chip_resets()
{
	LCD_DATA_DDR = 0x00;	// dataport is input
	LCD_DATA_PORT= 0xFF;

	// status read mode
	LCD_CMD_PORT &= ~(1<<CD);
	LCD_CMD_PORT |=(1<<RW);
	
	// poll busy flag
	LCD_LATCH;
	while( LCD_DATA_PIN & LCD_RESET ) LCD_LATCH;
	
	LCD_CMD_PORT &= ~((1<<EN)|(1<<RW)); // stop reading
}

// ----------------------
// Init LCD Controllers
// ----------------------
void lcd_init()
{
	LCD_CMD_DDR |= (1<<CD) | (1<<RW) | (1<<EN) | (1<<CS1) | (1<<CS2); 
	#ifndef LCD_NO_RES
		LCD_CMD_DDR |= (1<<RES);
	#endif

	lcd_chip_select(CHIP1|CHIP2);

	#ifndef LCD_NO_RES
	LCD_CMD_PORT &= ~(1<<RES);	// pull reset low for a moment
	_delay_ms(30);
	LCD_CMD_PORT |= (1<<RES);	// let reset rise	
	wait_while_chip_resets();
	#endif
	
	lcd_write_cmd(LCD_ON,CHIP1|CHIP2);
	lcd_write_cmd(LCD_DISP_START|0,CHIP1|CHIP2);
	lcd_write_cmd(LCD_SET_ADD|0,CHIP1|CHIP2);
	lcd_write_cmd(LCD_SET_PAGE|0,CHIP1|CHIP2);
	
}

void lcd_set_all(uint8_t all) {
	uint8_t l,k;
	for(l = 0; l < 8; l ++) {
		lcd_write_cmd(LCD_SET_PAGE| l, CHIP1|CHIP2);	
		lcd_write_cmd(LCD_SET_ADD | 0, CHIP1|CHIP2);	
		for(k = 0; k < 64; k ++) {
			lcd_write_data(all,CHIP1|CHIP2);	
	} 	}
	lcd_write_cmd(LCD_SET_PAGE| 0, CHIP1|CHIP2);	
	lcd_write_cmd(LCD_SET_ADD | 0, CHIP1|CHIP2);	
}
void lcd_set_call(uint8_t all, uint8_t chip) {
	uint8_t l,k;
	for(l = 0; l < 8; l ++) {
		lcd_write_cmd(LCD_SET_PAGE| l, chip);	
		lcd_write_cmd(LCD_SET_ADD | 0, chip);	
		for(k = 0; k < 64; k ++) {
			lcd_write_data(all,chip);	
	} 	}
	lcd_write_cmd(LCD_SET_PAGE| 0, chip);	
	lcd_write_cmd(LCD_SET_ADD | 0, chip);	
}
//----------------------------
// Clear the Display
//----------------------------
void lcd_clear()
{
	lcd_set_all(0x00);
}
//----------------------------
// Fill the Display
//----------------------------
void lcd_fill()
{
	lcd_set_all(0xFF);
}

void lcd_blink() {
		lcd_write_cmd(LCD_OFF,CHIP1|CHIP2);
		_delay_ms(5);
		lcd_write_cmd(LCD_ON,CHIP1|CHIP2);
		_delay_ms(5);
}

// --------------------
// Read Data
// --------------------
uint8_t lcd_read_data(uint8_t chip)
{ 
	uint8_t temp_data;

	lcd_chip_select(chip);
	
	LCD_DATA_DDR = 0x00;	// now data port is input
	LCD_DATA_PORT = 0xFF;   // pullup resistors
	// read mode
	LCD_CMD_PORT |= (1<<RW) | (1<<CD);
	
	LCD_CMD_PORT |= (1<<EN);	// request data
	_delay_us(2);				// wait a moment for stable data
	temp_data = LCD_DATA_PIN;	// read data
	LCD_CMD_PORT &= ~(1<<EN);	// end of request
	if(inverted) return ~temp_data;
		else	 return temp_data;
}
uint8_t lcd_get_data(uint8_t page, uint8_t addr, uint8_t chip) {
	// 1 dummy read
	lcd_write_cmd(LCD_SET_PAGE | page, chip); 
	lcd_write_cmd(LCD_SET_ADD  | addr, chip);  
	lcd_read_data(chip);	
	// real read
	lcd_write_cmd(LCD_SET_PAGE | page, chip); 
	lcd_write_cmd(LCD_SET_ADD  | addr, chip);  	
	return lcd_read_data(chip);	
}
//----------------------------
// Set Pixel BLACK or WHITE
//----------------------------
void lcd_set_pixel(uint8_t x, uint8_t y, uint8_t color)
{
	// check position
	if ((x >= 128) || (y >= 64)) return;	
	
	uint8_t chip, data, page, address;
	page = y / 8;
	address  = y % 8;
	// select controller chip
	chip = CHIP1;
	if (x > 63) { chip = CHIP2; x -= 64;  }
			
	// 1 dummy read
	lcd_write_cmd(LCD_SET_PAGE | page, chip); 
	lcd_write_cmd(LCD_SET_ADD  | x, chip);  
	data = lcd_read_data(chip);	
	// real read
	lcd_write_cmd(LCD_SET_PAGE | page, chip); 
	lcd_write_cmd(LCD_SET_ADD  | x, chip);  	
	data = lcd_read_data(chip);	
	
	if (color==BLACK)		
		 data |= (1 << address);	
	else data &= ~(1 << address);	
	
	// set right column
	lcd_write_cmd(LCD_SET_ADD  | x, chip);  
	
	lcd_write_data(data, chip);			
}

void lcd_set_block(uint8_t range, uint8_t i, uint8_t j, uint8_t colour) {
	
	uint8_t x,y,k,l;	
	uint8_t chip, data, page, address;
	uint8_t  leftovers;

	x = range * j + range+2; 
	y = range * i + 2;
	
	// check position
	if ((x >= 128) || (y >= 64)) return;	
	
	page = y / 8;
	address  = y % 8;
	
	chip = CHIP1;// select controller chip
	if (x > 63) { chip = CHIP2; x -= 64;  }
			
	data = lcd_get_data(page, x, chip);	
	
	// prepare data
	if (colour==BLACK)		
		for(k = 0; k < 8 && k < range; k++)
			data |= (1 << address+k);	
	else 
		for(k = 0; k < 8 && k < range; k++)
			data &= ~(1 << address+k);	
	
	// write to chip
	lcd_write_cmd(LCD_SET_ADD  | x, chip);  // set right column
	for(l = 0; l < range && x + l < 64; l++) 	
		lcd_write_data(data, chip);	
	if(chip == CHIP1 && x+range >= 64) { // crosses chip boarder 
		leftovers = x+range-64;
		lcd_write_cmd(LCD_SET_PAGE | page, CHIP2); 
		lcd_write_cmd(LCD_SET_ADD  | 0, CHIP2);  	
		for(l = 0; l < leftovers; l++) 	// data is the same!!
			lcd_write_data(data, CHIP2);
	}
	
	// if all information fit into page space
	if(address + range < 8) 
		return;
	
	// else
	leftovers = address + range - 8;
	page = page + 1;
	
	data = lcd_get_data(page, x, chip);	
	
	// prepare data
	if (colour==BLACK)		
		for(k = 0; k < 8 && k < leftovers; k++)
			data |= (1 << k);	
	else 
		for(k = 0; k < 8 && k < leftovers; k++)
			data &= ~(1 << k);	
	
	// write to chip
	lcd_write_cmd(LCD_SET_ADD  | x, chip);  
	for(l = 0; l < range && x + l < 64; l++) 	
		lcd_write_data(data, chip);	
	if(chip == CHIP1 && x+range >= 64) { // crosses chip boarder 
		leftovers = x + range - 64;
		lcd_write_cmd(LCD_SET_PAGE | page, CHIP2); 
		lcd_write_cmd(LCD_SET_ADD  | 0, CHIP2);  	
		for(l = 0; l < leftovers; l++) 	// data is the same!!
			lcd_write_data(data, CHIP2);
	}
	return;
}



// ---------------------------------------------
// inverts the screen and sets mode to inverted
// ---------------------------------------------

void lcd_invert()
{

	lcd_invert_chip( CHIP1 );
	lcd_invert_chip( CHIP2 );
	inverted++;
	inverted %=2;
}

void lcd_invert_chip(uint8_t chip)
{
	uint8_t x,y,data;
	lcd_chip_select(chip); 
	for (y = 0; y < 8; y++) {
		LCD_CMD_PORT &= ~((1<<CD)|(1<<RW));		// command mode
		lcd_write(LCD_SET_PAGE | y, chip);	
		for (x = 0; x <= 63; x++) {
			// dummy read and read CHIP1
			lcd_write(LCD_SET_ADD | x,chip);	
			data=lcd_read_data(chip);
			LCD_CMD_PORT &= ~((1<<CD)|(1<<RW));		// command mode
			lcd_write(LCD_SET_ADD|x,chip);	
			data=lcd_read_data(chip);

			LCD_CMD_PORT &= ~((1<<CD)|(1<<RW));		// command mode
			lcd_write(LCD_SET_ADD|x,chip);	

			LCD_CMD_PORT |= (1<<CD);	// Set Data mode, and write data
			lcd_write(~data,chip);		//set inverter data
		}
	}

	lcd_write_cmd(LCD_SET_ADD|0,chip);		//set hom home pos
	lcd_write_cmd(LCD_SET_PAGE|0,chip);

}



////////////////////////////////////////////////7
// TODO: From here nor tested!!!!!!!!!!!!!!!!!!!
////////////////////////////////////////////////





// ------------------------------------
// Draw an fullscreen Bitap 
// ------------------------------------
#ifndef NO_BMP
void lcd_draw_fullscreen_bmp(uint8_t* bitmap)
{  
	// INFO:
	// data is organized page after page (0-to-127)
	// (LSB-to-MSB TOP-to-Bottom)
	// from 0 to 127 in eight lines (127*8=1024 Byte)

	uint16_t byte=0;
	uint8_t x,y;
	for (y=0;y<8;y++)
	{
		lcd_write_cmd(LCD_SET_PAGE|y,CHIP1|CHIP2);	//	start a new page
		lcd_write_cmd(LCD_SET_ADD|0,CHIP1|CHIP2);	//  start at 0
		for (x=0;x<128;x++)
		{
			if (x<64)			
				lcd_write_data(pgm_read_byte(bitmap+(byte++)),CHIP1);			
			else			
				lcd_write_data(pgm_read_byte(bitmap+(byte++)),CHIP2);			
		}	
	}
	

	/* 
		this was used to test lcd_set_pixel	
		(can be used to overlay a bmp to current screen)
	*/

	/*	  	
	uint16_t byte=0;
	uint8_t x,y,bit,temp;
	for (y=0;y<8;y++)
	{
		 for(x=0;x<128;x++)
		 {
		 	temp= pgm_read_byte(bitmap+(byte++));
		 	for (bit=0;bit<8;bit++)			
				if ((temp& (1<<bit))!=0)
					lcd_set_pixel(x,y*8+bit,BLACK);			
		 }
	}
	*/
	
}
#endif //no-bmp



// ###############################################################################################
// # 																							 #
// # Text output																				 #
// #																							 #
// ###############################################################################################

#ifndef NO_TEXT
// ---------------------------------
// get the count of chars in font
// ---------------------------------
uint8_t font_number_of_chars(uint8_t* font)	
{	
	return(pgm_read_byte(font+5)); // return number of characters stored in this font	
}



// ---------------------------------
// get first char in font
// ---------------------------------
uint8_t font_first_char(uint8_t*font)	
{	
	return(pgm_read_byte(font+4)); // return the first used char of the font	
}



// ---------------------------------
// get width of given char
// ---------------------------------
uint8_t font_char_width(uint8_t* font,uint8_t chr)
{
	return(pgm_read_byte(font+6+(chr-font_first_char(font))));	// return the width of the given character
}


// ----------------------------------------
// check if given char is present in font
// ----------------------------------------
uint8_t font_char_present(uint8_t* font,uint8_t chr)
{
	if ( (chr>= font_first_char(font)) && (chr<=font_first_char(font) + font_number_of_chars(font)-1)) // check if char is present
		return(1);
	else
		return(0);
}

// ---------------------------------
// get height of font
// ---------------------------------
uint8_t font_char_height(uint8_t*font)
{
	return(pgm_read_byte(font+3)); // return the first used char of the font
}

// ---------------------------------
// get the start offset of char
// ---------------------------------
uint16_t font_start_offset(uint8_t*font,uint8_t chr) 
{
	uint16_t offset=(uint16_t)0x0000;
	uint8_t cnt;
	uint8_t height= (font_char_height(font)-1)/8;

	for (cnt= font_first_char(font);cnt<chr;cnt++)
	{
		offset+=font_char_width(font,cnt) * (height+1); // width * (number of pages) needed for each char
	}	
	
	offset+=font_number_of_chars(font)+6;				// add 6 for the header-bytes and the number of bytes need for the width-array

	return offset;
}

// ---------------------------------
// set cursor to x,y
// ---------------------------------
void lcd_set_cursor(uint8_t x, uint8_t y)
{
	if(x<128)	cursor_x=x;
	if(y<64) 	cursor_y=y;
}

// ---------------------------------
// put char to screen
// ---------------------------------
void lcd_putc(uint8_t* font,uint8_t chr)
{
	uint8_t x,page,bit,data,cnt=0;
	
	if (font_char_present(font,chr)==1)	// print only if letter is present in font
	{
		if ( ( (cursor_x + font_char_width(font,chr)) <128)  && ((cursor_y+font_char_height(font))<=64))	// only print leeter if it fits in screen
		{
			uint16_t offset= font_start_offset(font,chr);	// get the position of the first byte in font-array
			
			for(page=0;page<=(font_char_height(font)-1)/8;page++)	// write all pages(rows)
			{
				lcd_write_cmd(LCD_SET_PAGE|((cursor_y/8)+page),CHIP1|CHIP2);	// calc the current page

				if (cursor_x<64)	// set cursor to its poition (0 on the inactive controller so it can start directly on chip change)
				{
					lcd_write_cmd(LCD_SET_ADD|cursor_x,CHIP1);
					lcd_write_cmd(LCD_SET_ADD|0,CHIP2);
				}
				else
				{
					lcd_write_cmd(LCD_SET_ADD|(cursor_x-64),CHIP2);				
				}

				for (x=cursor_x;x<cursor_x + font_char_width(font,chr);x++)	//fill "pages"
				{
					data= pgm_read_byte(font+offset+cnt++);

					if ( page==font_char_height(font)/8)		
						data>>=8-(font_char_height(font)%8);	// if char height is bigger than 8 we have to remove some leading zeros
					
					#ifdef FASTTEXT								// this is much much faster than using set_pixel (see discription in header file)
					if (cursor_y%8==0 )
					{
						if (x<64)								
							lcd_write_data(data,CHIP1);						
						else									
							lcd_write_data(data,CHIP2);					
					}
					else
					#endif
					{
						for (bit=0;bit<8;bit++)
						{
							if ((data&(1<<bit))!=0)
							 lcd_set_pixel(x,cursor_y+page*8+bit,BLACK);
						}
					}
				}			
				
				#ifdef FASTTEXT
				if(cursor_y%8==0)
				{
					if (x<64)								
						lcd_write_data(0,CHIP1);						
					else									
						lcd_write_data(0,CHIP2);				
				}
				#endif
			}			
			cursor_x += font_char_width(font,chr)+1;


		}
	}
}



// ---------------------------------
// put string to screen (from ram)
// ---------------------------------
void lcd_puts(uint8_t* font,char* string)
{
	while(*string) {
		lcd_putc(font,*string++);
	}
}


void lcd_puts_slow(uint8_t* font,char* string, uint8_t bla)
{
	while(*string) {
		lcd_putc(font,*string++);
		_delay_ms(5);
	}
}



// ---------------------------------------
// put string to screen (from flash)
// use: lcd_puts_p(PSTR("HELLO WORLD"));
// ---------------------------------------
void lcd_puts_p(uint8_t* font,char* PROGMEM string)
{while(pgm_read_byte(string))lcd_putc(font,pgm_read_byte(string++));}

#endif //no-text


void lcd_display_txt( uint8_t i, uint8_t line, char* str ) {
	lcd_set_cursor(i,line);					// new start position
	lcd_puts_slow(small_font, str, 250);
}


// ###############################################################################################
// # 																							 #
// # Graphics																					 #
// #																							 #
// ###############################################################################################
#ifndef NO_GRAPHICS


// ---------------------------------
// draw line on screen
// ---------------------------------
void lcd_draw_line(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t color)
{
	uint8_t cnt;

	#ifndef NO_BRESENHAM
	uint8_t dy,dx,x,y;
	int8_t	addx, addy;
	int16_t err;
	#endif

	if (x1==x2 || y1==y2)
	{
		if (x1==x2)
		{
			for (cnt=min(y1,y2);cnt<=max(y1,y2);cnt++)			
				lcd_set_pixel(x1,cnt,color);			
		}
		else
		{
			for (cnt=min(x1,x2);cnt<=max(x1,x2);cnt++)			
				lcd_set_pixel(cnt,y1,color);			
		}
	}
	#ifndef NO_BRESENHAM	// see header
	else
	{
   		dx = max(x1,x2)-min(x1,x2);
   		dy = max(y1,y2)-min(y1,y2);

   		x = x1;
   		y = y1;

	   	if(x1 > x2)
	      addx = -1;
	   	else
	      addx = 1;

	   	if(y1 > y2)
	      addy = -1;
	   	else
	      addy = 1;

	   	if(dx >= dy)
	   	{
	      err = 2*dy - dx;

	      for(cnt=0; cnt<=dx; ++cnt)
	      {
	         lcd_set_pixel(x, y, color);

	         if(err < 0)
	         {
	            err += 2*dy;
	            x += addx;
	         }
	         else
	         {
	            err += 2*dy - 2*dx;
	            x += addx;
	            y += addy;
	         }
	      }
	   	}
	   	else
	   	{
	      err = 2*dx - dy;

	      for(cnt=0; cnt<=dy; ++cnt)
	      {
	         lcd_set_pixel(x, y, color);

	         if(err < 0)
	         {
	            err += 2*dx;
	            y += addy;
	         }
	         else
	         {
	            err += 2*dx - 2*dy;
	            x += addx;
	            y += addy;
	         }
	      }
	  	}	
	}
	#endif //no_bresenham
}


// ---------------------------------
// draw rectangle on screen
// ---------------------------------
void lcd_draw_rect(uint8_t x,uint8_t y,uint8_t width ,uint8_t height,uint8_t color)
{
	lcd_draw_line(x,y,x+width-1,y,color);					// Top Line
	lcd_draw_line(x+width-1,y,x+width-1,y+height-1,color); 	// Left Line
	lcd_draw_line(x,y+height-1,x+width-1,y+height-1,color);	// Bottom Line
	lcd_draw_line(x,y,x,y+height-1,color);					// Right Line
}


// ------------------------------------
// fills an rectangle with given color
// ------------------------------------
void lcd_fill_rect(uint8_t x,uint8_t y,uint8_t width,uint8_t height,uint8_t color)
{
	uint8_t cnt_x,cnt_y;
		for (cnt_y=y;cnt_y<y+height;cnt_y++)				// fill screen using set_pixel (slow) but needs less memory
			for (cnt_x=x;cnt_x<x+width;cnt_x++)
				lcd_set_pixel(cnt_x,cnt_y,color);

}

#endif //no-graphics
