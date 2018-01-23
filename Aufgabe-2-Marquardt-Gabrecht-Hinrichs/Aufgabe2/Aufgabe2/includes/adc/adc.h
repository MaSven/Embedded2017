/*
 * adc.h
 *
 * \date 11.12.2017 22:10:13
 *  \author Matthias Hinrichs
 */
#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <inttypes.h>
#include "global.h"

/**
 * \brief Initialisierung des AD-Wandlers
 */
void adc_init(void);

/**
 * \brief Auslesen der AD-Wandlung
 */
uint8_t adc_read(void);

#endif /* ADC_H_ */
