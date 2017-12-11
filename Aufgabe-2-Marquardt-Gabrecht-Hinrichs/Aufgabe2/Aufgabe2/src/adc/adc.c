/*
 * adc.c
 *
 * Created: 11.12.2017 22:10:42
 *  Author: Matthias Hinrichs
 */ 
#include "adc.h"

void adc_init(void)
{
	// VCC als Referenzspannung
	// Ausrichtung linksbuendig (nur ADCH auslesen ergibt 8-Bit)
	ADMUX = ((1<<REFS0)|(1<<ADLAR));
	// AD-Wandler einschalten
	// ADC-Frequenz sollte laut mikrocontroller.net zwischen 50kHz und 200kHz liegen
	// 1.000.000 /  50.000 = 20
	// 1.000.000 / 200.000 =  5
	// Prescaler: 8 => 125kHz ADC-Frequenz
	ADCSRA = ((1<<ADEN)|(1<<ADPS1)|(1<<ADPS0));
	// Einzelne Messung starten um moegliche Fehlerwerte auszuschliessen
	ADCSRA |= (1<<ADSC);
	// Auf Ende der Messung warten
	while (ADCSRA & (1<<ADSC))
	{
	}
	// Ergebnis verwerfen
	(void) ADCW;
}

uint8_t adc_read(uint8_t channel)
{
	// Bits 4:0 loeschen
	ADMUX &= ((1<<REFS1)|(1<<REFS0)|(1<<ADLAR));
	// Kanal einstellen (Welcher Pin)
	ADMUX |= channel;
	// AD-Wandlung starten
	ADCSRA |= (1<<ADSC);
	// Auf Ende der Wandlung warten
	while (ADCSRA & (1<<ADSC))
	{
	}
	// Nur ADCH zurueckgeben, um 8-Bit Wert zu erhalten. Bit 1:0 des 10-Bit Wertes werden verworfen
	return ADCH;
}