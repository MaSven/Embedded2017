/**
 * one_wire.h
 *
 * Created: 11.12.2017 22:10:33
 *  Author: Matthias Hinrichs
 */ 
#ifndef ONE_WIRE_H_
#define ONE_WIRE_H_

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