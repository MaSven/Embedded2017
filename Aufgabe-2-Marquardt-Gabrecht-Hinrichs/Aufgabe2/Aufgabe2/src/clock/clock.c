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

volatile uint8_t hours = 23;
volatile uint8_t minutes = 59;
volatile uint8_t seconds = 45;

void clock_display(void)
{
	char time[12];
	sprintf(time, "%.2d:%.2d:%.2d Uhr",hours,minutes,seconds);
	lcd_set_cursor(1,2);
	lcd_send_string(time);
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