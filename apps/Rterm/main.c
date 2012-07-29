/*
 * main.c - main() routine for Rterm.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

int
main(int argc, char *argv[])
{
    exit(Rterm_run(argc, argv, RTERM_DEFAULT_FLAGS));
}

