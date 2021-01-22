#ifndef PA_MALLOC_H
#define PA_MALLOC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void *PA_HEAP_ADDR;
extern size_t PA_HEAP_SIZE;
extern size_t PA_BLOCK_SIZE;

extern ssize_t pa_init(void);
extern void *pa_malloc(void);
extern void *pa_calloc(void);
extern void  pa_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif //PA_MALLOC_H
