/*
 * utf8.h - internal header file for Tornado C Library.
 * Copyright (C) 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef TORNADO_INTERNAL_UTF8_H
#define TORNADO_INTERNAL_UTF8_H

extern wchar_t __ucstbl8859[15][UWORD8_MAX + 1];

#define __UTF8_ENCODE(u, p) \
do { \
    if ((u) < 0x80) { \
	*(p)++ = (unsigned char)(u); \
    } else if ((u) < 0x100) { \
	*(p)++ = (unsigned char )(0xc0 | (((u) & 0xc0) >> 6)); \
	*(p)++ = (unsigned char)(0x80 | ((u) & 0x3f)); \
    } else if ((u) < 0x800) { \
	*(p)++ = (unsigned char)(0xc0 | (((u) & 0x7c0) >> 6)); \
	*(p)++ = (unsigned char)(0x80 | ((u) & 0x03f)); \
    } else if ((u) < 0x1000) { \
	*(p)++ = 0xe0; \
	*(p)++ = (unsigned char)(0x80 | (((u) & 0xfc0) >> 5)); \
	*(p)++ = (unsigned char)(0x80 | ((u) & 0x03f)); \
    } else if ((u) < 0x10000) { \
	*(p)++ = (unsigned char)(0xe0 | (((u) & 0xf000) >> 12)); \
	*(p)++ = (unsigned char)(0x80 | (((u) & 0x0fc0) >> 6)); \
	*(p)++ = (unsigned char)(0x80 | ((u) & 0x003f)); \
    } else if ((u) < 0x200000) { \
	*(p)++ = (unsigned char)(0xf0 | (((u) & 0x1c0000) >> 18)); \
	*(p)++ = (unsigned char)(0x80 | (((u) & 0x03f000) >> 12)); \
	*(p)++ = (unsigned char)(0x80 | (((u) & 0x000fc0) >> 6)); \
	*(p)++ = (unsigned char)(0x80 | ((u) & 0x00003f)); \
    } else { \
	; /* perhaps in the future. */ \
    } \
} while (FALSE)

#endif /*  TORNADO_INTERNAL_UTF8_H */

