

#include <avr/io.h>
#include <avr/interrupt.h>		// interrupts
#include <inttypes.h>			// Datentypen mit Bitbreiten
#include "pattern_save.h"




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
	UP_PATTERN=0x01,DOWN_PATTERN=0x08,LEFT_PATTERN=0x02,RIGHT_PATTERN=0x04
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
volatile uint8_t Lastbutton =0;
uint8_t volatile LightOn=0;
uint8_t lightCounter=0;

uint8_t volatile game_lost=0;

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
	if(GAME_MODE==game_state){
		game_state=END_SCREEN;
		game_lost=1;
	}
}

ISR (TIMER0_COMPA_vect) {
	IOInterruptEnabled = 1;
	if(lightCounter==5){
		LightOn = 0;
		lightCounter=0;
	}
	lightCounter++;
}

//External Interrupt ausgeloest
ISR (PCINT0_vect){
	volatile uint8_t button = PINA & ((1<<UP)|(1<<DOWN)|(1<<LEFT)|(1<<RIGHT)|(1<<ENTER)|(1<<CANCEL));
	if(!button){
		if(IOInterruptEnabled) {
			IOInterruptEnabled = 0;
			nextButtonPressed = 1;
			if(Lastbutton == (1<<CANCEL)){
				game_state = IDLE;
				game_lost=1;
			}
			if(game_state==IDLE){
				if(Lastbutton == (1<<ENTER)){
					game_state = GAME_MODE;
				}
				}else if(game_state==GAME_MODE){
				if (nextButton != Lastbutton){
					game_state = END_SCREEN;
					game_lost=1;
				}
			}
		}
		} else {
		Lastbutton = button;
	}
}

void external_interrupt_init(){
	PCMSK0 |= ((1<<UP)|(1<<DOWN)|(1<<LEFT)|(1<<RIGHT)|(1<<ENTER)|(1<<CANCEL));
	PCICR |= (1<<PCIE0);
}

void init() {
	//DDRs setzen
	OUTDDR_LEFT |= (1<<LED_LEFT);
	OUTDDR_UP_DOWN |= ((1<<LED_UP)|(1<<LED_DOWN));
	OUTDDR_RIGHT |= (1<<LED_RIGHT);
	DDRD = 0xFF;
	//Alle LEDs abschalten
	clear_led('L');
	clear_led('U');
	clear_led('D');
	clear_led('R');
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
	clear_led('L');
	clear_led('U');
	clear_led('D');
	clear_led('R');
}

uint8_t is_game_lost(){
	return game_lost;
}


int main(void)
{
	init();
	//Save Pattern erstellen
	pattern_save_t *pattern_save_ptr= pattern_save_create_new();
	
	while (1)
	{
		//Zurücksetzen der werte
		pattern_save_clean(pattern_save_ptr);
		game_lost=0;
		PORTD = 0x00;
		while (game_state == IDLE)
		{
			//Linke und rechte LED an, obere und untere LED aus
			set_led('L');
			set_led('R');
			clear_led('U');
			clear_led('D');
		}
		while (game_state == GAME_MODE){
			//Alle LEDs aus
			clear_all_led();
			//Zufaellige neue LED zum Pattern zufuegen
			uint8_t random = TCNT1%4;
			pattern_save_save_new_pattern(pattern_save_ptr,random);
			pattern_save_set_iterator_begin(pattern_save_ptr);
			while(pattern_save_has_next(pattern_save_ptr)){
				uint8_t pattern = pattern_save_get_next(pattern_save_ptr);
				cli();
				TCNT0=0x00;
				TCNT1=0x00;
				sei();
				LightOn=1;
				while(LightOn){
					clear_all_led();
				}
				LightOn=1;
				while(LightOn){
					switch(pattern){
						case UP_PATTERN:
						
						set_led('U');
						break;
						case DOWN_PATTERN:
						
						set_led('D');
						break;
						case LEFT_PATTERN:
						
						set_led('L');
						break;
						case RIGHT_PATTERN:
						
						set_led('R');
						break;
					}
				}
			}
			pattern_save_set_iterator_begin(pattern_save_ptr);
			while(pattern_save_has_next(pattern_save_ptr)&& !game_lost){
				TCNT1=0x00;
				nextButton = pattern_save_get_next(pattern_save_ptr);
				// Alle anzeigen
				// Daten durchgehen{
				nextButtonPressed = 0;
				while (nextButtonPressed == 0 && !game_lost){
					clear_all_led();
				}
			}
			for(int i=0;i<2;i++){
				LightOn=1;
				while(LightOn&&!game_lost){
					//Linke und rechte LED aus, obere und untere LED an
					clear_all_led();
					set_led('U');
					set_led('D');
				}
			}
		}
		clear_all_led();
		PORTD = pattern_save_get_size(pattern_save_ptr)-1;
		while (game_state == END_SCREEN){
			clear_all_led();
			set_led('L');
			set_led('U');
			set_led('D');
			set_led('R');
			
		}
		
	}
	return 0;
}

