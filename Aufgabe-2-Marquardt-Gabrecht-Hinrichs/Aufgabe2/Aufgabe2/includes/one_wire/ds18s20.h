/*
 * ds1820.h
 *
 * Created: 07.01.2018 16:55:57
 *  Author: Sven Marquardt
 */ 


#ifndef DS1820_H_
#define DS1820_H_
/*
*   Groesse die ein String haben muss um die Temperatur anzeigen zu koennen
*   8=Laenge von int16_t hoechster zahl + ein minus oder plus zeichen + ein komma + String terminator
*/
#define STRING_CPACITY 8
/**
*   \brief lesen der momentanen Temperatur
*   Lesen der Temperatur im DS18S20. Float werden umgerechnet z.B. wird 0.25 zu 25. Heisst,
*   die letzen beiden zahlen, stehen immer fuer die nachkomma stellen.
*   \return Liefert die temperatur als 16 bit.
*/
int16_t ds18s20_read_temperature();
/*
*   Wandelt die temperatur in eine String representation um
*   \param int16_t temperature Die temperatur die vom ds18s20 gelesen wurde
*   \param char* temperature_string Array in dem der String gespeichert werden soll. Sollte die laenge von STRING_CPACITY haben
*   \return Die temperatur als String. Komma wird richtig gesetzt
*/
char * ds18s20_temperature_as_string(int16_t temperature,char *temperature_string);
/*
*	Zeigt die Temperatur auf dem LCD an
*
*/
void temp_display(int row, int col);



#endif /* DS1820_H_ */