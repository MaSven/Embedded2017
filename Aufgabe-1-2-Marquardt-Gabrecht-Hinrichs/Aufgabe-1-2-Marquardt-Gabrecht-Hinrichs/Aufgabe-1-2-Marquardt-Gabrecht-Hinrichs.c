/*
 * Simpleblink
 * Demo1-1.c 
 *
 * Created: 01.04.2011 22:05:04
 *  Author: Ole
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>		// interrupts
#include <inttypes.h>			// Datentypen mit Bitbreiten
#include "pattern_save.h"
//#define SIMULATOR 




#define INITTIMERCOMPARE 977
#define TIMER_CANCEL 5*INITTIMERCOMPARE
#define UP PA0
#define DOWN PA3
#define LEFT PA1
#define RIGHT PA2
#define ENTER PA4
#define CANCEL PA5
#define LED_LEFT PA6
#define LED_RIGHT PC6
#define LED_UP PB5
#define LED_DOWN PB7
#define INPUT_PIN PINA
#define OUTPORT_LEFT PORTA
#define OUTPORT_UP_DOWN PORTB
#define OUTPORT_RIGHT PORTC
#define OUTDDR_LEFT DDRA
#define OUTDDR_UP_DOWN DDRB
#define OUTDDR_RIGHT DDRC
#define GAME_MODE 1
#define IDLE 0
#define END_SCREEN 2

enum BUTTON_DIRECTION{
	UP_PATTERN,DOWN_PATTERN,LEFT_PATTERN,RIGHT_PATTERN
	};



/************************************************************************/
/* Initialisieren von Timer1.                                           */
/************************************************************************/
void timer1Init () {
	// Taktquelle ist per Default der Systemtakt, Asynchrones Verhalten
	// wird in separatem ASSR ? Asynchronous Status Register konfiguriert

	// Timer-Modus: Clear Timer on Compare match (CTC)
	// d.h Zählen bis Wert in OCR1A, dann Reset
	// Konfigbits dafür in TCCR1A und TCCR1B verteilt
	// Prescaler steht in Bit 2..0 von TCCR1B
	TCCR1A = 0x00;		//CTC ON
	TCCR1B = 0x0D;		//Prescaler 1024; Grundfreq = 1 MHz / 1024 = 977Hz
	// mit Fuse CLKDIV8 gesetzt sind es effektiv 1MHz
	
	// Konfigurieren der "Output Compare Units"
	// (Vergleich des Zählerwertes auf Schwellwert --> Interrupt)
	OCR1BH = 0x00;		// 0,2*1.000.000 / 1024 = 195 für 0,2s
	//OCR1BL = 0xC3;		// 194 -> 0x00C3
	OCR1A= TIMER_CANCEL;
	// 1.000.000 / 1024 = 977 für eine Sekunde
	// Nun die relevanten Interrupts aktivieren: Timer Interrupt Mask
	TIMSK1 = (1<<1);	// Bit 1 ? OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	//TIMSK1 |= (1<<2);	// Bit 2 ? OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Enable
	
	TCNT1 = 0x00;		// Zählregister des Timers noch auf Null stellen
}

void timer0Init() {
	TCCR0A |= (1<<WGM01); // Normal Mode
	TCCR0B |= ((1<<CS02)|(1<<CS00)); // Prescaler 1024
	
	OCR0A = 0xC3; // 0,2*1.000.000/1024 = 195 für 0,2s
	OCR0B = 0x00;
	
	TIMSK0 |= (1<<OCIE0A);
	TCNT0 = 0x00;
}

//Speichert den momentanen State des Spieles
uint8_t game_state = IDLE;
uint8_t nextButton = 0;
uint8_t nextButtonPressed = 0;

uint8_t volatile IOInterruptEnabled = 1;

uint8_t volatile LightOn=0;

//Programm wird initialisiert
void init();
//Programm wird wieder in den IDLE zustand gestellt
void reset();
//Programm wird in der Sieger zustand gestellt
void winner();
//Starte das Spiel mit zwei spielern
void start_game();
//Schaltet nur die gegebene LED an
void set_led(uint8_t led);
//Schaltet nur die gegebene LED aus
void clear_led(uint8_t led);
//Schaltet alle LEDs aus
void clear_all_led();



/************************************************************************/
/* Interrupt Service Routinen für Timer1								*/
/************************************************************************/

/* Compare Interrupt A													*/
/* schaltet die LED ein													*/
/************************************************************************/
ISR (TIMER1_COMPA_vect) {
	
}

ISR (TIMER0_COMPA_vect) {
	IOInterruptEnabled = 1;
	LightOn = 0;
}

//External Interrupt ausgeloest
ISR (PCINT0_vect){
	if(IOInterruptEnabled) {
		IOInterruptEnabled = 0;
		nextButtonPressed = 1;
		volatile uint8_t button = PINA;
		button &= ~(1<<LED_LEFT);
		if(button == (1<<CANCEL)){
			//reset();
			game_state = IDLE;
		}
		if(game_state==IDLE){
			if(button == (1<<ENTER)){
				game_state = GAME_MODE;
			}
			}else if(game_state==GAME_MODE){
			
			
			if (nextButton != button){
				game_state = END_SCREEN;
			}
		}
	}	
}

void external_interrupt_init(){
	PCMSK0 |= ((1<<UP)|(1<<DOWN)|(1<<LEFT)|(1<<RIGHT)|(1<<ENTER)|(1<<CANCEL));
	EICRA |= ((1<<ISC01)|(1<<ISC00));
	PCICR |= (1<<PCIE0);
}

void init() {
	//DDRs setzen
	OUTDDR_LEFT |= (1<<LED_LEFT);
	OUTDDR_UP_DOWN |= ((1<<LED_UP)|(1<<LED_DOWN));
	OUTDDR_RIGHT |= (1<<LED_RIGHT);
	//Alle LEDs abschalten
	clear_led(LED_LEFT);
	clear_led(LED_UP);
	clear_led(LED_DOWN);
	clear_led(LED_RIGHT);
	//Interrupts einstellen und aktivieren
	cli();
	timer1Init();
	timer0Init();
	external_interrupt_init();
	sei();
}

void set_led(uint8_t led) {
	switch (led)
	{
	case 'L':
		OUTPORT_LEFT |= (1<<LED_LEFT);
		break;
	case 'U':
		OUTPORT_UP_DOWN |= (1<<LED_UP);
		break;
	case 'D':
		OUTPORT_UP_DOWN |= (1<<LED_DOWN);
		break;
	case 'R':
		OUTPORT_RIGHT |= (1<<LED_RIGHT);
		break;
	}
}

void clear_led(uint8_t led) {
	switch (led)
	{
		case 'L':
		OUTPORT_LEFT &= ~(1<<LED_LEFT);
		break;
		case 'U':
		OUTPORT_UP_DOWN &= ~(1<<LED_UP);
		break;
		case 'D':
		OUTPORT_UP_DOWN &= ~(1<<LED_DOWN);
		break;
		case 'R':
		OUTPORT_RIGHT &= ~(1<<LED_RIGHT);
		break;
	}
}

void clear_all_led() {
	clear_led(LED_LEFT);
	clear_led(LED_UP);
	clear_led(LED_DOWN);
	clear_led(LED_RIGHT);
}


int main(void)
{
	init();
	//Save Pattern erstellen
	pattern_save_t *pattern_save_ptr= pattern_save_create_new();
	
	while (1)
	{
		while (game_state == IDLE)
		{
			//Linke und rechte LED an, obere und untere LED aus
			set_led(LED_LEFT);
			set_led(LED_RIGHT);
			clear_led(LED_UP);
			clear_led(LED_DOWN);
		}
		while (game_state == GAME_MODE){
			//Alle LEDs aus
			clear_all_led();
			//Zufaellige neue LED zum Pattern zufuegen
			uint8_t random = TCNT1%4;
			pattern_save_save_new_pattern(pattern_save_ptr,random);
			while(pattern_save_has_next(pattern_save_ptr)){
				uint8_t pattern = pattern_save_get_next(pattern_save_ptr);
				cli();
				TCNT0=0x00;
				sei();
				LightOn=1;
				while(LightOn){
					switch(pattern){
						case UP_PATTERN:
						//OUTPORT_UP_DOWN |= (1<<LED_UP);
						set_led(LED_UP);
						break;
						case DOWN_PATTERN:
						//OUTPORT_UP_DOWN |= (1<<LED_DOWN);
						set_led(LED_DOWN);
						break;
						case LEFT_PATTERN:
						//OUTPORT_LEFT |= (1<<LED_LEFT);
						set_led(LED_LEFT);
						break;
						case RIGHT_PATTERN:
						//OUTPORT_RIGHT |= (1<<LED_RIGHT);
						set_led(LED_RIGHT);
						break;
					}
				}
			}
			pattern_save_set_iterator_begin(pattern_save_ptr);
			while(pattern_save_has_next(pattern_save_ptr)){
				nextButton = pattern_save_get_next(pattern_save_ptr);
				// Alle anzeigen
				// Daten durchgehen{
				nextButtonPressed = 0;
				// nextButton = Datenstruktur[i]
				// if (!game_state == GAME_MODE)
				//}
				while (nextButtonPressed == 0){
					// Nur für den Simulator
					clear_all_led();
				}
			}
		}
		clear_all_led();
		while (game_state == END_SCREEN){
			//Linke und rechte LED aus, obere und untere LED an
			clear_all_led();
			set_led(LED_UP);
			set_led(LED_DOWN);
		}
	}
	return 0; 
}