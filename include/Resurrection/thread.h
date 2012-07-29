/*
 * Resurrection/thread.h - multithread-related things for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef RESURRECTION_THREAD_H
#define RESURRECTION_THREAD_H

/*
 * Multithread support.
 */

#if defined(_POSIX_THREADS)
#   define USE_POSIX_THREADS 1
#endif

#if defined(CTHREADS)
#   define USE_CTHREADS 1
#endif

#if (USE_SVR4) && !defined(__sgi)
#   define USE_SVR4_THREADS 1
#endif

#if defined(WIN32)
#   define USE_WIN32_THREADS 1
#endif

/*
 * NOTE: <X11/Xthreads.h> defines an interface to threads. See the file for
 * more details.
 */

#if (USE_POSIX_THREADS) || (USE_CTHREADS) || (USE_SVR4_THREADS) || (USE_WIN32_THREADS)
#   if (HAVE_X11_XTHREADS_H)
#       include <X11/Xthreads.h>
#   elif (USE_POSIX_THREADS)
#       include <pthread.h>
#   elif (USE_CTHREADS)
#       include <cthreads.h>
#   elif (USE_SVR4_THREADS)
#       include <thread.h>
#       include <synch.h>
#   elif (USE_WIN32_THREADS)
#       include <windows.h>
#   endif
#endif

#endif /* RESURRECTION_THREAD_H */

