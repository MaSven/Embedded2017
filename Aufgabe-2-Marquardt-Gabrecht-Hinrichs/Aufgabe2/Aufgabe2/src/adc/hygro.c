/*
 * hygro.c
 *
 * Created: 17.01.2018 12:12:56
 *  Author: Matthias Hinrichs
 */ 
#include <stdio.h>
#include "adc/hygro.h"
#include "adc/adc.h"
#include "lcd/lcd.h"

void hygro_display(int row, int col)
{
	uint8_t adc_value = adc_read()/HYGRO_MAP_FACTOR;
	char hygro_str[5];
	sprintf(hygro_str,"%3u %%",adc_value);
	lcd_set_cursor(row, col);
	lcd_send_string(hygro_str);
}