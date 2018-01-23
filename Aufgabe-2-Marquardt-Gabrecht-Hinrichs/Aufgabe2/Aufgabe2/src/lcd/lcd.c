/*
 * lcd.c
 *
 * Created: 11.12.2017 22:10:52
 *  Author: Matthias Hinrichs
 */

#include <string.h>
#include <stdlib.h>
#include "global.h"
#include "lcd/lcd.h"

volatile uint8_t lcd_shift_flag = 0;
volatile uint8_t lcd_shift_abort = 0;

void lcd_init(void) {
	LCDDDR = ((0xF0) | (1 << LCD_RS_PIN) | (1 << LCD_E_PIN));
	LCDPORT = 0x00;
	// Warten auf interne Initialisierung
	_delay_ms(15);
	// Umschalten in 8-Bit Modus
	LCDPORT |= (LCD_FUNCTION_SET | LCD_FUNCTION_SET_EIGHT_BIT);
	lcd_send_enable_pulse();
	_delay_ms(LCD_EIGHT_BIT_DELAY_1_MS);
	lcd_send_enable_pulse();
	_delay_us(LCD_EIGHT_BIT_DELAY_2_US);
	lcd_send_enable_pulse();
	_delay_us(LCD_COMMAND_DELAY_US);
	// Umschalten in 4-Bit-Modus
	LCDPORT &= ~(LCD_FUNCTION_SET | LCD_FUNCTION_SET_EIGHT_BIT);
	LCDPORT |= (LCD_FUNCTION_SET | LCD_FUNCTION_SET_FOUR_BIT);
	lcd_send_enable_pulse();
	_delay_us(LCD_COMMAND_DELAY_US);
	LCDPORT &= ~(LCD_FUNCTION_SET | LCD_FUNCTION_SET_FOUR_BIT);
	// LCD mit zwei Zeilen und Zeichen der Grosse 5x7
	lcd_send_command(
			(LCD_FUNCTION_SET) | (LCD_FUNCTION_SET_TWO_LINES)
					| (LCD_FUNCTION_SET_EIGHT_DOTS));
	// Display an, Blinken aus, Cursor aus
	lcd_send_command(
			(LCD_DISPLAY_ON_OFF_CONTROL)
					| (LCD_DISPLAY_ON_OFF_CONTROL_DISPLAY_ON)
					| (LCD_DISPLAY_ON_OFF_CONTROL_BLINK_OFF)
					| (LCD_DISPLAY_ON_OFF_CONTROL_CURSOR_OFF));
	// Automatisches inkrementieren und kein Shift
	lcd_send_command(
			(LCD_ENTRY_MODE_SET) | (LCD_ENTRY_MODE_SET_INCREMENT)
					| (LCD_ENTRY_MODE_SET_NO_SHIFT));
	// Display loeschen
	lcd_clear();
}

void lcd_clear(void) {
	lcd_send_command(LCD_CLEAR_DISPLAY_COMMAND);
	_delay_ms(LCD_CLEAR_DELAY_MS);
}

void lcd_cursor_home(void) {
	lcd_send_command(LCD_CURSOR_HOME_COMMAND);
	_delay_ms(LCD_CURSOR_HOME_DELAY_MS);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
	// Kommando erstellen
	uint8_t DDRAMCommand = LCD_SET_DDRAM;
	if (row == 1) {
		DDRAMCommand |= ((LCD_DDRAM_FIRST_ROW) | (col));
	} else if (row == 2) {
		DDRAMCommand |= ((LCD_DDRAM_SECOND_ROW) | (col));
	}
	lcd_send_command(DDRAMCommand);
}

void lcd_send_nibble(uint8_t nibble) {
	// Sendet immer das obere Nibble

	// Aktuelles oberes Nibble auf dem Port loeschen
	LCDPORT &= (0x0F);
	// Uebergebenes Nibble auf den Port legen
	LCDPORT |= (nibble & (0xF0));
	// Enable-Puls
	lcd_send_enable_pulse();
}

void lcd_send_char(uint8_t character) {
	// Setzen von RS auf 1
	LCDPORT |= (1 << LCD_RS_PIN);
	// Senden des oberen Nibble
	lcd_send_nibble(character);
	// Senden des unteren Nibble
	lcd_send_nibble((character << LCD_SHIFT));
	// Warten
	_delay_us(LCD_DATA_DELAY_US);
}

void lcd_send_string(const char *string) {
	for (uint8_t i = 0;; i++) {
		if (string[i] == '\0') {
			break;
		} else {
			lcd_send_char(string[i]);
		}
	}
}

void lcd_display_string_shift(const char *string, uint8_t row) {
	uint8_t string_length = strlen(string);
	lcd_cursor_home();
	if (string_length < LCD_COLS) {
		lcd_set_cursor(row, (LCD_COLS - string_length) / 2);
	} else {
		lcd_set_cursor(row, 0);
	}
	lcd_send_string(string);
	if (string_length > LCD_COLS) {
		lcd_shift_left(string_length - LCD_COLS);
	}
}

void lcd_send_command(uint8_t command) {
	// Setzen von RS auf 0
	LCDPORT &= ~(1 << LCD_RS_PIN);
	// Senden des oberen Nibble
	lcd_send_nibble(command);
	// Senden des unteren Nibble
	lcd_send_nibble((command << LCD_SHIFT));
	// Warten
	_delay_us(LCD_COMMAND_DELAY_US);
}

void lcd_send_enable_pulse(void) {
	LCDPORT |= (1 << LCD_E_PIN);
	// Im Beispiel werden 3 Takte bei 4 MHz gewartet
	// https://www.mikrocontroller.net/articles/AVR-Tutorial:_LCD#Routinen_zur_LCD-Ansteuerung_im_4-Bit-Modus
	_delay_us(LCD_ENABLE_PULSE_US);
	LCDPORT &= ~(1 << LCD_E_PIN);
}

void lcd_shift_left(uint8_t x) {
	for (uint8_t i = 0; i <= x;) {
		if (lcd_shift_abort) {
			lcd_shift_abort = 0;
			break;
		}
		if (lcd_shift_flag) {
			lcd_send_command(
					LCD_CURSOR_DISPLAY_SHIFT
							| LCD_CURSOR_DISPLAY_SHIFT_DISPLAY_SHIFT
							| LCD_CURSOR_DISPLAY_SHIFT_LEFT_SHIFT);
			lcd_shift_flag = 0;
			i++;
		}
	}
}
