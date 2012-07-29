/*
 * util.c - Resurrection utility routines.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

static char *R_home_prefixes[] = {
    "/home",
    "/usr",
    "/usr/home",
    NULL
};

XFontStruct *
R_load_font(struct R_app *app, const char *fontname)
{
    XFontStruct *newfontstruct;

    fprintf(stderr, "FONT: %s\n", fontname);
    newfontstruct = XLoadQueryFont(app->display, fontname);
    if (newfontstruct == NULL) {
        fprintf(stderr, "NO FONT: %s\n", fontname);

	return NULL;
    }

    return newfontstruct;
}

char *
R_get_home_dir(void)
{
    char        *home;
    char        *tmp;
    char        *path;
    size_t       tlen;
    size_t       len;
    int          i;
    struct stat  sbuf;

    home = NULL;
    tmp = getenv("HOME");
    if (tmp) {
        home = strdup(tmp);
    } else {
        tmp = getenv("USER");
        if (!tmp) {
            tmp = getenv("LOGNAME");
        }
        if (tmp) {
            tlen = strlen(tmp);
            i = 0;
            do {
                path = R_home_prefixes[i];
                i++;
                len = strlen(path);
                home = calloc(1,
                            len + tlen + 2);
                strcat(home, path);
                home[len] = '/';
                strcat(home + len + 1, tmp);
                fprintf(stderr, "trying %s\n", home);
                if (stat(home,
                         &sbuf)) {
                    free(home);
                    home = NULL;

                    continue;
                }
                if (!S_ISDIR(sbuf.st_mode)) {
                    free(home);
                    home = NULL;
                }
            } while ((!home) && (R_home_prefixes[i]));
        }
    }

    return home;
}

