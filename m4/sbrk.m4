AC_DEFUN([R_CHECK_SBRK_KLUDGE],
[
    AC_CHECK_TYPE([intptr_t])
    if test "$ac_cv_type_intptr_t" = "yes" ; then
        R_cv_sbrk_param_type="intptr_t"
        AC_DEFINE([R_SBRK_T], [intptr_t])
    else
        AC_CHECK_TYPE([ptrdiff_t])
        R_cv_sbrk_param_type="ptrdiff_t"
        if test "$ac_cv_type_ptrdiff_t" = "yes" ; then
            AC_DEFINE([R_SBRK_T], [ptrdiff_t])
        else
            R_cv_sbrk_param_type="int"
            AC_MSG_WARN([R_SBRK_T set to int])
        fi
    fi
    AC_MSG_CHECKING([for sbrk param type KLUDGE])
    AC_MSG_RESULT([$R_cv_sbrk_param_type])
])

AC_DEFUN([R_CHECK_SBRK_PARAM_TYPE],
[
    AC_MSG_CHECKING([for sbrk parameter type])
    R_CHECK_SBRK_PARAM_INT
    R_CHECK_SBRK_PARAM_SSIZE_T
    R_CHECK_SBRK_PARAM_PTRDIFF_T
    R_CHECK_SBRK_PARAM_INTPTR_T
    R_CHECK_SBRK_PARAM_INT
    AC_MSG_RESULT([$R_cv_sbrk_param_type])
])

# This stuff does not work yet. :( /* vendu */
# I'd need to check for at least the following sbrk interfaces:
#     void *sbrk(intptr_t);
#     void *sbrk(ptrdiff_t);
#     void *sbrk(ssize_t);
#     void *sbrk(int);

AC_DEFUN([R_CHECK_SBRK_PARAM_INT],
[
    AC_CACHE_VAL(R_cv_sbrk_param_int,
    [
        AC_COMPILE_IFELSE(
        [
            AC_LANG_SOURCE(
            [
                #include <unistd.h>

                int
                main(int argc, char *argv[])
                {
                    void *(*func)(int);

                    func = sbrk;

                    exit(0);
                }
            ])
        ],
        R_cv_sbrk_param_int="yes",
        R_cv_sbrk_param_int="no")
    ])
    if test "$R_cv_sbrk_param_int" = "yes" ; then
        R_cv_sbrk_param_type="int"
    fi
])

AC_DEFUN([R_CHECK_SBRK_PARAM_SSIZE_T],
[
    AC_CACHE_VAL(R_cv_sbrk_param_ssize_t,
    [
        AC_COMPILE_IFELSE(
        [
            AC_LANG_SOURCE(
            [
                #include <unistd.h>

                int
                main(int argc, char *argv[])
                {
                    void *(*func)(ssize_t);

                    func = sbrk;

                    exit(0);
                }
            ])
        ],
        R_cv_sbrk_param_ssize_t="yes",
        R_cv_sbrk_param_ssize_t="no")
    ])
    if test "$R_cv_sbrk_param_ssize_t" = "yes" ; then
        R_cv_sbrk_param_type="ssize_t"
    fi
])

AC_DEFUN([R_CHECK_SBRK_PARAM_PTRDIFF_T],
[
    AC_CACHE_VAL(R_cv_sbrk_param_ptrdiff_t,
    [
        AC_COMPILE_IFELSE(
        [
            AC_LANG_SOURCE(
            [
                #include <stddef.h>

                int
                main(int argc, char *argv[])
                {
                    void *(*func)(ptrdiff_t);

                    func = sbrk;

                    exit(0);
                }
            ])
        ],
        R_cv_sbrk_param_ptrdiff_t="yes",
        R_cv_sbrk_param_ptrdiff_t="no")
    ])
    if test "$R_cv_sbrk_param_ptrdiff_t" = "yes" ; then
        R_cv_sbrk_param_type="ptrdiff_t"
    fi
])

AC_DEFUN([R_CHECK_SBRK_PARAM_INTPTR_T],
[
    AC_CACHE_VAL(R_cv_sbrk_param_intptr_t,
    [
        AC_COMPILE_IFELSE(
        [
            AC_LANG_SOURCE(
            [
                #include <stdint.h>

                int
                main(int argc, char *argv[])
                {
                    void *(*func)(intptr_t);

                    func = sbrk;

                    exit(0);
            ])
        ],
        R_cv_sbrk_param_intptr_t="yes",
        R_cv_sbrk_param_intptr_t="no")
    ])
    if test "$R_cv_sbrk_param_intptr_t" = "yes" ; then
        R_cv_sbrk_param_type="intptr_t"
    fi
])

