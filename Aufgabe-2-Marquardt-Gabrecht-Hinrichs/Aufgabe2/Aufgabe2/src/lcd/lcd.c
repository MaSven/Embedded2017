/*
 * lcd.c
 *
 * Created: 11.12.2017 22:10:52
 *  Author: Matthias Hinrichs
 */ 

#include "global.h"
#include "lcd.h"

void lcd_init(void)
{
	// Warten auf interne Initialisierung
	_delay_ms(15);
	// Umschalten in 8-Bit Modus
	LCDPORT |= LCD_EIGHT_BIT_MODE_COMMAND;
	lcd_send_enable_pulse();
	_delay_ms(LCD_EIGHT_BIT_DELAY_1_MS);
	lcd_send_enable_pulse();
	_delay_us(LCD_EIGHT_BIT_DELAY_2_US);
	lcd_send_enable_pulse();
	_delay_us(LCD_COMMAND_DELAY_US);
	// Umschalten in 4-Bit-Modus
	LCDPORT &= ~(LCD_EIGHT_BIT_MODE_COMMAND);
	LCDPORT |= LCD_FOUR_BIT_MODE_COMMAND;
	_delay_us(LCD_COMMAND_DELAY_US);
	LCDPORT &= ~(LCD_FOUR_BIT_MODE_COMMAND);
	// Display loeschen
	lcd_clear();	
}

void lcd_clear(void)
{
	lcd_send_command(LCD_CLEAR_DISPLAY_COMMAND);
	_delay_ms(LCD_CLEAR_DELAY_MS);
}

void lcd_cursor_home(void)
{
	lcd_send_command(LCD_CURSOR_HOME_COMMAND);
	_delay_ms(LCD_CURSOR_HOME_DELAY_MS);
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
	// Cursor auf Home setzen
	lcd_cursor_home();
	// Zweite Zeile beginnt ab 40
	if(row == 2)
	{
		col += 40;
	}
	while (col != 0)
	{
		lcd_send_command(LCD_CURSOR_MOVE_RIGHT_COMMAND);
		_delay_us(LCD_COMMAND_DELAY_US);
		col--;
	}
}

void lcd_send_nibble(uint8_t nibble)
{
	// Sendet immer das obere Nibble
	
	// Aktuelles oberes Nibble auf dem Port loeschen
	LCDPORT &= (0x0F);
	// Uebergebenes Nibble auf den Port legen
	LCDPORT |= (nibble & (0xF0));
	// Enable-Puls
	lcd_send_enable_pulse();
}

void lcd_send_char(uint8_t character)
{
	// Setzen von RS auf 1
	LCDPORT |= (1<<LCD_RS_PIN);
	// Senden des oberen Nibble
	lcd_send_nibble(character);
	// Senden des unteren Nibble
	lcd_send_nibble((character<<LCD_SHIFT));
	// Warten
	_delay_us(LCD_DATA_DELAY_US);
}

void lcd_send_string(const char *string, uint8_t length)
{
	for (int i=0; i<length;i++)
	{
		lcd_send_char(string[i]);
	}
}

void lcd_send_command(uint8_t command)
{
	// Setzen von RS auf 0
	LCDPORT &= ~(1<<LCD_RS_PIN);
	// Senden des oberen Nibble
	lcd_send_nibble(command);
	// Senden des unteren Nibble
	lcd_send_nibble((command<<LCD_SHIFT));
	// Warten
	_delay_us(LCD_COMMAND_DELAY_US);
}

void lcd_send_enable_pulse(void)
{
	LCDPORT |= (1<<LCD_E_PIN);
	// TODO: Wie lange soll der Puls sein?
	// Im Beispiel werden 3 Takte bei 4 MHz gewartet
	// https://www.mikrocontroller.net/articles/AVR-Tutorial:_LCD#Routinen_zur_LCD-Ansteuerung_im_4-Bit-Modus
	_delay_us(LCD_ENABLE_PULSE_US);
	LCDPORT &= ~(1<<LCD_E_PIN);
}