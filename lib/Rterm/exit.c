/*
 * exit.c - exit handling for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

void
app_exit(struct R_app *app)
{
#if 0
    if (app->display) {
	XSync(app->display, True);
//	XCloseDisplay(app->display);
    }
#endif

    exit(EXIT_SUCCESS);
}

void
Rterm_exit(void)
{
    struct R_app *app;
    struct R_term *term;
    struct R_pty *pty;
    int i;

    app = R_global.app;
    term = app->client;

    user_enable_privileges(&(term->user));

    for (i = 0 ; i < term->nscreens ; i++) {
        pty = &term->screens[i]->pty;
        PTY_RESET_SLAVE(pty->slavename, &(pty->stat));

#if 0
        userlog_remove_log_entries(&(pty->userlog));
#endif
    }

    user_disable_privileges(&(term->user));

#if 0
    app_exit(app);
#endif

    return;
}

#if 0
#if (USE_ATEXIT)
void
wm_exit(void)
{
    wm_cleanup(Rglobals.wm);

    return;
}
#elif (USE_ON_EXIT || defined(sun))
void
wm_exit(int exitstatus, void *arg)
{
    wm_cleanup(Rglobals.wm);

    return;
}
#endif
#endif /* 0 */

