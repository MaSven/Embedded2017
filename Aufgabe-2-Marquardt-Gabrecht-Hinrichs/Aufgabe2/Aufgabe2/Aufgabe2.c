/*
 * Aufgabe2.c
 *
 * Created: 11.12.2017 22:06:28
 *  Author: Matthias Hinrichs
 */ 


#include <avr/io.h>
#include "global.h"
#include "one_wire/one_wire.h"
int main(void)
{
    while(1)
    {
        DDRD = 0xFF;
        PORTD = 0xFF;
        uint8_t watch_for_1_wire = one_wire_reset();
        PIND = watch_for_1_wire;
        _delay_ms(500*4);
    }
}