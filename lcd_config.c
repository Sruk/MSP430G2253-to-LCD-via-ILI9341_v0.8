/*
 * lcd_config.c
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
#include "lcd_config.h"

#define INVERT 		0					// color negative: enable 1, disable 0

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

void init_ILI9340C(void)
{
	 	 	write_cmd_lcd(0x11);    	//Enter Sleep

	 	 	timer_wait(WAIT_100);

			write_cmd_lcd(0xCB);		// Power Control A
	        write_data_lcd(0x39);
	        write_data_lcd(0x2C);
	        write_data_lcd(0x00);
	        write_data_lcd(0x34);		// VCORE: min 0x30, max 0x35
	        write_data_lcd(0x02);		// VBC voltage: min 0x07, max 0x00

	        write_cmd_lcd(0xCF);		// Power Control B
	        write_data_lcd(0x00);
	        write_data_lcd(0X81);		// power-saving -> 0xC1
	        write_data_lcd(0X30);

	        write_cmd_lcd(0xE8);		// Driver Timing Ctrl A
	        write_data_lcd(0x85);		// overlaping TFT gate time: normal 0x84, extended 0x85
	        write_data_lcd(0x00);		// EQ time of gate driving: normal 0x00, extended 0x11
	        write_data_lcd(0x78);		// precharge time: normal 0x7A, long 0x79, longest 0x78

	        write_cmd_lcd(0xEA);		// Driver Timing Ctrl B
	        write_data_lcd(0x00);
	        write_data_lcd(0x00);

	        write_cmd_lcd(0xED);		// Power On Sequence CTRL
	        write_data_lcd(0x55);		// soft start time in 2B 0x(I)(II) by values: longest 4, long 5, short 6, off 7
	        write_data_lcd(0x03);		// frame enable in 2B 0x(I)(II) by values: 0, 1, 2, 3
	        write_data_lcd(0X12);		// frame enable in 2B 0x(I)(II) by values: 0, 1, 2, 3
	        write_data_lcd(0X81);		// DVDDH enhance: disable 0x01, enable 0x81

	        write_cmd_lcd(0xF7);		// Pump Ratio CTRL
	        write_data_lcd(0x20);		// DVDDH voltage multiplier: times two 0x20, times three 3 0x30

	        write_cmd_lcd(0xC0);		//Power control 1
	        write_data_lcd(0x23);   	//VRH[5:0] -> greyscale value: max 0x3F, min 0x03

	        write_cmd_lcd(0xC1);    	//Power control 2 (step-up voltage generator)
	        write_data_lcd(0x10);   	//SAP[2:0];BT[3:0]

	        write_cmd_lcd(0xC5);    	//VCM control
	        write_data_lcd(0x3e);   	// --
	        write_data_lcd(0x28);

	        write_cmd_lcd(0xC7);    	//VCM control2
	        write_data_lcd(0x86);  		// --

	        write_cmd_lcd(0x36);    	// Memory Access Control (refresh order)
	        write_data_lcd(0xD8); 		//

	        write_cmd_lcd(0x3A);		// Pixel Format
	        write_data_lcd(0x55);		// 16bit 0x55, 18bit 0x66

	        write_cmd_lcd(0xB1);		// Frame Rate CTRL
	        write_data_lcd(0x00);		//
	        write_data_lcd(0x1B);		// 199Hz max 0x10, 70Hz 0x1B, min 61Hz 0x1F

	        color_invert();				// Color Inversion CTRL

	        write_cmd_lcd(0xB6);    	// Display Function Control (ILI3941 datasheet pg164)
	        write_data_lcd(0x0A);		//
	        write_data_lcd(0x82);
	        write_data_lcd(0x27);

	        set_gamma();

	        write_cmd_lcd(0x11);    	// Exit Sleep
	        timer_wait(WAIT_100);

	        write_cmd_lcd(DSPLY_ON);    // Display on
	        write_cmd_lcd(0x13);		// Normal mode

}

void color_invert(void){

	if(INVERT)
		write_cmd_lcd(0x21);
	else
		write_cmd_lcd(0x20);
}


void set_gamma(void){

    write_cmd_lcd(0x26);    			//Gamma curve selected
    write_data_lcd(0x01);				// ostale opcije: 0x02, 0x04, 0x08

    write_cmd_lcd(0xF2);    			// 3Gamma Function Disable
    write_data_lcd(0x02);   			// disable: 0x02, enable: 0x03

    write_cmd_lcd(0xE0);    			//Set Positive Gamma
    write_data_lcd(0x0F);
    write_data_lcd(0x31);
    write_data_lcd(0x2B);
    write_data_lcd(0x0C);
    write_data_lcd(0x0E);
    write_data_lcd(0x08);
    write_data_lcd(0x4E);
    write_data_lcd(0xF1);
    write_data_lcd(0x37);
    write_data_lcd(0x07);
    write_data_lcd(0x10);
    write_data_lcd(0x03);
    write_data_lcd(0x0E);
    write_data_lcd(0x09);
    write_data_lcd(0x00);

    write_cmd_lcd(0XE1);    			//Set Negative Gamma
    write_data_lcd(0x00);
    write_data_lcd(0x0E);
    write_data_lcd(0x14);
    write_data_lcd(0x03);
    write_data_lcd(0x11);
    write_data_lcd(0x07);
    write_data_lcd(0x31);
    write_data_lcd(0xC1);
    write_data_lcd(0x48);
    write_data_lcd(0x08);
    write_data_lcd(0x0F);
    write_data_lcd(0x0C);
    write_data_lcd(0x31);
    write_data_lcd(0x36);
    write_data_lcd(0x0F);

}
