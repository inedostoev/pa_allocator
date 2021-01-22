/* A set of macros that cleans up code that needs to produce debug information. */

#define DBG_LOG_LEVEL_DEBUG (1)

#undef DBG_DEBUG

#ifndef DBGLOG_LEVEL
#define DBGLOG_LEVEL 0
#endif

#ifndef DBGLOG_FUNCTION
#define DBGLOG_FUNCTION fprintf
#endif

#if DBGLOG_LEVEL >= 1
#define DBGLOG_DEBUG(format, ...) DBGLOG_FUNCTION(stdout, format, ## __VA_ARGS__);  \
                                      fflush(stdout)
#else
#define DBGLOG_DEBUG(format, ...)
#endif


/*
 * Macros to protect the memory allocator in a multitasking system.
 * Macros must be manually configured for each platform.
 *
 * For ex. in freeRTOS it can be used macros from task.h
 * #define PA_CRITICAL_ENTRY()  taskENTER_CRITICAL()
 * #define PA_CRITICAL_EXIT()   taskEXIT_CRITICAL()
*/

#ifdef PA_MULTITASKING
/* your code */
#else
    #define PA_CRITICAL_ENTRY()
    #define PA_CRITICAL_EXIT()
#endif
