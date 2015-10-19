AC_DEFUN([R_SAVE_OPTS],
[
    saveopts=`echo $1`

    if test -n "$saveopts" ; then
        if test -z "$r_configure_opts" ; then
            r_configure_opts="$saveopts"
        else
            r_configure_opts="$r_configure_opts $saveopts"
        fi
    fi
])

AC_DEFUN([R_SET_INSTALL_DIRS],
[
    # Installation prefix.
    if test -z "$prefix" || test "$prefix" = "NONE" ; then
        prefix="$ac_default_prefix"
    fi

    # Installation prefix.
    if test -z "$exec_prefix" || test "$exec_prefix" = "NONE" ; then
        exec_prefix="$prefix"
    fi

    # Binary installation directory.
    if test -z "$bindir" || test "bindir" = "NONE" ; then
        bindir="$exec_prefix/bin"
    fi

    # System binary installation directory.
    if test -z "$sbindir" || test "sbindir" = "NONE" ; then
        sbindir="$exec_prefix/sbin"
    fi

    # Executable installation directory.
    if test -z "$libexecdir" || test "libexecdir" = "NONE" ; then
        libexecdir="$exec_prefix/libexec"
    fi

    # Configuration file installation directory.
    if test -z "$sysconfdir" || test "$sysconfdir" = "NONE" ; then
        sysconfdir="$prefix/etc"
    fi

    # Header installation directory.
    if test -z "$includedir" || test "$includedir" = "NONE" ; then
        includedir="$prefix/include"
    fi

    # Non-GCC header installation directory.
    if test -z "$oldincludedir" || test "$oldincludedir" = "NONE" ; then
        oldincludedir="/usr/include"
    fi

    # Object and library installation directory.
    if test -z "$libdir" || test "$libdir" = "NONE" ; then
        libdir="$exec_prefix/lib"
    fi

    # State file installation directory.
    if test -z "$sharedstatedir" || test "$sharedstatedir" = "NONE" ; then
        sharedstatedir="$prefix/com"
    fi

    # Machine-specific state file installation directory.
    if test -z "$localstatedir" || test "$localstatedir" = "NONE" ; then
        localstatedir="$prefix/var"
    fi

    # Data installation directory.
    if test -z "$datadir" || test "$datadir" = "NONE" ; then
        datadir="$prefix/share"
    fi

    # Manual page installation directory.
    if test -z "$mandir" || test "$mandir" = "NONE" ; then
        mandir="$prefix/man"
    fi

    # Info file installation directory.
    if test -z "$infodir" || test "$infodir" = "NONE" ; then
        infodir="$prefix/info"
    fi

    # Emacs lisp file installation directory.
    if test -z "$lispdir" || test "$lispdir" = "NONE" ; then
        lispdir="$prefix/share/emacs/site-lisp"
    fi

    if test -z "$PACKAGE" ; then
        PACKAGE="Resurrection"
    fi

    # Package object and library installation directory.
    if test -z "$pkglibdir" || test "$pkglibdir" = "NONE" ; then
        pkglibdir="$libdir/$PACKAGE"
    fi

    # Package header installation directory.
    if test -z "$pkgincludedir" || test "$pkgincludedir" = "NONE" ; then
        pkgincludedir="$includedir/$PACKAGE"
    fi

    # Package data installation directory.
    if test -z "$pkgdatadir" || test "$pkgdatadir" = "NONE" ; then
        pkgdatadir="$datadir/$PACKAGE"
    fi
])

AC_DEFUN([R_SET_PATHS],
[
    R_DIST_ROOT=`pwd`
    AC_SUBST([R_DIST_ROOT])

    R_SET_INSTALL_DIRS

    # header path
    AM_CPPFLAGS=-I$R_DIST_ROOT/include $INCLUDE
    AC_SUBST([AM_CPPFLAGS])

    # Image installation directories.
    imagedir="$pkgdatadir/images"
    AC_SUBST([imagedir])
    backgroundimagedir="$imagedir/background"
    AC_SUBST([backgroundimagedir])
    miscimagedir="$imagedir/misc"
    AC_SUBST([miscimagedir])
    wmimagedir="$imagedir/wm"
    AC_SUBST([wmimagedir])
    widgetimagedir="$imagedir/widget"
    AC_SUBST([widgetimagedir])
    pulseimagedir="$imagedir/pulse"
    AC_SUBST([pulseimagedir])

    # Font installation directories
    fontdir="$pkgdatadir/fonts"
    AC_SUBST([fontdir])
    ttffontdir="$fontdir/ttf"
    AC_SUBST([ttffontdir])

    resurrection_subst_dirs="$R_DIST_ROOT/aux/substdirs.make"
    AC_SUBST_FILE([resurrection_subst_dirs])
])

AC_DEFUN([R_PRINT_INFO],
[
    echo "----"
    if test -n "$r_configure_opts" ; then
        echo "configure options: $r_configure_opts"
    fi
    echo "package:  $PACKAGE/$VERSION"
    echo "host:     $host"
    echo "compiler: $CC"
    echo "flags:    $CFLAGS"
    echo "preproc   $CPP $CPPFLAGS"
    echo "prefix:   $prefix"
    echo "includes: $AM_CPPFLAGS"
    echo "libs:     $LIBS"
    echo "----"
    echo "do not forget to read README"
    echo "----"
    echo "now type 'make' to build the package"
])

