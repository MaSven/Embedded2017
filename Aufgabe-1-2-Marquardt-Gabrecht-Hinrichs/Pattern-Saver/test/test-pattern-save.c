#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "pattern_save.h"
pattern_save_t * pattern_save_ptr;

static void test_create_pattern_save(){
    pattern_save_ptr = pattern_save_create_new();
    assert_non_null(pattern_save_ptr);
}


int main(void){
    const struct CMUnitTest tests[] ={
        cmocka_unit_test(test_create_pattern_save),
    };
    return cmocka_run_group_tests(tests,NULL,NULL);
}