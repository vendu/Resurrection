/*
 * main.c - Rv main routine.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

int
main(int argc, char *argv[])
{
    struct R_app app;

    memset(&app,
           0,
           sizeof(app));
    if (!Rl_init(&app,
                 argc,
                 argv)) {
        fflush(stderr);

        exit(1);
    }
    while (TRUE) {
        R_handle_next_event(&app);
    }

    exit(0);
}

