/*
 * Resurrection/defs.h - system macros for Resurrection.
 */

#ifndef RESURRECTION_DEFS_H
#define RESURRECTION_DEFS_H

/*

Copyright (c) 1993, 1994  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

*/

/*
 * System identification and system dependent macros. Much of this was taken
 * from X11 Release 6.3 xterm and imake.
 */

/* operating system types and features. */

#if defined(att)
#   if !defined(ATT)
#       define ATT
#   endif /* !defined(ATT) */
#endif

#if 0
#if defined(SYSV) && defined(i386) && !defined(SVR4)
#   if !defined(ATT)
#       define ATT
#   endif /* !defined(ATT) */
#endif
#endif

/* Unixware defines __svr4__. */

#if defined(__svr4__) || defined(__SVR4)
#   if !defined(SVR4)
#       define SVR4
#   endif
#endif

#if defined(sun) && (defined(SVR4) || defined(__sol__))
#   if !defined(SVR4)
#       define SVR4
#   endif
#endif

#if defined(__sgi) && defined(SVR4)
#   undef ATT
#   if !defined(SYSV)
#       define SYSV
#   endif /* !defined(SYSV) */
#   undef SVR4
#endif

#if 0
/* FIXME: should I add #define USG here? */

#if defined(SVR4)
#   if !defined(ATT)
#       define ATT
#   endif /* !defined(ATT) */
#   if !defined(SYSV)
#       define SYSV
#   endif
#endif
#endif

#if defined(__CYGWIN__)
#   if !defined(SYSV)
#       define SYSV
#   endif /* !defined(SYSV) */
#   if !defined(SVR4)
#       define SVR4
#   endif /* !defined(SVR4) */
#if 0
#   if !defined(ATT)
#       define ATT
#   endif /* !defined(ATT) */
#endif
#endif

#if defined(SCO)
#   if defined(_SCO_DS)
#       if !defined(SCO325)
#           define SCO325
#       endif
#       if !defined(SVR4)
#           define SVR4
#       endif
#   endif
#endif

/* FIXME: does SVR4 belong here? */

#if defined(SCO325)
#   if !defined(_SVID3)
#       define _SVID3
#   endif /* !defined(_SVID3) */
#endif

#if defined(clipper) || defined(__clipper__)
#   if !defined(SYSV)
#       define SYSV
#   endif /* !defined(SYSV) */
#endif

#if defined(hpux)
#   if !defined(SYSV)
#       define SYSV
#   endif /* !defined(SYSV) */
#endif

#if defined(macII) || defined(_AUX_SOURCE)
#   if !defined(macII)
#       define macII
#   endif
#   if !defined(SYSV)
#       define SYSV
#   endif /* !defined(SYSV) */
#endif

#if defined(stellar)
#   if !defined(SYSV)
#       define SYSV
#   endif /* !defined(SYSV) */
#endif

#if defined(USL) || defined(__USLC__)
#   if !defined(USL)
#       define USL
#   endif
#endif

#if defined(USL) || defined(Oki) || defined(NCR)
#   if !defined(SVR4)
#       define SVR4
#   endif /* !defined(SVR4) */
#endif

/* SCO may define __USLC__ so put this after the __USLC__ check. */

#if defined(M_UNIX) || defined(_SCO_DS)
#   if !defined(sco)
#       define sco
#   endif /* !defined(sco) */
#   if !defined(SYSV)
#       define SYSV
#   endif /* !defined(SYSV) */
#endif

#if defined(sony)
#   if defined(SYSTYPE_SYSV) || defined(_SYSTYPE_SYSV)
#       if !defined(SVR4)
#           define SVR4
#       endif /* !defined(SVR4) */
#   else
#       if (NEWSOS < 41)
#           define NOSTDHDRS
#           define bsd43
#       elif (NEWSOS < 42)
#           define bsd43
#       endif
#   endif
#endif

#if defined(_CRAY)
#   if !defined(USG)
#       define USG
#   endif /* !defined(USG) */
#   if !defined(SYSV)
#       define SYSV
#   endif /* !defined(SYSV) */
#endif

#if defined(_IBMR2) || defined(aix)
#   if !defined(aix)
#       define aix
#   endif
#   if !defined(SYSV)
#       define SYSV
#   endif /* !defined(SYSV) */
#   if !defined(ibm)
#       define ibm
#   endif
#endif

#if defined(Mips)
#   if defined(SYSTYPE_BSD) || defined(BSD) || defined(BSD43)
#       if !defined(BSD43)
#           define BSD43
#       endif
#   else
#       if !defined(SYSV)
#           define SYSV
#       endif /* !defined(SYSV) */
#   endif
#endif

#if defined(is68k)
#   define luna
#   define uniosb
#endif

#if defined(SYSV386)
#   if !defined(SVR4)
#       if !defined(SYSV)
#           define SYSV
#       endif /* !defined(SYSV) */
#   endif
#endif

#if defined(apollo)
#   define X_NOT_POSIX
#endif

#if defined(WIN32)
#   if !defined(__STDC__)
#       define __STDC__
#   endif
#endif

#if defined(__uxp__)
#   if !defined(SVR4)
#       define SVR4
#   endif /* !defined(SVR4) */
#   if !defined(ANSICPP)
#       define ANSICPP
#   endif
#endif

#if defined(__sxg__)
#   if !defined(USG)
#       define USG
#   endif /* !defined(USG) */
#   if !defined(SYSV)
#       define SYSV
#   endif /* !defined(SYSV) */
#   define NOSTDHDRS
#endif

/* TODO: NOSTDHDRS would do the same... */

#if defined(sequent)
#   define X_NOT_STDC_ENV
#   define X_NOT_POSIX
#endif

#if defined(_SEQUENT_)
#   if !defined(USG)
#       define USG
#   endif /* !defined(USG) */
#   if !defined(SYSV)
#       define SYSV
#   endif /* !defined(SYSV) */
#endif

#if defined(SX) || defined(PC_UX)
#   if !defined(SYSV)
#       define SYSV
#   endif /* !defined(SYSV) */
#endif

#if defined(nec_ews_svr2)
#   if !defined(USG)
#       define USG
#   endif /* !defined(USG) */
#endif

#if defined(nec_ews_svr4) || defined(_nec_ews_svr4) || defined(_nec_up) || defined(_nec_ft)
#   if !defined(SVR4)
#       define SVR4
#   endif /* !defined(SVR4) */
#endif

#if defined(MACH) && !defined(__GNU__)
#   define NOSTDHDRS
#endif

/* This is for OS/2 under EMX. This won't work with DOS. */

#if defined(__EMX__)
#   define BSD43
#   define emxos2
#endif

#if defined(_AIX) || defined(aix)
#   if !defined(_AIX)
#       define _AIX
#   endif /* !defined(_AIX) */
#   if !defined(aix)
#       define aix
#   endif /* !defined(aix) */
#endif

#if defined(__minix_vmd)
#   define minix
#endif

#if defined(__QNX__) && !defined(__QNXNTO__)
#   define _i386
#endif

#if defined(linux) || defined(__linux__)
#   if !defined(linux)
#       define linux
#   endif /* !defined(linux) */
#   if !defined(__linux__)
#       define __linux__
#   endif /* !defined(__linux__) */
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(WINDOWS32)
#   if !defined(WIN32)
#       define WIN32
#   endif /* !defined(WIN32) */
#   if !defined(_WIN32)
#       define _WIN32
#   endif /* !defined(_WIN32) */
#   if !defined(__WIN32__)
#       define __WIN32__
#   endif /* !defined(__WIN32__) */
#   if !defined(WINDOWS32)
#       define WINDOWS32
#   endif /* !defined(WINDOWS32) */
#endif

#if defined(__amiga__) || defined(_AMIGA)
#   if !defined(__amiga__)
#       define __amiga__
#   endif /* !defined(__amiga__) */
#   if !defined(_AMIGA)
#       define _AMIGA
#   endif /* !defined(_AMIGA) */
#endif

/* FIXME: is this correct? */

#if defined(SYSV)
#   if !defined(ATT)
#       define ATT
#   endif
#   if !defined(USG)
#       define USG
#   endif
#endif

/* FIXME: does USG belong here? */

#if defined(SVR4)
#   if !defined(ATT)
#       define ATT
#   endif /* !defined(ATT) */
#   if !defined(USG)
#       define USG
#   endif
#   if !defined(SYSV)
#       define SYSV
#   endif
#endif

/* host CPU types. */

#if defined(__alpha__) || defined(__alpha)
#   if !defined(__alpha__)
#       define __alpha__
#   endif /* !defined(__alpha__) */
#   if !defined(__alpha)
#       define __alpha
#   endif /* !defined(__alpha) */
#endif

#if defined(__a29k__) || defined(_AM29K) || defined(_AM29000)
#   if !defined(__a29k__)
#       define __a29k__
#   endif /* !defined(__a29k__) */
#   if !defined(_AM29K)
#       define _AM29K
#   endif /* !defined(_AM29K) */
#   if !defined(_AM29000)
#       define _AM29000
#   endif /* !defined(_AM29000) */
#   define _EPI
#endif

#if defined(clipper) || defined(__clipper__)
#   if !defined(clipper)
#       define clipper
#   endif /* !defined(clipper) */
#   if !defined(__clipper__)
#       define __clipper__
#   endif /* !defined(__clipper__) */
#endif

#if defined(gmicro) || defined(__gmicro__)
#   if !defined(gmicro)
#       define gmicro
#   endif /* !defined(gmicro) */
#   if !defined(__gmicro__)
#       define __gmicro__
#   endif /* !defined(__gmicro__) */
#endif

#if defined(hppa) || defined(__hppa) || defined(_PA_RISC1_1)
#   if !defined(hppa)
#       define hppa
#   endif /* !defined(hppa) */
#   if !defined(__hppa)
#       define __hppa
#   endif /* !defined(__hppa) */
#endif

#if defined(i386) || defined(__i386__) || defined(_i386)
#   if !defined(i386)
#       define i386
#   endif
#   if !defined(__i386__)
#       define __i386__
#   endif
#   if !defined(_i386)
#       define _i386
#   endif
#endif

#if defined(i486) || defined(__i486__) || defined(__i486)
#   if !defined(i486)
#       define i486
#   endif /* !defined(i486) */
#   if !defined(__i486__)
#       define __i486__
#   endif /* !defined(__i486__) */
#   if !defined(__i486)
#       define __i486
#   endif /* !defined(__i486) */
#endif

#if defined(i860) || defined(__i860__)
#   if !defined(i860)
#       define i860
#   endif /* !defined(i860) */
#   if !defined(__i860__)
#       define __i860__
#   endif /* !defined(__i860__) */
#endif

#if defined(i960) || defined(__i960__) || defined(i80960) || defined(I960) || defined(I80960)
#   if !defined(i960)
#       define i960
#   endif /* !defined(i960) */
#   if !defined(__i960__)
#       define __i960__
#   endif /* !defined(__i960__) */
#   if !defined(i80960)
#       define i80960
#   endif /* !defined(i80960) */
#   if !defined(I960)
#       define I960
#   endif /* !defined(I960) */
#   if !defined(I80960)
#       define I80960
#   endif /* !defined(I80960) */
#endif

#if defined(mc68000) || defined(__mc68000__)
#   if !defined(mc68000)
#       define mc68000
#   endif /* !defined(mc68000) */
#   if !defined(__mc68000__)
#       define __mc68000__
#   endif /* !defined(__mc68000__) */
#   if !defined(__m68k__)
#       define __m68k__
#   endif /* !defined(__m68k__) */
#endif

#if defined(mc68020) || defined(__mc68020__)
#   if !defined(mc68020)
#       define mc68020
#   endif /* !defined(mc68020) */
#   if !defined(__mc68020__)
#       define __mc68020__
#   endif /* !defined(__mc68020__) */
#   if !defined(__m68k__)
#       define __m68k__
#   endif /* !defined(__m68k__) */
#endif

#if defined(mc68030) || defined(__mc68030__)
#   if !defined(mc68030)
#       define mc68030
#   endif /* !defined(mc68030) */
#   if !defined(__mc68030__)
#       define __mc68030__
#   endif /* !defined(__mc68030__) */
#   if !defined(__m68k__)
#       define __m68k__
#   endif /* !defined(__m68k__) */
#endif

#if defined(mc68040) || defined(__mc68040__)
#   if !defined(mc68040)
#       define mc68040
#   endif /* !defined(mc68040) */
#   if !defined(__mc68040__)
#       define __mc68040__
#   endif /* !defined(__mc68040__) */
#   if !defined(__m68k__)
#       define __m68k__
#   endif /* !defined(__m68k__) */
#endif

#if defined(mc68060) || defined(__mc68060__)
#   if !defined(mc68060)
#       define mc68060
#   endif /* !defined(mc68060) */
#   if !defined(__mc68060__)
#       define __mc68060__
#   endif /* !defined(__mc68060__) */
#   if !defined(__m68k__)
#       define __m68k__
#   endif /* !defined(__m68k__) */
#endif

#if defined(m88000) || defined(__m88000__)
#   if !defined(m88000)
#       define m88000
#   endif /* !defined(m88000) */
#   if !defined(__m88000__)
#       define __m88000__
#   endif /* !defined(__m88000__) */
#   if !defined(__m88k__)
#       define __m88k__
#   endif /* !defined(__m88k__) */
#endif

#if defined(m88100) || defined(__m88100__)
#   if !defined(m88100)
#       define m88100
#   endif /* !defined(m88100) */
#   if !defined(__m88100__)
#       define __m88100__
#   endif /* !defined(__m88100__) */
#   if !defined(__m88k__)
#       define __m88k__
#   endif /* !defined(__m88k__) */
#endif

#if defined(m88110) || defined(__m88110__)
#   if !defined(m88110)
#       define m88110
#   endif /* !defined(m88110) */
#   if !defined(__m88110__)
#       define __m88110__
#   endif /* !defined(__m88110__) */
#   if !defined(__m88k__)
#       define __m88k__
#   endif /* !defined(__m88k__) */
#endif

#if defined(m88k) || defined(__m88k__)
#   if !defined(m88k)
#       define m88k
#   endif /* !defined(m88k) */
#   if !defined(__m88k__)
#       define __m88k__
#   endif /* !defined(__m88k__) */
#endif

#if defined(sparc) || defined(__sparc__)
#   if !defined(sparc)
#       define sparc
#   endif /* !defined(sparc) */
#   if !defined(__sparc__)
#       define __sparc__
#   endif /* !defined(__sparc__) */
#endif

/* Compiler environment. */

#if defined(hpux)
#   define USE_CC_E
#endif

#if defined(WIN32)
#   define DEFAULT_CC "cl"
#   define USE_CC_E
#endif

#if defined(apollo)
#   define DEFAULT_CPP "/usr/lib/cpp"
#endif

#if defined(__clipper__)
#   define DEFAULT_CPP "/usr/lib/cpp"
#endif

#if defined(_IBMR2) && !defined(DEFAULT_CPP)
#   define DEFAULT_CPP "/usr/ccs/lib/cpp"
#endif

#if defined(sun) && defined(SVR4)
#   define DEFAULT_CPP "/usr/ccs/lib/cpp"
#endif

#if defined(__bsdi__)
#   define DEFAULT_CPP "/usr/bin/cpp"
#endif

#if defined(__uxp__)
#   define DEFAULT_CPP "/usr/ccs/lib/cpp"
#endif

#if defined(__sxg__)
#   define DEFAULT_CPP "/usr/lib/cpp"
#endif

#if defined(_CRAY)
#   define DEFAULT_CPP "/lib/pcpp"
#endif

#if defined(__386BSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#   define DEFAULT_CPP "/usr/libexec/cpp"
#endif

#if defined(__FreeBSD__)
#   define USE_CC_E
#endif

#if defined(__sgi) && defined(__ANSI_CPP__)
#   define USE_CC_E
#endif

#if defined(MACH)
#   define USE_CC_E
#endif

#if defined(__minix_vmd)
#   define DEFAULT_CPP "/usr/lib/cpp"
#endif

/* expects cpp in PATH. */

#if defined(__EMX__)
#   define DEFAULT_CPP "cpp"
#endif

#if defined(__GNU__)
#   define USE_CC_E
#endif

#if defined(__QNX__)
#   define DEFAULT_CPP "/usr/X11R6/bin/cpp"
#endif

/* compiler flags. */

#define DEFAULT_CC_FLAGS "-g -O -I. -I.. -L/usr/X11R6/lib"

/* remove unix symbol so that filename unix.c is OK. */

#if !defined(__NetBSD_Version__) || (__NetBSD_Version < 103080000) && defined(unix)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -Uunix"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

#if defined(__386BSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__FreeBSD__) || defined(MACH) || defined(linux) || defined(__GNU__)
#   if defined(__i386__)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -D__i386__"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   elif defined(__sparc__)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -D__sparc__"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   elif defined(__m68k__)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -D__m68k__"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#endif

/* TODO: see gcc for more Tektronix flags. */

#if defined(M4330)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -DM4330"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#elif defined(M4310)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -DM4310"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

/* Apple A/UX. */

#if defined(macII) || defined(_AUX_SOURCE)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -DmacII"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

#if defined(USL)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -DUSL"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

#if defined(sony) && defined(bsd43)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -Dbsd43"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

/* IBM RS/6000 (aix is defined above). */

#if defined(_IBMR2)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -D_IBMR2"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

/* AIX instead of AOS. */

#if defined(aix)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -Daix"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

/* IBM PS/2 and RT under both AOS and AIX. */

#if defined(ibm)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -Dibm"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

/* Omron Luna 68k and 88k. */

#if defined(luna)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -Dluna"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#   if defined(luna1)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -Dluna1"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#   if defined(uniosb)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -Duniosb"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#   if defined(uniosu)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -Duniosu"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#endif

#if defined(_CRAY)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -Ucray"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

/* Mips operating systems and computers. */

#if defined(Mips)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -DMips"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#   if defined(SYSTYPE_BSD) || defined(BSD) || defined(BSD43)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -DBSD43"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   else
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -DSVR4"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#endif

#if defined(MOTOROLA)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -DMOTOROLA"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#   if defined(SYSV)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -DSYSV"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#   if defined(SVR4)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -DSVR4"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#endif

#if defined(M_UNIX) || defined(sco)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -Dsco -DSYSV"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

/* Intel X86 CPU or System V/386 operating system. */

#if defined(i386) || defined(SYSV386)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -Di386"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#   if defined(ATT)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -DATT"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#   if defined(SYSV)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -DSYSV"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#       if defined(ISC)
#           undef CC_FLAGS
#           define CC_FLAGS DEFAULT_CC_FLAGS " -DISC"
#           undef DEFAULT_CC_FLAGS
#           define DEFAULT_CC_FLAGS CC_FLAGS
#           if defined(ISC40) /* ISC 4.0. */
#               undef CC_FLAGS
#               define CC_FLAGS DEFAULT_CC_FLAGS " -DISC40"
#               undef DEFAULT_CC_FLAGS
#               define DEFAULT_CC_FLAGS CC_FLAGS
#           elif defined(ISC30) /* ISC 3.0. */
#               undef CC_FLAGS
#               define CC_FLAGS DEFAULT_CC_FLAGS " -DISC30"
#               undef DEFAULT_CC_FLAGS
#               define DEFAULT_CC_FLAGS CC_FLAGS
#           elif defined(ISC202) /* ISC 2.0.2. */
#               undef CC_FLAGS
#               define CC_FLAGS DEFAULT_CC_FLAGS " -DISC202"
#               undef DEFAULT_CC_FLAGS
#               define DEFAULT_CC_FLAGS CC_FLAGS
#           else /* ISC 2.2.1. */
#               undef CC_FLAGS
#               define CC_FLAGS DEFAULT_CC_FLAGS " -DISC22"
#               undef DEFAULT_CC_FLAGS
#               define DEFAULT_CC_FLAGS CC_FLAGS
#           endif
#       endif /* defined(ISC) */
#       if defined(SCO) /* SCO Unix. */
#           undef CC_FLAGS
#           define CC_FLAGS DEFAULT_CC_FLAGS " -DSCO"
#           undef DEFAULT_CC_FLAGS
#           define DEFAULT_CC_FLAGS CC_FLAGS
#           if defined(_SCO_DS)
#               undef CC_FLAGS
#               define CC_FLAGS DEFAULT_CC_FLAGS " -DSCO325 -DSVR4"
#               undef DEFAULT_CC_FLAGS
#               define DEFAULT_CC_FLAGS CC_FLAGS
#           endif
#       endif
#   endif /* defined(SYSV) */
#   if defined(ESIX)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -DESIX"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#   if defined(DELL)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -DMips"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#endif /* defined(i386) || defined(SYSV386) */

#if defined(__osf__)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -D__osf__"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#   if defined(__mips__)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -D__mips__"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif /* defined(__mips__) */
#   if defined(__alpha)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -D__alpha"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif /* defined(__alpha) */
#   if defined(__alpha__)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -D__alpha__"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif /* defined(__alpha__) */
#   if defined(__amiga__)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -D__amiga__"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif /* defined(__amiga__) */
#   if defined(__i386__)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -D__i386__"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif /* defined(__i386__) */
#endif /* defined(__osf__) */

#if defined(Oki)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -DOki"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

#if defined(sun) && defined(SVR4)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -DSVR4"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

#if defined(WIN32)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -D__STDC__ -DWIN32 -nologo"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#   if (MSC_VER < 1000)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -batch"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#endif /* defined(WIN32) */

#if defined(NCR)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -DNCR"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

#if defined(linux)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -Dlinux"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

#if defined(__uxp__)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -D__uxp__"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

#if defined(__sxg__)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -D__sxg__"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

/* TODO: should I add -DUSG here? */

#if defined(nec_ews_svr2)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -Dnec_ews_svr2"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

#if defined(AMOEBA)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -DAMOEBA"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#   if defined(CROSS_COMPILE)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -DCROSS_COMPILE"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#       if defined(CROSS_i80386)
#           undef CC_FLAGS
#           define CC_FLAGS DEFAULT_CC_FLAGS " -Di80386"
#           undef DEFAULT_CC_FLAGS
#           define DEFAULT_CC_FLAGS CC_FLAGS
#       endif
#       if defined(CROSS_sparc)
#           undef CC_FLAGS
#           define CC_FLAGS DEFAULT_CC_FLAGS " -Dsparc"
#           undef DEFAULT_CC_FLAGS
#           define DEFAULT_CC_FLAGS CC_FLAGS
#       endif
#       if defined(CROSS_mc68000)
#           undef CC_FLAGS
#           define CC_FLAGS DEFAULT_CC_FLAGS " -Dmc68000"
#           undef DEFAULT_CC_FLAGS
#           define DEFAULT_CC_FLAGS CC_FLAGS
#       endif
#   else
#       if defined(i80386)
#           undef CC_FLAGS
#           define CC_FLAGS DEFAULT_CC_FLAGS " -Di80386"
#           undef DEFAULT_CC_FLAGS
#           define DEFAULT_CC_FLAGS CC_FLAGS
#       endif
#       if defined(sparc)
#           undef CC_FLAGS
#           define CC_FLAGS DEFAULT_CC_FLAGS " -Dsparc"
#           undef DEFAULT_CC_FLAGS
#           define DEFAULT_CC_FLAGS CC_FLAGS
#       endif
#       if defined(mc68000)
#           undef CC_FLAGS
#           define CC_FLAGS DEFAULT_CC_FLAGS " -Dmc68000"
#           undef DEFAULT_CC_FLAGS
#           define DEFAULT_CC_FLAGS CC_FLAGS
#       endif
#   endif /* defined(CROSS_COMPILE) */
#endif /* defined(AMOEBA) */

#if defined(__minix_vmd)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -Dminix"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

#if defined(__EMX__)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -Demxos2"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

#if defined(__QNX__) && !defined(__QNXNTO__)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -D__QNX__"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#endif

#if defined(__QNXNTO__)
#   undef CC_FLAGS
#   define CC_FLAGS DEFAULT_CC_FLAGS " -D__QNXNTO__"
#   undef DEFAULT_CC_FLAGS
#   define DEFAULT_CC_FLAGS CC_FLAGS
#   if defined(i386)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -Di386"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#   if defined(PPC)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -DPPC"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#   if defined(MIPS)
#       undef CC_FLAGS
#       define CC_FLAGS DEFAULT_CC_FLAGS " -DMIPS"
#       undef DEFAULT_CC_FLAGS
#       define DEFAULT_CC_FLAGS CC_FLAGS
#   endif
#endif

/* expects DEFAULT_CC or "cc" in PATH. */

#if defined(DEFAULT_CC)
#   define DEFAULT_CC_COMMAND DEFAULT_CC
#else
#   define DEFAULT_CC_COMMAND "cc"
#endif

#if defined(DEFAULT_CPP)
#   define DEFAULT_CPP_COMMAND DEFAULT_CPP
#elif defined(USE_CC_E)
#   define DEFAULT_CPP_COMMAND CC_COMMAND " -E"
#else
#   define DEFAULT_CPP_COMMAND "cpp"
#endif

#endif /* RESURRECTION_DEFS_H */

