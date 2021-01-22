#include "unity.h"

#include "pa_malloc.h"
#include <stdlib.h>

void *p[5];
void *ep[4];

void *PA_HEAP_ADDR;
size_t PA_HEAP_SIZE;
size_t PA_BLOCK_SIZE;

void setUp(void)
{
    PA_HEAP_SIZE = 64;
    PA_HEAP_ADDR = malloc(PA_HEAP_SIZE);
    PA_BLOCK_SIZE = sizeof(intptr_t)*2;

    for (size_t i = 0; i < PA_HEAP_SIZE / PA_BLOCK_SIZE; i++) {
        ep[i] = PA_HEAP_ADDR + i*PA_BLOCK_SIZE;
    }

    pa_init();

    p[0] = pa_malloc();
    p[1] = pa_malloc();
    p[2] = pa_malloc();
    p[3] = pa_malloc();
    p[4] = pa_malloc();
}

void tearDown(void) {}

void test_malloc(void)
{
    TEST_ASSERT_EQUAL_PTR(ep[0], p[0]);
    TEST_ASSERT_EQUAL_PTR(ep[1], p[1]);
    TEST_ASSERT_EQUAL_PTR(ep[2], p[2]);
    TEST_ASSERT_EQUAL_PTR(ep[3], p[3]);

    TEST_ASSERT_EQUAL_PTR(NULL, p[4]);
}

