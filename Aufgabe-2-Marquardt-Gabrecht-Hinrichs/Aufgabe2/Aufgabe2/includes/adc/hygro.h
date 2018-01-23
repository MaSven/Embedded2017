/*
 * hygro.h
 *
 * Created: 17.01.2018 12:10:06
 *  Author: Matthias Hinrichs
 */ 


#ifndef HYGRO_H_
#define HYGRO_H_

//Constants
#define HYGRO_MAP_FACTOR 2.48 // (Maximaler Wert von Poti)/100

/************************************************************************/
/* Ausgabe der Luftfeuchtigkeit                                         */
/************************************************************************/
void hygro_display(uint8_t row);

#endif /* HYGRO_H_ */