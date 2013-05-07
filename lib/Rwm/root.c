/*
 * root.c - Resurrection window manager root window routines.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

void Rwm_root_keypress_handler(void *arg,
                               XEvent *event);
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

extern struct R_window *Rwm_desktop_windows[];

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
                               KeyPress,
                               Rwm_root_keypress_handler);
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
Rwm_root_keypress_handler(void *arg,
                          XEvent *event)
{
    struct R_window *window = arg;
    int ctrl = event->xkey.state & ControlMask;
    int len;
    char str[8];
    KeySym keysym;
    XComposeStatus compose;

    if (ctrl) {
        len = XLookupString(&(event->xkey), str, 8, &keysym, &compose);
        switch (keysym) {
            case XK_F1:
                Rwm_switch_desktop(window->app, Rwm_desktop_windows[0]);

            case XK_F2:
                Rwm_switch_desktop(window->app, Rwm_desktop_windows[1]);

            case XK_F3:
                Rwm_switch_desktop(window->app, Rwm_desktop_windows[2]);

            case XK_F4:
                Rwm_switch_desktop(window->app, Rwm_desktop_windows[3]);

                break;
        }
    }

    return;
}

#if 0
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
        window->border = 0;
        border = window->border;
#if 0
        window->x = min2(wm->desktop->w - window->w, create->x);
        window->y = min2(wm->desktop->h - window->h, create->y);
        window->y = max2(window->y, RWM_MENU_ITEM_HEIGHT);
#else
        window->x = create->x;
        window->y = create->y;
#endif
        if (!create->override_redirect) {
#if (RWM_EXEC_RL)
            window->y = max2(window->y, RL_BUTTON_HEIGHT + RWM_MENU_ITEM_HEIGHT);
#else
            window->y = max2(window->y, RWM_MENU_ITEM_HEIGHT);
#endif
            window->w = min2(create->width + 2 * border, wm->desktop->w);
            window->h = min2(create->height + 2 * border, wm->desktop->h);
            window->h = max2(window->h, wm->desktop->h - RWM_MENU_ITEM_HEIGHT);
        } else {
            window->w = create->width;
            window->h = create->height;
        }
#if 0
        Rwm_move_resize_window(window,
                               window->x,
                               window->y,
                               window->w,
                               window->h);
#endif
        if (create->override_redirect) {
            window->typeflags = R_WINDOW_OVERRIDE_FLAG;
            Rwm_init_override_events(window);
        } else {
            window->typeflags = R_WINDOW_TOPLEVEL_FLAG;
#if 0
            frame = Rwm_frame_window(window);
            Rwm_init_frame_events(frame);
#endif
            _ignbadwindow = 1;
            _ignbadmatch = 1;
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
            Rwm_init_client_events(window);
//            R_add_window(window);
        }
        R_add_window(window);
    }

    return;
}
#endif

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
    if (create->override_redirect) {

        return;
    }
    window = R_alloc_window();
    if (window) {
        window->app = root->app;
        window->id = create->window;
        window->border = 0;
        border = window->border;
        window->x = create->x;
#if (RWM_EXEC_RL)
        window->y = max(create->y, RL_BUTTON_HEIGHT + RWM_MENU_ITEM_HEIGHT);
#else
        window->y = create->y;
#endif
        window->w = create->width;
        window->h = create->height;
        window->typeflags = R_WINDOW_TOPLEVEL_FLAG;
        _ignbadwindow = 1;
        _ignbadmatch = 1;
        XSetWindowBorderWidth(window->app->display,
                              window->id,
                              window->border);
        XSync(window->app->display,
              False);
        _ignbadwindow = 0;
        _ignbadmatch = 0;
        Rwm_init_client_events(window);
        R_add_window(window);
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
#if (RWM_EXEC_RL)
    winy = max2(request->y, RL_BUTTON_HEIGHT + RWM_MENU_ITEM_HEIGHT);
#else
    winy = max2(request->y, RWM_MENU_ITEM_HEIGHT);
#endif
    winw = request->width;
    winh = request->height;
    Rwm_move_resize_window(window,
                           winx,
                           winy,
                           winw,
                           winh);
    Rwm_send_configure(window);
    
    return;
}

void
Rwm_root_resizerequest_handler(void *arg,
                               XEvent *event)
{
    struct R_wm     *wm;
    struct R_window *window;
    XResizeRequestEvent *request;
    int winw;
    int winh;
    int x;
    int y;
    int w;
    int h;

    window = arg;
    if (!window->id) {

        return;
    }
    request = &event->xresizerequest;
    winw = request->width;
    winh = request->height;
    Rwm_resize_window(window,
                      winw,
                      winh);
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
    frame = window->parent;
    wm = window->app->client;
    if (!window->id) {

        return;
    }
//    Rwm_get_motif_wm_hints(window);
    if (!(window->typeflags & R_WINDOW_OVERRIDE_FLAG)) {
        frame = Rwm_frame_window(window);
        Rwm_init_frame_events(frame);
        R_add_save_window(window);
//        Rwm_init_client_events(window);
    } else if (frame) {
        frame = window->parent = NULL;
        Rwm_free_frame(frame);
    }
    if (frame) {
        Rwm_reparent_window(frame,
                            wm->desktop,
                            frame->x,
                            frame->y);
        R_map_window_raised(frame);
    } else {
        Rwm_reparent_window(window,
                            wm->desktop,
                            window->x,
                            window->y);
        R_map_window_raised(window);
    }
//    R_add_save_window(window);
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

