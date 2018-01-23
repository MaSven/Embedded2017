/*
 * clock.c
 *
 * Created: 16.01.2018 14:02:38
 *  Author: Matthias Hinrichs
 */ 

#include <stdio.h>
#include <inttypes.h>
#include "clock/clock.h"
#include "lcd/lcd.h"

volatile uint8_t hours = 0;
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;

volatile uint8_t clock_blink_flag = 0;

void clock_display(uint8_t row,uint8_t col,uint8_t blink_pos)
{
	char time[13];
	
	if (blink_pos == 1 && clock_blink_flag)
	{
		// Stunden nicht anzeigen
		sprintf(time, "  :%.2d:%.2d Uhr",minutes,seconds);
	}
	else if (blink_pos == 2 && clock_blink_flag)
	{
		// Minuten nicht anzeigen
		sprintf(time, "%.2d:  :%.2d Uhr",hours,seconds);
	}
	else
	{
		// Normale Anzeige
		sprintf(time, "%.2d:%.2d:%.2d Uhr",hours,minutes,seconds);
	} 
	lcd_display_string_shift(time, row);
}

void clock_hour_inc(void)
{
	cli();
	hours++;
	if (hours > 23)
	{
		hours = 0;
	}
	sei();
}

void clock_hour_dec(void)
{
	cli();
	hours--;
	if (hours > 23)
	{
		hours = 23;
	}
	sei();
}

void clock_min_inc(void)
{
	cli();
	minutes++;
	if (minutes > 59)
	{
		minutes = 0;
	}
	sei();
}

void clock_min_dec(void)
{
	cli();
	minutes--;
	if (minutes > 59)
	{
		minutes = 59;
	}
	sei();
}
