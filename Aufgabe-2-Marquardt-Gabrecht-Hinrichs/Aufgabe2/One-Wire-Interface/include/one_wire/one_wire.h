/**
* one_wire.h
*
* Created: 11.12.2017 22:10:33
*  Author: Matthias Hinrichs
*/
#ifndef ONE_WIRE_H_
#define ONE_WIRE_H_

/*
\brief Konvertierung von Analog in Digital
Initiiert die Umrechnung der analogen Temperatur in digitale Temperatur

DS1820 sendet den status der Umrechung an den Master. Nur im nicht parasit Modus moeglich

*/
#define FUNCTION_COMMAND_CONVERT_T 0x44;
/*
\brief Lese inhalt vom Scratchpad

Lese den Kompletten Inhalt der sich im moment im Scratchpad befindet
Master erhält bis zu 9 bytes
*/
#define FUNCTION_COMMAND_READ_SCRATCHPAD 0xbe;
/*
\brief Schreibe TH und TL in Scratchpad

Schreibe 2 byte in das Scratchpad vom DS1820.
Das erste byte wird dabei in das TH register(byte 2) geschrieben. Das zweite byte in das TL register (byte 3) geschrieben

*/
#define FUNCTION_COMMAND_WRITE_SCRATCHPAD 0x4e;
/*
\breif Kopiere den Inhalt vom Scratchpad in das EEPROM

Kopiert das TH und TL register in den EEPROM
*/
#define FUNCTION_COMMAND_COPY_SCRATCHPAD 0x48;
/*
\brief Alarm trigger neu setzen

Die werte die im EEPROM gespeichert sind, werden in TL und TH kopiert. Passiert automatisch beim starten
*/
#define FUNCTION_COMMAND_RECALL_E2 0xb8;
/*
\brief Herausfinden ob eines der DS1820 auf parasit modus läuft
*/
#define FUNCTION_COMMAND_READ_POWER_SUPPLY 0xb4;
#define ROM_COMMAND_SEARCH_ROM 0xf0;
#define ROM_COMMAND_READ_ROM 0x33;
#define ROM_COMMAND_MATCH_ROM 0x55;
#define ROM_COMMAND_SKIP_ROM 0xcc;
#define ROM_COMMAND_ALARM_SEARCH 0xec;

/**
\brief Signalisieren dass OneWire nicht mehr benutzt wird
*/
void one_wire_deactivate();
/**
Bereite OneWire interface darauf vor, das etwas gesendet werden könnte
*/
void one_wire_init();
/*
Reset befehl für onewire. Sollte ein 1 liefern wenn es ein onewire device gibt
*/
uint8_t one_wire_reset();


#endif /* ONE_WIRE_H_ */