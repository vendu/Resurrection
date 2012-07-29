/*
 * port.h - Resurrection porting utilities.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef __RESURRECTION_PORT_H__
#define __RESURRECTION_PORT_H__

/* Resurrection kludges. */
#define R_PAGESIZE  4096
#define R_PAGESHIFT 12

/* host system type. */
#if defined(USG)
#   define R_OS_SVR2 1
#endif
#if defined(SYSV)
#   define R_OS_SVR3 1
#endif
#if defined(SVR4)
#   define R_OS_SVR4 1
#endif

/* page size. */
#include <unistd.h>
#if !defined(_SC_PAGESIZE)
#   if defined(_SC_PAGE_SIZE)
#       define _SC_PAGESIZE _SC_PAGE_SIZE
#   endif
#endif
#if defined(_SC_PAGESIZE)
#   define R_GET_PAGE_SIZE() sysconf(_SC_PAGESIZE)
#else
#   define R_GET_PAGE_SIZE() getpagesize()
#endif

/* memory-mapping. */
#if defined(__FreeBSD__)
#include <sys/mman.h>
#   define R_MMAP_ANON     1
#   define R_MMAP_DEV_ZERO 0
#else
extern int _devzerofd;
#define R_MMAP_SVR3 (defined(R_OS_SVR3) && !defined(R_OS_SVR4))
#if (R_OS_SVR3)
#include <sys/immu.h>
#include <sys/region.h>
#endif
#include <sys/mman.h>
#if (R_MMAP_SVR3)
#   if (R_NEED_MMAP)
#       error fix SVR3 mmap() stuff in <Resurrection/port.h>
#   else
#       warning mmap() disabled for SVR3 in <Resurrection/port.h>
#       define R_NO_MMAP 1
#   endif
#elif defined(MAP_ANON)
#   define R_MMAP_MAP_ANON 1
#else
#   define R_MMAP_DEV_ZERO 1
#   define R_MMAP_CORE_DEVICE "/dev/zero"
#endif
#endif /* __FreeBSD__ */
#if !defined(MAP_FILE)
/* BSD-like systems require this to map files. Set to zero if not present. */
#   define MAP_FILE    0
#endif
#if !defined(MAP_FAILED)
/* Make a safe bet in case this is not present. Beware of dragons. :) */
#   define MAP_FAILED  ((void *)-1)
#endif
#endif /* __RESURRECTION_PORT_H__ */

