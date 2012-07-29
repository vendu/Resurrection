/*
 * <Resurrection/std.h> - standard-related things for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef RESURRECTION_STANDARD_H
#define RESURRECTION_STANDARD_H

/*
 * The following are included to get macros defining system type. <unistd.h>
 * is defined by POSIX.1 and XPG3.
 *
 * FIXME: don't include on systems where not available.
 */

#if !defined(NO_UNISTD_H)
#   include <unistd.h>
#endif

#if defined(DGUX)
#   include <sys/_int_unistd.h>
#endif

#if defined(_POSIX_VERSION)
#   define USE_POSIX_1 1
#   if (_POSIX_VERSION >= 2)
#       define USE_POSIX_2 1
#   endif
#   if (_POSIX_VERSION >= 199309L)
#       define USE_POSIX_1b 1
#   endif
#   if (_POSIX_VERSION >= 199506L)
#       define USE_POSIX_1c 1
#   endif
#endif

#if defined(_XOPEN_VERSION)
#   define USE_XPG 1
#endif

#if defined(_XOPEN_UNIX)
#   define USE_XOPEN_UNIX 1
#endif

/* NOTE: not sure if _XOPEN_XPG1 is defined anywhere. */

#if (_XOPEN_VERSION >= 1) || (_XOPEN_XPG1)
#   define USE_XPG_1 1
#endif

#if (_XOPEN_VERSION >= 2) || (_XOPEN_XPG2)
#   define USE_XPG_2 1
#endif

#if (_XOPEN_VERSION >= 3) || (_XOPEN_XPG3)
#   define USE_XPG_3 1
#endif

#if (_XOPEN_VERSION >= 4) || (_XOPEN_XPG4)
#   define USE_XPG_4 1
#endif

#if (_XOPEN_VERSION >= 500)
#   define USE_UNIX98 1
#endif

#endif /* RESURRECTION_STANDARD_H */

