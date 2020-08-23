 /******************************************************************************
 *
 * Module: WATCHDOG
 *
 * File Name: watchdog.c
 *
 * Description: Source file for the Watchdog driver
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#include "watchdog.h"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


void WDT_ON(void)
{
	// Watchdog timer enables with timeout period 2.1 second.
	WDTCR = (1<<WDE)|(1<<WDP2)|(1<<WDP1)|(1<<WDP0);
}



void WDT_OFF(void)
{
	// set the WDTOE & WDE bits in the same operation
	WDTCR = (1<<WDTOE)|(1<<WDE);
	//wait 4 cycles before clear the WDE bit
	_delay_us(4);
	WDTCR = 0x00;
}





