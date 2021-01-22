#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "pa_malloc.h"

#define DBGLOG_LEVEL 1
#include "cfg.h"

extern void *PA_HEAP_ADDR;
extern size_t PA_HEAP_SIZE;
extern size_t PA_BLOCK_SIZE;

typedef struct _pa_ptr_t {
    void *next;
} __attribute__((__packed__)) pa_ptr;

pa_ptr *pa_heap = NULL;
pa_ptr *pa_heap_end = NULL;
size_t pa_num_blocks = 0;
pa_ptr *pa_root = NULL;

#define PA_BLOCK(n)   ( (pa_ptr*)((uintptr_t)(pa_heap + ((PA_BLOCK_SIZE*n) / sizeof(uintptr_t)))) )
#define PA_NBLOCK(n)  ( (PA_BLOCK(n))->next)
#define PA_BLOCK_LAST ( (size_t)(pa_num_blocks - 1) )

ssize_t pa_verification()
{
    ssize_t r = 0;

    if (!PA_HEAP_ADDR) {
        DBGLOG_DEBUG("pa: Static pool not allocated!\n");
        r = -EINVAL;
        goto out;
    }
    if (!PA_HEAP_SIZE) {
        DBGLOG_DEBUG("pa: Invalid heap size. Must be more than 0!\n");
        r = -EINVAL;
        goto out;
    }
    if (PA_BLOCK_SIZE < sizeof(uintptr_t)) {
        DBGLOG_DEBUG("pa: Invalid block size. Must be more than ptr size(0x%zx)!\n", sizeof(uintptr_t));
        r = -EINVAL;
        goto out;
    }

out:
    return r;
}

ssize_t pa_init()
{
    DBGLOG_DEBUG("pa_init() call\n");
    ssize_t r = 0;

    r = pa_verification();
    if (r < 0) {
        DBGLOG_DEBUG("pa_verification() failed\n");
        goto out;
    }

    pa_heap = (pa_ptr*)PA_HEAP_ADDR;
    memset(pa_heap, 0x0, PA_HEAP_SIZE);

    pa_num_blocks = (PA_HEAP_SIZE / PA_BLOCK_SIZE);
    pa_heap_end = PA_BLOCK(PA_BLOCK_LAST);
    pa_root = pa_heap;

    DBGLOG_DEBUG("pa_heap := %p\n"
                "pa_heap_end := %p\n"
                "pa_num_blocks := %zu\n"
                "pa_block_size := %zu\n"
                "pa_root := %p\n",
                pa_heap, pa_heap_end, pa_num_blocks, PA_BLOCK_SIZE, pa_root);

    for (size_t i = 0; i < PA_BLOCK_LAST; i++) {
        PA_NBLOCK(i) = PA_BLOCK((size_t)(i+1));
        DBGLOG_DEBUG("\t(%zu)pa_next := %p\n", i, PA_NBLOCK(i));
    }
    PA_NBLOCK(PA_BLOCK_LAST) = NULL;
    DBGLOG_DEBUG("\t(%zu)pa_next := %p\n", PA_BLOCK_LAST, PA_NBLOCK(PA_BLOCK_LAST));

out:
    return r;
}

void *_pa_malloc()
{
    void *p = (void*)pa_root;
    pa_root = (pa_ptr*)(pa_root->next);
    return p;
}

void *pa_malloc()
{
    void *ptr = NULL;

    if (!pa_heap) {
        DBGLOG_DEBUG("pa: Call pa_init() before\n");
        errno = ENOMEM;
        goto out;
    }

    if (!pa_root) {
        DBGLOG_DEBUG("pa: Not enough space/cannot allocate memory\n");
        errno = ENOMEM;
        goto out;
    }

    PA_CRITICAL_ENTRY();
    ptr = _pa_malloc();
    PA_CRITICAL_EXIT();

    DBGLOG_DEBUG("pa: Alloc block %p\n", ptr);

out:
    return ptr;
}

void *pa_calloc(void) {
    void *p;
    p = pa_malloc();
    
    if (p)
        memset(p, 0x0, PA_BLOCK_SIZE);
    
    return p;
}

void _pa_free(pa_ptr *p)
{
    p->next = (void*)(pa_root);
    pa_root = p;
}

void pa_free(void *ptr)
{
    if (!pa_heap) {
        DBGLOG_DEBUG("pa: Call pa_init() before\n");
        errno = ENOMEM;
        goto out;
    }

    if (!ptr) {
        DBGLOG_DEBUG("pa: Free a null pointer\n");
        errno = EINVAL;
        goto out;
    }

    if ((pa_ptr *) ptr < pa_heap || (pa_ptr *) ptr > pa_heap_end) {
        DBGLOG_DEBUG("pa: Pointer out of range. Must be between %p and %p\n", pa_heap, pa_heap_end);
        errno = EINVAL;
        goto out;
    }

    if (((uintptr_t)(ptr) % PA_BLOCK_SIZE) != 0) {
        DBGLOG_DEBUG("pa: Pointer must be multiple of %zu\n", PA_BLOCK_SIZE);
        errno = EINVAL;
        goto out;
    }

    DBGLOG_DEBUG("pa: Freeing block %p\n", ptr);

    PA_CRITICAL_ENTRY();
    _pa_free((pa_ptr*)ptr);
    PA_CRITICAL_EXIT();

out:
    return;
}
