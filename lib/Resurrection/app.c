/*
 * x11.c - Resurrection application interface.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#define R_DEBUG_X_ERRORS 0

int R_handle_x_io_error(Display *display, XErrorEvent *event);

char *
R_parse_display_screen(struct R_app *app, int screen)
{
    int i;
    int argc;
    char **argv;
    char *dispname;
    char *disparg;
    char *str;
    size_t len;
    
    argc = app->argc;
    argv = app->argv;
    disparg = dispname = NULL;
    for (i = 1 ; i < argc; i++) {
        if (!strcmp(argv[i],
                    "-D")) {
            disparg = argv[i + 1];
        } else if (!strcmp(argv[i],
                           "--display")) {
            disparg = argv[i + 1];
        }
    }
    if (disparg) {
        len = strlen(disparg);
        dispname = calloc(len + 1, 1);
        strncpy(dispname, disparg, len);
        str = strchr(dispname, ':');
        if (str) {
            str++;
            i += '0';
            *str = (char)screen;
        }
    }
    
    return dispname;
}

char *
R_parse_display(struct R_app *app)
{
    int i;
    int argc;
    char **argv;
    char *dispname;
    char *disparg;
    char *str;
    size_t len;
    
    argc = app->argc;
    argv = app->argv;
    disparg = dispname = NULL;
    for (i = 1 ; i < argc; i++) {
        if (!strcmp(argv[i],
                    "-D")) {
            disparg = argv[i + 1];
        } else if (!strcmp(argv[i],
                           "--display")) {
            disparg = argv[i + 1];
        }
    }
    if (disparg) {
        len = strlen(disparg);
        dispname = calloc(len + 1, 1);
        strncpy(dispname, disparg, len);
    }
    
    return dispname;
}

int
R_init_display_screen(struct R_app *app,
                      int argc,
                      char **argv,
                      int screen)
{
    char *dispname;
    Display *disp;
    int retval;

    app->wintree = calloc(256, sizeof(void *));
    if (!app->wintree) {

        return FALSE;
    }
    app->argc = argc;
    app->argv = argv;
    dispname = R_parse_display_screen(app, screen);
    fprintf(stderr, "DISP: %s\n", dispname);
    XInitThreads();
    disp = XOpenDisplay(dispname);
    if (disp == NULL) {
        
        return FALSE;
    }
    app->display = disp;
    retval = ScreenCount(disp);

    return retval;
}

int
R_init_display(struct R_app *app,
               int argc,
               char **argv)
{
    char *dispname;
    Display *disp;
    int retval;

    app->argc = argc;
    app->argv = argv;
    dispname = R_parse_display(app);
    fprintf(stderr, "DISP: %s\n", dispname);
    XInitThreads();
    disp = XOpenDisplay(dispname);
    if (disp == NULL) {
        
        return FALSE;
    }
    app->display = disp;
    retval = ScreenCount(disp);

    return retval;
}

int
R_init_screen(struct R_app *app,
              int argc,
              char **argv,
              int screen)
{
    Display *disp;

    if (!app->display
        && !R_init_display_screen(app,
                                  argc,
                                  argv,
                                  screen)) {

        return FALSE;
    }
    disp = app->display;
#if (R_DEBUG_X_ERRORS)
    XSynchronize(app->display, True);
#endif
    XSetErrorHandler(R_handle_x_error);
    XSetIOErrorHandler(R_handle_x_io_error);
    app->screen = screen;
    app->depth = DefaultDepth(disp,
                              screen);
    app->visual = DefaultVisual(disp,
                                screen);
    app->colormap = DefaultColormap(disp,
                                    screen);
#if (USE_IMLIB2)
    R_init_imlib2(app);
#endif
    R_global.app = app;

    return TRUE;
}

int
R_init(struct R_app *app,
              int argc,
              char **argv)
{
    int      screen;
    Display *disp;

    app->wintree = calloc(256, sizeof(void *));
    if (!app->wintree) {
        
        return FALSE;
    }
    fprintf(stderr, "WINTREE: %x\n", (long)app->wintree);
    if (!app->display
        && !R_init_display(app,
                           argc,
                           argv)) {

        return FALSE;
    }
    disp = app->display;
#if (R_DEBUG_X_ERRORS)
    XSynchronize(app->display, True);
#endif
    XSetErrorHandler(R_handle_x_error);
    XSetIOErrorHandler(R_handle_x_io_error);
    app->screen = DefaultScreen(app->display);
    app->depth = DefaultDepth(disp,
                              screen);
    app->visual = DefaultVisual(disp,
                                screen);
    app->colormap = DefaultColormap(disp,
                                    screen);
#if (USE_IMLIB2)
    R_init_imlib2(app);
#endif
    R_global.app = app;

    return TRUE;
}

