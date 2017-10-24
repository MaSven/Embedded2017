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


#define BLINKPORTEINS PORTB 
#define BLINKPORTZWEI PORTA
#define BLINKDDRA DDRA
#define BLINKDDRB DDRB
#define BLINKBIT 0
#define EINGABEPIN PIND
#define EINGABEPORT DDRD
#define UP 6
#define DOWN 7
#define STEP 100
#define INITTIMERCOMPARE 977
#define WAIT 250

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
	OCR1A= INITTIMERCOMPARE;
	// 1.000.000 / 1024 = 977 für eine Sekunde
	// Nun die relevanten Interrupts aktivieren: Timer Interrupt Mask
	TIMSK1 = (1<<1);	// Bit 1 ? OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	//TIMSK1 |= (1<<2);	// Bit 2 ? OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Enable
	
	TCNT1 = 0x00;		// Zählregister des Timers noch auf Null stellen
}


/************************************************************************/
/* Interrupt Service Routinen für Timer1								*/
/************************************************************************/

/* Compare Interrupt A													*/
/* schaltet die LED ein													*/
/************************************************************************/
ISR (TIMER1_COMPA_vect) {
	BLINKPORTZWEI ^= 0x01;
	BLINKPORTEINS ^= 0x01;
}
//TODO Dioku interrupts nicht rechenn kurzer zugriff auf OCR1A nicht mit dem register rechen
void timer_1_update(int8_t direction){
	
	if((OCR1A<=UINT16_MAX-STEP) && (direction>0)){
		OCR1A += STEP;	
	}
	else if((OCR1A>=0+STEP) && (direction<0){
		OCR1A -= STEP; 
	}
	TCNT1 = 0x00;		// Zählregister des Timers noch auf Null stellen
	
	
	
}


/* Compare Interrupt b													*/
/* schaltet die (wieder) aus											*/
/************************************************************************/
//ISR (TIMER1_COMPB_vect) {
//	BLINKPORTEINS &= ~(1<<BLINKBIT);
//	BLINKPORTZWEI |= (1<<BLINKBIT);
//}


int main(void)
{
	// init
   	BLINKDDRB  = 0x01;		// set lower nibble of PORTA to output
	BLINKDDRA = 0x01;
    BLINKPORTEINS = 0x01;		// all LEDs off
	BLINKPORTZWEI = 0x00;
	EINGABEPORT |= UP|DOWN;
	unsigned char abort=0; 
	volatile unsigned char i=0x2A;
	int state=0;
	cli();
	timer1Init();
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
			//Warten auf Taster 
			do {
				
			} while (EINGABEPIN == (1<<UP));
			
			cli();
			timer_1_update(1);
			sei();
		}
		if(EINGABEPIN == (1<<DOWN)){
			for (int i = 0; i < WAIT; i++);
			do {
			} while (EINGABEPIN == (1<<DOWN));
			cli();
			timer_1_update(-1);
			sei();
		}
		abort=(PINA & 0x10);	// check for Button "ENTER"
	} while (!abort);
		BLINKPORTEINS = 0x00;  			// all LEDs off
		BLINKPORTZWEI = 0x00;  			// all LEDs off
	// Put a BREAKPOINT at the line below this comment
	return 0; 
}