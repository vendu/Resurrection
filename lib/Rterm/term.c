/*
 * term.c - terminal routines for Resurrection.
 * Copyright (C) 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#define RTERM_LOCALE "en_US.ISO-8859-1"

#define RTERM_WIDGET 1

#if 0
void Rterm_map_on_enter(void *arg, XEvent *event);
#endif

RETSIGTYPE Rterm_usr1_signal(int sig);
RETSIGTYPE Rterm_usr2_signal(int sig);

struct R_term *Rterms[RTERM_TABS];
volatile int Rtermtabs;

#if (SUPPORT_RTERM_UTF8_CHARS)
int
Rterm_probe_locale_utf8(void)
{
    char *str;
    int retval = 0;

    if (((str = getenv("LC_ALL")) && *str)
        || ((str = getenv("LC_CTYPE")) && *str)
        || ((str = getenv("LANG")) && *str)) {
        fprintf(stderr, "LOCALE: %s\n", str);
        if (strstr(str, "UTF-8")) {

            retval = 1;
        }
    }

    fprintf(stderr, "UTF-8: %d\n", retval);

    return retval;
}
#endif

struct R_term *
Rterm_alloc(void)
{
    return (calloc(1, sizeof(struct R_term)));
}

void Rterm_cleanup_draw_buffer(void)
{
    struct R_app *app;
    struct R_term *term;
    struct R_termscreen *screen;
    int i;
#if (RTERM_USE_XSHM)
    XShmSegmentInfo *shmseg;
#endif

    app = R_global.app;
    term = app->client;
#if (RTERM_USE_XSHM)
#if (SUPPORT_XSHM_FONT_BITMAP)
    shmseg = &screen->ftfont->shminfo;
    if (shmseg->shmaddr) {
        XShmDetach(app->display, shmseg);
        shmdt(shmseg->shmaddr);
        shmctl(shmseg->shmid, IPC_RMID, NULL);
        shmseg->shmid = -1;
        shmseg->shmaddr = NULL;
    }
#endif
    for (i = 0 ; i < term->nscreens ; i++) {
        screen = term->screens[i];
        shmseg = &screen->bufshminfo;
        if (shmseg->shmaddr) {
            XShmDetach(app->display, shmseg);
            shmdt(shmseg->shmaddr);
            shmctl(shmseg->shmid, IPC_RMID, NULL);
            shmseg->shmid = -1;
            shmseg->shmaddr = NULL;
        }
    }
#endif
}

#if (SUPPORT_TRUETYPE_FONTS)
void Rterm_cleanup_font_freetype(void)
{
    struct R_app *app;
    struct R_term *term;
    struct R_termscreen *screen;
    int i, j, k;
#if (RTERM_USE_XSHM)
    XShmSegmentInfo *shmseg;
#endif

    app = R_global.app;
    term = app->client;
#if (RTERM_USE_XSHM)
#if (SUPPORT_RTERM_UTF8_CHARS)
    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
        k= 65536;
    } else {
        k = 256;
    }
#else
    k = 256;
#endif
    for (i = 0 ; i < term->nscreens ; i++) {
        screen = term->screens[i];
#if (SUPPORT_XSHM_FONT_BITMAP)
            shmseg = &screen->ftfont->shminfo;
            if (shmseg->shmaddr) {
                XShmDetach(app->display, shmseg);
                shmdt(shmseg->shmaddr);
                shmctl(shmseg->shmid, IPC_RMID, NULL);
                shmseg->shmid = -1;
                shmseg->shmaddr = NULL;
            }
#else
        for (j = 0 ; j < k ; j++) {
            shmseg = &screen->ftfont->shminfos[i];
            if (shmseg->shmaddr) {
                XShmDetach(app->display, shmseg);
                shmdt(shmseg->shmaddr);
                shmctl(shmseg->shmid, IPC_RMID, NULL);
                shmseg->shmid = -1;
                shmseg->shmaddr = NULL;
            }
        }
#endif
    }
#endif
}
#endif

void
Rterm_cleanup(void)
{
    Rterm_cleanup_draw_buffer();
#if (SUPPORT_FREETYPE_FONTS)
    Rterm_cleanup_font_freetype();
#endif

    return;
}

void
Rterm_init_charsets(struct R_term *term)
{
    if (term == NULL) {

	return;
    }

    term->charset = RTERM_CHARSET_G0;
    term->charsets[RTERM_CHARSET_G0] = 'B'; /* ASCII. */
    term->charsets[RTERM_CHARSET_G1] = 'B'; /* DEC line drawing. */
    term->charsets[RTERM_CHARSET_G2] = 'B'; /* DEC supplemental. */
    term->charsets[RTERM_CHARSET_G3] = 'B';

    return;
}

void
Rterm_init_cursor(struct R_term *term)
{
    struct R_app *app;
    Cursor cursor;

    app = R_global.app;
    cursor = XCreateFontCursor(app->display, XC_xterm);
    XDefineCursor(app->display, term->window->id, cursor);
    XFreeCursor(app->display, cursor);

    return;
}

void
Rterm_print_usage(void)
{
    fprintf(stderr, "\tRterm options\n");
    fprintf(stderr, "\t-------------\n");
    fprintf(stderr, "\t-b <path>\tuse background image <path>\n");
    fprintf(stderr, "\t-f <name>\tuse font <name>\n");
    fprintf(stderr, "\t-g <geom>\tsupply X geometry string <geom>\n");
    fprintf(stderr, "\t-B\t\trun with no borders/decoration\n");
    fprintf(stderr, "\t-C\t\tenable console output\n");
    fprintf(stderr, "\t-X\t\tenable screen draw buffers\n");
    fprintf(stderr, "\t-s\t\tscale background image\n");

    return;
}

void
Rterm_parse_options(struct R_term *term, int argc, char *argv[])
{
    int i;
    int x;
    int y;
    unsigned int w;
    unsigned int h;
    int valmask;

    if (argc == 1) {
        fprintf(stderr, "NO OPTIONS to parse\n");
    }

    term->flags |= RTERM_DEFAULT_FLAGS;
    term->fontname = RTERM_DEFAULT_FONTNAME;
    for (i = 1 ; i < argc ; i++) {
//        fprintf(stderr, "argv[%d] == %s\n", i, argv[i]);
        if (!strcmp(argv[i], "-h")) {
            Rterm_print_usage();
            
            exit(0);
        } else if (!strcmp(argv[i], "-C")) {
            fprintf(stderr, "enabled console output\n");
            term->flags |= RTERM_CONSOLE;
        } else if (!strcmp(argv[i], "-b")) {
            fprintf(stderr, "enabled background image %s\n", argv[i + 1]);
            term->bgname = argv[++i];
        } else if (!strcmp(argv[i], "-s")) {
            fprintf(stderr, "enabled background image scaling\n");
            term->flags |= RTERM_SCALE_BACKGROUND_PIXMAP;
        } else if (!strcmp(argv[i], "-fg")) {
            term->fgcolor = atoi(argv[++i]);
            fprintf(stderr, "fg: %s\n", argv[i]);
        } else if (!strcmp(argv[i], "-f")) {
            fprintf(stderr, "enabled font %s\n", argv[i + 1]);
            term->fontname = argv[++i];
        } else if (!strcmp(argv[i], "-g")) {
            fprintf(stderr, "SIZE: %s\n", argv[++i]);
            valmask = XParseGeometry(argv[i], &x, &y, &w, &h);
            fprintf(stderr, "%dx%d @ %d, %d\n", w, h, x, y);
            if (valmask & XValue) {
                term->x = x;
            }
            if (valmask & YValue) {
                term->y = y;
            }
            if (valmask & WidthValue) {
                term->w = w;
            }
            if (valmask & HeightValue) {
                term->h = h;
            }
        } else if (!strcmp(argv[i], "-B")) {
            fprintf(stderr, "enabled borderless window\n");
            term->flags |= RTERM_BORDERLESS;
        } else if (!strcmp(argv[i], "-X")) {
            fprintf(stderr, "enabled draw buffer\n");
            term->flags |= RTERM_DOUBLE_BUFFER;
#if 0
        } else if (!strcmp(argv[i], "-R")) {
            fprintf(stderr, "enabled enter resize\n");
            term->flags |= RTERM_ENTER_RESIZE;
#endif
        }
    }
}

struct R_term *
Rterm_start(struct R_term *term)
{
    struct R_app *app = R_global.app;

#if 0
    if (term->maponenter) {
        fprintf(stderr, "MAP_ON_ENTER: %x\n", term->maponenter->id);
        R_set_window_event_handler(term->maponenter, EnterNotify, Rterm_map_on_enter);
        R_add_window_events(term->maponenter, EnterWindowMask);
//        R_add_window(term->maponenter);
    }
#endif

//    Rterm_parse_cmdline(term, argc, argv);

    /* store user and group ids. */
    user_init(&(term->user));

    /* disable root privileges in case we're setuid root. */
    user_disable_privileges(&term->user);

    /*
     * set input field separator for shell to TAB for old shells that don't
     * reset it. This is for security reasons to prevent users from setting
     * IFS to make system() execute a different program.
     */

    putenv("IFS= \t");

    /*
     * export terminal type.
     */

//    putenv("TERM=xterm");
    putenv("TERM=vt100");

    if ((term->path = calloc(1, PATH_MAX + 1)) == NULL) {

        return NULL;
    }

    if (getcwd(term->path, PATH_MAX + 1) == NULL) {

        return NULL;
    }

    term->flags |= RTERM_DEFAULT_FLAGS;
    term->privmodes |= RTERM_DEFAULT_PRIVMODES;

    Rterm_init_charsets(term);

#if 0
    if (app_init(app, RTERM_NAME, argc, argv) < 0) {

	return NULL;
    }
#endif

#if 0
    if (Rterm_register_options(term) < 0) {

	return NULL;
    }

    if (Rterm_register_configs(term) < 0) {

	return NULL;
    }
#endif

    if (Rterm_register_escape_handlers() < 0) {

	return NULL;
    }

    if (Rterm_register_csi_handlers() < 0) {

	return NULL;
    }

    if (Rterm_register_private_mode_handlers() < 0) {

	return NULL;
    }

    if (Rterm_register_sgr_handlers() < 0) {

	return NULL;
    }
 
    if (Rterm_register_escape_handlers() < 0) {

	return NULL;
    }

#if 0
    if (eterm_register_ipc_handlers() < 0) {

	return NULL;
    }

    if (eterm_register_winop_handlers() < 0) {

	return NULL;
    }
#endif

    if (term->privmodes & RTERM_PRIVMODE_MENUBAR) {
        term->privmodes &= ~RTERM_PRIVMODE_MENUBAR;
    }

    if (Rterm_init_screens(term, RTERM_SCREENS) < 0) {
        
        return NULL;
    }
        
//    atexit(Rterm_cleanup);
    
    if (Rterm_init_windows(term) < 0) {
        
        return NULL;
    }
    
    if (Rterm_init_selection_gc(term) < 0) {
        
        return NULL;
    }
        
#if 0
    if (term->privmodes & RTERM_PRIVMODE_MENUBAR) {
	if (Rterm_init_menubar(term) < 0) {
            
	    return NULL;
	}
    }
#endif
    
#if 0
    if (Rterm_init_pop_menu(term) < 0) {
        
        return NULL;
    }
#endif

#if 0    
    if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
	if (Rterm_init_scrollbar(term) < 0) {
            
	    return NULL;
	}
        
#if 0
	if (term->flags & RTERM_TRANSPARENT) {
	    term->scrollbar->window->images[R_WINDOW_BACKGROUND_IMAGE]->flags |= IMAGE_TRANSPARENT;
        }
#endif
    }
#endif
    
    Rterm_init_cursor(term);
    
    Rterm_map_windows(term);
    
    XSync(term->window->app->display, False);

#if (SUPPORT_RTERM_UTF8_CHARS)
    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
        term->funcs.read_char = Rterm_read_char_utf8;
    } else {
        term->funcs.read_char = Rterm_read_char;
    }
#else
    term->funcs.read_char = Rterm_read_char;
#endif
    term->funcs.queue_input = Rterm_queue_input;
    term->funcs.queue_output = Rterm_queue_output;
    term->funcs.add_text = Rterm_add_text;

    return term;
}
    

struct R_term *
Rterm_init(struct R_term *term, int argc, char *argv[], long flags)
{
    struct R_app *app;
#if (SUPPORT_RTERM_TABS)
    long l;
#endif

    if (!term) {
        if ((term = Rterm_alloc()) == NULL) {
            
            return NULL;
        }
    }
    term->flags |= flags;

#if 0
    SIGNAL(SIGUSR1, Rterm_usr1_signal);
    SIGNAL(SIGUSR2, Rterm_usr2_signal);
#endif

    app = R_global.app;
    app->client = term;

    Rterm_parse_options(term, argc, argv);

#if defined RTERM_LOCALE
    app_init_locale(RTERM_LOCALE);
#elif (SUPPORT_RTERM_UTF8_CHARS)
//    if (Rterm_probe_locale_utf8()) {
        if (app_init_locale() < 0) {
            
            return NULL;
        }
        term->privmodes |= RTERM_PRIVMODE_UTF8_CHARS;
//    }
#endif

#if (SUPPORT_TRUETYPE_FONTS) && (SUPPORT_FREETYPE2)
    if (app_init_freetype(app) < 0) {

        return NULL;
    }
#endif

    term->xconnfd = ConnectionNumber(app->display);
    if (term->flags & RTERM_BORDERLESS) {
        term->window = R_create_window(app,
                                       NULL,
                                       R_WINDOW_OVERRIDE_REDIRECT_FLAG
                                       | R_WINDOW_DO_NOT_PROPAGATE_FLAG);
    } else {
        term->window = R_create_window(app,
                                       NULL,
                                       R_WINDOW_DO_NOT_PROPAGATE_FLAG);
    }
    R_move_window(term->window, term->x, term->y);
    R_add_window(term->window);
    app->window = term->window;

    SIGNAL(SIGUSR1, Rterm_usr1_signal);
    SIGNAL(SIGUSR2, Rterm_usr2_signal);

    return term;
}

int
Rterm_loop(struct R_term *term)
{
    int ch;
    struct R_termscreen *screen;
#if (SUPPORT_RTERM_BLINKING_CURSOR) || (SUPPORT_RTERM_BLINKING_CHARS)
    struct timeval curtv;
#endif
    int (*read_char)(struct R_term *);
    void (*add_text)(struct R_term *, struct R_termscreen *);
    void (*refresh)(struct R_termscreen *, int);
#if (SUPPORT_RTERM_UTF8_CHARS)
    R_text_t utf8buf[8];
    int len;
    int i;
#endif

    read_char = term->funcs.read_char;
    add_text = term->funcs.add_text;
    do {
#if (SUPPORT_RTERM_UTF8_CHARS)
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
            memset(utf8buf, 0, sizeof(utf8buf));
            i = 0;
        }
#endif
        screen = term->screens[term->curscreen];
        refresh = screen->funcs.refresh;

#if (SUPPORT_RTERM_BLINKING_CURSOR) || (SUPPORT_RTERM_BLINKING_CHARS)
        
        while ((ch = read_char(term)) == 0) {
            gettimeofday(&curtv, NULL);
            screen = term->screens[term->curscreen];
            refresh = screen->funcs.refresh;
            if (screen->pty.masterfd == -1) {

                break;
            }
            if (TIMEVAL_CMP_USEC(&(term->blinktv), &curtv)
                > 0) {
                if (screen->flags & RTERM_SCREEN_HAS_BLINKING_CHARS) {
                    if (term->blink) {
                        refresh(screen, RTERM_SCREEN_DRAW_ALL);
                        term->blink = FALSE;
                    } else {
                        refresh(screen, RTERM_SCREEN_DRAW_BLINKING_ONLY);
                        term->blink = TRUE;
                    }
                }
                if (screen->flags & RTERM_SCREEN_HAS_BLINKING_CURSOR) {
                    if (term->blink) {
                        Rterm_draw_screen_cursor(screen);
                        term->blink = FALSE;
                    } else {
                        Rterm_clear_screen_cursor(screen);
                        term->blink = TRUE;
                    }
                }
                TIMEVAL_ADD_CONST_2(&(term->blinktv), &curtv,
                                    RTERM_SELECT_BLINK_TIMEOUT_USEC);
            }
            if (screen->refreshcnt && screen == term->screens[term->curscreen]) {
                screen->refreshcnt = 0;
                screen->refreshed = TRUE;
//                Rterm_touch_screen(screen);
                refresh(screen, RTERM_SCREEN_DRAW_ALL);
//        Rterm_update_scrollbar(term);
                if (!term->blink) {
                    Rterm_draw_screen_cursor(screen);
#if 0
                } else {
                    Rterm_clear_screen_cursor(screen);
#endif
                }
            }
        }
#else /* !((SUPPORT_RTERM_BLINKING_CURSOR) || (SUPPORT_RTERM_BLINKING_CHARS)) */
        while ((ch = read_char(term)) == 0) {
            ;
        }
        
#endif /* (SUPPORT_RTERM_BLINKING_CURSOR) || (SUPPORT_RTERM_BLINKING_CHARS) */

#if 0 && (SUPPORT_RTERM_UTF8_CHARS)
        if ((term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) && ch >= 0x80) {
            utf8buf[i++] = (unsigned char)ch;
            if (Rterm_decode_char_utf8(utf8buf, &len) && (len)) {
                add_text(term, screen);
            }
        } else if (ch > 0) {
            add_text(term, screen);
        }
#else        
        if (ch > 0) {
            add_text(term, screen);
        }
#endif
        
#if (SUPPORT_RTERM_TABS)
        else if (screen->pty.masterfd == -1) {
            if (Rtermtabs) {
                R_unmap_window(screen->window);
                term->curscreen = 0;
                screen = term->screens[term->curscreen];
                while (screen->pty.masterfd == -1) {
                    term->curscreen += 2;
                    screen = term->screens[term->curscreen];
                }
                R_map_window(screen->window);
//                XSync(screen->window->app->display, False);
//                Rterm_sync_screen(screen, 0, 0, 0, RTERM_SCREEN_DRAW_ALL);
                screen->refreshcnt = -1;
#if 0
                screen->refreshcnt = 0;
                screen->refreshed = TRUE;
//                Rterm_touch_screen(screen);
                Rterm_touch_screen(screen);
                Rterm_clear_screen_buffer(screen, 0, 0, 0, RTERM_SCREEN_CLEAR_BUFFER);
                refresh(screen, RTERM_SCREEN_DRAW_ALL);
                Rterm_sync_screen(screen, 0, 0, 0, RTERM_SCREEN_SYNC);
#endif
            } else {
                
                return -1;
            }
        }
#endif
    } while (ch != -1 || (SUPPORT_RTERM_TABS && (Rtermtabs)));

    return 0;
}

int
Rterm_start_tab(struct R_term *term, int id)
{
    struct R_app *app;
    struct R_termscreen *screen;
    pid_t pid;
    int ptyfd;

    screen = term->screens[2 * id];
//    term->screenids[id] = RTERM_PRIMARY_SCREEN;
    ptyfd = Rterm_init_master_pty(term, screen);
    if (ptyfd < 0) {
        fprintf(stderr, "failed initializing master pty\n");
        
        return -1;
    }            
    PTY_INIT_MODE(ptyfd, &(screen->pty.mode));
    Rterm_set_winsize(term, screen->rows, screen->columns, ptyfd);
    screen->pty.masterfd = ptyfd;
    
    pid = fork();

    if (pid < 0) {
        app = R_global.app;
        fprintf(stderr, "%s: Rterm_run_command(): fork() failed: %s\n",
                app->name, strerror(errno));
        
        return -1;
    }
    
#if 0
    term->args->commandpid = 0;
#endif
    
    fprintf(stderr, "TABS: %d\n", Rtermtabs);
    screen->cmdpid = pid;

    if (pid == 0) {
        /* child process. */
        SIGNAL(SIGINT, SIG_DFL);
        SIGNAL(SIGQUIT, SIG_DFL);
        SIGNAL(SIGCHLD, SIG_DFL);
        SIGNAL(SIGSEGV, SIG_DFL);
        SIGNAL(SIGBUS, SIG_DFL);
        SIGNAL(SIGABRT, SIG_DFL);
        SIGNAL(SIGFPE, SIG_DFL);
        SIGNAL(SIGILL, SIG_DFL);
#if defined(SIGSYS)
        SIGNAL(SIGSYS, SIG_DFL);
#endif
        SIGNAL(SIGALRM, SIG_DFL);
        
//#if (USE_JOB_CONTROL)
        SIGNAL(SIGTSTP, SIG_IGN);
        SIGNAL(SIGTTIN, SIG_IGN);
        SIGNAL(SIGTTOU, SIG_IGN);
//#endif
        SIGNAL(SIGCHLD, SIG_DFL);
        
        Rterm_run_command_child(term, screen);
    }

    /* parent process */
    fprintf(stderr, "PID: %ld\n", (long)pid);

    /* FIXME: glibc 2.1+ might not need root privileges for user logging. */
    
#if 0
    user_enable_privileges(&(term->args->user));
    
    userlog_init(&(term->args->userlog), ptyfd, term->args->masterpty,
                 DisplayString(app->display));
    
    userlog_add_log_entries(&(term->args->userlog));
    
    user_disable_privileges(&(term->args->user));
#endif
    Rtermtabs++;

    return ptyfd;
}

int
Rterm_run_command(struct R_term *term,
                  char *argv[])
{
    struct R_app *app;
    struct R_termscreen *screen;
    int ptyfd;
    pid_t pid;
    int i;
    
#if (CANNOT_IGNORE_SIGHUP)
    SIGNAL(SIGHUP, SIG_DFL);
#else
    SIGNAL(SIGHUP, SIG_IGN);
#endif
    
#if (CHECK_FOR_JOB_CONTROL_SHELL)
    
    if (getpid() == getpgrp()) {
        /* we were spawned by a shell that supports job control. */
        
    } else {
        /* we were spawned by a shell that doesn't support job control and
         * must ignore keyboard signals.
         */
    }
    
#else
    
#   if defined(IGNORE_PARENT_SIGINT)
    SIGNAL(SIGINT, SIG_IGN);
#   else
    SIGNAL(SIGINT, Rterm_exit_signal);
#   endif
    
#   if defined(SIGQUIT)
#       if (IGNORE_PARENT_SIGQUIT)
    SIGNAL(SIGQUIT, SIG_IGN);
#       else
    SIGNAL(SIGQUIT, Rterm_quit_signal);
#       endif
#   endif
    
#endif /* CHECK_FOR_JOB_CONTROL_SHELL */
    
    SIGNAL(SIGILL, Rterm_quit_signal);
#if 0 /* FIXME */
    SIGNAL(SIGABRT, Rterm_quit_signal);
#endif
    SIGNAL(SIGABRT, SIG_DFL);
    
#if defined(SIGBUS)
    SIGNAL(SIGBUS, Rterm_quit_signal);
#endif
    
    SIGNAL(SIGFPE, Rterm_quit_signal);
#if 0 /* FIXME */
    SIGNAL(SIGSEGV, Rterm_quit_signal);
#endif
    SIGNAL(SIGSEGV, SIG_DFL);
    
    SIGNAL(SIGTERM, Rterm_exit_signal);
    
#if defined(SIGSYS)
    SIGNAL(SIGSYS, Rterm_quit_signal);
#endif

#if defined(SIGCHLD)
    SIGNAL(SIGCHLD, Rterm_child_signal);
#endif

    term->pid = getpid();

    for (i = 0 ; i < RTERM_TABS ; i++) {
        Rterm_start_tab(term, i);
    }

//    ptyfd = term->screens[0]->pty.masterfd;
    
//    return ptyfd;

    return 0;
}

int
Rterm_init_master_pty(struct R_term *term, struct R_termscreen *screen)
{
    struct R_termscreen *screen2;
    int ptyfd;

    if (term == NULL) {

        return -1;
    }

    ptyfd = R_open_pty_master(&(screen->pty.mastername),
                              &(screen->pty.slavename),
                              TRUE);
    if (ptyfd < 0) {

	return -1;
    }

    screen->pty.masterfd = ptyfd;
//    screen2 = term->screens[(term->curscreen + term->nscreens)];
//    screen2->pty.slavefd = ptyfd;

    term->nfds = MAX(term->nfds, ptyfd);
    term->nfds = MAX(term->nfds, term->xconnfd) + 1;

    if (lstat(screen->pty.slavename, &(screen->pty.stat)) < 0) {
        fprintf(stderr, "lstat failed\n");

	return -1;
    }

    return ptyfd;
}

int
Rterm_run_command_child(struct R_term *term, struct R_termscreen *screen)
{
    struct R_app *app;
    int consolefd;
    size_t len;
    char *shellname, *shellbasename, *loginshellname;

    app = R_global.app;

    SIGNAL(SIGHUP, Rterm_exit_signal);
    SIGNAL(SIGINT, SIG_DFL);
    SIGNAL(SIGQUIT, SIG_DFL);
    SIGNAL(SIGCHLD, SIG_DFL);
    SIGNAL(SIGSEGV, SIG_DFL);
    SIGNAL(SIGBUS, SIG_DFL);
    SIGNAL(SIGABRT, SIG_DFL);
    SIGNAL(SIGFPE, SIG_DFL);
    SIGNAL(SIGILL, SIG_DFL);
#if defined(SIGSYS)
    SIGNAL(SIGSYS, SIG_DFL);
#endif
    SIGNAL(SIGALRM, SIG_DFL);
    
//#if (USE_JOB_CONTROL)
    SIGNAL(SIGTSTP, SIG_IGN);
    SIGNAL(SIGTTIN, SIG_IGN);
    SIGNAL(SIGTTOU, SIG_IGN);
//#endif
    SIGNAL(SIGCHLD, SIG_DFL);

//    term->screen = screen;
    if (R_init_pty_slave(&(term->user), screen->pty.slavename, term->nfds)
	< 0) {
	fprintf(stderr, "%s: failed to initialize slave pty %s: %s\n",
		app->name, screen->pty.slavename, strerror(errno));
	
	exit(EXIT_FAILURE);
    }
    
    PTY_SET_MODE(STDIN_FILENO, &(screen->pty.mode));
    
    chdir(term->path);
    
    if (term->flags & RTERM_CONSOLE) {
	user_enable_privileges(&(term->user));
	
#if defined(SRIOCSREDIR)
	if ((consolefd = open(RTERM_CONSOLE_DEVICE, O_WRONLY)) < 0
	    || PTY_IOCTL(consolefd, SRIOCSREDIR, 0) < 0) {
	    if (consolefd >= 0) {
		close(consolefd);
	    }
	}
#elif defined(TIOCCONS)
	PTY_IOCTL(0, TIOCCONS, &consolefd);
#endif /* SRIOCSREDIR */
	
    }
    
    user_disable_privileges(&(term->user));

    Rterm_set_winsize(term, screen->rows,
                      screen->columns, STDIN_FILENO);
    
    if ((shellname = getenv("SHELL")) == NULL || shellname[0] == '\0') {
	shellname = "/bin/sh";
    }
    
    if ((shellbasename = strrchr(shellname, '/')) == NULL) {
	shellbasename = shellname;
    } else {
	shellbasename++;
    }
    
    if (term->flags & RTERM_LOGIN_SHELL) {
	len = strlen(shellbasename);
	
	if ((loginshellname = calloc(1, len + 2))
	    == NULL) {
	    
	    return -1;
	}
	
	loginshellname[0] = '-';
	
	strncpy(&(loginshellname[1]), shellbasename, len);
	
	if (execlp(shellname, loginshellname, NULL) < 0) {
	    fprintf(stderr, "%s: failed to execute login shell \"%s\": %s\n",
		    app->name, shellname, strerror(errno));
	    
	    return -1;
	}
    } else {
	if (execlp(shellname, shellbasename, NULL) < 0) {
	    fprintf(stderr, "%s: failed to execute shell \"%s\": %s\n",
		    app->name, shellname, strerror(errno));
	    
	    return -1;
	}
    }
    
    return 0;
}

void
Rterm_set_font_flag(struct R_term *term)
{
    struct R_termscreen *screen;

    if (term == NULL) {

	return;
    }

    screen = term->screens[term->curscreen];

    screen->textflags &= ~RTERM_CHAR_FONT_MASK;

    switch (term->charsets[(term->charset)]) {
        case RTERM_CHARSET_DEC_LINE:
	    screen->textflags &= RTERM_CHAR_ACS_FONT;

	    break;
        case RTERM_CHARSET_UK:
	    screen->textflags &= RTERM_CHAR_UK_FONT;

	    break;
	    /* TODO: implement these. */
        case RTERM_CHARSET_US:

	    break;
        case RTERM_CHARSET_MULTINATIONAL:

	    break;
        case RTERM_CHARSET_FINNISH:

	    break;
        case RTERM_CHARSET_FINNISH2:

	    break;
        case RTERM_CHARSET_GERMAN:

	    break;
        default:

	    break;
    }

    return;
}

void
Rterm_set_charset(struct R_term *term, int set, char ch)
{
    if (term == NULL || ch == EOF) {
	
	return;
    }
    
#if (USE_DUAL_BYTE_CHARACTERS)
    if (set < 0 && set != RTERM_CHARSET_KANJI) {

	return;
    }
    
    if (set == RTERM_CHARSET_KANJI) {
	term->dualbyte = TRUE;

	set = -RTERM_CHARSET_KANJI;
    }
#endif
    
    if (set < 0 || set > RTERM_MAX_CHARSET) {
	
	return;
    }
    
    term->charsets[set] = ch;

    Rterm_set_font_flag(term);

    return;
}

void
Rterm_choose_charset(struct R_term *term, int set)
{
    if (term == NULL
	|| set < 0
	|| set > RTERM_MAX_CHARSET) {

	return;
    }

    term->charset = set;

    Rterm_set_font_flag(term);

    return;
}

void
Rterm_get_modifier_keys(struct R_term *term)
{
    struct R_app *app;
    int i, j, k, l, match;
    XModifierKeymap *modmap;
    KeyCode *keycodes;
    int modmasks[] = { Mod1Mask, Mod2Mask, Mod3Mask, Mod4Mask, Mod5Mask };

    app = R_global.app;

    modmap = XGetModifierMapping(app->display);
    keycodes = modmap->modifiermap;

    for (i = Mod5MapIndex ; i >= Mod1MapIndex ; i--) {
	k = i * modmap->max_keypermod;

	l = i - Mod1MapIndex;

	match = 0;

	for (j = 0 ; j < modmap->max_keypermod ; j++, k++) {
	    if (keycodes[k] == 0) {

		break;
	    }

	    switch(XKeycodeToKeysym(app->display,
				    keycodes[k], 0)) {
	        case XK_Meta_L:
	        case XK_Meta_R:
		    match = term->metamask = modmasks[l];

		    break;
	        case XK_Alt_L:
	        case XK_Alt_R:
		    match = term->altmask = modmasks[l];

		    break;
	        case XK_Num_Lock:
		    match = term->numlockmask = modmasks[l];

		    break;
	    }

	    if (match) {

		break;
	    }
	}
    }

    XFreeModifiermap(modmap);

    if (term->metamask == 0) {
	if (term->altmask != 0) {
	    /* default Meta key to match Alt mask. */

	    term->metamask = term->altmask;
	} else {
	    /* default Meta key to match Mod1Mask. */

	    term->metamask = Mod1Mask;
	}
    }

    if (term->altmask == 0) {
	/* default Alt key to match Meta mask. */

	term->altmask = term->metamask;
    }

    return;
}

void
Rterm_parse_cmdline(struct R_term *term, int argc, char *argv[])
{
    int ndx;

    ndx = 1;
    while (ndx < argc) {
        ndx++;
    }
}

int
Rterm_run(int argc, char *argv[], long flags)
{
    struct R_app app;
    struct R_term term;

    memset(&app, 0, sizeof(app));
    memset(&term, 0, sizeof(term));
    app.name = "Rterm";
    term.flags |= flags;
    term.ppid = getpid();
    if (!R_init(&app, argc, argv)) {

        exit(1);
    }
    R_global.app = &app;
    if (!Rterm_init(&term, argc, argv, RTERM_DEFAULT_FLAGS)) {
        
        return EXIT_FAILURE;
    }

    return (Rterm_main(&term, argc, argv));
}

int
Rterm_main(struct R_term *term)
{
    int retval;
    pid_t pid;

    SIGNAL(SIGHUP, Rterm_exit_signal);
    SIGNAL(SIGINT, SIG_DFL);
    SIGNAL(SIGQUIT, SIG_DFL);
    SIGNAL(SIGCHLD, SIG_DFL);
    SIGNAL(SIGSEGV, SIG_DFL);
    SIGNAL(SIGBUS, SIG_DFL);
    SIGNAL(SIGABRT, SIG_DFL);
    SIGNAL(SIGFPE, SIG_DFL);
    SIGNAL(SIGILL, SIG_DFL);
#if defined(SIGSYS)
    SIGNAL(SIGSYS, SIG_DFL);
#endif
    SIGNAL(SIGALRM, SIG_DFL);
    
//#if (USE_JOB_CONTROL)
    SIGNAL(SIGTSTP, SIG_DFL);
    SIGNAL(SIGTTIN, SIG_IGN);
    SIGNAL(SIGTTOU, SIG_IGN);
//#endif
    SIGNAL(SIGCHLD, SIG_DFL);

    if (!Rterm_start(term)) {

        return -1;
    }

    fprintf(stderr, "PARENT: %ld\n", (long)getppid());

    if (Rterm_run_command(term, NULL) < 0) {
            
        return -1;
    }
#if (USE_ATEXIT)
    if (atexit(Rterm_exit) < 0) {
        
        return -1;
    }
#elif ((USE_ON_EXIT) || defined(sun))
    if (on_exit(Rterm_exit, NULL) < 0) {
        
        return -1;
    }
#endif /* USE_ATEXIT */

    retval = Rterm_loop(term);

    /* NOTREACHED */
    exit(retval);
}

#if 0
void Rterm_resize(struct R_term *term, int rows, int columns)
{
    struct R_termscreen *screen;
    int i;

    screen = term->screens[term->curscreen];
    R_resize_window(term->window,
                    columns * screen->charw,
                    rows * screen->charh);
    for (i = 0 ; i < term->nscreens ; i++) {
        screen = term->screens[i];
        Rterm_resize_screen(screen, rows, columns);
        
//            fprintf(stderr, "rows: %d, columns: %d\n", rows, columns);
        Rterm_set_winsize(term, rows, columns, screen->pty.masterfd);
            
#if (!SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
        R_render_image_imlib2(term->window->images[R_WINDOW_BACKGROUND_IMAGE],
                              screen->window,
                              term->window->images[R_WINDOW_BACKGROUND_IMAGE]->w,
                              term->window->images[R_WINDOW_BACKGROUND_IMAGE]->h,
                              0);
#else
        if (!Rterm_load_screen_draw_image(screen)) {
            R_render_image_imlib2(&screen->drawimage,
                                  screen->window,
                                  R_TILE_IMAGE,
                                  R_TILE_IMAGE,
                                  0);
        }
#endif
    }

    return;
}
#endif
