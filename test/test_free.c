#include "unity.h"

#include "pa_malloc.h"

#include <errno.h>
#include <stdlib.h>

void *PA_HEAP_ADDR;
size_t PA_HEAP_SIZE;
size_t PA_BLOCK_SIZE;

int errno;
void *p[2];

void setUp(void)
{
	PA_HEAP_SIZE = 64;
	PA_HEAP_ADDR = malloc(PA_HEAP_SIZE);
	PA_BLOCK_SIZE = sizeof(intptr_t)*2;

	pa_init();
}

void tearDown(void) {}

void test_free_null(void)
{
    errno = 0;
	pa_free(NULL);
    TEST_ASSERT_EQUAL_INT(EINVAL, errno);
}

void test_wrong_ptr(void)
{
    errno = 0;
	pa_free(PA_HEAP_ADDR - 0x1);
	TEST_ASSERT_EQUAL_INT(EINVAL, errno);

	errno = 0;
	pa_free(PA_HEAP_ADDR + 0x1);
	TEST_ASSERT_EQUAL_INT(EINVAL, errno);
}

void test_free(void)
{
    p[0] = pa_malloc();
    pa_free(p[0]);
    p[1] = pa_malloc();

    TEST_ASSERT_EQUAL_PTR(p[0], p[1]);;
}
