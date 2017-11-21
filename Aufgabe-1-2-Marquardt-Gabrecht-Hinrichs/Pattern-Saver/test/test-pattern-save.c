#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <inttypes.h>
#include <stdio.h>
#include "pattern_save.h"
#include "bit_operations.h"
pattern_save_t * pattern_save_ptr;


static void set_index_beginning(){
    printf("----------set_index_beginning-----\n");
    pattern_save_set_iterator_begin(pattern_save_ptr);
}

static void test_create_pattern_save(){
    printf("----------test_create_pattern_save-----\n");
    pattern_save_ptr = pattern_save_create_new();
    assert_non_null(pattern_save_ptr);
}

static void test_create_new_pattern(){
    printf("----------test_create_new_pattern-----\n");
    uint8_t new_pattern = 0b000011;
    pattern_save_save_new_pattern(pattern_save_ptr,new_pattern);
    assert_true(pattern_save_has_next(pattern_save_ptr));
    uint8_t get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(new_pattern,get_pattern);
    pattern_save_save_new_pattern(pattern_save_ptr,0b00000000);
    assert_true(pattern_save_has_next(pattern_save_ptr));
    get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(get_pattern,0);
    pattern_save_save_new_pattern(pattern_save_ptr,0b00000011);
    assert_true(pattern_save_has_next(pattern_save_ptr));
    get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(get_pattern,3);
    pattern_save_save_new_pattern(pattern_save_ptr,0b00000000);
    assert_true(pattern_save_has_next(pattern_save_ptr));
    get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(get_pattern,0);
    pattern_save_save_new_pattern(pattern_save_ptr,0b0000001);
    assert_true(pattern_save_has_next(pattern_save_ptr));
    get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(get_pattern,1);
    set_index_beginning();
}

static void test_set_index_beginning(){
    printf("----------test_set_idex_beginning-----\n");
    set_index_beginning();
    assert_true(pattern_save_has_next(pattern_save_ptr));
    uint8_t get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(3,get_pattern);
    assert_true(pattern_save_has_next(pattern_save_ptr));
    get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(get_pattern,0);
    assert_true(pattern_save_has_next(pattern_save_ptr));
    get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(get_pattern,3);
    assert_true(pattern_save_has_next(pattern_save_ptr));
    get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(get_pattern,0);
    get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(get_pattern,1);


}

static void test_overwrite_old_values(){
    printf("----------test_overwrite_old_values-----\n");
    uint8_t new_pattern = 0b00000011;
    pattern_save_save_new_pattern(pattern_save_ptr,new_pattern);
    assert_true(pattern_save_has_next(pattern_save_ptr));
    uint8_t get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(new_pattern,get_pattern);
    pattern_save_save_new_pattern(pattern_save_ptr,0b00000011);
    assert_true(pattern_save_has_next(pattern_save_ptr));
    get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(get_pattern,3);
    pattern_save_save_new_pattern(pattern_save_ptr,0b00000000);
    assert_true(pattern_save_has_next(pattern_save_ptr));
    get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(get_pattern,0);
    pattern_save_save_new_pattern(pattern_save_ptr,0b00000010);
    assert_true(pattern_save_has_next(pattern_save_ptr));
    get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(get_pattern,2);
    pattern_save_save_new_pattern(pattern_save_ptr,0b0000001);
    assert_true(pattern_save_has_next(pattern_save_ptr));
    get_pattern = pattern_save_get_next(pattern_save_ptr);
    printf("Alter wert %d\n",get_pattern);
    assert_int_equal(get_pattern,1);
    set_index_beginning();
}

static void test_copy_n_bits(){
    uint8_t value = get_nth_bit(0b11000010,0);
    printf("%d value\n",value);
    assert_int_equal(value,2);
    value= get_nth_bit(0b00001100,2);
    assert_int_equal(value,3);
    
}

static int test_pattern_save_free(void **state){
    pattern_save_free(pattern_save_ptr);
    assert_null(pattern_save_ptr);
    return 0;
}




int main(void){
    cmocka_set_message_output(CM_OUTPUT_XML);
    const struct CMUnitTest tests[] ={
        cmocka_unit_test(test_copy_n_bits),
        cmocka_unit_test(test_create_pattern_save),
        cmocka_unit_test(test_create_new_pattern),
        cmocka_unit_test(test_set_index_beginning),
        cmocka_unit_test(test_overwrite_old_values),
    };

    return cmocka_run_group_tests(tests,NULL,test_pattern_save_free);
}