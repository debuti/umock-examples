#include <gtest/gtest.h>

#include <stdio.h>
#include <stdlib.h>

#include "mylib.h"
#include "umock.h"

MOCK(void *, malloc, trace_and_check, (size_t __size))
{
    printf("Mocked malloc!\n");
    return malloc(__size);
}

MOCK(void, free, trace, (void *__ptr))
{
    printf("Mocked free!\n");
    free(__ptr);
}


TEST(mylib_test, init_and_term) {
    struct mylib_st *opaque;

    MOCKUSEONCE(malloc, trace_and_check);
    MOCKUSEONCE(free, trace);
    EXPECT_EQ(mylib_init(&opaque), 0);
    EXPECT_EQ(mylib_term(opaque), 0);
}