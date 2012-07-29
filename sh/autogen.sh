#! /bin/sh
#
# autogen.sh - GNU build system preparation for Lightning.
# Copyright (C) 2003 - 2004 Tuomo Venäläinen
#
# See the file COPYING for information about using this software.
#
# This module uses the following GNU tools: Automake, Autoconf, gettext,
# intltool, and libtool.
#
# <WARNING>
# - do NOT use the -C and -D flags unless you have read the function
#   autogen_clean.
# </WARNING>
#
# <TODO>
# - update the list below.
# </TODO>
#
# The following is a list of files installed by GNU build tools that should be
# distributed with software packages:
#     Makefile.am
#     Makefile.in
#     aclocal.m4
#     config.guess
#     config.sub
#     configure
#     configure.ac
#     depcomp
#     install-sh
#     intltool-extract.in
#     intltool-merge.in
#     intltool-update.in
#     libtool
#     ltmain.sh
#     missing
#     mkinstalldirs
#
# <TODO>
# - support ACLOCAL_AMFLAGS="-I m4dir" in Makefile.am.
# - clean up the function interfaces.
# </TODO>
#

# GNU build tool command-line options.
aclocal_opts=
autoconf_opts=
autoheader_opts=
#case $CC in
#xlc) automake_opts="--include-deps" ;;
#esac
automake_opts="--copy --add-missing"
autopoint_opts=
configure_opts="$configure_default_opts"
gettextize_opts="--copy --no-changelog --intl"
intltoolize_opts="--copy --automake"
libtoolize_opts="--copy --automake"

autogen_version="0.0.0"
autogen_topdir=`pwd`
autogen_potfile_exts="c h"
autogen_potfile_excludedirs="intl po"

# autogen command-line options.
autogen_opt_autopoint=
autogen_opt_clean=
autogen_opt_configure=
autogen_opt_debug=
autogen_opt_dist_clean=
autogen_opt_dry_run=
autogen_opt_force=
autogen_opt_gettextize=
autogen_opt_intltoolize=
autogen_opt_make=
autogen_opt_silent=
autogen_opt_verbose=

HAVE_ACLOCAL=0
HAVE_AUTOCONF=0
HAVE_AUTOHEADER=0
HAVE_AUTOMAKE=0
HAVE_AUTOPOINT=0
HAVE_GETTEXT=0
HAVE_GETTEXTIZE=0
HAVE_INTLTOOLIZE=0
HAVE_LIBTOOL=0
HAVE_LIBTOOLIZE=0

USE_GNU_GETTEXT=0

autogen_main() {
    configure_ac_files=
    dir=

    autogen_get_opts $@ \
    && autogen_print_start_message \
    && autogen_check_tools \
    && {
        configure_ac_files=`find $autogen_topdir -name configure.ac`
        if test -n "$configure_ac_files" ; then
            for file in $configure_ac_files
            do
                dir=`dirname $file`
                if test "$autogen_opt_silent" != "yes" ; then
                    lsh_message "entering $dir"
                    lsh_message "'cd $dir'"
                fi
                cd $dir \
                && autogen_check_src \
                && autogen_clean \
                && autogen_create_version_files \
                && autogen_exec_tools \
                && autogen_create_potfiles_in
            done
            cd $autogen_topdir
        fi
    }

    return 0;
}

autogen_get_opts() {
    if test "$autogen_running" != "yes" ; then
        autogen_opt_silent="yes"
    fi
    for arg
    do
        case $arg in
            -A | --autopoint)
                if test -z "$autogen_opt_autopoint" ; then
                    autogen_opt_autopoint="yes"
                fi
                ;;
            -C | --clean)
                if test -z "$autogen_opt_clean" ; then
                    autogen_opt_clean="yes"
                fi
                ;;               
            -c | --configure)
                if test -z "$autogen_opt_configure" ; then
                    autogen_opt_configure="yes"
                fi
                ;;
            -d | --debug)
                if test -z "$autogen_opt_debug" ; then
                    lsh_message "enabling shell trace mode"
                    set -x
                    autogen_opt_debug="yes"
                fi
                ;;
            -D | --dist-clean)
                if test -z "$autogen_opt_dist_clean" ; then
                    autogen_opt_dist_clean="yes"
                    autogen_opt_clean="yes"
                fi
                ;;
            -n | --dry-run)
                if test -z "$autogen_opt_dry_run" ; then
                    autogen_opt_dry_run="yes"
                fi
                ;;
            -f | --force)
                if test -z "$autogen_opt_force" ; then
                    autoconf_opts="--force $autoconf_opts"
                    autoheader_opts="--force $autoheader_opts"
                    automake_opts="--force-missing $automake_opts"
                    gettextize_opts="--force $gettextize_opts"
                    intltoolize_opts="--force $intltoolize_opts"
                    libtoolize_opts="--force $libtoolize_opts"
                    autogen_opt_force="yes"
                fi
                ;;
            -G | --gettextize)
                if test -z "$autogen_opt_gettextize" ; then
                    autogen_opt_gettextize="yes"
                fi
                ;;
            -h | --help)
cat <<EOF
    Usage: $lsh_scriptname [option] ...

    Prepare $autogen_distribution for 'configure' or compile.

    -A, --autopoint    run 'autopoint'. Not run with 'gettextize'.
    -c, --configure    run 'configure'.
    -C, --clean        clean extra files and exit.
    -d, --debug        enable shell trace mode for debugging.
    -D, --dist-clean   clean non-distribution files and exit.
    -n, --dry-run      print commands without running them.
    -f, --force        force reconfiguration.
    -G, --gettextize   run 'gettextize'.
    -h, --help         print this help message.
    -I, --intltoolize  run 'intltoolize'.
    -m, --make         run 'make'.
    -q, -s, --quiet, --silent
                       work silently. Passed to GNU tools.
    -v, --verbose      be verbose. VERY noisy. Passed to GNU tools.
    -V, --version      print version information. Passed to GNU tools.

    Other options are passed to the 'configure' script.

    Default 'configure' options: $configure_default_opts
EOF
                return 1
                ;;
            -I | --intltoolize)
                if test -z "$autogen_opt_intltoolize" ; then
                    autogen_opt_intltoolize="yes"
                fi
                ;;
            -m | --make)
                if test -z "$autogen_opt_make" ; then
                    autogen_opt_make="yes"
                fi
                ;;
            -q | -s | --quiet | --silent)
                if test -z "$autogen_opt_silent" ; then
                    configure_opts="$configure_opts --silent"
                    autogen_opt_silent="yes"
                fi
                ;;
            -v | --verbose)
                #
                # <FIXME>:
                # - does --verbose apply to configure as well?
                # </FIXME>
                #
                if test -z "$autogen_opt_verbose" ; then
                    aclocal_opts="--verbose $aclocal_opts"
                    autoconf_opts="--verbose $autoconf_opts"
                    autoheader_opts="--verbose $autoheader_opts"
                    automake_opts="--verbose $automake_opts"
                    autogen_opt_verbose="yes"
                fi
                ;;
            -V | --version)
#                echo "autogen.sh $autogen_version"
                autogen_print_versions
                return 1
                ;;
             *)
                configure_opts="$configure_opts $arg"
                ;;
        esac
    done

    return 0;
}

autogen_print_versions() {
    aclocal --version | grep GNU
    autoconf --version | grep GNU
    autoheader --version | grep GNU
    automake --version | grep GNU
    autopoint --version | grep GNU
    gettext --version | grep GNU
    gettextize --version | grep GNU
    intltoolize --version | grep GNU
    libtool --version | grep GNU
    libtoolize --version | grep GNU
    echo "$autogen_distribution autogen.sh $autogen_version"
}

autogen_print_start_message() {
    if test "$autogen_opt_silent" != "yes" ; then
        if test "$autogen_opt_dist_clean" = "yes" ; then
            lsh_message "cleaning $autogen_distribution non-distribution files..."
        elif test "$autogen_opt_clean" = "yes" ; then
            lsh_message "cleaning $autogen_distribution extra files..."
        elif test "$autogen_opt_configure" = "yes" ; then
            if test "$autogen_opt_make" = "yes" ; then
                lsh_message "building $autogen_distribution..."
            else
                lsh_message "configuring $autogen_distribution for compile..."
            fi
        else
            lsh_message "preparing $autogen_distribution for 'configure'..."
        fi
        lsh_message "packages: $autogen_packages"
    fi

    return 0;
}

autogen_check_tools() {
    (aclocal --version) < /dev/null > /dev/null 2>&1 && HAVE_ACLOCAL=1
    (autoconf --version) < /dev/null > /dev/null 2>&1 && HAVE_AUTOCONF=1
    (autoheader --version) < /dev/null > /dev/null 2>&1 && HAVE_AUTOHEADER=1
    (automake --version) < /dev/null > /dev/null 2>&1 && HAVE_AUTOMAKE=1
    (autopoint --version) < /dev/null > /dev/null 2>&1 && HAVE_AUTOPOINT=1
    (gettext --version) < /dev/null > /dev/null 2>&1 && HAVE_GETTEXT=1
    (gettextize --version) < /dev/null > /dev/null 2>&1 && HAVE_GETTEXTIZE=1
    (intltoolize --version) < /dev/null > /dev/null 2>&1 && HAVE_INTLTOOLIZE=1
    (libtool --version) < /dev/null > /dev/null 2>&1 && HAVE_LIBTOOL=1
    (libtoolize --version) < /dev/null > /dev/null 2>&1 && HAVE_LIBTOOLIZE=1

    return 0;
}

autogen_check_src() {
    dir=`pwd`
    uniquefile=

    if test -f "ignore.ag" ; then
        lsh_message "$dir/ignore.ag exists, skipping $dir"

        return 1
    fi

    if test ! -f "configure.ac" ; then
        lsh_message "$dir/configure.ac doesn't exist"
        lsh_message "please run autogen.sh from the top directory of your source tree"

        return 1
    fi

    uniquefile=`$lsh_sed -n 's,^AC_INIT(\(.*\)),\1,p' < configure.ac`
    if test -n "$uniquefile" ; then
        if test ! -f "$uniquefile" ; then
            lsh_message "$dir/$uniquefile doesn't exist"
            lsh_message "please check the call to AC_INIT in $autogen_srcdir/configure.ac"

            return 1
        fi
    else
        lsh_message "$dir/configure.ac doesn't call AC_INIT"
        lsh_message "please fix $dir/configure.ac"

        return 1
    fi

    return 0;
}

autogen_clean() {
    configure_ac_files=
    dir=
    currentdir=
    aclocal_m4_files=

    if test "$autogen_opt_clean" = "yes" || test "$autogen_opt_dist_clean" = "yes" ; then
        configure_ac_files=`find $autogen_topdir -name configure.ac`
        if test -n "$configure_ac_files" ; then
            for file in $configure_ac_files
            do
                dir=`dirname $file`
                currentdir=`pwd`                

                if test "$dir" = "$currentdir" ; then
                    :
                else
                    if test "$autogen_opt_silent" != "yes" ; then
                        lsh_message "entering $dir"
                        lsh_message "'cd $dir'"
                    fi
                    cd $dir
                fi
                if test "$autogen_opt_dry_run" != "yes" ; then
                    if test "$autogen_opt_dist_clean" != "yes" ; then
                        autogen_clean_dirs
                    fi
                    autogen_clean_files
                    if test -f "./clean.sh" ; then
                        ./clean.sh
                    fi
                fi
            done
            if test "$autogen_opt_dist_clean" != "yes" ; then
                aclocal_m4_files=`find $autogen_topdir -name aclocal.m4`
                if test -n "$aclocal_m4_files" ; then
                    if test "$autogen_opt_silent" != "yes" ; then
                        lsh_message "cleaning 'aclocal.m4' files"
                    fi
                    for file in $aclocal_m4_files
                    do
                        dir=`dirname $file`
                        currentdir=`pwd`

                        if test "$dir" = "$currentdir" ; then
                            :
                        else
                            if test "$autogen_opt_silent" != "yes" ; then
                                lsh_message "entering $dir"
                                lsh_message "'cd $dir'"
                            fi
                            cd $dir
                        fi
                        if test "$autogen_opt_dry_run" != "yes" ; then
                            rm -f aclocal.m4
                        fi
                    done
                fi
            fi
            cd $autogen_topdir
        fi

        return 1
    fi

    return 0;
}

autogen_clean_dirs() {
    configdirs="`$lsh_sed -n 's,^AC_CONFIG_AUX_DIR(\(.*\)),\1,gp' < configure.ac`"
    if test -f "aclocal.m4" ; then
        configdirs="$configdirs `$lsh_sed -n 's,^AC_CONFIG_AUX_DIR(\(.*\)),\1,gp' < aclocal.m4`"
    fi
    if test -n "$configdirs" ; then
        for configdir in $configdirs
        do
            if test "$autogen_opt_dry_run" != "yes" ; then
                rm -rf $configdir
            fi
        done
    fi

    return 0;
}

autogen_clean_files() {
    if test "$autogen_opt_dry_run" != "yes" ; then
        if test -f Makefile ; then
            make clean
        fi
        if test -x "clean.sh" ; then
            ./clean.sh
        fi

        if test "$autogen_opt_dist_clean" != "yes" ; then
            rm -f `find . -name Makefile.in`
            rm -f `find . -name POTFILES.in`
#            rm -f `find . -name aclocal.m4`
            rm -f `find . -name autoscan.log`
            rm -f `find . -name configure`
            rm -f `find . -name configure.scan`
            rm -f `find . -name core`
            rm -f `find . -name version.h`
            rm -f `find . -name version.m4`
        fi

        rm -f `find . -name Makefile`
        rm -f `find . -name autoconf.h\*`
        rm -f `find . -name config.cache`
        rm -f `find . -name config.h\*`
        rm -f `find . -name config.log`
        rm -f `find . -name config.status`
        rm -f `find . -name configure.sh`
        rm -f `find . -name .dirstamp`
        rm -f `find . -name libtool`
        rm -f `find . -name stamp-h\*`
        rm -f `find . -name \*~`
        rm -f `find . -name \*.o`
        rm -f `find . -name \*.lo`
        rm -f `find . -name \*.la`
        rm -f `find . -name \*.a`

        rm -rf `find . -name .deps`
        rm -rf `find . -name .libs`
        rm -rf `find . -name autom4te.cache`
    fi

    return 0;
}

#
# <FIXME>
# - this may destroy version.m4 files unnecessarily...
# </FIXME>
#
autogen_create_version_files() {
    loop=0
    line=
    header=
    major=
    minor=
    patchlevel=
    package_uppercase=
    major_macro=
    minor_macro=
    patchlevel_macro=
    set_version_macro=
    version_macro=
    version_info_macro=
    header_basename=
    header_macro=

    if test ! -f "versions.ag" ; then
        return 0
    fi

    cat > m4/version.m4 <<EOF
#
# NOTE: this file was generated by $lsh_scriptname. DO NOT EDIT!
#
# version.m4 - version information for $autogen_distribution.
#
# See the file COPYING for information about using this software.

EOF
    for package in $autogen_packages
    do
        line=`grep "^$package" versions.ag`
        if test -n "$line" ; then
            header=`echo $line | $lsh_sed -n 's,^\(['$lsh_letters']*\):\(['$lsh_letters'/_.]*\):\(['$lsh_digits']*\)\.\(['$lsh_digits']*\)\.\(['$lsh_digits']*\),\2,p'`
            major=`echo $line | $lsh_sed -n 's,^\(['$lsh_letters']*\):\(['$lsh_letters'/_.]*\):\(['$lsh_digits']*\)\.\(['$lsh_digits']*\)\.\(['$lsh_digits']*\),\3,p'`
            minor=`echo $line | $lsh_sed -n 's,^\(['$lsh_letters']*\):\(['$lsh_letters'/_.]*\):\(['$lsh_digits']*\)\.\(['$lsh_digits']*\)\.\(['$lsh_digits']*\),\3,p'`
            patchlevel=`echo $line | $lsh_sed -n 's,^\(['$lsh_letters']*\):\(['$lsh_letters'/_.]*\):\(['$lsh_digits']*\)\.\(['$lsh_digits']*\)\.\(['$lsh_digits']*\),\3,p'`
            package_uppercase=`echo $package | $lsh_sed 'y,['$lsh_lowercase_letters'],['$lsh_uppercase_letters'],'`
            major_macro=__$package_uppercase'_MAJOR__'
            minor_macro=__$package_uppercase'_MINOR__'
            patchlevel_macro=__$package_uppercase'_PATCHLEVEL__'
            set_version_macro=$package_uppercase'_SET_VERSION'
            version_macro=$package_uppercase'_VERSION'
            version_info_macro=$package_uppercase'_VERSION_INFO'

            if test -n "$header" ; then
                lsh_message "creating $header"
                header_basename=`$lsh_basename $header`
                header_macro=`echo $package_uppercase'_'$header_basename | $lsh_sed 'y,['$lsh_lowercase_letters'/.],['$lsh_uppercase_letters'__],'`

                cat > $header <<EOF
/*
 * NOTE: this file was generated by $lsh_scriptname. DO NOT EDIT!
 *
 * $header_basename - version information for $package.
 *
 * See the file COPYING for information about using this software.
 */
 
#ifndef $header_macro
#define $header_macro
 
#define $major_macro      $major
#define $minor_macro      $minor
#define $patchlevel_macro $patchlevel
 
#endif /* $header_macro */

EOF
            fi

            if test "$loop" -eq 0 ; then
                lsh_message "creating $autogen_topdir/m4/version.m4"
                loop=1
            else
                lsh_message "appending to $autogen_topdir/m4/version.m4"
            fi

            cat >> m4/version.m4 <<EOF
# Set $package version information.
AC_DEFUN([$set_version_macro],
[
    $version_macro="$major.$minor.$patchlevel"
    AC_SUBST($version_macro)
    $version_info_macro="$major:$minor:$patchlevel"
    AC_SUBST($version_info_macro)
])

EOF
        fi
    done

    return 0;
}

autogen_exec_tools() {
    autogen_exec_aclocal \
    && autogen_create_config_dirs \
    && autogen_check_gnu_gettext \
    && autogen_exec_gettextize \
    && autogen_exec_intltoolize \
    && autogen_exec_autopoint \
    && autogen_exec_libtoolize \
    && autogen_exec_autoheader \
    && autogen_exec_automake \
    && autogen_exec_autoconf \
    && autogen_exec_configure \
    && autogen_exec_make

    return 0;
}

autogen_exec_aclocal() {
    aclocal_cmd=
    aclocal_includes=
    includedirs=`$lsh_sed -n 's,^AM_ACLOCAL_INCLUDE(\(.*\)),\1,gp' < configure.ac`

    if test -z "$aclocal_opts" ; then
        aclocal_cmd="aclocal"
    else
        aclocal_cmd="aclocal $aclocal_opts"
    fi

    if test -n "$aclocal_default_includes" ; then
        aclocal_includes="$aclocal_default_includes"
    fi

    if test -n "$includedirs" ; then
        for includedir in $includedirs
        do
            aclocal_includes="$aclocal_includes -I $includedir"
        done
    fi

    if test -n "$aclocal_includes" ; then
        aclocal_cmd="$aclocal_cmd $aclocal_includes"
    fi

    if test "$autogen_opt_silent" != "yes" ; then
        lsh_message "'$aclocal_cmd'"
    fi
    if test "$autogen_opt_dry_run" != "yes" ; then
        if test "$HAVE_ACLOCAL" -eq 1 ; then
            $aclocal_cmd
        else
            autogen_missing_aclocal

            return 1
        fi
    fi

    includedirs=`$lsh_sed -n 's,^AM_ACLOCAL_INCLUDE(\(.*\)),\1,gp' < aclocal.m4`
    if test -n "$includedirs" ; then
        for includedir in $includedirs
        do
            aclocal_includes="$aclocal_includes -I $includedir"
        done
        aclocal_cmd="$aclocal_cmd $aclocal_includes"

        if test "$autogen_opt_silent" != "yes" ; then
            lsh_message "'$aclocal_cmd'"
        fi
        if test "$autogen_opt_dry_run" != "yes" ; then
            $aclocal_cmd
        fi
    fi

    return 0;
}

autogen_create_config_dirs() {
    configdirs=`$lsh_sed -n 's,^AC_CONFIG_AUX_DIR(\(.*\)),\1,gp' < configure.ac`
    configdirs="$configdirs `$lsh_sed -n 's,^AC_CONFIG_AUX_DIR(\(.*\)),\1,gp' < aclocal.m4`"
    if test -n "$configdirs" ; then
        for configdir in $configdirs
        do
            if test "$silent" != "yes" ; then
                lsh_message "'mkdir $configdir'"
            fi

            if test "$autogen_opt_dry_run" != "yes" ; then
                mkdir $configdir
            fi
        done
    fi

    return 0;
}

autogen_check_gnu_gettext() {
    if grep "^AM_GNU_GETTEXT" configure.ac > /dev/null \
       || (test -r aclocal.m4 \
           && grep "^AM_GNU_GETTEXT" aclocal.m4 > /dev/null) \
       || grep "^AM_GNOME_GETTEXT" configure.ac > /dev/null \
       || (test -r aclocal.m4 \
           && grep "^AM_GNOME_GETTEXT" aclocal.m4 > /dev/null) ; then
        USE_GNU_GETTEXT=1
    fi

    return 0;
}

autogen_exec_gettextize() {
    gettextize_cmd=

    if test "$USE_GNU_GETTEXT" -ne 1 ; then

        return 0
    fi

    if test -z "$gettextize_opts" ; then
        gettextize_cmd="gettextize"
    else
        gettextize_cmd="gettextize $gettextize_opts"
    fi

    if grep "sed.*POTFILES" configure.ac > /dev/null ; then
        : # do nothing - we still have an unmodified configure.ac
    elif test "$autogen_opt_gettextize" = "yes" ; then
        autogen_opt_autopoint="no"

        if test "$autogen_opt_silent" != "yes" ; then
            lsh_message "'$gettextize_cmd > gettextize.log'"
        fi
        if test "$autogen_opt_dry_run" != "yes" ; then
            if test "$HAVE_GETTEXTIZE" -eq 1 ; then
                $gettextize_cmd > gettextize.log
            else
                autogen_missing_gettextize

                return 1
            fi
        fi
    else
        lsh_message "NOTE: you seem to be using GNU gettext."
        lsh_message "NOTE: you might want to run 'gettextize'."
        lsh_message "NOTE: try '$0 --help' for more information."
    fi

    return 0;
}

autogen_exec_intltoolize() {
    intltoolize_cmd=

    if test "$USE_GNU_GETTEXT" -ne 1 ; then

        return 0
    fi

    if test -z "$intltoolize_opts" ; then
        intltoolize_cmd="intltoolize"
    else
        intltoolize_cmd="intltoolize $intltoolize_opts"
    fi

    if grep "^AC_PROG_INTLTOOL" configure.ac > /dev/null \
       || (test -r aclocal.m4 \
           && grep "^AC_PROG_INTLTOOL" aclocal.m4 > /dev/null) ; then
        if test "$autogen_opt_silent" != "yes" ; then
            lsh_message "'$intltoolize_cmd > intltoolize.log'"
        fi
        if test "$autogen_opt_dry_run" != "yes" ; then
            if test "$HAVE_INTLTOOLIZE" -eq 1 ; then
                $intltoolize_cmd > intltoolize.log
            else
                autogen_missing_intltoolize

                return 1
            fi
        fi
    fi

    return 0;
}

autogen_exec_autopoint() {
    autopoint_cmd=

    if test "$USE_GNU_GETTEXT" -ne 1 ; then

        return 0
    fi

    if test -z "$autopoint_opts" ; then
        autopoint_cmd="autopoint"
    else
        autopoint_cmd="autopoint $autopoint_opts"
    fi

    if grep "sed.*POTFILES" configure.ac > /dev/null ; then
        : # do nothing - we still have an unmodified configure.ac
    elif test "$autogen_opt_autopoint" = "yes" ; then
        if test "$autogen_opt_silent" != "yes" ; then
            lsh_message "'$autopoint_cmd > autopoint.log'"
        fi
        if test "$autogen_opt_dry_run" != "yes" ; then
            if test "$HAVE_AUTOPOINT" -eq 1 ; then
                $autopoint_cmd > autopoint.log
            else
                autogen_missing_autopoint

                return 1
            fi
        fi
    else
        lsh_message "NOTE: you seem to be using GNU gettext."
        lsh_message "NOTE: you might want to run 'autopoint'."
        lsh_message "NOTE: try '$0 --help' for more information."
    fi

    return 0;
}

autogen_exec_libtoolize() {
    libtoolize_cmd=

    if test -z "$libtoolize_opts" ; then
        libtoolize_cmd="libtoolize"
    else
        libtoolize_cmd="libtoolize $libtoolize_opts"
    fi

    if grep "^AC_PROG_LIBTOOL" configure.ac > /dev/null \
       || (test -r aclocal.m4 \
           && grep "^AC_PROG_LIBTOOL" aclocal.m4 > /dev/null) ; then
        if test "$autogen_opt_silent" != "yes" ; then
            lsh_message "'$libtoolize_cmd'"
        fi
        if test "$autogen_opt_dry_run" != "yes" ; then
            if test "$HAVE_LIBTOOLIZE" -eq 1 ; then
                $libtoolize_cmd
            else
                autogen_missing_libtoolize

                return 1
            fi
        fi
    fi

    return 0;
}

autogen_exec_autoheader() {
    autoheader_cmd=

    if test -z "$autoheader_opts" ; then
        autoheader_cmd="autoheader"
    else
        autoheader_cmd="autoheader $autoheader_opts"
    fi

    if grep "^AC_CONFIG_HEADERS" configure.ac > /dev/null \
       || (test -r aclocal.m4 \
           && grep "^AC_CONFIG_HEADERS" aclocal.m4 > /dev/null) ; then
        if test "$autogen_opt_silent" != "yes" ; then
            lsh_message "'$autoheader_cmd'"
        fi
        if test "$autogen_opt_dry_run" != "yes" ; then
            if test "$HAVE_AUTOHEADER" -eq 1 ; then
                $autoheader_cmd
            else
                autogen_missing_autoheader

                return 1
            fi
        fi
    fi

    return 0;
}

autogen_exec_automake() {
    automake_cmd=

    if test -z "$automake_opts" ; then
        automake_cmd="automake"
    else
        automake_cmd="automake $automake_opts"
    fi

    if test "$autogen_opt_silent" != "yes" ; then
        lsh_message "'$automake_cmd'"
    fi
    if test "$autogen_opt_dry_run" != "yes" ; then
        if test "$HAVE_AUTOMAKE" -eq 1 ; then
            $automake_cmd
        else
            autogen_missing_automake

            return 1
        fi
    fi

    return 0;
}

autogen_exec_autoconf() {
    autoconf_cmd=

    if test -z "$autoconf_opts" ; then
        autoconf_cmd="autoconf"
    else
        autoconf_cmd="autoconf $autoconf_opts"
    fi

    if test "$autogen_opt_silent" != "yes" ; then
        lsh_message "'$autoconf_cmd'"
    fi
    if test "$autogen_opt_dry_run" != "yes" ; then
        if test "$HAVE_AUTOCONF" -eq 1 ; then
            $autoconf_cmd
        else
            autogen_missing_autoconf

            return 1
        fi
    fi

    return 0;
}

autogen_exec_configure() {
    configure_cmd=

    if test -z "$configure_opts" ; then
        configure_cmd="./configure"
    else
        configure_cmd="./configure $configure_opts"
    fi

    autogen_create_configure_lsh

    if test "$autogen_opt_configure" = "yes" ; then
        if test "$autogen_opt_silent" != "yes" ; then
            lsh_message "'$configure_cmd'"
        fi
        if test "$autogen_opt_dry_run" != "yes" ; then
            $configure_cmd || return 1
        fi
    else
        if test "$autogen_opt_silent" != "yes" ; then
            if test "$autogen_opt_dry_run" != "yes" ; then
                lsh_message "NOTE: run './configure [option] ...' to configure $autogen_distribution."
                lsh_message "NOTE: run './configure --help' for more information."
            fi
        fi

        return 1
    fi

    return 0;
}

autogen_exec_make() {
    if test "$autogen_opt_make" = "yes" ; then
        if test "$autogen_opt_silent" != "yes" ; then
            lsh_message "'make'"
        fi
        if test "$autogen_opt_dry_run" != "yes" ; then
            make || return 1
        fi
    fi

    return 0;
}

autogen_create_configure_lsh() {
    if test "$autogen_opt_silent" != "yes" ; then
        lsh_message "creating configure.sh"
    fi
    if test "$autogen_opt_dry_run" != "yes" ; then
        cat > configure.sh <<EOF
#! /bin/lsh

$configure_cmd
EOF

        chmod u+x configure.sh
    fi

    return 0;
}

autogen_create_potfiles_in() {
    sortednames=
    excluded=
    potfiles=

    if test "$USE_GNU_GETTEXT" -ne 1 ; then

        return 0
    fi

    for ext in $autogen_potfile_exts
    do
        if test -z "$sortednames" ; then
            sortednames=`find . -name \*.$ext | sort`
        else
            sortednames="$sortednames `find . -name \*.$ext | sort`"
        fi
    done

    for name in $sortednames
    do
        excluded="false"
        if test -d "$name" ; then
            :
        else
            for excludedir in $autogen_potfile_excludedirs
            do
                if echo "$name" | grep "^\./$excludedir[/]" > /dev/null 2>&1 ; then
                    excluded="true"

                    break
                else
                    :
                fi
            done
            if test "$excluded" = "true" ; then
                :
            elif test -z "$potfiles" ; then
                potfiles="$name"
            else
                potfiles="$potfiles $name"
            fi
        fi
    done

    if test -n "$potfiles" ; then
        touch po/POTFILES.in
        for potfile in $potfiles
        do
            potfile=`echo "$potfile" | sed 's,^\(\./\),,'`
            echo $potfile >> po/POTFILES.in
        done
    fi
}

autogen_missing_autoconf() {
    echo
    lsh_message "You must have 'autoconf' installed to compile $autogen_distribution."
    lsh_message "Download the appropriate package for your distribution,"
    lsh_message "or get the latest source from ftp://ftp.gnu.org/pub/gnu/"

    return 1;
}

autogen_missing_automake() {
    echo
    lsh_message "You must have 'automake' installed to compile $autogen_distribution."
    lsh_message "Download the appropriate package for your distribution,"
    lsh_message "or get the latest source from ftp://ftp.gnu.org/pub/gnu/"

    return 1;
}

autogen_missing_aclocal() {
    if test "$HAVE_AUTOMAKE" -eq 1 ; then
        echo
        lsh_message "Missing 'aclocal'. The version of 'automake' installed"
        lsh_message "doesn't appear recent enough."
        lsh_message "Download the appropriate package for your distribution,"
        lsh_message "or get the latest source from ftp://ftp.gnu.org/pub/gnu/"
    else
        autogen_missing_automake
    fi

    return 1;
}

autogen_missing_autoheader() {
    if test "$HAVE_AUTOMAKE" -eq 1 ; then
        echo
        lsh_message "Missing 'autoheader'. The version of 'automake' installed"
        lsh_message "doesn't appear recent enough."
        lsh_message "Download the appropriate package for your distribution,"
        lsh_message "or get the latest source from ftp://ftp.gnu.org/pub/gnu/"
    else
        autogen_missing_automake
    fi

    return 1;
}

autogen_missing_gettext() {
    echo
    lsh_message "You must have 'gettext' installed to compile $autogen_distribution."
    lsh_message "Download the appropriate package for your distribution,"
    lsh_message "or get the latest source from ftp://alpha.gnu.org/gnu/"

    return 1;
}

autogen_missing_autopoint() {
    if test "$HAVE_GETTEXT" -eq 1 ; then
        echo
        lsh_message "Missing 'autopoint'. The version of 'gettext' installed"
        lsh_message "doesn't appear recent enough."
        lsh_message "Download the appropriate package for your distribution,"
        lsh_message "or get the latest source from ftp://ftp.gnu.org/pub/gnu/"
    else
        autogen_missing_gettext
    fi

    return 1;
}

autogen_missing_gettextize() {
    if test "$HAVE_GETTEXT" -eq 1 ; then
        echo
        lsh_message "Missing 'gettextize'. The version of 'gettext' installed"
        lsh_message "doesn't appear recent enough."
        lsh_message "Download the appropriate package for your distribution,"
        lsh_message "or get the latest source from ftp://ftp.gnu.org/pub/gnu/"
    else
        autogen_missing_gettext
    fi

    return 1;
}

autogen_missing_intltoolize() {
    echo
    lsh_message "You must have 'intltoolize' installed to compile $autogen_distribution."
    lsh_message "Download the appropriate package for your distribution,"
    lsh_message "or get the latest source from ftp://ftp.gnome.org/pub/GNOME/"

    return 1;
}

autogen_missing_libtool() {
    echo
    lsh_message "You must have 'libtool' installed to compile $autogen_distribution."
    lsh_message "Download the appropriate package for your distribution,"
    lsh_message "or get the latest source from ftp://ftp.gnu.org/pub/gnu/"

    return 1;
}

autogen_missing_libtoolize() {
    if test "$HAVE_LIBTOOL" -eq 1 ; then
        echo
        lsh_message "Missing 'libtoolize'. The version of 'libtool' installed"
        lsh_message "doesn't appear recent enough."
        lsh_message "Download the appropriate package for your distribution,"
        lsh_message "or get the latest source from ftp://ftp.gnu.org/pub/gnu/"
    else
        autogen_missing_libtool
    fi

    return 1;
}

