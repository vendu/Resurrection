/*
 * scrollbar.c - scrollbar routines for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#if (RTERM_SUPPORT_SCROLLBAR)

struct R_termscrollbar *
Rterm_alloc_scrollbar(void)
{ 
    return (calloc(1, sizeof(struct R_termscrollbar)));
}

struct R_termscrollbar *
Rterm_add_scrollbar(struct R_term *term)
{
    struct R_termscrollbar *newscrollbar;

    if (term == NULL
	|| (term->scrollbar)) {

	return NULL;
    }

    if ((newscrollbar = Rterm_alloc_scrollbar()) == NULL) {

	return NULL;
    }

    term->scrollbar = newscrollbar;

    return newscrollbar;
}

void
Rterm_map_scrollbar(struct R_term *term)
{
    struct R_termscrollbar *scrollbar;

    scrollbar = term->scrollbar;
    R_map_subwindows(scrollbar->window);
    R_map_window_raised(scrollbar->window);

    return;
}

#if (USE_IMLIB2)
void
Rterm_load_scrollbar_images(struct R_term *term)
{
    struct R_app *app;
    struct R_termscrollbar *scrollbar;

    app = R_global.app;
    scrollbar = term->scrollbar;
    R_load_image_imlib2(app,
                        RESURRECTION_IMAGE_SEARCH_PATH "misc/hbar_bronze.png",
                        scrollbar->anchor->images[R_WINDOW_BACKGROUND_IMAGE]);
    R_load_image_imlib2(app,
                        RESURRECTION_IMAGE_SEARCH_PATH "misc/hbar_silver.png",
                        scrollbar->anchor->images[R_WINDOW_SELECTED_IMAGE]);
    R_load_image_imlib2(app,
                        RESURRECTION_IMAGE_SEARCH_PATH "misc/hbar_gold.png",
                        scrollbar->anchor->images[R_WINDOW_CLICKED_IMAGE]);
    R_set_image_border_imlib2(scrollbar->anchor->images[R_WINDOW_BACKGROUND_IMAGE],
                              2, 2, 2, 2);
    R_set_image_border_imlib2(scrollbar->anchor->images[R_WINDOW_SELECTED_IMAGE],
                              2, 2, 2, 2);
    R_set_image_border_imlib2(scrollbar->anchor->images[R_WINDOW_CLICKED_IMAGE],
                              2, 2, 2, 2);

    scrollbar->anchor->images[R_WINDOW_BACKGROUND_IMAGE]->h = -1;
    scrollbar->anchor->images[R_WINDOW_SELECTED_IMAGE]->h = -1;
    scrollbar->anchor->images[R_WINDOW_CLICKED_IMAGE]->h = -1;

    R_load_image_imlib2(app,
                        RESURRECTION_IMAGE_SEARCH_PATH "misc/arrow_up.png",
                        scrollbar->up->images[R_WINDOW_BACKGROUND_IMAGE]);
    R_load_image_imlib2(app,
                        RESURRECTION_IMAGE_SEARCH_PATH "misc/arrow_down.png",
                        scrollbar->down->images[R_WINDOW_BACKGROUND_IMAGE]);

    return;
}
#endif

void
Rterm_set_scrollbar_event_handlers(struct R_termscrollbar *scrollbar)
{
    if (scrollbar == NULL) {

	return;
    }

    R_set_window_event_handler(scrollbar->window, ButtonPress,
                               Rterm_scrollbar_buttonpress);
    R_set_window_event_handler(scrollbar->window, ButtonRelease,
                               Rterm_scrollbar_buttonrelease);
    R_set_window_event_handler(scrollbar->window, LeaveNotify,
                               Rterm_scrollbar_leavenotify);

    R_add_window_events(scrollbar->window,
                        ButtonPressMask
                        | ButtonReleaseMask
                        | OwnerGrabButtonMask
                        | LeaveWindowMask);
    
    R_set_window_event_handler(scrollbar->anchor, ButtonPress,
                               Rterm_scrollbar_anchor_buttonpress);
    R_set_window_event_handler(scrollbar->anchor, ButtonRelease,
                               Rterm_scrollbar_anchor_buttonrelease);
    R_set_window_event_handler(scrollbar->anchor, EnterNotify,
                               Rterm_scrollbar_anchor_enternotify);
    R_set_window_event_handler(scrollbar->anchor, LeaveNotify,
                               Rterm_scrollbar_anchor_leavenotify);
    R_set_window_event_handler(scrollbar->anchor, MotionNotify,
                               Rterm_scrollbar_anchor_motionnotify);
    
    R_add_window_events(scrollbar->anchor, ButtonPressMask
                        | ButtonReleaseMask
                        | OwnerGrabButtonMask
                        | EnterWindowMask | LeaveWindowMask
                        | PointerMotionHintMask
                        | Button1MotionMask);
    
    R_set_window_event_handler(scrollbar->up, ButtonPress,
                               Rterm_scrollbar_up_buttonpress);
    R_set_window_event_handler(scrollbar->up, ButtonRelease,
                               Rterm_scrollbar_up_buttonrelease);
    R_set_window_event_handler(scrollbar->up, EnterNotify,
                               Rterm_scrollbar_up_enternotify);
    R_set_window_event_handler(scrollbar->up, LeaveNotify,
                               Rterm_scrollbar_up_leavenotify);
    
    R_add_window_events(scrollbar->up, ButtonPressMask
                        | ButtonReleaseMask
                        | OwnerGrabButtonMask
                        | EnterWindowMask | LeaveWindowMask);
    
    R_set_window_event_handler(scrollbar->down, ButtonPress,
                               Rterm_scrollbar_down_buttonpress);
    R_set_window_event_handler(scrollbar->down, ButtonRelease,
                               Rterm_scrollbar_down_buttonrelease);
    R_set_window_event_handler(scrollbar->down, EnterNotify,
                               Rterm_scrollbar_down_enternotify);
    R_set_window_event_handler(scrollbar->down, LeaveNotify,
                               Rterm_scrollbar_down_leavenotify);
    
    R_add_window_events(scrollbar->down, ButtonPressMask
                        | ButtonReleaseMask
                        | OwnerGrabButtonMask
                        | EnterWindowMask | LeaveWindowMask);

    fprintf(stderr, "EVENTS\n");
    
    return;
}

int
Rterm_init_scrollbar_windows(struct R_term *term)
{
    int dummyi;
    unsigned int w, h, dummyui;
    struct R_app *app;
    struct R_termscrollbar *scrollbar;
    Window dummywin;

    if (term == NULL
	|| (scrollbar = term->scrollbar) == NULL) {
        fprintf(stderr, "#0\n");

	return -1;
    }

    app = R_global.app;

#if 0
    XGetGeometry(app->display, term->window->id, &dummywin,
		 &dummyi, &dummyi,
		 &w, &h,
		 &dummyui, &dummyui);
#endif

    w = Rterm_screen_width(term->screens[term->curscreen]) + RTERM_SCROLLBAR_DEFAULT_WIDTH;
    h = Rterm_screen_height(term->screens[term->curscreen]);
    if ((scrollbar->window = R_create_window(app,
                                             term->window,
                                             0))
        == NULL) {
                                             
        fprintf(stderr, "#1\n");

	return -1;
    }
    R_add_window(scrollbar->window);

    if ((scrollbar->anchor = R_create_window(app,
                                             scrollbar->window,
                                             0))
        == NULL) {
        fprintf(stderr, "#2\n");
	
	return -1;
    }
    R_add_window(scrollbar->anchor);

    /* FIXME: only do save unders when there's no trough. */
#if 0    
    window_set_save_under(scrollbar->anchor);
#endif
    
    if ((scrollbar->up = R_create_window(app,
                                         scrollbar->window,
                                         0))
	== NULL) {
        fprintf(stderr, "#3\n");
	
	return -1;
    }
    R_add_window(scrollbar->up);
    
    if ((scrollbar->down = R_create_window(app,
                                           scrollbar->window,
                                           0))
	== NULL) {
        fprintf(stderr, "#4\n");
	
	return -1;
    }
    R_add_window(scrollbar->down);

#if 0
    scrollbar_set_screen(scrollbar, term->screens[term->curscreen]);
#endif
    
    R_move_resize_window(scrollbar->window,
                         ((term->privmodes & RTERM_SCROLLBAR_RIGHT)
                          ? w - RTERM_SCROLLBAR_DEFAULT_WIDTH
                          : 0),
                         ((term->privmodes & RTERM_PRIVMODE_MENUBAR)
                          ? RTERM_MENU_DEFAULT_HEIGHT
                          : 0),
                         RTERM_SCROLLBAR_DEFAULT_WIDTH,
                         h);
    
    R_move_resize_window(scrollbar->anchor,
                         0,
                         RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT,
                         RTERM_SCROLLBAR_DEFAULT_WIDTH,
                         h - 2 * RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT);
    
    R_move_resize_window(scrollbar->up,
                         0,
                         0,
                         RTERM_SCROLLBAR_DEFAULT_ARROW_WIDTH,
                         RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT);
    
    R_move_resize_window(scrollbar->down,
                         0,
                         h - RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT,
                         RTERM_SCROLLBAR_DEFAULT_ARROW_WIDTH,
                         RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT);
    
#if (USE_IMLIB2)
    Rterm_load_scrollbar_images(term);
    
    R_set_background_imlib2(scrollbar->up->images[R_WINDOW_BACKGROUND_IMAGE],
                            scrollbar->up,
                            0,
                            0,
                            0);
    R_set_background_imlib2(scrollbar->down->images[R_WINDOW_BACKGROUND_IMAGE],
                            scrollbar->down,
                            0,
                            0,
                            0);
#endif

    Rterm_set_scrollbar_event_handlers(scrollbar);

    return 0;
}

int
Rterm_init_scrollbar_cursor(struct R_termscrollbar *scrollbar)
{
    struct R_app *app;
    Cursor cursor;

    if (scrollbar == NULL) {

	return -1;
    }

    app = R_global.app;

    cursor = XCreateFontCursor(app->display, XC_left_ptr);

    XDefineCursor(app->display, scrollbar->window->id, cursor);
    XDefineCursor(app->display, scrollbar->anchor->id, cursor);
    XDefineCursor(app->display, scrollbar->up->id, cursor);
    XDefineCursor(app->display, scrollbar->down->id, cursor);

    XFreeCursor(app->display, cursor);

    return 0;
}

/* FIXME: should free memory on failure. */
int
Rterm_init_scrollbar(struct R_term *term)
{
    if (term == NULL
	|| ((term->scrollbar)
	    && (term->scrollbar->flags & RTERM_SCROLLBAR_INITIALIZED))) {
        fprintf(stderr, "FAIL1\n");

	return -1;
    }

    if (Rterm_add_scrollbar(term) == NULL) {
        fprintf(stderr, "FAIL2\n");

	return -1;
    }
    fprintf(stderr, "added scrollbar\n");

    if (Rterm_init_scrollbar_windows(term) < 0) {

	return -1;
    }
    fprintf(stderr, "initialized scrollbar windows\n");

    if (Rterm_init_scrollbar_cursor(term->scrollbar) < 0) {
        fprintf(stderr, "FAIL3\n");

	return -1;
    }
    fprintf(stderr, "initialized scrollbar cursor\n");

    Rterm_map_scrollbar(term);

    term->scrollbar->flags |= RTERM_SCROLLBAR_INITIALIZED | RTERM_SCROLLBAR_REFRESHED;

    return 0;
}

void
Rterm_move_scrollbar(struct R_termscrollbar *scrollbar, int x, int y)
{
    R_move_window(scrollbar->window, x, y);

    return;
}

void
Rterm_update_scrollbar(struct R_termscrollbar *scrollbar)
{
    struct R_app *app;
    struct R_term *term;
    int anchorheight, dummyi;
    unsigned int h, dummyui;
    double pixelsperrow;
    struct R_termscreen *screen;
    Window dummywin;
    void (*refresh)(struct R_termscreen *, int);

    if (scrollbar == NULL || !scrollbar->window->id || !scrollbar->anchor) {

	return;
    }

    app = R_global.app;
    term = app->client;
    screen = term->screens[term->curscreen];
    refresh = screen->funcs.refresh;
    if (refresh == NULL) {

	return;
    }

    XGetGeometry(app->display, scrollbar->window->id,
		 &dummywin, &dummyi, &dummyi, &dummyui, &h,
		 &dummyui, &dummyui);

    pixelsperrow = (double)(h - 2 * RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT)
	/ (screen->nsavelines + screen->rows);

    scrollbar->anchortop = (screen->viewrow
			    - (screen->savelines - screen->nsavelines))
	* pixelsperrow
	+ RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT;
    scrollbar->anchortop = MIN(scrollbar->anchortop,
			       h - RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT - 8);

    scrollbar->anchorbottom = scrollbar->anchortop
	+ screen->rows * pixelsperrow;
    scrollbar->anchorbottom = MAX(scrollbar->anchorbottom,
				  scrollbar->anchortop + 7);

    anchorheight = scrollbar->anchorbottom - scrollbar->anchortop + 1;

    R_move_resize_window(scrollbar->anchor, 0, scrollbar->anchortop,
                         RTERM_SCROLLBAR_DEFAULT_WIDTH, anchorheight);

#if (USE_IMLIB2)
    if (!(scrollbar->flags & RTERM_SCROLLBAR_ANCHOR_PRESSED)) {
	R_set_background_imlib2(scrollbar->anchor->images[R_WINDOW_BACKGROUND_IMAGE],
                                scrollbar->anchor,
                                -1,
                                -1,
                                0);
                                
    }
#endif

    Rterm_touch_screen(screen);

    refresh(screen, RTERM_SCREEN_DRAW_ALL);

    scrollbar->flags |= RTERM_SCROLLBAR_REFRESHED;

    return;
}

void
Rterm_resize_scrollbar(struct R_term *term, int width, int height)
{
    struct R_termscrollbar *scrollbar;
    unsigned int w, dummyui;
    int dummyi;
    struct R_app *app;
    struct R_window *parent;
    Window dummywin;

    scrollbar = term->scrollbar;
    if (scrollbar == NULL
	|| width <= 0
	|| height <= 0) {

	return;
    }

    app = R_global.app;

    fprintf(stderr, "resize_scrollbar: %dx%d\n", width, height);

    R_move_resize_window(scrollbar->window,
                         ((term->flags & RTERM_SCROLLBAR_RIGHT)
                          ? width - RTERM_SCROLLBAR_DEFAULT_WIDTH
                          : 0),
                         ((term->privmodes & RTERM_PRIVMODE_MENUBAR)
                          ? RTERM_MENU_DEFAULT_HEIGHT
                          : 0), 
                         RTERM_SCROLLBAR_DEFAULT_WIDTH,
                         height);

    R_move_resize_window(scrollbar->anchor,
                         0,
                         RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT,
                         RTERM_SCROLLBAR_DEFAULT_WIDTH,
                         height - 2 * RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT);

    R_move_resize_window(scrollbar->up,
                         0,
                         0,
                         RTERM_SCROLLBAR_DEFAULT_WIDTH,
                         RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT);

    R_move_resize_window(scrollbar->down,
                         0,
                         height - RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT,
                         RTERM_SCROLLBAR_DEFAULT_WIDTH,
                         RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT);

#if 0
    R_set_background_imlib2(scrollbar->window->images[R_WINDOW_BACKGROUND_IMAGE],
                            scrollbar->window,
                            -1,
                            -1,
                            0);
#endif
#if (USE_IMLIB2)
    R_set_background_imlib2(scrollbar->anchor->images[R_WINDOW_BACKGROUND_IMAGE],
                            scrollbar->anchor,
                            -1,
                            -1,
                            0);
    R_set_background_imlib2(scrollbar->up->images[R_WINDOW_BACKGROUND_IMAGE],
                            scrollbar->up,
                            0,
                            0,
                            0);
    R_set_background_imlib2(scrollbar->down->images[R_WINDOW_BACKGROUND_IMAGE],
                            scrollbar->down,
                            0,
                            0,
                            0);
#endif

    Rterm_update_scrollbar(scrollbar);

    return;
/*
 * NOTE: everything below really belongs into Rterm/scrollbar.c and it should
 * only be called through function pointers from the scrollbar structure...
 */

}

#if 0
void
scrollbar_set_screen(struct R_termscrollbar *scrollbar, void *screen)
{
    struct R_termscreen *screen1;

    if (scrollbar == NULL
	|| screen == NULL) {

	return;
    }

#if 0
    scrollbar->window->screen = screen;
    scrollbar->anchor->screen = screen;
    scrollbar->up->screen = screen;
    scrollbar->down->screen = screen;
#endif

    screen1 = scrollbar->window->screen;
    screen1->window->scrollbar = scrollbar;
    
    return;
}
#endif

void
Rterm_show_scrollbar(struct R_term *term)
{
    int x, y;
    struct R_termscreen *screen;

    if (term == NULL) {

	return;
    }

    screen = term->screens[term->curscreen];

    if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
	Rterm_init_scrollbar(term);

#if 0	
	scrollbar_set_screen(term->scrollbar, screen);
#endif

	x = (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) && !(term->flags & RTERM_SCROLLBAR_RIGHT) ? RTERM_SCROLLBAR_DEFAULT_WIDTH : 0;
	y = (term->privmodes & RTERM_PRIVMODE_MENUBAR)
	    ? RTERM_MENU_DEFAULT_HEIGHT
	    : 0;
	R_move_window(term->screens[term->curscreen]->window, x, y);
	term->screens[term->curscreen]->window->x = x;
	term->screens[term->curscreen]->window->y = y;
	Rterm_resize(term, screen->rows, screen->columns);
    } else {
	R_unmap_window(term->scrollbar->window);
	
	R_move_window(term->screens[term->curscreen]->window,
                      0,
                      ((term->privmodes & RTERM_PRIVMODE_MENUBAR)
                       ? RTERM_MENU_DEFAULT_HEIGHT
                       : 0));
	
	Rterm_resize(term, screen->rows, screen->columns);
    }

    return;
}

#if 0
void
term_move_scrollbar(struct R_term *term)
{
    unsigned int winw, dummyui;
    int dummyi;
    Window dummywin;

    if (term->flags & RTERM_SCROLLBAR_RIGHT) {
	XGetGeometry(term->window->app->display,
		     term->window->id,
		     &dummywin,
		     &dummyi, &dummyi, &winw, &dummyui,
		     &dummyui, &dummyui);
    }

    R_move_window(term->screens[term->curscreen]->window,
                  ((term->flags & RTERM_SCROLLBAR_RIGHT)
                   ? 0
                   : RTERM_SCROLLBAR_DEFAULT_WIDTH),
                  ((term->privmodes & RTERM_PRIVMODE_MENUBAR)
                   ? RTERM_MENU_DEFAULT_HEIGHT
                   : 0));
    Rterm_move_scrollbar(term->scrollbar,
                         ((term->flags & RTERM_SCROLLBAR_RIGHT)
                          ? winw - RTERM_SCROLLBAR_DEFAULT_WIDTH
                          : 0),
                         ((term->privmodes & RTERM_PRIVMODE_MENUBAR)
                          ? RTERM_MENU_DEFAULT_HEIGHT
                          : 0));

    return;
}
#endif

#if 0
void
term_resize_scrollbar(struct R_term *term)
{
    Rterm_resize_scrollbar(term->scrollbar,
                           RTERM_SCROLLBAR_DEFAULT_WIDTH,
                           Rterm_screen_height(term->screens[term->curscreen]));

    return;
}
#endif

void
term_floating_scrollbar(struct R_term *term)
{
#if (USE_IMLIB2)
    R_set_background_imlib2(term->scrollbar->window->images[R_WINDOW_BACKGROUND_IMAGE],
                            term->scrollbar->window,
                            -1,
                            -1,
                            0);
#endif

    return;
}

#endif /* RTERM_SUPPORT_SCROLLBAR */

