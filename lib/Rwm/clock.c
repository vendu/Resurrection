/*
 * clock.c - Rwm clock widget.
 * Copyright (C) 2007 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#define RWM_CLOCK_WIDTH RWM_MENU_ITEM_WIDTH //(RWM_MENU_ITEM_WIDTH + 2 * RWM_MENU_RULER_WIDTH)

volatile int _updateclock = 0;

void Rwm_sigalrm_handler(int sig);
void Rwm_clock_expose_handler(void *arg,
                              XEvent *event);

void
Rwm_update_clock(struct Rwm_clock *clock)
{
    struct R_app *app;
    struct R_wm *wm;
    time_t timeval;
    size_t len;
    unsigned int delay;

    app = clock->window->app;
    wm = app->client;
    if (wm->optflags & (RWM_DESKTOP_CLOCK_FLAG | RWM_FOCUS_CLOCK_FLAG)) {
        if (wm->optflags & RWM_DESKTOP_CLOCK_FLAG) {
            R_clear_area(clock->window,
                         clock->x,
                         clock->y,
                         clock->window->w,
                         clock->window->h,
                         False);
        }
        timeval = time(NULL);
        clock->tm = localtime(&timeval);
        delay = 60 - clock->tm->tm_sec;
        if (wm->optflags & RWM_CLOCK_12_FLAG) {
            len = strftime(clock->str, RWM_CLOCK_STR_LENGTH, "%I:%M", clock->tm);
        } else {
            len = strftime(clock->str, RWM_CLOCK_STR_LENGTH,
                           "%H:%M", clock->tm);
        }
        if (wm->optflags & RWM_DESKTOP_CLOCK_FLAG) {
            Rwm_draw_text(_clock.window);
        }
        if ((wm->optflags & RWM_FOCUS_CLOCK_FLAG) && (_focus)) {
            if (_focus->parent) {
                _focus->parent->chain->timestr = _clock.str;
                Rwm_draw_text(_focus->parent->chain);
            }
        }
//        R_global.synch = 1;
        alarm(delay);
    }
    XSync(app->display,
          False);

    return;
}

int
Rwm_create_clock_window(struct R_app *app)
{
    struct R_wm *wm;
    struct R_window *root;
    struct R_window *window;

    wm = app->client;
    root = app->window;
    window = R_create_window(app,
                             root,
                             R_WINDOW_OVERRIDE_REDIRECT_FLAG);
    if (window == NULL) {

        return FALSE;
    }
    window->font = _menufont;
#if (USE_NEW_MENU)
    Rwm_move_resize_window(window,
                           2 * RWM_MENU_ITEM_WIDTH
                           + 4 * RWM_MENU_RULER_WIDTH,
                           0,
                           RWM_CLOCK_WIDTH,
                           RWM_MENU_ITEM_HEIGHT);
#else
    Rwm_move_resize_window(window,
                           2 * RWM_MENU_ITEM_WIDTH,
                           0,
                           RWM_CLOCK_WIDTH,
                           RWM_MENU_ITEM_HEIGHT);
#endif
    window->typeflags = R_WINDOW_MENU_FLAG;
    _clock.window = window;
    wm->clock = window;

    return TRUE;
}

int
Rwm_init_clock_window(struct R_window *window)
{
    struct R_image *image;

    image = R_load_image_imlib2(window->app,
                                RESURRECTION_IMAGE_SEARCH_PATH "widget/winh.png",
                                NULL);
    image->w = R_TILE_FIT_IMAGE;
    image->h = RWM_MENU_ITEM_HEIGHT;
    window->images[R_WINDOW_NORMAL_STATE] = image;
    RWM_SET_WINDOW_IMAGE(window, R_WINDOW_NORMAL_STATE);
    window->timestr = _clock.str;
    Rwm_draw_text(window);
#if 0
    R_set_background_imlib2(image,
                            window,
                            RWM_MENU_ITEM_WIDTH,
                            RWM_MENU_ITEM_HEIGHT,
                            0);
#endif
    

    return TRUE;
}

int
Rwm_init_clock_gc(struct R_app *app)
{
    GC newgc;
    XFontStruct *fontinfo;
    XGCValues gcvalues;
    /* TODO: color allocation */

    gcvalues.foreground = WhitePixel(app->display,
                                     DefaultScreen(app->display));
    fontinfo = XLoadQueryFont(app->display, "fixed");
    if (!fontinfo) {

        return FALSE;
    }
    gcvalues.font = fontinfo->fid;
    newgc = XCreateGC(app->display,
                      _clock.window->id,
                      GCForeground | GCFont,
                      & gcvalues);
    if (!newgc) {

        return FALSE;
    }
    _clock.gc = newgc;
    _clock.h = X_FONT_HEIGHT(fontinfo);
    _clock.w = 5 * X_FONT_WIDTH(fontinfo);
    _clock.x = 8;
    _clock.y = (RWM_MENU_ITEM_HEIGHT + _clock.h) >> 1;
    XFree(fontinfo);

    return TRUE;
}

void
Rwm_init_clock_events(struct R_window *window)
{
    R_set_window_event_handler(window,
                               Expose,
                               Rwm_clock_expose_handler);
    R_add_window_events(window,
                        ExposureMask);

    return;
}

void
Rwm_init_clock_time(struct Rwm_clock *clock)
{
    unsigned int delay;

//    _updateclock = 1;
    SIGNAL(SIGALRM, Rwm_sigalrm_handler);
    Rwm_update_clock(clock);

    return;
}

void
Rwm_sigalrm_handler(int sig)
{
//    Rwm_update_clock(&_clock);
    R_global.leavefunc = Rwm_update_clock;
    R_global.leavearg = &_clock;
#if 0
    Rwm_send_proto(R_global.app->window,
                   RWM_SYNCH_PROTO,
                   CurrentTime);
#endif

    return;
}

void
Rwm_clock_expose_handler(void *arg,
                         XEvent *event)
{
    if (!event->xexpose.count) {
//        Rwm_update_clock(&_clock);
        _updateclock = 1;
    }

    return;
}

int
Rwm_init_clock(struct R_app *app)
{
    struct R_window *window;
    Window win;
    int dummyi;

    if (!Rwm_create_clock_window(app)) {

        return FALSE;
    }
    if (!Rwm_init_clock_window(_clock.window)) {
        
        return FALSE;
    }
    if (!Rwm_init_clock_gc(app)) {

        return FALSE;
    }

    XGetInputFocus(app->display,
                   &win,
                   &dummyi);
    window = R_find_window(win);
    if ((window) && (window->parent)) {
        _focus = window;
    }

#if 0
    SIGNAL(SIGALRM, Rwm_sigalrm_handler);
    Rwm_update_clock(&_clock);
#endif
    Rwm_init_clock_events(_clock.window);
    R_add_window(_clock.window);
    R_map_window_raised(_clock.window);
    Rwm_init_clock_time(&_clock);

    return TRUE;
}

