/*
 * init.c - Rl initialization.
 * Copyright (C) 2011 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>
#include <X11/extensions/shape.h>
#if (RL_TERM_TABS)
#include <pthread.h>
#endif

struct R_window * Rl_init_windows(struct R_app *app);
void Rl_init_parent_events(struct R_window *window);
void Rl_init_button_events(struct R_window *window);
void Rl_button_visibilitynotify_handler(void *arg, XEvent *event);
void Rl_button_expose_handler(void *arg, XEvent *event);
void Rl_button_enternotify_handler(void *arg, XEvent *event);
void Rl_button_leavenotify_handler(void *arg, XEvent *event);
void Rl_button_buttonpress_handler(void *arg, XEvent *event);
void Rl_parent_enternotify_handler(void *arg, XEvent *event);
void Rl_parent_leavenotify_handler(void *arg, XEvent *event);
void Rl_terminal_button_expose_handler(void *arg, XEvent *event);
void Rl_terminal_button_enternotify_handler(void *arg, XEvent *event);
void Rl_terminal_button_leavenotify_handler(void *arg, XEvent *event);

struct Rl_image {
    struct R_image *normal;
    struct R_image *active;
    struct R_image *clicked;
    Pixmap          npmap;
    Pixmap          nmask;
    Pixmap          apmap;
    Pixmap          amask;
    Pixmap          cpmap;
    Pixmap          cmask;
};

#if (RL_MENU)
static long             labelndx;
static long             stkndx;
static long             stkbot;
#endif
#if (RL_LABEL)
static struct Rl_image  labelimg;
#endif
static struct Rl_image  buttonimgs[RL_COMMANDS];
static struct R_window *tooltips[RL_COMMANDS];

//static pthread_mutex_t  termtabmtx;

#if (NEW_TOOLTIPS)
static const char *Rl_tooltips[RL_COMMANDS]
= {
    RL_TERM_COMMAND_1 " / " RL_TERM_COMMAND_2 " / " RL_TERM_COMMAND_3,
    RL_EDIT_COMMAND_1 " / " RL_EDIT_COMMAND_2 " / " RL_EDIT_COMMAND_3,
    RL_WEB_COMMAND_1 " / " RL_WEB_COMMAND_2 " / " RL_WEB_COMMAND_3,
    RL_AUDIO_COMMAND_1 " / " RL_AUDIO_COMMAND_2 " / " RL_AUDIO_COMMAND_3,
    RL_VIDEO_COMMAND_1 " / " RL_VIDEO_COMMAND_2 " / " RL_VIDEO_COMMAND_3,
    RL_GRAPHICS_COMMAND_1,
    RL_RESTART_COMMAND_1, // " / " RL_RESTART_COMMAND_2 " / " RL_RESTART_COMMAND_3,
    "exit"
};
#else
static const char *Rl_tooltips[RL_COMMANDS]
= {
    "terminal",
    "editor",
    "browser",
#if (RL_OLDE_THEME)
    "audio"
#else
    "audio",
    "graphics"
#endif
};
#endif

#define RL_TERM_COMMANDS { RL_TERM_COMMAND_1, RL_TERM_COMMAND_2, RL_TERM_COMMAND_3 }
#define RL_EDIT_COMMANDS { RL_EDIT_COMMAND_1, RL_EDIT_COMMAND_2, RL_EDIT_COMMAND_3 }
#define RL_WEB_COMMANDS { RL_WEB_COMMAND_1, RL_WEB_COMMAND_2, RL_WEB_COMMAND_3 }
#define RL_AUDIO_COMMANDS { RL_AUDIO_COMMAND_1, RL_AUDIO_COMMAND_2, RL_AUDIO_COMMAND_3 }
#define RL_VIDEO_COMMANDS { RL_VIDEO_COMMAND_1, RL_VIDEO_COMMAND_2, RL_VIDEO_COMMAND_3 }
#define RL_GRAPHICS_COMMANDS { RL_GRAPHICS_COMMAND_1, RL_GRAPHICS_COMMAND_2, RL_GRAPHICS_COMMAND_3 }
#define RL_RESTART_COMMANDS { RL_RESTART_COMMAND_1, RL_RESTART_COMMAND_2, RL_RESTART_COMMAND_3 }
#define RL_EXIT_COMMANDS { RL_EXIT_COMMAND, RL_EXIT_COMMAND, RL_EXIT_COMMAND }

static const char *Rl_commands[RL_COMMANDS][RL_BUTTONS]
= {
#if (RL_OLDE_THEME)
    RL_TERM_COMMANDS, RL_EDIT_COMMANDS, RL_WEB_COMMANDS, RL_AUDIO_COMMANDS
#else
    RL_TERM_COMMANDS, RL_EDIT_COMMANDS, RL_WEB_COMMANDS, RL_AUDIO_COMMANDS, RL_VIDEO_COMMANDS, RL_GRAPHICS_COMMANDS, RL_RESTART_COMMANDS, RL_EXIT_COMMANDS
#endif
};

#if (RL_OLDE_THEME)
static const char *Rl_imagenames[RL_SUBWINDOWS][RL_BUTTON_STATES]
= {
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/q1.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/q2.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/q2.png",
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/icon_monitor_1.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/icon_monitor_2.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/icon_monitor_3.png",
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/icon_floppy_1.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/icon_floppy_2.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/icon_floppy_3.png",
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/icon_N_1.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/icon_N_2.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/icon_N_3.png",
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/icon_beethoven_1.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/icon_beethoven_2.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/icon_beethoven_3.png",
    }
};
#else
static const char *Rl_imagenames[RL_SUBWINDOWS][RL_BUTTON_STATES]
= {
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winh.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winh.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/winh.png",
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_terminal_normal.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_terminal_hilited.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_terminal_clicked.png",
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_edit_normal.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_edit_hilited.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_edit_clicked.png",
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_web_normal.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_web_hilited.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_web_clicked.png",
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_music_normal.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_music_hilited.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_music_clicked.png",
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_imageview_normal.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_imageview_hilited.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_imageview_clicked.png",
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_paint_normal.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_paint_hilited.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_exec_paint_clicked.png",
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_action_restart_normal.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_action_restart_hilited.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_action_restart_clicked.png",
    },
    {
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_action_exit_normal.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_action_exit_hilited.png",
        RESURRECTION_IMAGE_SEARCH_PATH "widget/btn_action_exit_clicked.png",
    }
};
#endif

#if (RL_OLDE_THEME)
//Region clipreg;
//GC     clipgc;
Pixmap shapebitmap;
int    hasshape;
#endif

#if (RL_TERM_TABS)
struct R_window *tabwins[RL_COMMANDS];
struct R_window *Rltermbuttons[RL_MAX_TABS];
pid_t            Rltermpids[RL_MAX_TABS];
int              ipcpipes[RL_MAX_TABS][2];
volatile long  Rltermcnt;
pid_t Rlcurterm;
pthread_mutex_t termmtx;
#endif

static struct Rl_image Rl_images[RL_COMMANDS];

int Rl_init(struct R_app *app,
            int argc,
            char **argv);

#if (RL_TERM_TABS)
void
Rl_exit(void)
{
    int i;
    pid_t pid;
    int waitstat;

    signal(SIGCHLD, SIG_DFL);
    for (i = 0 ; i < RL_TERM_TABS ; i++) {
        pid = Rltermpids[i];
        if (pid) {
            fprintf(stderr, "TERM -> %d\n", (int)pid);
            kill(pid, SIGTERM);
            pid = waitpid(pid, &waitstat, 0);
            fprintf(stderr, "WAIT: %d\n", (int)pid);
#if 0
            pthread_mutex_lock(&termmtx);
            Rl_remove_terminal_tab(child);
            pthread_mutex_unlock(&termmtx);
#endif
        }
    }

    exit(0);
}

RETSIGTYPE
Rl_child_signal(int sig)
{
    int waitstat;
    pid_t child;

    child = waitpid(-1, &waitstat, 0);
    fprintf(stderr, "WAIT: %d\n", child);
    pthread_mutex_lock(&termmtx);
    Rl_remove_terminal_tab(child);
    pthread_mutex_unlock(&termmtx);
}

void
Rl_remove_terminal_tab(pid_t pid)
{
    struct R_window *button;
    int i;
    int j;

//    pthread_mutex_lock(&termtabmtx);
//    R_unmap_window(tabwins[0]);
    for (i = 0 ; i < RL_MAX_TABS ; i++) {
        if (Rltermpids[i] == pid) {
            Rltermcnt--;
            if (Rltermcnt) {
                R_resize_window(tabwins[0],
                                Rltermcnt * RL_BUTTON_WIDTH,
                                RL_BUTTON_HEIGHT);
            } else {
                R_unmap_window(tabwins[0]);
            }
            button = Rltermbuttons[i];
            R_set_window_events(button, NoEventMask);
            R_remove_window(button);
            R_unmap_window(button);
            R_free_window(button);
            for (j = i ; j <= Rltermcnt ; j++) {
                Rltermpids[j] = Rltermpids[j + 1];
                button = Rltermbuttons[j] = Rltermbuttons[j + 1];
                if (button) {
                    button->num = j;
                    R_move_window(button,
                                  j * RL_BUTTON_WIDTH,
                                  0);
                }
            }
            Rltermpids[j] = 0;
            Rltermbuttons[j] = NULL;
            R_move_window(tooltips[0],
                          (Rltermcnt + 1) * RL_BUTTON_WIDTH,
                          0);
            for (j = 0 ; j < Rltermcnt ; j++) {
                button = Rltermbuttons[j];
                R_set_thumb_imlib2(buttonimgs[0].normal, button,
                                   RL_BUTTON_WIDTH, RL_BUTTON_HEIGHT,
                                   0);
            }
        }
#if 0
        if (Rltermcnt) {
            R_map_window(tabwins[0]);
        }
#endif

        return;
    }
//    pthread_mutex_unlock(&termtabmtx);

    return;
}
#endif

int
Rl_loop(struct R_app *app)
{
    while (TRUE) {
        R_handle_next_event(app);
    }

    return 0;
}

int
Rl_init(struct R_app *app,
        int argc,
        char **argv)
{
    struct R_window *window;
    int dummyi;
#if (RL_EXEC_RTERM)
    pid_t pid;
#endif

#if (RL_EXEC_RTERM)
    pid = fork();
    if (!pid) {
        char geomstr[32];
        char *arg[6] = { "Rterm", "-B", "-E", "-g", geomstr, NULL };

        snprintf(geomstr, 32, "80x3+%d+0", RL_BUTTON_WIDTH);

        execvp("Rterm", arg);
    }
#endif
    
#if (RL_TERM_TABS)
    atexit(Rl_exit);
    SIGNAL(SIGCHLD, Rl_child_signal);
#endif

    if (!R_init(app,
                argc,
                argv)) {

        return FALSE;
    }
    imlib_set_cache_size(8 * 1024 * 1024);
    XSynchronize(app->display, True);
#if (RL_OLDE_THEME)
    if (XShapeQueryExtension(app->display,
                             &dummyi, &dummyi)) {
        hasshape = 1;
    }
#endif
    Rl_init_windows(app);
#if (RL_OLDE_THEME)
    if (hasshape) {
        shapebitmap = XCreatePixmap(app->display,
                                    app->window->id,
                                    RL_BUTTON_WIDTH,
                                    RL_LABEL_HEIGHT + RL_COMMANDS * RL_BUTTON_HEIGHT,
                                    1);
    }
#endif
    Rl_map_windows(app);
    return (Rl_loop(app));
}

#if (RL_TOOLTIPS)
struct R_window *
Rl_init_tooltip(struct R_app *app, int i)
{
    struct R_window *window;
    struct R_image  *image;
    Imlib_Image      textimg;

    window = R_create_window(app,
                             NULL,
                             R_WINDOW_OVERRIDE_REDIRECT_FLAG);
#if (RL_HORIZONTAL)
    R_move_resize_window(window,
                         i * RL_BUTTON_WIDTH,
                         RL_BUTTON_HEIGHT,
                         RL_TOOLTIP_WIDTH,
                         RL_TOOLTIP_HEIGHT);
#else
    R_move_resize_window(window,
                         RL_BUTTON_WIDTH,
                         i * RL_BUTTON_HEIGHT,
                         RL_TOOLTIP_WIDTH,
                         RL_TOOLTIP_HEIGHT);
#endif
    image = R_load_image_imlib2(app,
                                Rl_imagenames[0][RL_BUTTON_NORMAL],
                                NULL);
    image->flags |= R_IMAGE_STATIC_FLAG;
    R_render_image_imlib2(image, window, RL_TOOLTIP_WIDTH, RL_TOOLTIP_HEIGHT, 0);
    imlib_context_set_drawable(image->pixmap);
    textimg = imlib_create_image(RL_TOOLTIP_WIDTH, RL_TOOLTIP_HEIGHT);
    imlib_context_set_image(textimg);
    imlib_context_set_mask(0);
    imlib_image_clear();
    imlib_context_set_blend(1);
    imlib_image_set_has_alpha(1);
    imlib_context_set_color(0x00, 0xff, 0x00, 0xff);
    imlib_text_draw(0, 0, Rl_tooltips[i]);
#if (RL_SMALL)
    imlib_render_image_on_drawable(8, 4);
#else
    imlib_render_image_on_drawable(8, 13);
#endif
    imlib_free_image();
    R_set_background_imlib2(image,
                            window,
                            RL_TOOLTIP_WIDTH,
                            RL_TOOLTIP_HEIGHT,
                            0);
    R_free_image_imlib2(image);

    return window;
}
#endif

struct R_window *
Rl_init_windows(struct R_app *app)
{
    struct R_window *parent;
#if (RL_LABEL)
    struct R_window *label;
#endif
#if (RL_TOOLTIPS)
    struct R_window *tooltip;
    struct R_image  *tooltipimg;
#endif
    struct R_window *button;
    struct R_window *ruler;
#if (RL_TERM_TABS)
    struct R_window *tabwin;
#endif
    struct R_image  *image;
    struct R_window *curwindow;
    unsigned long    flags;
    Imlib_Image      img;
    const char      *string = "apps";
    int textw, texth;
    Imlib_Font       im2font;
    Imlib_Image      textimg;
    int i;

    flags = R_WINDOW_OVERRIDE_REDIRECT_FLAG;
    parent = R_create_window(app,
                             NULL,
                             flags);
    if (parent == NULL) {

        return FALSE;
    }
    R_add_window(parent);
    app->window = parent;
    Rl_init_parent_events(parent);

#if (RL_LABEL)
    image = R_load_image_imlib2(app,
                                Rl_imagenames[0][RL_BUTTON_NORMAL],
                                NULL);
    image->flags |= R_IMAGE_STATIC_FLAG;
    label = R_create_window(app,
                            parent,
                            flags);
    R_render_thumb_imlib2(image,
                          label,
                          RL_LABEL_WIDTH,
                          RL_LABEL_HEIGHT,
                          0);
    R_set_thumb_imlib2(image,
                       label,
                       RL_LABEL_WIDTH,
                       RL_LABEL_HEIGHT,
                       0);
    parent->chain = label;
//    R_add_window(label);
    R_map_window_raised(label);
#if (RLRULERS)
    R_move_resize_window(label,
                         0,
                         0,
                         RL_LABEL_WIDTH,
                         RL_LABEL_HEIGHT);
#else
    R_move_resize_window(label,
                         RL_RULER_HEIGHT,
                         RL_RULER_WIDTH,
                         RL_LABEL_WIDTH,
                         RL_LABEL_HEIGHT);
#endif
#endif
#if (RL_LABEL || RL_TOOLTIPS)
    imlib_context_set_drawable(parent->id);
#if (RL_SMALL)
    im2font = imlib_load_font("VeraMono/10");
#else
    im2font = imlib_load_font("VeraMono/12");
#endif
    imlib_context_set_font(im2font);
    imlib_context_set_color(0xff, 0xff, 0xff, 0xff);
#endif
#if (RL_LABEL)
    imlib_get_text_size(string,
                        &textw, &texth);
    textimg = imlib_create_image(textw, texth);
    imlib_context_set_image(textimg);
    imlib_text_draw(0,
                    0,
		    string);
    imlib_image_clear();
    imlib_image_set_has_alpha(1);
    imlib_context_set_blend(0);
    imlib_context_set_anti_alias(0);
    imlib_context_set_drawable(image->tpixmap);
    imlib_render_image_on_drawable(0, 0);
    XSetWindowBackgroundPixmap(app->display,
                               label->id,
                               image->tpixmap);
    XClearWindow(app->display, label->id);
    curwindow = label;
#else
    curwindow = parent;
#endif
#if (RLRULERS)
    ruler = R_create_window(app,
                            parent,
                            flags);
    if (ruler == NULL) {
        
        exit(1);
    }
    image = R_load_image_imlib2(app,
                                RESURRECTION_IMAGE_SEARCH_PATH "widget/vruler.png",
                                NULL);
    image->flags |= R_IMAGE_STATIC_FLAG;
    R_render_image_imlib2(image, ruler, RL_LABEL_WIDTH, RL_COMMANDS * (RL_BUTTON_WIDTH + RL_RULER_WIDTH), 0);
    R_move_resize_window(ruler,
                         0,
                         0,
                         RL_BUTTON_WIDTH + 2 * RL_RULER_WIDTH,
                         RL_COMMANDS * (RL_BUTTON_WIDTH + RL_RULER_HEIGHT));
    ruler = R_create_window(app,
                            parent,
                            flags);
    if (ruler == NULL) {
        
        exit(1);
    }
    R_render_image_imlib2(image, ruler, RL_LABEL_WIDTH, RL_COMMANDS * (RL_BUTTON_WIDTH + RL_RULER_WIDTH), 0);
    R_move_resize_window(ruler,
                         0,
                         0,
                         RL_RULER_WIDTH,
                         RL_BUTTON_WIDTH + 2 * RL_RULER_HEIGHT);
    ruler = R_create_window(app,
                            parent,
                            flags);
    if (ruler == NULL) {
        
        exit(1);
    }
    R_move_resize_window(ruler,
                         0,
                         RL_COMMANDS * (RL_BUTTON_HEIGHT + RL_RULER_WIDTH),
                         RL_BUTTON_WIDTH + 2 * RL_RULER_WIDTH,
                         RL_COMMANDS * (RL_BUTTON_WIDTH + RL_RULER_HEIGHT));
#endif
    for (i = 0 ; i < RL_COMMANDS ; i++) {
        button = R_create_window(app,
                                 parent,
                                 flags);
        if (button == NULL) {

            exit(1);
        }
        button->parent = parent;
//        button->id = i;
        button->num = i;
#if (RL_TERM_TABS)
        if (!i) {
            tabwins[i] = R_create_window(app,
                                         NULL,
                                         R_WINDOW_OVERRIDE_REDIRECT_FLAG);
        }
#endif
#if (RL_HORIZONTAL)
#if (RL_BAR)
        R_move_resize_window(button,
                             i * RL_BUTTON_WIDTH,
                             0,
                             RL_BUTTON_WIDTH,
                             RL_BUTTON_HEIGHT);
#else
        R_move_resize_window(button,
                             i * RL_BUTTON_WIDTH,
                             0,
                             RL_BUTTON_WIDTH,
                             RL_BUTTON_HEIGHT);
#endif
#else
#if (RL_BAR)
        R_move_resize_window(button,
                             0,
                             RL_RULER_HEIGHT + i * RL_BUTTON_HEIGHT,
                             RL_BUTTON_WIDTH,
                             RL_BUTTON_HEIGHT);
#else
        R_move_resize_window(button,
                             0,
                             i * RL_BUTTON_HEIGHT,
                             RL_BUTTON_WIDTH,
                             RL_BUTTON_HEIGHT);
#endif
#endif
#if (RL_TOOLTIPS)
        imlib_context_set_drawable(button->id);
        tooltips[i] = Rl_init_tooltip(app, i);
#endif
#if 0
        RL_SET_WINDOW_COORDS(button,
                             x,
                             0);
        RL_SET_WINDOW_SIZE(button,
                           RL_BUTTON_WIDTH,
                           RL_BUTTON_HEIGHT);
#endif
        buttonimgs[i].normal = R_load_image_imlib2(app, Rl_imagenames[i + 1][RL_BUTTON_NORMAL], NULL);
        buttonimgs[i].active = R_load_image_imlib2(app, Rl_imagenames[i + 1][RL_BUTTON_ACTIVE], NULL);
        buttonimgs[i].clicked = R_load_image_imlib2(app, Rl_imagenames[i + 1][RL_BUTTON_CLICKED], NULL);
//        imlib_context_set_image(buttonimgs[i].normal);
        R_render_thumb_imlib2(buttonimgs[i].normal,
                              button,
                              RL_BUTTON_WIDTH,
                              RL_BUTTON_HEIGHT,
                              0);
#if (RL_OLDE_THEME)
        if (hasshape && (buttonimgs[i].normal->tmask)) {
            XShapeCombineMask(app->display,
                              parent->id,
                              ShapeBounding,
                              0,
                              RL_LABEL_HEIGHT + i * RL_BUTTON_HEIGHT,
                              buttonimgs[i].normal->tmask,
                              ShapeUnion);
        } else {
            fprintf(stderr, "BLAH\n");
        }
#endif
                          
        R_render_thumb_imlib2(buttonimgs[i].active,
                              button,
                              RL_BUTTON_WIDTH,
                              RL_BUTTON_HEIGHT,
                              0);
        R_render_thumb_imlib2(buttonimgs[i].clicked,
                              button,
                              RL_BUTTON_WIDTH,
                              RL_BUTTON_HEIGHT,
                              0);
        R_set_thumb_imlib2(buttonimgs[i].normal,
                           button,
                           RL_BUTTON_WIDTH,
                           RL_BUTTON_HEIGHT,
                           0);
        Rl_init_button_events(button);
        R_add_window(button);
        R_map_window_raised(button);
        curwindow->chain = button;
        curwindow = button;
    }
    button->chain = NULL;
#if 0 && (RL_OLDE_THEME)
    if (hasshape) {
        XShapeCombineMask(app->display,
                          parent->id,
                          ShapeBounding,
                          0, 0,
                          shapebitmap,
                          ShapeSet);
    }
#endif
#if (RL_HORIZONTAL)
#if (RL_BAR)
    R_move_resize_window(parent,
                         0,
                         0,
                         RL_COMMANDS * RL_BUTTON_WIDTH,
                         RL_BUTTON_HEIGHT);
#else
    R_move_resize_window(parent,
                         0,
                         0,
                         RL_BUTTON_WIDTH,
                         RL_BUTTON_HEIGHT);
#endif
#else
#if (RL_BAR)
    R_move_resize_window(parent,
                         0,
                         0,
                         RL_BUTTON_WIDTH,
                         RL_COMMANDS * RL_BUTTON_HEIGHT);
#else
    R_move_resize_window(parent,
                         0,
                         0,
                         RL_BUTTON_WIDTH,
                         RL_BUTTON_HEIGHT);
#endif
#endif

    return parent;
}

void
Rl_map_windows(struct R_app *app)
{
    struct R_window *window;

    window = app->window;
    R_map_window_raised(window);
#if 0
    R_map_subwindows(window);
#endif

    return;
}

void
Rl_init_parent_events(struct R_window *window)
{
    long mask;

    R_set_window_event_handler(window,
                               EnterNotify,
                               Rl_parent_enternotify_handler);
    R_set_window_event_handler(window,
                               LeaveNotify,
                               Rl_parent_leavenotify_handler);
    mask = R_add_window_events(window,
                               EnterWindowMask | LeaveWindowMask);

    return;
}

void
Rl_init_button_events(struct R_window *window)
{
    long mask;

    R_set_window_event_handler(window,
                               VisibilityNotify,
                               Rl_button_visibilitynotify_handler);
    R_set_window_event_handler(window,
                               Expose,
                               Rl_button_expose_handler);
    R_set_window_event_handler(window,
                               ButtonPress,
                               Rl_button_buttonpress_handler);
    R_set_window_event_handler(window,
                               EnterNotify,
                               Rl_button_enternotify_handler);
    R_set_window_event_handler(window,
                               LeaveNotify,
                               Rl_button_leavenotify_handler);
    mask = R_add_window_events(window,
                               ExposureMask | EnterWindowMask | LeaveWindowMask
                               | VisibilityChangeMask
                               | ButtonPressMask);

    return;
}

void
Rl_init_terminal_button_events(struct R_window *window)
{
    long mask;

#if (RL_TERM_TABS)
    R_set_window_event_handler(window,
                               Expose,
                               Rl_terminal_button_expose_handler);
    R_set_window_event_handler(window,
                               EnterNotify,
                               Rl_terminal_button_enternotify_handler);
    R_set_window_event_handler(window,
                               LeaveNotify,
                               Rl_terminal_button_leavenotify_handler);
#else
    R_set_window_event_handler(window,
                               Expose,
                               Rl_button_expose_handler);
    R_set_window_event_handler(window,
                               EnterNotify,
                               Rl_button_enternotify_handler);
    R_set_window_event_handler(window,
                               LeaveNotify,
                               Rl_button_leavenotify_handler);
#endif
    mask = R_add_window_events(window,
                               ExposureMask | EnterWindowMask | LeaveWindowMask);

    return;
}

void
Rl_parent_enternotify_handler(void *arg,
                              XEvent *event)
{
    struct R_window *window;
    pid_t pid;
    char *argv[2];

    window = arg;
#if !RL_BAR
#if (RL_HORIZONTAL)
    R_resize_window(window,
                    RL_COMMANDS * RL_BUTTON_WIDTH,
                    RL_BUTTON_HEIGHT);
#else
    R_resize_window(window,
                    RL_BUTTON_WIDTH,
                    RL_COMMANDS * RL_BUTTON_HEIGHT);
    R_raise_window(window);
#endif
    XSync(window->app->display, False);
#endif

    return;
}

void
Rl_parent_leavenotify_handler(void *arg,
                              XEvent *event)
{
    struct R_window *window;
    pid_t pid;
    char *argv[2];

    window = arg;
#if !RL_BAR
    R_resize_window(window,
                    RL_BUTTON_WIDTH,
                    RL_BUTTON_HEIGHT);
    XSync(window->app->display, False);
#endif

    return;
}

void
Rl_button_visibilitynotify_handler(void *arg,
                                   XEvent *event)
{
    struct R_window *window;
    
    window = arg;
    R_raise_window(window->parent);

    return;
}

void
Rl_button_expose_handler(void *arg,
                         XEvent *event)
{
    struct R_window *window;
    
    window = arg;
#if (USE_IMLIB2)
    if (!event->xexpose.count && buttonimgs[window->num].normal) {
        R_set_thumb_imlib2(buttonimgs[window->num].normal,
                           window,
                           RL_BUTTON_WIDTH,
                           RL_BUTTON_HEIGHT,
                           0);
    }
#endif

    return;
}

void
Rl_button_enternotify_handler(void *arg,
                  XEvent *event)
{
    struct R_window *window;
    
    window = arg;
#if (USE_IMLIB2)
    if (buttonimgs[window->num].active) {
        R_set_thumb_imlib2(buttonimgs[window->num].active,
                           window,
                           RL_BUTTON_WIDTH,
                           RL_BUTTON_HEIGHT,
                           0);
    }
#if (RL_TOOLTIPS)
    R_map_window_raised(tooltips[window->num]);
#endif
#endif

    return;
}

void
Rl_button_leavenotify_handler(void *arg,
                  XEvent *event)
{
    struct R_window *window;
    int i;
    
    window = arg;
#if (USE_IMLIB2)
    if (buttonimgs[window->num].normal) {
        R_set_thumb_imlib2(buttonimgs[window->num].normal,
                           window,
                           RL_BUTTON_WIDTH,
                           RL_BUTTON_HEIGHT,
                           0);
    }
#if (RL_TOOLTIPS)
    for (i = 0 ; i < RL_COMMANDS ; i++) {
        R_unmap_window(tooltips[i]);
    }
#endif
#endif
}

#if (RL_TERM_TABS)
struct Rl_termargs {
    int argc;
    char *argv;
    int flags;
};

void
Rl_start_terminal_tab(long flags)
{
    struct R_app app;
    struct R_term term;
    struct R_window *button;
    pid_t pid;
    int fifo;
    int loop;
    sigset_t     oldset;
    sigset_t     blkset;
    ssize_t      nb;
    ssize_t      ndone;
    char *argv[6] = { "Rterm", "-B", "-X", "-g", "80x24+40+40", NULL };
    char ipcstr = "TERM";
    char buf[4];
    char  res[4];
    char *cp = res;
//    char fifoname[L_tmpnam];
#if (RL_TERM_TABS)
    fd_set       readset;
#endif

//    nb = 0;
//    ndone = 0;
    sigaddset(&blkset, SIGCHLD);
    sigprocmask(SIG_BLOCK, &blkset, &oldset);

    if (Rltermcnt == RL_MAX_TABS) {

        return;
    }
    memset(&app, 0, sizeof(app));
    memset(&term, 0, sizeof(term));
    app.name = "Rterm";
    term.flags |= flags;
#if (RL_RTERM_IPC)
    fifo = pipe(ipcpipes[Rltermcnt]);
#endif
    fprintf(stderr, "ipcpipes[0] == %d, ipcpipes[1] == %d\n",
            ipcpipes[Rltermcnt][0], ipcpipes[Rltermcnt][1]);
    pid = fork();
    if (!pid) {
#if (RL_RTERM_IPC)
        int maxfd = ipcpipes[Rltermcnt][0];
#endif

        SIGNAL(SIGCHLD, SIG_DFL);
        sigprocmask(SIG_UNBLOCK, &blkset, NULL);

#if (RL_RTERM_IPC)
        fprintf(stderr, "MAXFD: %d\n", (int)maxfd);
        FD_SET(ipcpipes[Rltermcnt][0], &readset);
        select(maxfd + 1, &readset, NULL, NULL, NULL);
        if (FD_ISSET(ipcpipes[Rltermcnt][0], &readset)) {
            Rread(ipcpipes[Rltermcnt][0], buf, 4);
            if (strncmp(buf, ipcstr, 4)) {
                execvp("Rterm", argv);
            }
#if 0
            close(ipcpipes[Rltermcnt][0]);
            close(ipcpipes[Rltermcnt][1]);
#endif
        }
#endif /* RLRTERMIPC */
    } else {
        fprintf(stderr, "PID[%d] = %d\n", Rltermcnt, pid);
        Rltermpids[Rltermcnt] = pid;
        button = R_create_window(R_global.app, tabwins[0], 0);
        Rltermbuttons[Rltermcnt] = button;
        Rltermcnt++;
        fprintf(stderr, "LAUNCH: %ld\n", pid);
        R_move_resize_window(tabwins[0],
                             RL_BUTTON_WIDTH, 0,
                             Rltermcnt * RL_BUTTON_WIDTH, RL_BUTTON_HEIGHT);
        R_move_resize_window(button,
                             (Rltermcnt - 1) * RL_BUTTON_WIDTH, 0,
                             RL_BUTTON_WIDTH,
                             RL_BUTTON_HEIGHT);
        R_set_thumb_imlib2(buttonimgs[0].normal, button,
                           RL_BUTTON_WIDTH, RL_BUTTON_HEIGHT,
                           0);
        Rl_init_terminal_button_events(button);
        R_add_window(button);
        R_map_window_raised(button);
        R_map_window_raised(tabwins[0]);
        R_move_window(tooltips[0],
                      RL_BUTTON_WIDTH + Rltermcnt * RL_BUTTON_WIDTH,
                      0);
        XSync(R_global.app->display, False);
        sigprocmask(SIG_UNBLOCK, &blkset, NULL);
#if (RL_RTERM_IPC)
        Rwrite(ipcpipes[Rltermcnt - 1][1], ipcstr, 4);
#endif
    }

    return;
}
#endif

void
Rl_button_buttonpress_handler(void *arg,
                              XEvent *event)
{
    struct R_window *window;
    int button;
    uintptr_t cmd;

    button = event->xbutton.button;
    button--;
    window = arg;
#if (RL_TERM_TABS)
    if (event->xbutton.button == Button1
        && (event->xbutton.state & ControlMask)) {
        pthread_mutex_lock(&termmtx);
        Rl_start_terminal_tab(RTERM_DEFAULT_FLAGS);
        pthread_mutex_unlock(&termmtx);

        return;
    } else {
#endif
        cmd = (uintptr_t)(Rl_commands[window->num][button]);
        if (cmd < RL_MAX_ACTION) {
            switch (cmd) {
                case RL_EXIT:
                    
                    exit(0);
                case RL_RESTART:
                {
                    char *arg[2] = { "Rwm", NULL };
                    pid_t pid = fork();

                    if (!pid) {
                        execvp("Rwm", arg);
                    }
                }
                break;
                default:
                    
                    return;
            }
        } else if (cmd) {
            char *arg[2] = { cmd, NULL };
            pid_t pid;

            pid = fork();
            if (!pid) {
                execvp((void *)cmd, arg);
            }
        }
#if (RL_TERM_TABS)
    }
#endif
#if (USE_IMLIB2)
    if (buttonimgs[window->num].clicked) {
        R_set_thumb_imlib2(buttonimgs[window->num].clicked,
                           window,
                           RL_BUTTON_WIDTH,
                           RL_BUTTON_HEIGHT,
                           0);
    }
#endif

    return;
}

void
Rl_terminal_button_expose_handler(void *arg,
                                  XEvent *event)
{
    struct R_window *window;

    window = arg;
    if (buttonimgs[0].normal) {
        R_set_thumb_imlib2(buttonimgs[0].normal,
                           window,
                           RL_BUTTON_WIDTH,
                           RL_BUTTON_HEIGHT,
                           0);
    }

    return;
}

#if (RL_TERM_TABS)

void
Rl_terminal_button_enternotify_handler(void *arg,
                                       XEvent *event)
{
    struct R_window *window;

    window = arg;
    fprintf(stderr, "ENTER: %ld\n", Rltermpids[window->num]);
    if (window->num) {
        if (!Rlcurterm) {
            if (kill(Rltermpids[window->num], SIGUSR1) < 0 && errno == ESRCH) {
                pthread_mutex_lock(&termmtx);
                Rl_remove_terminal_tab(Rltermpids[window->num]);
                pthread_mutex_unlock(&termmtx);
                
                Rlcurterm = 0;
            } else {
                Rlcurterm = Rltermpids[window->num];
            }
        }
#if 0
        if (Rlcurterm) {
            if (kill(Rltermpids[window->num], SIGUSR2) < 0 && errno == ESRCH) {
                Rl_remove_terminal_tab(Rltermpids[window->num]);
            }
        }
        if (kill(Rltermpids[window->num], SIGUSR1) < 0 && errno == ESRCH) {
            Rl_remove_terminal_tab(Rltermpids[window->num]);
            
            Rlcurterm = 0;
        } else {
            Rlcurterm = Rltermpids[window->num];
        }
#endif
    }

    return;
}

void
Rl_terminal_button_leavenotify_handler(void *arg,
                                       XEvent *event)
{
    struct R_window *window;

#if 0
    window = arg;
    fprintf(stderr, "LEAVE: %ld\n", Rltermpids[window->num]);
    if (kill(Rltermpids[window->num], SIGUSR2) < 0 && errno == ESRCH) {
        Rl_remove_terminal_tab(Rltermpids[window->num]);
    }
#endif

    return;
}

#endif /* RL_TERM_TABS */
