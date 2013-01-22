# it looks like new BSD systems don't like <malloc.h> to be included; they
# say it's made obsolete by <stdlib.h>. therefore, let's check if it's useful.
AC_DEFUN([R_CHECK_HEADERS],
[
    AC_CHECK_HEADER(malloc.h)
    AC_CHECK_HEADER(termios.h)
    AC_CHECK_HEADER(sys/termios.h)
    AC_CHECK_HEADER(sys/ttycom.h)
    AC_CHECK_HEADER(sys/endian.h)
#    AC_CHECK_HEADER(freetype2/freetype/freetype.h, AM_CPPFLAGS="$AM_CPPFLAGS -I/usr/include/freetype2")
])

AC_DEFUN([R_CHECK_SIZES],
[
    AC_CHECK_SIZEOF(unsigned long)
    AC_CHECK_SIZEOF(void *)
    AC_CHECK_SIZEOF(size_t)
    AC_CHECK_SIZEOF([off_t])
])

AC_DEFUN([R_CHECK_TYPES],
[
    R_CHECK_64BIT_OFF_T
])

AC_DEFUN([R_CHECK_LIBS],
[
    AC_CHECK_LIB(pthread, pthread_create, LIBS="$LIBS -pthread")
    AC_CHECK_LIB(Xext, XShmAttach, LIBS="$LIBS -lXext")
    AC_CHECK_LIB(Imlib2, imlib_context_set_display, LIBS="$LIBS -lImlib2")
    AC_CHECK_LIB(freetype, FT_New_Face, enable_freetype="yes", enable_freetype="no")
    if test "$enable_freetype" = "yes" ; then
        AM_CPPFLAGS="$AM_CPPFLAGS -I/usr/include/freetype2"
	LIBS="$LIBS -lfreetype"
    fi
])

AC_DEFUN([R_ENABLE_GPROF],
[
    CFLAGS="$CFLAGS -pg"
])

# this trick was adopted from libgfortran.
AC_DEFUN([R_CHECK_64BIT_OFF_T],
[
    AC_REQUIRE([R_CHECK_SIZES])
    AC_MSG_CHECKING([for how to enable 64-bit off_t])
    if test "$ac_cv_sizeof_off_t" = "8" ; then
        R_cv_64bit_off_t="yes"
        AC_MSG_RESULT([native])
    else
        AC_CACHE_VAL([R_cv_64bit_off_t],
        [
            AC_LINK_IFELSE(
            [
                AC_LANG_SOURCE(
                [
                    #include <unistd.h>

                    int
                    main(int argc, char *argv[])
                    {
                        fopen64("t", "w");
                        lseek64(1, 0, SEEK_CUR);
                    }
                ])
            ],
            R_cv_64bit_off_t="yes",
            R_cv_64bit_off_t="no")
        ])
        if test "$R_cv_64bit_off_t" = "yes" ; then
            CFLAGS="$CFLAGS -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64"
            AC_DEFINE([_FILE_OFFSET_BITS], [64])
            AC_MSG_RESULT([_FILE_OFFSET_BITS=64])
        else
            AC_MSG_WARN([cannot determine how to enable 64-bit off_t])
        fi
    fi
])

