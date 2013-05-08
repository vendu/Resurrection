/*
 * init.c - Resurrection X11 window manager initialization.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#include <signal.h>

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
extern struct R_color _wmcolorconfs[];
#endif

#if 0
void sig_alrm(int sig);
#endif

void Rwm_load_title_font(struct R_app *app);

pid_t           *Rwmdeskpids;
struct R_window *Rwmdesktopwin;
pid_t            _Rwmlaunchpid;
Imlib_Font       _titlefont;
Imlib_Font       _menufont;

const char *_wmatomstrs[] = {
    "WM_PROTOCOLS",
//    "_MOTIF_WM_HINTS",
    "_NET_WM_NAME",
    "_NET_WM_ICON_NAME",
    "_NET_WM_USER_TIME",
    NULL
};

const char *_wmprotostrs[] = {
    "WM_DELETE_WINDOW",
    "WM_TAKE_FOCUS",
    NULL
};

void
Rwm_exit(void)
{
    pid_t pid;
    int   stat;
    
    errno = 0;
#if 0
    if (_Rwmlaunchpid) {
        kill(_Rwmlaunchpid, SIGTERM);
    }
#endif
    /* wait for children */
    do {
        pid = wait(&stat);
        if (pid < 0) {
            if (errno == EINTR) {
                errno = 0;
            } else {

                break;
            }
        }
    } while (1);
    XCloseDisplay(R_global.app->display);

    exit(0);
}

void
Rwm_exit_handler(int sig)
{
    exit(sig);
}

void
Rwm_crash_handler(int sig)
{
    SIGNAL(sig, SIG_DFL);

//    XCloseDisplay(R_global.app->display);

    abort();
}

#if 0
void
sig_alrm(int sig)
{
    exit(1);
}
#endif

void
Rwm_usr1_handler(int sig)
{
    fprintf(stderr, "USR1: %lx\n", (long)Rwmdesktopwin);
    R_map_window_raised(Rwmdesktopwin);
}

void
Rwm_init_optflags(struct R_wm *wm)
{
    wm->optflags
        |= RWM_FOCUS_STYLE
        | RWM_MOVE_STYLE
        | RWM_RESIZE_STYLE
        | RWM_MENU_STYLE
        | RWM_CLOCK_STYLE
        | RWM_DEFAULT_FLAGS;

    return;
}

void
Rwm_init_atoms(struct R_wm *wm)
{
    struct R_app *app;
    const char **str;
    Atom *aptr;

    _ignbadatom = 1;
    app = R_global.app;
    str = _wmatomstrs;
    aptr = _wmatoms;
    while (*str) {
        *aptr = XInternAtom(app->display,
                            *str,
                            False);
        str++;
        aptr++;
    }
    str = _wmprotostrs;
    aptr = _wmprotos;
    while (*str) {
        *aptr = XInternAtom(app->display,
                            *str,
                            False);
        str++;
        aptr++;
    }
    XSync(app->display,
          False);
    _ignbadatom = 0;

    return;
}

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
#   define RWM_SET_COLOR(sp, dp) (*(uint32_t *)(dp) = bswap32(*(uint32_t *)(sp)))
#else
#   define RWM_SET_COLOR(sp, dp) (*(uint32_t *)(dp) = *(uint32_t *)(sp))
#endif

void
Rwm_init_colors(struct R_wm *wm)
{
    RWM_SET_COLOR(&_wmcolorconfs[0], &_wmcolors[0]);
}

int
Rwm_init(struct R_app *app,
         int argc,
         char **argv)
{
    struct R_app    *newapp = NULL;
    struct R_window *root;
    struct R_wm     *wm;
    int              loop = 1;
    int              nscreen = 0;
    int              i;
    int              defscreen;
    int              xfd;
    pid_t            pid;
    fd_set           readfds;

#if (RWM_EXEC_RL)
    pid = fork();
    if (!pid) {
        char *arg[2] = { "Rl", NULL };

        execvp("Rl", arg);
#if 0
    } else {
        _Rwmlaunchpid = pid;
#endif
    }
#endif
    app->wintree = calloc(256, sizeof(void *));
    if (!app->wintree) {
        
        return FALSE;
    }
    nscreen = R_init_display_screen(app,
                                    argc,
                                    argv,
                                    0);
    if (!nscreen) {
        
                return FALSE;
    }
    XCloseDisplay(app->display);
    fprintf(stderr, "%d screens\n", nscreen);
    defscreen = DefaultScreen(app->display);
    R_global.app = app;
    for (i = 0 ; i < nscreen ; i++) {
        pid = fork();
        if (pid) {
//            Rwmdeskpids[i] = pid;
            fprintf(stderr, "SCRPID: %d\n", (int)pid);
        } else {
            newapp = calloc(1, sizeof(struct R_app));
            newapp->wintree = app->wintree;
            if (!newapp->wintree) {
                
                return FALSE;
            }
            if (!R_init_screen(newapp,
                               argc,
                               argv,
                               i)) {
                
                return FALSE;
            }
//
            wm = calloc(1, sizeof(struct R_wm));
            newapp->client = wm;
#if 0
            if (i == defscreen) {
                
                continue;
            }
#endif
#if 0
            signal(SIGALRM, sig_alrm);
            alarm(15);
#endif
            newapp->flags = RWM_DEFAULT_FLAGS;
            atexit(Rwm_exit);
            
            SIGNAL(SIGINT, Rwm_exit_handler);
            SIGNAL(SIGTERM, Rwm_exit_handler);
            SIGNAL(SIGQUIT, Rwm_exit_handler);
            SIGNAL(SIGILL, Rwm_crash_handler);
#if defined(SIGBUS)
            SIGNAL(SIGBUS, Rwm_crash_handler);
#endif
            SIGNAL(SIGFPE, Rwm_crash_handler);
            SIGNAL(SIGSEGV, Rwm_crash_handler);
#if defined(SIGSYS)
            SIGNAL(SIGSYS, Rwm_crash_handler);
#endif
            Rwm_init_optflags(wm);
            Rwm_init_colors(wm);
#if (RWM_DEBUG_X_ERRORS)
            XSynchronize(newapp->display, True);
#endif
            XSetErrorHandler(R_handle_x_error);
            Rwm_init_atoms(wm);
#if (USE_IMLIB2)
            Rwm_load_title_font(newapp);
#endif
            newapp->screen = i;
//            fprintf(stderr, "initialising screen %d\n", i);
            if (!Rwm_set_root_window(newapp)) {
                
                return FALSE;
            }
            
            if (!Rwm_init_desktops(newapp)) {
                
                return FALSE;
            }
            wm->desktop = wm->desktops[0];
            Rwmdesktopwin = wm->desktop;
            SIGNAL(SIGUSR1, Rwm_usr1_handler);
            
            gettimeofday(&Rwm_action_info.tv, NULL);
            Rwm_init_frame_event_handlers(newapp);
#if (USE_IMLIB2)
            if (!Rwm_init_frame_images(newapp)) {
                
                return FALSE;
            }
#endif
            
#if (!R_DEBUG_WM)
            if (!Rwm_take_windows(newapp)) {
                
                return FALSE;
            }
#endif
            
            Rwm_init_root_events(newapp);
            if (!Rwm_init_bindings(newapp)) {
                
                return FALSE;
            }

#if (!RWM_EXEC_RL)           
            if (!Rwm_init_menu(newapp)) {
                
                return FALSE;
            }
            Rwm_init_menu_events(newapp);
#endif
            
            if (!Rwm_init_pager(newapp)) {
                
                return FALSE;
            }
            Rwm_init_pager_events(newapp);
            if (!Rwm_init_frame_cursors(newapp)) {
                
                return FALSE;
            }
            if (wm->optflags
                & (RWM_DESKTOP_CLOCK_FLAG | RWM_FOCUS_CLOCK_FLAG)) {
                if (!Rwm_init_clock(newapp)) {
                    
                    return FALSE;
                }
            }
            
#if (R_DEBUG_WM)
            Rwm_init_test(newapp);
#endif
            xfd = XConnectionNumber(newapp->display);
            FD_SET(xfd, &readfds);
            while (TRUE) {
                R_handle_events(newapp);
#if 0
                if (_updateclock) {
                    Rwm_update_clock(&_clock);
                    _updateclock = 0;
                }
#endif
                Rselect(xfd + 1, &readfds, NULL, NULL, NULL);
            }
        }
    }
    app->screen = defscreen;

    return TRUE;
}

#if (USE_IMLIB2)
void
Rwm_load_title_font(struct R_app *app)
{
#if 0
    _titlefont = R_load_font_imlib2("VeraMono/12");
    _menufont = R_load_font_imlib2("VeraMono/16");
#endif
    _titlefont = R_load_font_imlib2("VeraMono/10");
    _menufont = R_load_font_imlib2("bladerunner/10");
    if (!_titlefont) {

        exit(1);
    }

    return;
}
#endif

int
Rwm_take_windows(struct R_app *app)
{
    struct R_wm *wm;
    struct R_window *root;
    int screen;
    int x;
    int y;
    int w;
    int h;
    unsigned int nchildren;
    unsigned int ui;
    Screen *screenptr;
    Window *children;
    Window rootwin;
    Window win;
    struct R_window *window;
    struct R_window *frame;
    XWindowAttributes attr;

    wm = app->client;
    root = app->window;
    screen = app->screen;
    screenptr = ScreenOfDisplay(app->display,
                                screen);
    w = root->w;
    h = root->h;
    rootwin = app->winid;
    if (XQueryTree(app->display,
                   rootwin,
                   &rootwin,
                   &win,
                   &children,
                   &nchildren)) {
        if (nchildren) {
            ui = 0;
            _ignbadwindow = 1;
            _ignbadmatch = 1;
            while (ui < nchildren) {
                win = children[ui];
                memset(&attr, 0, sizeof(attr));
                XGetWindowAttributes(app->display,
                                     win,
                                     &attr);
                x = attr.x;
                if (!attr.override_redirect) {
#if (RWM_EXEC_RL)
                    y = max(attr.y, RL_BUTTON_HEIGHT + RWM_MENU_ITEM_HEIGHT);
#else
                    y = max(attr.y, RWM_MENU_ITEM_HEIGHT);
#endif
                }
                w = attr.width;
                h = attr.height;
                window = R_find_window(win);
                if (!window) {
                    window = R_alloc_window();
                    if (window) {
                        window->app = app;
                        window->id = win;
                        window->x = x;
                        window->y = y;
                        window->w = w;
                        window->h = h;
                        window->border = attr.border_width;
                        if (!attr.override_redirect) {
                            R_add_save_window(window);
                            window->sysflags |= R_WINDOW_STATIC_IMAGES_FLAG;
                            if (attr.save_under) {
                                window->sysflags |= R_WINDOW_SAVE_UNDER_FLAG;
                            }
                            if (attr.backing_store) {
                                window->sysflags |= R_WINDOW_BACK_STORE_FLAG;
                            }
                            XGetTransientForHint(app->display,
                                                 window->id,
                                                 &win);
                            if (win) {
                                window->typeflags = R_WINDOW_TRANSIENT_FLAG;
                            } else {
                                window->typeflags = R_WINDOW_TOPLEVEL_FLAG;
                            }
                            XSetWindowBorderWidth(window->app->display,
                                                  window->id,
                                                  window->border);
                            frame = Rwm_frame_window(window);
                            Rwm_reparent_window(frame,
                                                wm->desktop,
                                                window->x,
                                                window->y);
                            R_map_window_raised(frame);
                            Rwm_init_frame_events(frame);
                            Rwm_get_title(window);
                            Rwm_draw_text(frame->chain);
                            R_add_window(window);
                            Rwm_init_client_events(window);
                        } else {
                            window->typeflags = R_WINDOW_OVERRIDE_FLAG;
                            Rwm_reparent_window(window,
//                                                wm->desktop,
                                                app->window,
                                                window->x,
                                                window->y);
                            R_add_window(window);
                            R_map_window_raised(window);
                            R_set_window_events(window,
                                                NoEventMask);
                            R_free_window(window);
                        }
                    }
                    XSync(app->display,
                          False);
                }
                ui++;
            }
            _ignbadwindow = 0;
            _ignbadmatch = 0;
            XFree(children);
        }
    }

    return TRUE;
}

