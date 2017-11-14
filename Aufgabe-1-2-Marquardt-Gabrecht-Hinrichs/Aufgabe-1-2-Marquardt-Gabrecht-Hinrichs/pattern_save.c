/*
 * pattern_save.c
 *
 * Created: 14.11.2017 08:44:53
 *  Author: Sven Marquardt
 */ 
#include "pattern_save.h"

//Die groesse die patterns einnehmen pro uint8_t
#define SIZE_PER_PATTERN_SLICE 4

typedef struct pattern_save{
	//Speicher für die patterns
	uint8_t [UINT8_MAX] patterns;
	//Index des momentan durchlaufenden patterns
	uint8_t index;
	//Gibt an an welcher stelle in einem uint8_t wir uns befinden
	uint8_t index_in_save;
}pattern_save_t;


pattern_save_t * patteern_save_create_new(){
	
	pattern_save_t pattern_save;
	pattern_save.index=0;
	pattern_save.index_in_save=0;
	pattern_save.size=0;
	pattern_save.patterns = {0};
	return &pattern_save;
	
}

uint8_t pattern_save_get_next_with_index(pattern_save_t *pattern_save,uint8_t index){
	//Erhalte den ersten pattern slice
	uint8_t index_without_offset = index/SIZE_PER_PATTERN_SLICE;
	uint8_t buffer_pattern = pattern_save->patterns[index/SIZE_PER_PATTERN_SLICE];
	
}
