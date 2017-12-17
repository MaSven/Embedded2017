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
Bereite OneWire interface darauf vor, das etwas gesendet werden k�nnte
*/
void one_wire_init();


#endif /* ONE_WIRE_H_ */