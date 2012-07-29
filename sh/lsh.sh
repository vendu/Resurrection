#! /bin/sh
#
# lsh.sh - shell environment initialization for Lightning.
#
# <NOTE>
# This file borrows from GNU Autoconf and Automake.
# </NOTE>
#
# Variables:
#     lsh_emulation		- shell emulation type.
#     lsh_exec_exts		- list of executable extensions.
#     lsh_exithandlers		- list of exit handlers
#     lsh_pkgdatadir		- shell utility directory.
#     lsh_processid		- shell process id.
#     lsh_scriptname		- name of script being executed.
#     lsh_tmpfiles		- list of temporary files.
#
# Shell commands and programs:
#     lsh_awk			- AWK interpreter.
#     lsh_basename		- get file base name.
#     lsh_cc			- C compiler.
#     lsh_cpp			- C preprocessor.
#     lsh_ld			- linker.
#     lsh_expr			- expr.
#     lsh_install		- install program.
#     lsh_symlink		- command to create symbolic links.
#     lsh_mkpath			- command to create directories.
#     lsh_sed			- sed.
#     lsh_uname			- uname.
#     lsh_unset			- unset variable
#
# Exit status:
#      1 - cannot access file			- lsh_erracces
#      2 - command failed			- lsh_errcmd
#      3 - file exists				- lsh_errexist
#      4 - inappropriate file type or format	- lsh_errftype
#      5 - invalid argument			- lsh_errinval
#      6 - input/output error			- lsh_errio
#      7 - is a directory			- lsh_errisdir
#      8 - too many symbolic links		- lsh_errloop
#      9 - command not found			- lsh_errnocmd
#     10 - no such device			- lsh_errnodev
#     11 - no such file or directory		- lsh_errnoent
#     12 - not executable			- lsh_errnoexec
#     13 - block device required		- lsh_errnotblk
#     14 - not a directory			- lsh_errnotdir
#     15 - directory not empty			- lsh_errnotempty
#     16 - not a socket				- lsh_errnotsock
#     17 - not a terminal			- lsh_errnotty
#     18 - broken pipe				- lsh_errpipe
#
# TODO: write lsh_prog_cpp, lsh_prog_egrep, ...
#       add libtool support flags for compiler/linker.
#       add traps for cleaning up temporary files.
#       add error handling.
#       implement lsh_create_rcfile.
#       clean up the function interfaces.

lsh_opt_debug="yes"

lsh_version="0.0.0"
Lightning_pkgdatadir="@pkgdatadir@"
if test -z "$lsh_pkgdatadir" ; then
    lsh_pkgdatadir="@lshdir@"
fi

lsh_initialized="0"

lsh_init() {
    lsh_processid=

    if test "$lsh_initialized" -eq 1 ; then

        return 0
    fi

    lsh_processid="$$"

    if test -z "$Lightning_pkgdatadir" || test "$Lightning_pkgdatadir" = "@pkgdatadir@" ; then
        Lightning_pkgdatadir=`pwd`
    fi
    if test -z "$lsh_pkgdatadir" || test "$lsh_pkgdatadir" = "@lshdir@" ; then
        lsh_pkgdatadir="$Lightning_pkgdatadir/lsh"
    fi

    lsh_init_traps
    lsh_init_emulation
    lsh_init_cmds
    lsh_init_vars
    lsh_init_defaults

    lsh_add_exit_handler "lsh_cleanup"
    lsh_init_scriptname
    lsh_init_host
    lsh_init_progs
    lsh_init_features

    return 0;
}

# System V Release 4.2 shell is said not to allow trapping of SIGSEGV, SIGALRM,
# or SIGCHLD.
lsh_init_traps() {
    lsh_exit_handlers=
    lsh_tmpfiles=
    lsh_exec_exts=

    lsh_erracces=1
    lsh_errcmd=2
    lsh_errexist=3
    lsh_errftype=4
    lsh_errinval=5
    lsh_errio=6
    lsh_errisdir=7
    lsh_errloop=8
    lsh_errnocmd=9
    lsh_errnodev=10
    lsh_errnoent=11
    lsh_errnoexec=12
    lsh_errnotblk=13
    lsh_errnotdir=14
    lsh_errnotempty=15
    lsh_errnotsock=16
    lsh_errnotty=17
    lsh_errpipe=18
    

    # The signals that are known to be safe to trap.
    lsh_TRAPEXIT=0
    lsh_SIGHUP=1
    lsh_SIGINT=2
    lsh_SIGPIPE=13
    lsh_SIGTERM=15

    # Other signals.
    lsh_SIGQUIT=3
    lsh_SIGILL=4
    lsh_SIGABRT=6
    lsh_SIGFPE=8
    lsh_SIGKILL=9
    lsh_SIGSEGV=11
    lsh_SIGALRM=14

    lsh_trap 'lsh_exit "$?"' "$lsh_TRAPEXIT"
    lsh_trap 'lsh_trap_sighup "$?"' "$lsh_SIGHUP"
    lsh_trap 'lsh_trap_sigint "$?"' "$lsh_SIGINT"
    lsh_trap 'lsh_trap_sigpipe "$?"' "$lsh_SIGPIPE"
    lsh_trap 'lsh_trap_sigterm "$?"' "$lsh_SIGTERM"

    return 0;
}

lsh_init_emulation() {
    lsh_emulation=

    if test -n "${ZSH_VERSION+set}" && (emulate sh) > /dev/null 2>&1 ; then
        # Make zsh emulate Bourne shell.
        if test "$lsh_opt_debug" = "yes" ; then
            echo "setting Zsh $ZSH_VERSION to emulate Bourne shell."
        fi
        emulate sh
        NULLCMD=":"
        alias -g '${1+"$@"}'='"$@"'
        lsh_emulation="sh"
    elif test -n "${BASH_VERSION+set}" \
         && (set -o posix) > /dev/null 2>&1 ; then
        if test "$lsh_opt_debug" = "yes" ; then
            echo "setting Bash $BASH_VERSION to emulate POSIX shell."
        fi
        set -o posix
        lsh_emulation="posix"
    fi

    return 0;
}

lsh_init_cmds() {
    lsh_unset="false"

    if (LSHTEST="lsh" ; unset LSHTEST) > /dev/null 2>&1 ; then
        lsh_unset="unset"
    fi

    if test "$lsh_opt_debug" = "yes" ; then
        echo "lsh_init_cmds:"
        echo "    lsh_unset: $lsh_unset"
    fi

    return 0;
}

lsh_init_vars() {
    newline='
'
    save_IFS=
    dot=
    path=

    # Unset search path for cd with relative pathnames.
    $lsh_unset CDPATH

    # Disable Balsh script startup file.
    $lsh_unset BASH_ENV

    # We need space, tab and new line, in precisely that order.
    IFS=" 	$newline"

    # Set locale to "C".
    for lc_var in \
        LANG LANGUAGE LC_ADDRESS LC_ALL LC_COLLATE LC_CTYPE LC_IDENTIFICATION \
        LC_MEASUREMENT LC_MESSAGES LC_MONETARY LC_NAME LC_NUMERIC LC_PAPER \
        LC_TELEPHONE LC_TIME
    do
        if (set +x ; test -n "`(eval $lc_var="C" \
           ; export $lc_var) 2>&1`") ; then
            eval $lc_var="C" ; export $lc_var
        else
            $lsh_unset $lc_var
        fi
    done

    # Work around bugs in pre-3.0 UWIN ksh.
    $lsh_unset ENV
    $lsh_unset MAIL
    $lsh_unset MAILPATH
    PS1="$ "
    PS2="> "
    PS4="+ "

    # Try to set TMPDIR.
    if test -z "${TMPDIR+set}" ; then
        if test -d "/tmp" && test -w "/tmp" ; then
            TMPDIR="/tmp"
        elif test -d ~ && test -w ~ ; then
            TMPDIR=~
        elif test -d "\\temp" && test -w "\\temp" ; then
            TMPDIR="\temp"
        else
            $lsh_unset TMPDIR
        fi
    fi

    # Set PATH_SEPARATOR.
    if test "${PATH_SEPARATOR+set}" != "set" ; then
        lsh_add_tmpfile "$TMPDIR/$$.lsh"
        echo "#! /bin/lsh" > $TMPDIR/$$.lsh
        echo "(exit 0)" >> $TMPDIR/$$.lsh
        echo "exit 0" >> $TMPDIR/$$.lsh
        chmod +x $TMPDIR/$$.lsh
        if (PATH="/nonexistent;$TMPDIR" ; $$.lsh) > /dev/null 2>&1 ; then
            PATH_SEPARATOR=";"
        else
            PATH_SEPARATOR=":"
        fi
        lsh_remove_tmpfile "$TMPDIR/$$.lsh"
    fi

    # Trim PATH.
    save_IFS="$IFS"
    dot=
    path=
    IFS="$PATH_SEPARATOR"
    for dir in $PATH
    do
        IFS="$save_IFS"
        dir=`echo "$dir" | sed 's,/*$,,'`
	if test -z "$dir" ; then
            dot="."
        elif test "$dir" = "." ; then
            dot="."
        elif test -z "$path" ; then
            path="$dir"
        else
            if test "$path" = "$dir" || echo "$path" | grep -E "^$dir$PATH_SEPARATOR" > /dev/null || echo "$path" | grep -E "$PATH_SEPARATOR$dir$PATH_SEPARATOR" > /dev/null || echo "$path" | grep -E "$PATH_SEPARATOR$dir$" > /dev/null ; then
                :
            else
                path="$path$PATH_SEPARATOR$dir"
            fi
        fi
    done
    if test -z "$path" && test -n "$dot" ; then
        path="$dot"
    elif test -n "$dot" ; then
        path="$path$PATH_SEPARATOR$dot"
    fi
    PATH="$path"

    return 0;
}

lsh_init_defaults() {
    automakepkgdatadir=

    lsh_lowercase_letters="abcdefghijklmnopqrstuvwxyzåäö"
    lsh_uppercase_letters="ABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖ"
    lsh_letters="$lsh_lowercase_letters$lsh_uppercase_letters"
    lsh_digits="0123456789"
    lsh_alnum="$lsh_letters$lsh_digits"

    # Set path so we can find config.guess and other GNU scripts.
    lsh_path="/usr/bin$PATH_SEPARATOR/bin$PATH_SEPARATOR/usr/local/bin$PATH_SEPARATOR/usr/ccs/bin$PATH_SEPARATOR/usr/contrib/bin$PATH_SEPARATOR/usr/ucb/bin$PATH_SEPARATOR/usr/xpg4/bin"
    PATH="$lsh_pkgdatadir$PATH_SEPARATOR$PATH$PATH_SEPARATOR$lsh_path"
    if test -f "$Lightning_pkgdatadir/config_aux/config.guess" ; then
        PATH="$PATH$PATH_SEPARATOR$Lightning_pkgdatadir/config_aux"
    else
        lsh_config_guess=`locate config.guess | sed -n 's,\([./]*automake[-'$lsh_digits'.]*\),\1,p' | tail -1`
        automakepkgdatadir=`dirname $lsh_config_guess`
        PATH="$PATH$PATH_SEPARATOR$automakepkgdatadir"
    fi

    return 0;
}

lsh_init_scriptname() {
    lsh_scriptname=`echo "$0" | sed 's,^.*/,,'`
#   lsh_scriptname=`echo "$lsh_scriptname" | sed 's,[.]lsh$,,'`

    return 0;
}

lsh_init_host() {
    lsh_init_hostname
    lsh_init_uname
    lsh_init_hosttype

    if test "$lsh_opt_debug" = "yes" ; then
        echo "lsh_init_host: $hostname: $hosttype"
    fi

    return 0;
}

lsh_init_hostname() {
    hostname=`(hostname || uname -n || echo "unknown") 2> /dev/null | sed 1q`

    return 0;
}

lsh_init_uname() {
    uname_machine=`uname -m` || uname_machine="unknown"
    uname_processor=`uname -p` || uname_processor="unknown"
    uname_system=`uname -s` || uname_system="unknown"
    uname_release=`uname -r` || uname_release="unknown"
    uname_version=`uname -v` || uname_version="unknown"

    return 0;
}

lsh_init_hosttype() {
    hosttype_alias=`config.guess`
    hosttype=`config.sub $hosttype_alias`
    hostcpu=`echo $hosttype | sed 's,^\([^-]*\)-\([^-]*\)-\(.*\)$,\1,'`
    hostvendor=`echo $hosttype | sed 's,^\([^-]*\)-\([^-]*\)-\(.*\)$,\2,'`
    hostos=`echo $hosttype | sed 's,^\([^-]*\)-\([^-]*\)-\(.*\)$,\3,'`

    return 0;
}

# TODO: look for echo replacements.
lsh_init_progs() {
    lsh_prog_awk
    lsh_prog_basename
    lsh_prog_cc
    lsh_prog_cpp
    lsh_prog_ld
    lsh_prog_expr
    lsh_prog_install
    lsh_prog_ln_s
    lsh_prog_mkdir_p
    lsh_prog_sed
    lsh_prog_uname

    if test "$lsh_opt_debug" = "yes" ; then
        echo "lsh_init_progs:"
        echo "    lsh_awk:      $lsh_awk"
        echo "    lsh_basename: $lsh_basename"
        echo "    lsh_cc:       $lsh_cc"
        echo "    lsh_cpp:      $lsh_cpp"
        echo "    lsh_ld:       $lsh_ld"
        echo "    lsh_expr:     $lsh_expr"
        echo "    lsh_install:  $lsh_install"
        echo "    lsh_symlink:  $lsh_symlink"
        echo "    lsh_mkpath:   $lsh_mkpath"
        echo "    lsh_sed:      $lsh_sed"
        echo "    lsh_uname:    $lsh_uname"
    fi

    return 0;
}

lsh_prog_awk() {
    lsh_awk="false"
    save_IFS=

    for prog in gawk mawk nawk awk
    do
        if test -n "$AWK" ; then
            lsh_awk="$AWK"
        else
            save_IFS="$IFS"
            IFS="$PATH_SEPARATOR"
            for dir in $PATH
            do
                IFS="$save_IFS"
                test -z "$dir" && dir="."
                for ext in "" $lsh_exec_exts
                do
                    if test -f "$dir/$prog$ext" ; then
                        lsh_awk="$dir/$prog$ext"
                        break 2
                    fi
                done
            done
        fi
    done

    return 0;
}

lsh_prog_basename() {
    lsh_basename="false"

    if (basename /) > /dev/null 2>&1 && test "x`basename / 2>&1`" = "x/" ; then
        lsh_basename="`which basename`"
    fi

    return 0;
}

#
# <FIXME>
# - add checks to make sure CC actually works.
# </FIXME>
#
lsh_prog_cc() {
    lsh_cc="false"
    cc_supports_ansi=
    cc_opts_ansi=

    progname="$TMPDIR/$$"
    progfiles="$progname $progname.c $progname.o $progname.rel"
    status=
    checkcmd=

    lsh_add_tmpfiles "$progfiles"

    # Check for compiler.
    case $CC_FOR_BUILD,$HOST_CC,$CC in
        ,,)
            echo "int x;" > $progname.c
            for cc in gcc cc c89 c99 cl
            do
                if ($cc -c -o $progname.o $progname.c) > /dev/null 2>&1 ; then
                    lsh_cc=`which $cc`
                    cc_opts_o="-c -o"
                    CC="$lsh_cc"
                    CC_FOR_BUILD="$CC"
                    HOST_CC="$CC"
                    break
                elif ($cc -o $progname.o $progname.c) > /dev/null 2>&1 ; then
                    lsh_cc=`which $cc`
                    cc_opts_o="-o"
                    CC="$lsh_cc"
                    CC_FOR_BUILD="$lsh_cc"
                    HOST_CC="$lsh_cc"
                    break
                fi
            done
            rm -f "$progfiles"
            ;;
        ,,*)
            lsh_cc="$CC"
            CC_FOR_BUILD="$lsh_cc"
            HOST_CC="$lsh_cc"
            ;;
        ,*,*)
            lsh_cc="$HOSTCC"
            CC="$lsh_cc"
            CC_FOR_BUILD="$lsh_cc"
            ;;
    esac

    if test -z "$CC" ; then

        return 1
    fi

    # Check for compiler flag to accept ANSI C.
    cat > $progname.c <<EOF
#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
/* Most of the following tests are stolen from RCS 5.7's src/conf.lsh.  */
int argc;
char **argv;

struct buf { int x; };
struct s1 { int (*f) (int a); };
struct s2 { int (*f) (double a); };

FILE * (*rcsopen)(struct buf *, struct stat *, int);
int test(int i, double x);
int pairnames(int, char **, FILE *(*)(struct buf *, struct stat *, int), int, int);

static char *e(p, i)
    char **p;
    int i;
{
    return p[i];
}

static char *f(char * (*g) (char **, int), char **p, ...)
{
    char *s;
    va_list v;
    va_start(v,p);
    s = g(p, va_arg(v,int));
    va_end(v);

    return s;
}

int
main ()
{
    return (f(e, argv, 0) != argv[0] || f(e, argv, 1) != argv[1]);
    ;

    return 0;
}

EOF

    # Don't try gcc -ansi; that turns off useful extensions and
    # breaks some systems' header files.
    # AIX                   -qlanglvl=ansi
    # Ultrix and OSF/1      -std1
    # HP-UX 10.20 and later -Ae
    # HP-UX older versions  -Aa -D_HPUX_SOURCE
    # SVR4                  -Xc -D__EXTENSIONS__
    for opts in "" -qlanglvl=ansi -std1 -Ae \
        "-Aa -D_HPUX_SOURCE" "-Xc -D__EXTENSIONS__"
    do
        rm -f "$progname"
        if {
            ($CC $opts $cc_opts_o $progname $progname.c) 2> /dev/null
            status="$?"
            (exit $status);
        } && {
            checkcmd="test -s $progname"
            {
                (eval $checkcmd) 2> /dev/null
                status="$?"
                (exit $status);
            }
        } ; then
            cc_supports_ansi="yes"
            cc_opts_ansi="$opts"
            break
        fi
    done

    lsh_remove_tmpfiles "$progfiles"

    return 0;
}

# TODO: suggest additional cpp paths to the GNU Autoconf team. :)
lsh_prog_cpp() {
    lsh_cpp="false"

    cppsuccess=
    cppfailure=
    srcfile="$TMPDIR/$$.c"
    status=

    lsh_add_tmpfile "$srcfile"

    # CPP may name a directory on some systems (Suns).
    if test -n "$CPP" && test -d "$CPP" ; then
        CPP=
    fi

    if test -z "$CPP" ; then
        for cpp in "$CC -E" "$CC -E -traditional-cpp" "cpp" "/lib/cpp" \
            "/lib/pcpp" "/usr/bin/cpp" "/usr/lib/cpp" "/usr/libexec/cpp" \
            "/usr/ccs/lib/cpp" "/usr/X11R6/bin/cpp"
        do
            cppsuccess="no"
            cppfailure="no"

            # Use a header file that comes with gcc, so configuring glibc
            # with a frelsh cross-compiler works.
            # Prefer <limits.h> to <assert.h> if __STDC__ is defined, since
            # <limits.h> exists even on freestanding compilers.
            cat > $srcfile <<EOF
#line 666 "lsh"
#if defined(__STDC__)
#   include <limits.h>
#else
#   include <assert.h>
#endif
    Syntax error
EOF
            if {
                ($cpp $CPPFLAGS $srcfile) > /dev/null 2>&1
                status="$?"
                (exit $status)
            } && cppsuccess="yes" ; then
                :
            fi

            if {            
                cat > $srcfile <<EOF
#include <lsh_dummy.h>
EOF
                ($cpp $CPPFLAGS $srcfile) > /dev/null 2>&1
                status="$?"
                (exit $status)
            } || cppfailure="yes" ; then
                :
            fi

            rm -f $srcfile

            if test "$cppsuccess" = "yes" && test "$cppfailure" = "yes" ; then
                lsh_cpp="$cpp"
                CPP="$lsh_cpp"
                break
            fi
        done
    else
        lsh_cpp="$CPP"
    fi

    lsh_remove_tmpfile "$srcfile"

    return 0;
}

# The QNX 4.25 `expr', in addition of preferring `0' to the empty
# string, has a funny behavior in its exit status: it's always 1
# when parentheses are used!
lsh_prog_expr() {
    lsh_expr="false"

    if expr a : '\(a\)' > /dev/null 2>&1 ; then
        lsh_expr="`which expr`"
    fi

    return 0;
}

# Find a good install program.  We prefer a C program (faster),
# so one script is as good as another.  But avoid the broken or
# incompatible versions:
# SysV /etc/install, /usr/sbin/install
# SunOS /usr/etc/install
# IRIX /sbin/install
# AIX /bin/install
# AmigaOS /C/install, which installs bootblocks on floppy discs
# AIX 4 /usr/bin/installbsd, which doesn't work without a -g flag
# AFS /usr/afsws/bin/install, which milshandles nonexistent args
# SVR4 /usr/ucb/install, which tries to use the nonexistent group "staff"
# ./install, which can be erroneously created by make from ./install.lsh.
#
# <FIXME>
# - look for shell scripts in some sane place. :)
# </FIXME>
lsh_prog_install() {
    lsh_install="false"

    local save_IFS="$IFS"
    IFS="$PATH_SEPARATOR"
    for dir in $PATH
    do
        IFS="$save_IFS"
        test -z "$dir" && dir="."
        # Account for people who put trailing slalshes in PATH elements.
        case $dir/ in
            ./ | .// | /cC/* \
            | /etc/* | /usr/sbin/* | /usr/etc/* | /sbin/* | /usr/sfsws/bin/* \
            | /usr/ucb/*)
                ;;
            *)
            # OSF1 and SCO ODT 3.0 have their own names for install.
            # Don't use installbsd from OSF since it installs stuff as root
            # by default.
                for prog in ginstall scoinst install
                do
                    for ext in "" $lsh_exec_exts
                    do
                        if test -f "$dir/$prog$ext" > /dev/null 2>&1 ; then
                            if test "$prog" = "install" \
                               && grep dspmsg "$dir/$prog$ext" \
                                  >& /dev/null 2>&1 ; then
                                # AIX install. Incompatible calling convention.
                                :
                            elif test "$prog" = "install" \
                                 && grep pwplus "$dir/$prog$ext" \
                                    > /dev/null 2>&1 ; then
                                # program-specific install script used by
                                # HP pwplus - don't use.
                                :
                            else
                                lsh_install="$dir/$prog$ext"
                                break 3
                            fi
                        fi
                    done
                done
                ;;
        esac
    done

    if test -z "${lsh_install+set}" ; then
        if test -f "$Lightning_pkgdatadir/config_aux/install-lsh" ; then
            lsh_install="$Lightning_pkgdatadir/config_aux/install-lsh -c"
        elif test -f "$Lightning_pkgdatadir/config_aux/install.lsh" ; then
            lsh_install="$Lightning_pkgdatadir/config_aux/install.lsh -c"
        elif test -f "$Lightning_pkgdatadir/config_aux/lshtool" ; then
            lsh_install="$Lightning_pkgdatadir/config_aux/lshtool install -c"
        fi
    fi

    return 0;
}

lsh_prog_ld() {
    lsh_ld="false"

    prog="ld"
    save_IFS=

    # First see if (g)cc -print-prog-name=ld works.
    case $hosttype in
        *-*-mingw*)
            # gcc leaves a trailing carriage return, which upsets mingw.
            prog=`($CC -print-prog-name=ld) 2> /dev/null | tr -d '\015'`
            ;;
        *)
            prog=`($CC -print-prog-name=ld)` 2> /dev/null
            ;;
    esac

    case $prog in
        # Accept absolute paths.
        "")
            prog="ld"
            ;;
        *)
            test -z "$LD" && LD="`which $prog`"
            ;;
    esac

    if test -z "$LD" ; then
        save_IFS="$IFS"
        IFS="$PATH_SEPARATOR"
        for dir in $PATH
        do
            IFS="$save_IFS"
            test -z "$dir" && dir="."
            for ext in "" $lsh_exec_exts
            do
                if test -f "$dir/$prog$ext" ; then
                    lsh_ld="$dir/$prog$ext"
                    echo "$lsh_ld"
                    break 2
                fi
            done
        done
    else
        lsh_ld="$LD"
    fi

    return 0;
}

lsh_prog_ln_s() {
    lsh_symlink="false"

    tmpfiles="$TMPDIR/$$.tmp $TMPDIR/$$"

    lsh_add_tmpfiles "$tmpfiles"

    #
    # <FIXME>
    # - fix this for DJGPP; see Autoconf configure scripts.
    # </FIXME>
    #
    echo > $TMPDIR/$$.tmp
    if ln -s $TMPDIR/$$.tmp $TMPDIR/$$ 2> /dev/null ; then
        lsh_symlink="`which ln` -s"
    else
        lsh_symlink="`which cp` -p"
    fi

    lsh_remove_tmpfiles "$tmpfiles"

    return 0;
}

lsh_prog_mkdir_p() {
    lsh_mkpath="false"

    if mkdir -p . 2> /dev/null ; then
        lsh_mkpath="`which mkdir` -p"
    fi

    return 0;
}

lsh_prog_sed() {
    lsh_sed="false"

    local save_IFS="$IFS"
    list=
    tmpdir=
    max=
    count=

    IFS="$PATH_SEPARATOR"
    for dir in $PATH
    do
        IFS="$save_IFS"
        test -z $dir && dir="."
        for prog in sed gsed
        do
            for ext in "" $lsh_exec_exts
            do
                if test -f "$dir/$prog$ext" ; then
                    list="$list $dir/$prog$ext"
                fi
            done
        done
    done

    {
        tmpdir=`(umask 077 && mktemp -d -q "$TMPDIR/sedXXXXXX") \
                 2> /dev/null` \
                 && test -n "$tmpdir" && test -d "$tmpdir"
    } || {
        tmpdir="$TMPDIR/sed$$-$RANDOM"
        (umask 077 && mkdir $tmpdir)
    } || {

        return 1
    }
    lsh_add_tmpfile "$tmpdir"

    max=0
    count=0
    # /usr/xpg4/bin/sed is typically found on Solaris along with /bin/sed which
    # truncates output.
    for prog in $list
    do
        test ! -f "$prog" && break
        cat /dev/null > "$tmpdir/sed.in"
        count=0
        echo "$lsh_digits" > "$tmpdir/sed.in"

        # Check for GNU sed and select it if it's found.
        if "$prog" --version 2>&1 < /dev/null | grep "GNU" > /dev/null ; then
            lsh_sed=`which $prog`
            break
        fi

        while true
        do
            cat "$tmpdir/sed.in" "$tmpdir/sed.in" > "$tmpdir/sed.tmp"
            mv "$tmpdir/sed.tmp" "$tmpdir/sed.in"
            cp "$tmpdir/sed.in" "$tmpdir/sed.nl"
            echo >> "$tmpdir/sed.nl"
            $prog -e 's/a$//' < "$tmpdir/sed.nl" > "$tmpdir/sed.out" || break
            cmp -s "$tmpdir/sed.out" "$tmpdir/sed.nl" || break

            test "$count" -gt 8 && break
            count=`$lsh_expr $count + 1`
            if test "$count" -gt "$max" ; then
                max="$count"
                lsh_sed=`which $prog`
            fi
        done
    done

    lsh_remove_tmpfile "$tmpdir"

    return 0;
}

lsh_prog_uname() {
    # Hack for Pyramid OS/BSD.
    if (test -f /.attbin/uname) > /dev/null 2>&1 ; then
        lsh_uname="/.attbin/uname"
    else
        lsh_uname=`which uname`
    fi

    return 0;
}

lsh_init_features() {
    shell_supports_lineno=

    lineno1=$LINENO
    lineno2=$LINENO
    lineno3=`($lsh_expr $lineno1 + 1) 2>/dev/null`
    if test "$lineno1" != "$lineno2" && test "$lineno3" = "$lineno2" ; then
        shell_supports_lineno="yes"
    else
        shell_supports_lineno="no"
    fi

    return 0;
}

lsh_abort() {
    echo "$lsh_scriptname: $2"

    lsh_exit "$1";
}

lsh_error() {
    echo "$1"
    lsh_cleanup

    return 0;
}

lsh_trap() {
    commands='$1'
    signal="$2"

    case $signal in
        EXIT)
            signal=0
            ;;
        HUP | SIGHUP)
            signal=1
            ;;
        INT | SIGINT)
            signal=2
            ;;
        QUIT | SIGQUIT)
            signal=3
            ;;
        ILL | SIGILL)
            signal=4
            ;;
        ABRT | SIGABRT)
            signal=6
            ;;
        FPE | SIGFPE)
            signal=8
            ;;
        KILL | SIGKILL)
            signal=9
            ;;
        SEGV | SIGSEGV)
            signal=11
            ;;
        ALRM | SIGALRM)
            signal=14
            ;;
        PIPE | SIGPIPE)
            signal=13
            ;;
        TERM | SIGTERM)
            signal=15
            ;;
    esac

    case $signal in
        0 | 1 | 2 | 13 | 15)
            ;;
        9 | 14 | CHLD | SIGCHLD)
            echo "$lsh_scriptname: not all shells allow trapping of $signal"
            ;;
        *)
            echo "$lsh_scriptname: it may not be safe/portable to trap $signal"
            ;;
    esac

    trap "$@"

    return 0;
}

lsh_trap_sighup() {
    lsh_exit "$lsh_SIGHUP";
}

lsh_trap_sigint() {
    lsh_exit "$lsh_SIGINT";
}

lsh_trap_sigpipe() {
    lsh_exit "$lsh_SIGPIPE";
}

lsh_trap_sigterm() {
    lsh_exit "$lsh_SIGTERM";
}

lsh_exit() {
    if test -n "$lsh_exit_handlers" ; then
        for handler in $lsh_exit_handlers
        do
            $handler "$1"
        done
        lsh_exit_handlers=
    fi

    (exit "$1")
    exit "$1";
}

lsh_add_exit_handler() {
    handler="$1"

    if test -n "$handler" ; then
        if test -z "$lsh_exit_handlers" ; then
            lsh_exit_handlers="$handler"
        else
            lsh_exit_handlers="$lsh_exit_handlers $handler"
        fi
    fi

    return 0;
}

lsh_remove_exit_handler() {
    handler="$1"
    exithandlers=

    if test -n "$handler" && test -n "$lsh_exit_handlers" ; then
        for exithandler in $lsh_exit_handlers
        do
            if test "$exithandler" = "$handler" ; then
                :
            elif test -z "$exithandlers" ; then
                exithandlers="$exithandler"
            else
                exithandlers="$exithandlers $exithandler"
            fi
        done
        lsh_exit_handlers="$exithandlers"
    fi

    return 0;
}

lsh_cleanup() {
    if test -n "$lsh_tmpfiles" ; then
        for tmpfile in $lsh_tmpfiles
        do
            if test -d "$tmpfile" ; then
                rm -rf "$tmpfile"
            else
                rm -f "$tmpfile"
            fi
        done
        $lsh_tmpfiles=
    fi

    return 0;
}

lsh_add_tmpfile() {
    tmpfile="$1"

    if test -n "$tmpfile" ; then
        if test -z "$lsh_tmpfiles" ; then
            lsh_tmpfiles="$tmpfile"
        else
            lsh_tmpfiles="$lsh_tmpfiles $tmpfile"
        fi
    fi

    return 0;
}

lsh_add_tmpfiles() {
    tmpfiles="$@"

    if test -n "$tmpfiles" ; then
        for tmpfile in $tmpfiles
        do
            lsh_add_tmpfile "$tmpfile"
        done
    fi

    return 0;
}

lsh_remove_tmpfile() {
    file="$1"
    tmpfiles=

    if test -n "$lsh_tmpfiles" ; then
        for tmpfile in $lsh_tmpfiles
        do
            if test "$tmpfile" = "$file" ; then
                if test -d "$tmpfile" ; then
                    rm -rf "$tmpfile"
                else
                    rm -f "$tmpfile"
                fi
            else
                if test -z "$tmpfiles" ; then
                    tmpfiles="$tmpfile"
                else
                    tmpfiles="$tmpfiles $tmpfile"
                fi
            fi
        done
        lsh_tmpfiles="$tmpfiles"
    fi

    return 0;
}

lsh_remove_tmpfiles() {
    tmpfiles="$@"

    if test -n "$lsh_tmpfiles" ; then
        for tmpfile in $tmpfiles
        do
            lsh_remove_tmpfile "$tmpfile"
        done
    fi

    return 0;
}

lsh_message() {
    echo "$lsh_scriptname: $@"

    return 0;
}

lsh_load_autogen() {
    . $lsh_pkgdatadir/autogen.sh

    return 0;
}

lsh_var_name() {
    shell_varname=`echo $1 | sed 'y,*+,pp,;s,[^_$lsh_alnum],_,g'`

    return 0;
}

lsh_cpp_name() {
    cpp_varname=`echo $1 | sed 'y,*$lsh_lowercase_letters,P$lsh_uppercase_letters,;s,[^_$lsh_alnum],_,g'`

    return 0;
}

