/*
 * Resurrection/system.h - portability header for (Unix-like) systems.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 *
 * NOTE: for the most part, this header file doesn't rely on GNU Autoconf and
 * related tools to be available. Try to keep it that way, please. :)
 */

#ifndef RESURRECTION_SYSTEM_H
#define RESURRECTION_SYSTEM_H

/* X11 headers. */

#if (HAVE_X11_XOS_R_H)
#   include <X11/Xos_r.h>
#elif defined(HAVE_X11_XOS_H)
#   include <X11/Xos.h>
#endif

/*
 * ANSI C header file for system-dependent limits. Should also include header
 * files defining the limits specified by the standards the system conforms to.
 * It's good to include <limits.h> before <sys/param.h>, because <sys/param.h>
 * may depend on the limits defined by <limits.h>.
 */
#if !defined(NO_LIMITS_H)
#   include <limits.h>
#endif

/*
 * Apparently, <sys/types.h> needs to be included before <unistd.h> on some
 * systems (Minix).
 *
 * FIXME: this header file is probably available on a wider variety of
 * platforms. Not sure about the versions of BSD and XPG it first appeared in.
 */

#if defined(_CRAY)
#   define word word_t
#endif

#if (HAVE_SYS_TYPES_H) || defined(_POSIX_VERSION) || (_XOPEN_VERSION >= 3) || (defined(USG) && !defined(__TYPES__)) || defined(BSD) || defined(MINIX) || defined(MOTOROLA) || defined(AMOEBA) || defined(_SEQUENT_)
#   if defined(_POSIX_SOURCE) && (defined(MOTOROLA) || defined(AMOEBA))
#       undef _POSIX_SOURCE
#       include <sys/types.h>
#       define _POSIX_SOURCE 1
#   else
#       include <sys/types.h>
#   endif
#endif

/*
 * <sys/param.h> is available on many systems. At least some BSD systems define
 * BSD and related macros here. Apparently, Sony systems define NEWSOS here, so
 * it needs to be included before <Resurrection/defs.h>.
 */
#if ((HAVE_SYS_PARAM_H) || defined(SYSV) || defined(CSRG_BASED) || defined(__NetBSD__) || defined(DGUX) || defined(sony) || defined(__EMX__) || defined(__QNX__) || defined(__MVS__)) && !defined(MINIX) && !defined(WIN32) && !defined(Lynx) && !defined(__GNU__)
#   include <sys/param.h>
#endif

#include <Resurrection/defs.h>
#include <Resurrection/standard.h>
#include <Resurrection/os.h>
#include <Resurrection/thread.h>

/* ANSI C headers. */

/*
 * ANSI C header file for standard I/O. Defines _NFILE on some systems.
 */
#if !defined(NO_STDIO_H)
#   include <stdio.h>
#endif

/*
 * ANSI C header file for utility functions. Usually gets the value for NULL,
 * most likely by including <stddef.h>.
 */
#if !defined(NO_STDLIB_H)
#   include <stdlib.h>
#endif

/*
 * ANSI C header file for errno.
 */
#if !defined(NO_ERRNO_H)
#   include <errno.h>
#endif

/*
 * ANSI C header file for setjmp/longjmp.
 */
#if (HAVE_SETJMP_H) || defined(__STDC__) || (USE_POSIX)
#   include <setjmp.h>
#endif

/*
 * NOTE: signal.h needs to be included before <X11/Xos.h> / <X11/Xos_r.h>.
 * signal.h is an ANSI C header file.
 */
#if defined(linux) && !(USE_POSIX_1)
#   include <bsd/signal.h>
#else
#   include <signal.h>
#endif

/*
 * POSIX header file for open(), fcntl(), etc.
 */
#if !defined(NO_FCNTL_H)
#   include <fcntl.h>
#endif

/*
 * Header files and macros for readdir().
 */
#if (HAVE_DIRENT_H) || defined(_POSIX_VERSION) || (USE_SVR2) || (USE_BSD_43)
#   include <dirent.h>
#   if !defined(d_ino)
#       define d_ino d_fileno
#   endif
#   if !defined(D_NAMLEN)
#       define D_NAMLEN(d) (strlen((d)->d_name))
#   endif
#else
#   if (HAVE_SYS_NDIR_H)
#       include <sys/ndir.h>
#   elif (HAVE_SYS_DIR_H)
#       include <sys/dir.h>
#   elif (HAVE_NDIR_H)
#       include <ndir.h>
#   endif
#   if !defined(d_fileno)
#       define d_fileno d_ino
#   endif
#   if !defined(dirent)
#       define dirent direct
#   endif
#   if !defined(D_NAMLEN)
#       define D_NAMLEN(d) ((d)->d_namlen)
#   endif
#endif

/*
 * Header file for stat() and related functions.
 */
#if !defined(NO_SYS_STAT_H)
#   include <sys/stat.h>
#endif

/*
 * Header file for wait() and related functions.
 */
#if !defined(NO_SYS_WAIT_H)
#   include <sys/wait.h>
#endif

/*
 * System information.
 */
#if defined(linux)
#   include <linux/version.h>
//#   include <asm/page.h>
//#   include <asm/pgtable.h>
#endif

#if (HAVE_SYS_UTSNAME_H) || (USE_POSIX_1) || (USE_SVR4) || (USE_XPG) || defined(__hpux) || defined(_SEQUENT_)
#   include <sys/utsname.h>
#endif

/*
 * FIXME: could test for the headers separately.
 */
#if defined(__QNX__) && !defined(__QNXNTO__)
#   include <sys/proc_msg.h>
#   include <sys/kernel.h>
#endif

/*
 * FIXME: this might be available on earlier versions of GNU libc as well.
 * Other systems too.
 */
#if (HAVE_SYS_SYSMACROS_H) || (USE_SVR3) || defined(__sgi) || (__GLIBC__ >= 2)
#   include <sys/sysmacros.h>
#endif

/*
 * getrlimit() should be available on BSD systems from at least 4.3BSD.
 *
 * FIXME: getrlimit() might also be available on older GNU libc and other
 * systems.
 *
 * FIXME: find out if <sys/vlimit.h> is correct.
 *
 * NOTE: RLIMIT_OFILE is the BSD name for RLIMIT_NOFILE.
 */

#if ((HAVE_SYS_RESOURCE_H) || (HAVE_RESOURCE_H) || (USE_SVR4) || (USE_BSD_42) || (__GLIBC__ >= 2) || defined(macII) || defined(Lynx)) && !defined(AMOEBA) && !defined(MINIX) && !defined(WIN32) && !defined(__QNX__)
#   if (HAVE_RESOURCE_H) || defined(Lynx)
#       include <resource.h>
#   else
#       include <sys/resource.h>
#   endif
#   define USE_GETRLIMIT 1
#   define USE_SETRLIMIT 1
#   if !defined(RLIMIT_NOFILE)
#       define RLIMIT_NOFILE RLIMIT_OFILE
#   endif /* !defined(RLIMIT_NOFILE) */
#   if !defined(RLIMIT_OFILE)
#       define RLIMIT_OFILE RLIMIT_NOFILE
#   endif
#elif (USE_SVR2)
#   include <ulimit.h>
#   define USE_ULIMIT 1
#else
#   include <sys/vlimit.h>
#   define USE_VLIMIT 1
#endif

/*
 * NOTE: on Linux systems, <sys/acct.h> defines ACCT_COM, so it needs to be
 * included before the <sys/param.h> macros below.
 *
 * FIXME: add more systems...?
 */
#if (HAVE_SYS_ACCT_H) || (USE_SVR4) || (USE_BSD_43) || defined(linux)
#   include <sys/acct.h>
#endif

/*
 * user and group management.
 */

#if !defined(NO_PWD_H)
#   include <pwd.h>
#endif

#if !defined(NO_GRP_H)
#   include <grp.h>
#endif

/*
 * File I/O.
 */

#if (HAVE_IOCTL_H) || defined(__QNX__)
#   include <ioctl.h>
#endif

/* 
 * <sys/ioctl.h> causes problems on some (some) Sun systems when <termios.h> is
 * also included.
 */

#if ((HAVE_SYS_IOCTL_H) || !defined(__CYGWIN__)) && !defined(sun)
#   include <sys/ioctl.h>
#endif

#if (HAVE_SYS_FILIO_H) || defined(sun)
#   include <sys/filio.h>
#endif

/*
 * select() support.
 */
#include <Resurrection/select.h>

/*
 * poll() support.
 */

/*
 * FIXME: poll() has been available in GNU libc since 5.4.28. It may also be
 * available on earlier XPG and System V  systems. Not sure about the versions
 * of GNU libc that have poll(). Also not sure if XPG4 has <poll.h> or
 * <sys/poll.h>.
 */
#if (HAVE_POLL_H) || (USE_SVR3)
#   include <poll.h>
#elif (HAVE_SYS_POLL_H) || defined(_POSIX_POLL) || (USE_XPG4) || defined(__GLIBC__)
#   include <sys/poll.h>
#endif

/*
 * System V STREAMS support.
 */
#if (USE_SVR3)
#   include <sys/stream.h>
#   include <sys/stropts.h>
#   if (HAVE_SYS_PTEM_H) || !(USE_SVR4) || defined(SCO325)
#       include <sys/ptem.h>
#   endif /* (HAVE_SYS_PTEM_H) || !(USE_SVR4) || defined(SCO325) */
#endif /* (USE_SVR3) */

/*
 * Socket I/0.
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/*
 * Pseudo-terminal device control.
 */

/*
 * FIXME: not sure about __CYGWIN__ and DGUX. Should be correct for sun from
 * SunOS 4 up.
 */
#if (USE_POSIX_1) || defined(__QNX__) || defined(__FreeBSD__)
#   define USE_POSIX_TERMIOS 1
#elif (USE_SVR4) || defined(linux) || defined(CSRG_BASED) || defined(AMOEBA) || defined(MINIX) || defined(__CYGWIN__) || defined(DGUX) || defined(sun)
#   define USE_TERMIOS 1
#elif (USE_SVR3) || defined(Lynx) || defined(__EMX__) || defined(macII) || defined(__hpux) || (defined(ISC) && !(USE_SVR3))
#   define USE_SYSV_TERMIO 1
#endif

#if (HAVE_SYS_TTYCHARS_H) || defined(BDS4_3) || defined(macII)
#   include <sys/ttychars.h>
#endif
#if defined(HAVE_SYS_TTYCOM_H)
#   include <sys/ttycom.h>
#endif

#if (USE_POSIX_TERMIOS) || (USE_TERMIOS)
#if 0
#   if (HAVE_SYS_TERMIOS_H) || defined(MINIX)
#       include <sys/termios.h>
#   elif defined(AMOEBA)
#       include <posix/termios.h>
#   else
#       include <termios.h>
#   endif
#endif
#   if (HAVE_TERMIOS_H)
#       include <termios.h>
#   elif (HAVE_SYS_TERMIOS_H) || defined(MINIX)
#       include <sys/termios.h>
#   elif defined(AMOEBA)
#       include <posix/termios.h>
#   endif
#elif (USE_SYSV_TERMIO)
#   if (HAVE_TERMIO_H) || defined(Lynx)
#       include <termio.h>
#   elif (HAVE_SYS_TERMIO_H) || (USE_SVR3)
#       include <sys/termio.h>
#   elif defined(sun) && !(USE_SVR4)
#       include <sys/ttycom.h>
#   endif
#else
#if 0
#   if (HAVE_SYS_TTYCHARS_H) || defined(BDS4_3) || defined(macII)
#       include <sys/ttychars.h>
#   endif
#endif
#   include <sgtty.h>
#endif

#if defined(macII)
#   include <sys/ttychars.h>
/* FIXME: is this really needed? I thought macII is (USE_SYSV_TERMIO) */
#   include <sgtty.h>
#endif

#if (__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 1))
#   include <pty.h>
#endif

#if defined(__hpux)
#   include <sys/ptyio.h>
#   include <sys/bsdtty.h>
#endif /* defined(__hpux) */

#if defined(PUCC_PTYD)
#   include <local/openpty.h>
#endif

/*
 * FIXME: this must have appeared in BSDs earlier, because the file on my Linux
 * system dates back to 1989...
 */
#if defined(CSRG_BASED)
#   include <ttyent.h>
#endif

/* Solaris defines SRIOCSREDIR in <sys/strredir.h>. */
#if defined(sun)
#   include <sys/strredir.h>
#endif

#include <Resurrection/memory.h>

/* IPC */
#include <sys/ipc.h>
#include <sys/shm.h>

/*
 * User logging.
 */
/*
 * FIXME: I'm not sure if <utmp.h> should be included for Lynx. xterm does,
 * though.
 */

#if ((USE_SVR4) || defined(SCO325)) && !defined(__CYGWIN__) && !defined(__hpux) && !defined(_AIX) && !((__GLIBC__ >= 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 1)))
#   define USE_UTMPX 1
#endif

#if (USE_UTMPX)
#   include <utmpx.h>
#else /* !USE_UTMPX */
#   if defined(Lynx)
#       undef UTMP
#   endif
#   include <utmp.h>
#endif /* USE_UTMPX */

/*
 * NOTE: this should be correct on BSD systems from 4.2 up.
 *
 * FIXME: not sure about the versions of linux, HP-UX, AIX, and Sun systems
 * this is correct for.
 */
//#if defined(linux) && (LINUX_VERSION_CODE >= KERNEL_VERSION(2,1,44))
#if defined(linux)
#   undef USE_SAVED_IDS
#   define USE_SAVED_IDS 1
#   define USE_SETRESUID 1
#   define USE_SETRESGID 1
//#elif ((USE_SVR4) || (defined(linux) && (LINUX_VERSION_CODE >= KERNEL_VERSION(1,1,37))) && || (BSD >= 199103)) && !defined(__CYGWIN__)
#elif ((USE_SVR4) || (BSD >= 199103)) && !defined(__CYGWIN__)
#   undef USE_SAVED_IDS
#   define USE_SAVED_IDS 1
#   define USE_SETEUID   1
#   define USE_SETEGID   1
#elif defined(_hpux) || defined(_AIX) || defined(sun)
#   define USE_SAVED_IDS 1
#endif

#if (HAVE_GETGRNAM) || (USE_POSIX_1) || (USE_SVR4) || (USE_BSD_43)
#   define USE_GETGRNAM 1
#endif

/*
 * File I/O.
 */

/*
 * FIXME: Not sure which System V versions this is correct for.
 *
 * On (some versions of) System V, read() on a nonblocking device returns zero
 * if no data is available. Normally, read() returns zero on EOF.
 */

#if (USE_SVR3) && defined(O_NDELAY) && !(USE_POSIX_1)
#   define NONBLOCKING_READ_RETURNS_ZERO 1
#endif

/*
 * POSIX macros for standard I/O file descriptors.
 */

#if !defined(STDIN_FILENO)
#   define STDIN_FILENO 0
#endif

#if !defined(STDOUT_FILENO)
#   define STDOUT_FILENO 1
#endif

#if !defined(STDERR_FILENO)
#   define STDERR_FILENO 2
#endif

/*
 * NOTE: see sysdeps/unix/bsd/sun/sunos4/bits/fcntl.h in glibc.
 *
 * NOTE: see sysdeps/mach/hurd/bits/fcntl.h in glibc.
 *
 * NOTE: see sysdeps/unix/bsd/bsd4.4/bits/fcntl.h.
 *
 * NOTE: sysdeps/unix/bsd/ultrix4/bits/fcntl.h has new flags.
 *
 * FIXME: is O_IGNORE_CTTY used correctly?
 */

/*
 * POSIX file modes.
 */

#if !defined(O_RDONLY)
#   if defined(FREAD)
#       define O_RDONLY FREAD
#   endif
#endif

#if !defined(O_WRONLY)
#   if defined(FWRITE)
#       define O_WRONLY FWRITE
#   endif
#endif

#if !defined(O_RDWR)
#   if defined(FREAD) && defined(FWRITE)
#       define O_RDWR (FREAD | FWRITE)
#   endif
#endif

#if !defined(O_ACCMODE)
#    define O_ACCMODE (O_RDONLY | O_WRONLY | O_RDWR)
#endif

#if !defined(O_CREAT)
#   if defined(FCREAT)
#       define O_CREAT FCREAT
#   endif
#endif

#if !defined(O_EXCL)
#   if defined(FEXCL)
#       define O_EXCL FEXCL
#   endif
#endif

#if !defined(O_TRUNC)
#   if defined(FTRUNC)
#       define O_TRUNC FTRUNC
#   endif
#endif

#if !defined(O_NOCTTY)
#   if defined(FNOCTTY)
#       define O_NOCTTY FNOCTTY
#   elif defined(O_IGNORE_CTTY)
#       define O_NOCTTY O_IGNORE_CTTY
#   endif
#endif

#if !defined(O_APPEND)
#   if defined(FAPPEND)
#       define O_APPEND FAPPEND
#   endif
#endif

/*
 * NOTE: FNBIO and FNONBIO are from sysdeps/unix/bsd/sun/sunos4/bits/fcntl.h
 * in GNU libc 2.1.3. FNBLOCK is from sysdeps/unix/bsd/ultrix4/bits/fcntl.h.
 */

#if !defined(O_NONBLOCK)
#   if defined(FNONBLOCK)
#       define O_NONBLOCK FNONBLOCK
#   elif defined(FNONBIO)
#       define O_NONBLOCK FNONBIO
#   elif defined(FNBLOCK)
#       define O_NONBLOCK FNBLOCK
#   elif defined(O_NDELAY)
#       define O_NONBLOCK O_NDELAY
#   elif defined(FNDELAY)
#       define O_NONBLOCK FNDELAY
#   elif defined(FNBIO)
#       define O_NONBLOCK FNBIO
#   endif
#endif

/*
 * Unix file modes.
 */

#if !defined(FREAD)
#   define FREAD O_RDONLY
#endif

#if !defined(FWRITE)
#   define FWRITE O_WRONLY
#endif

#if !defined(FCREAT)
#   define FCREAT O_CREAT
#endif

#if !defined(FEXCL)
#   define FEXCL O_EXCL
#endif

#if !defined(FTRUNC)
#   define FTRUNC O_TRUNC
#endif

#if !defined(FNOCTTY)
#   define FNOCTTY O_NOCTTY
#endif

#if !defined(FAPPEND)
#   define FAPPEND O_APPEND
#endif

#if !defined(FNONBLOCK)
#   define FNONBLOCK O_NONBLOCK
#endif

#if !defined(FNONBIO)
#   define FNONBIO O_NONBLOCK
#endif

#if !defined(FNBLOCK)
#   define FNBLOCK O_NONBLOCK
#endif

/*
 * Unix file modes that may not be available on a POSIX system.
 *
 * NOTE: FSYNCRON is from sysdeps/unix/bsd/ultrix4/bits/fcntl.h in GNU libc
 * 2.1.3.
 */

#if !defined(O_ASYNC)
#   if defined(FASYNC)
#       define O_ASYNC FASYNC
#   endif
#endif

#if !defined(O_FSYNC)
#   if defined(FFSYNC)
#       define O_FSYNC FFSYNC
#   elif defined(FSYNCRON)
#       define O_FSYNC FSYNCRON
#   endif
#endif

#if !defined(O_SYNC)
#   if defined(FSYNC)
#       define O_SYNC FSYNC
#   endif
#endif

/*
 * TODO: write an Ropen() macro or function that handles these:
 *
 * O_SHLOCK: open with shared file lock.
 * O_EXLOCK: open with shared exclusive lock.
 */

#if !defined(O_SHLOCK)
#   define O_SHLOCK 0x0000
#endif

#if !defined(O_EXLOCK)
#   define O_EXLOCK 0x0000
#endif

#if !defined(O_NDELAY)
#   define O_NDELAY O_NONBLOCK
#endif

#if !defined(FASYNC)
#   if defined(O_ASYNC)
#       define FASYNC O_ASYNC
#   endif
#endif

#if !defined(FFSYNC)
#   if defined(O_FSYNC)
#       define FFSYNC O_FSYNC
#   endif
#endif

#if !defined(FSYNCRON)
#   if defined(O_FSYNC)
#       define FSYNCRON O_FSYNC
#   endif
#endif

#if !defined(FSYNC)
#   if defined(O_SYNC)
#       define FSYNC O_SYNC
#   endif
#endif

#if !defined(FNDELAY)
#   define FNDELAY O_NDELAY /* BSD O_NDELAY. */
#endif

#if !defined(FNBIO)
#   define FNBIO O_NDELAY /* System V O_NDELAY. */
#endif

/*
 * POSIX.1b-1993 / Unix98 file modes.
 */

#if !defined(O_DSYNC)
#   if defined(O_SYNC)
#       define O_DSYNC O_SYNC
#   endif
#endif

#if !defined(O_RSYNC)
#   if defined(O_SYNC)
#       define O_RSYNC O_SYNC
#   endif
#endif

/*
 * GNU file modes.
 */

#if !defined(O_READ)
#   define O_READ O_RDONLY
#endif

#if !defined(O_WRITE)
#   define O_WRITE O_WRONLY
#endif

#if 0
#if !defined(O_EXEC)
#   define O_EXEC 0x0000
#endif
#endif

#if !defined(O_NORW)
#   define O_NORW 0x0000
#endif

#if !defined(O_IGNORE_CTTY)
#   define O_IGNORE_CTTY O_NOCTTY
#endif

/*
 * Large File Support file modes.
 */

#if !defined(O_LARGEFILE)
#   define O_LARGEFILE 0x0000
#endif

/*
 * POSIX file permissions.
 * See io/sys/stat.h in glibc.
 */

#if !defined(S_ISUID)
#   define S_ISUID 04000
#endif

#if !defined(S_ISGID)
#   define S_ISGID 02000
#endif

#if !defined(S_IRUSR)
#   define S_IRUSR 00400
#endif

#if !defined(S_IWUSR)
#   define S_IWUSR 00200
#endif

#if !defined(S_IXUSR)
#   define S_IXUSR 00100
#endif

#if !defined(S_IRWXU)
#   define S_IRWXU 00700
#endif

#if !defined(S_IRGRP)
#   define S_IRGRP 00040
#endif

#if !defined(S_IWGRP)
#   define S_IWGRP 00020
#endif

#if !defined(S_IXGRP)
#define S_IXGRP 00010
#endif

#if !defined(S_IRWXG)
#   define S_IRWXG 00070
#endif

#if !defined(S_IROTH)
#   define S_IROTH 00004
#endif

#if !defined(S_IWOTH)
#   define S_IWOTH 00002
#endif

#if !defined(S_IXOTH)
#   define S_IXOTH 00001
#endif

#if !defined(S_IRWXO)
#   define S_IRWXO 00007
#endif

/*
 * Unix file permissions.
 */

#if !defined(S_ISVTX)
#   define S_ISVTX 01000
#endif

#if !defined(S_IREAD)
#   define S_IREAD S_IRUSR
#endif

#if !defined(S_IWRITE)
#   define S_IWRITE S_IWUSR
#endif

#if !defined(S_IEXEC)
#   define S_IEXEC S_IXUSR
#endif

#if !defined(ACCESSPERMS)
#   define ACCESSPERMS (S_IRWXU | S_IRWXG | S_IRWXO)
#endif

#if !defined(ALLPERMS)
#   define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | ACCESSPERMS)
#endif

#if !defined(DEFFILEMODE)
#   define DEFFILEMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | \
                        S_IROTH | S_IWOTH)
#endif

/*
 * Unix block size.
 *
 * NOTE: DEV_BSIZE is defined above if not in <sys/param.h>.
 */

#if !defined(S_BLKSIZE)
#   define S_BLKSIZE DEV_BSIZE
#endif

/*
 * POSIX file types. These flag macros aren't defined by POSIX.1.
 */

#if !defined(S_IFMT)
#   if defined(__S_IFMT)
#       define S_IFMT __S_IFMT
#   else
#       define S_IFMT 00170000
#   endif
#endif

#if !defined(S_IFSOCK)
#   if defined(__S_IFSOCK)
#       define S_IFSOCK __S_IFSOCK
#   else
#       define S_IFSOCK 0140000
#   endif
#endif

#if !defined(S_IFLNK)
#   if defined(__S_IFLNK)
#       define S_IFLNK __S_IFLNK
#   else
#       define S_IFLNK 0120000
#   endif
#endif

#if !defined(S_IFREG)
#   if defined(__S_IFREG)
#       define S_IFREG __S_IFREG
#   else
#       define S_IFREG 0100000
#   endif
#endif

#if !defined(S_IFBLK)
#   if defined(__S_IFBLK)
#       define S_IFBLK __S_IFBLK
#   else
#       define S_IFBLK 0060000
#   endif
#endif

#if !defined(S_IFDIR)
#   if defined(__S_IFDIR)
#       define S_IFDIR __S_IFDIR
#   else
#       define S_IFDIR 0040000
#   endif
#endif

#if !defined(S_IFCHR)
#   if defined(__S_IFCHR)
#       define S_IFCHR __S_IFCHR
#   else
#       define S_IFCHR 0020000
#   endif
#endif

#if !defined(S_IFIFO)
#   if defined(__S_IFIFO)
#       define S_IFIFO __S_IFIFO
#   else
#       define S_IFIFO 0010000
#   endif
#endif

/*
 * Macro to check file type from the st_mode member of struct stat.
 */
#undef S_ISTYPE
#define S_ISTYPE(m,t) (((m) & S_IFMT) == (t))

/*
 * POSIX file type macros.
 */

#if !defined(S_ISSOCK)
#   define S_ISSOCK(m) S_ISTYPE((m), S_IFSOCK)
#endif

#if !defined(S_ISLNK)
#   define S_ISLNK(m) S_ISTYPE((m), S_IFLNK)
#endif

#if !defined(S_ISREG)
#   define S_ISREG(m) S_ISTYPE((m), S_IFREG)
#endif

#if !defined(S_ISBLK)
#   define S_ISBLK(m) S_ISTYPE((m), S_IFBLK)
#endif

#if !defined(S_ISDIR)
#   define S_ISDIR(m) S_ISTYPE((m), S_IFDIR)
#endif

#if !defined(S_ISCHR)
#   define S_ISCHR(m) S_ISTYPE((m), S_IFCHR)
#endif

#if !defined(S_ISFIFO)
#   define S_ISFIFO(m) S_ISTYPE((m), S_IFIFO)
#endif

/*
 * POSIX values for the last argument of lseek().
 */

#if !defined(SEEK_SET)
#   if defined(L_SET)
#       define SEEK_SET L_SET
#   endif
#endif

#if !defined(SEEK_CUR)
#   if defined(L_INCR)
#       define SEEK_CUR L_INCR
#   endif
#endif

#if !defined(SEEK_END)
#   if defined(L_XTND)
#       define SEEK_END L_XTND
#   endif
#endif

/*
 * BSD Unix values for the last argument of lseek().
 */

#if !defined(L_SET)
#   define L_SET SEEK_SET
#endif

#if !defined(L_INCR)
#   define L_INCR SEEK_CUR
#endif

#if !defined(L_XTND)
#   define L_XTND SEEK_END
#endif

/*
 * Terminal interface.
 */

#if (USE_POSIX_1) || (USE_SVR4) || defined(__QNX__)
#   define USE_TCSETPGRP 1
#endif

/* FIXME: not sure how good this is. */
#if (USE_SVR3) || defined(linux) || defined(__GNU__)
#   define PTY_OWNER_GROUP "tty"
#elif (USE_BSD)
#   define PTY_OWNER_GROUP "wheel"
#elif defined(__CYGWIN__) || defined(__EMX__)
#   undef PTY_OWNER_GROUP
#endif

#if defined(TIOCSLTC) && !defined(__GNU__) && !defined(__QNX__) && !defined(linux) && !defined(Lynx) && !(USE_SVR4)
#   define USE_LTCHARS 1
#endif

#if defined(TIOCLSET) && !defined(__GNU__) && !defined(__QNX__) && !defined(__sgi) && !(defined(linux) && defined(__sparc__))
#   define USE_SYSV_TERMIO_WITH_LOCAL 1
#endif

#define PTY_NAME_LENGTH (PATH_MAX + 1)

#define PTY_SLAVE_MODE (S_IRUSR | S_IWUSR | S_IWGRP)

#define DEV_TTY_DEVICE "/dev/tty"

#define DEV_PTMX_DEVICE "/dev/ptmx"
#define DEV_PTC_DEVICE "/dev/ptc"
#define DEV_PTYM_CLONE_DEVICE "/dev/ptym/clone"

#if defined(__CYGWIN__) && !defined(TIOCSPGRP)
#   define TIOCSPGRP (_IOW('t', 118, pid_t))
#endif

#if defined(ISC)
#   define TIOCGPGRP TCGETPGRP
#   define TIOCSPGRP TCSETPGRP
#endif

/*
 * NOTE: TAB3 is an obsolete name for XTABS.
 */

#if !defined(XTABS)
#   if defined(TAB3)
#       define XTABS TAB3
#   endif
#endif

#if !defined(TAB3)
#   if defined(XTABS)
#       define TAB3 XTABS
#   endif
#endif

/* NOTE: USE_USG_PTYS used to have (defined(ATT) && !defined(__sgi)), but ATT
 * is undefined for __sgi SVR4 systems above... This may be incorrect for some
 * systems, though.
 */

#if !defined(AMOEBA)
#   if defined(PUCC_PTYD)
#       define USE_OPENRPTY 1
#   elif ((USE_SVR3) && defined(i386) && !(USE_SVR4)) || defined(__QNXNTO__)
#       define USE_GENERIC_PTYS 1
#   elif defined(__FreeBSD__) || defined(ATT) || (__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 1))
#       define USE_USG_PTYS 1
#       if (__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 1))
#           define USE_GETPT 1
#       elif defined(__FreeBSD__)
#           define USE_POSIX_OPENPT 1
#       endif
#   elif defined(__osf__) || (defined(__GLIBC__) && !(USE_USG_PTYS))
#       define USE_OPENPTY 1
#   elif defined(AIXV3)
#       define USE_AIXV3_PTYS 1
#   elif defined(__sgi) && (OSMAJORVERSION >= 4)
#       define USE_SGI_4_PTYS 1
#   elif defined(__convex__)
#       define USE_CONVEX_PTYS 1
#   elif defined(sequent)
#       define USE_GETPSEUDOTTY 1
#   elif (defined(__sgi) && (OSMAJORVERSION < 4)) || (defined(umips) && defined(SYSTYPE_SYSV))
#       define USE_DEV_PTC_PTYS 1
#   elif defined(__hpux)
#       define USE_DEV_PTYM_CLONE_PTYS 1
#   elif defined(_SCO_DS)
#       define USE_SCO_PTYS 1
#   else
#       define USE_GENERIC_PTYS 1
#   endif
#endif /* !defined(AMOEBA) */

#if (USE_SCO_PTYS)
#   define SCO_PTY_DEVICES 256
#endif /* USE_USG_PTYS */

#if defined(__hpux)
#   define PTY_MASTER_PREFIX "/dev/ptym/ptyxx"
#elif !defined(__osf__)
#   define PTY_MASTER_PREFIX "/dev/ptyxx"
#else
#   define PTY_MASTER_PREFIX ""
#endif /* defined(__hpux) */

#if defined(__hpux)
#   define PTY_SLAVE_PREFIX "/dev/pty/ttyxx"
#elif defined(__osf__)
#   define PTY_SLAVE_PREFIX "/dev/ttydirs/xxx/xxxxxxxxxxxxxx"
#else
#   define PTY_SLAVE_PREFIX "/dev/ttyxx"
#endif /* defined(__hpux) */

#if defined(__hpux)
#   define PTY_CHARS_1 "zyxwvutsrqp"
#elif defined(__EMX__)
#   define PTY_CHARS_1 "pq"
#else
#   define PTY_CHARS_1 "pqrstuvwxyzPQRSTUVWXYZ"
#endif /* defined(__hpux) */

#if defined(__hpux)
#   define PTY_CHARS_2 "fedcba9876543210"
#elif defined(__FreeBSD__)
#   define PTY_CHARS_2 "0123456789abcdefghijklmnopqrstuv"
#else
#   define PTY_CHARS_2 "0123456789abcdef"
#endif /* defined(__hpux) */

#if defined(CRAY)
#   define PTY_MASTER_FORMAT "/dev/pty/%03d"
#else
#   define PTY_MASTER_FORMAT "/dev/ptyp%d"
#endif

#if defined(CRAY)
#   define PTY_SLAVE_FORMAT "/dev/ttyp%03d"
#else
#   define PTY_SLAVE_FORMAT "/dev/ttyp%d"
#endif /* defined(CRAY) */

#if defined(CRAY)
#   define MAX_PTYS 256
#else
#   define MAX_PTYS 2048
#endif

#if defined(linux)
#   if defined(PTY_BUF_SIZE)
#       define PTY_MAX_WRITE PTY_BUF_SIZE
#   endif
#endif

#if !defined(PTY_MAX_WRITE)
#   if (USE_POSIX_1)
#       if defined(_POSIX_MAX_INPUT)
#           define PTY_MAX_WRITE _POSIX_MAX_INPUT
#       else
#           define PTY_MAX_WRITE 255 /* POSIX minimum MAX_INPUT */
#       endif
#   endif
#endif

#if !defined(PTY_MAX_WRITE)
#   define PTY_MAX_WRITE 128
#endif

/*
 * NOTE: values over B38400 are not defined by POSIX.1. Some systems (BSD)
 * define some of the values CBAUD, CBAUDEX, EXTA, or EXTB.
 */

#if defined(B57600)
#   define PTY_BAUDRATE B57600
#elif defined(B38400)
#   define PTY_BAUDRATE B38400
#elif defined(B19200)
#   define PTY_BAUDRATE B19200
#else
#   define PTY_BAUDRATE B9600
#endif

/* BSD Unix names for baud rates. */

#if !defined(EXTA)
#   if defined(B19200)
#       define EXTA B19200
#   endif
#endif

#if !defined(EXTB)
#   if defined(B38400)
#       define EXTB B38400
#   endif
#endif

/* disable special character functions. */

#if (USE_SYSV_TERMIO)
#   if defined(CNUL)
#       define VDISABLE CNUL
#   else
#       define VDISABLE '\0'
#   endif
#elif defined(_POSIX_VDISABLE)
#   define VDISABLE _POSIX_VDISABLE
#else
#   define VDISABLE 255
#endif

/* FIXME: not sure if this is correct. */

#if !defined(VSWTC)
#   if defined(VSWTCH)
#       define VSWTC VSWTCH
#   endif
#endif

#undef CTRL
#define CTRL(c) ((c) & 0x1f)

/* terminal control characters. */

#if !defined(CINTR)
#   define CINTR      CTRL('c')
#endif
#if !defined(CQUIT)
#   define CQUIT      CTRL('\\')
#endif
#if !defined(CERASE)
#   if defined(linux)
#       define CERASE CTRL('?')
#   else
#       define CERASE CTRL('h')
#   endif
#endif
#if !defined(CKILL)
#   define CKILL      CTRL('u')
#endif
#if !defined(CEOF)
#   define CEOF       CTRL('d')
#endif
#if !defined(CSTART)
#   define CSTART     CTRL('q')
#endif
#if !defined(CSTOP)
#   define CSTOP      CTRL('s')
#endif
#if !defined(CSUSP)
#   define CSUSP      CTRL('z')
#endif
#if !defined(CDSUSP)
#   define CDSUSP     CTRL('y')
#endif
#if !defined(CRPRNT)
#   define CRPRNT     CTRL('r')
#endif
#if !defined(CFLUSH)
#   define CFLUSH     CTRL('o')
#endif
#if !defined(CWERASE)
#   define CWERASE    CTRL('w')
#endif
#if !defined(CLNEXT)
#   define CLNEXT     CTRL('v')
#endif
#if !defined(CSTATUS)
#   define CSTATUS    CTRL('t')
#endif

#if !defined(VDISCARD)
#   if defined(VDISCRD)
#       define VDISCARD VDISCRD
#   endif
#endif

#if !defined(VWERASE)
#   if defined(VWERSE)
#       define VWERASE VWERSE
#   endif
#endif

/*
 * remap or define non-existing termios flags.
 *
 * FIXME: do this on any system...
 */

#if defined(MINIX)
#   if !defined(OCRNL)
#       define OCRNL '\0'
#   endif
#   if !defined(ONLRET)
#       define ONLRET '\0'
#   endif
#   if !defined(NLDLY)
#       define NLDLY '\0'
#   endif
#   if !defined(CRDLY)
#       define CRDLY '\0'
#   endif
#   if !defined(TABDLY)
#       define TABDLY '\0'
#   endif
#   if !defined(BSDLY)
#       define BSDLY '\0'
#   endif
#   if !defined(VTDLY)
#       define VTDLY '\0'
#   endif
#   if !defined(FFDLY)
#       define FFDLY '\0'
#   endif
#endif /* defined(MINIX) */

#if defined(Lynx)
#   if !defined(BSDLY)
#       define BSDLY '\0'
#   endif
#   if !defined(VTDLY)
#       define VTDLY '\0'
#   endif
#   if !defined(FFDLY)
#       define FFDLY '\0'
#   endif
#endif

#if defined(__EMX__)
extern int ptioctl(int fd, int func, void *data);

#   define PTY_IOCTL ptioctl

#   define TIOCSWINSZ 113
#   define TIOCGWINSZ 117

struct winsize {
    unsigned short ws_row;         /* rows, in characters */
    unsigned short ws_col;         /* columns, in characters */
    unsigned short ws_xpixel;      /* horizontal size, pixels */
    unsigned short ws_ypixel;      /* vertical size, pixels */
};
#else /* !defined(__EMX__) */
#   define PTY_IOCTL ioctl
#endif /* defined(__EMX__) */

#if (USE_OPENRPTY)
#   define R_OPEN_PTY_MASTER(m,s) R_open_pty_master_openrpty(m, s)
#elif (USE_GENERIC_PTYS)
#   define R_OPEN_PTY_MASTER(m,s) R_open_pty_master_generic(m, s)
#elif (USE_USG_PTYS)
#   define R_OPEN_PTY_MASTER(m,s) R_open_pty_master_usg(m, s)
#elif (USE_OPENPTY)
#   define R_OPEN_PTY_MASTER(m,s) R_open_pty_master_openpty(m, s)
#elif (USE_AIXV3_PTYS)
#   define R_OPEN_PTY_MASTER(m,s) R_open_pty_master_aixv3(m, s)
#elif (USE_SGI_4_PTYS)
#   define R_OPEN_PTY_MASTER(m,s) R_open_pty_master_sgi_4(m, s)
#elif (USE_CONVEX_PTYS)
#   define R_OPEN_PTY_MASTER(m,s) R_open_pty_master_convex(m, s)
#elif (USE_GETPSEUDOTTY)
#   define R_OPEN_PTY_MASTER(m,s) R_open_pty_master_getpseudotty(m, s)
#elif (USE_DEV_PTC_PTYS)
#   define R_OPEN_PTY_MASTER(m,s) R_open_pty_master_dev_ptc(m, s)
#elif (USE_DEV_PTYM_CLONE_PTYS)
#   define R_OPEN_PTY_MASTER(m,s) R_open_pty_master_dev_ptym_clone(m, s)
#elif (USE_SCO_PTYS)
#   define R_OPEN_PTY_MASTER(m,s) R_open_pty_master_sco(m, s)
#endif

#if (USE_POSIX_TERMIOS) || (USE_TERMIOS)
#   define PTY_INIT_MODE(fd,p) pty_init_mode_termios((fd), (p))
#elif (USE_SYSV_TERMIO)
#   define PTY_INIT_MODE(fd,p) pty_init_mode_sysv_termio((fd), (p))
#else
#   define PTY_INIT_MODE(fd,p) pty_init_mode_sgtty((fd), (p))
#endif /* USE_TERMIOS */

#if (USE_TCSETPGRP)
#   define PTY_SET_FOREGROUND_PGRP_ID(pid) tcsetpgrp(0, (pid))
#elif defined(TIOCSPGRP)
#   define PTY_SET_FOREGROUND_PGRP_ID(pid) ioctl(0, TIOCSPGRP, &(pid))
#endif /* USE_TCSETPGRP */

#if (USE_USG_PTYS)

/* 
 * push STREAMS modules.
 * ptem     - pseudo-terminal hardware emulation.
 * ldterm   - standard terminal line discipline.
 * ttcompat - V7, 4BSD, and XENIX STREAMS compatibility.
 */

#   if defined(I_PUSH)
#       define PTY_INIT_SLAVE(u,fd) \
do { \
    if (ioctl((fd), I_PUSH, "ptem") < 0) { \
\
        return -1; \
    } \
\
#       if !(USE_SVR4) && !((USE_SVR3) && defined(i386)) \
    if (!getenv("CONSEM") && ioctl((fd), I_PUSH, "consem") < 0) { \
\
        return -1; \
    } \
#       endif /* !(USE_SVR4) && !((USE_SVR3) && defined(i386)) */ \
\
    if (ioctl((fd), I_PUSH, "ldterm") < 0) { \
\
        return -1; \
    }\
\
#       if (USE_SVR4) \
    if (ioctl((fd), I_PUSH, "ttcompat") < 0) { \
\
        return -1; \
    } \
#       endif /* (USE_SVR4) */ \
} while (FALSE)
#   else /* !defined(I_PUSH) */
#       define PTY_INIT_SLAVE(u,fd)
#   endif /* defined(I_PUSH) */

#else /* !(USE_USG_PTYS) */

#   if (USE_GETGRNAM) && defined(PTY_OWNER_GROUP)

#       define PTY_GET_GID(gid) \
do { \
    struct group *group = getgrnam(PTY_OWNER_GROUP); \
\
    if (group) { \
        (gid) = group->gr_gid; \
    } \
} while (FALSE)

#   else /* !((USE_GETGRNAM) && defined(PTY_OWNER_GROUP)) */

#       define PTY_GET_GID(gid)

#   endif /* (USE_GETGRNAM) && defined(PTY_OWNER_GROUP) */

#   if defined(__CYGWIN__) || defined(__EMX__)

#       define PTY_INIT_SLAVE_FD(u,fd,uid,gid)

#   else /* !(defined(__CYGWIN__) || defined(__EMX__)) */

#       define PTY_INIT_SLAVE_FD(u,fd,uid,gid) \
do { \
    user_enable_privileges((u)); \
\
    fchown((fd), (uid), (gid)); \
    fchmod((fd), PTY_SLAVE_MODE); \
\
    user_disable_privileges((u)); \
} while (FALSE)

#   endif /* defined(__CYGWIN__) || defined(__EMX__) */ \

/* set owner and permissions of pty slave device. */
#   define PTY_INIT_SLAVE(u,fd) \
do { \
    uid_t uid = getuid(); \
    gid_t gid = getgid(); \
\
    PTY_GET_GID(gid); \
\
    PTY_INIT_SLAVE_FD((u), (fd), uid, gid); \
} while (FALSE)

#endif /* (USE_USG_PTYS) */

#if defined(ultrix)

#   define PTY_DETACH_SLAVE(fd,pid,s) \
do { \
    if (((fd) = open(DEV_TTY_DEVICE, O_RDONLY)) >= 0) { \
        ioctl((fd), TIOCNOTTY, 0); \
\
        close((fd)); \
    } else { \
        (pid) = PROCESS_BECOME_GROUP_LEADER(); \
    } \
} while (FALSE)

#else /* !defined(ultrix) */

#   if defined(TIOCNOTTY) && !((__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 1)))
#       define PTY_DETACH_CONTROLLING_TERMINAL(s) \
do { \
    int ttyfd; \
\
    if (access(DEV_TTY_DEVICE, F_OK) < 0) { \
\
        return -1; \
    } \
\
    if ((ttyfd = open(DEV_TTY_DEVICE, O_RDWR)) >= 0) { \
        ioctl(ttyfd, TIOCNOTTY, NULL); \
\
	close(ttyfd); \
    } \
} while (FALSE)

#   else
#       define PTY_DETACH_CONTROLLING_TERMINAL(s)
#   endif

#   if defined(CSRG_BASED)
#       define PTY_UNLOCK_SLAVE(s) revoke((s))
#   else
#       define PTY_UNLOCK_SLAVE(s)
#   endif

#   if defined(CRAY) && defined(TCSETCTTY)
#       defined PTY_BECOME_CONTROLLING_TERMINAL(fd) ioctl((fd), TCSETCTTY, 0)
#   elif defined(TIOCSCTTY) && !defined(sun)
#       define PTY_BECOME_CONTROLLING_TERMINAL(fd) ioctl((fd), TIOCSCTTY, 0)
#   else /* !defined(TIOCSCTTY) */
#       define PTY_BECOME_CONTROLLING_TERMINAL(fd)
#   endif /* defined(TIOCSCTTY) */

#   define PTY_DETACH_SLAVE(fd,pid,s) \
do { \
    PTY_BECOME_CONTROLLING_TERMINAL((fd)); \
\
    PTY_SET_FOREGROUND_PGRP_ID((pid)); \
\
/* SVR4 problems: reports no tty, no job control. */ \
    close(open((s), O_RDWR, 0)); \
} while (FALSE)

#endif /* defined(ultrix) */

#if (USE_POSIX_TERMIOS) || (USE_TERMIOS)

#   if defined(TCSANOW) /* POSIX */

#       define PTY_GET_TERMIOS(fd,tp) tcgetattr((fd), (tp))
#       define PTY_SET_TERMIOS(fd,tp) \
do { \
    cfsetospeed((tp), PTY_BAUDRATE); \
    cfsetispeed((tp), PTY_BAUDRATE); \
\
    tcsetattr((fd), TCSANOW, (tp)); \
} while (FALSE)

#   elif defined(TIOCSETA)

#       define PTY_GET_TERMIOS(fd,tp) ioctl((fd), TIOCGETA, (tp))
#       define PTY_SET_TERMIOS(fd,tp) \
do { \
    tp->c_cflag |= PTY_BAUDRATE; \
\
    ioctl((fd), TIOCSETA, (tp)); \
} while (FALSE)

#   else /* !(defined(TCSANOW) || defined(TIOCSETA)) */

#       define PTY_GET_TERMIOS(fd,tp) ioctl((fd), TCGETS, (tp))
#       define PTY_SET_TERMIOS(fd,tp) \
do { \
    tp->c_cflag |= PTY_BAUDRATE; \
\
    ioctl((fd), TCSETS, (tp)); \
} while (FALSE)

#   endif /* defined(TCSANOW) */

#   define PTY_SET_MODE(fd,tp) PTY_SET_TERMIOS((fd), (tp))

#elif (USE_SYSV_TERMIO)

#   define PTY_SET_MODE(fd,tp) \
do { \
    ioctl((fd), TCSETA, &((tp)->tio)); \
#   if (USE_LTCHARS) \
    ioctl((fd), TIOCSLTC, &((tp)->lc)); \
#   endif \
#   if (USE_SYSV_TERMIO_WITH_LOCAL) \
    ioctl((fd), TIOCLSET, &((tp)->local)); \
#   endif \
} while (FALSE)

#else /* !(USE_POSIX_TERMIOS || USE_TERMIOS || USE_SYSV_TERMIO) */

#   define PTY_SET_MODE(fd,tp) \
do { \
    (tp)->sg.sg_ispeed = (tp)->sg.sg_ospeed = PTY_BAUDRATE; \
\
    ioctl((fd), TIOCSETP, &((tp)->sg)); \
    ioctl((fd), TIOCSETC, &((tp)->tc)); \
#   if (USE_LTCHARS) \
    ioctl((fd), TIOCSLTC, &((tp)->lc)); \
#   endif /* USE_LTCHARS */ \
    ioctl((fd), TIOCSETD, &((tp)->line)); \
    ioctl((fd), TIOCLSET, &((tp)->local)); \
#   if defined(sony) \
    ioctl((fd), TIOCKSET &((tp)->jmode)); \
    ioctl((fd), TIOCKSETC &((tp)->jc)); \
#   endif /* sony */ \
} while (FALSE)

#endif /* (USE_POSIX_TERMIOS) || (USE_TERMIOS) */

#if defined(__CYGWIN__)

#   define PTY_RESET_SLAVE(s,sp)

#else /* !defined(__CYGWIN__) */

#   define PTY_RESET_SLAVE(s,sp) \
do { \
    if ((s)) { \
	chmod((s), (sp)->st_mode); \
	chown((s), (sp)->st_uid, (sp)->st_gid); \
    } \
} while (FALSE)

#endif /* defined(__CYGWIN__) */

#if 0
/*
 * NOTE: some (BSD) systems define the action TCSASOFT which doesn't alter
 * hardware state for tcsetattr().
 */

#define tcsetattr(f,TCSASOFT,tp)
#endif

/*
 * Signal handling.
 */

/*
 * Biggest signal + 1 on the system if available.
 */

#if defined(NSIG)
#   define SIGNALS NSIG
#elif defined(_NSIG)
#   define SIGNALS _NSIG
#endif

/*
 * Old and alternative Unix names for signals.
 */

#if !defined(SIGIOT)
#   define SIGIOT SIGABRT
#endif

#if !defined(SIGCLD)
#   define SIGCLD SIGCHLD
#endif

#if !defined(SIGPOLL)
#   if defined(SIGIO)
#       define SIGPOLL SIGIO
#   endif
#endif

#if !defined(SIGIO)
#   if defined(SIGPOLL)
#       define SIGIO SIGPOLL
#   endif
#endif

#if !defined(SIGPWR)
#   if defined(SIGINFO)
#       define SIGPWR SIGINFO
#   endif
#endif

#if !defined(SIGINFO)
#   if defined(SIGPWR)
#       define SIGINFO SIGPWR
#   endif
#endif

/*
 * Single Unix names for Linux sigaction flags.
 */

#if !defined(SA_NODEFER)
#   if defined(SA_NOHAND)
#       define SA_NODEFER SA_NOHAND
#   else
#       define SA_NODEFER 0
#   endif
#endif

#if !defined(SA_RESETHAND)
#   if defined(SA_ONESHOT)
#       define SA_RESETHAND SA_ONESHOT
#   else
#       define SA_RESETHAND 0
#   endif
#endif

/*
 * Linux names for Single Unix sigaction flags.
 */

#if !defined(SA_NOMASK)
#   if defined(SA_NODEFER)
#       define SA_NOMASK SA_NODEFER
#   else
#       define SA_NOMASK 0
#   endif
#endif

#if !defined(SA_ONESHOT)
#    if defined(SA_RESETHAND)
#        define SA_ONESHOT SA_RESETHAND
#    else
#        define SA_ONESHOT 0
#    endif
#endif

#if (HAVE_POSIX_SIGNALS) || (USE_POSIX_1) || (USE_SVR4) || defined(SCO)
#   define USE_POSIX_SIGNALS 1
#   define USE_SIGACTION 1
#elif (HAVE_SVR3_SIGNALS) || (USE_SVR3)
/* signal() on System V uses the old, unreliable signals. */
#   define USE_SVR3_SIGNALS 1
#   define USE_SIGSET 1
#else
/* new BSD systems define signal() in terms of sigaction(). */
#   if (HAVE_BSD_SIGNALS) || (USE_BSD_42)
#       define USE_BSD_SIGNALS 1
#   endif
#   define USE_SIGNAL 1
#endif

#if defined(SIGTSTP)
#   define SUPPORT_JOB_CONTROL 1
#endif

/*
 * This should be correct for BSDs from at least 4.2BSD up.
 */

#if (USE_SIGNAL) && !(USE_BSD_SIGNALS) 
#   define REINSTALL_SIGNAL_HANDLERS 1
#endif

#if (REINSTALL_SIGNAL_HANDLERS) && defined(__hpux)
#   define REINSTALL_SIGCHLD_HANDLER_AFTER_WAIT 1
#else
#   define REINSTALL_SIGCHLD_HANDLER_AFTER_WAIT 0
#endif

/*
 * Older System V systems can't ignore SIGHUP.
 */

#if (USE_SVR3) && !(USE_SVR4) && !defined(ISC22) && !defined(ISC30) && !defined(ISC40)
#   define CANNOT_IGNORE_SIGHUP 1
#endif

#if (USE_SVR3) || defined(__osf__)
#   if (SUPPORT_JOB_CONTROL)
#       define CHECK_FOR_JOB_CONTROL_SHELL 1
#   else
#       define IGNORE_PARENT_SIGINT 1
#       if defined(__osf__)
#           define IGNORE_PARENT_SIGQUIT 1
#       endif
#   endif
#endif

#if (USE_SIGACTION)
#   define SIGNAL(s,f) R_signal_sigaction((s), (f))
#elif (USE_SIGSET)
/* use reliable signals on System V R3 */
#   define SIGNAL(s,f) sigset((s), (f))
#else
/* use reliable signals on 4.2 and later BSDs, unreliable signals on earlier
 * BSDs and System V pre-R3.
 */
#   define SIGNAL(s,f) signal((s), (f))
#endif

/*
 * Determine the function to be used to wait for terminated or stopped (child)
 * processes without blocking.
 */

#if (USE_POSIX_1) || (USE_SVR4) || (BSD >= 199103) || defined(AMOEBA) || defined(SCO) || defined(__hpux) || defined(__QNX__)
#   define USE_NONBLOCKING_WAITPID 1
#elif (USE_SVR3) && (defined(CRAY) || !(SUPPORT_JOB_CONTROL))
#   define CANNOT_DO_NONBLOCKING_WAIT 1
#else
#   define USE_NONBLOCKING_WAIT3 1
#endif

/*
 * Determine the type of the status argument of wait() and related functions.
 * FIXME: how good is this?
 */

#if (USE_POSIX_WAIT) || defined(__EMX__) || defined(Lynx)
#   define WAIT_STATUS_INT 1
#else
#   define WAIT_STATUS_UNION_WAIT 1
#endif

/*
 * Macros for examining the status of a stopped job. All but WCOREDUMP() are
 * defined by POSIX.1.
 */

#if (WAIT_STATUS_INT)
#   if !defined(WIFSTOPPED)
#       define WIFSTOPPED(s) (((s) & 0xff) == 0x7f)
#   endif

#   if !defined(WSTOPSIG)
#       define WSTOPSIG(s) (((s) & 0xff00) >> 8)
#   endif

#   if !defined(WIFEXITED)
#       define WIFEXITED(s) (((s) & 0xff) == 0)
#   endif

#   if !defined(WEXITSTATUS)
#       define WEXITSTATUS(s) (((s) & 0xff00) >> 8)
#   endif

#   if !defined(WIFSIGNALED)
#       define WIFSIGNALED(s) (!WIFSTOPPED((s)) && !WIFEXITED((s)))
#   endif

#   if !defined(WSTOPSIG)
#       define WSTOPSIG(s) (((s) & 0xff00) >> 8)
#   endif

#   if !defined(WTERMSIG)
#       define WTERMSIG(s) ((s) & 0x7f)
#   endif

#   if !defined(WCOREDUMP)
#       define WCOREDUMP(s) ((s) & 0x80)
#   endif
#else /* !(WAIT_STATUS_INT) */
#   if !defined(WEXITSTATUS)
#       define WEXITSTATUS(s) ((s).w_retcode)
#   endif
#   if !defined(WSTOPSIG)
#       define WSTOPSIG(s) ((s).w_stopsig)
#   endif
#   if !defined(WTERMSIG)
#       define WTERMSIG(s) ((s).w_termsig)
#   endif
#   if !defined(WCOREDUMP)
#       define WCOREDUMP(s) ((s).w_coredump)
#   endif
#endif /* (WAIT_STATUS_INT) */

/*
 * Macros for blocking and unblocking signals.
 *
 * TODO: support SVR3 signals.
 */

#if (USE_POSIX_SIGNALS)
#   define BLOCK_SIGNALS(ss,os) \
do { \
    sigfillset(&(ss)); \
    sigemptyset(&(os)); \
    sigprocmask(SIG_BLOCK, &(ss), &(os)); \
} while (FALSE)
#   define UNBLOCK_SIGNALS(os) \
    (sigprocmask(SIG_SETMASK, &(os), NULL))
#elif (USE_BSD_SIGNALS)
#   define BLOCK_SIGNALS(om) ((om) = sigsetmask(-1))
#   define UNBLOCK_SIGNALS(om) (sigsetmask((om)))
#endif

/*
 * Error handling.
 */

/*
 * POSIX errno values.
 *
 * NOTE: Linux defines ENOTSUPP.
 */

#if !defined(EAGAIN)
#   if defined(EWOULDBLOCK)
#       define EAGAIN EWOULDBLOCK /* Resource temporarily unavailable. */
#   endif
#endif

#if !defined(ENOTSUP)
#   if defined(ENOTSUPP)
#       define ENOTSUP ENOTSUPP /* Operation is not supported. */
#   endif
#endif

/* Unix errno values. */

#if !defined(EWOULDBLOCK)
#   if defined(EAGAIN)
#       define EWOULDBLOCK EAGAIN /* Operation would block. */
#   endif
#endif

#if !defined(ENOTSUPP)
#   if defined(ENOTSUP)
#       define ENOTSUPP ENOTSUP /* Operation is not supported. */
#   endif
#endif

#if 0

/* Define undefined ANSI C errno values to invalid values. */

#if !defined(EDOM)
#   define EDOM -1 /* Numerical argument out of domain. */
#endif

#if !defined(ERANGE) /* Numerical result out of range. */
#   define ERANGE -1
#endif

#if !defined(EILSEQ) /* Invalid or incomplete multibyte or wide character. */
#   define EILSEQ -1
#endif

/* Define undefined POSIX errno values to invalid values. */

#if !defined(E2BIG)
#   define E2BIG -1 /* Argument list too long. */
#endif

#if !defined(EACCES)
#   define EACCES -1 /* Permission denied. */
#endif

#if !defined(EAGAIN)
#   define EAGAIN -1 /* Resource temporarily unavailable. */
#endif

#if !defined(EBADF)
#   define EBADF -1 /* Bad file descriptor. */
#endif

#if !defined(EBADMSG) 
#   define EBADMSG -1 /* Bad message. */
#endif

#if !defined(EBUSY)
#   define EBUSY -1 /* Device or resource busy. */
#endif

#if !defined(ECANCELED)
#   define ECANCELED -1 /* Operation canceled. */
#endif

#if !defined(ECHILD)
#   define ECHILD -1 /* No child processes. */
#endif

#if !defined(EDEADLK)
#   define EDEADLK -1 /* Resource deadlock avoided. */
#endif

#if !defined(EEXIST)
#   define EEXIST -1 /* File exists. */
#endif

#if !defined(EFAULT)
#   define EFAULT -1 /* Bad address. */
#endif

#if !defined(EFBIG)
#   define EFBIG -1 /* File too large. */
#endif

#if !defined(EINPROGRESS)
#   define EINPROGRESS -1 /* Operation now in progress. */
#endif

#if !defined(EINTR)
#   define EINTR -1 /* Interrupted system call. */
#endif

#if !defined(EINVAL)
#   define EINVAL -1 /* Invalid argument. */
#endif

#if !defined(EIO)
#   define EIO -1 /* Input/output error. */
#endif

#if !defined(EISDIR)
#   define EISDIR -1 /* Is a directory. */
#endif

#if !defined(EMFILE)
#   define EMFILE -1 /* Too many open files. */
#endif

#if !defined(EMLINK)
#   define EMLINK -1 /* Too many links. */
#endif

#if !defined(EMSGSIZE)
#   define EMSGSIZE -1 /* Message too long. */
#endif

#if !defined(ENAMETOOLONG)
#   define ENAMETOOLONG -1 /* File name too long. */
#endif

#if !defined(ENFILE)
#   define ENFILE -1 /* Too many open files in system. */
#endif

#if !defined(ENODEV)
#   define ENODEV -1 /* No such device. */
#endif

#if !defined(ENOENT) /* No such file or directory. */
#   define ENOENT -1
#endif

#if !defined(ENOEXEC)
#   define ENOEXEC -1 /* Exec format error. */
#endif

#if !defined(ENOLCK)
#   define ENOLCK -1 /* No locks available. */
#endif

#if !defined(ENOMEM)
#   define ENOMEM -1 /* Cannot allocate memory. */
#endif

#if !defined(ENOSPC)
#   define ENOSPC -1 /* No space left on device. */
#endif

#if !defined(ENOSYS)
#   define ENOSYS -1 /* Function not implemented. */
#endif

#if !defined(ENOTDIR)
#   define ENOTDIR -1 /* Not a directory. */
#endif

#if !defined(ENOTEMPTY)
#   define ENOTEMPTY -1 /* Directory not empty. */
#endif

#if !defined(ENOTSUP)
#   define ENOTSUP -1 /* Operation not supported. */
#endif

#if !defined(ENOTTY)
#   define ENOTTY -1 /* Inappropriate ioctl for device. */
#endif

#if !defined(ENXIO)
#   define ENXIO -1 /* Device not configured. */
#endif

#if !defined(EPERM)
#   define EPERM -1 /* Operation not permitted. */
#endif

#if !defined(EPIPE)
#   define EPIPE -1 /* Broken pipe. */
#endif

#if !defined(EROFS)
#   define EROFS -1 /* Read-only file system. */
#endif

#if !defined(ESPIPE)
#   define ESPIPE -1 /* Illegal seek. */
#endif

#if !defined(ESRCH)
#   define ESRCH -1 /* No such process. */
#endif

#if !defined(ETIMEDOUT)
#   define ETIMEDOUT -1 /* Connection timed out. */
#endif

#if !defined(EXDEV)
#   define EXDEV -1 /* Invalid cross-device link. */
#endif

/* Define undefined 4.4BSD errno values to invalid values. */

#if !defined(ENOTBLK)
#   define ENOTBLK -1 /* Block device required. */
#endif

#if !defined(ETXTBSY)
#   define ETXTBSY -1 /* Text file busy. */
#endif

#if !defined(EWOULDBLOCK)
#   define EWOULDBLOCK -1 /* Operation would block. */
#endif

#if !defined(EALREADY)
#   define EALREADY -1 /* Operation already in progress. */
#endif

#if !defined(ENOTSOCK)
#   define ENOTSOCK -1 /* Socket operation on non-socket. */
#endif

#if !defined(EDESTADDRREQ)
#   define EDESTADDRREQ -1 /* Destination address required. */
#endif

#if !defined(EPROTOTYPE)
#   define EPROTOTYPE -1 /* Protocol wrong type for socket. */
#endif

#if !defined(ENOPROTOOPT)
#   define ENOPROTOOPT -1 /* Protocol not available. */
#endif

#if !defined(EPROTONOSUPPORT)
#   define EPROTONOSUPPORT -1 /* Protocol not supported. */
#endif

#if !defined(ESOCKTNOSUPPORT)
#   define ESOCKTNOSUPPORT -1 /* Socket type not supported. */
#endif

#if !defined(EOPNOTSUPP)
#   define EOPNOTSUPP -1 /* Operation not supported. */
#endif

#if !defined(EPFNOSUPPORT)
#   define EPFNOSUPPORT -1 /* Protocol family not supported. */
#endif

#if !defined(EAFNOSUPPORT)
#   define EAFNOSUPPORT -1 /* Address family not supported by protocol. */
#endif

#if !defined(EADDRINUSE)
#   define EADDRINUSE -1 /* Address already in use. */
#endif

#if !defined(EADDRNOTAVAIL)
#   define EADDRNOTAVAIL -1 /* Cannot assign requested address. */
#endif

#if !defined(ENETDOWN)
#   define ENETDOWN -1 /* Network is down. */
#endif

#if !defined(ENETUNREACH)
#   define ENETUNREACH -1 /* Network is unreachable. */
#endif

#if !defined(ENETRESET)
#   define ENETRESET -1 /* Network dropped connection on reset. */
#endif

#if !defined(ECONNABORTED)
#   define ECONNABORTED -1 /* Software caused connection abort. */
#endif

#if !defined(ECONNRESET)
#   define ECONNRESET -1 /* Connection reset by peer. */
#endif

#if !defined(ENOBUFS)
#   define ENOBUFS -1 /* No buffer space available. */
#endif

#if !defined(EISCONN)
#   define EISCONN -1 /* Transport endpoint is already connected. */
#endif

#if !defined(ENOTCONN)
#   define ENOTCONN -1 /* Transport endpoint is not connected. */
#endif

#if !defined(ESHUTDOWN)
#   define ESHUTDOWN -1 /* Cannot send after transport endpoint shutdown. */
#endif

#if !defined(ETOOMANYREFS)
#   define ETOOMANYREFS -1 /* Too many references: cannot splice. */
#endif

#if !defined(ECONNREFUSED)
#   define ECONNREFUSED -1 /* Connection refused. */
#endif

#if !defined(EHOSTDOWN)
#   define EHOSTDOWN -1 /* Host is down. */
#endif

#if !defined(EHOSTUNREACH)
#   define EHOSTUNREACH -1 /* No route to host. */
#endif

#if !defined(EPROCLIM)
#   define EPROCLIM -1 /* Too many processes. */
#endif

#if !defined(EUSERS)
#   define EUSERS -1 /* Too many users. */
#endif

#if !defined(EDQUOT)
#   define EDQUOT -1 /* Disk quota exceeded. */
#endif

#if !defined(ESTALE)
#   define ESTALE -1 /* Stale NFS file handle. */
#endif

#if !defined(EREMOTE)
#   define EREMOTE -1 /* Object is remote. */
#endif

#if !defined(EBADRPC)
#   define EBADRPC -1 /* RPC structure is bad. */
#endif

#if !defined(ERPCMISMATCH)
#   define ERPCMISMATCH -1 /* RPC version is wrong. */
#endif

#if !defined(EPROGUNAVAIL)
#   define EPROGUNAVAIL -1 /* RPC program is not available. */
#endif

#if !defined(EPROGMISMATCH)
#   define EPROGMISMATCH -1 /* RPC program version is wrong. */
#endif

#if !defined(EPROCUNAVAIL)
#   define EPROCUNAVAIL -1 /* RPC bad procedure for program. */
#endif

#if !defined(EFTYPE)
#   define EFTYPE -1 /* Inappropriate file type or format. */
#endif

#if !defined(EAUTH)
#   define EAUTH -1 /* Authentication error. */
#endif

#if !defined(ENEEDAUTH)
#   define ENEEDAUTH -1 /* Need authenticator. */
#endif

/* Define undefined GNU Hurd errno values to invalid values. */

#if !defined(ELOOP)
#   define ELOOP -1 /* Too many levels of symbolic links. */
#endif

#if !defined(EBACKGROUND)
#   define EBACKGROUND -1 /* Inappropriate operation for background process. */
#endif

#if !defined(EDIED)
#   define EDIED -1 /* Translator died. */
#endif

#if !defined(EIDRM)
#   define EIDRM -1 /* Identifier removed. */
#endif

#if !defined(EMULTIHOP)
#   define EMULTIHOP -1 /* Multihop attempted. */
#endif

#if !defined(ENODATA)
#   define ENODATA -1 /* No data available. */
#endif

#if !defined(ENOLINK)
#   define ENOLINK -1 /* Link has been severed. */
#endif

#if !defined(ENOMSG)
#   define ENOMSG -1 /* No message of desired type. */
#endif

#if !defined(ENOSR)
#   define ENOSR -1 /* Out of streams resources. */
#endif

#if !defined(ENOSTR)
#   define ENOSTR -1 /* Device not a stream. */
#endif

#if !defined(EOVERFLOW)
#   define EOVERFLOW -1 /* Value too large for defined data type. */
#endif

#if !defined(EPROTO)
#   define EPROTO -1 /* Protocol error. */
#endif

#if !defined(ETIME)
#   define ETIME -1 /* Timer expired. */
#endif

/* Define undefined Sun Solaris 2 errno values to invalid values. */

#if !defined(ECHRNG)
#   define ECHRNG -1 /* Channel number out of range. */
#endif

#if !defined(EL2NSYNC)
#   define EL2NSYNC -1 /* Level 2 not synchronized. */
#endif

#if !defined(EL3HLT)
#   define EL3HLT -1 /* Level 3 halted. */
#endif

#if !defined(EL3RST)
#   define EL3RST -1 /* Level 3 reset. */
#endif

#if !defined(ELNRNG)
#   define ELNRNG -1 Link number out of range. */
#endif

#if !defined(EUNATCH)
#   define EUNATCH -1 /* Protocol driver not attached. */
#endif

#if !defined(ENOCSI)
#   define ENOCSI -1 /* No CSI structure available. */
#endif

#if !defined(EL2HLT)
#   define EL2HLT -1 /* Level 2 halted. */
#endif

#if !defined(EBADE)
#   define EBADE -1 /* Invalid exchange. */
#endif

#if !defined(EBADR)
#   define EBADR -1 /* Invalid request descriptor. */
#endif

#if !defined(EXFULL)
#   define EXFULL -1 /* Exchange full. */
#endif

#if !defined(ENOANO)
#   define ENOANO -1 /* No anode. */
#endif

#if !defined(EBADROC)
#   define EBADROC -1 /* Invalid request code. */
#endif

#if !defined(EBADSLT)
#   define EBADSLT -1 /* Invalid slot. */
#endif

#if !defined(EDEADLOCK)
#   define EDEADLOCK EDEADLK /* Resource deadlock avoided. */
#endif

#if !defined(EBFONT)
#   define EBFONT -1 /* Bad font file format. */
#endif

#if !defined(ENONET)
#   define ENONET -1 /* Machine is not on the network. */
#endif

#if !defined(ENOPKG)
#   define ENOPKG -1 /* Package not installed. */
#endif

#if !defined(EADV)
#   define EADV -1 /* Advertise error. */
#endif

#if !defined(SRMNT)
#   define SRMNT -1 /* Srmount error. */
#endif

#if !defined(ECOMM)
#   define ECOMM -1 /* Communication error on send. */
#endif

#if !defined(ENOTUNIQ)
#   define ENOTUNIQ -1 /* Name not unique on network. */
#endif

#if !defined(EBADFD)
#   define EBADFD -1 /* File descriptor in bad state. */
#endif

#if !defined(EREMCHG)
#   define EREMCHG -1 /* Remote address changed. */
#endif

#if !defined(ELIBACC)
#   define ELIBACC -1 /* Can not access a needed shared library. */
#endif

#if !defined(ELIBBAD)
#   define ELIBBAD -1 /* Accessing a corrupted shared library. */
#endif

#if !defined(ELIBSCN)
#   define ELIBSCN -1 /* .lib section in a.out corrupted. */
#endif

#if !defined(ELIBMAX)
#   define ELIBMAX -1 /* Ateempting to link in too many shared libraries. */
#endif

#if !defined(ELIBEXEC)
#   define ELIBEXEC -1 /* Attempting to exec a shared library. */
#endif

#if !defined(ERESTART)
#   define ERESTART -1 /* Interrupted system call should be restarted. */
#endif

#if !defined(ESTRPIPE)
#   define ESTRPIPE -1 /* Streams pipe error. */
#endif

/* Define undefined Linux 2.4.1 errno values to invalid values. */

#if !defined(EDOTDOT)
#   define EDOTDOT -1 RPC specific error. */
#endif

#if !defined(EUCLEAN)
#   define EUCLEAN -1 /* Structure needs cleaning. */
#endif

#if !defined(ENOTNAM)
#   define ENOTNAM -1 /* Not a XENIX named type file. */
#endif

#if !defined(ENAVAIL)
#   define ENAVAIL -1 /* No XENIX semaphores available. */
#endif

#if !defined(EISNAM)
#   define EISNAM -1 /* Is a named type file. */
#endif

#if !defined(EREMOTEIO)
#   define EREMOTEIO -1 /* Remote I/O error. */
#endif

#if !defined(ENOMEDIUM)
#   define ENOMEDIUM -1 /* No medium found. */
#endif

#if !defined(EMEDIUMTYPE)
#   define EMEDIUMTYPE -1 /* Wrong medium type. */
#endif

#if !defined(EHASHCOLLISION)
#   define EHASHCOLLISION -1 /* Number of hash collisions exceeds maximum
			      * generation counter value.
			      */
#endif

#endif /* 0 */

/*
 * Non-local goto, i.e. longjmp(), siglongjmp(), and related routines.
 *
 * NOTE: use of non-local goto (or goto) makes programs hard to understand and
 * maintain, and should therefore be avoided.
 */

#if (HAVE_POSIX_SIGSETJMP) || (USE_POSIX)
#   define jmpbuf_t sigjmp_buf
#   if !defined(__OPENNT)
#       undef setjmp
#       define setjmp(x) sigsetjmp((x), 1)
#       undef longjmp
#       define longjmp(x,n) siglongjmp((x), (n))
#   endif /* !defined(__OPENNT) */
#else
#   define jmpbuf_t jmp_buf
#endif

/*
 * Exit handling.
 */

#if (USE_ANSI_C) || (USE_SVR4) || (USE_BSD_43)
#   define USE_ATEXIT 1
#elif defined(sun)
#   define USE_ON_EXIT 1
#endif

/* exit status. */

#if !defined(EXIT_SUCCESS)
#   define EXIT_SUCCESS 0
#endif

#if !defined(EXIT_FAILURE)
#   define EXIT_FAILURE 1
#endif

/*
 * User logging.
 */

#if (defined(__osf__) || (USE_SVR4) || (((USE_SVR3) && (defined(i386)) || defined(CRAY) || defined(macII) || defined(SCO) || defined(__hpux) || defined(AIXV3))) || defined(linux)) && !defined(AMOEBA) && !defined(MINIX) && !defined(__CYGWIN__) && !defined(__QNX__) && !defined(Lynx) && !defined(__GNU__) && !defined(__sgi)
#   define USE_SYSV_UTMP 1
#endif

#if !defined(UTMP_FILENAME)
#   if defined(UTMP_FILE)
#       define UTMP_FILENAME UTMP_FILE
#   elif defined(_PATH_UTMP)
#       define UTMP_FILENAME _PATH_UTMP
#   else
#       define UTMP_FILENAME "/etc/utmp"
#   endif
#endif /* !defined(UTMP_FILENAME) */

#if !defined(WTMP_FILENAME)
#   if defined(WTMP_FILE)
#       define WTMP_FILENAME WTMP_FILE
#   elif defined(_PATH_WTMP)
#       define WTMP_FILENAME _PATH_WTMP
#   elif (USE_SVR3)
#       define WTMP_FILENAME "/etc/wtmp"
#   else
#       define WTMP_FILENAME "/usr/adm/wtmp"
#   endif
#endif

#if (USE_SYSV_UTMP) && (USE_UTMPX)
#   undef WTMP_FILENAME
#   define WTMP_FILENAME WTMPX_FILE
#endif

#if !defined(USER_PROCESS)
#   define USER_PROCESS 7
#endif

#if !defined(DEAD_PROCESS)
#   define DEAD_PROCESS 8
#endif

/* Process management. */

/* System V setpgrp takes no arguments, BSD has setpgrp(pid, pgid). */

#if (USE_POSIX_1) || (USE_SVR4) || defined(__convex__) || defined(SCO325) || (defined(__QNX__) && !defined(__QNXNTO__)) || (defined(CRAY) && (OSMAJORVERSION > 5))
#   define USE_SETSID 1
#elif (SETPGRP_VOID) || defined(linux) || defined(__GNU__) || (USE_SVR3) || defined(__MVS__)
#   define USE_SYSV_SETPGRP 1
#elif (USE_SVR3) && defined(macII)
#   define USE_SYSV_SETPGRP 1
#elif defined(__QNXNTO__)
#   define USE_SYSV_SETPGRP 1
#elif defined(MINIX)
#   define USE_BSD_SETPGRP 1
#   define setpgrp setpgid
#elif defined(__osf__)
#   defined USE_BSD_SETPGRP
#   define setpgrp setpgid
#else
/* 4.2BSD and others. */
#   define USE_BSD_SETPGRP 1
#endif

/* become process group leader. */

#if (USE_SETSID)
#   define PROCESS_BECOME_GROUP_LEADER() setsid()
#elif (USE_SYSV_SETPGRP)
#   define PROCESS_BECOME_GROUP_LEADER setpgrp();
#elif (USE_BSD_SETPGRP)
#   define PROCESS_BECOME_GROUP_LEADER() setpgrp(0, 0)
#endif /* USE_SETSID */

#if defined(ultrix)
#   define PROCESS_CREATE_SESSION(pid) ((pid) = 0)
#else /* !defined(ultrix) */
#   define PROCESS_CREATE_SESSION(pid) ((pid) = PROCESS_BECOME_GROUP_LEADER())
#endif /* defined(ultrix) */

/*
 * Time and sleeping.
 *
 * TODO: add more systems that support gettimeofday().
 */

#if (HAVE_GETTIMEOFDAY) || (USE_SVR4) || (USE_BSD_43) || defined(linux)
#   define USE_GETTIMEOFDAY 1
#endif

/*
 * Compute tv2 - tv1 in microseconds.
 */

#define TIMEVAL_CMP_USEC(tv1,tv2) \
    (((tv2)->tv_sec - (tv1)->tv_sec) * 1000000 \
     + ((tv2)->tv_usec - (tv1)->tv_usec))

/*
 * Compute tv += i.
 */

#define TIMEVAL_ADD_CONST(tv,i) \
do { \
    (tv)->tv_sec += i / 1000000; \
    (tv)->tv_usec += i % 1000000; \
\
    if ((tv)->tv_usec >= 1000000) { \
        (tv)->tv_sec++; \
        (tv)->tv_usec -= 1000000; \
    } else if ((t)->tv_usec < 0) { \
        (tv)->tv_sec--; \
        (tv)->tv_usec += 1000000; \
    }\
} while (FALSE)

/*
 * Compute tv1 = tv2 + i.
 */

#define TIMEVAL_ADD_CONST_2(tv1,tv2,i) \
do { \
    (tv1)->tv_sec = (tv2)->tv_sec + (i) / 1000000; \
    (tv1)->tv_usec = (tv2)->tv_usec + (i) % 1000000; \
\
    if ((tv1)->tv_usec >= 1000000) { \
	(tv1)->tv_sec++; \
	(tv1)->tv_usec -= 1000000; \
    } else if ((tv1)->tv_usec < 0) { \
        (tv1)->tv_sec--; \
        (tv1)->tv_usec += 1000000; \
    }\
} while (FALSE)

/*
 * Compute tv -= i.
 */

#define TIMEVAL_SUBTRACT_CONST(tv,i) TIMEVAL_ADD_CONST((tv), -(i))

/*
 * Compute tv1 = tv2 - i.
 */

#define TIMEVAL_SUBTRACT_CONST_2(tv1,tv2,i) \
    TIMEVAL_ADD_CONST_2((tv1), (tv2), -(i))

/* sleeping. */

#if (HAVE_USLEEP) || (USE_BSD_43)
#   define USLEEP(u) usleep((u))
#elif (HAVE_NANOSLEEP) || (USE_POSIX_1b)
#   define USLEEP(u) \
do { \
    struct timespec timespec; \
\
    timespec.tv_sec = (u) / 1000000; \
    timespec.tv_nsec = ((u) % 1000000) * 1000; \
\
    do { \
        errno = 0; \
\
        nanosleep(&timespec, &timespec); \
    } while (errno == EINTR); \
} while (FALSE)

#else
/* !(HAVE_USLEEP) && !(USE_BSD_43) && !(HAVE_NANOSLEEP) && !(USE_POSIX1b) */
#   if (USE_GETTIMEOFDAY)
#       define USLEEP(u) \
do { \
    struct timeval selecttimeval; \
    struct timeval currenttimeval, stoptimeval; \
\
    gettimeofday(&currenttimeval, NULL); \
\
    stoptimeval = currenttimeval; \
    TIMEVAL_ADD_CONST(&stoptimeval, (u)); \
\
    if (TIMEVAL_CMP_USEC(&currenttimeval, &stoptimeval) > 0) { \
	do { \
	    errno = 0; \
\
	    selecttimeval.tv_sec = 0; \
	    selecttimeval.tv_usec = 0; \
\
	    TIMEVAL_ADD_CONST(&selecttimeval, \
			      TIMEVAL_CMP_USEC(&currenttimeval, \
					       &stoptimeval)); \
\
	    Rselect(0, NULL, NULL, NULL, &selecttimeval); \
\
	    gettimeofday(&currenttimeval); \
	} while (errno == EINTR \
		 && TIMEVAL_CMP_USEC(&currenttimeval, &stoptimeval) > 0); \
    } \
} while (FALSE)
#   else /* !(USE_GETTIMEOFDAY) */
/*
 * NOTE: this will stop sleeping on the next signal to interrupt the call to
 * select() It also can't be fixed for systems that restart the call
 * automatically after it has been interrupted; selecttimeval would possibly
 * have its original values. If a signal occurs during the call to select(),
 * the time spent sleeping may be longer than u.
 */
#       define USLEEP(u) \
do { \
    struct timeval selecttimeval; \
\
    selecttimeval.tv_sec = 0; \
    selecttimeval.tv_usec = 0; \
\
    stoptimeval = selecttimeval; \
    TIMEVAL_ADD_CONST(selecttimeval, (u)); \
\
    if ((u) > 0) { \
	do { \
	    errno = 0; \
\
	    Rselect(0, NULL, NULL, NULL, &selecttimeval); \
	} \
    } \
} while (FALSE)
#   endif /* (USE_GETTIMEOFDAY) */
#endif /*  (HAVE_USLEEP) || (USE_BSD_43) */

/*
 * Miscellaneous features.
 */

#if defined(AMOEBA)
#   define NULLCAP ((capability *)NULL)
#endif /* defined(AMOEBA) */

#if defined(sony) && defined(bsd43) && !defined(KANJI)
#   define KANJI
#endif

/*
 * System data types.
 */

/* Missing prototypes. */

#if (__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 1))
extern int getpt(void);
extern int seteuid(uid_t uid);
extern int setegid(uid_t uid);
#endif

//#if defined(linux) && (LINUX_VERSION_CODE >= KERNEL_VERSION(2,1,44))
#if defined(linux)
extern int setresuid(uid_t ruid, uid_t euid, uid_t suid);
extern int setresgid(gid_t rgid, gid_t egid, gid_t sgid);
#endif

/*
 * Macros typically in <sys/param.h>.
 */

/*
 * POSIX and ANSI C names for values typically defined in <sys/param.h>.
 * NOTE: these need to be defined after including <signal.h> on systems based
 * on the GNU C Library.
 */

#if !defined(CHAR_BIT)
#   if defined(NBBY)
#       define CHAR_BIT NBBY
#   endif
#endif

#if !defined(CHILD_MAX)
#   if defined(MAXUPRC)
#       define CHILD_MAX MAXUPRC
#   endif
#endif

#if !defined(NGROUPS_MAX)
#   if defined(NGROUPS)
#       define NGROUPS_MAX NGROUPS
#   endif
#endif

/*
 * NOTE: SYMLOOP_MAX is defined by POSIX.1a.
 */

#if !defined(SYMLOOP_MAX)
#   if defined(MAXSYMLINKS)
#       define SYMLOOP_MAX MAXSYMLINKS
#   endif
#endif

#if !defined(ARG_MAX)
#   if defined(NCARGS)
#       define ARG_MAX NCARGS
#   endif
#endif

#if !defined(PATH_MAX)
#   if defined(MAXPATHLEN)
#       define PATH_MAX MAXPATHLEN
#   else
#       define PATH_MAX 4095 /* not necessarily adequate. */
#   endif
#endif

#if !defined(OPEN_MAX)
#   if defined(NOFILE)
#       define OPEN_MAX NOFILE
#   elif defined(_NFILE)
#       define OPEN_MAX _NFILE
#   elif defined(FOPEN_MAX)
#       define OPEN_MAX FOPEN_MAX
#   else
#       define OPEN_MAX 256 /* not necessarily adequate. */
#   endif
#endif

/*
 * POSIX names for values defined in standard ANSI C <stdio.h> header file and
 * <dirent.h> header file.
 */

#if !defined(NAME_MAX)
#   if defined(FILENAME_MAX)
#       define NAME_MAX FILENAME_MAX
#   elif defined(MAXNAMLEN)
#       define NAME_MAX MAXNAMLEN
#   endif
#endif

/*
 * Values defined in <dirent.h>, at least on some systems.
 */

#if !defined(MAXNAMLEN)
#   if defined(NAME_MAX)
#       define MAXNAMLEN NAME_MAX
#   endif
#endif

/*
 * Values defined in ANSI C <stdio.h> header file.
 */

#if !defined(FILENAME_MAX)
#   if defined(NAME_MAX)
#       define FILENAME_MAX NAME_MAX
#   endif
#endif

#if !defined(FOPEN_MAX)
#   if defined(OPEN_MAX)
#       define FOPEN_MAX OPEN_MAX
#   endif
#endif

/*
 * Values defined in <stdio.h> on some systems.
 */

#if !defined(_NFILE)
#   if defined(OPEN_MAX)
#       define _NFILE OPEN_MAX
#   endif
#endif

/*
 * Linux name for a value typically found in <sys/param.h>.
 */

#if !defined ACCT_COM
#   if defined(MAXCOMLEN)
#       define ACCT_COM MAXCOMLEN
#   endif
#endif

/*
 * BSD names for values typically found in <sys/param.h>.
 */

#if !defined(NBBY)
#   define NBBY CHAR_BIT /* bytes in a character. */
#endif

#if !defined(MAXUPRC)
#   if defined(CHILD_MAX)
#       define MAXUPRC CHILD_MAX /* max simultaneous processes. */
#   endif
#endif

#if !defined(NGROUPS)
#   if defined(NGROUPS_MAX)
#       define NGROUPS NGROUPS_MAX /* max number of supplemental group ids. */
#   endif
#endif

#if !defined(MAXSYMLINKS)
#   if defined(SYMPLOOP_MAX)
#       define MAXSYMLINKS SYMLOOP_MAX /* max number of symlinks expanded. */
#   endif
#endif

#if !defined(CANBSIZ)
#   if defined(MAX_CANON)
#       define CANBSIZ MAX_CANON /* bytes on a terminal's canonical input
				  * queue
				  */
#   endif
#endif

#if !defined(NCARGS)
#   if defined(ARG_MAX)
#       define NCARGS ARG_MAX /* max length of arguments to exec functions. */
#   endif
#endif

#if !defined(MAXPATHLEN)
#   if defined(PATH_MAX)
#       define MAXPATHLEN PATH_MAX /* max length of path. */
#   endif
#endif

#if !defined(MAXHOSTNAMELEN)
#   define MAXHOSTNAMELEN 256 /* max host name length. */
#endif

#if !defined(NOFILE)
#   if defined(OPEN_MAX)
#       define NOFILE OPEN_MAX /* max open files per process. */
#   endif
#endif

/*
 * NOTE: Linux systems define ACCT_COM. This could (or should?) be defined to
 * ACCT_COM + 1.
 */

#if !defined(MAXCOMLEN)
#   if defined(ACCT_COM)
#       define MAXCOMLEN ACCT_COM /* max command name remembered. */
#   else
#       define MAXCOMLEN 16
#   endif
#endif

#if !defined(MAXINTERP)
#   define MAXINTERP 32 /* max interpreter file name length. */
#endif

#if !defined(MAXLOGNAME)
#   define MAXLOGNAME 8 /* max login name length. */
#endif

#if !defined(NOGROUP)
#   define NOGROUP 65535
#endif

/* FIXME: (dev_t)-1? */

#if !defined(NODEV)
#   define NODEV (-1)
#endif

#if !defined(CMASK)
#   define CMASK (S_IWGRP | S_IWOTH) /* default file creation mask. */
#endif

/*
 * Unit of st_blocks in struct stat.
 */

#if !defined(DEV_BSHIFT)
#   define DEV_BSHIFT 9 /* log2(DEV_BSIZE) */
#endif

#if !defined(DEV_BSIZE)
#   define DEV_BSIZE 512
#endif

/*
 * Machine type dependent values.
 */

#if !defined(NBPW)
#   define NBPW sizeof(unsigned long) /* number of bytes per word. */
#endif

/*
 * Values often in <sys/param.h>.
 */

#if !defined(PDROFSET)
#   if defined(NBPDR)
#       define PDROFSET (NBPDR - 1)
#   endif
#endif

#if !defined(CLSIZE)
#   define CLSIZE 1
#endif

#if !defined(CLBYTES)
#   define CLBYTES (CLSIZE * NBPG)
#endif

#if !defined(CLOFSET)
#   define CLOFSET (CLSIZE * NBPG - 1)
#endif

#if !defined(claligned)
#   define claligned(x) ((((int)(x)) & CLOFSET) == 0)
#endif

#if !defined(CLOFF)
#   define CLOFF CLOFSET
#endif

#if (CLSIZE == 1)

#   if !defined(clbase)
#       define clbase(i) (i)
#   endif

#   if !defined(clrnd)
#       define clrnd(i) (i)
#   endif

#else /* !(CLSIZE == 1) */

#   if !defined(clbase)
#       define clbase(i) ((i) & ~(CLSIZE - 1))
#   endif

#   if !defined(clrnd)
#       define clrnd(i) (((i) + (CLSIZE - 1)) & ~(CLSIZE - 1))
#   endif

#endif /* (CLSIZE == 1) */

/* Bitmap-related macros typically in <sys/param.h>. */

#if !defined(setbit)
#   define setbit(a,i) ((a)[(i) / NBBY] |= 1 << ((i) % NBBY))
#endif

#if !defined(clrbit)
#   define clrbit(a,i) ((a)[(i) / NBBY] &= ~(1 << ((i) % NBBY)))
#endif

#if !defined(isset)
#   define isset(a,i) ((a)[(i) / NBBY] & (1 << ((i) % NBBY)))
#endif

#if !defined(isclr)
#   define isclr(a,i) (((a)[(i) / NBBY] & (1 << ((i) % NBBY))) == 0)
#endif

/* Macros for counting and rounding typically in <sys/param.h>. */

#if !defined(howmany)
#   define howmany(x,y) (((x) + ((y) - 1)) / (y))
#endif

#if !defined(rounddown)
#   define rounddown(x,y) (((x) / (y)) * (y))
#endif

#if !defined(roundup)
#   define roundup(x,y) ((((x) + ((y) - 1)) / (y)) * (y))
#endif

#if !defined(powerof2)
#   define powerof2(x) ((((x) - 1) & (x)) == 0)
#endif

/*
 * Macros for min and max typically in <sys/param.h>.
 */

#if !defined(MIN)
#   define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#if !defined(MAX)
#   define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

/*
 * Macros often in <machine/param.h>.
 */

/*
 * Mach-derived conversion macros.
 */

#if !defined(trunc_page)
#   if defined(NBPG)
#       define trunc_page(x) ((unsigned int)(x) & ~(NBPG - 1))
#   endif
#endif

#if !defined(round_page)
#   if defined(NBPG)
#       define round_page(x) ((((unsigned int)(x)) + NBPG - 1) & ~(NBPG - 1))
#   endif
#endif

#if !defined(atop)
#   if defined(PGSHIFT)
#       define atop(x) ((unsigned int)(x) >> PGSHIFT)
#   endif
#endif

#if !defined(ptoa)
#   if defined(PGSHIFT)
#       define ptoa(x) ((unsigned int)(x) << PGSHIFT)
#   endif
#endif

/*
 * This is correct for BSDs from 4.2 up. <X11/Xpoll.h>
 * (or <Resurrection/select.h>) defines getdtablesize() on some systems.
 */

#if !(HAVE_GETDTABLESIZE) && !(USE_XOPEN_UNIX) && !(USE_SVR4) && !(USE_BSD_42) && !(__GLIBC__ > 2) && !((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 1)) && !defined(getdtablesize)
#   if (USE_ULIMIT)
#       define getdtablesize() (ulimit(4, 0L))
#   elif (USE_SYSCONF) && defined(_SC_OPEN_MAX)
#       define getdtablesize() sysconf(_SC_OPEN_MAX)
#   elif defined(OPEN_MAX)
#       define getdtablesize() (OPEN_MAX)
#   endif /* defined(_SC_OPEN_MAX) */
#endif

/*
 * File I/O macros.
 */

#if !(HAVE_DUP2) && (USE_SVR3) && !defined(_CRAY) && !defined(Mips) && !defined(_SEQUENT_) && !defined(sco)
#   define dup2(fd1,fd2) (((fd1) == (fd2)) \
			  ? (fd1) \
			  : (close((fd2)), fcntl((fd1), F_DUPFD, (fd2))))
#endif

/*
 * FIXME: add Rnbread() and Rnbwrite() for __EMX__.
 */
#if defined(MINIX)
#   define Rnbread(f,b,n) nbio_read(f, b, n)
#   define Rnbwrite(f,b,n) nbio_write(f, b, n)
#   define Rselect(n,r,w,x,t) nbio_select(n, r, w, x, t)
#elif defined(__EMX__)
#   define Rselect(n,r,w,x,t) os2PseudoSelect(n, r, w, x, t)
#elif defined(WIN32)
#   define Rselect(n,r,w,x,t) select(0, (fd_set *)r, (fd_set *)w, (fd_set *)x,\
                                         (struct timeval *)t);
#else
#   define Rnbread(f,b,n) read(f, b, n)
#   define Rnbwrite(f,b,n) write(f, b, n)
#   if defined(hpux) /* FIXME: perhaps old BSD. */
#       define Rselect(n,r,w,x,t) select(n, (int *)r, (int *)w, (int *)x, \
					 (struct timeval *)t)
#   else
#       define Rselect(n,r,w,x,t) select(n, r, w, x, t)
#   endif
#endif

#if defined(__EMX__)
#   define link rename
#endif

/*
 * Resurrection replacements for common functions. These'd better go last to
 * redefine anything included above. :)
 */

#endif /* RESURRECTION_SYSTEM_H */

