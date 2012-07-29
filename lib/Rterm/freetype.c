/*
 * freetype.c - freetype font library interface for Resurrection.
 * Copyright (C) 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#if (SUPPORT_TRUETYPE_FONTS)

int
app_init_freetype(struct R_app *app)
{
    if (FT_Init_FreeType(&app->ftlib)) {

	return -1;
    }

    return 0;
}

#endif /* SUPPORT_TRUETYPE_FONTS */

