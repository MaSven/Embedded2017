/*
 * Aufgabe2.c
 *
 * Created: 11.12.2017 22:06:28
 *  Author: Matthias Hinrichs
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>		// interrupts
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "lcd/lcd.h"
#include "one_wire/ds18s20.h"

#define UP PA0
#define DOWN PA3
#define ENTER PA4
#define CANCEL PA5

#define IDLE 0
#define MENUE_TIME 1
#define MENUE_TIME_EDIT_H 2
#define MENUE_TIME_EDIT_M 3
#define MENUE_DISPLAY 4
#define MENUE_DISPLAY_TIME 5
#define MENUE_DISPLAY_TIME_TEMP 6
#define MENUE_DISPLAY_TIME_TEMP_LF 7

#define DISPLAY_TIME 10
#define DISPLAY_TIME_TEMP 11
#define DISPLAY_TIME_TEMP_LF 12
#define DEBUG 1

uint8_t volatile menue_state = IDLE;
uint8_t volatile display_state = MENUE_DISPLAY_TIME;
uint8_t volatile enter_was_pressed = 0;
uint8_t volatile up_was_pressed = 0;
uint8_t volatile down_was_pressed = 0;
uint8_t volatile key_was_pressed = 0;
uint8_t volatile IOInterruptEnabled = 1;
volatile uint8_t Lastbutton =0;

// Prototypes
void display_shift_left(int x);
void display_shift_right(int x);
void display_string(void);

int main(void){
	LCDDDR = ((0xF0)|(1<<LCD_RS_PIN)|(1<<LCD_E_PIN));
	LCDPORT = 0x00;
	lcd_init();
		
	while(1)
	{
		display_string();
		_delay_ms(1000);
	}
}

void display_string(void)
{
	char string[] = "Uhrzeit einstellen";
	int string_length = strlen(string);
	
	lcd_set_cursor(1,0);
	lcd_send_string(string);
	
	if (string_length > LCD_COLS)
	{
		display_shift_left(string_length - LCD_COLS);
	}
}

void display_shift_left(int x)
{
	for (int i=0;i<x;i++)
	{
		_delay_ms(500);
		lcd_send_command(LCD_CURSOR_DISPLAY_SHIFT | LCD_CURSOR_DISPLAY_SHIFT_DISPLAY_SHIFT | LCD_CURSOR_DISPLAY_SHIFT_LEFT_SHIFT);
	}
}

void display_shift_right(int x)
{
	_delay_ms(500);
	for (int i=0;i<x;i++)
	{
		lcd_send_command(LCD_CURSOR_DISPLAY_SHIFT | LCD_CURSOR_DISPLAY_SHIFT_DISPLAY_SHIFT | LCD_CURSOR_DISPLAY_SHIFT_RIGHT_SHIFT);
	}	
}