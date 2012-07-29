/*
 * binding.c - Resurrection window manager key and button bindings.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

//#define RWM_CONFIGURE_DELAY_USEC 50000
#define RWM_CONFIGURE_DELAY_USEC 0

XSizeHints Rwm_sizehints;

static struct Rwm_configure_info Rwm_client_info;
struct Rwm_configure_info Rwm_action_info;

#include <Resurrection/Resurrection.h>

int
Rwm_init_bindings(struct R_app *app)
{
//    struct R_window *root;
    struct R_wm *wm;
    struct R_window *desktop;
    struct R_binding *binding;
    struct R_binding *desktopbinding;
    int i;
    
    wm = R_global.app->client;
    for (i = 0 ; i < RWM_DESKTOPS ; i++) {
        desktop = wm->desktops[i];
        desktopbinding = &Rwm_desktop_bindings[0];
        while (desktopbinding->mask) {
            binding = R_create_binding(desktop,
                                       desktopbinding->mask,
                                       desktopbinding->func,
                                       desktopbinding->arg);
            if (binding == NULL) {
                
                return FALSE;
            }
            if (!R_add_binding(binding,
                               desktop)) {
                
                return FALSE;
            }
            desktopbinding++;
        }
    }

    return TRUE;
}

void
Rwm_switch_desktop_func(void *arg,
                        XEvent *event,
                        void *bindarg)
{
    struct R_app *app;
    struct R_wm *wm;
    struct Rwm_binding_arg *bindingarg;
    struct R_window *window;
    struct R_window *desktop;
    int id;

    window = arg;
    app = window->app;
    wm = app->client;
    bindingarg = bindarg;
    id = bindingarg->val;
    desktop = wm->desktops[id];
    Rwm_switch_desktop(app,
                       desktop);

    return;
}

void
Rwm_exec_func(void *arg,
              XEvent *event,
              void *bindarg)
{
    struct Rwm_binding_arg *bindingarg;
    pid_t pid;
    char *argv[2];

    bindingarg = bindarg;
    if (bindingarg->val == RWM_EXIT) {

        exit(0);
    }
    argv[0] = bindingarg->cmd;
    argv[1] = NULL;
    pid = fork();
    if (!pid) {
        execvp(bindingarg->cmd,
               argv);
    }

    return;
}

void
Rwm_exit_func(void *arg,
              XEvent *event,
              void *bindarg)
{
    struct R_window *window;

    window = arg;
    XCloseDisplay(window->app->display);

    exit(0);
}

void
Rwm_shade_func(void *arg,
               XEvent *event,
               void *bindarg)
{
    struct R_window *frame;
    struct R_window *window;
    int winw;
    int winh;

    window = arg;
    frame = window->parent;
    if (frame->app->flags & RWM_CLICK_RAISE_FLAG) {
        R_raise_window(frame);
    }
    winw = frame->w;
    if (frame->stateflags & R_WINDOW_SHADED_FLAG) {
        winh = frame->top + window->child->h + frame->bottom;
        frame->stateflags &= ~R_WINDOW_SHADED_FLAG;
    } else {
        winh = frame->top;
        frame->stateflags |= R_WINDOW_SHADED_FLAG;
    }
    Rwm_move_resize_window(frame,
                           frame->x,
                           frame->y,
                           winw,
                           winh);

    return;
}

/* TODO: implement this */
void
Rwm_kill_func(void *arg,
               XEvent *event,
               void *bindarg)
{
    struct R_window *window;
    struct R_window *frame;
    struct R_window *client;

    fprintf(stderr, "KILL_WINDOW\n");
    window = arg;

    return;
}

void
Rwm_close_func(void *arg,
              XEvent *event,
              void *bindarg)
{
    struct R_window *window;
    struct R_window *frame;
    struct R_window *client;

    fprintf(stderr, "CLOSE_WINDOW\n");
    window = arg;
    Rwm_delete_window(window->parent->child);

    return;
}

void
Rwm_start_move_func(void *arg,
                    XEvent *event,
                    void *bindarg)
{
    struct R_wm *wm;
    struct R_window *frame;
    struct R_window *window;
    int dummyi;
    unsigned int dummyui;
    Window dummywin;

    window = arg;
    wm = window->app->client;
    frame = window->parent;
    if (!(window->stateflags & R_WINDOW_DESTROYED_FLAG)) {
        Rwm_action_info.x = frame->x;
        Rwm_action_info.y = frame->y;
        if (!(window->stateflags & R_WINDOW_MOVE_FLAG)) {
            if (frame->app->flags & RWM_CLICK_RAISE_FLAG) {
                R_raise_window(frame);
            }
            XGrabPointer(window->app->display,
                         window->id,
                         False,
                         ButtonReleaseMask
                         | (bindarg == (void *)2
                            ? Button2MotionMask
                            : Button1MotionMask)
                         | PointerMotionHintMask,
                         GrabModeAsync,
                         GrabModeAsync,
                         wm->desktop->id,
                         None,
                         event->xbutton.time);
            XQueryPointer(window->app->display,
                          window->id,
                          &dummywin,
                          &dummywin,
                          &dummyi,
                          &dummyi,
                          &Rwm_client_info.x,
                          &Rwm_client_info.y,
                          &dummyui);
            window->stateflags |= R_WINDOW_MOVE_FLAG;
            gettimeofday(&Rwm_action_info.tv, NULL);
        }
    }

    return;
}

void
Rwm_move_func(void *arg,
              XEvent *event,
              void *bindarg)
{
    unsigned long    tval;
    struct R_wm *wm;
    struct R_window *client;
    struct R_window *frame;
    struct R_window *window;
    int deskx;
    int desky;
    int winx;
    int winy;
    unsigned int dummyui;
    Screen *screenptr;
    Window dummywin;
    struct timeval curtv;

    window = arg;
    wm = window->app->client;
    if (wm->optflags & RWM_ASYNCHRONOUS_MOVE_FLAG) {
        gettimeofday(&curtv, NULL);
        tval = TIMEVAL_CMP_USEC(&Rwm_action_info.tv, &curtv);
        if (tval < RWM_CONFIGURE_DELAY_USEC) {
            usleep(tval);

            return;
        }
    }
    frame = window->parent;
    screenptr = ScreenOfDisplay(window->app->display,
                                DefaultScreen(window->app->display));
    XQueryPointer(window->app->display,
                  window->id,
                  &dummywin,
                  &dummywin,
                  &deskx,
                  &desky,
                  &winx,
                  &winy,
                  &dummyui);
    winx += Rwm_action_info.x - Rwm_client_info.x;
#if 0
    winx = min2(winx,
                wm->desktop->w - frame->w);
    winx = max2(winx,
                0);
#endif
    winy += Rwm_action_info.y - Rwm_client_info.y;
#if 0
    winy = min2(winy,
                wm->desktop->h - frame->h);
#endif
#if 0
    winy = max2(winy,
                RWM_MENU_ITEM_HEIGHT);
#endif
    client = window->child;
    Rwm_move_window(frame,
                    winx,
                    winy);
    Rwm_action_info.x = winx;
    Rwm_action_info.y = winy;
    Rwm_send_configure(client);
    Rwm_action_info.tv = curtv;

    return;
}

void
Rwm_finish_move_func(void *arg,
                     XEvent *event,
                     void *bindarg)
{
    struct R_window *window;
    struct R_window *frame;
    struct R_window *client;

    window = arg;
    frame = window->parent;
    client = window->child;
    window->stateflags &= ~R_WINDOW_MOVE_FLAG;
    XUngrabPointer(window->app->display,
                   event->xbutton.time);
#if 0
    Rwm_move_window(frame,
                    frame->x,
                    frame->y);
#endif
    Rwm_send_configure(client);

    return;
}

void
Rwm_start_resize_func(void *arg,
                       XEvent *event,
                       void *bindarg)
{
    struct R_wm *wm;
    struct R_window *client;
    struct R_window *frame;
    struct R_window *window;
    long dummyl;
    int dummyi;
    unsigned int dummyui;
    Window dummywin;

    window = arg;
    wm = window->app->client;
    client = window->child;
    frame = window->parent;
    if (!((window->stateflags)
          & (R_WINDOW_DESTROYED_FLAG | R_WINDOW_RESIZE_FLAG))) {
        Rwm_client_info.x = client->x;
        Rwm_client_info.y = client->y;
        Rwm_client_info.w = client->w;
        Rwm_client_info.h = client->h;
        memset(&Rwm_sizehints, 0, sizeof(Rwm_sizehints));
        if (!XGetWMNormalHints(window->app->display,
                               window->id,
                               &Rwm_sizehints,
                               &dummyl)) {
            memset(&Rwm_sizehints, 0, sizeof(Rwm_sizehints));
        }
        XGrabPointer(window->app->display,
                     window->id,
                     False,
                     ButtonReleaseMask
                     | (bindarg == (void *)2
                        ? Button2MotionMask
                        : Button1MotionMask)
                     | PointerMotionHintMask,
                     GrabModeAsync,
                     GrabModeAsync,
                     wm->desktop->id,
                     None,
                     event->xbutton.time);
        XQueryPointer(window->app->display,
                      wm->desktop->id,
                      &dummywin,
                      &dummywin,
                      &dummyi,
                      &dummyi,
                      &Rwm_action_info.x,
                      &Rwm_action_info.y,
                      &dummyui);
        gettimeofday(&Rwm_action_info.tv, NULL);
        window->stateflags |= R_WINDOW_RESIZE_FLAG;
    }

    return;
}

void
Rwm_finish_resize_func(void *arg,
                       XEvent *event,
                       void *bindarg)
{
    struct R_window *window;
    struct R_window *frame;
    struct R_window *client;

    window = arg;
    frame = window->parent;
    client = window->child;
    window->stateflags &= ~R_WINDOW_RESIZE_FLAG;
    XUngrabPointer(window->app->display,
                   event->xbutton.time);
#if 0
    Rwm_move_resize_client(client,
                           frame->x,
                           frame->y,
                           client->w,
                           client->h);
#endif
    client->x = Rwm_client_info.x;
    client->y = Rwm_client_info.y;
    client->w = Rwm_client_info.w;
    client->h = Rwm_client_info.h;
    Rwm_send_configure(client);

    return;
}

void
Rwm_resizev_func(void *arg,
                 XEvent *event,
                 void *bindarg)
{
    struct R_wm *wm;
    struct R_window *client;
    struct R_window *frame;
    struct R_window *window;
    int deskx;
    int desky;
    int winx;
    int winy;
    int winw;
    int winh;
    int border;
    int hinc;
    unsigned int dummyui;
    Window dummywin;
    struct timeval curtv;

    window = arg;
    wm = window->app->client;
    if (wm->optflags & RWM_ASYNCHRONOUS_RESIZE_FLAG) {
        gettimeofday(&curtv, NULL);
        if (TIMEVAL_CMP_USEC(&Rwm_action_info.tv, &curtv)
            < RWM_CONFIGURE_DELAY_USEC) {

            return;
        }
    }
    frame = window->parent;
    client = window->child;
    border = client->border;
    XQueryPointer(window->app->display,
                  wm->desktop->id,
                  &dummywin,
                  &dummywin,
                  &deskx,
                  &desky,
                  &winx,
                  &winy,
                  &dummyui);
    hinc = Rwm_sizehints.height_inc;
    winw = Rwm_client_info.w;
    if (hinc) {
        winh = Rwm_client_info.h + ((winy - Rwm_action_info.y) / hinc) * hinc;
    } else {
        winh = Rwm_client_info.h + winy - Rwm_action_info.y;
    }
    if (client->h != winh) {
        Rwm_resize_client(client,
                          winw,
                          winh);
        Rwm_action_info.x = winx;
        Rwm_action_info.y = winy;
        Rwm_client_info.w = winw;
        Rwm_client_info.h = winh;
    }
    Rwm_send_configure(client);
    Rwm_action_info.tv = curtv;
    XSync(client->app->display,
          False);

    return;
}

void
Rwm_resizeh_func(void *arg,
                 XEvent *event,
                 void *bindarg)
{
    struct R_wm *wm;
    struct R_window *client;
    struct R_window *frame;
    struct R_window *window;
    int deskx;
    int desky;
    int winx;
    int winy;
    int winw;
    int winh;
    int winc;
    unsigned int dummyui;
    Window dummywin;
    struct timeval curtv;

    window = arg;
    wm = window->app->client;
    if (wm->optflags & RWM_ASYNCHRONOUS_RESIZE_FLAG) {
        gettimeofday(&curtv, NULL);
        if (TIMEVAL_CMP_USEC(&Rwm_action_info.tv, &curtv)
            < RWM_CONFIGURE_DELAY_USEC) {

            return;
        }
    }
    frame = window->parent;
    client = window->child;
    XQueryPointer(window->app->display,
                  wm->desktop->id,
                  &dummywin,
                  &dummywin,
                  &deskx,
                  &desky,
                  &winx,
                  &winy,
                  &dummyui);
    winc = Rwm_sizehints.width_inc;
    winh = Rwm_client_info.h;
    if (winc) {
        winw = Rwm_client_info.w + ((winx - Rwm_action_info.x) / winc) * winc;
    } else {
        winw = Rwm_client_info.w + winx - Rwm_action_info.x;
    }
    if (client->w != winw) {
        Rwm_resize_client(client,
                          winw,
                          winh);
        Rwm_action_info.x = winx;
        Rwm_action_info.y = winy;
        Rwm_client_info.w = winw;
        Rwm_client_info.h = winh;
    }
    Rwm_send_configure(client);
    Rwm_action_info.tv = curtv;
    XSync(client->app->display,
          False);

    return;
}

