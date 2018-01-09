/*
* Aufgabe2.c
*
* Created: 11.12.2017 22:06:28
*  Author: Matthias Hinrichs
*/


#include <avr/io.h>
#include <stdlib.h>
#include "global.h"
#include "adc/adc.h"
#include "lcd/lcd.h"

//Prototypes
uint8_t hygro_read(void);

int main(void)
{
	LCDDDR = 0xFF;
	LCDPORT = 0x00;
	lcd_init();
	adc_init();
	while(1)
	{
		_delay_ms(500);
		lcd_clear();
		uint8_t hygro = hygro_read();
		char hygro_string[4];
		itoa(adc,hygro_string,10);
		lcd_send_string(hygro_string);
	}
}

uint8_t hygro_read(void)
{
	uint8_t value = ADC_MAP_FACTOR/adc_read();
	return value;
}