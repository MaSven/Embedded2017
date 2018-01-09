/**
* one_wire.h
*
* Created: 11.12.2017 22:10:33
*  Author: Matthias Hinrichs
*/
#ifndef ONE_WIRE_H_
#define ONE_WIRE_H_

/*
* \brief Konvertierung von Analog in Digital
* Initiiert die Umrechnung der analogen Temperatur in digitale Temperatur
*
* DS1820 sendet den status der Umrechung an den Master. Nur im nicht parasit Modus moeglich
*
*/
#define FUNCTION_COMMAND_CONVERT_T 0x44
/*
* \brief Lese inhalt vom Scratchpad
*
*Lese den Kompletten Inhalt der sich im moment im Scratchpad befindet
* Master erhält bis zu 9 bytes
*/
#define FUNCTION_COMMAND_READ_SCRATCHPAD 0xbe
/*
* \brief Schreibe TH und TL in Scratchpad
*
* Schreibe 2 byte in das Scratchpad vom DS1820.
* Das erste byte wird dabei in das TH register(byte 2) geschrieben. Das zweite byte in das TL register (byte 3) geschrieben
*
*/
#define FUNCTION_COMMAND_WRITE_SCRATCHPAD 0x4e
/*
* \brief Kopiere den Inhalt vom Scratchpad in das EEPROM
* 
* Kopiert das TH und TL register in den EEPROM
*/
#define FUNCTION_COMMAND_COPY_SCRATCHPAD 0x48
/*
* \brief Alarm trigger neu setzen
* 
* Die werte die im EEPROM gespeichert sind, werden in TL und TH kopiert. Passiert automatisch beim starten
*/
#define FUNCTION_COMMAND_RECALL_E2 0xb8
/*
* \brief Herausfinden ob eines der DS1820 auf parasit modus läuft
*/
#define FUNCTION_COMMAND_READ_POWER_SUPPLY 0xb4
/*
*   \brief Eine runde Search rom initiieren
*   Weiteres dazu auf
*/
#define ROM_COMMAND_SEARCH_ROM 0xf0
/*
    \brief lesen des ROM
*/
#define ROM_COMMAND_READ_ROM 0x33
/*
*   \brief bestimmtes device auswaehlen
*/
#define ROM_COMMAND_MATCH_ROM 0x55
/*
*   \brief Alle devices auswaehlen
*/
#define ROM_COMMAND_SKIP_ROM 0xcc
/*
*   \brief suche nur devices mit alarm gesetzt
*/
#define ROM_COMMAND_ALARM_SEARCH 0xec


/*
* Reset befehl für onewire. Sollte ein 1 liefern wenn es ein onewire device gibt
*/
uint8_t one_wire_reset();
/*
    Sende byte an one wire
*/
void one_wire_write_byte(uint8_t data);
/*
    Schreibe gegebenes byte zum momentanen onewire. Erhalte Antwort
*/
uint8_t one_wire_read_write_byte(uint8_t data);
/*
*   Lese einzelnes bit vom scratchpad
*/
uint8_t one_wire_read_bit();
/*
*   Lese byte vom onewire
*/
uint8_t one_wire_read_byte();



#endif /* ONE_WIRE_H_ */