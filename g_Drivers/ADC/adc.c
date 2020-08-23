 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: adc.c
 *
 * Description: Source file for the ATmega16 ADC driver
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#include "adc.h"


volatile uint16 g_adcResult = 0;

#ifdef ADC_INTERRUPT
ISR(ADC_vect)
{
	/* Read ADC Data after conversion complete */
	g_adcResult = ADC;
}
#endif


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void ADC_init(void)
{
	
	/* ADMUX Register Bits Description:
	/* REFS1:0 = 00 to choose to connect external reference voltage by input this voltage through AREF pin --> Internal Vreff is off.
	/* ADLAR   = 0 right adjusted
	/* MUX4:0  = 00000 to choose channel 0 as initialization
	*/
	ADMUX = 0;


	#ifdef ADC_INTERRUPT

	/* ADCSRA Register Bits Description:
	/* ADEN    = 1 Enable ADC
	/* ADIE    = 1 Enable ADC Interrupt
	/* ADPS2:0 = 011 to choose ADC_Clock=F_CPU/8=1Mhz/8=125Khz --> ADC must operate in range 50-200Khz
	*/

		ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS1) | (1<<ADPS0);

	#else

	/* ADCSRA Register Bits Description:
	/* ADEN    = 1 Enable ADC
	/* ADIE    = 0 Disable ADC Interrupt --> this will work by polling.
	/* ADPS2:0 = 011 to choose ADC_Clock=F_CPU/8=1Mhz/8=125Khz --> ADC must operate in range 50-200Khz
	*/
	ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);

	#endif
}

uint16 ADC_readChannel(uint8 channel_num)
{
	ADMUX = (ADMUX & 0xE0) | (channel_num & 0x07); /* channel number is from 0 to 7 so we only clear the first 3 LSB's
	                                                  channel number value is stored in the first 5 bits in the ADMUX so we clear the first 5 bits in ADMUX
	                                                  finally, setting the channel number in MUX4:0 bits */

	SET_BIT(ADCSRA,ADSC); /* start conversion write '1' to ADSC */
	while(BIT_IS_CLEAR(ADCSRA,ADIF)); /* Polling until ADIF becomes '1' --> conversion is done */
	SET_BIT(ADCSRA,ADIF); /* clear ADIF by write '1' to it */

	#ifdef ADC_INTERRUPT

	return g_adcResult;

	#else

	return ADC; /* return the data register */

	#endif
}
