/*
 * typedef.h - Resurrection type definitions.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef __RESURRECTION_TYPEDEF_H__
#define __RESURRECTION_TYPEDEF_H__

#include <stdint.h>

/* sized types */
typedef uint8_t       R_byte_t;
#if 0
typedef uint16_t      R_dual_t;
typedef uint32_t      R_quad_t;
#endif

typedef R_byte_t      R_text_t;
typedef uint32_t      R_textflags_t;

typedef uint32_t      R_binding_t;

/* CPU types */
typedef long          R_word_t;
typedef unsigned long R_uword_t;

/* pixel types */
typedef int32_t       R_argb32_t;
typedef unsigned long R_color_t;
#if (USE_IMLIB2)
typedef double        R_colormod_t;
#else
typedef R_color_t     R_colormod_t;
#endif

/* function types */
typedef RETSIGTYPE    R_signalhandler_t(int);

#endif /* __RESURRECTION_TYPEDEF_H__ */

