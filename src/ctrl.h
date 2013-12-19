#ifndef __AZ_CTRL_H
#define __AZ_CTRL_H

// KONTROLLER - PLAYER 1
#define DDR_CTRL_1 DDRB
#define PIN_CTRL_1 PINB
#define PORT_CTRL_1 PORTB 

// KONTROLLER - PLAYER 1
#define DDR_CTRL_2 DDRD
#define PIN_CTRL_2 PIND
#define PORT_CTRL_2 PORTD 

// MAKROS
#define CTRL_L1 (1 << 0)
#define CTRL_R1 (1 << 3)
#define CTRL_U1 (1 << 2)
#define CTRL_D1 (1 << 1)
#define CTRL_L2 (1 << 1) 
#define CTRL_R2 (1 << 3) 
#define CTRL_U2 (1 << 0)
#define CTRL_D2 (1 << 2)  


void ctrl_init() {
		// Buttons for Player IN
		DDR_CTRL_1 &= ~( CTRL_L1 | CTRL_R1 | CTRL_U1 | CTRL_D1 );
		DDR_CTRL_2 &= ~( CTRL_L2 | CTRL_R2 | CTRL_U2 | CTRL_D2 );
		// da an Masse angeschlossen: Internen Pull-Up-Widerstand aktivieren
		PORT_CTRL_1 |=  CTRL_L1 | CTRL_R1 | CTRL_U1 | CTRL_D1; // Player 1
		PORT_CTRL_2 |=  CTRL_L2 | CTRL_R2 | CTRL_U2 | CTRL_D2; // Player 2
}

#endif // __AZ_CTRL_H
