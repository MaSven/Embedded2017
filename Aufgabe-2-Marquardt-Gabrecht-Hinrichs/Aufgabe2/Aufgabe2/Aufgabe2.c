/*
 * Aufgabe2.c
 *
 * Created: 11.12.2017 22:06:28
 *  Author: Matthias Hinrichs
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>		// interrupts

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

uint8_t menue_state = IDLE;
uint8_t display_state = MENUE_DISPLAY_TIME;
uint8_t enter_was_pressed = 0;
uint8_t up_was_pressed = 0;
uint8_t down_was_pressed = 0;
uint8_t key_was_pressed = 0;
uint8_t volatile IOInterruptEnabled = 1;
volatile uint8_t Lastbutton =0;

void external_interrupt_init(){
	PCMSK0 |= ((1<<UP)|(1<<DOWN)|(1<<ENTER)|(1<<CANCEL));
	PCICR |= (1<<PCIE0);
}

//External Interrupt ausgeloest
ISR (PCINT0_vect){
	volatile uint8_t button = PINA & ((1<<UP)|(1<<DOWN)|(1<<ENTER)|(1<<CANCEL));
	if(!button){
		if(IOInterruptEnabled) {
			IOInterruptEnabled = 0;
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

int main(void)
{
	cli();
	external_interrupt_init();
	sei();
    while(1)
    {
		switch (menue_state)
		{
		case IDLE:
			// LCD Display Mode
			break;
		case MENUE_TIME:
		//LCD Zeit einstellen
			while (key_was_pressed == 0)
			{
				
			}
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
			//LCD Nur Uhrzeit
			while (key_was_pressed == 0)
			{
				
			}
			key_was_pressed = 0;
			if (enter_was_pressed)
			{
				display_state = DISPLAY_TIME;
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
			//LCD Nur Uhrzeit
			while (key_was_pressed == 0)
			{
			
			}
			key_was_pressed = 0;
			if (enter_was_pressed)
			{
				display_state = DISPLAY_TIME_TEMP;
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
			//LCD Nur Uhrzeit
			while (key_was_pressed == 0)
			{
				
			}
			key_was_pressed = 0;
			if (enter_was_pressed)
			{
				display_state = DISPLAY_TIME_TEMP_LF;
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
			break;
		}
	}
}