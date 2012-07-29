/*
 * root.c - Resurrection window manager root window routines.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

void Rwm_root_configurerequest_handler(void *arg,
                                       XEvent *event);
void Rwm_root_resizerequest_handler(void *arg,
                                    XEvent *event);
void Rwm_root_maprequest_handler(void *arg,
                                 XEvent *event);
#if 0
void Rwm_root_reparentnotify_handler(void *arg,
                                     XEvent *event);
#endif

#if (R_DEBUG_WM)
#define RWM_DEBUG_ROOT_WIDTH  1024
#define RWM_DEBUG_ROOT_HEIGHT 768

int
Rwm_set_root_window(struct R_app *app)
{
    struct R_window *window;

    window = R_create_window(app,
                             NULL,
                             0);
    if (window == NULL) {

        return FALSE;
    }
    Rwm_resize_window(window,
                      RWM_DEBUG_ROOT_WIDTH,
                      RWM_DEBUG_ROOT_HEIGHT);
    window->app = app;
    window->w = RWM_DEBUG_ROOT_WIDTH;
    window->h = RWM_DEBUG_ROOT_HEIGHT;
    app->window = window;
    app->winid = window->id;
    R_add_window(window);
    R_map_window(window);

    return TRUE;
}
#else
int
Rwm_set_root_window(struct R_app *app)
{
    struct R_window *root;
    R_eventhandler_t **eventhandlers;
    int screen;
    Window rootwin;
    Screen *screenptr;

    root = R_alloc_window();
    eventhandlers = calloc(sizeof(R_eventhandler_t *),
                           LASTEvent);
    if (!eventhandlers) {
        /* FIXME: don't fail softly, return error. */
        
        return FALSE;
    }
    root->eventhandlers = eventhandlers;
    root->nevents = LASTEvent;
    root->app = app;
    screen = app->screen;
    rootwin = RootWindow(app->display,
                         screen);
    screenptr = ScreenOfDisplay(app->display,
                                screen);
    root->app = app;
    root->typeflags = R_WINDOW_ROOT_FLAG;
    root->id = rootwin;
    root->w = WidthOfScreen(screenptr);
    root->h = HeightOfScreen(screenptr);
    app->window = root;
    app->winid = rootwin;
    R_add_window(root);

    return TRUE;
}
#endif

void
Rwm_init_root_events(struct R_app *app)
{
    struct R_window *root;

    root = app->window;
    R_set_window_event_handler(root,
                               CreateNotify,
                               Rwm_root_createnotify_handler);
    R_set_window_event_handler(root,
                               ConfigureRequest,
                               Rwm_root_configurerequest_handler);
    R_set_window_event_handler(root,
                               ResizeRequest,
                               Rwm_root_resizerequest_handler);
    R_set_window_event_handler(root,
                               MapRequest,
                               Rwm_root_maprequest_handler);
#if 0
    R_set_window_event_handler(root,
                               ReparentNotify,
                               Rwm_root_reparentnotify_handler);
#endif
    R_add_window_events(root,
                        SubstructureNotifyMask
                        | SubstructureRedirectMask
                        | ResizeRedirectMask);

    return;
}

void
Rwm_root_createnotify_handler(void *arg,
                              XEvent *event)
{
    struct R_wm *wm;
    XCreateWindowEvent *create;
    struct R_window *root;
    struct R_window *window;
    struct R_window *frame;
    int border;
    Window win;
    XSetWindowAttributes attr;

    root = arg;
    wm = root->app->client;
    create = &event->xcreatewindow;
    window = R_alloc_window();
    if (window) {
        window->app = root->app;
        window->id = create->window;
//        window->border = create->border_width;
        window->border = 0;
        border = window->border;
        window->x = min2(wm->desktop->w - window->w, create->x);
        window->y = min2(wm->desktop->h - window->h, create->y);
        window->y = max2(window->y, RWM_MENU_ITEM_HEIGHT);
        window->w = min2(create->width + 2 * border, wm->desktop->w);
        window->h = min2(create->height + 2 * border, wm->desktop->h);
        window->h = max2(window->h, wm->desktop->h - RWM_MENU_ITEM_HEIGHT);
        Rwm_move_resize_window(window,
                               window->x,
                               window->y,
                               window->w,
                               window->h);
        fprintf(stderr, "CREATE(%dx%d @ (%d, %d))\n", window->w, window->h, window->x, window->y);
        R_add_window(window);
        if (create->override_redirect) {
            window->typeflags = R_WINDOW_OVERRIDE_FLAG;
            
            return;
        }
        _ignbadwindow = 1;
        _ignbadmatch = 1;
        R_add_save_window(window);
        XSetWindowBorderWidth(window->app->display,
                              window->id,
                              window->border);
#if 0
        attr.do_not_propagate_mask = EnterNotify;
        XChangeWindowAttributes(window->app->display,
                                window,
                                CWDontPropagate,
                                &attr);
#endif
        XSync(window->app->display,
              False);
        _ignbadwindow = 0;
        _ignbadmatch = 0;
#if 0
        Rwm_reparent_window(window,
                            wm->desktop,
                            window->x,
                            window->y);
#endif
        Rwm_init_client_events(window);
    }

    return;
}

#if 0
void
Rwm_root_enternotify_handler(void *arg,
                             XEvent *event)
{
    struct R_window *window;

#if 0
    window = arg;
    if (window) {
        R_set_input_focus(window);
    }
#endif

    return;
}
#endif

void
Rwm_root_configurerequest_handler(void *arg,
                                  XEvent *event)
{
    struct R_window *frame;
    struct R_window *window;
    XConfigureRequestEvent *request;
    int winx;
    int winy;
    int winw;
    int winh;

    window = arg;
    if (!window->id) {

        return;
    }
    request = &event->xconfigurerequest;
    winx = max2(0, request->x);
    winy = max2(request->y, RWM_MENU_ITEM_HEIGHT);
    winw = request->width;
    winh = request->height;
    Rwm_move_resize_window(window,
                           winx,
                           winy,
                           winw,
                           winh);
    fprintf(stderr, "CONF(%dx%d @ (%d, %d))\n", window->w, window->h, window->x, window->y);
    Rwm_send_configure(window);
    
    return;
}

void
Rwm_root_resizerequest_handler(void *arg,
                               XEvent *event)
{
    struct R_wm     *wm;
    struct R_window *frame;
    struct R_window *window;
    XResizeRequestEvent *request;
    int winw;
    int winh;
    int x;
    int y;
    int w;
    int h;

    window = arg;
//    frame = window->parent;
    if (!window->id) {

        return;
    }
    request = &event->xresizerequest;
    winw = request->width;
    winh = request->height;
#if 0
    if (!window->typeflags & R_WINDOW_OVERRIDE_FLAG) {
        if (frame) {
            wm = frame->app->client;
            x = frame->x;
            y = frame->x;
            w = window->w;
            h = window->h;
            if (winw != w) {
                w = winw;
            x = max2(frame->x, 0);
            } else {
                x = -(frame->left);
                w += frame->left + frame->right;
            }
            if (winh != h) {
                h = winh;
                y = max2(frame->y, 0);
            } else {
            y = -(frame->top);
            h += frame->top + frame->bottom;
            }
            frame->x = x;
            frame->y = y;
            frame->w = w;
            frame->h = h;
        }
    }
#endif
    Rwm_resize_window(window,
                      winw,
                      winh);
    fprintf(stderr, "RESIZE(%dx%d)\n", window->w, window->h);
    Rwm_send_configure(window);
    
    return;
}

void
Rwm_root_maprequest_handler(void *arg,
                            XEvent *event)
{
    struct R_wm *wm;
    struct R_window *window;
    struct R_window *frame;
    XMapRequestEvent *request;
    XWindowAttributes attr;
    Window win;
    int winx;
    int winy;
    int winw;
    int winh;
    int wmin;
    int hmin;
    XSizeHints *sizehints;
    long dummyl;

    request = &event->xmaprequest;
    window = arg;
    wm = window->app->client;
    if (!window->id) {

        return;
    }
#if 0
    if (window->typeflags & R_WINDOW_OVERRIDE_FLAG) {
        R_map_window_raised(window);

        return;
    }
    winx = window->x;
    winy = window->y;
    winw = window->w;
    winh = window->h;
    frame = window->parent;
//    Rwm_get_motif_wm_hints(window);
    if (!frame && (!(window->typeflags & R_WINDOW_OVERRIDE_FLAG))) {
        frame = Rwm_frame_window(window);
        Rwm_init_frame_events(frame);
    }
    if (!frame) {
        Rwm_move_resize_window(window,
                               winx,
                               winy,
                               winw,
                               winh);
        R_map_window_raised(window);
    } else {
        Rwm_reparent_window(frame,
                            wm->desktop,
                            winx,
                            winy);
        Rwm_move_resize_client(window,
                               winx,
                               winy,
                               winw,
                               winh);
        R_map_window_raised(frame);
    }
#endif
    fprintf(stderr, "MAP(%dx%d @ (%d, %d))\n", window->w, window->h, window->x, window->y);
    R_map_window_raised(window);
    Rwm_send_configure(window);

    return;
}

#if 0
void
Rwm_root_reparentnotify_handler(void *arg,
                                XEvent *event)
{

    return;
}
#endif /* 0 */

