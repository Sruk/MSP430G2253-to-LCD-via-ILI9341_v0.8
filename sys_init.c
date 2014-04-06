/*
 * sys_init.c
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
#include "sys_init.h"

#ifndef SYS_INIT_C_
#define SYS_INIT_C_

/* Init. system, CLOCK, COMMUNICATION, GPIO */

void sys_init(){

	DCOCTL = 0;					// Select lowest DCOx and MODx settings (1MHz)
	BCSCTL1 |= CALBC1_8MHZ;				// Set DCO to 8MHz
	DCOCTL = CALDCO_8MHZ;
	//BCSCTL2 |= DIVS_1;				// SMCLK = MCLK(=DCO) / 2 --> SMCLK = 4MHz

	P1OUT = 0x00;					// P1 -> reset
	P2OUT = 0x00;					// P2 -> reset

	P1DIR = BIT0 + BIT6 + BIT5;			// configuration of P1 DIRection registers -> output
	P2DIR = BIT3 + BIT4 + BIT5;			// configuration of P1 DIRection registers -> output

	P1SEL = BIT1 + BIT2  + BIT4;			// configure P1 modules to SPI mode
	P1SEL2 = BIT1 + BIT2  + BIT4;			// --

	UCA0CTL1 |= UCSWRST; 				// **STOP USCI**
	UCA0CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC;  	// 3-pin to send, 8-bit SPI master, sync., MSBit out first
	UCA0CTL1 = UCSSEL_2;                     	// SMCLK -> generates clock for USCI
	UCA0BR0 = 0;                          		// USCI CLK divider
	UCA0BR1 = 0;                              	//
	UCA0MCTL = 0;                             	// no modulation
	UCA0CTL1 &= ~UCSWRST;                     	// **START USCI**


	P1OUT |= BIT5;					// CS signal, initialized to HIGH, active LOW

	P1OUT &= ~BIT0; 				// P1.0 set LOW -> RED status LED

	P2OUT |= BIT3 + BIT4;				// LCD RESET & C/D -> HIGH (used for LCD DISABLE & LCD DATA ), active LOW

	P1OUT |= BIT6;					// GREEN status LED set on after sys_init is done
}

#endif /* INIT_C_ */
