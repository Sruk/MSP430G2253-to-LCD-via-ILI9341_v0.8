/*
 * lcd_driver.c
 *
 *  This software is distributed under the GPL.
 *  Software has been tested under real circumstances. Use this software on your on risk.
 *  Any source code resemblance to the any property of the third party's is purely coincidental.
 *
 *  Created on: 10. 3. 2014.
 *  Edited on: 6. 4. 2014.
 *      Author: Sruk
 */

#include <msp430.h>
#include "lcd_driver.h"
#include "timer_init.h"

//Basic COMMANDS
#define DSPLY_OFF 	0x28
#define DSPLY_ON 	0x29

//Basic Colors
#define RED 		0xf800
#define GREEN 		0x07e0
#define BLUE 		0x001f
#define BLACK 		0x0000
#define YELLOW 		0xffe0
#define WHITE 		0xffff

//Other Colors
#define CYAN		0x07ff
#define BRIGHT_RED	0xf810
#define GRAY1 		0x8410
#define GRAY2 		0x4208

//Timer Wait Custom Values (1000 ms = 32768 CLK cycles)
#define WAIT_1		33
#define WAIT_5		164
#define WAIT_10		328
#define WAIT_50		1638
#define WAIT_100	3277
#define WAIT_500	16384
#define WAIT_1000	32768

/* LCD hardware reset */

void reset_lcd(){

	P2OUT ^= BIT3;		// set reset pin LOW
	timer_wait(WAIT_5);	// wait 5 ms
	P2OUT ^= BIT3;		// set reset pin HIGH
	timer_wait(WAIT_10);	// wait 10 ms and resume
}

/* Sends 1B of data to LCD */

void write_data_lcd(char data){

		write_SPI(data);

}

/* Sends 2B of data to LCD */

void write_16bdata_lcd(uint16 data16b){

		write_SPI(data16b >> 8);		// send lower byte
		write_SPI(data16b);			// send upper byte

}

/* Sends command (2B) to LCD */

void write_cmd_lcd(char cmd){				


		P2OUT ^= BIT4;				// LCD enable COMMAND (C/D -> LOW)
			write_SPI(cmd);
		P2OUT ^= BIT4;				// LCD disable COMMAND (C/D -> HIGH)

}

/* Sends 8 bit data over SPI, with SEND-DONE control */

void write_SPI(char i){

		P1OUT ^= BIT0;				// P1.6 (RED) TOGGLE -> sending

		P1OUT ^= BIT5;				// CS -> LOW (active)

		UC0IFG &= ~UCA0RXIFG;			// ready to send

		UCA0TXBUF = i;				// send 1 B

		while ((UC0IFG & UCA0RXIFG) == 0) { }	// wait until send is done

		P1OUT ^= BIT5;				// CS -> HIGH (inactive)

		P1OUT ^= BIT0;				// P1.6 (RED) TOGGLE -> done


}

/* Addresses x,y segment of the LCD memory */

void lcd_address_set(uint16 x1,uint16 x2,uint16 y1,uint16 y2)
{
	write_cmd_lcd(0x2a); 				// set X limits
	write_16bdata_lcd(x1);
	write_16bdata_lcd(x2);
     	write_cmd_lcd(0x2b); 				// set Y limits
     	write_16bdata_lcd(y1);
     	write_16bdata_lcd(y2);
     	write_cmd_lcd(0x2c);				// write to memory
}

/* Filled square drawing function */

void lcd_test_color_adv(uint16 x1, uint16 x2, uint16 y1, uint16 y2, uint16 value){

		int i,j;
		lcd_address_set(x1,(x2-1),y1,(y2-1));

		if(x2 > x1 && y2 > y1){

			for (i=(x2-x1) ;i!=0 ;i--){

				for (j=(y2-y1); j!=0 ;j--){

					write_16bdata_lcd(value);

					}
			}
		}
}

/* Simple test routine */

void RGB_test_lcd(void){

	while(1){

		lcd_test_color_adv(0,240,0,320, RED);

		lcd_test_color_adv(0,240,0,320, GREEN);

		lcd_test_color_adv(0,240,0,320, BLUE);

		lcd_test_color_adv(0,240,0,320, WHITE);

		timer_wait(WAIT_1000);



	}

}
