/**
 * Aufgabe2.c
 *
 * \date 11.12.2017 22:06:28
 *  \author Marco Gabrecht
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "global.h"
#include "lcd/lcd.h"
#include "adc/adc.h"
#include "adc/hygro.h"
#include "one_wire/ds18s20.h"
#include "clock/clock.h"

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
#define CLOCK_UPDATE_TIME 20
#define ENTER_WAIT_TIME 5

uint8_t volatile menue_state = MENUE_TIME_EDIT_H;
uint8_t volatile display_state = DISPLAY_MODE_TIME;
uint8_t volatile enter_was_pressed = 0;
uint8_t volatile up_was_pressed = 0;
uint8_t volatile down_was_pressed = 0;
uint8_t volatile key_was_pressed = 0;
uint8_t volatile IOInterruptEnabled = 1;
volatile uint8_t Lastbutton = 0;

extern volatile uint8_t hours;
extern volatile uint8_t minutes;
extern volatile uint8_t seconds;

volatile uint8_t clock_flag = 1;
volatile uint8_t temp_flag = 1;
volatile uint8_t temp_counter = 0;
volatile uint8_t hygro_flag = 1;
volatile uint8_t hygro_counter = 0;
volatile uint8_t display_mode_change_flag = 0;
volatile uint8_t display_mode_change_flag_counter = 0;
volatile uint8_t clock_blink_counter = 0;
volatile uint8_t debounce_counter = 0;
volatile uint8_t lcd_shift_flag_counter = 0;
volatile uint8_t lcd_clear_flag = 0;
extern volatile uint8_t lcd_shift_flag;
extern volatile uint8_t clock_blink_flag;
extern volatile uint8_t lcd_shift_abort;

// Prototypes
void timer0Init(void);
void timer1Init(void);
void pin_change_interrupt_init(void);
void init(void);

/**
 * Setzt alle Flags der Tasten zur�ck
 */
inline void global_keys_reset(void) {
	if (key_was_pressed) {
		lcd_clear();
	}
	enter_was_pressed = 0;
	up_was_pressed = 0;
	down_was_pressed = 0;
	key_was_pressed = 0;
}
/**
 * Erfuellt anforderung der Aufgabe f) i
 */
inline void display_mode_time() {
	if (clock_flag) {
		clock_display(1, 0);
		clock_flag = 0;
	}
}
/**
 * Erfuellt anforderung der Aufgabe f) ii
 */
inline void display_mode_time_temp(void) {
	if (clock_flag) {
		clock_display(1, 0);
		clock_flag = 0;
	}
	if (temp_flag) {
		temp_display(2);
		temp_flag = 0;
	}
}

/**
 * Zeigt wenn n�tig Temperatur und Luftfeuchtigkeit auf dem Display an
 */
inline void display_mode_temp_lf(void) {
	// Anforderung f) iii.
	if (temp_flag) {
		temp_display(1);
		temp_flag = 0;
	}
	if (hygro_flag) {
		hygro_display(2);
		hygro_flag = 0;
	}
}

/**
 * Zeigt wenn n�tig Temperatur und Luftfeuchtigkeit im wechsel mit der Uhrzeit auf dem Display an
 */
inline void display_mode_time_temp_lf(void) {
	// Anforderung f) iV.
	// Uhrzeit im Wechsel mit Temperatur/Luftfeuchtigkeit
	if (lcd_clear_flag) {
		lcd_clear_flag = 0;
		lcd_clear();
	}
	if (display_mode_change_flag) {
		if (temp_flag) {
			temp_display(1);
			temp_flag = 0;
		}
		if (hygro_flag) {
			hygro_display(2);
			hygro_flag = 0;
		}
	} else {
		if (clock_flag) {
			clock_display(1, 0);
			clock_flag = 0;
		}
	}
}

/**
 * Zeigt an das man die Uhrzeit einstellen kann auf dem Display
 */
inline void menu_time(void) {
	//LCD Uhrzeit einstellen
	lcd_display_string_shift("Uhrzeit einstellen", 1);
	if (enter_was_pressed) {
		menue_state = MENUE_TIME_EDIT_H;
		lcd_clear();
	}
	if (down_was_pressed) {
		menue_state = MENUE_DISPLAY;
		lcd_clear();
	}
	if (up_was_pressed) {
		menue_state = MENUE_DISPLAY;
		lcd_clear();
	}
	global_keys_reset();
}

/**
 * Zeigt die Uhrzeit an mit Blinkenden Stunden und reagiert auf die Buttonflags um die Stunden zu ver�ndern.
 */
inline void menu_time_edit_h(void) {
	clock_display(1, 1);
	if (enter_was_pressed) {
		menue_state = MENUE_TIME_EDIT_M;
		lcd_clear();
	}
	if (down_was_pressed) {
		clock_hour_dec();
	}
	if (up_was_pressed) {
		clock_hour_inc();
	}
	global_keys_reset();
}

/**
 * Zeigt die Uhrzeit an mit Blinkenden Minuten und reagiert auf die Buttonflags um die Minuten zu ver�ndern.
 */
inline void menu_time_edit_m(void) {
	clock_display(1, 2);
	if (enter_was_pressed) {
		menue_state = IDLE;
		lcd_clear();
	}
	if (down_was_pressed) {
		clock_min_dec();
	}
	if (up_was_pressed) {
		clock_min_inc();
	}
	global_keys_reset();
}

/**
 * Zeigt die Eisntellung zum Display modus auf dem Display an.
 */
inline void menu_display(void) {
	// LCD Nur Urzeit
	lcd_display_string_shift("Displaymodus einstellen", 1);
	if (enter_was_pressed) {
		menue_state = MENUE_DISPLAY_TIME;
		lcd_clear();
	}
	if (down_was_pressed) {
		menue_state = MENUE_TIME;
		lcd_clear();
	}
	if (up_was_pressed) {
		menue_state = MENUE_TIME;
		lcd_clear();
	}
	global_keys_reset();
}

/**
 * Zeigt die Uhrzeit an und kann mit enter best�tigt werden.
 */
inline void menue_display_time(void) {
	//LCD Nur Uhrzeit
	lcd_display_string_shift("Uhrzeit", 1);
	if (enter_was_pressed) {
		display_state = DISPLAY_MODE_TIME;
		menue_state = IDLE;
		lcd_clear();
	}
	if (down_was_pressed) {
		menue_state = MENUE_DISPLAY_TIME_TEMP;
		lcd_clear();
	}
	if (up_was_pressed) {
		menue_state = MENUE_DISPLAY_TIME_TEMP_LF;
		lcd_clear();
	}
	global_keys_reset();
}

/**
 * Zeigt wenn n�tig die Zeit auf dem Display an
 */
inline void menue_display_time_temp(void) {
	//LCD Uhrzeit und Temperatur
	lcd_display_string_shift("Uhrzeit und Temperatur", 1);
	if (enter_was_pressed) {
		display_state = DISPLAY_MODE_TIME_TEMP;
		menue_state = IDLE;
		lcd_clear();
	}
	if (down_was_pressed) {
		menue_state = MENUE_DISPLAY_TEMP_LF;
		lcd_clear();
	}
	if (up_was_pressed) {
		menue_state = MENUE_DISPLAY_TIME;
		lcd_clear();
	}
	global_keys_reset();
}

/**
 * Zeigt den Men�punkt Temperatur und Luftfeuchtigkeit auf dem Display an
 */
inline void menue_display_temp_lf(void) {
	//LCD Temperatur und Luftfeuchtigkeit
	lcd_display_string_shift("Temperatur und Luftfeuchtigkeit", 1);
	if (enter_was_pressed) {
		display_state = DISPLAY_MODE_TEMP_LF;
		menue_state = IDLE;
		lcd_clear();
	}
	if (down_was_pressed) {
		menue_state = MENUE_DISPLAY_TIME_TEMP_LF;
		lcd_clear();
	}
	if (up_was_pressed) {
		menue_state = MENUE_DISPLAY_TIME_TEMP;
		lcd_clear();
	}
	global_keys_reset();
}

/**
 * Zeigt den Men�punkt Temperatur und Luftfeuchtigkeit auf dem Display an
 */
inline void menue_display_time_temp_lf(void) {
	//LCD Temperatur&Luftfeuchtigkeit im Wechsel mit Uhrzeit
	lcd_display_string_shift("Uhrzeit oder Temperatur&Luftfeuchtigkeit", 1);
	if (enter_was_pressed) {
		display_state = DISPLAY_MODE_TIME_TEMP_LF;
		menue_state = IDLE;
		lcd_clear();
	}
	if (down_was_pressed) {
		menue_state = MENUE_DISPLAY_TIME;
		lcd_clear();
	}
	if (up_was_pressed) {
		menue_state = MENUE_DISPLAY_TIME_TEMP;
		lcd_clear();
	}
	global_keys_reset();
}

/**
 * Der Ablauf des Men�s und hier werden alle aufrufe get�tigt, die nicht interrupt gesteuert sind.
 */
int main(void) {
	init();
	while (1) {
		switch (menue_state) {

		case IDLE:
			if (enter_was_pressed) {
				menue_state = MENUE_TIME;
				enter_was_pressed = 0;
			} else {
				switch (display_state) {
				case DISPLAY_MODE_TIME:
					display_mode_time();
					break;
				case DISPLAY_MODE_TIME_TEMP:
					display_mode_time_temp();
					break;
				case DISPLAY_MODE_TEMP_LF:
					display_mode_temp_lf();
					break;
				case DISPLAY_MODE_TIME_TEMP_LF:
					display_mode_time_temp_lf();
					break;
				}
			}
			break;
		case MENUE_TIME:
			menu_time();
			break;
		case MENUE_TIME_EDIT_H:
			menu_time_edit_h();
			break;
		case MENUE_TIME_EDIT_M:
			menu_time_edit_m();
			break;
		case MENUE_DISPLAY:
			menu_display();
			break;
		case MENUE_DISPLAY_TIME:
			menue_display_time();
			break;
		case MENUE_DISPLAY_TIME_TEMP:
			menue_display_time_temp();
			break;
		case MENUE_DISPLAY_TEMP_LF:
			menue_display_temp_lf();
			break;
		case MENUE_DISPLAY_TIME_TEMP_LF:
			menue_display_time_temp_lf();
			break;
		}
	}
}

/**
 * Initiirt alle Interrupts und Komponenten
 */
void init(void) {
	cli();
	lcd_init();
	adc_init();
	timer0Init();
	timer1Init();
	pin_change_interrupt_init();
	sei();
}

/**
 * initiirt einen Timerinterrupt
 */
void timer0Init(void) {
	TCCR0A |= (1 << WGM01); // CTC ON
	TCCR0B |= ((1 << CS02) | (1 << CS00)); // Prescaler 1024
	// Timer 25ms, 0,025s
	// 0,025 * 8.000.000 / 1024 = 195,3125
	OCR0A = 195;
	// Interrupt aktivieren
	TIMSK0 |= (1 << OCIE0A);
	// Zaehlregister auf 0
	TCNT0 = 0x00;
}

ISR (TIMER0_COMPA_vect) {
	clock_blink_counter++;
	debounce_counter++;
	lcd_shift_flag_counter++;
	if (clock_blink_counter == CLOCK_UPDATE_TIME) {
		clock_blink_counter = 0;
		clock_blink_flag ^= 1;
	}
	if (debounce_counter == ENTER_WAIT_TIME) {
		debounce_counter = 0;
		IOInterruptEnabled = 1;
	}
	if (lcd_shift_flag_counter == 20) {
		lcd_shift_flag_counter = 0;
		lcd_shift_flag = 1;
	}
}

void timer1Init(void) {
	TCCR1A = 0x00;
	TCCR1B |= ((1 << WGM12) | (1 << CS12)); // CTC ON, Prescaler 1024
	// Timer 1,0s
	// 1 * 8.000.000 / 256 = 31250
	OCR1A = 31250;
	// Interrupt aktivieren
	TIMSK1 |= (1 << OCIE1A);
	// Zaehlregister auf 0
	TCNT1 = 0x00;
}

ISR (TIMER1_COMPA_vect) {
	temp_counter++;
	hygro_counter++;
	display_mode_change_flag_counter++;
	seconds++;
	if (seconds == 60) {
		seconds = 0;
		minutes++;
		if (minutes == 60) {
			minutes = 0;
			hours++;
			if (hours == 24) {
				hours = 0;
			}
		}
	}
	clock_flag = 1;
	if (temp_counter == 9) {
		temp_counter = 0;
		temp_flag = 1;
	}
	if (hygro_counter == 5) {
		hygro_counter = 0;
		hygro_flag = 1;
	}
	//LCD muss wechseln
	if (display_mode_change_flag_counter == 10) {
		display_mode_change_flag_counter = 0;
		lcd_clear_flag = 1;
		display_mode_change_flag ^= 1;
		temp_flag = 1;
		hygro_flag = 1;
	}
}

void pin_change_interrupt_init() {
	PCMSK0 |= ((1 << UP) | (1 << DOWN) | (1 << ENTER) | (1 << CANCEL));
	PCICR |= (1 << PCIE0);
}

//Pin Change Interrupt ausgeloest
ISR (PCINT0_vect) {
	volatile uint8_t button = PINA
			& ((1 << UP) | (1 << DOWN) | (1 << ENTER) | (1 << CANCEL));
	if (IOInterruptEnabled) {
		if (!button) {

			IOInterruptEnabled = 0;
			key_was_pressed = 1;
			lcd_shift_abort = 1;
			if (Lastbutton == (1 << CANCEL)) {
				menue_state = IDLE;
			}
			if (Lastbutton == (1 << ENTER)) {
				enter_was_pressed = 1;
			}
			if (Lastbutton == (1 << UP)) {
				up_was_pressed = 1;
			}
			if (Lastbutton == (1 << DOWN)) {
				down_was_pressed = 1;
			}

			button = 0;
		} else {
			Lastbutton = button;
		}
	}
}
