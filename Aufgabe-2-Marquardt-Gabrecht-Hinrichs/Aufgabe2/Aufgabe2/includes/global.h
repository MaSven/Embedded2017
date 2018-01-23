/*
 * global.h
 *
 * Created: 12.12.2017 15:03:37
 *  Author: 240747
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>

//IO
//SENSORS
#define HYGROPORT PORTA
#define HYGRODDR DDRA
#define HYGROPIN PA2
#define TEMPPORT PORTB
#define TEMPDDR DDRB
#define TEMPPIN PB2
#define TEMPPININPUT PINB
//BUTTONS
#define BTNPORT PORTA
#define BTNDDR DDRA
#define UPBTN PA0
#define DOWNBTN PA3
#define ENTER PA4
#define CANCEL PA5
//LCD
#define LCDPORT PORTD
#define LCDDDR DDRD
#define LCD_RS_PIN PD2
#define LCD_E_PIN PD3
#define LCD_SHIFT 4
#define LCD_ROWS 2
#define LCD_COLS 16

//DELAYS
//ONEWIRE
#define ONEWIRE_DELAY_A_US 6
#define ONEWIRE_DELAY_B_US 64
#define ONEWIRE_DELAY_C_US 60
#define ONEWIRE_DELAY_D_US 10
#define ONEWIRE_DELAY_E_US 14
#define ONEWIRE_DELAY_F_US 45
#define ONEWIRE_DELAY_G_US 0
#define ONEWIRE_DELAY_H_US 480
#define ONEWIRE_DELAY_I_US 70
#define ONEWIRE_DELAY_J_US 410
#define ONEWIRE_RESET_SLOT 1
//LCD
#define LCD_ENABLE_PULSE_US 3
#define LCD_DATA_DELAY_US 45
#define LCD_COMMAND_DELAY_US 45
#define LCD_CLEAR_DELAY_MS 2
#define LCD_CURSOR_HOME_DELAY_MS 2
#define LCD_EIGHT_BIT_DELAY_1_MS 5
#define LCD_EIGHT_BIT_DELAY_2_US 100

#endif /* GLOBAL_H_ */
