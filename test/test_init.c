#include "unity.h"

#include "pa_malloc.h"
#include <stdlib.h>
#include <errno.h>

void *p[5];
void *ep[4];

void *PA_HEAP_ADDR;
size_t PA_HEAP_SIZE;
size_t PA_BLOCK_SIZE;

/*  PA_HEAP_SIZE = 64;
    PA_HEAP_ADDR = malloc(PA_HEAP_SIZE);
    PA_BLOCK_SIZE = sizeof(intptr_t)*2;

    pa_init();
    */

void setUp(void) {}
void tearDown(void) {}

void test_null_heap(void)
{
    ssize_t r = 0;

    PA_HEAP_ADDR = NULL;
    r = pa_init();
    TEST_ASSERT_EQUAL_INT(-EINVAL, r);

    PA_HEAP_ADDR = malloc(64);

    PA_HEAP_SIZE = 0;
    r = pa_init();
    TEST_ASSERT_EQUAL_INT(-EINVAL, r);

    PA_HEAP_SIZE = 66;
    r = pa_init();
    TEST_ASSERT_EQUAL_INT(-EINVAL, r);
}
