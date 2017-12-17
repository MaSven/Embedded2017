/*
 * one_wire.c
 *
 * Created: 11.12.2017 22:11:02
 *  Author: Matthias Hinrichs
 */ 

#include "one_wire/one_wire.h"
#include <avr/io.h>
#include  <avr/cpufunc.h>
#include <util/delay.h>
#define TEMPERATURE_REGISTER DDRB
#define TEMPERATURE_PORT PORTB

void one_wire_deactivate(){
	TEMPERATURE_REGISTER = (1<<DDB2);
	TEMPERATURE_PORT = (0<<PORTB2);
	_NOP();
}


void reset(){
	TEMPERATURE_PORT = (1<<PORTB2);
	TEMPERATURE_PORT = (0<<PORTB2);
	_delay_us(480);
	TEMPERATURE_PORT = (1<<PORTB2);
	_delay_us(70);
	uint8_t read_slave_count = PORTB;
}
