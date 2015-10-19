/*
 * window.c - window management for Resurrection.  Copyright (C) 2003
 * Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

void Rterm_mapnotify(void *arg, XEvent *event);
void Rterm_clientmessage(void *arg, XEvent *event);

#if 0
void
window_set_rootpmap_ids(struct R_window *window)
{
    int screen, format;
    unsigned long datalen, left;
    unsigned char *data;
    Atom rootatom, esetrootatom, typeatom;
    struct R_window *rootpmapwindow;

    if (window == NULL) {

        return;
    }

    screen = DefaultScreen(window->app->display);
    rootatom = XInternAtom(window->app->display,
			   "_XROOTPMAP_ID", False);
    esetrootatom = XInternAtom(window->app->display,
			       "ESETROOT_PMAP_ID", False);
    rootpmapwindow = NULL;
#if 0
    if (rootatom) {
        XGetWindowProperty(window->app->display,
                           window->id,
                           rootatom,
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
                           rootatom,
                           0L, 1L,
                           False,
                           AnyPropertyType,
                           &typeatom,
                           &format,
                           &datalen,
                           &left,
                           &data);
    }
#endif
    if (window->type == R_WINDOW_DESKTOP) {
	rootpmapwindow = window->desktop->window;
    } else if (window->type == R_WINDOW_ROOT) {
	rootpmapwindow = window;
    }
    rootpmapwindow->app->flags |= APP_DESKTOP_PIXMAP_IS_MINE;
    XChangeProperty(window->app->display,
		    rootpmapwindow->id,
		    rootatom,
		    XA_PIXMAP,
		    32,
		    PropModeReplace,
		    (unsigned char *)&window->desktop->pixmap,
		    1);
    XChangeProperty(window->app->display,
		    rootpmapwindow->id,
		    esetrootatom,
		    XA_PIXMAP,
		    32,
		    PropModeReplace,
		    (unsigned char *)&window->desktop->pixmap,
		    1);

    return;
}

void
window_clear(struct R_window *window)
{
    if (window == NULL) {

	return;
    }

    XClearWindow(window->app->display, window->id);
//    XFlush(window->app->display);
    XSync(window->app->display, False);

    return;
}

void
window_clear_area(struct R_window *window, int x, int y,
		  unsigned int width, unsigned int height)
{
    if (window == NULL) {

	return;
    }

    XClearArea(window->app->display, window->id, x, y, width, height, False);
//    XFlush(window->app->display);
    XSync(window->app->display, False);

    return;
}

void
window_map(struct R_window *window)
{
    if (window == NULL
	|| window->id == None) {

	return;
    }

    XMapWindow(window->app->display, window->id);
    fprintf(stderr, "mapped %d\n", window->id);

    return;
}

void
window_map_raised(struct R_window *window)
{
    if (window == NULL
	|| window->id == None) {

	return;
    }

    XMapRaised(window->app->display, window->id);

    return;
}

void
window_map_subwindows(struct R_window *window)
{
    if (window == NULL
	|| window->id == None) {

	return;
    }

    XMapSubwindows(window->app->display, window->id);
    fprintf(stderr, "mapped children of %d\n", window->id);

    return;
}

void
window_unmap(struct R_window *window)
{
    if (window == NULL
	|| window->id == None) {

	return;
    }

    XUnmapWindow(window->app->display, window->id);

    return;
}

void
window_iconify(struct R_window *window)
{
    if (window == NULL
	|| window->id == None) {

	return;
    }

    XIconifyWindow(window->app->display, window->id,
		   DefaultScreen(window->app->display));

    return;
}

void
window_withdraw(struct R_window *window)
{
    if (window == NULL
	|| window->id == None) {

	return;
    }

    XWithdrawWindow(window->app->display, window->id,
		    DefaultScreen(window->app->display));

    return;
}

void
window_reparent(struct R_window *window, struct R_window *parent, int x, int y)
{
    if (window == NULL
	|| parent == NULL) {

	return;
    }

    XReparentWindow(window->app->display,
		    window->id, parent->win,
		    x, y);

    return;
}

/*
 * FIXME: check for border width here.
 */
void
window_move(struct R_window *window, int x, int y)
{
    XEvent configureevent;

    if (window == NULL
	|| window->id == None) {

	return;
    }

    XMoveWindow(window->app->display, window->id, x, y);

    return;
}

void
R_resize_window(struct R_window *window, int w, int h)
{
    if (window == NULL
	|| window->id == None
	|| w <= 0
	|| h <= 0) {

	return;
    }

    XResizeWindow(window->app->display, window->id, w, h);

    return;
}

void
window_move_resize(struct R_window *window, int x, int y, int w, int h)
{
    if (window == NULL
	|| window->id == None
	|| w <= 0
	|| h <= 0) {

	return;
    }

    XMoveResizeWindow(window->app->display, window->id, x, y, w, h);

    return;
}

void
window_raise(struct R_window *window)
{
    if (window == NULL
	|| window->id == None) {

	return;
    }

    XRaiseWindow(window->app->display, window->id);

    return;
}

void
window_lower(struct R_window *window)
{
    if (window == NULL
	|| window->id == None) {

	return;
    }

    XLowerWindow(window->app->display, window->id);

    return;
}

void
window_kill_client(struct R_window *window)
{
    if (window == NULL
	|| window->id == None) {

	return;
    }

    XKillClient(window->app->display, window->id);

    return;
}

void
window_add_to_save_set(struct R_window *window)
{
    if (window == NULL
	|| window->id == None) {

	return;
    }

    XAddToSaveSet(window->app->display, window->id);

    return;
}

void
window_set_background_color(struct R_window *window, Rcolor_t color)
{
    if (window == NULL
	|| window->id == None) {

	return;
    }

    XSetWindowBackground(window->app->display, window->id, color);

    window_clear(window);

    XSync(window->app->display, False);

    return;
}

struct R_window *
app_get_desktop_window(Rapp_t *app)
{
    int format;
    unsigned int nchildren;
    unsigned long nitems = 0, nleft = 0;
    unsigned char *data;
    struct R_window *oldwindow, *newwindow;
    Window win, root, parent, *children;
    Atom rootpmapatom = None, rootpixelatom = None, typeatom = None;

    if (app == NULL) {

        return NULL;
    }

    rootpmapatom = XInternAtom(app->display, ROOT_PIXMAP_ATOM_STRING, True);
    rootpixelatom = XInternAtom(app->display, ROOT_PIXEL_ATOM_STRING, True);

#if 0
    if (app->desktopwindow) {
        if ((oldwindow = app_remove_window(app,
					   app->desktopwindow->id)) != NULL) {
            R_add_window_events(oldwindow, NoEventMask);
            
            free(oldwindow);
        }
        
        app->desktopwindow = NULL;
    }
#endif

    if (rootpmapatom == None && rootpixelatom == None) {
        if (app->desktopwindow) {
            if ((oldwindow = app_remove_window(app, app->desktopwindow->id))
                != NULL) {
                R_add_window_events(oldwindow, NoEventMask);
                
                free(oldwindow);
            }
            
            app->desktopwindow = NULL;
        }

        return NULL;
    }

    for (win = app->window->id ; win ; win = parent) {
        if ((XQueryTree(app->display, win, &root, &parent, &children,
                        &nchildren)) == False) {
            if (app->desktopwindow) {
                if ((oldwindow = app_remove_window(app,
						   app->desktopwindow->id))
                    != NULL) {
                    R_add_window_events(oldwindow, NoEventMask);
                    
                    free(oldwindow);
                }
                
                app->desktopwindow = NULL;
            }

            return NULL;
        }

        if (nchildren) {
            XFree(children);
        }

        if (rootpmapatom != None) {
            XGetWindowProperty(app->display, win, rootpmapatom, 0, 1, False,
                               AnyPropertyType, &typeatom, &format, &nitems,
                               &nleft, &data);
        } else if (rootpixelatom != None) {
            XGetWindowProperty(app->display, win, rootpixelatom, 0, 1, False,
                               AnyPropertyType, &typeatom, &format, &nitems,
                               &nleft, &data);
        }

        XFree(data);

        if (typeatom != None) {
            if (app_find_window(app, win) != NULL) {
                /* desktop window hasn't changed. */

                return ((struct R_window *)1);
            }

            /* desktop window has changed. */

            if (app->desktopwindow) {
                if ((oldwindow = app_remove_window(app,
						   app->desktopwindow->id))
                    != NULL) {
                    R_add_window_events(oldwindow, NoEventMask);
                    
                    free(oldwindow);
                }

                app->desktopwindow = NULL;
            }

            if ((newwindow = window_alloc()) == NULL) {

                return NULL;
            }

            newwindow->app = app;
            newwindow->id = win;

            if (app_add_window(app, newwindow) < 0) {
                /* FIXME: should be window_destroy() */
                
                free(newwindow);

                return NULL;
            }


            R_set_window_event_handler(newwindow, PropertyNotify,
                                     app_propertynotify);
            R_add_window_events(newwindow, PropertyChangeMask);

            app->desktopwindow = newwindow;

            return newwindow;
        }
    }

    return (app->desktopwindow);
}

#if 0
Rhashwindow_t *
app_alloc_hashwindow(void)
{
    return (calloc(1, sizeof(Rhashwindow_t)));
}
#endif

#endif /* 0 */

#if 0

struct R_window **
app_alloc_windowhash(void)
{
    return (calloc(WINDOW_HASH_SIZE, sizeof(struct R_window *)));
}

int
app_add_window(Rapp_t *app, struct R_window *window)
{
    unsigned long key;

    if (app == NULL
	|| window == NULL) {

	return -1;
    }

    key = R_WINDOW_HASH(window->id);
    if (app->windowhash[key]) {
	window->prev = NULL;
	window->next = app->windowhash[key];
	window->next->prev = window;
	app->windowhash[key] = window;
    } else {
	window->prev = NULL;
	window->next = NULL;
	app->windowhash[key] = window;
    }
    
    return 0;
}

struct R_window *
app_remove_window(Rapp_t *app, Window win)
{
    unsigned long key;
    struct R_window *hashwindow;

    if (app == NULL
	|| win == None) {

	return NULL;
    }

    key = R_WINDOW_HASH(win);
    hashwindow = app->windowhash[key];
    if (hashwindow) {
	if (hashwindow->id == win) {
	    app->windowhash[key] = hashwindow->next;
	    if (hashwindow->next) {
		hashwindow->next->prev = NULL;
	    }

	    return hashwindow;
	} else {
	    hashwindow = hashwindow->next;
	    while (hashwindow) {
		if (hashwindow->id == win) {
		    hashwindow->prev->next = hashwindow->next;
		    if (hashwindow->next) {
			hashwindow->next->prev = hashwindow->prev;
		    }

		    return hashwindow;
		}
		hashwindow = hashwindow->next;
	    }
	}
    }

    return NULL;
}

struct R_window *
app_find_window(Rapp_t *app, Window win)
{
    unsigned long key;
    struct R_window *hashwindow;

    if (app == NULL
        || win == None) {

        return NULL;
    }

    key = R_WINDOW_HASH(win);
    hashwindow = app->windowhash[key];
    if (hashwindow) {
	if (hashwindow->id == win) {

	    return hashwindow;
	} else {
	    hashwindow = hashwindow->next;
	    while (hashwindow) {
		if (hashwindow->id == win) {

		    return hashwindow;
		}
		hashwindow = hashwindow->next;
	    }
	}
    }

    return NULL;
}

#if 0
int
imageview_init_windows(Rimageview_t *imageview)
{
    struct R_window *window;
    int i, ncontexts;
    
    if (imageview->flags & IMAGEVIEW_BORDERLESS) {
	window_set_override_redirect(imageview->window, True);
    } else {
	R_set_window_title(imageview->window, "Rimv", TRUE);
    }
    R_set_window_event_handler(imageview->window,
			     Expose,
			     imageview_expose);
    R_set_window_event_handler(imageview->window,
                             KeyPress,
                             imageview_keypress);
    R_set_window_event_handler(imageview->window, ButtonPress,
			     imageview_buttonpress);
    R_add_window_events(imageview->window,
			 ExposureMask
                         | KeyPressMask
			 | ButtonPressMask);
    if (imageview->flags & IMAGEVIEW_CONTEXT_THUMBS) {
        imageview->contextwindow = window_create(imageview->app,
                                                 NULL,
                                                 IMAGEVIEW_THUMB_WIDTH,
                                                 ncontexts * IMAGEVIEW_THUMB_HEIGHT);
        if (imageview->contextwindow == NULL) {
            
            return -1;
        }
    }
    if (imageview->flags & IMAGEVIEW_PREVIEW_THUMBS) {
        if (imageview->flags & IMAGEVIEW_HORIZONTAL_THUMBS) {
            imageview->previewwindow = window_create(imageview->app,
                                                     NULL,
                                                     IMAGEVIEW_THUMBS * IMAGEVIEW_THUMB_WIDTH,
                                                     IMAGEVIEW_THUMB_HEIGHT);
        } else {
            imageview->previewwindow = window_create(imageview->app,
                                                     NULL,
                                                     IMAGEVIEW_THUMB_WIDTH,
                                                     IMAGEVIEW_THUMBS * IMAGEVIEW_THUMB_HEIGHT);
        }
        if (imageview->previewwindow == NULL) {
            
            return -1;
        }
    }
    if (imageview->flags & IMAGEVIEW_BORDERLESS) {
        window_set_override_redirect(imageview->contextwindow, True);
        window_set_override_redirect(imageview->previewwindow, True);
    }
    if (imageview->flags & IMAGEVIEW_CONTEXT_THUMBS) {
        imageview->contextchildren = calloc(ncontexts, sizeof(struct R_window *));
        if (imageview->contextchildren == NULL) {

            return -1;
        }
    }
    if (imageview->flags & IMAGEVIEW_PREVIEW_THUMBS) {
        imageview->previewchildren = calloc(IMAGEVIEW_THUMBS,
                                            sizeof(struct R_window *));
        if (imageview->previewchildren == NULL) {

            return -1;
        }
    }
    if (imageview->flags & IMAGEVIEW_CONTEXT_THUMBS) {
	for (i = 0 ; i < ncontexts ; i++) {
	    window = window_create(imageview->app,
                                   imageview->contextwindow,
				   IMAGEVIEW_THUMB_WIDTH,
				   IMAGEVIEW_THUMB_HEIGHT);
            if (window == NULL) {

                return -1;
            }
            window->appid = i;
	    window->imageview = imageview;
	    R_set_window_event_handler(window, ButtonPress,
				     imageview_context_buttonpress);
	    R_add_window_events(window, ButtonPressMask);
            window_move(window, 0, i * IMAGEVIEW_THUMB_HEIGHT);
	    imageview->contextchildren[i] = window;
	}
	window_move(imageview->contextwindow, 0, WM_PANEL_HEIGHT);
	window_map_subwindows(imageview->contextwindow);
    }
    if (imageview->flags & IMAGEVIEW_PREVIEW_THUMBS) {
	for (i = 0 ; i < IMAGEVIEW_THUMBS ; i++) {
	    window = window_create(imageview->app,
                                   imageview->previewwindow,
				   IMAGEVIEW_THUMB_WIDTH,
				   IMAGEVIEW_THUMB_HEIGHT);
            if (window == NULL) {

                return -1;
            }
            window->appid = i;
	    window->imageview = imageview;
	    R_set_window_event_handler(window, ButtonPress,
				     imageview_preview_buttonpress);
	    R_add_window_events(window, ButtonPressMask);
            if (imageview->flags & IMAGEVIEW_HORIZONTAL_THUMBS) {
                window_move(window, i * IMAGEVIEW_THUMB_WIDTH, 0);
            } else {
                window_move(window, 0, i * IMAGEVIEW_THUMB_HEIGHT);
            }
	    imageview->previewchildren[i] = window;
	}
        if (imageview->flags & IMAGEVIEW_CONTEXT_THUMBS) {
            window_move(imageview->previewwindow, IMAGEVIEW_THUMB_WIDTH, WM_PANEL_HEIGHT);
        } else {
            window_move(imageview->previewwindow, 0, WM_PANEL_HEIGHT);
        }
	window_map_subwindows(imageview->previewwindow);
    }
    if (imageview->flags & IMAGEVIEW_HORIZONTAL_THUMBS
        && !(imageview->flags & IMAGEVIEW_CONTEXT_THUMBS)) {
        window_move(imageview->window,
                    0,
                    WM_PANEL_HEIGHT + IMAGEVIEW_THUMB_HEIGHT);
    } else if ((imageview->flags & IMAGEVIEW_PREVIEW_THUMBS)
               && (imageview->flags & IMAGEVIEW_CONTEXT_THUMBS)) {
        window_move(imageview->window,
                    IMAGEVIEW_THUMB_WIDTH,
                    WM_PANEL_HEIGHT + IMAGEVIEW_THUMB_HEIGHT);
    } else if (imageview->flags & IMAGEVIEW_PREVIEW_THUMBS) {
        window_move(imageview->window,
                    IMAGEVIEW_THUMB_WIDTH,
                    WM_PANEL_HEIGHT);
    } else {
        window_move(imageview->window,
                    0,
                    WM_PANEL_HEIGHT);
    }
    window_map_raised(imageview->window);
    if (imageview->flags & IMAGEVIEW_CONTEXT_THUMBS) {
        window_map_raised(imageview->contextwindow);
    }
    if (imageview->flags & IMAGEVIEW_PREVIEW_THUMBS) {
        window_map_raised(imageview->previewwindow);
    }
    imageview->gc = imageview_create_gc(imageview);
    if (imageview->gc == NULL) {
        
        return -1;
    }
#if 0
    if (imageview->flags & IMAGEVIEW_CONTEXT_THUMBS) {
        for (i = 0 ; i < imageview->ncontexts ; i++) {
            imageview_render_context_thumb(imageview, i, 0);
        }
    }
    if (imageview->flags & IMAGEVIEW_PREVIEW_THUMBS) {
        imageview_render_preview_thumbs(imageview, 0, IMAGEVIEW_THUMBS);
    }
#endif

    return 0;
}

int
panel_init_windows(Rpanel_t *panel)
{
    if (panel == NULL) {

	return -1;
    }

    panel->window = panel->app->window;
    R_set_window_title(panel->window, "Rpanel", TRUE);
    window_set_override_redirect(panel->window, True);
    R_resize_window(panel->window, panel->w, panel->h);
    panel_set_normal_hints(panel);

    return 0;
}

void
panel_set_normal_hints(Rpanel_t *panel)
{
    int screenw;
    Screen *screenptr;

    if (panel == NULL) {

	return;
    }

    screenptr = ScreenOfDisplay(panel->window->app->display,
				DefaultScreen(panel->window->app->display));
    screenw = WidthOfScreen(screenptr);

    R_set_normal_hints(panel->window,
			    screenw, PANEL_DEFAULT_HEIGHT,
			    screenw, PANEL_DEFAULT_HEIGHT,
			    -1, -1,
			    -1, -1,
			    -1, -1,
			    -1, -1, -1);

    return;
}

void
panel_map_windows(Rpanel_t *panel)
{
    if (panel == NULL) {

	return;
    }

    window_map_subwindows(panel->window);
    window_map(panel->window);

    return;
}
#endif /* 0 */

#endif /* 0 */

void
Rterm_set_normal_hints(struct R_term *term)
{
    struct R_termscreen *screen;

    if (term == NULL) {

	return;
    }

    screen = term->screens[0];
    
    R_set_normal_hints(term->window, -1, -1,
			    RTERM_SCREEN_MAX_COLUMNS * screen->charw
			    + RTERM_SCROLLBAR_DEFAULT_WIDTH,
			    -1,
			    screen->charw, screen->charh,
			    -1, -1, -1, -1,
			    ((term->privmodes & RTERM_PRIVMODE_SCROLLBAR)
			     ? RTERM_SCROLLBAR_DEFAULT_WIDTH
			     : screen->charw),
			    ((term->privmodes & RTERM_PRIVMODE_MENUBAR)
			     ? RTERM_MENU_DEFAULT_HEIGHT
			     : screen->charh),
			    -1);
    
    return;
}

int
Rterm_init_windows(struct R_term *term)
{
    struct R_termscreen *screen;

    if (term == NULL) {

	return -1;
    }

#if 0
    if (term->privmodes & RTERM_PRIVMODE_MENUBAR) {
	term->window->flags |= R_WINDOW_HAS_MENUBAR;
    }
#endif

#if 0
    if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
	term->window->flags |= R_WINDOW_HAS_SCROLLBAR;
    }

    if (term->flags & RTERM_SCROLLBAR_RIGHT) {
	term->window->flags |= R_WINDOW_SCROLLBAR_RIGHT;
    }

    if (term->flags & RTERM_FLOATING_SCROLLBAR) {
	term->window->flags |= R_WINDOW_FLOATING_SCROLLBAR;
    }

    if (term->flags & RTERM_POPUP_SCROLLBAR) {
	term->window->flags |= R_WINDOW_POPUP_SCROLLBAR;
    }
#endif

    if (term->flags & RTERM_TRANSPARENT) {
#if 0
	term->window->flags |= R_WINDOW_TRANSPARENT;

	term->window->images[R_WINDOW_BACKGROUND_IMAGE].flags
	    |= IMAGE_TRANSPARENT;
#endif
    }
#if (SUPPORT_IMAGE_EFFECTS)
    else if (term->flags & RTERM_PLASMA_BACKGROUND) {
#if (USE_IMLIB2)
	term->window->images[R_WINDOW_BACKGROUND_IMAGE].flags
	    |= IMAGE_PLASMA;
#endif
    }
#endif /* SUPPORT_IMAGE_EFFECTS */
    else {
#if (!SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
#if (USE_IMLIB2)	
        R_load_image_imlib2(term->window->app,
                            RESURRECTION_IMAGE_SEARCH_PATH "background/tile_caverock.jpg",
                            term->window->images[R_WINDOW_BACKGROUND_IMAGE]);
#endif
#endif
    }
    if (term->flags & RTERM_SCALE_BACKGROUND_PIXMAP) {
	term->window->images[R_WINDOW_BACKGROUND_IMAGE]->w = -1;
	term->window->images[R_WINDOW_BACKGROUND_IMAGE]->h = -1;
    }
    
    R_set_window_title(term->window, RTERM_NAME, TRUE);
    term->wmdeletewindow = XInternAtom(term->window->app->display,
                                       "WM_DELETE_WINDOW",
                                       False);
    XSetWMProtocols(term->window->app->display,
                    term->window->id,
                    &term->wmdeletewindow,
                    1);

//    window_set_icon_name(term->window, RTERM_NAME);

#if 0
    if ((term->iconwindow = window_create(term->app, term->window, 153, 53))
	== NULL) {

	return -1;
    }
#endif

#if 0
    if (window_load_image(term->iconwindow, R_WINDOW_BACKGROUND_IMAGE,
			  "Eterm_icon.png") == 0) {
	window_set_pixmap(term->iconwindow, R_WINDOW_BACKGROUND_IMAGE);

	window_destroy_image_data(term->iconwindow, R_WINDOW_BACKGROUND_IMAGE);
    }
#endif

#if 0
    R_set_wm_hints(term->window, True, NormalState,
			None, term->iconwindow,
			-1, -1,
			None, None, -1);
#endif

    screen = term->screens[RTERM_PRIMARY_SCREEN];

    Rterm_set_normal_hints(term);

#if 0
    if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
	if (Rterm_init_scrollbar(term) < 0) {
	    
	    return -1;
	}
#if 0
	if (term->flags & RTERM_TRANSPARENT) {
	    term->scrollbar->window->images[R_WINDOW_BACKGROUND_IMAGE].flags |= IMAGE_TRANSPARENT;
	}
#endif
    }
#endif

#if 0
    window_set_pixmap(term->window, R_WINDOW_BACKGROUND_IMAGE);
#endif

    R_set_wm_hints(term->window, True, NormalState, None, None,
			-1, -1,
			None, term->window->id, False);

    R_set_class_hint(term->window, RTERM_NAME, RTERM_NAME);

    fprintf(stderr, "term->x = %d, term->y = %d\n", term->x, term->y);
    R_move_resize_window(term->window,
                         term->x,
                         term->y,
                         Rterm_screen_width(screen)
                         + ((term->privmodes & RTERM_PRIVMODE_SCROLLBAR)
                            ? RTERM_SCROLLBAR_DEFAULT_WIDTH
                            : 0),
                         Rterm_screen_height(screen)
                         + ((term->privmodes & RTERM_PRIVMODE_MENUBAR)
                            ? RTERM_MENU_DEFAULT_HEIGHT
                            : 0));

#if 0
    if (term->privmodes & RTERM_PRIVMODE_MENUBAR) {
	if (term_init_menubar(term) < 0) {
	    
	    return -1;
	}
    }
#endif

#if 0
    if (term_init_pop_menu(term) < 0) {

        return -1;
    }
#endif

#if 0
#endif

    fprintf(stderr, "EVENTS: term->window->id: %ld\n", term->window->id);
    R_set_window_event_handler(term->window, ConfigureNotify,
                               Rterm_configurenotify);
    R_set_window_event_handler(term->window,
                               EnterNotify, Rterm_enternotify);
    if (term->flags & RTERM_START_UNMAPPED) {
        R_set_window_event_handler(term->window,
                                   MapNotify, Rterm_mapnotify);
    }
    R_set_window_event_handler(term->window,
                               FocusIn, Rterm_focusin);
    R_set_window_event_handler(term->window,
                               FocusOut, Rterm_focusout);
#if 0
    R_set_window_event_handler(term->window,
                               KeyPress, Rterm_keypress);
#endif
    R_set_window_event_handler(term->window,
                               SelectionNotify, Rterm_paste_selection);
    R_set_window_event_handler(term->window,
                               SelectionRequest, Rterm_make_selection);
    R_set_window_event_handler(term->window,
                               SelectionClear, Rterm_destroy_selection);
    R_set_window_event_handler(term->window,
                               ClientMessage, Rterm_clientmessage);
    
    R_add_window_events(term->window,
                         FocusChangeMask
//                         | KeyPressMask
                         | EnterWindowMask
                         | StructureNotifyMask
                         | PropertyChangeMask);

    return 0;
}

void
Rterm_map_windows(struct R_term *term)
{
    if (term == NULL) {
	
	return;
    }

    if (!(term->flags & RTERM_START_UNMAPPED)) {
        R_map_window(term->window);
    }

//    R_map_window(term->window->screen->window);

//    R_map_window(term->screens[TERM_PRIMARY_SCREEN]->window);
    
#if 0
    if (term->privmodes & RTERM_PRIVMODE_MENUBAR) {
	R_map_window(term->menuwindow);
	
	R_map_subwindows(term->menuwindow);
    }
#endif

#if 0
    if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
	Rterm_map_scrollbar(term);
        R_move_window(term->screens[term->curscreen]->window,
                      (term->flags & RTERM_SCROLLBAR_RIGHT)
                      ? 0
                      : RTERM_SCROLLBAR_DEFAULT_WIDTH,
                      0);
    }
#endif
    R_map_window(term->screens[term->curscreen]->window);

    return;
}

void
Rterm_resize(struct R_term *term, int rows, int columns)
{
    struct R_termscreen *screen;

    if (term == NULL
	|| rows <= 0
	|| columns <= 0) {

	return;
    }

    columns = MIN(columns, RTERM_SCREEN_MAX_COLUMNS);

    screen = term->screens[term->curscreen];

    Rterm_set_normal_hints(term);
    
    R_resize_window(term->window,
                    screen->charw * columns
                    + ((term->privmodes & RTERM_PRIVMODE_SCROLLBAR)
                       ? RTERM_SCROLLBAR_DEFAULT_WIDTH
                       : 0),
                    screen->charh * rows
                    + ((term->privmodes & RTERM_PRIVMODE_MENUBAR)
                       ? RTERM_MENU_DEFAULT_HEIGHT
                       : 0));
    
    return;
}

#if defined(TIOCSWINSZ)

void
Rterm_set_winsize(struct R_term *term, int rows, int columns, int fd)
{
    struct winsize winsize;

    if (term == NULL
	|| rows <= 0
	|| columns <= 0
	|| fd < 0) {

	return;
    }

    winsize.ws_row = (unsigned short)rows;
    winsize.ws_col = (unsigned short)columns;
#ifndef __CYGWIN__
    winsize.ws_xpixel = winsize.ws_ypixel = 0;
#endif /* __CYGWIN__ */

    PTY_IOCTL(fd, TIOCSWINSZ, &winsize);

    return;
}

#elif defined(TIOCSSIZE) && defined(sun) && !(USE_SVR4)

void
Rterm_set_winsize(struct R_term *term, int rows, int columns, int fd)
{
    int pgrp;
    struct ttysize ttysize;

    ttysize.ts_lines = rows;
    ttysize.ts_cols = columns;

    PTY_IOCTL(fd, TIOCSSIZE, &ttysize);

#if defined(SIGWINCH)
    if (term->childpid > 1) {
	if (PTY_IOCTL(fd, TIOCGPGRP, &pgrp) < 0) {

	    return;
	}

	Rterm_send_signal(term, pgrp, SIGWINCH);
    }
#endif /* defined(SIGWINCH) */

    return;
}

#endif /* defined(TIOCSWINSZ) */

