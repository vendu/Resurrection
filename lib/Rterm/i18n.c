/*
 * i18n.c - internationalization for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

int
app_init_locale(const char *locname)
{
    if (!setlocale(LC_ALL, locname)) {
        setlocale(LC_ALL, NULL);
    }

    if (!XSupportsLocale()) {
	fprintf(stderr, "X doesn't support locale %s\n",
		setlocale(LC_ALL, ""));

	return -1;
    }
    fprintf(stderr, "locale set to %s\n", setlocale(LC_ALL, locname));

    XSetLocaleModifiers("");

    return 0;
}

