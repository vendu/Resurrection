/*
 * main.c - screen saver module for Resurrection.
 * Copyright (C) 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

extern uint8_t *__text;
extern long __textsize;


int
main(int argc, char *argv[])
{
#if 0
    hexdump_main(argc, argv);
#endif
    alien_main(argc, argv);

    /* NOTREACHED */
    return 0;
}

