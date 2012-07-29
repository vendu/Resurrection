AC_DEFUN([R_CHECK_MMAP_CORE],
[
    R_CHECK_MMAP_MAP_ANON
    R_CHECK_MMAP_DEV_ZERO
])

AC_DEFUN([R_CHECK_MMAP_MAP_ANON],
[
    AC_MSG_CHECKING([if mmap works with MAP_ANON])
    AC_CACHE_VAL(R_cv_mmap_map_anon,
    [
        AC_RUN_IFELSE(
        [
            AC_LANG_SOURCE(
            [
                #include <stdlib.h>
                #include <sys/mman.h>
                #if !defined(MAP_FAILED)
                #   define MAP_FAILED ((void *)-1)
                #endif
                #if !defined(MAP_FILE)
                #   define MAP_FILE 0
                #endif

                #define TEST_MMAP_SIZE 1048576

                int
                main(int argc, char *argv[])
                {
                    void *ptr;

                    ptr = MAP_FAILED;
                    ptr = mmap(NULL,
                               TEST_MMAP_SIZE,
                               PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_FILE | MAP_ANON,
                               -1,
                               0);
                    if (ptr != MAP_FAILED) {

                        exit(0);
                    };

                    exit(1);
                }
            ])
        ],
        R_cv_mmap_map_anon="yes",
        R_cv_mmap_map_anon="no",
        R_cv_mmap_map_anon="no")
    ])
    AC_MSG_RESULT([$R_cv_mmap_map_anon])
])

AC_DEFUN([R_CHECK_MMAP_DEV_ZERO],
[
    AC_MSG_CHECKING([if mmap works on /dev/zero])
    AC_CACHE_VAL(R_cv_mmap_dev_zero,
    [
        AC_RUN_IFELSE(
        [
            AC_LANG_SOURCE(
            [
                #include <stdlib.h>
                #include <sys/types.h>
                #include <sys/stat.h>
                #include <fcntl.h>
                #include <sys/mman.h>
                #if !defined(MAP_FAILED)
                #   define MAP_FAILED ((void *)-1)
                #endif
                #if !defined(MAP_FILE)
                #   define MAP_FILE 0
                #endif

                #define TEST_MMAP_SIZE 1048576

                int
                main(int argc, char *argv[])
                {
                    void *ptr;
                    int   zerofd;

                    zerofd = -1;
                    ptr = MAP_FAILED;
                    zerofd = open("/dev/zero", O_RDWR);
                    if (zerofd < 0) {

                        exit(1);
                    }
                    ptr = mmap(NULL,
                               TEST_MMAP_SIZE,
                               PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_FILE,
                               zerofd,
                               0);
                    if (ptr != MAP_FAILED) {

                        exit(0);
                    };

                    exit(1);
                }
            ])
        ],
        R_cv_mmap_dev_zero="yes",
        R_cv_mmap_dev_zero="no",
        R_cv_mmap_dev_zero="no")
    ])
    AC_MSG_RESULT([$R_cv_mmap_dev_zero])
])

