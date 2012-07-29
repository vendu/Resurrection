/*
 * prof.h - Resurrection profiling tools.
 * Copyright (C) 2005 - 2006 Tuomo Venäläinen.
 *
 * See the file COPYING for information about using this software.
 */

#ifndef __RESURRECTION_PROF_H__
#define __RESURRECTION_PROF_H__

#define R_PROF_RESOURCE 0

#include <stdint.h>

/* FIXME: don't belong here */
#define R_TIMEVAL_COMPARE(tv1, tv2) \
    (((tv2)->tv_sec - (tv1)->tv_sec) * 1000000 \
     + ((tv2)->tv_usec - (tv1)->tv_usec))

#include <string.h>
#include <sys/time.h>
#if (R_PROF_RESOURCE)
#include <sys/resource.h>
#endif

#define R_PROF_CLOCK_DECL(id)     \
    struct timeval __starttv_##id, __stoptv_##id
#define R_PROF_CLOCK_INIT(id)     \
do { \
    memset(&__starttv_##id, 0, sizeof(__starttv_##id)); \
    memset(&__stoptv_##id, 0, sizeof(__stoptv_##id)); \
} while (FALSE)
#define R_PROF_CLOCK_START(id)    \
    gettimeofday(&__starttv_##id, NULL)
#define R_PROF_CLOCK_STOP(id)     \
    gettimeofday(&__stoptv_##id, NULL)
#define R_PROF_CLOCK_ELAPSED(id)  \
    (R_TIMEVAL_COMPARE(&__starttv_##id, &__stoptv_##id))

#if (R_PROF_RESOURCE)
#   define R_PROF_RESOURCE_DECL(id)         \
    struct rusage __startru_##id, __stopru_##id
#   define R_PROF_RESOURCE_INIT(id)         \
do { \
    memset(&__startru_##id, 0, sizeof(__startru_##id)); \
    memset(&__stopru_##id, 0, sizeof(__stopru_##id)); \
} while (FALSE)
#   define R_PROF_RESOURCE_START(id)        \
    getrusage(RUSAGE_SELF, &__startru_##id)
#   define R_PROF_RESOURCE_STOP(id)         \
    getrusage(RUSAGE_SELF, &__stopru_##id)
#   define R_PROF_RESOURCE_SYS_ELAPSED(id)  \
    (R_TIMEVAL_COMPARE(&__startru_##id.ru_stime, &__stopru_##id.ru_stime))
#   define PROF_RESOURCE_USER_ELAPSED(id) \
    (R_TIMEVAL_COMPARE(&__startru_##id.ru_stime, &__stopru_##id.ru_stime))
#   define R_PROF_RESOURCE_ELAPSED(id)      \
    (R_PROF_RESOURCE_SYS_ELAPSED(id) + PROF_RESOURCE_USER_ELAPSED(id))
#endif /* R_PROF_RESOURCE */

union __x86tickcnt {
    uint64_t u64val;
    uint32_t u32vals[2];
};

static __inline__ uint64_t
__x86rdtsc(union __x86tickcnt *cnt)
{
    __asm__ __volatile ("rdtsc; mov %%eax, %0; mov %%edx, %1"
                         : "=rm" (cnt->u32vals[0]), "=rm" (cnt->u32vals[1])
                         : 
                         : "eax", "edx");
    
    return (cnt->u64val);
};

#define R_PROF_TICK_DECL(id)    \
    union __x86tickcnt __starttc_##id, __stoptc_##id
#define R_PROF_TICK_INIT(id)    \
    __starttc_##id.u64val = __stoptc_##id.u64val = UINT64_C(0)
#define R_PROF_TICK_START(id)   \
    __x86rdtsc(&__starttc_##id)
#define R_PROF_TICK_STOP(id)    \
    __x86rdtsc(&__stoptc_##id)
#define R_PROF_TICK_ELAPSED(id) \
    ((__stoptc_##id.u64val > __starttc_##id.u64val) \
     ? (__stoptc_##id.u64val - __starttc_##id.u64val) \
     : (UINT64_C(0xffffffffffffffff) \
        - __starttc_##id.u64val \
        + __stoptc_##id.u64val))

#endif /* __RESURRECTION_PROF_H__ */

