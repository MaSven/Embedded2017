/*
 * one_wire.c
 *
 * Created: 11.12.2017 22:11:02
 *  Author: Matthias Hinrichs
 */ 


#include <avr/io.h>
#include  <avr/cpufunc.h>
#include <util/delay.h>
#include "one_wire/one_wire.h"
#include "global.h"
#define TEMPERATURE_REGISTER TEMPDDR
#define TEMPERATURE_PORT TEMPPORT
/**
    Setze den Temperaturport auf low    
*/
inline void set_temperature_port_off(){
	TEMPERATURE_PORT = (0<<PORTB2);
}
/**
    Setze den Temperaturport auf high
*/
inline void set_temperature_port_on(){
	TEMPERATURE_PORT = (1<<PORTB2);
}

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



