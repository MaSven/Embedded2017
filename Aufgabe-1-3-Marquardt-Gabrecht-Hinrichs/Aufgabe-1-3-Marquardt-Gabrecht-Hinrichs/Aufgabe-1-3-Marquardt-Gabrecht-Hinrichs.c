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
#define OUTDDR DDRA
#define OUTPORT PORTA
#define PADDDR DDRC
#define PADPORT PORTC
#define PADPIN PINC
// Prototypes
void init (void);
void timer1Init (void);
void clearLeds (void);
uint8_t readKeypad (void);
void showButtonValue (uint8_t);


int main(void)
{
    while(1)
    {
        //Auslesen welche Buttons gedrueckt sind den Wert in Variable addieren und der Anzeigefunktion uebergeben
		uint8_t sum = readKeypad();
		if (sum != 0)
		{
			cli();
			TCNT1 = 0x00;
			sei();
			showButtonValue(sum);
		}
    }
}

/************************************************************************/
/* Initialisierung                                                      */
/************************************************************************/
void init() {
	OUTDDR = 0xFF; //Kompletter Port als Output
	PADDDR = 0xF0; // Pins 0-3 als Input, 4-7 als Output
	
	
	clearLeds(); //Alle LEDs aus
}

/************************************************************************/
/* Initialisieren von Timer0                                            */
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
	clearLeds();
}

/************************************************************************/
/* Clear LEDs                                                           */
/************************************************************************/
clearLeds() {
	OUTPORT = 0x00;
}

/************************************************************************/
/* Keypad auslesen                                                      */
/************************************************************************/
uint8_t readKeypad() {
	uint8_t sum = 0;
	uint8_t buttons;
	//PC7-4 nacheinander auf 1 schalten
	//PC3-0 auf 1 pruefen
	//bei 1 Taste gedrueckt, bei 0 Taste nicht gedrueckt
	
	PADPORT |= (1<<PC4); // Unterstes Bit im oberen Nibble setzen
	buttons = (PADPIN & ~(0xF0)); // Einlesen des unteren Nibbles
	
	if ((buttons & (1<<PC0)) == 1)
	{
		//Button 1 gedrueckt
		sum += 1;
	}
	if ((buttons & (1<<PC1)) == 1)
	{
		//Button 2 gedrueckt
		sum += 2;
	}
	if ((buttons & (1<<PC2)) == 1)
	{
		//Button 3 gedrueckt
		sum += 3;
	}
	if ((buttons & (1<<PC3)) == 1)
	{
		//Button A gedrueckt
		sum += 0x0A;
	}
	
	PADPORT &= ~(1<<PC4);
	PADPORT |= (1<<PC5);
	
	if ((buttons & (1<<PC0)) == 1)
	{
		//Button 4 gedrueckt
		sum += 4;
	}
	if ((buttons & (1<<PC1)) == 1)
	{
		//Button 5 gedrueckt
		sum += 5;
	}
	if ((buttons & (1<<PC2)) == 1)
	{
		//Button 6 gedrueckt
		sum += 6;
	}
	if ((buttons & (1<<PC3)) == 1)
	{
		//Button B gedrueckt
		sum += 0x0B;
	}
	
	PADPORT &= ~(1<<PC5);
	PADPORT |= (1<<PC6);
	
	if ((buttons & (1<<PC0)) == 1)
	{
		//Button 7 gedrueckt
		sum += 7;
	}
	if ((buttons & (1<<PC1)) == 1)
	{
		//Button 8 gedrueckt
		sum += 8;
	}
	if ((buttons & (1<<PC2)) == 1)
	{
		//Button 9 gedrueckt
		sum += 9;
	}
	if ((buttons & (1<<PC3)) == 1)
	{
		//Button C gedrueckt
		sum += 0x0C;
	}
	
	PADPORT &= ~(1<<PC6);
	PADPORT |= (1<<PC7);
	
	if ((buttons & (1<<PC0)) == 1)
	{
		//Button E gedrueckt
		sum += 0x0E;
	}
	if ((buttons & (1<<PC1)) == 1)
	{
		//Button 10 gedrueckt
		sum += 10;
	}
	if ((buttons & (1<<PC2)) == 1)
	{
		//Button F gedrueckt
		sum += 0x0F;
	}
	if ((buttons & (1<<PC3)) == 1)
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
showButtonValue(uint8_t sum) {
	OUTPORT = sum;
}