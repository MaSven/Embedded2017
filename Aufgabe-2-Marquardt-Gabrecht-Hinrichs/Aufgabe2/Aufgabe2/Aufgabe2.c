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

int main(void)
{
	lcd_init();
	adc_init();
    while(1)
    {
          
		  uint8_t adc = adc_read();
		  char adc_string[4];
		  itoa(adc,adc_string,10);
		  lcd_send_string("Hallo Welt");
    }
}