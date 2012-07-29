/*
 * freetype.c - Resurrection Freetype font library support.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

int
R_init_freetype(struct R_app *app)
{
    if (FT_Init_FreeType(&app->ftlib)) {

        return FALSE;
    }

    return TRUE;
}

