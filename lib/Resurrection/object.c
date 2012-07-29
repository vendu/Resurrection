/*
 * object.c - Resurrection object allocator.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#if 0

#include <Resurrection/Resurrection.h>

void *
R_alloc_object(int zero)
{
    void *obj;

    obj = calloc(1, sizeof(struct R_object));

    return obj;
}

void
R_free_object(void *obj)
{
//    memset(obj, 0, sizeof(struct R_object));
    free(obj);

    return;
}

#endif
