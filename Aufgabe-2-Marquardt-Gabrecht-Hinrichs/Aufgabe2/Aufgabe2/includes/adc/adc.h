/*
 * adc.h
 *
 * Created: 11.12.2017 22:10:13
 *  Author: Matthias Hinrichs
 */
#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <inttypes.h>

/************************************************************************/
/* Initialisierung des AD-Wandlers                                      */
/************************************************************************/
void adc_init(void);

/************************************************************************/
/* Auslesen der AD-Wandlung												*/
/************************************************************************/
uint8_t adc_read(uint8_t channel);

#endif /* ADC_H_ */