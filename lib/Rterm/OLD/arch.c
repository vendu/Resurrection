/*
 * arch.c - architecture-specific information for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#if 0
Rarch_t *
arch_alloc(void)
{
    return (calloc(1, sizeof(Rarch_t)));
}

int
arch_init(Rarch_t *arch)
{
    if (arch == NULL) {

	return -1;
    }

    arch->byteorder = arch_byte_order();

    return 0;
}

int
arch_byte_order(void)
{
    unsigned long ul;
    char *cp;

    ul = 1;

    cp = (char *)(&ul);

    if (*cp) {

	return ARCH_BYTE_ORDER_LSB_FIRST;
    } else {

	return ARCH_BYTE_ORDER_LSB_FIRST;
    }

    return -1;
}
#endif /* 0 */
