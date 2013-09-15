/*
 * client.c - Resurrection window manager client routines.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#define RWMLAZYSYNC 1
#define DEBUG_ATOM  0
#define KLUDGE      0
#define RWMKLUDGES  0
#define NEWKLUDGES  1

void Rwm_client_configurerequest_handler(void *arg,
                                         XEvent *event);
void Rwm_client_resizerequest_handler(void *arg,
                                      XEvent *event);
void Rwm_client_maprequest_handler(void *arg,
                                   XEvent *event);
void Rwm_client_unmapnotify_handler(void *arg,
                                    XEvent *event);
void Rwm_client_destroynotify_handler(void *arg,
                                      XEvent *event);
void Rwm_client_buttonpress_handler(void *arg,
                                    XEvent *event);
void Rwm_client_enternotify_handler(void *arg,
                                    XEvent *event);
#if 0
void Rwm_client_leavenotify_handler(void *arg,
                                    XEvent *event);
void Rwm_client_focusout_handler(void *arg,
                                 XEvent *event);
#endif
#if 0
void Rwm_client_reparentnotify_handler(void *arg,
                                       XEvent *event);
#endif
void Rwm_client_propertynotify_handler(void *arg,
                                       XEvent *event);

extern XSizeHints Rwm_sizehints;

void
Rwm_init_client_events(struct R_window *window)
{
    long mask;

    if (!window->eventhandlers) {
        window->eventhandlers = calloc(LASTEvent,
                                       sizeof (R_eventhandler_t *));
        window->nevents = LASTEvent;
    }
    mask = R_set_window_event_handler(window,
                                      MapRequest,
                                      Rwm_client_maprequest_handler);
    mask = R_set_window_event_handler(window,
                                      ConfigureRequest,
                                      Rwm_client_configurerequest_handler);
    mask = R_set_window_event_handler(window,
                                      ResizeRequest,
                                      Rwm_client_resizerequest_handler);
#if 0
    mask = R_set_window_event_handler(window,
                                      MapNotify,
                                      Rwm_client_mapnotify_handler);
#endif
    mask = R_set_window_event_handler(window,
                                      UnmapNotify,
                                      Rwm_client_unmapnotify_handler);
    mask = R_set_window_event_handler(window,
                                      DestroyNotify,
                                      Rwm_client_destroynotify_handler);
    mask = R_set_window_event_handler(window,
                                      EnterNotify,
                                      Rwm_client_enternotify_handler);
#if 0
    mask = R_set_window_event_handler(window,
                                      ButtonPress,
                                      Rwm_client_buttonpress_handler);
    mask = R_set_window_event_handler(window,
                                      LeaveNotify,
                                      Rwm_client_leavenotify_handler);
    mask = R_set_window_event_handler(window,
                                      FocusOut,
                                      Rwm_client_focusout_handler);
#endif
#if 0
    mask = R_set_window_event_handler(window,
                                      FocusIn,
                                      Rwm_client_focusin_handler);

    mask = R_set_window_event_handler(window,
                                      ReparentNotify,
                                      Rwm_client_reparentnotify_handler);
#endif
    mask = R_set_window_event_handler(window,
                                      PropertyNotify,
                                      Rwm_client_propertynotify_handler);
#ifdef KLUDGE
    _ignbadwindow = 1;
#endif
    R_add_window_events(window,
                        PropertyChangeMask
                        | EnterWindowMask);

    return;
}

#if 0
void
Rwm_init_override_events(struct R_window *window)
{
    long mask;

    if (!window->eventhandlers) {
        window->eventhandlers = calloc(LASTEvent,
                                       sizeof (R_eventhandler_t *));
        window->nevents = LASTEvent;
    }
    mask = R_set_window_event_handler(window,
                                      DestroyNotify,
                                      Rwm_client_destroynotify_handler);
    R_add_window_events(window,
                        StructureNotifyMask);

    return;
}
#endif

void
Rwm_add_client_events(struct R_window *client)
{
    if (!client->id) {

        return;
    }
    R_add_window_events(client,
                        PropertyChangeMask
                        | EnterWindowMask);

    return;
}

void
Rwm_resize_client(struct R_window *client,
                  int w,
                  int h)
{
    struct R_app *app;
    struct R_wm *wm;
    struct R_window *frame;
    struct R_window *window = NULL;
    long dummyl;
    int winw;
    int winh;
    int framew;
    int frameh;
    int x;
    int y;
    int border;
    float inc;
    float winc;
    float hinc;
    float wr;
    float hr;

    if (!client->id) {

        return;
    }
    app = client->app;
    wm = app->client;
    frame = client->parent;
    if (client->typeflags
        & (R_WINDOW_TOPLEVEL_FLAG | R_WINDOW_TRANSIENT_FLAG)) {
        border = client->border;
        w = min2(w + 2 * border, wm->desktop->w);
        h = min2(h + 2 * border, wm->desktop->h);
        _ignbadwindow = 1;
        if (!XGetWMNormalHints(client->app->display,
                               client->id,
                               &Rwm_sizehints,
                               &dummyl)) {
            memset(&Rwm_sizehints, 0, sizeof(Rwm_sizehints));
        } else if (w == wm->desktop->w && h == wm->desktop->h) {
            winc = Rwm_sizehints.width_inc;
            hinc = Rwm_sizehints.height_inc;
            wr = w / winc;
            hr = h / hinc;
            inc = min(wr, hr);
            w = (int)(inc * (float)(winc));
            h = (int)(inc * (float)(hinc));
        }
#if (!RWMLAZYSYNC)
        XSync(client->app->display, False);
        _ignbadwindow = 0;
#endif
        if (frame) {
            if (w == wm->desktop->w) {
                frame->x = 0;
                w = wm->desktop->w - frame->left - frame->right;
            }
            if (h == wm->desktop->h) {
#if (RWM_EXEC_RL)
                frame->y = RL_BUTTON_HEIGHT + RWM_MENU_ITEM_HEIGHT;
#else
                frame->y = RWM_MENU_ITEM_HEIGHT;
#endif
                h = wm->desktop->h - RWM_MENU_ITEM_HEIGHT - frame->top - frame->bottom;
            }
            framew = w + frame->left + frame->right;
            frameh = h + frame->top + frame->bottom;
            frame->w = framew;
            frame->h = frameh;

            Rwm_move_resize_window(frame,
                                   frame->x,
                                   frame->y,
                                   framew,
                                   frameh);
            Rwm_move_resize_window(client,
                                   frame->left,
                                   frame->top,
                                   w,
                                   h);
#if 0
            Rwm_move_resize_window(client,
                                   client->x,
                                   client->y,
                                   w,
                                   h);
#endif
            /* top */
            window = frame->chain;
            x = 0;
            y = 0;
            winw = framew;
            winh = frame->top;
            R_free_image_imlib2(window->image);
            Rwm_move_resize_window(window,
                                   x,
                                   y,
                                   winw,
                                   winh);
#if (USE_IMLIB2)
            if (client->str && !(window->typeflags & R_WINDOW_TRANSIENT_FLAG)) {
                window->str = client->str;
//            window->font = _titlefont;
                Rwm_draw_text(window);
            } else {
                R_render_image_imlib2(window->image,
                                      window,
                                      winw,
                                      winh,
                                      0);
                R_set_background_imlib2(window->image,
                                        window,
                                        winw,
                                        winh,
                                        0);
            }
#endif
            
            /* bottom */
            window = window->chain;
            y = frame->top + h;
//        winh = 2 * border + frame->bottom;
            winh = frame->bottom;
            R_free_image_imlib2(window->image);
            Rwm_move_resize_window(window,
                                   x,
                                   y,
                                   winw,
                                   winh);
#if (USE_IMLIB2)
            R_render_image_imlib2(window->image,
                                  window,
                                  winw,
                                  winh,
                                  0);
            R_set_background_imlib2(window->image,
                                    window,
                                    winw,
                                    winh,
                                    0);
#endif
            
            /* left */
            window = window->chain;
            x = 0;
            y = 0;
            winw = frame->left;
            winh = 2 * border + frameh;
            R_free_image_imlib2(window->image);
            Rwm_move_resize_window(window,
                                   x,
                                   y,
                                   winw,
                                   winh);
#if (USE_IMLIB2)
            R_render_image_imlib2(window->image,
                                  window,
                                  winw,
                                  winh,
                                  0);
            R_set_background_imlib2(window->image,
                                    window,
                                    winw,
                                    winh,
                                    0);
#endif
            /* right */
            window = window->chain;
            x = frame->left + w;
            winw = frame->right;
            R_free_image_imlib2(window->image);
            Rwm_move_resize_window(window,
                                   x,
                                   y,
                               winw,
                                   winh);
#if (USE_IMLIB2)
            R_render_image_imlib2(window->image,
                                  window,
                                  winw,
                                  winh,
                                  0);
            R_set_background_imlib2(window->image,
                                    window,
                                    winw,
                                    winh,
                                    0);
#endif
        }
//        Rwm_send_configure(client);
    } else {
        x = client->x;
        y = client->y;
        border = client->border;
        winw = min2(w + 2 * border, wm->desktop->w);
        winh = min2(h + 2 * border, wm->desktop->h - RWM_MENU_ITEM_HEIGHT);
        Rwm_move_resize_window(client,
                               x,
                               y,
                               winw, // + 2 * border,
                               winh); // + 2 * border);
    }

    return;
}

void
Rwm_move_resize_client(struct R_window *client,
                       int x,
                       int y,
                       int w,
                       int h)
{
    struct R_app    *app;
    struct R_wm     *wm;
    struct R_window *frame;
    int border;
    int winw;
    int winh;

    if (!client->id) {

        return;
    }
    app = client->app;
    wm = app->client;
    frame = client->parent;
#if 0
    x = max2(x, 0);
    y = max2(y, 0);
#endif
    w = max2(w, 1);
    h = max2(h, 1);
    border = client->border;
    winw = w;
    winh = h;
    winw = min2(winw + 2 * border, wm->desktop->w);
    winh = min2(winh + 2 * border, wm->desktop->h - RWM_MENU_ITEM_HEIGHT);
    if (client->typeflags
        & (R_WINDOW_TOPLEVEL_FLAG | R_WINDOW_TRANSIENT_FLAG)) {
        if (frame) {
            client->x = frame->left;
            client->y = frame->top;
            frame->x = x;
            frame->y = y;
        } else {
            client->x = x;
            client->y = y;
        }
//        client->y = min2(y, RWM_MENU_ITEM_HEIGHT);
#if 0
        Rwm_move_window(client,
                        client->x,
                        client->y);
#endif
        Rwm_resize_client(client,
                          winw,
                          winh);
    } else {
        winw = min2(winw, wm->desktop->w);
        winh = min2(winh, wm->desktop->h - RWM_MENU_ITEM_HEIGHT);
        Rwm_move_resize_window(client,
                               x,
                               y,
                               winw,
                               winh);
    }

    return;
}

void
Rwm_client_configurerequest_handler(void *arg,
                                    XEvent *event)
{
    struct R_wm *wm;
    struct R_window *client;
    struct R_window *frame;
    unsigned long flags;
    XConfigureRequestEvent *request;
    XSizeHints *sizehints;
    XSetWindowAttributes attr;
    Window win;

    client = arg;
    if (!client->id) {

        return;
    }
    wm = client->app->client;
    request = &event->xconfigurerequest;
    Rwm_configure_client(client, event);
#if 0
    _ignbadwindow = 1;
    _ignbadmatch = 1;
    border = client->border;
    XSetWindowBorderWidth(client->app->display,
                          client->id,
                          border);
    attr.do_not_propagate_mask = CWDontPropagate;
    XSetWindowAttributes(client->app->display,
                         client,
                         CWDontPropagate,
                         &attr);
#if (!RWMLAZYSYNC)
    XSync(client->app->display,
          False);
    _ignbadwindow = 0;
    _ignbadmatch = 0;
#endif
#endif
#if 0
    client->w = min2(client->w, wm->desktop->w - client->x);
    client->h = min2(client->h, wm->desktop->h - client->y - RWM_MENU_ITEM_HEIGHT);
#endif
    frame = client->parent;
    if (frame) {
        Rwm_move_resize_client(client,
                               client->x,
                               client->y,
                               client->w,
                               client->h);
    } else {
        Rwm_move_resize_window(client,
                               client->x,
                               client->y,
                               client->w,
                               client->h);
    }
    R_raise_window(client);
    Rwm_send_configure(client);
    
    return;
}

void
Rwm_client_resizerequest_handler(void *arg,
                                 XEvent *event)
{
    struct R_app *app;
    struct R_wm *wm;
    struct R_window *frame;
    struct R_window *client;
    XResizeRequestEvent *request;
    int x;
    int y;
    XSizeHints *sizehints;
    long dummyl;
    int wmin;
    int hmin;
    Window win;
    XWindowAttributes attr;

    client = arg;
    if (!client->id) {

        return;
    }
    app = client->app;
    wm = app->client;
    request = &event->xresizerequest;
    if (!(client->typeflags & R_WINDOW_OVERRIDE_FLAG)) {
        Rwm_configure_client(client, event);
    }
    frame = client->parent;
    if (frame) {
        Rwm_move_resize_client(client,
                               frame->x,
                               frame->y,
                               client->w,
                               client->h);
    } else {
        client->w = min2(client->w, wm->desktop->w);
        client->h = min2(client->h, wm->desktop->h - RWM_MENU_ITEM_HEIGHT);
        Rwm_move_resize_window(client,
                               client->x,
                               client->y,
                               client->w, // + 2 * border,
                               client->h); // + 2 * border);
    }
    Rwm_send_configure(client);

    return;
}

void
Rwm_client_maprequest_handler(void *arg,
                              XEvent *event)
{
    struct R_app *app;
    struct R_wm *wm;
    struct R_window *client;
    struct R_window *frame;
    struct R_window *window;
    XMapRequestEvent *request;
    Window win;
    int border;
    int x;
    int y;
    XSizeHints *sizehints;
    long dummyl;
    XWindowAttributes attr;

    request = &event->xmaprequest;
    client = arg;
    if (!client->id) {

        return;
    }
    Rwm_get_wm_hints(client);
    app = client->app;
    wm = app->client;
    _ignbadwindow = 1;
    memset(&attr, 0, sizeof(attr));
    frame = client->parent;
    XGetWindowAttributes(client->app->display,
                         client->id,
                         &attr);
    R_move_window(client, attr.x, attr.y);
    if (attr.override_redirect) {
        client->typeflags = R_WINDOW_OVERRIDE_FLAG;
        client->x = attr.x;
        client->y = attr.y;
    } else {
#if (RWM_EXEC_RL)
        y = max2(RL_BUTTON_WIDTH + RWM_MENU_ITEM_HEIGHT, attr.y);
#else
        y = max2(RWM_MENU_ITEM_HEIGHT, attr.y);
#endif
//        Rwm_move_window(client, attr.x, attr.y);
        Rwm_move_window(client, attr.x, y);
        if (XGetTransientForHint(client->app->display,
                                 client->id,
                                 &win)) {
            client->typeflags = R_WINDOW_TRANSIENT_FLAG;
        } else {
            client->typeflags = R_WINDOW_TOPLEVEL_FLAG;
        }
    }
    if (client->typeflags & R_WINDOW_OVERRIDE_FLAG) {
        if (frame) {
            R_set_window_events(frame,
                                NoEventMask);
            client->parent = NULL;
            Rwm_free_frame(frame);
            R_free_window(client);
//            Rwm_init_override_events(client);
        } else {
            R_add_save_window(client);
        }
        Rwm_reparent_window(client,
                            wm->desktop,
                            client->x,
                            client->y);
        R_map_window_raised(client);
    } else {
        if (!frame) {
            R_add_save_window(client);
            frame = Rwm_frame_window(client);
            Rwm_init_frame_events(frame);
            Rwm_init_client_events(client);
        }
        if (frame) {
            Rwm_get_title(client);
            Rwm_reparent_window(frame,
                                wm->desktop,
                                frame->x,
                                frame->y);
            R_map_subwindows(frame);
            R_map_window_raised(frame);
        }
    }
#if (RWMKLUDGES)
    Rwm_send_configure(window);
#endif
#if (!RWMLAZYSYNC)
    XSync(client->app->display,
          False);
    _ignbadwindow = 0;
#endif
    
    return;
}

#if 0
void
Rwm_client_mapnotify_handler(void *arg,
                               XEvent *event)
{
    struct R_window *client;
    struct R_window *frame;

    client = arg;
//    client->stateflags &= ~R_WINDOW_MAPPED_FLAG;
//    _ignbadwindow = 1;
    R_add_save_window(client);
#if 0
    frame = client->parent;
    if (frame) {
        R_raise_window(frame);
    }
#endif
    R_raise_window(client);
#if (!RWMLAZYSYNC)
    XSync(client->app->display,
          False);
#endif
//    _ignbadwindow = 0;

    return;
}
#endif

void
Rwm_client_unmapnotify_handler(void *arg,
                               XEvent *event)
{
    struct R_window *client;
    struct R_window *frame;

    client = arg;
//    client->stateflags &= ~R_WINDOW_MAPPED_FLAG;
    _ignbadwindow = 1;
    R_remove_save_window(client);
    frame = client->parent;
    if (frame) {
        R_unmap_window(frame);
    }
//#if (!RWMLAZYSYNC)
    XSync(client->app->display,
          False);
//#endif
    _ignbadwindow = 0;

    return;
}

void
Rwm_client_destroynotify_handler(void *arg,
                                 XEvent *event)
{
    struct R_wm     *wm;
    struct R_window *client;
    struct R_window *frame;
    Display         *display;

    client = arg;
    if (client == _focus) {
        _focus = NULL;
    }
    display = client->app->display;
    wm = client->app->client;
    frame = client->parent;
    _ignbadwindow = 1;
    R_remove_window(client);
    if (frame) {
        client->parent = NULL;
        R_set_window_events(client,
                            NoEventMask);
        R_set_window_events(frame,
                            NoEventMask);
        Rwm_free_frame(frame);
        client->str = NULL;
        R_free_window(client);
    }
#if (!RWMLAZYSYNC)
    XSync(display,
          False);
    _ignbadwindow = 0;
#endif
#if 0
    if (wm->optflags & (RWM_DESKTOP_CLOCK_FLAG | RWM_FOCUS_CLOCK_FLAG)) {
//        Rwm_update_clock(&_clock);
        _updateclock = 1;
    }
#endif

    return;
}

void
Rwm_client_buttonpress_handler(void *arg,
                               XEvent *event)
{
    struct R_window *client;
    struct R_wm     *wm;

    client = arg;
    wm = R_global.app->client;
    if (event->xbutton.button == Button1) {
        if (wm->optflags & RWM_CLICK_FOCUS_FLAG) {
            _ignbadwindow = 1;
            R_set_input_focus(client, event->xbutton.time);
            R_raise_window(client);
#if (!RWMLAZYSYNC)
            XSync(client->app->display, False);
            _ignbadwindow = 0;
#endif
        }
    }
}

void
Rwm_client_focusin_handler(void *arg,
                           XEvent *event)
{
    struct R_wm     *wm;
    struct R_window *client;
    struct R_window *subframe;
    int i;

    client = arg;
#if 0
    if (client->typeflags & R_WINDOW_TRANSIENT_FLAG) {

        return;
    }
#endif
    wm = client->app->client;
    client->stateflags |= R_WINDOW_FOCUSED_FLAG;
    _focus = client;
#if (USE_IMLIB2)
#if 0
    i = RWM_FRAME_SUBWINDOWS;
    while (i--) {
        RWM_SET_WINDOW_IMAGE(subframe,
                             R_WINDOW_FOCUSED_STATE);
        if (subframe->str) {
            Rwm_draw_text(subframe);
        } else {
            R_render_image_imlib2(subframe->image,
                                  subframe,
                                  subframe->image->w,
                                  subframe->image->h,
                                  0);
            R_set_background_imlib2(subframe->image,
                                    subframe,
                                    subframe->image->w,
                                    subframe->image->h,
                                    0);
        }
        subframe = subframe->chain;
    }
#endif
//        R_global.synch = 1;
#endif
    if (wm->optflags & (RWM_DESKTOP_CLOCK_FLAG | RWM_FOCUS_CLOCK_FLAG)) {
        Rwm_update_clock(&_clock);
//        _updateclock = 1;
    }
    if (client->parent) {
        subframe = client->parent->chain;
        if (wm->optflags & RWM_FOCUS_CLOCK_FLAG) {
            subframe->timestr = _clock.str;
            Rwm_draw_text(subframe);
        }
    }
    _ignbadwindow = 1;
#if (!RWMLAZYSYNC)
    XSync(client->app->display,
          False);
    _ignbadwindow = 0;
#endif

    return;
}

void
Rwm_client_enternotify_handler(void *arg,
                               XEvent *event)
{
    struct R_wm     *wm;
    struct R_window *client;
    int i;

    client = arg;
    wm = R_global.app->client;
    if (wm->optflags & (RWM_SLOPPY_FOCUS_FLAG | RWM_POINTER_FOCUS_FLAG)) {
        client->stateflags |= R_WINDOW_ACTIVE_FLAG | R_WINDOW_FOCUSED_FLAG;;
        XUngrabPointer(client->app->display,
                       event->xcrossing.time);
        Rwm_set_input_focus(client, event->xcrossing.time);
    }

    return;
}

#if 0
void
Rwm_client_leavenotify_handler(void *arg,
                               XEvent *event)
{
    struct R_wm     *wm;
    struct R_window *client;
    int i;

    client = arg;
    wm = R_global.app->client;
    if (!(wm->optflags & (RWM_SLOPPY_FOCUS_FLAG))) {
        client->stateflags &= ~(R_WINDOW_ACTIVE_FLAG | R_WINDOW_FOCUSED_FLAG);
        XUngrabPointer(client->app->display,
                       event->xcrossing.time);
        Rwm_set_input_focus(wm->desktop, event->xcrossing.time);
    }

    return;
}

void
Rwm_client_focusout_handler(void *arg,
                            XEvent *event)
{
    struct R_wm     *wm;
    struct R_window *client;
    struct R_window *subframe;
    int i;

    client = arg;
    if (client->typeflags & R_WINDOW_TRANSIENT_FLAG) {

        return;
    }
    _focus = NULL;
#if 0
    if (client->timestr) {
//        free(client->timestr);
        client->timestr = NULL;
    }
#endif
    wm = client->app->client;
    client->stateflags &= ~R_WINDOW_FOCUSED_FLAG;
#if (USE_IMLIB2)
    if (client->parent) {
        subframe = client->parent->chain;
        if (wm->optflags & RWM_FOCUS_CLOCK_FLAG) {
            subframe->timestr = NULL;
            Rwm_draw_text(subframe);
        }
    }
#if 0
    i = RWM_FRAME_SUBWINDOWS;
    while (i--) {
        RWM_SET_WINDOW_IMAGE(subframe,
                             R_WINDOW_NORMAL_STATE);
        if (subframe->str) {
            Rwm_draw_text(subframe);
        } else {
            R_render_image_imlib2(subframe->image,
                                  subframe,
                                  subframe->image->w,
                                  subframe->image->h,
                                  0);
            R_set_background_imlib2(subframe->image,
                                    subframe,
                                    subframe->image->w,
                                    subframe->image->h,
                                    0);
        }
        subframe = subframe->chain;
    }
#endif
#endif
#if (!RWMLAZYSYNC)
    XSync(client->app->display,
          False);
#endif
//    R_global.synch = 1;

    return;
}
#endif /* 0 */

#if 0
void
Rwm_client_reparentnotify_handler(void *arg,
                                  XEvent *event)
{
    struct R_window *client;
    struct R_window *frame;

    client = arg;
    frame = client->parent;
    if (frame) {
        _ignbadwindow = 1;
        R_set_window_events(client,
                            NoEventMask);
#if (!RWMLAZYSYNC)
        XSync(client->app->display,
              False);
        _ignbadwindow = 0;
#endif
        R_free_window(client);
    } else if (!(client->typeflags & (R_WINDOW_TOPLEVEL_FLAG | R_WINDOW_TRANSIENT_FLAG | R_WINDOW_FRAME_FLAG))) {
        R_set_window_events(client,
                            NoEventMask);
        R_free_window(client);
    }

    return;
}
#endif

void
Rwm_client_propertynotify_handler(void *arg,
                                  XEvent *event)
{
    struct R_window *client;
#if 0
    struct Rwm_property *property;
#endif
    XSizeHints *sizehints;
    XWMHints *wmhints;
    long dummyl;
#if (DEBUG_ATOM)
    static int   first = 1;
    static FILE *errfile;
#endif

#if (DEBUG_ATOM)
    if (first) {
        errfile = fopen("/tmp/rwm.log", "a");
        first = 0;
    }
#endif
    client = arg;
#if 0
    property = Rwm_find_property(event->xproperty.atom);
#endif
    if ((event->xproperty.atom)
        && (client->typeflags
            & (R_WINDOW_TOPLEVEL_FLAG
               | R_WINDOW_TRANSIENT_FLAG))) {
        if ((_wmatoms[RWM_PROTOCOLS_ATOM])
            && event->xproperty.atom == _wmatoms[RWM_PROTOCOLS_ATOM]) {
            if (event->xproperty.state == PropertyNewValue) {
                Rwm_get_wm_protocols(client);
            } else {
                client->protoflags = 0;
            }
        } else if ((_wmatoms[RWM_MOTIF_WM_HINTS_ATOM])
                    && event->xproperty.atom
                       == _wmatoms[RWM_MOTIF_WM_HINTS_ATOM]) {
//            fprintf(stderr, "MOTIF hints detected\n");
            if (client->motifwmhints) {
                XFree(client->motifwmhints);
                client->motifwmhints = NULL;
            }
#if 0
            if (event->xproperty.state == PropertyNewValue) {
                Rwm_get_motif_wm_hints(client);
            }
#endif
        } else if (event->xproperty.atom == XA_WM_ICON_NAME) {
            if (event->xproperty.state == PropertyNewValue) {
                if (client->parent
                    && !(client->typeflags & R_WINDOW_TRANSIENT_FLAG)) {
                    Rwm_get_title(client);
                    Rwm_draw_text(client->parent->chain);
                }
            } else if ((client->parent) && (client->parent->chain->str)) {
                free(client->parent->chain->str);
                client->parent->chain->str = NULL;
            }
        } else if (event->xproperty.atom ==  XA_WM_NAME) {
            if (event->xproperty.state == PropertyNewValue) {
                if (client->parent
                    && !(client->typeflags & R_WINDOW_TRANSIENT_FLAG)) {
                    Rwm_get_title(client);
                    Rwm_draw_text(client->parent->chain);
                }
            } else if ((client->parent) && (client->parent->chain->str)) {
                free(client->parent->chain->str);
                client->parent->chain->str = NULL;
            }
        } else if (event->xproperty.atom == XA_WM_HINTS) {
            if (event->xproperty.state == PropertyNewValue) {
                Rwm_get_wm_hints(client);
            } else if (client->wmhints) {
                XFree(client->wmhints);
                client->wmhints = NULL;
            }
        } else if (event->xproperty.atom == XA_WM_NORMAL_HINTS
                   || event->xproperty.atom == XA_WM_SIZE_HINTS) {
            if (event->xproperty.state == PropertyNewValue) {
                Rwm_get_size_hints(client);
            } else if (client->sizehints) {
                XFree(client->sizehints);
                client->sizehints = NULL;
            }
        } else if (event->xproperty.atom == XA_WM_TRANSIENT_FOR) {
            if (event->xproperty.state == PropertyNewValue) {
                client->typeflags |= R_WINDOW_TRANSIENT_FLAG;
            } else {
                client->typeflags &= ~R_WINDOW_TRANSIENT_FLAG;
            }
        } else if (event->xproperty.atom) {
#if (DEBUG_ATOM)
            fprintf(errfile, "%s\n", XGetAtomName(client->app->display,event->xproperty.atom));
#else
            fprintf(stderr, "unknown atom %d: %s\n",
                    event->xproperty.atom,
                    XGetAtomName(client->app->display,event->xproperty.atom));
#endif

        }
    }
#if (!RWMLAZYSYNC)
    XSync(client->app->display,
          False);
#endif

    return;
}

