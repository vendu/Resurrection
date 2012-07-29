/*
 * init.c - Rvm initialization.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#define RV_THUMB_WIDTH  128
#define RV_THUMB_HEIGHT 96

#define RV_NAVI_BUTTONS 4

int Rv_init(struct R_app *app,
            int argc,
            char **argv);
struct R_window * Rv_init_view_window(struct R_app *app);
struct R_window * Rv_init_thumb_window(struct R_app *app);
struct R_window * Rv_init_navi_window(struct R_app *app);
void Rv_init_navi_images(struct R_app *app);
void Rv_init_navi_events(struct R_app *app);
void Rv_show(struct R_app *app);
void Rv_map_windows(struct R_app *app);
int Rv_init_files(struct R_app *app);
void Rv_init_view_events(struct R_window *window);
void Rv_init_thumb_events(struct R_window *window);
void Rv_view_expose_handler(void *arg,
                            XEvent *event);
void Rv_thumb_buttonpress_handler(void *arg,
                                  XEvent *event);
void Rv_prev_buttonpress_handler(void *arg,
                                 XEvent *event);
void Rv_next_buttonpress_handler(void *arg,
                                 XEvent *event);
void Rv_bskip_buttonpress_handler(void *arg,
                                  XEvent *event);
void Rv_fskip_buttonpress_handler(void *arg,
                                  XEvent *event);

static int Rv_thumb_windows;

static char *Rv_navi_image_names[] = {
    RESURRECTION_IMAGE_SEARCH_PATH "widget/prev.png",
    RESURRECTION_IMAGE_SEARCH_PATH "widget/next.png",
    RESURRECTION_IMAGE_SEARCH_PATH "widget/bskip.png",
    RESURRECTION_IMAGE_SEARCH_PATH "widget/fskip.png"
};

void
Rv_alarm_handler(int sig)
{
    Rv_set_file(R_global.app, 1);
}

int
Rv_init(struct R_app *app,
        int argc,
        char **argv)
{
    struct R_window *thumb;
    struct R_window *view;
    struct R_window *navi;
    int thumbw;
    int thumbh;

    if (!R_init(app,
                argc,
                argv)) {

        return FALSE;
    }
    view = Rv_init_view_window(app);
    if (view == NULL) {

        return FALSE;
    }
    app->window = view;
    Rv_init_view_events(view);
#if 0
    thumb = Rv_init_thumb_window(app);
    if (thumb == NULL) {

        return FALSE;
    }
    view->chain = thumb;
#endif
    app->window = view;
#if 0
    navi = Rv_init_navi_window(app);
    if (navi == NULL) {

        return FALSE;
    }
    app->navi = navi;
#if (USE_IMLIB2)
    Rv_init_navi_images(app);
#endif
    Rv_init_navi_events(app);
#endif
    Rv_init_files(app);
    Rv_init_view_bindings(app);
    Rv_map_windows(app);
    signal(SIGALRM, Rv_alarm_handler);

    return TRUE;
}

struct R_window *
Rv_init_view_window(struct R_app *app)
{
    struct R_window *view;
    unsigned long flags;

    flags = R_WINDOW_OVERRIDE_FLAG;
    view = R_create_window(app,
                           NULL,
                           flags);
    if (view == NULL) {

        return FALSE;
    }
    R_add_window(view);
    R_move_resize_window(view,
                         0,
                         -1,
                         RV_THUMB_WIDTH,
                         RV_THUMB_HEIGHT);
    RV_SET_WINDOW_COORDS(view,
                         0,
                         0);
    RV_SET_WINDOW_SIZE(view,
                       RV_THUMB_WIDTH,
                       RV_THUMB_HEIGHT);

    return view;
}

struct R_window *
Rv_init_thumb_window(struct R_app *app)
{
    struct R_window *window;
    struct R_window *curwindow;
    struct R_window *thumb;
#if (USE_IMLIB2)
    struct R_image *image;
#endif
    int screen;
    int thumbw;
    int x;
    Screen *screenptr;
    unsigned long flags;
    int i;

    flags = R_WINDOW_OVERRIDE_FLAG;
    thumb = R_create_window(app,
                            NULL,
                            flags);
    if (thumb == NULL) {

        exit(1);
    }
    R_add_window(thumb);
    screen = app->screen;
    screenptr = ScreenOfDisplay(app->display,
                                screen);
    thumbw = WidthOfScreen(screenptr);
//    thumbw = RV_THUMB_WIDTH;
    R_move_resize_window(thumb,
                         0,
                         0,
                         thumbw,
                         RV_THUMB_HEIGHT);
    RV_SET_WINDOW_COORDS(thumb,
                         0,
                         0);
    RV_SET_WINDOW_SIZE(thumb,
                       thumbw,
                       RV_THUMB_HEIGHT);
#if (USE_IMLIB2)
//    thumb->image = image;
#endif
    curwindow = thumb;
    i = 0;
//    for (x = RV_THUMB_WIDTH + 2 ;
    for (x = 0 ;
         x <= thumbw - RV_THUMB_WIDTH - 2;
         x += RV_THUMB_WIDTH + 2) {
        window = R_create_window(app,
                                 thumb,
                                 0);
        if (window == NULL) {

            exit(1);
        }
        R_move_resize_window(window,
                             x,
                             0,
                             RV_THUMB_WIDTH,
                             RV_THUMB_HEIGHT);
        RV_SET_WINDOW_COORDS(window,
                             x,
                             0);
        RV_SET_WINDOW_SIZE(window,
                           RV_THUMB_WIDTH,
                           RV_THUMB_HEIGHT);
        Rv_init_thumb_events(window);
        R_add_window(window);
        curwindow->chain = window;
        curwindow = window;
        i++;
    }
    window->chain = NULL;
    Rv_thumb_windows = i;

    return thumb;
}

struct R_window *
Rv_init_navi_window(struct R_app *app)
{
    struct R_window *navi;
    struct R_window *button;
    struct R_window *curwindow;

    navi = R_create_window(app,
                           app->window->chain,
                           0);
    if (navi == NULL) {

        return NULL;
    }
    R_add_window(navi);
    R_move_resize_window(navi,
                         0,
                         0,
                         RV_THUMB_WIDTH,
                         RV_THUMB_HEIGHT);
    curwindow = navi;
    /* prev */
    button = R_create_window(app,
                             navi,
                             0);
    if (button == NULL) {

        return NULL;
    }
    R_add_window(button);
    R_move_resize_window(button,
                         1,
                         1,
                         RV_THUMB_WIDTH / 2 - 2,
                         RV_THUMB_HEIGHT / 2 - 2);
    curwindow->chain = button;
    curwindow = button;
    /* next */
    button = R_create_window(app,
                             navi,
                             0);
    if (button == NULL) {

        return NULL;
    }
    R_add_window(button);
    R_move_resize_window(button,
                         RV_THUMB_WIDTH / 2 + 1,
                         1,
                         RV_THUMB_WIDTH / 2 - 2,
                         RV_THUMB_HEIGHT / 2 - 2);
    curwindow->chain = button;
    curwindow = button;
    /* bskip */
    button = R_create_window(app,
                             navi,
                             0);
    if (button == NULL) {

        return NULL;
    }
    R_add_window(button);
    R_move_resize_window(button,
                         1,
                         RV_THUMB_HEIGHT / 2 + 1,
                         RV_THUMB_WIDTH / 2 - 2,
                         RV_THUMB_HEIGHT / 2 - 2);
    curwindow->chain = button;
    curwindow = button;
    /* fskip */
    button = R_create_window(app,
                             navi,
                             0);
    if (button == NULL) {

        return NULL;
    }
    R_add_window(button);
    R_move_resize_window(button,
                         RV_THUMB_WIDTH / 2 + 1,
                         RV_THUMB_HEIGHT / 2 + 1,
                         RV_THUMB_WIDTH / 2 - 2,
                         RV_THUMB_HEIGHT / 2 - 2);
    curwindow->chain = button;

    return navi;
}

#if (USE_IMLIB2)
void
Rv_init_navi_images(struct R_app *app)
{
    struct R_window *navi;
    struct R_window *button;
    struct R_image *image;
    int i;

    navi = app->navi;
    button = navi->chain;
    for (i = 0 ; i < RV_NAVI_BUTTONS ; i++) {
        image = R_load_image_imlib2(app,
                                    Rv_navi_image_names[i],
                                    NULL);
        if (image == NULL) {

            continue;
        }
        button->image = image;
        R_set_background_imlib2(button->image,
                                button,
                                RV_THUMB_WIDTH / 2,
                                RV_THUMB_HEIGHT / 2,
                                0);
        button = button->chain;
    }
}
#endif

void
Rv_init_navi_events(struct R_app *app)
{
    struct R_window *navi;
    struct R_window *button;
    long mask;

    navi = app->navi;
    /* prev */
    button = navi->chain;
    R_set_window_event_handler(button,
                               ButtonPress,
                               Rv_prev_buttonpress_handler);
    mask = R_add_window_events(button,
                               ButtonPressMask);
    /* next */
    button = button->chain;
    R_set_window_event_handler(button,
                               ButtonPress,
                               Rv_next_buttonpress_handler);
    mask = R_add_window_events(button,
                               ButtonPressMask);
    /* bskip */
    button = button->chain;
    R_set_window_event_handler(button,
                               ButtonPress,
                               Rv_bskip_buttonpress_handler);
    R_add_window_events(button,
                        ButtonPressMask);
    /* fskip */
    button = button->chain;
    R_set_window_event_handler(button,
                               ButtonPress,
                               Rv_fskip_buttonpress_handler);
    R_add_window_events(button,
                        ButtonPressMask);

    return;
}

void
Rv_show(struct R_app *app)
{
    struct R_window *view;
    struct R_window *thumb;
//    struct R_window *cur;
#if (USE_IMLIB2)
    struct R_image *image;
#endif
    struct R_file *curfile;
    int vieww;
    int viewh;
    int loaded;

    loaded = FALSE;
#if (USE_IMLIB2)
    image = NULL;
#endif
    curfile = _Rvcurfile;
    view = app->window;
//    thumb = view->chain;
#if (USE_IMLIB2)
//    cur = thumb;
    while ((curfile) && !loaded) {
        image = R_load_image_imlib2(app,
                                    curfile->name,
                                    NULL);
        if (image) {
#if 0
            cur->image = image;
            cur->image->file = curfile;
            R_render_thumb_imlib2(image,
                                  cur,
                                  RV_THUMB_WIDTH,
                                  RV_THUMB_HEIGHT,
                                  R_IMAGE_FIT_FLAG);
            if (image->tpixmap) {
                XSetWindowBackgroundPixmap(cur->app->display,
                                           cur->id,
                                           image->tpixmap);
                XClearWindow(thumb->app->display, cur->id);
            }
            R_free_image_imlib2(image);
#endif
            loaded = TRUE;
            Rv_show_image(view,
                          curfile->name);
//            cur = cur->chain;
            
        } else {
            if (curfile->prev) {
                curfile->prev->next = curfile->next;
            } else {
                _Rvcurfile = curfile->next;
            }
            if (curfile->next) {
                curfile->next->prev = curfile->prev;
            } else {
                _Rvtailfile = curfile->prev;
            }
        }
        curfile = curfile->next;
    }

#if 0
    curfile = _Rvcurfile;
    while ((curfile) && (thumb)) {
        fprintf(stderr, "BAH!\n");
        image = R_load_image_imlib2(app,
                                    curfile->name,
                                    NULL);
        if (image == NULL) {
            if (curfile->prev) {
                curfile->prev->next = curfile->next;
            } else {
                curfile = _Rvcurfile = curfile->next;
            }
            if (curfile->next) {
                    curfile->next->prev = curfile->prev;
            } else {
                _Rvtailfile = curfile->prev;
            }
            curfile = curfile->next;
            
            continue;
        } else {
            image->file = curfile;
            R_render_thumb_imlib2(image,
                                  thumb,
                                  R_INTEGER_TILE_IMAGE,
                                  R_INTEGER_TILE_IMAGE,
                                  0);
            thumb->image = image;
            if (image->tpixmap) {
                XSetWindowBackgroundPixmap(thumb->app->display,
                                           thumb->id,
                                           image->tpixmap);
                XClearWindow(thumb->app->display, thumb->id);
            }
        }
        thumb = thumb->chain;
        curfile = curfile->next;
    }
#endif
#endif

    return;
}

void
Rv_map_windows(struct R_app *app)
{
    struct R_window *window;

    window = app->window;
    R_map_window(window);
    R_map_subwindows(window);
#if 0
    window = window->chain;
    R_map_window(window);
#endif
    R_map_subwindows(window);
#if 0
    window = app->navi;
    R_map_window(window);
    R_map_subwindows(window);
#endif

    return;
}

int
Rv_init_files(struct R_app *app)
{
    int i;

    for (i = 1 ; i < app->argc ; i++) {
        Rv_add_file(app->argv[i]);
    }
    _Rvcurfile = _Rvheadfile;

    return TRUE;
}

void
Rv_init_thumb_events(struct R_window *window)
{
    long mask;

    R_set_window_event_handler(window,
                               ButtonPress,
                               Rv_thumb_buttonpress_handler);
    mask = R_add_window_events(window,
                               ButtonPressMask);

    return;
}

void
Rv_init_view_events(struct R_window *window)
{
    R_set_window_event_handler(window,
                               Expose,
                               Rv_view_expose_handler);
    R_add_window_events(window,
                        ExposureMask
                        | KeyPressMask
                        | ButtonPressMask);

    return;
}

void
Rv_view_expose_handler(void *arg,
                       XEvent *event)
{
    struct R_window *window;
    static int initialized = 0;

    window = arg;
    if (!initialized) {
        Rv_show(window->app);
        initialized = 1;
    }
}

void
Rv_thumb_buttonpress_handler(void *arg,
                             XEvent *event)
{
    struct R_window *window;

    window = arg;
#if (USE_IMLIB2)
    fprintf(stderr, "BAH\n");
    if (window->image) {
        _Rvcurfile = window->image->file;
        Rv_show(window->app);
    }
#endif

    return;
}

void
Rv_prev_buttonpress_handler(void *arg,
                            XEvent *event)
{
    struct R_window *window;
    struct R_file *file;

    window = arg;
    file = _Rvcurfile;
    if ((file) && (file->prev)) {
        _Rvcurfile = file->prev;
        Rv_show(window->app);
    }

    return;
}

void
Rv_next_buttonpress_handler(void *arg,
                            XEvent *event)
{
    struct R_window *window;
    struct R_file *file;

    window = arg;
    file = _Rvcurfile;
    if ((file) && (file->next)) {
        _Rvcurfile = file->next;
        Rv_show(window->app);
    }

    return;
}

void
Rv_bskip_buttonpress_handler(void *arg,
                             XEvent *event)
{
    struct R_window *window;
    struct R_file *oldfile;
    struct R_file *curfile;
    int i;

    window = arg;
    i = Rv_thumb_windows - 1;
    oldfile = curfile = _Rvcurfile;
    while ((i--) && (curfile)) {
        curfile = curfile->prev;
    }
    if (curfile != oldfile) {
        _Rvcurfile = curfile;
        Rv_show(window->app);
    }

    return;
}

void
Rv_fskip_buttonpress_handler(void *arg,
                             XEvent *event)
{
    struct R_window *window;
    struct R_file *oldfile;
    struct R_file *curfile;
    int i;

    window = arg;
    i = Rv_thumb_windows - 1;
    oldfile = curfile = _Rvcurfile;
    while ((i--) && (curfile)) {
        curfile = curfile->next;
    }
    if (curfile != oldfile) {
        _Rvcurfile = curfile;
        Rv_show(window->app);
    }

    return;
}

