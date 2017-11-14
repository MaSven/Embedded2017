/*
 * pattern_save.h
 *
 * Created: 14.11.2017 08:25:22
 *  Author: Sven Marquardt
 */ 


#ifndef PATTERN_SAVE_H_
#define PATTERN_SAVE_H_
//Speichert das Pattern und zugehörige attribute um den zugriff zu sichern
typedef struct pattern_save pattern_save_t;
//Erhalte einen neuen Speicher für patterns
pattern_save_t * create_new();
//Erhalte das Pattern an der Stelle x. Error bei ueberschreitung der groesse.
uint8_t get_next_pattern(pattern_save_t * pattern_save,uint8_t index);
//Erhalte das nächste pattern. Schiebt den iterator um einen nach vorne. Error bei ueberschreitung der groesse. Vorher also has_next_pattern aufrufen
uint8_t get_next_pattern(pattern_save_t * pattern_save);
//speicher ein neues pattern an der stelle des iterators
uint8_t save_pattern(pattern_save_t * pattern_save,uint8_t new_pattern);
//Liefert 1 wenn ein nächstes pattern vorhanden ist. 0 wenn kein weiteres vorhanden ist
uint8_t has_next_pattern(pattern_save_t *pattern_save);




#endif /* PATTERN_SAVE_H_ */