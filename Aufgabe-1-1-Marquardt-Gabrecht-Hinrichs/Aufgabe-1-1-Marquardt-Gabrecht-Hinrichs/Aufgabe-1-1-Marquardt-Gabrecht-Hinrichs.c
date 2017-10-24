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


#define BLINKPORT PORTB
#define BLINKDDR DDRB
#define BLINKBIT 0
#define EINGABEPIN PIND
#define UP 6
#define DOWN 7


/************************************************************************/
/* Initialisieren von Timer1.                                           */
/************************************************************************/
void timer1Init (uint8_t direction) {
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
	OCR1BL = 0xC3;		// 194 -> 0x00C3
	OCR1A = 977;		// 1.000.000 / 1024 = 977 für eine Sekunde
	// Nun die relevanten Interrupts aktivieren: Timer Interrupt Mask
	TIMSK1 = (1<<1);	// Bit 1 ? OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	TIMSK1 |= (1<<2);	// Bit 2 ? OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Enable
	
	TCNT1 = 0x00;		// Zählregister des Timers noch auf Null stellen
}


/************************************************************************/
/* Interrupt Service Routinen für Timer1								*/
/************************************************************************/

/* Compare Interrupt A													*/
/* schaltet die LED ein													*/
/************************************************************************/
ISR (TIMER1_COMPA_vect) {
	BLINKPORT |= (1<<BLINKBIT);
}

/* Compare Interrupt b													*/
/* schaltet die (wieder) aus											*/
/************************************************************************/
ISR (TIMER1_COMPB_vect) {
	BLINKPORT &= ~(1<<BLINKBIT);
}


int main(void)
{
	// init
   	BLINKDDR  = 0x0F;		// set lower nibble of PORTA to output
    BLINKPORT = 0x00;		// all LEDs off
	unsigned char abort=0; 
	volatile unsigned char i=0x2A;
	int state=0;
	cli();
	timer1Init(0);
	sei();
	
		
    do {
		// Put a BREAKPOINT at the line below this comment
		
#ifdef SIMULATOR
		for (loop=0; loop<0x4; loop++)
			i=loop%3; 	
#else
		
#endif
		if (EINGABEPIN == (1<<UP))
		{
			
			for (int i = 0; i < WAIT; i++);
			do {
			} while (EINGABEPIN != (1<<UP));
			cli();
			timer1Init(1);
			sei();
		}
		abort=(PINA & 0x10);	// check for Button "ENTER"
	} while (!abort);
		BLINKPORT = 0x00;  			// all LEDs off
	// Put a BREAKPOINT at the line below this comment
	return 0; 
}