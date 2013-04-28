/*
 * frame.c - Resurrection window manager reparent-frame routines.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#define RWM_NEW_FRAME_BINDINGS 1

#define RWM_STATIC_FRAME_PIXMAPS 0

R_eventhandler_t *Rwm_frame_event_handlers[LASTEvent];
R_eventhandler_t *Rwm_subframe_event_handlers[RWM_FRAME_WINDOWS][LASTEvent];

void Rwm_frame_buttonpress_handler(void *arg,
                                   XEvent *event);
#if 0
void Rwm_frame_enternotify_handler(void *arg,
                                   XEvent *event);
void Rwm_subframe_enternotify_handler(void *arg,
                                      XEvent *event);
#endif
#if 0
void Rwm_frame_motionnotify_handler(void *arg,
                                   XEvent *event);
#endif
void Rwm_subframe_buttonpress_handler(void *arg,
                                      XEvent *event);
void Rwm_subframe_buttonrelease_handler(void *arg,
                                        XEvent *event);
#if 0
void Rwm_subframe_enternotify_handler(void *arg,
                                      XEvent *event);
#endif

struct Rwm_dim {
    int w;
    int h;
};

struct Rwm_border {
    int top;
    int bottom;
    int left;
    int right;
};

#if (RWM_OLDE_THEME)
static char *Rwm_frame_image_names[RWM_FRAME_SUBWINDOWS][R_WINDOW_STATES] = {
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/menu.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/menu.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/menu.png",
        NULL,
        NULL
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/hruler.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/hruler.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/hruler.png",
        NULL,
        NULL
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/vruler.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/vruler.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/vruler.png",
        NULL,
        NULL
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/vruler.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/vruler.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/vruler.png",
        NULL,
        NULL
    },
};
#elif (RWM_NEW_THEME)
static char *Rwm_frame_image_names[RWM_FRAME_SUBWINDOWS][R_WINDOW_STATES] = {
    {
        RESURRECTION_IMAGE_SEARCH_PATH "wm/bordergold_horiz.png",
        RESURRECTION_IMAGE_SEARCH_PATH "wm/bordergold_horiz.png",
        RESURRECTION_IMAGE_SEARCH_PATH "wm/bordergold_horiz.png",
        NULL,
        NULL
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "wm/bordergold_horiz.png",
        RESURRECTION_IMAGE_SEARCH_PATH "wm/bordergold_horiz.png",
        RESURRECTION_IMAGE_SEARCH_PATH "wm/bordergold_horiz.png",
        NULL,
        NULL
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "wm/bordergold_vert.png",
        RESURRECTION_IMAGE_SEARCH_PATH "wm/bordergold_vert.png",
        RESURRECTION_IMAGE_SEARCH_PATH "wm/bordergold_vert.png",
        NULL,
        NULL
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "wm/bordergold_vert.png",
        RESURRECTION_IMAGE_SEARCH_PATH "wm/bordergold_vert.png",
        RESURRECTION_IMAGE_SEARCH_PATH "wm/bordergold_vert.png",
        NULL,
        NULL
    },
};
#elif (RWM_ORIGINAL_THEME)
static char *Rwm_frame_image_names[RWM_FRAME_SUBWINDOWS][R_WINDOW_STATES] = {
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winh.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winselh.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winclickh.png",
        NULL,
        NULL
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winh.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winselh.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winclickh.png",
        NULL,
        NULL
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winv.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winselv.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winclickv.png",
        NULL,
        NULL
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winv.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winselv.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winclickv.png",
        NULL,
        NULL
    },
};
#else
static char *Rwm_frame_image_names[RWM_FRAME_SUBWINDOWS][R_WINDOW_STATES] = {
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_norm_h_title.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_over_h_title.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_click_h_title.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_focus_h_title.png",
        NULL
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_norm_h_bottom.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_over_h_bottom.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_click_h_bottom.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_focus_h_bottom.png",
        NULL
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_norm_v.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_over_v.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_click_v.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_focus_v.png",
        NULL
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_norm_v.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_over_v.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_click_v.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/win_focus_v.png",
        NULL
    },
};
#endif

#if (USE_IMLIB2)
struct R_image Rwm_frame_images[RWM_FRAME_SUBWINDOWS][R_WINDOW_STATES];
#endif

static struct Rwm_dim Rwm_frame_dims[RWM_FRAME_SUBWINDOWS] = {
    { R_FIT_IMAGE, 16 },
    { R_FIT_IMAGE, 4 },
    { 4, R_FIT_IMAGE },
    { 4, R_FIT_IMAGE }
};

#if (RWM_ORIGINAL_THEME)
static struct Rwm_border Rwm_frame_borders[RWM_FRAME_SUBWINDOWS] = {
    { 2, 2, 2, 2 },
    { 2, 2, 2, 2 },
    { 2, 2, 2, 2 },
    { 2, 2, 2, 2 }
};
#else
static struct Rwm_border Rwm_frame_borders[RWM_FRAME_SUBWINDOWS] = {
    { 2, 2, 4, 4 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};
#endif

static Cursor Rwm_frame_cursors[RWM_FRAME_SUBWINDOWS];

/* top */
#define RWM_TOP_START_MOVE  0
#define RWM_TOP_MOVE        1
#define RWM_TOP_FINISH_MOVE 2
#define RWM_TOP_SHADE       3
#define RWM_TOP_CLOSE       4
#define RWM_TOP_KILL        5
#define RWM_TOP_ACTIONS     9
struct R_binding Rwm_top_bindings[RWM_TOP_ACTIONS] = {
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON1_FLAG),
        Rwm_start_move_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_MOTION_FLAG
                       | R_BINDING_BUTTON1_FLAG),
        Rwm_move_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_RELEASE_FLAG
                       | R_BINDING_BUTTON1_FLAG),
        Rwm_finish_move_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON2_FLAG),
        Rwm_start_move_func,
        (void *)2,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_MOTION_FLAG
                       | R_BINDING_BUTTON2_FLAG),
        Rwm_move_func,
        (void *)2,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_RELEASE_FLAG
                       | R_BINDING_BUTTON2_FLAG),
        Rwm_finish_move_func,
        (void *)2,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON1_FLAG
                       | R_BINDING_CTRL_FLAG),
        Rwm_shade_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON1_FLAG
                       | R_BINDING_SHIFT_FLAG),
        Rwm_close_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON3_FLAG
                       | R_BINDING_SHIFT_FLAG),
        Rwm_kill_func,
        NULL,
        NULL
    },
};

/* bottom */
#define RWM_BOTTOM_START_RESIZEV  0
#define RWM_BOTTOM_RESIZEV        1
#define RWM_BOTTOM_FINISH_RESIZEV 2
#define RWM_BOTTOM_START_MOVE     3
#define RWM_BOTTOM_MOVE           4
#define RWM_BOTTOM_FINISH_MOVE    5
#define RWM_BOTTOM_ACTIONS        6
struct R_binding Rwm_bottom_bindings[RWM_BOTTOM_ACTIONS] = {
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON1_FLAG),
        Rwm_start_resize_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_MOTION_FLAG
                       | R_BINDING_BUTTON1_FLAG),
        Rwm_resizev_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_RELEASE_FLAG
                       | R_BINDING_BUTTON1_FLAG),
        Rwm_finish_resize_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON2_FLAG),
        Rwm_start_move_func,
        (void *)2,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_MOTION_FLAG
                       | R_BINDING_BUTTON2_FLAG),
        Rwm_move_func,
        (void *)2,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_RELEASE_FLAG
                       | R_BINDING_BUTTON2_FLAG),
        Rwm_finish_move_func,
        (void *)2,
        NULL
    }
};

/* left */
#define RWM_LEFT_START_MOVE  0
#define RWM_LEFT_MOVE        1
#define RWM_LEFT_FINISH_MOVE 2
#define RWM_LEFT_ACTIONS     3
struct R_binding Rwm_left_bindings[RWM_LEFT_ACTIONS] = {
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON1_FLAG),
        Rwm_start_move_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_MOTION_FLAG
                       | R_BINDING_BUTTON1_FLAG),
        Rwm_move_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_RELEASE_FLAG
                       | R_BINDING_BUTTON1_FLAG),
        Rwm_finish_move_func,
        NULL,
        NULL
    }
};

/* right */
#define RWM_RIGHT_START_RESIZEV  0
#define RWM_RIGHT_RESIZEH        1
#define RWM_RIGHT_FINISH_RESIZEH 2
#define RWM_RIGHT_START_MOVE     3
#define RWM_RIGHT_MOVE           4
#define RWM_RIGHT_FINISH_MOVE    5
#define RWM_RIGHT_ACTIONS        6
struct R_binding Rwm_right_bindings[RWM_RIGHT_ACTIONS] = {
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON1_FLAG),
        Rwm_start_resize_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_MOTION_FLAG
                       | R_BINDING_BUTTON1_FLAG),
        Rwm_resizeh_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_RELEASE_FLAG
                       | R_BINDING_BUTTON1_FLAG),
        Rwm_finish_resize_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON2_FLAG),
        Rwm_start_move_func,
        (void *)2,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_MOTION_FLAG
                       | R_BINDING_BUTTON2_FLAG),
        Rwm_move_func,
        (void *)2,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_RELEASE_FLAG
                       | R_BINDING_BUTTON2_FLAG),
        Rwm_finish_move_func,
        (void *)2,
        NULL
    }
};

void
Rwm_init_frame_event_handlers(struct R_app *app)
{
    R_eventhandler_t **funcpp;
    int                i;

    funcpp = Rwm_frame_event_handlers;
    funcpp[ButtonPress] = Rwm_title_buttonpress_handler;
//    funcpp[EnterNotify] = Rwm_frame_enternotify_handler;
//    funcpp[LeaveNotify] = Rwm_frame_leavenotify_handler;
//    funcpp[MotionNotify] = Rwm_frame_motionnotify_handler;
    funcpp[Expose] = Rwm_title_expose_handler;
    for (i = 0 ; i < RWM_FRAME_SUBWINDOWS ; i++) {
        funcpp = Rwm_subframe_event_handlers[i];
        funcpp[ButtonPress] = Rwm_subframe_buttonpress_handler;
        funcpp[ButtonRelease] = Rwm_subframe_buttonrelease_handler;
//        funcpp[EnterNotify] = Rwm_subframe_enternotify_handler;
//        funcpp[LeaveNotify] = Rwm_subframe_leavenotify_handler;
//        funcpp[MotionNotify] = Rwm_frame_motionnotify_handler;
    }

    return;
}

void
Rwm_init_frame_events(struct R_window *frame)
{
    struct R_window   *window;
    struct R_window   *client;
    long               mask;
    int i;

    /* frame */
    frame->eventhandlers = Rwm_frame_event_handlers;
    frame->nevents = LASTEvent;
    mask = R_add_window_events(frame,
                               SubstructureNotifyMask
                               | SubstructureRedirectMask);
    client = frame->child;
#if 0
    if (client->typeflags & (R_WINDOW_TRANSIENT_FLAG | R_WINDOW_TOPLEVEL_FLAG)) {
        mask = R_add_window_events(client,
                                   ButtonPressMask
                                   | ExposureMask
                                   | ButtonReleaseMask
                                   | OwnerGrabButtonMask);
    }
#endif
    /* top */
    window = frame->chain;
    for (i = 0 ; i < RWM_TOP_ACTIONS ; i++) {
        R_add_binding(&Rwm_top_bindings[i],
                      window);
    }
    window->font = _titlefont;
    window->eventhandlers = Rwm_subframe_event_handlers[RWM_TOP_FRAME];
    window->nevents = LASTEvent;
    mask = R_add_window_events(window,
                               ButtonPressMask
                               | ExposureMask
                               | ButtonReleaseMask
                               | OwnerGrabButtonMask);
    
    /* bottom */
    window = window->chain;
    for (i = 0 ; i < RWM_BOTTOM_ACTIONS ; i++) {
        R_add_binding(&Rwm_bottom_bindings[i],
                      window);
    }
    window->eventhandlers = Rwm_subframe_event_handlers[RWM_BOTTOM_FRAME];
    window->nevents = LASTEvent;
    mask = R_add_window_events(window,
                               ButtonPressMask
                               | ButtonReleaseMask
                               | OwnerGrabButtonMask
                               | Button1MotionMask
                               | Button3MotionMask
                               | PointerMotionMask);

    /* left */
    window = window->chain;
    for (i = 0 ; i < RWM_LEFT_ACTIONS ; i++) {
        R_add_binding(&Rwm_left_bindings[i],
                      window);
    }
    window->eventhandlers = Rwm_subframe_event_handlers[RWM_LEFT_FRAME];
    window->nevents = LASTEvent;
    mask = R_add_window_events(window,
                               ButtonPressMask
                               | ButtonReleaseMask
                               | OwnerGrabButtonMask
                               | Button1MotionMask
                               | PointerMotionMask);

    /* right */
    window = window->chain;
    for (i = 0 ; i < RWM_RIGHT_ACTIONS ; i++) {
        R_add_binding(&Rwm_right_bindings[i],
                      window);
    }
    window->eventhandlers = Rwm_subframe_event_handlers[RWM_RIGHT_FRAME];
    window->nevents = LASTEvent;
    mask = R_add_window_events(window,
                               ButtonPressMask
                               | ButtonReleaseMask
                               | OwnerGrabButtonMask
                               | Button1MotionMask
                               | Button3MotionMask
                               | PointerMotionMask);

    return;
}

#if (USE_IMLIB2)
int
Rwm_init_frame_images(struct R_app *app)
{
    int i;
    int j;
#if (RWM_STATIC_FRAME_PIXMAPS)
    struct R_wm *wm = app->client;
#endif

    for (i = 0 ; i < RWM_FRAME_SUBWINDOWS ; i++) {
        for (j = 0 ; j < R_WINDOW_STATES ; j++) {
            if (Rwm_frame_image_names[i][j]) {
                if (!R_load_image_imlib2(app,
                                         Rwm_frame_image_names[i][j],
                                         &Rwm_frame_images[i][j])) {
                    
                    return FALSE;
                }
                R_set_image_border_imlib2(&Rwm_frame_images[i][j],
                                          Rwm_frame_borders[i].top,
                                          Rwm_frame_borders[i].bottom,
                                          Rwm_frame_borders[i].left,
                                          Rwm_frame_borders[i].right);
                Rwm_frame_images[i][j].w = Rwm_frame_dims[i].w;
                Rwm_frame_images[i][j].h = Rwm_frame_dims[i].h;
                Rwm_frame_images[i][j].flags |= R_IMAGE_STATIC_FLAG;
#if (RWM_STATIC_FRAME_PIXMAPS)
                R_render_image_imlib2(&Rwm_frame_images[i][j],
                                      wm->desktop,
                                      wm->desktop->w,
                                      wm->desktop->h,
                                      0);
#endif
            }
        }
    }

    return TRUE;
}
#endif

int
Rwm_init_frame_cursors(struct R_app *app)
{
    Rwm_frame_cursors[RWM_BOTTOM_FRAME] = XCreateFontCursor(app->display,
                                                            XC_bottom_side);
    Rwm_frame_cursors[RWM_RIGHT_FRAME] = XCreateFontCursor(app->display,
                                                           XC_right_side);

    return TRUE;
}

struct R_window *
Rwm_frame_window(struct R_window *window)
{
    struct R_window *frame;
    unsigned long sysflags;
    unsigned long typeflags;
    Window win;

//    window->sysflags |= R_WINDOW_STATIC_IMAGES_FLAG;
    frame = Rwm_create_frame(window->app,
                             window);
    if (frame) {
        frame->child = window;
        Rwm_init_frame(frame);
    }
    R_add_window(frame);

    return frame;
}

struct R_window *
Rwm_create_frame(struct R_app *app,
                 struct R_window *client)
{
    struct R_wm *wm;
    struct R_window *frame;
    struct R_window *window;
    struct R_window *tmp;
    unsigned long flags;
    int i;

    wm = app->client;
    flags = R_WINDOW_STATIC_EVENTS_FLAG;
    if (client->app->flags & RWM_SAVE_UNDER_FLAG) {
        flags |= R_WINDOW_SAVE_UNDER_FLAG;
    }
    /* allocate _all_ window structures */
    frame = R_create_window(app,
                            wm->desktop,
                            flags | R_WINDOW_OVERRIDE_REDIRECT_FLAG);
    frame->typeflags |= R_WINDOW_FRAME_FLAG;
    if (frame) {
        frame->x = client->x;
        frame->y = client->y;
        /* chain window structures */
        window = frame;
        i = RWM_FRAME_SUBWINDOWS;
        while (i--) {
            tmp = R_create_window(app, frame, flags);
            tmp->typeflags = R_WINDOW_FRAME_FLAG;
            window->chain = tmp;
            window = tmp;
            window->app = app;
//            window->child = client;
            window->parent = frame;
        }
        client->parent = frame;
        frame->child = client;
        if (client->typeflags & R_WINDOW_TOPLEVEL_FLAG) {
            frame->top = Rwm_frame_dims[RWM_TOP_FRAME].h;
        } else if (client->typeflags & R_WINDOW_TRANSIENT_FLAG) {
            frame->top = Rwm_frame_dims[RWM_BOTTOM_FRAME].h;
        }
        frame->top = Rwm_frame_dims[RWM_TOP_FRAME].h;
        frame->bottom = Rwm_frame_dims[RWM_BOTTOM_FRAME].h;
        frame->left = Rwm_frame_dims[RWM_LEFT_FRAME].w;
        frame->right = Rwm_frame_dims[RWM_RIGHT_FRAME].w;
    }

    return frame;
}

int
Rwm_init_frame(struct R_window *frame)
{
    struct R_app *app;
    struct R_wm *wm;
    struct R_window *client;
    struct R_window *window;
#if (USE_IMLIB2)
    struct R_image *image;
#endif
    int x;
    int y;
    int w;
    int h;
    int border;
    int i;
    XWindowAttributes attr;

    frame->typeflags |= R_WINDOW_FRAME_FLAG;
    client = frame->child;
    if (!client) {

        return;
    }
    app = client->app;
    wm = app->client;
    if (!wm) {

        return;
    }
    border = client->border;
    memset(&attr, 0, sizeof(attr));
    frame = client->parent;
    _ignbadwindow = 1;
    XGetWindowAttributes(client->app->display,
                         client->id,
                         &attr);
    XSync(client->app->display,
          False);
    _ignbadwindow = 0;
#if 0
    client->w = min2(client->w,
                     wm->desktop->w - frame->left - frame->right);
    client->h = min2(client->h,
                     wm->desktop->h - frame->top - frame->bottom - RWM_MENU_ITEM_WIDTH);
#endif
    /* frame */
    frame->typeflags = R_WINDOW_FRAME_FLAG;
#if 0
    frame->x = client->x;
    frame->y = max2(client->y, RWM_MENU_ITEM_HEIGHT);
#endif
    frame->x = attr.x;
#if (RWM_EXEC_RL)
    frame->y = max2(attr.y, RL_BUTTON_HEIGHT + RWM_MENU_ITEM_HEIGHT);
#else
    frame->y = max2(attr.y, RWM_MENU_ITEM_HEIGHT);
#endif
    frame->w = frame->left + client->w + frame->right;
    frame->h = frame->top + client->h + frame->bottom;
    Rwm_move_resize_window(frame,
                           frame->x,
                           frame->y,
                           frame->w,
                           frame->h);

    /* top */
    x = 0;
    y = 0;
    window = frame->chain;
//    window->typeflags = R_WINDOW_FRAME_FLAG;
    window->parent = frame;
    w = frame->w;
    h = frame->top;
#if (USE_IMLIB2)
    for (i = 0 ; i < R_WINDOW_STATES ; i++) {
        image = &Rwm_frame_images[RWM_TOP_FRAME][i];
        window->images[i] = image;
    }
    RWM_SET_WINDOW_IMAGE(window,
                         R_WINDOW_NORMAL_STATE);
#endif
    Rwm_move_resize_window(window,
                           x,
                           y,
                           w,
                           h);
#if (USE_IMLIB2)
#if (!RWM_STATIC_FRAME_PIXMAPS)
    R_render_image_imlib2(window->image,
                          window,
                          window->image->w,
                          window->image->h,
                          0);
#endif
    window->str = client->str;
    window->font = _titlefont;
    R_set_background_imlib2(window->image,
                            window,
                            window->image->w,
                            window->image->h,
                            0);
#endif
    window->child = client;
    R_add_window(window);

    /* bottom */
    window = window->chain;
//    window->typeflags = R_WINDOW_FRAME_FLAG;
    window->parent = frame;
    w = frame->w;
    h = frame->bottom;
    y = frame->top + client->h;
    XDefineCursor(window->app->display,
                  window->id,
                  Rwm_frame_cursors[RWM_BOTTOM_FRAME]);
#if (USE_IMLIB2)
    for (i = 0 ; i < R_WINDOW_STATES ; i++) {
        image = &Rwm_frame_images[RWM_BOTTOM_FRAME][i];
        window->images[i] = image;
    }
    RWM_SET_WINDOW_IMAGE(window,
                         R_WINDOW_NORMAL_STATE);
#endif
    Rwm_move_resize_window(window,
                           x,
                           y,
                           w,
                           h);
#if (USE_IMLIB2)
#if (!RWM_STATIC_FRAME_PIXMAPS)
    R_render_image_imlib2(window->image,
                          window,
                          window->image->w,
                          window->image->h,
                          0);
#endif
    R_set_background_imlib2(window->image,
                            window,
                            window->image->w,
                            window->image->h,
                            0);
#endif
    window->child = client;
    R_add_window(window);

    /* left */
    window = window->chain;
//    window->typeflags = R_WINDOW_FRAME_FLAG;
    window->parent = frame;
    x = 0;
#if (RWM_ORIGINAL_THEME)
    y = frame->top;
#else
    y = 0;
#endif
    w = frame->left;
#if (RWM_ORIGINAL_THEME)
    h = client->h;
#else
    h = client->h + frame->top;
#endif
#if (USE_IMLIB2)
    for (i = 0 ; i < R_WINDOW_STATES ; i++) {
        image = &Rwm_frame_images[RWM_LEFT_FRAME][i];
        window->images[i] = image;
    }
    RWM_SET_WINDOW_IMAGE(window,
                         R_WINDOW_NORMAL_STATE);
#endif
    Rwm_move_resize_window(window,
                           x,
                           y,
                           w,
                           h);
#if (USE_IMLIB2)
#if (!RWM_STATIC_FRAME_PIXMAPS)
    R_render_image_imlib2(window->image,
                          window,
                          window->image->w,
                          window->image->h,
                          0);
#endif
    R_set_background_imlib2(window->image,
                            window,
                            window->image->w,
                            window->image->h,
                            0);
#endif
    window->child = client;
    R_add_window(window);

    /* right */
    window = window->chain;
//    window->typeflags = R_WINDOW_FRAME_FLAG;
    window->parent = frame;
    x = client->w + frame->left;
    w = frame->right;
    XDefineCursor(window->app->display,
                  window->id,
                  Rwm_frame_cursors[RWM_RIGHT_FRAME]);
#if (USE_IMLIB2)
    for (i = 0 ; i < R_WINDOW_STATES ; i++) {
        image = &Rwm_frame_images[RWM_RIGHT_FRAME][i];
        window->images[i] = image;
    }
    RWM_SET_WINDOW_IMAGE(window,
                         R_WINDOW_NORMAL_STATE);
#endif
    Rwm_move_resize_window(window,
                           x,
                           y,
                           w,
                           h);
#if (USE_IMLIB2)
#endif
#if (!RMW_STATIC_FRAME_PIXMAPS)
    R_render_image_imlib2(window->image,
                          window,
                          window->image->w,
                          window->image->h,
                          0);
#endif
    R_set_background_imlib2(window->image,
                            window,
                            window->image->w,
                            window->image->h,
                            0);
    window->child = client;
    R_add_window(window);
    _ignbadwindow = 1;
    Rwm_reparent_window(client,
                        frame,
                        frame->left,
                        frame->top);
    Rwm_reparent_window(frame,
                        wm->desktop,
                        attr.x,
                        attr.y);
    R_map_subwindows(frame);
    XSync(client->app->display, False);
    _ignbadwindow = 0;

    return 0;
}

void
Rwm_free_frame(struct R_window *window)
{
    struct R_window *curwindow;
    struct R_window *nextwindow;
    int i;

    if (window) {
        R_add_window_events(window,
                            NoEventMask);
        curwindow = window->chain;
        while (curwindow) {
            nextwindow = curwindow->chain;
            R_add_window_events(curwindow,
                                NoEventMask);
//            R_remove_window(curwindow);
            XDestroyWindow(curwindow->app->display,
                           curwindow->id);
            R_free_window(curwindow);
            curwindow = nextwindow;
        }
        XDestroyWindow(window->app->display,
                       window->id);
        R_free_window(window);
    }

    return;
}

void
Rwm_title_buttonpress_handler(void *arg,
                              XEvent *event)
{
    struct R_window *frame;

    frame = arg;
    if (frame->child) {
        XUngrabPointer(frame->app->display,
                       event->xbutton.time);
        R_set_input_focus(frame->child,
                          event->xbutton.time);
        XSync(frame->app->display, False);
    }

    return;
}

void
Rwm_title_expose_handler(void *arg,
                         XEvent *event)
{
    struct R_window *title;

    title = arg;
    Rwm_draw_text(title);

    return;
}

void
Rwm_subframe_buttonpress_handler(void *arg,
                                 XEvent *event)
{
    struct R_window *subframe;
    struct R_window *client;
    struct R_window *frame;
    struct R_wm     *wm;

    wm = R_global.app->client;
    subframe = arg;
    client = subframe->child;
    frame = subframe->parent;
    if (wm->optflags & RWM_CLICK_RAISE_FLAG) {
        if (event->xbutton.button == 3) {
            R_lower_window(frame);
        } else if (event->xbutton.button == 1) {
            R_raise_window(frame);
        }
    }
    if ((client->typeflags & (R_WINDOW_TRANSIENT_FLAG | R_WINDOW_TOPLEVEL_FLAG)) & ((client) && subframe == frame->chain)) {
        /* top/title. */
        XUngrabPointer(frame->app->display,
                       event->xcrossing.time);
        R_set_input_focus(subframe->child, event->xbutton.time);
    }
    subframe->stateflags |= R_WINDOW_CLICKED_FLAG;
#if (USE_IMLIB2)
#if 0
    RWM_SET_WINDOW_IMAGE(subframe,
                         R_WINDOW_CLICKED_STATE);
#endif
    if (subframe->str) {
        Rwm_draw_text(subframe);
    } else {
        R_set_background_imlib2(subframe->image,
                                subframe,
                                subframe->image->w,
                                subframe->image->h,
                                0);
    }
#endif

    return;
}

void
Rwm_subframe_buttonrelease_handler(void *arg,
                                   XEvent *event)
{
    struct R_window *subframe;

    subframe = arg;
    subframe->stateflags &= ~R_WINDOW_CLICKED_FLAG;
#if (USE_IMLIB2)
#if 0
    RWM_SET_WINDOW_IMAGE(subframe,
                         R_WINDOW_ACTIVE_STATE);
#endif
    if (subframe->str) {
        Rwm_draw_text(subframe);
    } else {
        R_set_background_imlib2(subframe->image,
                                subframe,
                                subframe->image->w,
                                subframe->image->h,
                                0);
    }
#endif

    return;
}

void
Rwm_frame_enternotify_handler(void *arg,
                              XEvent *event)
{
    struct R_window *window;
    struct R_window *frame;
    struct R_window *client;
    struct R_wm     *wm;

    fprintf(stderr, "FRAME\n");
    frame = arg;
    client = frame->child;
    wm = R_global.app->client;
    if (wm->optflags & (RWM_SLOPPY_FOCUS_FLAG | RWM_POINTER_FOCUS_FLAG)) {
        client->stateflags |= R_WINDOW_ACTIVE_FLAG | R_WINDOW_FOCUSED_FLAG;;
        XUngrabPointer(client->app->display,
                       event->xcrossing.time);
        R_set_input_focus(client, event->xcrossing.time);
        XSync(client->app->display, False);
    }
#if 0
    frame = arg;
    client = frame->child;
    wm = R_global.app->client;
    if (wm->optflags & (RWM_SLOPPY_FOCUS_FLAG | RWM_POINTER_FOCUS_FLAG)) {
        client->stateflags |= R_WINDOW_ACTIVE_FLAG | R_WINDOW_FOCUSED_FLAG;;
        XUngrabPointer(frame->app->display,
                       event->xcrossing.time);
        Rwm_set_input_focus(client, event->xcrossing.time);
    }
#endif
}

#if 0
void
Rwm_frame_motionnotify_handler(void *arg,
                              XEvent *event)
{
    struct R_window *frame;
    struct R_window *client;
    struct R_wm     *wm;

    frame = arg;
    client = frame->child;
    if (_focus == client) {
#if 0
        R_set_window_events(frame,
                            ButtonPressMask
                            | ButtonReleaseMask
                            | EnterWindowMask
                            | LeaveWindowMask);
#endif

        return;
    }
    wm = R_global.app->client;
    if (wm->optflags & (RWM_SLOPPY_FOCUS_FLAG | RWM_POINTER_FOCUS_FLAG)) {
        client->stateflags |= R_WINDOW_ACTIVE_FLAG | R_WINDOW_FOCUSED_FLAG;;
        XUngrabPointer(frame->app->display,
                       event->xcrossing.time);
        R_set_input_focus(client, event->xcrossing.time);
    }
}
#endif

void
Rwm_subframe_enternotify_handler(void *arg,
                                 XEvent *event)
{
    struct R_window *client;
    struct R_window *frame;
    struct R_window *subframe;
    struct R_wm     *wm;
    
    subframe = arg;
    client = subframe->child;
    frame = client->parent;
    subframe->stateflags &= ~R_WINDOW_ACTIVE_FLAG;
#if (USE_IMLIB2)
    RWM_SET_WINDOW_IMAGE(subframe,
                         R_WINDOW_NORMAL_STATE);
    if (subframe->str) {
        Rwm_draw_text(subframe);
    } else {
        R_set_background_imlib2(subframe->image,
                                subframe,
                                subframe->image->w,
                                subframe->image->h,
                                0);
    }
#endif
    wm = R_global.app->client;
    if ((frame) && (frame->desktop)
        && (wm->optflags & (RWM_SLOPPY_FOCUS_FLAG | RWM_POINTER_FOCUS_FLAG))) {
        client->stateflags &= ~R_WINDOW_FOCUSED_FLAG;
        XUngrabPointer(frame->app->display,
                       event->xcrossing.time);
        R_set_input_focus(frame->desktop, event->xcrossing.time);
    }

    return;
}

