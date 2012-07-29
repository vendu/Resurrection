/*
 * Resurrection/select.h - select() support for Resurrection.
 */

#ifndef RESURRECTION_SELECT_H
#define RESURRECTION_SELECT_H

/*

Copyright (c) 1994  X Consortium

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/

/* <X11/Xpoll.h>, slightly reworked by Tuomo 'vendu' Venäläinen */

/*
 * macros for select() support.
 *
 * NOTE: this could all be done with #include <X11/Xpoll.h> without having
 * USE_POLL defined.
 */

#if defined(WIN32)

#   define FD_SETSIZE 256

#   define getdtablesize() FD_SETSIZE

#else /* !defined(WIN32) */

#if defined(__QNX__)
#   define FD_SETSIZE 256
#endif

#if 0
#if (HAVE_SYS_SELECT_H) || (USE_SVR4) || defined(CRAY) || defined(AIXV3) || defined(__QNX__)
#   include <sys/select.h>
#elif defined(MINIX)
#   include <sys/nbio.h>
#endif
#endif
#if defined(MINIX)
#   include <sys/nbio.h>
#else
#   include <sys/select.h>
#endif

/*
 * AIX 4.2 has broken <sys/select.h>. I guess we need to avoid reincluding it.
 */

#if defined(AIXV4) && !defined(NFDBITS)
#   include <sys/select.h>
#endif

#if defined(luna)
#   include <sysent.h>
#endif

#   if defined(CSRG_BASED) && (BSD < 199103)
typedef long fd_mask;
#   endif

#   if !defined(FD_SETSIZE)
#       define FD_SETSIZE 256 /* not necessarily adequate. */
#   endif

#   define FDBITS (sizeof(fd_mask) * NBBY)

/* 
 * BSD == 198911 -> OSF/1. Luna is 4.3BSD, but it has fd_set.
 */

#   if (USE_BSD) && (BSD < 198911)
#       if !defined(luna)
typedef struct fd_set {
    fd_mask fds_bits[howmany(FD_SETSIZE, FDBITS)];
} fd_set;
#       endif /* !defined(luna) */
#   endif /* (USE_BSD) && (BSD < 198911) */

#   if !defined(FD_SET)
#       define FD_SET(n,p) ((p)->fds_bits[((n) / FDBITS)] |= (1 << ((n) % FDBITS)))
#   endif

#   if !defined(FD_CLR)
#       define FD_CLR(n,p) ((p)->fdb_bits[((n) / FDBITS)] &= ~(1 << ((n) % FDBITS)))
#   endif

#   if !defined(FD_ISSET)
#       define FD_ISSET(n,p) ((p)->fds_bits[((n) / FDBITS)] & (1 << ((n) % FDBITS)))
#   endif

#   if !defined(FD_ZERO)
#       define FD_ZERO(p) bzero((char *)(p), sizeof(*(p)))
#   endif

#endif /* defined(WIN32) */

#endif /* RESURRECTION_SELECT_H */

