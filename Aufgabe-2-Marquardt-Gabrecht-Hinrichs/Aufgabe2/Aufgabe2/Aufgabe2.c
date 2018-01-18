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
#include "adc/adc.h"
#include "adc/hygro.h"
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
#define MENUE_DISPLAY_TEMP_LF 7
#define MENUE_DISPLAY_TIME_TEMP_LF 8

#define DISPLAY_TIME 10
#define DISPLAY_TIME_TEMP 11
#define DISPLAY_TIME_TEMP_LF 12
#define DEBUG 1

#define DISPLAY_MODE_TIME 13
#define DISPLAY_MODE_TIME_TEMP 14
#define DISPLAY_MODE_TEMP_LF 16
#define DISPLAY_MODE_TIME_TEMP_LF 17

uint8_t volatile menue_state = MENUE_TIME_EDIT_H;
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
			}
		}
	}
	
}

void external_interrupt_init(){
	PCMSK0 |= ((1<<UP)|(1<<DOWN)|(1<<ENTER)|(1<<CANCEL));
	PCICR |= (1<<PCIE0);
}

//External Interrupt ausgeloest
ISR (PCINT0_vect){
	volatile uint8_t button = PINA & ((1<<UP)|(1<<DOWN)|(1<<ENTER)|(1<<CANCEL));
	if(!button){
		if(IOInterruptEnabled) {
			//IOInterruptEnabled = 0;
			key_was_pressed = 1;
			if(Lastbutton == (1<<CANCEL)){
				menue_state = IDLE;
			}
			if(Lastbutton == (1<<ENTER)){
				if (menue_state == IDLE)
				{
					menue_state = MENUE_TIME;
				}
				enter_was_pressed = 1;
			}
			if(Lastbutton == (1<<UP)){
				up_was_pressed = 1;
			}
			if(Lastbutton == (1<<DOWN)){
				down_was_pressed = 1;
			}
		}
		} else {
		Lastbutton = button;
	}
}





// Prototypes
void timer1Init (void);


int main(void){
	LCDDDR = ((0xF0)|(1<<LCD_RS_PIN)|(1<<LCD_E_PIN));
	LCDPORT = 0x00;
	lcd_init();
	cli();
	timer1Init();
	external_interrupt_init();
	sei();
	while(1) {
		switch (menue_state) {
			case IDLE:
			switch(display_state){
				case DISPLAY_MODE_TIME:
				// Anforderrung f) i.
				lcd_clear();
				clock_display();
				break;
				case DISPLAY_MODE_TIME_TEMP:
				// Anforderrung f) ii.
				lcd_clear();
				clock_display();
				lcd_set_cursor(2, 0);
				lcd_send_string("100 Grad ");//
				break;
				case DISPLAY_MODE_TEMP_LF:
				// Anforderrung f) iii.
				lcd_clear();
					lcd_send_string("100 Grad ");
					hygro_display(0,2);
					lcd_send_string("%");
				break;
				case DISPLAY_MODE_TIME_TEMP_LF:
				// Anforderrung f) iV.
				lcd_clear();
				if (seconds<30)
				{
					clock_display();
				}
				else
				{
					lcd_send_string("100 Grad ");
					hygro_display(0,2);
					lcd_send_string("%");
				}
				break;
			}
			break;
			case MENUE_TIME:
			//LCD Zeit einstellen
			lcd_clear();
			lcd_send_string("Uhrzeit");
			lcd_set_cursor(2, 0);
			lcd_send_string("einstellen");
			key_was_pressed = 0;
			while (key_was_pressed == 0)
			{
				// warte auf eingbabe
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
			lcd_clear();
			clock_display();
			lcd_set_cursor(2, 0);
			lcd_send_string("Stunden edit");	// bearbeiten w�re zu lang
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
				clock_hour_dec();
			}
			if (up_was_pressed)
			{
				clock_hour_inc();
			}
			enter_was_pressed = 0;
			up_was_pressed = 0;
			down_was_pressed = 0;
			key_was_pressed = 0;
			break;
			case MENUE_TIME_EDIT_M:
			//LCD
			lcd_clear();
			clock_display();
			lcd_set_cursor(2, 0);
			lcd_send_string("Minuten edit");	// bearbeiten w�re zu lang
			key_was_pressed = 0;
			while (key_was_pressed == 0)
			{
				
			}
			key_was_pressed = 0;
			if (enter_was_pressed)
			{
				menue_state = IDLE;
			}
			if (down_was_pressed)
			{
				clock_min_dec();
			}
			if (up_was_pressed)
			{
				clock_min_inc();
			}
			enter_was_pressed = 0;
			up_was_pressed = 0;
			down_was_pressed = 0;
			key_was_pressed = 0;
			break;
			case MENUE_DISPLAY:
			// LCD Nur Urzeit
			lcd_clear();
			lcd_send_string("Display");//---------------------
			lcd_set_cursor(2, 0);
			lcd_send_string("einstellen");
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
			lcd_send_string("NZeit");
			lcd_set_cursor(2, 0);
			lcd_send_string("anzeigen");
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
			lcd_send_string("Temperatur/Zeit");
			lcd_set_cursor(2, 0);
			lcd_send_string("anzeigen");
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
				menue_state = MENUE_DISPLAY_TEMP_LF;
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
			case MENUE_DISPLAY_TEMP_LF:
			key_was_pressed = 0;
			//LCD Nur Uhrzeit
			lcd_clear();
			lcd_send_string("Temp/Luftfeuchte");
			lcd_set_cursor(2, 0);
			lcd_send_string("anzeigen");
			while (key_was_pressed == 0)
			{
				
			}
			key_was_pressed = 0;
			if (enter_was_pressed)
			{
				display_state = DISPLAY_MODE_TEMP_LF;
				menue_state = IDLE;
			}
			if (down_was_pressed)
			{
				menue_state = MENUE_DISPLAY_TIME_TEMP_LF;
			}
			if (up_was_pressed)
			{
				menue_state = MENUE_DISPLAY_TIME_TEMP;
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
			lcd_send_string("Temp/Uhr wechsel");
			lcd_set_cursor(2, 0);
			lcd_send_string("anzeigen");
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
			}
			enter_was_pressed = 0;
			up_was_pressed = 0;
			down_was_pressed = 0;
			key_was_pressed = 0;
		}
	}
}