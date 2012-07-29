# !/bin/sh
#
# tardist.sh - utilities to create portable distribution tar archives.
#
# <NOTE>
# - requires GNU tar.
# </NOTE>
#
# Copyright (C) 2003 Tuomo Venäläinen
#
# See the file COPYING for information about using this software.
#
# <FIXME>
# - fix this to default to $HOME rather than $TMPDIR when archiving upper
#   directories.
# </FIXME>
#
# <TODO>
# - clean up the function interfaces. Add check for GNU tar.
# </TODO>
#
# set -x

tardist_version="0.0.0"
tardist_autogen_opts="--dist-clean"
lsh_pkgdatadir="@lshdir@"
if test -z "$lsh_pkgdatadir" || test "$lsh_pkgdatadir" = "@lshdir@" ; then
    lsh_pkgdatadir="/data2/src/Lightning/lsh"
fi
. $lsh_pkgdatadir/lsh.sh

tardist_main() {
    lsh_init
    lsh_load_autogen

    tardist_init_traps
    tardist_init
    tardist_get_opts $@

#    cd "$tardist_topdir"
    for tardist_argdir in $tardist_argdirs
    do
        tardist_check_src \
        && tardist_check_version \
        && tardist_create_filelists \
        && tardist_make_backup \
        && tardist_make_archive \

        tardist_cleanup

        tardist_exec "cd $tardist_topdir"
    done

    return 0;
}

tardist_init_traps() {
    lsh_add_exit_handler "tardist_cleanup"

    if test "$lsh_processid" = "$$" ; then
        :
    else
        lsh_init_traps
    fi

    return 0;
}

tardist_init() {
    tardist_topdir=`pwd` || lsh_abort $lsh_errcmd "'pwd' failed"

    tardist_backup="false"
    tardist_pkgdir="false"
    tardist_opt_dry_run="no"
    tardist_opt_verbose="no"

    return 0;
}

tardist_get_opts() {
    if test "$#" -lt 1 ; then
        lsh_abort $lsh_errinval "no arguments"
    fi

    for arg
    do
        case $arg in
            -D | --dist-clean)
                tardist_autogen_opts="$tardist_autogen_opts --dist-clean"
                ;;
            -n | --dry-run)
                tardist_opt_dry_run="yes"
                tardist_autogen_opts="$tardist_autogen_opts --dry-run"
                ;;
            -h | --help)
                cat <<EOF
    Usage: $0 directory <destination directory>

    Create tar archive of directory with files in (mostly) alphabetical order.

    -C, --clean      clean extra files.
    -D, --dist-clean clean non-distribution files.
    -n, --dry-run    print commands without running them.
    -h, --help       print this help message.
    -V, --verbose    be verbose.
EOF

                return 1
                ;;
            -V | --verbose)
                tardist_opt_verbose="yes"
                ;;
            *)
                if test -z "$tardist_argdirs" ; then
                    tardist_argdirs="$arg"
                else
                    tardist_argdirs="$tardist_argdirs $arg"
                fi
                ;;
        esac
    done

    return 0;
}

tardist_check_src() {
    tardist_srcdir=
    tardist_destdir=
    tardist_packagedir=

    destdir=

    tardist_exec "cd $tardist_argdir"
    tardist_srcdir=`pwd` || lsh_abort $lsh_errcmd "'pwd' failed"
    tardist_srcdir=`echo "$tardist_srcdir" | sed 's,\(.\)\([/]*\)$,\1,'` \
    || lsh_abort $lsh_errcmd "'sed' failed"
    if echo "$tardist_srcdir" | grep "^/" > /dev/null ; then
        if echo "$tardist_srcdir" | grep "^[/][/]*$" ; then
            lsh_error "invalid argument '$tardist_argdir': cannot archive root directory"

            return 1
        fi
    fi

    cd > /dev/null 2>&1 \
    || {
        lsh_error "'cd' failed"

        return 1
    }
    destdir=`pwd`
    if echo "$destdir" | grep "^$tardist_srcdir[/]*$" > /dev/null ; then
        cd "$TMPDIR"
        destdir=`pwd`
        if echo "$destdir" | grep "^$tardist_srcdir[/]*$" > /dev/null ; then
            lsh_error "invalid argument '$tardist_argdir': cannot archive $tardist_srcdir"

            return 1
        fi
    fi
    if test "$tardist_destdir" = "$tardist_topdir" ; then
        tardist_destdir="."
    else
        tardist_destdir="$destdir"
    fi
    cd "$tardist_topdir"

    tardist_packagedir=`basename $tardist_srcdir`

    return 0;
}

tardist_check_version() {
    tardist_versionfile="$tardist_srcdir/versions.ag"
    tardist_package=
    tardist_packageversion=

    line=
    major=
    minor=
    patchlevel=

    if test -f "$tardist_srcdir/versions.ag" ; then
        line=`grep "^$tardist_packagedir" $tardist_srcdir/versions.ag`
        if test -n "$line" ; then
            major=`echo $line | $lsh_sed -n 's,^\(['$lsh_letters']*\):\(['$lsh_letters'/_.]*\):\(['$lsh_digits']*\)\.\(['$lsh_digits']*\)\.\(['$lsh_digits']*\),\3,p'`
            minor=`echo $line | $lsh_sed -n 's,^\(['$lsh_letters']*\):\(['$lsh_letters'/_.]*\):\(['$lsh_digits']*\)\.\(['$lsh_digits']*\)\.\(['$lsh_digits']*\),\3,p'`
            patchlevel=`echo $line | $lsh_sed -n 's,^\(['$lsh_letters']*\):\(['$lsh_letters'/_.]*\):\(['$lsh_digits']*\)\.\(['$lsh_digits']*\)\.\(['$lsh_digits']*\),\3,p'`
        fi
        if test -n "$major" && test -n "$minor" && test -n "$patchlevel" ; then
            tardist_packageversion="$major.$minor.$patchlevel"
        fi
    fi

    if test -n "$tardist_packageversion" ; then
        tardist_package="$tardist_packagedir-$tardist_packageversion"
    else
        tardist_package="$tardist_packagedir"
    fi

    if test -e "$tardist_destdir/$tardist_package.tar.gz" ; then
        lsh_error "'$tardist_destdir/$tardist_package.tar.gz' already exists"

        return 1
    fi

    return 0;
}

tardist_create_filelists() {
    tardist_srctopfiles=
    tardist_srcsubdirs=
    tardist_srcsubdirfiles=

    sortedlist=`find $tardist_srcdir | sort` \
    || lsh_abort $lsh_errcmd "'find $tardist_srcdir | sort' failed"
    itemdir=
    for item in $sortedlist
    do
        if test "$item" = "." || test "$item" = ".." || test "$item" = "$tardist_srcdir" ; then
            :
        elif test -d "$item" ; then
            if test -z "$tardist_srcsubdirs" ; then
                tardist_srcsubdirs="$item"
            else
                tardist_srcsubdirs="$tardist_srcsubdirs $item"
            fi
        else
            itemdir=`dirname $item`
            if test "$itemdir" = "$tardist_srcdir" ; then
                if test -z "$tardist_srctopfiles" ; then
                    tardist_srctopfiles="$item"
                else
                    tardist_srctopfiles="$tardist_srctopfiles $item"
                fi
            elif test -z "$tardist_srcsubdirfiles" ; then
                tardist_srcsubdirfiles="$item"
            else
                tardist_srcsubdirfiles="$tardist_srcsubdirfiles $item"
            fi
        fi
    done

    return 0;
}

tardist_make_backup() {
    tardist_backup="false"
    package="$tardist_destdir/$tardist_package"

    if test -e "$package" ; then
        tardist_exec "mv $package $package-$$" \
        && tardist_backup="true" \
        || return 1
    fi

    return 0;
}

tardist_make_archive() {
    tardist_pkgdir="false"

    package="$tardist_destdir/$tardist_package"
    destfile=

    tardist_exec "mkdir $package" || return 1
    tardist_pkgdir="true"
    for srcfile in $tardist_srctopfiles
    do
        destfile=`echo "$srcfile" | sed 's,^\('$tardist_srcdir'\),'$package','` \
        || lsh_abort $lsh_errcmd "'sed' failed"
        if test "$tardist_backup" = "true" ; then
            srcfile=`echo "$srcfile" | sed 's,^\('$tardist_srcdir'\),'$package-$$','` \
            || lsh_abort $lsh_errcmd "'sed' failed"
        fi
        tardist_exec "cp -p $srcfile $destfile" || return 1
    done

    for destdir in $tardist_srcsubdirs
    do
        destdir=`echo "$destdir" | sed 's,^\('$tardist_srcdir'\),'$package','` \
        || lsh_abort $lsh_errcmd "'sed' failed"
        if test "$tardist_opt_verbose" = "yes" ; then
            echo "'mkdir $destdir'"
        fi
        if test "$tardist_opt_dry_run" != "yes" ; then
            mkdir $destdir \
            || lsh_error "'mkdir $destdir' failed" \
            || return 1
        fi
    done
    for srcfile in $tardist_srcsubdirfiles
    do
        destfile=`echo "$srcfile" | sed 's,^\('$tardist_srcdir'\),'$package','` \
        || lsh_abort $lsh_errcmd "'sed' failed"
        if test "$tardist_backup" = "true" ; then
            srcfile=`echo "$srcfile" | sed 's,^\('$tardist_srcdir'\),'$package-$$','` \
            || lsh_abort $lsh_errcmd "'sed' failed"
        fi
        if test "$tardist_opt_verbose" = "yes" ; then
            echo "'cp -p $srcfile $destfile'"
        fi
        if test "$tardist_opt_dry_run" != "yes" ; then
            cp -p $srcfile $destfile \
            || lsh_error "'cp -p $srcfile $destfile' failed" \
            || return 1
        fi
    done
    tardist_exec "cd $tardist_destdir" \
    && tardist_exec "tar cozf $package.tar.gz $tardist_package" \
    && echo "$0: created $package.tar.gz"

    return 0;
}

tardist_exec() {
    cmd="$1"

    if test "$tardist_opt_verbose" = "yes" ; then
        echo "'$1'"
    fi
    if test "$tardist_opt_dry_run" != "yes" ; then
        $1 \
        || {
            lsh_error "'$1' failed"

            return 1
        }
    fi

    return 0;
}

tardist_cleanup() {
    package="$tardist_destdir/$tardist_package"

    if test "$tardist_backup" = "true" ; then
        tardist_exec "rm -rf $package" \
        && tardist_exec "mv $package-$$ $package"
        tardist_backup="false"
    elif test "$tardist_pkgdir" = "true" ; then
        tardist_exec "rm -rf $package"
        tardist_pkgdir="false"
    fi

    return 0;
}

tardist_main $@

