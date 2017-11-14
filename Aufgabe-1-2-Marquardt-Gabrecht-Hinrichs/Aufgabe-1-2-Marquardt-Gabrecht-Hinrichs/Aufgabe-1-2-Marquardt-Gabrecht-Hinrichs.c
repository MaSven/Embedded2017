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
#define OUTPORT1 PORTA
#define OUTPORT2 PORTB
#define OUTPORT3 PORTC
#define OUTDDR1 DDRA
#define OUTDDR2 DDRB
#define OUTDDR3 DDRC
#define GAME_MODE 1
#define IDLE 0
#define WIN_SCREEN 2



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

//Speichert den momentanen State des Spieles
uint8_t game_state =0;

//Schaltet die gegebene LED an und alle anderen LED aus
// led_number: Nummer der LED
// on: LED an oder aus
void set_led(uint8_t led_number,uint8_t on);

//Programm wird wieder in den IDLE zustand gestellt
void reset();
//Programm wird in der Sieger zustand gestellt
void winner();
//Starte das Spiel mit zwei spielern
void start_game();



/************************************************************************/
/* Interrupt Service Routinen für Timer1								*/
/************************************************************************/

/* Compare Interrupt A													*/
/* schaltet die LED ein													*/
/************************************************************************/
ISR (TIMER1_COMPA_vect) {
	//BLINKPORTZWEI ^= 0x01;
	//BLINKPORTEINS ^= 0x01;
}
//TODO Dioku interrupts nicht rechenn kurzer zugriff auf OCR1A nicht mit dem register rechen
void timer_1_update(int8_t direction){
	
	if((OCR1A<=UINT16_MAX) && (direction>0)){
		//OCR1A += ;	
	}
	//else if((OCR1A>=) && (direction<0)){
		//OCR1A -= ; 
	//}
	TCNT1 = 0x00;		// Zählregister des Timers noch auf Null stellen
	
	
	
}

//Timer Interrupt fuer Cancel
//Soll nach 5 sekunden ausloesen
ISR (TIMER1_COMPB_vect){
	
}
//External Interrupt ausgeloest
ISR (PCINT0_vect){
	uint8_t button = PINA;
	if(button == (1<<CANCEL)){
		//reset();
	}
	if(game_state==0){
		if(button == (1<<ENTER)){
					
		}
	}else if(game_state==1){
		
	}
}

void external_interrupt_init(){
	PCMSK0 |= ((1<<UP)|(1<<DOWN)|(1<<LEFT)|(1<<RIGHT)|(1<<ENTER)|(1<<CANCEL));
	EICRA |= ((1<<ISC01)|(1<<ISC00));
}






int main(void)
{
	//INIT
	OUTDDR1 |= (1<<LED_LEFT);
	OUTDDR2 |= ((1<<LED_UP)|(1<<LED_DOWN));
	OUTDDR3 |= (1<<LED_RIGHT);
	OUTPORT1 &= ~(1<<LED_LEFT);
	OUTPORT2 &= ~((1<<LED_UP)|(LED_DOWN));
	OUTPORT3 &= ~(1<<LED_RIGHT);
	
	cli();
	timer1Init();
	sei();
	return 0; 
}



