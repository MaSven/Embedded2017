/*
 * lcd.h
 *
 * Created: 11.12.2017 22:10:23
 *  Author: Matthias Hinrichs
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "global.h"

//COMMANDS
//LCD
#define LCD_COMMAND_RS 0
#define LCD_COMMAND_RW 0
#define LCD_CLEAR_DISPLAY_COMMAND 0b00000001
#define LCD_CURSOR_HOME_COMMAND 0b00000010
#define LCD_CURSOR_MOVE_RIGHT_COMMAND 0b00010100
//ENTRY SET
#define LCD_ENTRY_MODE_SET 0x04 // 0b0000.0100
#define LCD_ENTRY_MODE_SET_INCREMENT 0x02 // 0b0000.0010
#define LCD_ENTRY_MODE_SET_DECREMENT 0x00 // 0b0000.0000
#define LCD_ENTRY_MODE_SET_SHIFT 0x01 // 0b0000.0001
#define LCD_ENTRY_MODE_SET_NO_SHIFT 0x00 // 0b0000.0001
//DISPLAY ON OFF CONTROL
#define LCD_DISPLAY_ON_OFF_CONTROL 0x08 // 0b0000.1000
#define LCD_DISPLAY_ON_OFF_CONTROL_DISPLAY_ON 0x04 // 0b0000.0100
#define LCD_DISPLAY_ON_OFF_CONTROL_DISPLAY_OFF 0x00 // 0b0000.0000
#define LCD_DISPLAY_ON_OFF_CONTROL_CURSOR_ON 0x02 // 0b0000.0010
#define LCD_DISPLAY_ON_OFF_CONTROL_CURSOR_OFF 0x00 // 0b0000.0000
#define LCD_DISPLAY_ON_OFF_CONTROL_BLINK_ON 0x01 // 0b0000.0001
#define LCD_DISPLAY_ON_OFF_CONTROL_BLINK_OFF 0x00 // 0b0000.0000
//CURSOR OR DISPLAY SHIFT
#define LCD_CURSOR_DISPLAY_SHIFT 0x10 // 0b0001.0000
#define LCD_CURSOR_DISPLAY_SHIFT_DISPLAY_SHIFT 0x08 // 0b0000.1000
#define LCD_CURSOR_DISPLAY_SHIFT_CURSOR_MOVE 0x00 // 0b0000.0000
#define LCD_CURSOR_DISPLAY_SHIFT_RIGHT_SHIFT 0x04 // 0b0000.0100
#define LCD_CURSOR_DISPLAY_SHIFT_LEFT_SHIFT 0x00 // 0b0000.0000
//FUNCTION SET
#define LCD_FUNCTION_SET 0x20 // 0b0010.0000
#define LCD_FUNCTION_SET_EIGHT_BIT 0x10 // 0b0001.0000
#define LCD_FUNCTION_SET_FOUR_BIT 0x00 // 0b0000.0000
#define LCD_FUNCTION_SET_ONE_LINE 0x00 // 0b0000.0000
#define LCD_FUNCTION_SET_TWO_LINES 0x08 // 0b0000.1000
#define LCD_FUNCTION_SET_TEN_DOTS 0x04 // 0b0000.0100
#define LCD_FUNCTION_SET_EIGHT_DOTS 0x00 // 0b0000.0000
//SET DDRAM
#define LCD_SET_DDRAM 0x80 // 0b1000.0000
#define LCD_DDRAM_FIRST_ROW 0x00 // 0b0000.0000
#define LCD_DDRAM_SECOND_ROW 0x40 // 0b0100.0000

/************************************************************************/
/* Initialisieren des Displays                                          */
/************************************************************************/
void lcd_init(void);

/************************************************************************/
/* Loeschen des Displays                                                */
/************************************************************************/
void lcd_clear(void);

/************************************************************************/
/* Zuruecksetzen des Cursors auf Home                                   */
/************************************************************************/
void lcd_cursor_home(void);

/************************************************************************/
/* Platzieren des Cursors an beliebiger Stelle                          */
/************************************************************************/
void lcd_set_cursor(uint8_t row, uint8_t col);

/************************************************************************/
/* Senden eines einzelnen Nibbles an das Display                        */
/************************************************************************/
void lcd_send_nibble(uint8_t nibble);

/************************************************************************/
/* Senden eines einzelnen Zeichens an das Display                       */
/************************************************************************/
void lcd_send_char(uint8_t character);

/************************************************************************/
/* Senden eines Strings an das Display                                  */
/************************************************************************/
void lcd_send_string(const char *string);

/************************************************************************/
/* Senden eines Strings und shiften des Displays                        */
/************************************************************************/
void lcd_display_string_shift(const char *string);

/************************************************************************/
/* Senden eines Befehls an das Display                                  */
/************************************************************************/
void lcd_send_command(uint8_t command);

/************************************************************************/
/* Senden eines Enable-Pulses                                           */
/************************************************************************/
void lcd_send_enable_pulse(void);

/************************************************************************/
/* Linksshift des Displays um x Stellen                                                                     */
/************************************************************************/
void lcd_left_shift(int x);

#endif /* LCD_H_ */