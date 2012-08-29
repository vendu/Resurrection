/*
 * util.c - Resurrection window manager utility routines.
 * Copyright (C) 2006 - 2007 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

GC _utilgc;

void Rwm_draw_text(struct R_window *window);
#if 0
void Rwm_draw_string(char *str,
                     struct R_window *window,
                     int x,
                     int y,
                     int render);
#endif

void
Rwm_print_configurerequest(XEvent *event)
{
    XConfigureRequestEvent *request = &event->xconfigurerequest;
    struct R_window *window = R_find_window(request->window);

    fprintf(stderr, "ConfigureRequest -> %x\n", request->window);
    fprintf(stderr, "----------------\n");
    fprintf(stderr, "\tx\t\t%d\n", request->x);
    fprintf(stderr, "\ty\t\t%d\n", request->y);
    fprintf(stderr, "\twidth\t\t%d\n", request->width);
    fprintf(stderr, "\theight\t\t%d\n", request->height);
    fprintf(stderr, "\tborder\t\t%d\n", request->border_width);
    fprintf(stderr, "\tabove\t\t%d\n", request->above);
    fprintf(stderr, "\tdetail\t\t%d\n", request->detail);
    fprintf(stderr, "\tvalue_mask\t%x\n", request->value_mask);
    fprintf(stderr, "window->x = %d\n", window->x);
    fprintf(stderr, "window->y = %d\n", window->y);
    fprintf(stderr, "window->w = %d\n", window->w);
    fprintf(stderr, "window->h = %d\n", window->h);
}

void
Rwm_print_resizerequest(XEvent *event)
{
    XConfigureRequestEvent *request = &event->xresizerequest;

    fprintf(stderr, "ResizeRequest -> %x\n", request->window);
    fprintf(stderr, "----------------\n");
    fprintf(stderr, "\twidth\t\t%d\n", request->width);
    fprintf(stderr, "\theight\t\t%d\n", request->height);
}

void
Rwm_print_configurenotify(XEvent *event)
{
    XConfigureEvent *configure = &event->xconfigure;

    fprintf(stderr, "ConfigureNotify -> %x\n", configure->window);
    fprintf(stderr, "---------------\n");
    fprintf(stderr, "\tx\t\t%d\n", configure->x);
    fprintf(stderr, "\ty\t\t%d\n", configure->y);
    fprintf(stderr, "\tw\t\t%d\n", configure->width);
    fprintf(stderr, "\th\t\t%d\n", configure->height);
    fprintf(stderr, "\tborder\t\t%d\n", configure->border_width);
    fprintf(stderr, "\tabove\t%d\n", configure->above);
    fprintf(stderr, "\toverride\t%d\n", configure->override_redirect);

    return;
}

void
Rwm_send_configure(struct R_window *window)
{
    XEvent configure;
    struct R_window *frame;
    int x;
    int y;

    if (!window || !window->id) {

        return;
    }
    memset(&configure, 0, sizeof(configure));
    configure.xconfigure.type = ConfigureNotify;
    configure.xconfigure.window = window->id;
    x = window->x;
    y = window->y;
#if 0
    frame = window->parent;
    if (frame) {
        x += frame->x;
        x += frame->y;
    }
#endif
    configure.xconfigure.x = x;
    configure.xconfigure.y = y;
    configure.xconfigure.width = window->w;
    configure.xconfigure.height = window->h;
    configure.xconfigure.border_width = window->border;
    configure.xconfigure.above = window->above;
    if (window->typeflags & R_WINDOW_OVERRIDE_FLAG) {
        configure.xconfigure.override_redirect = True;
    } else {
        configure.xconfigure.override_redirect = False;
    }
//    Rwm_print_configurenotify(&configure);
    _ignbadwindow = 1;
    XSendEvent(window->app->display,
               window->id,
               True,
               NoEventMask,
               &configure);
    XSync(window->app->display,
          False);
    _ignbadwindow = 0;

    return;
}

void
Rwm_send_proto(struct R_window *window, int proto, Time timestamp)
{
    XEvent event;
    XClientMessageEvent *message = &event.xclient;

    if (!window->id) {

        return;
    }
    memset(&event, 0, sizeof(event));
    message->type = _wmatoms[RWM_PROTOCOLS_ATOM];
    if (message->type) {
        message->format = 32;
        message->data.l[0] = _wmprotos[proto];
        message->data.l[1] = timestamp;
        _ignbadwindow = 1;
        XSendEvent(window->app->display,
                   window->id,
                   False,
                   0,
                   &event);
        XSync(window->app->display,
              False);
        _ignbadwindow = 0;
    }

    return;
}

void
Rwm_reparent_window(struct R_window *window,
                    struct R_window *parent,
                    int x,
                    int y)
{
    if (!window->id || !parent->id) {

        return;
    }
    RWM_SET_WINDOW_COORDS(window,
                          x,
                          y);
    XReparentWindow(window->app->display,
                    window->id,
                    parent->id,
                    x,
                    y);

    return;
}

void
Rwm_move_window(struct R_window *window,
                int x,
                int y)
{
    struct R_window *frame;

    if (!window->id) {

        return;
    }
    frame = window->parent;
    RWM_SET_WINDOW_COORDS(window, x, y);
    _ignbadwindow = 1;
    XMoveWindow(window->app->display,
                window->id,
                x,
                y);
    XSync(window->app->display,
          False);
    _ignbadwindow = 0;

    return;
}

void
Rwm_resize_window(struct R_window *window,
                int w,
                int h)
{
    if (!window->id) {

        return;
    }
    _ignbadwindow = 1;
    XResizeWindow(window->app->display,
                  window->id,
                  w,
                  h);
    XSync(window->app->display,
          False);
    _ignbadwindow = 0;
#if 0
    if (window->typeflags
        & (R_WINDOW_TOPLEVEL_FLAG
           | R_WINDOW_TRANSIENT_FLAG
           | R_WINDOW_OVERRIDE_FLAG)) {
        Rwm_send_configure(window);
    }
#endif

    return;
}

void
Rwm_move_resize_window(struct R_window *window,
                       int x,
                       int y,
                       int w,
                       int h)
{
    struct R_window *frame;
    int border;

    if (!window->id) {

        return;
    }
    _ignbadwindow = 1;
    XMoveResizeWindow(window->app->display,
                      window->id,
                      x,
                      y,
                      w,
                      h);
    XSync(window->app->display,
          False);
    _ignbadwindow = 0;
#if 0
    frame = window->parent;
    if (frame) {
        x += frame->x;
        y += frame->y;
    }
    if (window->typeflags
        & (R_WINDOW_TOPLEVEL_FLAG
           | R_WINDOW_TRANSIENT_FLAG
           | R_WINDOW_OVERRIDE_FLAG)) {
        Rwm_send_configure(window);
    }
#endif
    RWM_SET_WINDOW_COORDS(window, x, y);
    RWM_SET_WINDOW_SIZE(window, w, h);

    return;
}

#if 0
void
Rwm_place_window(struct R_window *window)
{
    XMoveResizeWindow(window->app->display,
                      window->id,
                      window->x,
                      window->y,
                      window->w,
                      window->h);

    return;
}
#endif

void
Rwm_get_title(struct R_window *window)
{
    struct R_window *frame;
    XTextProperty textprop;

    frame = window->parent;
    if (frame) {
        _ignbadwindow = 1;
        if (window->str) {
            free(window->str);
        }
        window->str = frame->chain->str = NULL;
        if ((XGetWMIconName(window->app->display,
                            window->id,
                            &textprop)
             || XGetWMName(window->app->display,
                           window->id,
                           &textprop))) {
            window->typeflags |= R_WINDOW_TOPLEVEL_FLAG;
            window->str = strdup(textprop.value);
            XFree(textprop.value);
            frame->chain->str = window->str;
        }
        XSync(window->app->display,
              False);
        _ignbadwindow = 0;
    }

    return;
}

#define R_CENTER_TEXT      -1
#define R_ALIGN_TEXT_RIGHT -2
#if (USE_IMLIB2)
void Rwm_draw_text(struct R_window *window)
{
    int render = 1;

    if (!window->id) {

        return;
    }
    if (window->str) {
        Rwm_draw_string(window->str, window, 8, R_CENTER_TEXT, render);
        render = 0;
#if 0
        if (window->timestr) {
            Rwm_draw_string(window->timestr, window,
                            R_ALIGN_TEXT_RIGHT, R_CENTER_TEXT, render);
        }
#endif
    } else if (window->timestr) {
        Rwm_draw_string(window->timestr, window,
                        R_CENTER_TEXT, R_CENTER_TEXT, render);
    }
}

void
Rwm_draw_string(char *str,
                struct R_window *window,
                int x,
                int y,
                int render)
{
    int textw;
    int texth;
    Imlib_Image textimg;
    int screen;
    struct R_color *color;

    if (!str || !window->id) {

        return;
    }
    if (window->image) {
        imlib_context_set_font(window->font);
        imlib_get_text_size(str, &textw, &texth);
//    x = (RWM_MENU_ITEM_WIDTH - textw) / 2;
        if (x == R_CENTER_TEXT) {
            x = (window->w - textw) / 2;
        } else if (x == R_ALIGN_TEXT_RIGHT) {
            x = window->w - textw - 8;
        }
        if (y == R_CENTER_TEXT) {
            y = (window->h - texth) / 2;
        } else {
            y = window->h;
        }
        textimg = imlib_create_image(textw, texth);
        if (textimg) {
            color = &_wmcolors[RWM_TEXT_COLOR_INDEX];
            if ((render) || !(window->image->pixmap) && (window->typeflags & R_WINDOW_TOPLEVEL_FLAG)) {
                if (R_render_image_imlib2(window->image,
                                          window,
                                          window->image->w,
                                          window->image->h,
                                          0) < 0) {
                    
                    return;
                }
            }
            imlib_context_set_image(textimg);
            imlib_context_set_mask(0);
            imlib_image_clear();
            imlib_image_set_has_alpha(1);
            imlib_context_set_color(255, 215, 0, 255);
//            imlib_context_set_color(color->r, color->g, color->b, color->a);
            imlib_text_draw(0,
                            0,
                            str);
            imlib_context_set_blend(1);
            imlib_context_set_drawable(window->image->pixmap);
            imlib_render_image_on_drawable(x,
                                           y);
            imlib_free_image();
            XSetWindowBackgroundPixmap(window->app->display,
                                       window->id,
                                       window->image->pixmap);
            XClearWindow(window->app->display, window->id);
            R_free_image_imlib2(window->image);
        }
    }

    return;
}
#else
void
Rwm_draw_string(struct R_window *window)
{
    ;
}
#endif

#if 0
void
Rwm_draw_text(struct R_window *window)
{
    if (window->str) {
        Rwm_draw_string(window->parent->chain);
    }

    return;
}
#endif

void
Rwm_get_size_hints(struct R_window *window)
{
    XSizeHints *sizehints;
    long dummyl = 0;

    if (!window->id) {

        return;
    }
    sizehints = window->sizehints;
    if (sizehints) {
        XFree(sizehints);
        window->sizehints = NULL;
    }
    sizehints = XAllocSizeHints();
    memset(sizehints, 0, sizeof(*sizehints));
    _ignbadwindow = 1;
    if (!XGetWMNormalHints(window->app->display,
                           window->id,
                           sizehints,
                           &dummyl)) {
        memset(sizehints, 0, sizeof(*sizehints));
        if (!XGetWMSizeHints(window->app->display,
                             window->id,
                             sizehints,
                             &dummyl,
                             XA_WM_SIZE_HINTS)) {
            XFree(sizehints);
            sizehints = NULL;
        }
    }
    window->sizehints = sizehints;
    XSync(window->app->display,
          False);
    _ignbadwindow = 0;

    return;
}

void
Rwm_delete_window(struct R_window *client)
{
    XEvent ev;

    ev.type = ClientMessage;
    ev.xclient.window = client->id;
    ev.xclient.message_type = _wmatoms[RWM_PROTOCOLS_ATOM];
    ev.xclient.format = 32;
    ev.xclient.data.l[0] = _wmprotos[RWM_DELETE_WINDOW_PROTO];
    ev.xclient.data.l[1] = CurrentTime;
    XSendEvent(client->app->display,
               client->id,
               False,
               NoEventMask,
               &ev);

    return;
}

void
Rwm_configure_client(struct R_window *client, XEvent *event)
{
    struct R_window *frame;
    XSizeHints *sizehints;
    unsigned long flags;
    int x;
    int y;
    int w;
    int h;
    int border;
    int wmin;
    int hmin;
    XConfigureRequestEvent *configure;
    XResizeRequestEvent *resize;
//    int fixedw = 0;
//    int fixedh = 0;

    if (!client->id) {

        return;
    }
    frame = client->parent;
    if (event->xany.type == ConfigureRequest) {
        Rwm_print_configurerequest(event);
        configure = &event->xconfigurerequest;
        flags = configure->value_mask;
        if (flags & CWX) {
            x = configure->x;
        } else if (frame) {
            x = frame->x;
        } else {
            x = client->x;
        }
        if (flags & CWY) {
            y = configure->y;
        } else if (frame) {
            y = frame->y;
        } else {
            y = client->y;
        }
        if (flags & CWWidth) {
            w = configure->width;
        } else {
            w = client->w;
        }
        if (flags & CWHeight) {
            h = configure->height;
        } else {
            h = client->h;
        }
        if ((flags & CWSibling)
            && (flags & CWStackMode)) {
            client->above = configure->above;
        }
        if (flags & CWBorderWidth) {
            client->border = configure->border_width;
#if 0
            XSetWindowBorderWidth(client->app->display,
                                  client->id,
                                  border);
#endif
        }
    } else {
        Rwm_print_resizerequest(event);
        resize = &event->xresizerequest;
        if (frame) {
            x = frame->x;
        } else {
            x = client->x;
        }
        if (frame) {
            y = frame->y;
        } else {
            y = client->y;
        }
        w = resize->width;
        h = resize->height;
    }
    sizehints = client->sizehints;
#if 0
    if (!sizehints && (client->typeflags)) {
        Rwm_get_size_hints(client);
    }
#endif
    if (sizehints) {
        flags = sizehints->flags;
        if ((flags & USPosition)
            || ((client->typeflags & R_WINDOW_TRANSIENT_FLAG)
                && (flags & PPosition))) {
            x = sizehints->x;
            y = sizehints->y;
        }
        if (flags & PBaseSize) {
            wmin = sizehints->base_width;
            hmin = sizehints->base_height;
        } else {
            wmin = sizehints->min_width;
            hmin = sizehints->min_height;
        }
        if ((flags & PMinSize)
            && (sizehints->flags & PMaxSize)) {
            if (sizehints->min_width == sizehints->max_width) {
                w = sizehints->min_width;
            } else {
                w = max2(w, wmin);
            }
            if (sizehints->min_height == sizehints->max_height) {
                h = sizehints->min_height;
            } else {
                h = max2(h, hmin);
            }
        } else {
            w = max2(w, wmin);
            h = max2(h, hmin);
        }
    }
    client->x = x;
    client->y = y;
    client->w = w;
    client->h = h;

    Rwm_send_configure(client);

    return;
}

void
Rwm_set_borderless(struct R_window *client, unsigned long decor)
{
    struct R_wm     *wm;
    struct R_window *frame;

    frame = client->parent;
    if (decor) {
        if (frame) {
            Rwm_reparent_window(client,
                                frame,
                                frame->left,
                                frame->top);
            R_map_window(frame);
        }
        client->stateflags &= ~R_WINDOW_BORDERLESS_FLAG;
    } else {
        if (frame) {
            wm = R_global.app->client;
            Rwm_reparent_window(client,
                                wm->desktop,
                                client->x,
                                client->y);
            R_unmap_window(frame);
        }
        client->stateflags |= R_WINDOW_BORDERLESS_FLAG;
    }

    return;
}

void
Rwm_get_wm_hints(struct R_window *window)
{
    XWMHints *wmhints;

    if (!window->id) {

        return;
    }
    wmhints = window->wmhints;
    if (wmhints) {
        XFree(wmhints);
        window->wmhints = NULL;
    }
    window->wmhints = XGetWMHints(window->app->display,
                                  window->id);
    XSync(window->app->display,
          False);
    _ignbadwindow = 0;

    return;
}

void
Rwm_get_wm_protocols(struct R_window *window)
{
    Atom *atoms;
    Atom *aptr;
    Atom *awmptr;
    int cnt;
    int ndx;
    unsigned long flags;

    if (!window->id) {

        return;
    }
    flags = 0;
    _ignbadwindow = 1;
    if (XGetWMProtocols(window->app->display,
                        window->id,
                        &atoms,
                        &cnt)) {
        aptr = atoms;
        while (cnt--) {
            awmptr = _wmprotos;
            for (ndx = 0 ; ndx < RWM_PROTOS; ndx++) {
                if ((*awmptr) && *aptr == *awmptr) {
                    flags |= (1UL << ndx);
                }
                *awmptr++;
            }
            aptr++;
        }
        window->protoflags = flags;
        XFree(atoms);
    }
    XSync(window->app->display,
          False);
    _ignbadwindow = 0;

    return;
}

void
Rwm_get_motif_wm_hints(struct R_window *client)
{
    Atom                     realtype;
    int                      format;
    unsigned long            n;
    unsigned long            extra;
    struct R_motif_wm_hints *mwmhints;

    if (!client->id) {

        return;
    }
    _ignbadwindow = 1;
    XGetWindowProperty(client->app->display,
                       client->id,
                       _wmatoms[RWM_MOTIF_WM_HINTS_ATOM],
                       0L,
                       5L,
                       False,
                       _wmatoms[RWM_MOTIF_WM_HINTS_ATOM],
                       &realtype,
                       &format,
                       &n,
                       &extra,
                       (unsigned char **)&client->motifwmhints);
    XSync(client->app->display,
          False);
    if ((n) == 5) {
        fprintf(stderr, "CHANGING MOTIF WM HINTS\n");
        mwmhints = client->motifwmhints;
        if (mwmhints->flags & R_MOTIF_DECOR_FLAG) {
            Rwm_set_borderless(client, mwmhints->decor);
        }
    }
    _ignbadwindow = 0;
    if (n == 0) {
        XFree(client->motifwmhints);
        client->motifwmhints = NULL;
    }

    return;
}

void
Rwm_set_input_focus(struct R_window *client, Time timestamp)
{
    XWMHints *wmhints;
    XWindowAttributes attr;
    int try = 0;

    if (!client->id) {

        return;
    }
    wmhints = client->wmhints;
    _ignbadmatch = 1;
    _ignbadwindow = 1;
    if ((wmhints) && (wmhints->flags & InputHint)) {
        if (wmhints->input) {
            XSetInputFocus(client->app->display,
                           client->id,
                           RevertToPointerRoot,
                           timestamp);
            _focus = client;
        } else {
            Rwm_send_proto(client,
                           RWM_TAKE_FOCUS_PROTO,
                           timestamp);
        }
    } else if (client->typeflags
               & (R_WINDOW_OVERRIDE_FLAG
                  | R_WINDOW_TOPLEVEL_FLAG
                  | R_WINDOW_TRANSIENT_FLAG)) {
        try = 1;
    }
    if (try) {
        memset(&attr, 0, sizeof(attr));
        if (XGetWindowAttributes(client->app->display,
                                 client->id,
                                 &attr)) {
            if (attr.class != InputOnly) {
                client->stateflags |= R_WINDOW_FOCUSED_FLAG;
                XSetInputFocus(client->app->display,
                               client->id,
                               RevertToPointerRoot,
                               timestamp);
                _focus = client;
            }
        }
    }
    if (!wmhints || (!(wmhints->flags & InputHint) && !(wmhints->input))) {
        memset(&attr, 0, sizeof(attr));
        XGetWindowAttributes(client->app->display,
                             client->id,
                             &attr);
        if (attr.class != InputOnly) {
            client->stateflags |= R_WINDOW_FOCUSED_FLAG;
            _ignbadwindow = 1;
            XSetInputFocus(client->app->display,
                           client->id,
                           RevertToPointerRoot,
                           timestamp);
            _focus = client;
        }
    }
    XSync(client->app->display,
          False);
    _ignbadwindow = 0;
    _ignbadmatch = 0;
    
    return;
}

