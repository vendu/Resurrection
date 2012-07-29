/*
 * wait.c - child process control for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

extern volatile int Rtermtabs;

pid_t
wait_nonblocking(void)
{
    int errnosave;
#if (USE_NONBLOCKING_WAITPID) || (WAIT_STATUS_INT)
    int status;
#else
    union wait status;
#endif
    pid_t pid;
#if (SUPPORT_RTERM_TABS)
    struct R_term *term = R_global.app->client;
    struct R_termscreen *screen;
    int i;
#endif

    errnosave = errno;

    pid = -1;

#if (CANNOT_DO_NONBLOCKING_WAIT)

    return -1;

#elif (SUPPORT_RTERM_TABS && USE_NONBLOCKING_WAITPID)

    do {
	errno = 0;
    } while ((pid = waitpid(-1, &status, WNOHANG)) == -1 && errno == EINTR);

#elif (USE_NONBLOCKING_WAITPID)

    do {
	errno = 0;
    } while ((pid = waitpid(-1, &status, WNOHANG)) == -1 && errno == EINTR);

#elif (USE_NONBLOCKING_WAIT3)

    do {
	errno = 0;
    } while ((pid = wait3(&status, WNOHANG, NULL)) == -1 && errno == EINTR);

#endif /* USE_NONBLOCKING_WAITPID */

    errno = errnosave;

    return pid;
}

