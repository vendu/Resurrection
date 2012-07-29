/*
 * app.c - application routines for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#if 0
Rapp_t *
app_alloc(void)
{
    return (calloc(1, sizeof(Rapp_t)));
}
#endif

#if 0
/* FIXME: use free() or implement app_destroy(). */
int
app_init(struct R_app *app, const Rtext_t *appname, int argc, char *argv[])
{
    size_t len;
    char *displayname;
    Window eipcwin;

    if (app == NULL) {

	return -1;
    }

    if (appname == NULL) {
	appname = strrchr(argv[0], '/') + 1;
    }

    len = strlen(appname);

    app->name = calloc(1, len + 1);
    if (app->name == NULL) {
	
	return -1;
    }
    
    strncpy(app->name, appname, len);

    if (app_init_locale() < 0) {

	return -1;
    }

#if (SUPPORT_THREADS)
    XInitThreads();
#endif

    if (app->display == NULL) {
	app->display = XOpenDisplay(app->displayname);
	if (app->display == NULL) {
	    fprintf(stderr, "%s: can't open display %s\n",
		    app->name, XDisplayName(app->displayname));
	    
	    return -1;
	}

	if (app->displayname == NULL) {
	    displayname = XDisplayName(NULL);

	    len = strlen(displayname);

	    app->displayname = calloc(1, len + 1);
	    if (app->displayname == NULL) {

		return -1;
	    }

	    strncpy(app->displayname, displayname, len);
	}
    }

    XSynchronize(app->display, True);

    XSetErrorHandler(x_handle_error);

    app_create_rgb_colormaps(app);

    app_set_colormap(app);

    app->windowhash = app_alloc_windowhash();
    if (app->windowhash == NULL) {

	return -1;
    }

    app->window = window_create(app, NULL, 1, 1);
    if (app->window == NULL) {

	return -1;
    }

    app->optiontable = option_alloc_table();
    if (app->optiontable == NULL) {

	return -1;
    }

    app->arch = arch_alloc();
    if (app->arch == NULL) {

	return -1;
    }

    if (arch_init(app->arch) < 0) {

	return -1;
    }

    app_init_imlib2(app);
#if (SUPPORT_TRUETYPE_FONTS)
    app_init_freetype(app);
#endif

    Rglobals.app = app;

    return 0;
}
#endif /* 0 */
