/*
 * override.c - Resurrection window manager override_redirect window routines.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

void
Rwm_override_configurerequest_handler(void *arg,
                                      XEvent *event)
{
    struct R_window *frame;
    struct R_window *window;
    XConfigureRequestEvent *request;
    unsigned long flags;
    int winx;
    int winy;
    int winw;
    int winh;
    int border;

    window = arg;
    request = &event->xconfigurerequest;
    border = request->border_width;
    flags = request->value_mask;
    if (flags & CWX) {
        window->x = request->x;
    }
    if (flags & CWY) {
        window->y = request->y;
    }
    if (flags & CWWidth) {
        window->w = request->width;
    }
    if (flags & CWHeight) {
        window->w = request->height;
    }
    winh = request->height;
    window->border = border;
    Rwm_move_resize_window(window,
                           window->x,
                           window->y,
                           window->w,
                           window->h);
    
    return;
}

void
Rwm_override_resizerequest_handler(void *arg,
                                   XEvent *event)
{
    struct R_window *frame;
    struct R_window *window;
    XResizeRequestEvent *request;
    int winw;
    int winh;
    int border;

    window = arg;
    request = &event->xresizerequest;
    winw = request->width;
    winh = request->height;
    border = window->border;
    Rwm_resize_window(window,
                      winw,
                      winh);
    
    return;
}

void
Rwm_override_maprequest_handler(void *arg,
                                XEvent *event)
{
    struct R_window *window;
    struct R_window *frame;
    XMapRequestEvent *request;
    XWindowAttributes attr;
    Window win;

    window = arg;
//    R_add_save_window(window);
    R_map_window_raised(window);
    
    return;
}

