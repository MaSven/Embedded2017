#include "bit_operations.h"
#include <stdio.h>

uint8_t get_nth_bit(uint8_t bitfield, uint8_t n){
	uint8_t mask = 3;
	printf("mask: %d bitfield: %d \n",mask,bitfield);
	printf("bitfieldshift: %d \n",(bitfield>>n));
	return (bitfield >> n) & mask;

}