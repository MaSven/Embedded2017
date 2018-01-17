/*
 * Aufgabe2.c
 *
 * Created: 11.12.2017 22:06:28
 *  Author: Matthias Hinrichs
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>		// interrupts
#include <stdlib.h>
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

int main(void){
	LCDDDR = ((0xF0)|(1<<LCD_RS_PIN)|(1<<LCD_E_PIN));
	LCDPORT = 0x00;
	lcd_init();
		
	while(1)
	{
		lcd_set_cursor(1,0);
		lcd_send_string("Test1");
		lcd_set_cursor(1,4);
		lcd_send_string("Test2");
		lcd_set_cursor(2,4);
		lcd_send_string("Test3");
		_delay_ms(500);
		lcd_clear();
		_delay_ms(500);
	}
	/*while (1)
	{
		uint8_t changeDDRAMCommand = 0x80; // 0b1000.0000
		uint8_t secondRowCommand = 0x40; // 0b0100.0000
		uint8_t firstRowFirstCol = ((changeDDRAMCommand)|0);
		uint8_t firstRowFifthCol = ((changeDDRAMCommand)|5);
		uint8_t secondRowFirstCol = ((changeDDRAMCommand)|(secondRowCommand)|0);
		uint8_t secondRowFifthCol = ((changeDDRAMCommand)|(secondRowCommand)|5);
		
		lcd_clear();
		_delay_ms(500);
		lcd_send_command(firstRowFirstCol);
		lcd_send_string("Test1");
		lcd_send_command(firstRowFifthCol);
		lcd_send_string("Test2");
		lcd_send_command(secondRowFirstCol);
		lcd_send_string("Test3");
		lcd_send_command(secondRowFifthCol);
		lcd_send_string("Test4");
		_delay_ms(500);
	}*/
	/*while (1)
	{
		uint8_t changeDDRAMCommand = 0x80; // 0b1000.0000
		uint8_t secondRowCommand = 0x40; // 0b0100.0000
		uint8_t firstRowFirstCol = ((changeDDRAMCommand)|0);
		uint8_t firstRowFifthCol = ((changeDDRAMCommand)|5);
		uint8_t secondRowFirstCol = ((changeDDRAMCommand)|(secondRowCommand)|0);
		uint8_t secondRowFifthCol = ((changeDDRAMCommand)|(secondRowCommand)|5);
		
		lcd_send_command(firstRowFirstCol);
		_delay_ms(500);
		lcd_send_command(firstRowFifthCol);
		_delay_ms(500);
		lcd_send_command(secondRowFirstCol);
		_delay_ms(500);
		lcd_send_command(secondRowFifthCol);
		_delay_ms(500);
	}*/
}