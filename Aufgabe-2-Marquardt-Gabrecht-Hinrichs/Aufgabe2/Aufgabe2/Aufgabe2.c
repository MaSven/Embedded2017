/*
* Aufgabe2.c
*
* Created: 11.12.2017 22:06:28
*  Author: Matthias Hinrichs
*/


#include <avr/io.h>
#include <stdlib.h>
#include "global.h"
#include "one_wire/ds18s20.h"
#include "lcd.h"

int main(void)
{
	LCDDDR = 0xFF;
	LCDPORT = 0x00;
	lcd_init();
	while(1)
	{
		_delay_ms(500);
		lcd_clear();
		uint8_t temp = ds18s20_read_temperature();
		char adc_string[STRING_CPACITY];
		itoa(adc,adc_string,10);
		lcd_send_string(adc_string);
	}
}