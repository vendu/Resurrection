/*
 * Resurrection.h - Resurrection main header file.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef __RESURRECTION_RESURRECTION_H__
#define __RESURRECTION_RESURRECTION_H__

#define USE_IMLIB2         1
#define USE_XSHM           0 // breaks stuff for me
#define SUPPORT_FREETYPE2  1

#include <Resurrection/debug.h>
#include <Resurrection/setup.h>

#define R_OPTIMIZE_SPEED 1

#define FALSE 0
#define TRUE  1

#define _LARGEFILE_SOURCE 1

#define _POSIX_SOURCE     1
#define _POSIX_C_SOURCE   199506L

#define _XOPEN_SOURCE     600

#define _GNU_SOURCE       1

#define _BSD_SOURCE       1

#define _REENTRANT        1
#define _THREAD_SAFE      1

#include <Resurrection/autoconf.h>
#include <Resurrection/paths.h>

#ifndef RETSIGTYPE
#   define RETSIGTYPE void
#endif

/* ISO */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
/* UNIX */
#if (HAVE_MALLOC_H)
#include <malloc.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <unistd.h>
#include <sys/param.h>

#if (HAVE_SYS_ENDIAN_H)
#include <sys/endian.h>
#else
#include <endian.h>
#endif
#if (__GLIBC__)
#include <byteswap.h>
#define bswap32 bswap_32
#endif

#include <Resurrection/system.h>

#include <X11/Xlibint.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <X11/Xlocale.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
//#include <X11/X.h>

#include <X11/Xlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

#include <Imlib2.h>
#if (SUPPORT_FREETYPE2)
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_FREETYPE_H
#include FT_GLYPH_H
#endif

//#include <Resurrection/version.h>
#include <Resurrection/typedef.h>
#include <Resurrection/cc.h>

#include <Resurrection/image.h>
#include <Resurrection/struct.h>

#include <Resurrection/prof.h>

#include <Resurrection/port.h>

#include <Resurrection/macros.h>
#include <Resurrection/protos.h>
#include <Resurrection/binding.h>
#include <Resurrection/imlib2/protos.h>

#include <Resurrection/global.h>

#include <Resurrection/term.h>
#include <Resurrection/launch.h>
#include <Resurrection/wm.h>
#include <Resurrection/conf/term.h>
#include <Resurrection/conf/launch.h>
#include <Resurrection/conf/wm.h>

#endif /* __RESURRECTION_RESURRECTION_H__ */

