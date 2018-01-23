/**
 * hygro.h
 *
 * \date 17.01.2018 12:10:06
 *  \author Matthias Hinrichs
 */
#ifndef HYGRO_H_
#define HYGRO_H_

/**
 * Maximaler Wert von Poti/100
 */
#define HYGRO_MAP_FACTOR 2.48

/**
 * \brief Ausgabe der Luftfeuchtigkeit
 */
void hygro_display(uint8_t row);

#endif /* HYGRO_H_ */
