/*
 * Resurrection/os.h - operating system related things for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef RESURRECTION_OS_H
#define RESURRECTION_OS_H

/*
 * Operating systems.
 */

/*
 * Versions of System V Unix.
 */

#if defined(USG)
#   define USE_SVR2 1
#endif

#if defined(SYSV)
#   define USE_SVR3 1
#endif

#if defined(SVR4)
#   define USE_SVR4 1
#endif

/*
 * Versions of BSD Unix.
 */

#if defined(BSD)
#   define USE_BSD 1
#endif

#if defined(BSD4_2) || defined(BSD42)
#   define USE_BSD_42 1
#endif

#if defined(BSD4_3) || defined(BSD43)
#   define USE_BSD_42 1
#   define USE_BSD_43 1
#endif

#if defined(BSD4_4) || defined(BSD44)
#   define USE_BSD_42 1
#   define USE_BSD_43 1
#   define USE_BSD_44 1
#endif

#if (USE_BSD) && (BSD >= 199103)
#   if !defined(CSRG_BASED)
#       define CSRG_BASED 1
#   endif
#endif

#endif /* RESURRECTION_OS_H */

