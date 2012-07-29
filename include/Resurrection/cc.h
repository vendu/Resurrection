/*
 * cc.h - Resurrection compiler declarations.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef __RESURRECTION_CC_H__
#define __RESURRECTION_CC_H__

#define R_CACHELINESIZE 32   /* i386 */
#define R_PAGESIZE      4096 /* i386 */

#if defined(__GNUC__)
#   define __CC_PACK__        __attribute__ ((__packed__))
#   define __CC_ALIGN__(a)    __attribute__ ((__aligned__((a))))
#endif /* __GNUC__ */

#define __CC_WORD_ALIGN__     __CC_ALIGN__(sizeof(long))
#define __CC_TYPE_ALIGN__(t)  __CC_ALIGN__(sizeof(t))
#define __CC_CACHE_ALIGN__    __CC_ALIGN__(R_CACHELINESIZE)
#define __CC_PAGE_ALIGN__     __CC_ALIGN__(R_PAGESIZE)

#endif /* __RESURRECTION_CC_H__ */

