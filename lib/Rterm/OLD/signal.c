/*
 * signal.c - signal handling for Resurrection
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#define USE_POSIX1

#if (USE_SIGACTION)

/*
 * This code is based on the code from Stevens, Advanced Programming in the
 * UNIX Environment.
 */
typedef RETSIGTYPE R_signalhandler_t(int);

R_signalhandler_t *
signal_sigaction(int sig, R_signalhandler_t *handler)
{
    struct sigaction action, oldaction;

    if (sig <= 0
#if defined(SIGNALS)
	|| sig >= SIGNALS
#endif
	|| handler == NULL) {

	return SIG_ERR;
    }

    action.sa_handler = handler;

    sigemptyset(&action.sa_mask);

    action.sa_flags = 0;

    /*
     * NOTE: R_system.h defines sa_mask values to 0 if not available.
     */
    
    if (sig == SIGALRM) {
#   if defined(SA_INTERRUPT)
	action.sa_flags |= SA_INTERRUPT;
#   endif /* SA_INTERRUPT */
    } else {
#   if defined(SA_RESTART)
	action.sa_flags |= SA_RESTART;
#   endif /* SA_RESTART */
    }

    if (sigaction(sig, &action, &oldaction) < 0) {

	return SIG_ERR;
    }

    return (oldaction.sa_handler);
}

#endif /* USE_SIGACTION */


int
term_send_signal(struct R_term *term, int pid, int sig)
{
#if defined(AMOEBA)

    if (pid != 2) {
	fprintf(stderr, "unexpected process id %d\n", pid);

	abort();
    }

    amoeba_send_pty_signal(sig);

    return 0;

#elif (USE_POSIX_1) || (USE_SVR3) || (USE_SVR4)

    return (kill(-pid, sig));

#else

    return (killpg(pid, sig));
    
#endif
}

