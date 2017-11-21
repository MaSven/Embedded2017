/*
 * pattern_save.c
 *
 * Created: 14.11.2017 08:44:53
 *  Author: Sven Marquardt
 */
#include "pattern_save.h"
#include "bit_operations.h"
#include <stdlib.h>
#include <stdio.h>
//Die groesse die patterns einnehmen pro uint8_t
#define PATTERN_SLICES 4

struct pattern_save
{
	//Speicher für die patterns
	uint8_t patterns[UINT8_MAX];
	//Index des momentan durchlaufenden patterns
	uint16_t index;
	//momentan gefuellte groesse der patterns
	uint16_t size;
};

pattern_save_t *pattern_save_create_new()
{

	pattern_save_t *pattern_save = calloc(1, sizeof(pattern_save_t));
	return pattern_save;
}
//Hole den wert mit index
uint8_t pattern_save_get_next_with_index(pattern_save_t *pattern_save,
										 uint16_t index)
{
	//Erhalte den ersten pattern slice
	uint8_t index_without_offset = index / PATTERN_SLICES;
	if (index_without_offset > UINT8_MAX)
	{
		return 0;
	}
	uint8_t buffer_pattern = pattern_save->patterns[index_without_offset];
	uint8_t index_in_pattern = index % PATTERN_SLICES;
	
	buffer_pattern = get_nth_bit(buffer_pattern,(index_in_pattern));
	printf("%d index: %d \n",buffer_pattern,index);
	return buffer_pattern;
}

uint8_t pattern_save_has_next(pattern_save_t *pattern_save)
{

	if (pattern_save->index >= pattern_save->size)
	{
		return 0;
	}
	return 1;
}

uint8_t pattern_save_get_next(pattern_save_t *pattern_save)
{
	uint8_t buffer = pattern_save_get_next_with_index(pattern_save, pattern_save->index);
	pattern_save->index += 2;
	return buffer;
}

uint8_t pattern_save_save_new_pattern(pattern_save_t *pattern_save, uint8_t pattern)
{
	uint8_t index_in_pattern = pattern_save->size % PATTERN_SLICES;
	uint8_t index_without_offset = pattern_save->size / PATTERN_SLICES;
	uint8_t old_pattern_value = pattern_save->patterns[index_without_offset];
	old_pattern_value = old_pattern_value | (pattern << index_in_pattern);
	printf("%d index: %d pattern: %d\n",old_pattern_value,index_in_pattern,pattern);
	pattern_save->patterns[index_without_offset] = old_pattern_value;
	pattern_save->size += 2;
	return 1;
}
uint8_t pattern_save_set_iterator_begin(pattern_save_t *pattern_save)
{
	pattern_save->index = 0;
	return 1;
}