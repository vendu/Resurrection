/*
 * hash.c - hash table routines for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#if 0
unsigned long
hash_compute_key(const R_text_t *str, unsigned long prime)
{
    int i;
    unsigned long value;

    if (str == NULL
	|| *str == '\0') {

	return 0;
    }

    for (i = 0, value = 0 ; (*str) && (value < ULONG_MAX - *str) ; i++) {
	value += *str++;
    }

    return (value % prime);
}
#endif

