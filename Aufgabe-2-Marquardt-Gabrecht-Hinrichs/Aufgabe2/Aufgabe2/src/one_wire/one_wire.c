/*
* one_wire.c
*
* Created: 11.12.2017 22:11:02
*  Author: Matthias Hinrichs
*/
/************************************************************************/
/* Step 1. Initialization
Step 2. ROM Command (followed by any required data exchange)
Step 3. DS18S20 Function Command (followed by any required data exchange)                                                                     */
/************************************************************************/

#include <avr/io.h>
#include  <avr/cpufunc.h>
#include "one_wire/one_wire.h"
#include "global.h"
/**
Setze den Temperaturport als eingang
*/
inline void set_port_input(){
	TEMPDDR &= ~(1 << TEMPPIN);
}
/**
Setze den Temperaturport aus ausgang
*/
inline void set_port_output(){
	TEMPDDR |= (1<<TEMPPIN);
}
/**
Setze den Temperaturport auf low
*/
inline void set_temperature_port_off(){
	TEMPPORT &= ~(1<<TEMPPIN);
	
}

/**
Setze den Temperaturport auf high
*/
inline void set_temperature_port_on(){
	TEMPPORT |= (1<<TEMPPIN);
}

inline uint8_t read_bit_of_port(){
	set_port_input();
	set_temperature_port_on();
	uint8_t bit = (TEMPPININPUT & (1<<TEMPPIN));
	return bit;
}



/**
TEMPPORT eine 1 an alle slaves
*/
inline void send_high(){
	TEMPPORT |= (1<<TEMPPIN);
}
/**
Sende eine 0 an alle slaves
*/
inline void send_low(){
	TEMPPORT &= ~(1<<TEMPPIN);
}

void one_wire_init(){
	set_port_output();
	set_temperature_port_on();
}


uint8_t reset(){
	uint8_t wait_for_bit=0;
	uint8_t read_slave_bit=0;
	set_port_output();
	set_temperature_port_on();
	_delay_us(ONEWIRE_DELAY_G_US);
	set_temperature_port_off();
	_delay_us(ONEWIRE_DELAY_H_US);
	set_port_input();
	set_temperature_port_on();
	while(wait_for_bit<=ONEWIRE_DELAY_I_US){
		_delay_us(30);
		//Wenn eine 1 gelesen wurde, muss nicht mehr eingelesen werden. Onewire wurde erkannt
		if(!read_slave_bit){
			read_slave_count = (TEMPPININPUT & (1<<TEMPPIN))
		}
		wait_for_bit+=30;
	}
	_delay_us(ONEWIRE_DELAY_J_US);
	return read_slave_count;
}

void write_one_bit(uint8_t bit){
	if(bit){
		set_temperature_port_off();
		_delay_us(ONEWIRE_DELAY_A_US);
		set_temperature_port_on();
		_delay_us(ONEWIRE_DELAY_B_US);
		
		}else{
		set_temperature_port_off();
		_delay_us(ONEWIRE_DELAY_C_US);
		set_temperature_port_on();
		_delay_us(ONEWIRE_DELAY_D_US);
	}
}

uint8_t read_one_bit(){
	uint8_t result;
	set_temperature_port_off();
	_delay_us(ONEWIRE_DELAY_A_US);
	set_temperature_port_on();
	_delay_us(ONEWIRE_DELAY_E_US);
	result = read_bit_of_port();
	_delay_us(ONEWIRE_DELAY_F_US);
	return result;
}

uint8_t read_byte(){
	uint8_t result=0;
	for(uint8_t i = 0;i<8;i++){
		result >>=1;
		if(read_one_bit()){
			result |= 0x80;
		}
	}
	return result;
}



void write_byte(uint8_t byte){
	for(uint8_t i=0;i<8;i++){
		write_one_bit(byte & 0x01);
		byte >>=1;
	}
}

uint8_t write_and_result(uint8_t byte){
	uint8_t result=0;
	for(uint8_t i=0;i<8;i++){
		result >>=1;
		if(byte & 0x01){
			if(read_one_bit()){
				result |= 0x80;
			}
			}else{
			write_one_bit(0);
		}
	}
	return result;
}

void write_byte_block(uint8_t *data,uint8_t data_len){
	for(uint8_t i=0;i<data_len;i++){
		data[i]= write_and_result(data[i]);
	}
}



uint8_t one_wire_reset(){
	return reset();
}

void one_wire_write_byte(uint8_t data){
	write_byte(data);
}

uint8_t one_wire_read_write_byte(uint8_t data){
	return write_and_result(data);
}

uint8_t one_wire_read_bit(){
	return read_one_bit();
}

uint8_t one_wire_read_byte(){
	return read_byte();
}