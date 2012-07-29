/*
 * Resurrection/memory.h - memory related things for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef RESURRECTION_MEMORY_H
#define RESURRECTION_MEMORY_H

/*
 * Memory management.
 */

/*
 * NOTE: the POSIX name is _SC_PAGE_SIZE, but some SVR4 systems call it
 * _SC_PAGESIZE.
 */

#if (USE_POSIX_1) || (USE_SVR4)
#   define USE_SYSCONF 1
#endif

#if !defined(_SC_PAGE_SIZE)
#   if defined(_SC_PAGESIZE)
#       define _SC_PAGE_SIZE _SC_PAGESIZE
#   endif
#endif

/*
 * Values often in <machine/param.h>.
 */

#if !defined(PGSHIFT)
#   if defined(PAGE_SHIFT)
#       define PGSHIFT PAGE_SHIFT /* log2(NBPG) */
#   endif
#endif

#if !defined(PG_SHIFT)
#   if defined(PGSHIFT)
#       define PG_SHIFT PGSHIFT /* log2(NBPG) */
#   endif
#endif

#if !defined(NBPG)
#   if defined(EXEC_PAGESIZE)
#       define NBPG EXEC_PAGESIZE
#   elif defined(PAGE_SIZE)
#       define NBPG PAGE_SIZE
#   elif defined(PAGESIZE)
#       define NBPG PAGESIZE
#   elif defined(NBPC)
#       define NBPG NBPC
#   elif defined(PGSHIFT)
#       define NBPG (1UL << PGSHIFT)
#   endif /* defined(PAGE_SIZE) */
#endif

#if !defined (PGOFSET)
#   if defined(NBPG)
#       define PGOFSET (NBPG - 1)
#   endif
#endif

#if !defined(PDRSHIFT)
#   if defined(PGDIR_SHIFT)
#       define PDRSHIFT PGDIR_SHIFT
#   endif
#endif

#if !defined(PD_SHIFT)
#   if defined(PDRSHIFT)
#       define PD_SHIFT PDRSHIFT
#   endif
#endif

#if !defined(NBPDR)
#   if defined(PGDIR_SIZE)
#       define NBPDR PGDIR_SIZE
#   elif defined(PDRSHIFT)
#       define NBPDR (1UL << PDRSHIFT)
#   endif
#endif

/*
 * This is correct for BSD systems from 4.2 up.
 *
 * NOTE: NBPG is defined to other possible values above. We don't try to guess
 * page size in case a suitable (known) value is not present, because it could
 * break things seriously...
 */
#if ((HAVE_GETPAGESIZE) || (USE_XOPEN_UNIX) || (USE_SVR4) || (USE_BSD_42) || defined(linux) || defined(DGUX) || defined(sun) || defined(getpagesize)) && !defined(__hpux)
/* nothing; getpagesize() exists. */
#elif defined(_SC_PAGE_SIZE)
#   define getpagesize() sysconf(_SC_PAGE_SIZE)
#elif defined(NBPG)
#   if defined(CLSIZE)
#       define getpagesize() (NBPG * CLSIZE)
#   else
#       define getpagesize() (NBPG)
#   endif
#elif (defined(vax) && defined(VMS))
#   define getpagesize() 512
#endif

/*
 * Memory mapped I/O.
 */

#if defined(_POSIX_MAPPED_FILES)
#   define USE_POSIX_MMAP 1
#endif

/*
 * FIXME: not sure if this is correct for __osf__. I guess it should be...
 */
#if defined(MAP_ANON) || defined(CSRG_BASED) || defined(linux) || defined(__hpux) || defined(_AIX) || defined(__osf__)
#   define USE_MMAP_ANON 1
#elif (USE_POSIX_MMAP) || (USE_SVR4) || defined(DGUX) || defined(sun)
#   define USE_MMAP_DEV_ZERO 1
#elif (USE_SVR3)
#   define USE_SVR3_MMAP 1
#endif

#if (USE_SVR3_MMAP)
#   include <sys/immu.h>
#   include <sys/region.h>
#endif

#if (HAVE_MMAN_H) || (USE_POSIX_1b) || (USE_MMAP_ANON) || (USE_MMAP_DEV_ZERO) || (USE_SVR3_MMAP)
#   include <sys/mman.h>
#endif

#define DEV_ZERO_DEVICE "/dev/zero"

/*
 * Alternative names for flags for mmap().
 *
 * NOTE: Linux defines MAP_ANONYMOUS, BSD Unix defines MAP_FILE and MAP_ANON.
 */

/*
 * NOTE: BSD Unix requires MAP_FILE to map files. Define it to zero if not
 * defined to make programs compile on other systems.
 */

#if !defined(MAP_FILE)
#   define MAP_FILE 0
#endif

#if !defined(MAP_ANON)
#   if defined(MAP_ANONYMOUS)
#       define MAP_ANON MAP_ANONYMOUS
#   endif
#endif

#if !defined(MAP_ANONYMOUS)
#   if defined(MAP_ANON)
#       define MAP_ANONYMOUS MAP_ANON
#   endif
#endif

#if !defined(MAP_NORESERVE)
#   if defined MAP_AUTORESRV
#       define MAP_NORESERVE MAP_AUTORESRV
#   else
#       define MAP_NORESERVE 0
#   endif
#endif

#if !defined(MAP_FAILED)
#   define MAP_FAILED (-1)
#endif

/*
 * Memory mapped I/O macros.
 *
 * NOTE: MMAP_CHUNK(fd, addr, size, prot, flags)
 *
 * MMAP_CHUNK maps with MAP_ANON if available, uses "/dev/zero" or "/dev/mmap"
 * otherwise.
 * MMAP_CHUNK returns pointer to an mmap()'d area on success, -1 on failure.
 *
 * FIXME: SVR3 code...?
 */

#if (USE_MMAP_ANON)
#   define MMAP_CHUNK(fd,a,s,p,f) \
    mmap((a), (s), (p), (f) | MAP_PRIVATE | MAP_ANON | MAP_NORESERVE, -1, 0)
#   define MUNMAP_CHUNK(a,s) (munmap((a), (s)))
#elif (USE_MMAP_DEV_ZERO)
#   define MMAP_CHUNK(fd,a,s,p,f)
    ((fd < 0)
     ? (fd = open(DEV_ZERO_DEVICE, O_RDWR), mmap((a), (s), (p),
					         (f) | MAP_PRIVATE | MAP_FILE
						 | MAP_NORESERVE, (fd), 0))
     : mmap((a), (s), (p), (f) | MAP_FILE, (fd), 0))
#   define MUNMAP_CHUNK(a,s) (munmap((a), (s)))
#else
#   define MMAP_CHUNK(fd,a,s,p,f) (MAP_FAILED)
#   define MUNMAP_CHUNK(a,s)
#endif

#endif /* RESURRECTION_MEMORY_H */

