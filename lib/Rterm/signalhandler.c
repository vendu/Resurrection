/*
 * signalhandler.c - signal handlers for Resurrection.
 * Copyright (C) 2003 - 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

extern volatile int Rtermtabs;

RETSIGTYPE
Rterm_exit_signal(int sig)
{
    exit(sig);
}

RETSIGTYPE
Rterm_usr1_signal(int sig)
{
    struct R_term *term = R_global.app->client;

    fprintf(stderr, "USR1\n");

    if (term->window) {
        R_map_window_raised(term->window);
        XSync(term->window->app->display, False);
    }
#if 0
    if (term->ppid) {
        kill(term->ppid, SIGUSR1);
    }
#endif

    return;
}

RETSIGTYPE
Rterm_usr2_signal(int sig)
{
    struct R_term *term = R_global.app->client;

    fprintf(stderr, "USR2\n");

    if (term->window) {
        R_unmap_window(term->window);
        XSync(term->window->app->display, False);
    }
#if 0
    if (term->ppid) {
        kill(term->ppid, SIGUSR2);
    }
#endif

    return;
}

RETSIGTYPE
Rterm_quit_signal(int sig)
{
    struct R_term *term;

#if 0
/* let the OS handle recursive signals such as segmentation faults. */

    SIGNAL(sig, SIG_DFL);
#endif

    term = R_global.app->client;

    user_disable_privileges(&(term->user));

    umask(077);

    abort();

//    exit(sig);
}

RETSIGTYPE
Rterm_child_signal(int sig)
{
    pid_t pid;
    struct R_term *term;
#if (SUPPORT_RTERM_TABS)
    int i;
#endif

#if (REINSTALL_SIGNAL_HANDLERS) && !(REINSTALL_SIGCHLD_HANDLER_AFTER_WAIT)
    SIGNAL(SIGCHLD, Rterm_child_signal);
#endif

    term = R_global.app->client;

#if (!SUPPORT_RTERM_TABS)
    pid = wait(NULL);
#endif

#if (REINSTALL_SIGNAL_HANDLERS) && (REINSTALL_SIGCHLD_HANDLER_AFTER_WAIT)
    SIGNAL(SIGCHLD, Rterm_child_signal);
#endif /* REINSTALL_SIGNAL_HANDLERS */

#if (SUPPORT_RTERM_TABS)
    if ((pid = wait_nonblocking()) > 0) {
        if (pid) {
            for (i = 0 ; i < term->nscreens ; i += 2) {
                if (pid == term->screens[i]->cmdpid) {
                    printf("WAITPID(%ld)\n", (long)pid);
                    --Rtermtabs;
                }
            }
            if (!Rtermtabs) {
                exit(EXIT_SUCCESS);
            } else {
                for (i = 0 ; i < term->nscreens - 1 ; i += 2) {
                    if (pid == term->screens[i]->cmdpid) {
                        term->screens[i]->cmdpid = 0;
                        term->screens[i]->pty.masterfd = -1;
                        term->screens[i + 1]->pty.masterfd = -1;
                    }
                }
            }
        }
    }
#else
    do {
        if (pid == term->screen->cmdpid) {
            exit(EXIT_SUCCESS);
        }
    } while ((pid = wait_nonblocking()) >= 0);
#endif

    return;
}

#if 0
RETSIGTYPE
wm_exit_signal(int sig)
{
    exit(sig);
}

RETSIGTYPE
wm_quit_signal(int sig)
{
    SIGNAL(sig, SIG_DFL);

    XCloseDisplay(Rglobals.wm->app->display);

    abort();
}

RETSIGTYPE
wm_child_signal(int sig)
{
#if (REINSTALL_SIGNAL_HANDLERS) && !(REINSTALL_SIGCHLD_HANDLER_AFTER_WAIT)
    SIGNAL(SIGCHLD, Rterm_child_signal);
#endif

    wait(NULL);

#if (REINSTALL_SIGNAL_HANDLERS) && (REINSTALL_SIGCHLD_HANDLER_AFTER_WAIT)
    SIGNAL(SIGCHLD, Rterm_child_signal);
#endif /* REINSTALL_SIGNAL_HANDLERS */

    return;
}

RETSIGTYPE
wm_alrm_signal(int sig)
{
    SIGNAL(sig, SIG_DFL);
}
#endif /* 0 */
