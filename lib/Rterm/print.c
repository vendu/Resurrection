/*
 * print.c - printing routines for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#if defined(RTERM_PRINT_PIPE)

FILE *
popen_printer(const char *cmd)
{
    FILE *printfp;

    if (cmd == NULL) {
	printfp = popen(RTERM_PRINT_PIPE, "w");
    } else {
	printfp = popen(cmd, "w");
    }

    return printfp;
}

void
pclose_printer(FILE *printfp)
{
    if (printfp == NULL) {

	return;
    }

    fflush(printfp);

    pclose(printfp);
}

#endif /* defined(RTERM_PRINT_PIPE) */

