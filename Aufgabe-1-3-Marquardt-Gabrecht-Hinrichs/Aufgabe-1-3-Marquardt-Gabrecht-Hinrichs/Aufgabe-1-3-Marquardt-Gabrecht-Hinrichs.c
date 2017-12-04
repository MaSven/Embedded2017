/*
 * Aufgabe_1_3_Marquardt_Gabrecht_Hinrichs.c
 *
 * Created: 28.11.2017 08:22:32
 *  Author: Matthias Hinrichs
 */ 

// Includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

// Defines
#define OUTDDR DDRD
#define OUTPORT PORTD
#define PADDDR DDRC
#define PADPORT PORTC
#define PADPIN PINC
#define COL0 0b00000001
#define COL1 0b00000010
#define COL2 0b00000100
#define COL3 0b00001000
// Prototypes
void init (void);
void timer1Init (void);
void clearLeds (void);
uint8_t readKeypad (void);
void showButtonValue (uint8_t);
// Globals
uint8_t volatile reset = 0;

int main(void)
{
	
	init();
	
    while(1)
    {
        //Auslesen welche Buttons gedrueckt sind den Wert in Variable addieren und der Anzeigefunktion uebergeben
		uint8_t volatile sum = readKeypad();
		if (sum != 0)
		{
			showButtonValue(sum);
		}
		else
		{
			cli();
			TCNT1 = 0x00;
			sei();
			reset = 1;
		}
    }
}

/************************************************************************/
/* Initialisierung                                                      */
/************************************************************************/
void init() {
	OUTDDR = 0xFF; //Kompletter Port als Output
	PADDDR = 0xF0; // Pins 0-3 als Input, 4-7 als Output
	cli();
	timer1Init();
	sei();	
	clearLeds(); //Alle LEDs aus
}

/************************************************************************/
/* Initialisieren von Timer1                                            */
/************************************************************************/
void timer1Init() {
	TCCR1A = 0x00;
	TCCR1B |= ((1<<WGM12) | (1<<CS12) | (1<<CS10));	// CTC ON, Prescaler 1024
	//Timer 0,5s
	//0,5 * 1.000.000 / 1024 = 488
	OCR1A = 488;
	//Interrupt aktivieren
	TIMSK1 |= (1<<OCIE1A);
	//Zaehlregister auf 0 stellen
	TCNT1 = 0x00;
}

/************************************************************************/
/* Interrupt Service Routine für Timer0 Compare Interrupt A             */
/************************************************************************/
ISR (TIMER1_COMPA_vect) {
	//0,5s nach Loslassen der (letzten) Taste die Anzeige loeschen
	if (reset)
	{
		clearLeds();
		reset = 0;
	}
}

/************************************************************************/
/* Clear LEDs                                                           */
/************************************************************************/
void clearLeds() {
	OUTPORT = 0x00;
}

/************************************************************************/
/* Keypad auslesen                                                      */
/************************************************************************/
uint8_t readKeypad() {
	uint8_t volatile sum = 0;
	//PC7-4 nacheinander auf 1 schalten
	//PC3-0 auf 1 pruefen
	//bei 1 Taste gedrueckt, bei 0 Taste nicht gedrueckt
	
	PADPORT |= (1<<PC4); // Unterstes Bit im oberen Nibble setzen 
	uint8_t volatile buttons = PADPIN; // Einlesen der gedrueckten Buttons
	if ((buttons & (1<<PC0)) == COL0)
	{
		//Button 1 gedrueckt
		sum += 1;
	}
	if ((buttons & (1<<PC1)) == COL1)
	{
		//Button 2 gedrueckt
		sum += 2;
	}
	if ((buttons & (1<<PC2)) == COL2)
	{
		//Button 3 gedrueckt
		sum += 3;
	}
	if ((buttons & (1<<PC3)) == COL3)
	{
		//Button A gedrueckt
		sum += 0x0A;
	}
	
	PADPORT &= ~(1<<PC4);
	PADPORT |= (1<<PC5);
	buttons = PADPIN;
	
	if ((buttons & (1<<PC0)) == COL0)
	{
		//Button 4 gedrueckt
		sum += 4;
	}
	if ((buttons & (1<<PC1)) == COL1)
	{
		//Button 5 gedrueckt
		sum += 5;
	}
	if ((buttons & (1<<PC2)) == COL2)
	{
		//Button 6 gedrueckt
		sum += 6;
	}
	if ((buttons & (1<<PC3)) == COL3)
	{
		//Button B gedrueckt
		sum += 0x0B;
	}
	
	PADPORT &= ~(1<<PC5);
	PADPORT |= (1<<PC6);
	buttons = PADPIN;
	
	if ((buttons & (1<<PC0)) == COL0)
	{
		//Button 7 gedrueckt
		sum += 7;
	}
	if ((buttons & (1<<PC1)) == COL1)
	{
		//Button 8 gedrueckt
		sum += 8;
	}
	if ((buttons & (1<<PC2)) == COL2)
	{
		//Button 9 gedrueckt
		sum += 9;
	}
	if ((buttons & (1<<PC3)) == COL3)
	{
		//Button C gedrueckt
		sum += 0x0C;
	}
	
	PADPORT &= ~(1<<PC6);
	PADPORT |= (1<<PC7);
	buttons = PADPIN;
	
	if ((buttons & (1<<PC0)) == COL0)
	{
		//Button E gedrueckt
		sum += 0x0E;
	}
	if ((buttons & (1<<PC1)) == COL1)
	{
		//Button 10 gedrueckt
		sum += 10;
	}
	if ((buttons & (1<<PC2)) == COL2)
	{
		//Button F gedrueckt
		sum += 0x0F;
	}
	if ((buttons & (1<<PC3)) == COL3)
	{
		//Button D gedrueckt
		sum += 0x0D;
	}
	
	PADPORT &= ~(1<<PC7); // Oberstes Bit loeschen
	return sum;
}

/************************************************************************/
/* Anzeigen der Summe                                                   */
/************************************************************************/
void showButtonValue(uint8_t sum) {
	OUTPORT = sum;
}