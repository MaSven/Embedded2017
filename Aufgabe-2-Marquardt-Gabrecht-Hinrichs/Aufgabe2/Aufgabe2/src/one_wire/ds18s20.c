/*
* ds18s20.c
*
* Created: 07.01.2018 16:57:44
*  Author: Sven Marquardt
*/

#include <util/crc16.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "one_wire/one_wire.h"
#include "global.h"
#include "one_wire/ds18s20.h"
#ifdef DEBUG
#include "lcd/lcd.h"
#endif
/*
*   Scratchpad ist 9 byte gross im DS18S20
*/
#define SCRATCHPAD_SIZE_IN_BYTE 9
#define SCRATCHPAD_LS_BYTE 0
#define SCRATCHPAD_MS_BYTE 1
#define SCRATCHPAD_CRC_BYTE 8
#define SCRATCHPAD_COUNT_REMAIN_BYTE 6
#define SCRATCHPAD_COUNT_PER_C_BYTE 7
#define SIZE_OF_UINT8 sizeof(uint8_t)
#define TEMPERATURE_CONSTANT 25

/*
*   Offset um float berechnung in einem int16_t speichern zu koennen
*   100 wurde gewaehlt, weil 2 nachkomma stellen reichen
*/
#define INTEGER_OFFSET 100
/*
*   Ueberprueft die daten auf richtigkeit
\return 0 wenn die daten falsch sind ansonsten 1
*/
uint8_t crc_check(uint8_t data,uint8_t crc);
/*
*   Ueberprueft das scratchpad array, ob alle daten richtig uebertragen wurden
*   \return 0 wenn eines der daten korrupt ist ansonsten 1
*/
uint8_t check_scratchpad_data(uint8_t * data,uint8_t length,uint8_t crc_byte);
/*
*   Ist das msbyte negativ
*   \return 1 wenn es eine negative nummer ist ansonsten 0
*/
static inline uint8_t is_temperature_negative(uint8_t ms_byte);
/*
*   Wandelt
*/
int16_t ls_and_ms_to_temperature(uint8_t ls_byte,uint8_t ms_byte);

int16_t ds18s20_read_temperature(){
	uint8_t scratchpad_data[SCRATCHPAD_SIZE_IN_BYTE]={0};
	if(one_wire_reset()){
		one_wire_write_byte(ROM_COMMAND_SKIP_ROM);
		one_wire_write_byte(FUNCTION_COMMAND_CONVERT_T);
		while(!one_wire_read_bit());
		one_wire_write_byte(FUNCTION_COMMAND_READ_SCRATCHPAD);
		//Lesen des scratchpads beinhaltet 9 bytes im ds18s20
		for(int i=0;i<SCRATCHPAD_SIZE_IN_BYTE;i++){
			scratchpad_data[i]=one_wire_read_byte();
		}
		#ifdef DEBUG
		for(uint8_t i=0;i<SCRATCHPAD_SIZE_IN_BYTE;i++){
			lcd_clear();
			char int_string[4]={0};
			itoa(scratchpad_data[i],int_string,10);
			_delay_ms(500);
		}
		#endif
		if(check_scratchpad_data(scratchpad_data,SCRATCHPAD_SIZE_IN_BYTE,scratchpad_data[SCRATCHPAD_CRC_BYTE])){
			uint8_t count_remain = scratchpad_data[SCRATCHPAD_COUNT_REMAIN_BYTE];
			uint8_t count_per_c = scratchpad_data[SCRATCHPAD_COUNT_PER_C_BYTE];
			uint8_t ls_byte = scratchpad_data[SCRATCHPAD_LS_BYTE];
			uint8_t ms_byte = scratchpad_data[SCRATCHPAD_MS_BYTE];
			int16_t temperature = ls_and_ms_to_temperature(ls_byte,ms_byte);
			#ifdef DEBUG
			char int_string[4]={0};
			itoa(((temperature - TEMPERATURE_CONSTANT)+(((count_per_c-count_remain)/count_per_c)*INTEGER_OFFSET)),int_string,10);
			#endif
			return (temperature - TEMPERATURE_CONSTANT)+(((count_per_c-count_remain)/count_per_c)*INTEGER_OFFSET);
		}
	}
	return 0;
}

uint8_t crc_check(uint8_t data,uint8_t crc){
	if(_crc_ibutton_update(crc,data)){
		return 1;
	}
	return 0;
	
}

uint8_t check_scratchpad_data(uint8_t* data,uint8_t length,uint8_t crc_byte){
	//-1 weil das letzte byte das crc byte nicht geprueft werden muss
	for(uint8_t i=0;i<length-1;i++){
		if(!crc_check(data[i],crc_byte)){
			return 0;
		}
	}
	return 1;
}

static inline uint8_t is_temperature_negative(uint8_t ms_byte){
	return ms_byte>0;
}

int16_t ls_and_ms_to_temperature(const uint8_t ls_byte,const uint8_t ms_byte){
	int16_t temperature=0;
	uint8_t check_byte=0;
	int8_t sign = 1;
	if(is_temperature_negative(ms_byte)){
		sign = -1;
	}
	for(uint8_t i=0;i<SIZE_OF_UINT8;i++){
		check_byte=0;
		//An der stelle die auf 1 geprueft werden soll 1 schreiben
		check_byte = (1<<i);
		//nur dei 1 kopieren die ueberprueft wird
		check_byte = (ls_byte & (check_byte));
		if(check_byte){
			if(i==0){
				temperature = 50*sign;
				}else{
				temperature += (pow((2*sign),i-1)*INTEGER_OFFSET);
			}
		}
	}
	return temperature;
	
}

char * ds18s20_temperature_as_string(int16_t temperature,char *temperature_string){
	char temp_as_string[STRING_CPACITY];
	itoa(temperature,temp_as_string,10);
	uint8_t counter =0;
	while(counter<STRING_CPACITY){
		if(counter==2){
			//Setze das komma nach der zweiten stelle
			temperature_string[counter++]=',';
			temperature_string[counter]=temp_as_string[counter];
			}else{
			temperature_string[counter]=temp_as_string[counter];
		}
		counter++;
	}
	return temperature_string;
	
	
	
}