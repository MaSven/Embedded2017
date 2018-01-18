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
#include <stdio.h>
#include "one_wire/one_wire.h"
#include "global.h"
#include "one_wire/ds18s20.h"
//#ifdef DEBUG
#include "lcd/lcd.h"
//#endif
/*
*   Scratchpad ist 9 byte gross im DS18S20
*/
#define SCRATCHPAD_SIZE_IN_BYTE 9
#define SCRATCHPAD_LS_BYTE 0
#define SCRATCHPAD_MS_BYTE 1
#define SCRATCHPAD_CRC_BYTE 8
#define SCRATCHPAD_COUNT_REMAIN_BYTE 6
#define SCRATCHPAD_COUNT_PER_C_BYTE 7
#define SIZE_OF_UINT8 sizeof(uint8_t)*8
/*
	16*0.25 = 4
*/
#define TEMPERATURE_CONSTANT 4

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
int8_t ls_and_ms_to_temperature(uint8_t ls_byte,uint8_t ms_byte);

int16_t ds18s20_read_temperature(){
	uint8_t scratchpad_data[SCRATCHPAD_SIZE_IN_BYTE]={0};
	if(one_wire_reset()){
		one_wire_write_byte(ROM_COMMAND_SKIP_ROM);
		one_wire_write_byte(FUNCTION_COMMAND_CONVERT_T);
		//Warte bis die temperatur in digitalform im scratchpad ist
		while(!one_wire_read_bit());
		// Reset vor jedem Command
		one_wire_reset();
		one_wire_write_byte(ROM_COMMAND_SKIP_ROM);
		one_wire_write_byte(FUNCTION_COMMAND_READ_SCRATCHPAD);
		//Lesen des scratchpads beinhaltet 9 bytes im ds18s20
		for(int i=0;i<SCRATCHPAD_SIZE_IN_BYTE;i++){
			scratchpad_data[i]=one_wire_read_byte();
		}
		/*// Test scratchpad leer?
		char scratchpad_string[3]={0};
		itoa(scratchpad_data[0], scratchpad_string, 10);
		lcd_set_cursor(2,0);
		lcd_send_string(scratchpad_string);
		itoa(scratchpad_data[1], scratchpad_string, 10);
		lcd_set_cursor(2,4);
		lcd_send_string(scratchpad_string);*/
		/*#ifdef DEBUG
		for(uint8_t i=0;i<SCRATCHPAD_SIZE_IN_BYTE;i++){
			//lcd_clear();
			lcd_set_cursor(1,5);
			lcd_send_string("Test");
			char int_string[4]={0};
			itoa(scratchpad_data[i],int_string,10);
			lcd_set_cursor(2,0);
			lcd_send_string(int_string);
			_delay_ms(500);
		}
		#endif*/
		if(check_scratchpad_data(scratchpad_data,SCRATCHPAD_SIZE_IN_BYTE,scratchpad_data[SCRATCHPAD_CRC_BYTE])){
			uint8_t count_remain = scratchpad_data[SCRATCHPAD_COUNT_REMAIN_BYTE];
			uint8_t count_per_c = scratchpad_data[SCRATCHPAD_COUNT_PER_C_BYTE];
			uint8_t ls_byte = scratchpad_data[SCRATCHPAD_LS_BYTE];
			uint8_t ms_byte = scratchpad_data[SCRATCHPAD_MS_BYTE];
			int16_t temperature = ((int16_t)ms_byte<<8|ls_byte);
			temperature &= ~1;
			temperature <<=3;
			/*#ifdef DEBUG
			char int_string[4]={0};
			//itoa(((temperature - TEMPERATURE_CONSTANT)+(((count_per_c-count_remain)/count_per_c)*INTEGER_OFFSET)),int_string,10);
			itoa(ms_byte,int_string,10);
			lcd_set_cursor(2,6);
			lcd_send_string(int_string);
			itoa(ls_byte,int_string,10);
			lcd_set_cursor(2,10);
			lcd_send_string(int_string);
			#endif*/
			temperature=(temperature - TEMPERATURE_CONSTANT)+(((count_per_c-count_remain)));
			return ((temperature * 10)>> 4)*10;
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

int8_t ls_and_ms_to_temperature(const uint8_t ls_byte,const uint8_t ms_byte){
	int8_t temperature=0;
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
			if(i!=0){
				//bit 0 ignorieren um eine hoehere aufloesung zu erreichen
				temperature += (pow((2*sign),i-1));
			}
		}

	}
	return temperature;

}

char * ds18s20_temperature_as_string(int16_t temperature,char *temperature_string){
	char temp_as_string[STRING_CPACITY];
	itoa(temperature,temp_as_string,10);
	uint8_t counter =0;
	uint8_t length_of_number_before_com = 1;
	if(temperature<0){
		length_of_number_before_com++;
	}
	if(temperature>9999||temperature<(-9999)){
		length_of_number_before_com++;
	}
	while(counter<STRING_CPACITY){
		if(counter==length_of_number_before_com){
			//Setze das nach dem die vorkomma zahlen behandelt wurden
			temperature_string[counter++]=',';
			//Es gibt nur 2 nachkomma stellen
			temperature_string[counter]=temp_as_string[counter-1];
			counter++;
			//Alle Zahlen eingetragen nun die zeichen fuer Temperatur
			temperature_string[counter++]=0xB2;
			temperature_string[counter++]='C';
			temperature_string[counter]='\0';
			break;
			}else if(counter<length_of_number_before_com){
			temperature_string[counter]=temp_as_string[counter];
		}
		counter++;
	}
	return temperature_string;



}

void temp_display(int row, int col){
	int16_t temperature = ds18s20_read_temperature();
	char temp_string[STRING_CPACITY];
	ds18s20_temperature_as_string(temperature,temp_string);
	lcd_set_cursor(row,col);
	lcd_send_string(temp_string);
}
