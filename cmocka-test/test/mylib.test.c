#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>

#include "mylib.h"
#include "umock.h"

MOCK(void *, malloc, trace_and_check, (size_t __size))
{
    printf("Mocked malloc!\n");
    check_expected(__size);
    return malloc(__size);
}

MOCK(void, free, trace, (void *__ptr))
{
    printf("Mocked free!\n");
    free(__ptr);
}

static void mylib_test(void **state)
{
    struct mylib_st *opaque;

    expect_value(MOCKNAME(malloc, trace_and_check), __size, 4);
    MOCKUSEONCE(malloc, trace_and_check);
    MOCKUSEONCE(free, trace);
    assert_int_equal(mylib_init(&opaque), 0);
    assert_int_equal(mylib_term(opaque), 0);
}

int main()
{
    const struct CMUnitTest tests[] =
        {
            cmocka_unit_test(mylib_test),
        };

    return cmocka_run_group_tests(tests, NULL, NULL);
}