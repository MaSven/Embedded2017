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
#include "global.h"

/************************************************************************/
/* Initialisierung des AD-Wandlers                                      */
/************************************************************************/
void adc_init(void);

/************************************************************************/
/* Auslesen der AD-Wandlung												*/
/************************************************************************/
uint8_t adc_read(void);

#endif /* ADC_H_ */
