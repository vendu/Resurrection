AC_DEFUN([R_SET_CC_FLAGS],
[
    R_SET_CFLAGS
    R_SET_LDFLAGS
    R_SET_INCLUDES
])

AC_DEFUN([R_SET_CFLAGS],
[
    R_CFLAGS="-Wall"
    AC_SUBST([R_CFLAGS])
])

AC_DEFUN([R_SET_LDFLAGS],
[
    R_LDFLAGS="-pthread"
    AC_SUBST([R_LDFLAGS])
])

AC_DEFUN([R_SET_INCLUDES],
[
    R_INCLUDES="-I/usr/local/include -I/usr/local/X11R6/include -I/usr/X11R6/include"
    AC_SUBST([R_INCLUDES])
])

