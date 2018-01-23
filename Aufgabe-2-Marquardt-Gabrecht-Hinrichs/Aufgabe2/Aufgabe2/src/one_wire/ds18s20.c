/*
 * ds18s20.c
 *
 * Created: 07.01.2018 16:57:44
 *  Author: Sven Marquardt
 */
#include <util/crc16.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "one_wire/one_wire.h"
#include "global.h"
#include "one_wire/ds18s20.h"
#include "lcd/lcd.h"
/*
 *   Scratchpad ist 9 byte gross im DS18S20
 */
#define SCRATCHPAD_SIZE_IN_BYTE 9
/**
 * LS_Byte index im Scratchpad
 */
#define SCRATCHPAD_LS_BYTE 0
/**
 * MS_Byte index im Scratchpad
 */
#define SCRATCHPAD_MS_BYTE 1
/**
 * CRC_Byte index im Scratchpad
 */
#define SCRATCHPAD_CRC_BYTE 8
/**
 * Count_Remain_Byte index im Scratchpad
 */
#define SCRATCHPAD_COUNT_REMAIN_BYTE 6
/**
 * Count_per_c ist beim ds18s20 immer 16
 */
#define COUNT_PER_C 16
/**
 *	Größe von uint8_t in bits
 */
#define SIZE_OF_UINT8 sizeof(uint8_t)*8
/*
 16*0.25 = 4. 16 weil die count_per_c_count register beim ds18s20 immer 16 ist
 */
#define TEMPERATURE_CONSTANT 0.25*COUNT_PER_C

/*
 *   Offset um float berechnung in einem int16_t speichern zu koennen
 *   100 wurde gewaehlt, weil 2 nachkomma stellen reichen
 */
#define INTEGER_OFFSET 100
/*
 *   \brief Ueberprueft die daten auf richtigkeit
 * \return 0 wenn die daten falsch sind ansonsten 1
 */
uint8_t crc_check(uint8_t data, uint8_t crc);
/*
 *   Ueberprueft das scratchpad array, ob alle daten richtig uebertragen wurden
 *   \return 0 wenn eines der daten korrupt ist ansonsten 1
 */
uint8_t check_data_with_crc(uint8_t * data, uint8_t length, uint8_t crc_byte);
/**
 * Error beim CRC check. Temperaturdaten sind nicht in ordnung
 */
#define ERROR_CRC 0
/**
 * Lesen der Temperaturdaten war erfolgreich
 */
#define READ_SUCCESFULL 1
/**
 * Status ob das Lesen des Temperatursensor erfolgreich war
 */
volatile uint8_t status_read_temperature;

int16_t ds18s20_read_temperature() {
	uint8_t scratchpad_data[SCRATCHPAD_SIZE_IN_BYTE] = { 0 };
	if (one_wire_reset()) {
		one_wire_write_byte(ROM_COMMAND_SKIP_ROM);
		one_wire_write_byte(FUNCTION_COMMAND_CONVERT_T);
		//Warte bis die temperatur in digitalform im scratchpad ist
		while (!one_wire_read_bit())
			;
		// Reset vor jedem ROM Command
		one_wire_reset();
		one_wire_write_byte(ROM_COMMAND_SKIP_ROM);
		one_wire_write_byte(FUNCTION_COMMAND_READ_SCRATCHPAD);
		//Lesen des scratchpads beinhaltet 9 bytes im ds18s20
		for (int i = 0; i < SCRATCHPAD_SIZE_IN_BYTE; i++) {
			scratchpad_data[i] = one_wire_read_byte();
		}
		if (check_data_with_crc(scratchpad_data, SCRATCHPAD_SIZE_IN_BYTE,
				scratchpad_data[SCRATCHPAD_CRC_BYTE])) {
			status_read_temperature = READ_SUCCESFULL;
			uint8_t count_remain = scratchpad_data[SCRATCHPAD_COUNT_REMAIN_BYTE];
			uint8_t count_per_c = COUNT_PER_C;
			uint8_t ls_byte = scratchpad_data[SCRATCHPAD_LS_BYTE];
			uint8_t ms_byte = scratchpad_data[SCRATCHPAD_MS_BYTE];
			int16_t temperature = ((int16_t) ms_byte << 8 | ls_byte);
			//Erstes bit clearen
			temperature &= ~1;
			//Auf 1/16 temperatur und platz schaffen fuer nachkomma
			temperature <<= 3;
			//Rechnung durchführen um eine 0,1 genauigkeit zu bekommen
			temperature = (temperature - TEMPERATURE_CONSTANT)
					+ (((count_per_c - count_remain)));
			//Auf richtigen wert skalieren für die anzeige
			return ((temperature * 10) >> 4) * 10;
		}
		status_read_temperature = ERROR_CRC;
	}
	status_read_temperature=ERROR_CRC;
	return 0;
}
/**
 * Überprüft, ob ein byte gekippte bits enthält durch einen crc check
 * \return 0 wenn das byte einen fehler hatte. Ansonsten 1
 */
uint8_t crc_check(uint8_t data, uint8_t crc) {
	if (_crc_ibutton_update(crc, data)) {
		return 1;
	}
	return 0;

}
/**
 * Überprüft, ob die bytes kein gekipptes bit enthalten durch crc
 * \return 1 wenn alle daten korrekt waren ansonsten 0
 */
uint8_t check_data_with_crc(uint8_t* data, uint8_t length, uint8_t crc_byte) {
	//-1 weil das letzte byte das crc byte nicht geprueft werden muss
	for (uint8_t i = 0; i < length - 1; i++) {
		if (!crc_check(data[i], crc_byte)) {
			return 0;
		}
	}
	return 1;
}

char * ds18s20_temperature_as_string(int16_t temperature,
		char *temperature_string) {
	char temp_as_string[STRING_CPACITY];
	sprintf(temp_as_string, "%d", temperature);
	uint8_t counter = 0;
	uint8_t length_of_number_before_com = 1;
	//Temperatur hat ein minus
	if (temperature < 0) {
		length_of_number_before_com++;
	}
	//Temperatu hat 2 zahlen vor dem komma
	if (temperature > 999 || temperature < (-999)) {
		length_of_number_before_com++;
	}
	//Temperatur ist 
	if (temperature > 9999 || temperature < (-9999)) {
		length_of_number_before_com++;
	}
	while (counter < STRING_CPACITY) {
		if (counter == length_of_number_before_com) {
			//Setze das nach dem die vorkomma zahlen behandelt wurden
			temperature_string[counter++] = ',';
			//Es gibt nur 1 nachkomma stelle
			if (temperature == 0) {
				temperature_string[counter] = '0';
			} else {
				temperature_string[counter] = temp_as_string[counter - 1];
			}
			counter++;
			//Alle Zahlen eingetragen nun die zeichen fuer Temperatur
			temperature_string[counter++] = 0xB2;
			temperature_string[counter++] = 'C';
			temperature_string[counter] = '\0';
			break;
		} else if (counter < length_of_number_before_com) {
			temperature_string[counter] = temp_as_string[counter];
		}
		counter++;
	}
	return temperature_string;

}

void temp_display(uint8_t row) {
	int16_t temperature = ds18s20_read_temperature();
	if (status_read_temperature == READ_SUCCESFULL) {
		char temp_string[STRING_CPACITY];
		ds18s20_temperature_as_string(temperature, temp_string);
		lcd_display_string_shift(temp_string, row);
	}
}
