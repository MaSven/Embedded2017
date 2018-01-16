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
#include "clock/clock.h"

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

#define DISPLAY_MODE_TIME = 13
#define DISPLAY_MODE_TIME_TEMP = 14
#define DISPLAY_MODE_TIME_TEMP_LF = 15

uint8_t volatile menue_state = IDLE;
uint8_t volatile display_state = DISPLAY_MODE_TIME;
uint8_t volatile enter_was_pressed = 0;
uint8_t volatile up_was_pressed = 0;
uint8_t volatile down_was_pressed = 0;
uint8_t volatile key_was_pressed = 0;
uint8_t volatile IOInterruptEnabled = 1;
volatile uint8_t Lastbutton =0;

extern volatile uint8_t hours;
extern volatile uint8_t minutes;
extern volatile uint8_t seconds;


// Prototypes
void timer1Init (void);


int main(void){
	LCDDDR = ((0xF0)|(1<<LCD_RS_PIN)|(1<<LCD_E_PIN));
	LCDPORT = 0x00;
	lcd_init();
	cli();
	timer1Init();
	sei();
<<<<<<< HEAD
    while(1) {
		switch (menue_state) {
		case IDLE:
			// LCD Display Mode
			break;
		case MENUE_TIME:
		//LCD Zeit einstellen
			lcd_clear();
			lcd_send_string("MENUE_TIME");
			key_was_pressed = 0;
			while (key_was_pressed == 0)
			{
				
			}
			key_was_pressed = 0;
			if (enter_was_pressed)
			{
				menue_state = MENUE_TIME_EDIT_H;
			}
			if (down_was_pressed)
			{
				menue_state = MENUE_DISPLAY;
			}
			if (up_was_pressed)
			{
				menue_state = MENUE_DISPLAY;
			}
			enter_was_pressed = 0;
			up_was_pressed = 0;
			down_was_pressed = 0;
			key_was_pressed = 0;
			break;
		case MENUE_TIME_EDIT_H:
		//LCD 
			lcd_clear();
			lcd_send_string("MENUE_TIME_EDIT_H");
			key_was_pressed = 0;
			while (key_was_pressed == 0)
			{
				
			}
			key_was_pressed = 0;
			if (enter_was_pressed)
			{
				menue_state = MENUE_TIME_EDIT_M;
			}
			if (down_was_pressed)
			{
				// Studnen -1
			}
			if (up_was_pressed)
			{
				// Studen +1
			}
			enter_was_pressed = 0;
			up_was_pressed = 0;
			down_was_pressed = 0;
			key_was_pressed = 0;
			break;
		case MENUE_DISPLAY:
			// LCD Nur Urzeit
			lcd_clear();
			lcd_send_string("MENU_DISPLAY");
			key_was_pressed = 0;
			while (key_was_pressed == 0)
			{
				
			}
			key_was_pressed = 0;
			if (enter_was_pressed)
			{
				menue_state = MENUE_DISPLAY_TIME;
			}
			if (down_was_pressed)
			{
				menue_state = MENUE_TIME;
			}
			if (up_was_pressed)
			{
				menue_state = MENUE_TIME;
			}
			enter_was_pressed = 0;
			up_was_pressed = 0;
			down_was_pressed = 0;
			key_was_pressed = 0;
			break;
		case MENUE_DISPLAY_TIME:
			key_was_pressed = 0;
			//LCD Nur Uhrzeit
			lcd_clear();
			lcd_send_string("MENUE_DISPLAY_TIME");
			while (key_was_pressed == 0)
			{
				
			}
			key_was_pressed = 0;
			if (enter_was_pressed)
			{
				display_state = DISPLAY_MODE_TIME;
				menue_state = IDLE;
			}
			if (down_was_pressed)
			{
				menue_state = MENUE_DISPLAY_TIME_TEMP;
			}
			if (up_was_pressed)
			{
				menue_state = MENUE_DISPLAY_TIME_TEMP_LF;
			}
			enter_was_pressed = 0;
			up_was_pressed = 0;
			down_was_pressed = 0;
			key_was_pressed = 0;
			break;
		case MENUE_DISPLAY_TIME_TEMP:
			key_was_pressed = 0;
			//LCD Nur Uhrzeit
			lcd_clear();
			lcd_send_string("MENUE_DISPLAY_TIME_TEMP");
			while (key_was_pressed == 0)
			{
			
			}
			key_was_pressed = 0;
			if (enter_was_pressed)
			{
				display_state = DISPLAY_MODE_TIME_TEMP;
				menue_state = IDLE;
			}
			if (down_was_pressed)
			{
				menue_state = MENUE_DISPLAY_TIME_TEMP_LF;
			}
			if (up_was_pressed)
			{
				menue_state = MENUE_DISPLAY_TIME;
			}
			enter_was_pressed = 0;
			up_was_pressed = 0;
			down_was_pressed = 0;
			key_was_pressed = 0;
			break;
		case MENUE_DISPLAY_TIME_TEMP_LF:
			key_was_pressed = 0;
			//LCD Nur Uhrzeit
			lcd_clear();
			lcd_send_string("MENUE_DISPLAY_TIME_TEMP_LF");
			while (key_was_pressed == 0)
			{
				
			}
			key_was_pressed = 0;
			if (enter_was_pressed)
			{
				display_state = DISPLAY_MODE_TIME_TEMP_LF;
				menue_state = IDLE;
			}
			if (down_was_pressed)
			{
				menue_state = MENUE_DISPLAY_TIME;
			}
			if (up_was_pressed)
			{
				menue_state = MENUE_DISPLAY_TIME_TEMP;
=======
	while (1)
	{
		clock_display();
	}
}

void timer1Init(void)
{
	TCCR1A = 0x00;
	TCCR1B |= ((1<<WGM12)|(1<<CS12)); // CTC ON, Prescaler 1024
	// Timer 1,0s
	// 1 * 8.000.000 / 256 = 31250
	OCR1A = 31250;
	// Interrupt aktivieren
	TIMSK1 |= (1<<OCIE1A);
	// Zaehlregister auf 0
	TCNT1 = 0x00;
}

ISR (TIMER1_COMPA_vect)
{
	seconds++;
	if (seconds == 60)
	{
		seconds = 0;
		minutes++;
		if (minutes == 60)
		{
			minutes = 0;
			hours++;
			if (hours == 24)
			{
				hours = 0;
>>>>>>> origin/clock
			}
		}
	}
	
}