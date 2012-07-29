/*
 * action.c - widget actions for Resurrection.
 * Copyright (C) 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#if 0
void
action_exec(Rwindow_t *window, Roption_t *option)
{
    pid_t pid;

    pid = fork();
    if (pid < 0) {

	return;
    }

    if (pid == 0) {
	/* child process */
	if (option->optionstr) {
	    execlp(option->commandstr, option->commandstr,
		   option->optionstr, NULL);
	} else {
	    execlp(option->commandstr, option->commandstr, NULL);
	}

	return;
    }

    return;
}

void
action_exit(Rwindow_t *window, Roption_t *option)
{
    if (window == NULL
	|| option == NULL) {

	return;
    }

    exit(0);
}

void
action_tty_string(Rwindow_t *window, Roption_t *option)
{
    Rterm_t *term;
    int (*queue_input)();

    term = Rglobals.term;
    if (term) {
	queue_input = term->funcs.queue_input;
	if (queue_input == NULL) {

	    return;
	}
	queue_input(term, option->value.str, strlen(option->value.str));
    }

    return;
}

void
action_tty_echo(Rwindow_t *window, Roption_t *option)
{
    Rterm_t *term;
    Rtermscreen_t *screen;

    term = Rglobals.term;
    if (term) {
	screen = term->window->screen;
	
	pty_write(screen->ptyfd,
		  option->value.str,
		  strlen(option->value.str));
    }

    return;
}

#if 0
#if (!(USE_IMLIB2))
void
action_screenshot(Rwindow_t *window, Roption_t *option)
{
    int screenw, screenh;
    Screen *screenptr;
    ImlibImage *imlibimage;

    screenptr = ScreenOfDisplay(window->app->display,
				DefaultScreen(window->app->display));
    screenw = WidthOfScreen(screenptr);
    screenh = HeightOfScreen(screenptr);

    sleep(2);

    XGrabServer(window->app->display);

    imlibimage = Imlib_create_image_from_drawable(window->app->imlibid,
						  DefaultRootWindow(window->app->display),
						  None,
						  0, 0,
						  screenw, screenh);

    XUngrabServer(window->app->display);

    Imlib_save_image(window->app->imlibid, imlibimage, "/tmp/screenshot.png", NULL);
    Imlib_destroy_image(window->app->imlibid, imlibimage);

    XBell(window->app->display, 50);
	
    return;
}
#endif /* !USE_IMLIB2 */
#endif

void
action_terminal(Rwindow_t *window, Roption_t *option)
{
    pid_t pid;

    pid = fork();
    if (pid == 0) {
	/* child process */
	term_main(0, NULL);
    }

    return;
}

#if (!(USE_IMLIB2))
void
action_desktop_background(Rwindow_t *window, Roption_t *option)
{
    int screen, screenw, screenh;
    int format;
    unsigned long datalen, left;
    unsigned char *data;
    Pixmap pixmap;
    Screen *screenptr;
    Atom atom, rootpmapatom, typeatom;
    Rimage_t loadimage;
    Rwindow_t *desktopwindow;

    if (window == NULL
	|| option == NULL) {

	return;
    }

    memset(&loadimage, 0, sizeof(loadimage));

    screen = DefaultScreen(window->app->display);
    screenptr = ScreenOfDisplay(window->app->display,
				DefaultScreen(window->app->display));
    screenw = WidthOfScreen(screenptr);
    screenh = HeightOfScreen(screenptr);

    if (image_load_imlib2(&loadimage, option->str) < 0) {

	return;
    }

    desktopwindow = app_get_desktop_window(window->app);

    Imlib_render(window->app->imlibid, loadimage.img, screenw, screenh);
    pixmap = Imlib_move_image(window->app->imlibid, loadimage.img);

    if (Rglobals.wm) {
	; /* FIXME */
    } else {
	atom = XInternAtom(window->app->display, "_XROOTPMAP_ID", True);
	rootpmapatom = None;
	if (atom) {
	    if (desktopwindow) {
		XGetWindowProperty(window->app->display,
				   desktopwindow->win,
				   rootpmapatom,
				   0L, 1L,
				   False,
				   AnyPropertyType,
				   &typeatom,
				   &format,
				   &datalen,
				   &left,
				   &data);
	    } else {
		XGetWindowProperty(window->app->display,
				   RootWindow(window->app->display, screen),
				   rootpmapatom,
				   0L, 1L,
				   False,
				   AnyPropertyType,
				   &typeatom,
				   &format,
				   &datalen,
				   &left,
				   &data);
	    }
	    if (typeatom == XA_PIXMAP) {
		if (data) {
#if 0
		    XKillClient(window->app->display, *((Pixmap *)data));
#endif
		    XFreePixmap(window->app->display, *((Pixmap *)data));
		}
	    }
	}
	
	atom = XInternAtom(window->app->display, "_XROOTPMAP_ID", False);
	if (atom) {
	    if (desktopwindow) {
		XChangeProperty(window->app->display,
				desktopwindow->win,
				atom,
				XA_PIXMAP,
				32,
				PropModeReplace,
				(unsigned char *)&pixmap,
				1);
	    } else {
		XChangeProperty(window->app->display,
				RootWindow(window->app->display, screen),
				atom,
				XA_PIXMAP,
				32,
				PropModeReplace,
				(unsigned char *)&pixmap,
				1);
	    }
	}
	
	if (desktopwindow) {
	    XSetWindowBackgroundPixmap(window->app->display,
				       desktopwindow->win,
				       pixmap);
	    window_clear(desktopwindow);
	} else {
	    XSetWindowBackgroundPixmap(window->app->display,
				       RootWindow(window->app->display, screen),
				       pixmap);
	    XClearWindow(window->app->display,
			 RootWindow(window->app->display, screen));
	}
	
	window->app->flags |= APP_DESKTOP_PIXMAP_IS_MINE;
	if (window->app->desktoppixmap) {
	    Imlib_free_pixmap(window->app->imlibid,
			      window->app->desktoppixmap);
	}
	window->app->desktoppixmap = pixmap;
    }

    XSync(window->app->display, False);
    image_destroy_imlib2(&loadimage);

    return;
}
#endif /* !USE_IMLIB2 */

void
action_move(Rwindow_t *window, Roption_t *option)
{

    return;
}

void
action_resize(Rwindow_t *window, Roption_t *option)
{

    return;
}

void
action_help(Rwindow_t *window, Roption_t *option)
{

    return;
}

void
action_shade(Rwindow_t *window, Roption_t *option)
{

    return;
}
#endif /* 0 */
