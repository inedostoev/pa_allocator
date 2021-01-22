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
    PA_HEAP_SIZE = 32;
    PA_HEAP_ADDR = malloc(PA_HEAP_SIZE);
    PA_BLOCK_SIZE = sizeof(intptr_t);

    for (size_t i = 0; i < PA_HEAP_SIZE / PA_BLOCK_SIZE; i++) {
	ep[i] = PA_HEAP_ADDR + i*PA_BLOCK_SIZE;
    }

    pa_init();

    p[0] = pa_calloc();
    p[1] = pa_calloc();
    p[2] = pa_calloc();
    p[3] = pa_calloc();
    p[4] = pa_calloc();
}

void tearDown(void) {}

void test_calloc(void)
{
	TEST_ASSERT_EQUAL_PTR(ep[0], p[0]);
	TEST_ASSERT_EQUAL_PTR(ep[1], p[1]);
	TEST_ASSERT_EQUAL_PTR(ep[2], p[2]);
	TEST_ASSERT_EQUAL_PTR(ep[3], p[3]);

    intptr_t x = 0x0;
    TEST_ASSERT_EQUAL_MEMORY(&x, p[0], PA_BLOCK_SIZE);
    TEST_ASSERT_EQUAL_MEMORY(&x, p[1], PA_BLOCK_SIZE);
    TEST_ASSERT_EQUAL_MEMORY(&x, p[2], PA_BLOCK_SIZE);
    TEST_ASSERT_EQUAL_MEMORY(&x, p[3], PA_BLOCK_SIZE);

    TEST_ASSERT_EQUAL_PTR(NULL, p[4]);
}

