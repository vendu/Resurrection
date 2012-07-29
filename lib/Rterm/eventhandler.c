/*
 * eventhandler.c - event handlers for Resurrection.
 * Copyright (C) 2003 - 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#define DEBUG_KEYSYM 0

extern volatile int Rtermtabs;

void Rterm_mapnotify(void *arg, XEvent *event);

#if 0
static struct {
    struct R_window *lastmenuwindow;
    int pressx, pressy;
    int lastx, lasty;
    int lastw, lasth;
    XSizeHints sizehints;
} globals;
#endif

#if 0
void
window_button_action(void *arg, XEvent *event, int action)
{
    R_buttonaction_t *buttonaction;

    buttonaction = window->buttonactions[action];
    if (buttonaction) {
        buttonaction(window, event);
    }
#if 0
    Rbuttonactionlist_t *list;

    if (window == NULL
	|| action < 0
        || action > R_BUTTON_ACTIONS - 1) {

        return;
    }

    list = &window->buttonactions[action];
    while (list) {
	buttonaction = list->handler;
	if (buttonaction) {
	    buttonaction(window, event);
	}
	list = list->next;
    }
#endif

    return;
}
#endif

void
window_click(void *arg, XEvent *event)
{
    struct R_window *window;
    struct timeval currenttv;

    window = arg;
    gettimeofday(&currenttv, NULL);
    if (TIMEVAL_CMP_USEC(&window->clicks.lasttv, &currenttv)
	<= R_MULTICLICK_DELAY_USEC) {
	window->clicks.clicks++;
    } else {
	window->clicks.clicks = 1;
    }
    window->clicks.lasttv.tv_sec = currenttv.tv_sec;
    window->clicks.lasttv.tv_usec = currenttv.tv_usec;

    return;
}

/* TODO: add checks for WINDOW_HAS_SCREEN etc. and support for solid color. */

#if 0
void
app_propertynotify(void *arg, XEvent *event)
{
    struct R_term *term;
    struct R_window *mainwindow, *desktopwindow;
    struct R_termscreen *screen;
    Pixmap pixmap = None;
    Atom rootpmapatom = None;
    Atom workspaceatom = None;
    void (*refresh)(struct R_termscreen *, int);
    
    if (window == NULL
	|| event == NULL) {
	
	return;
    }

    term = R_global.app->client;
    mainwindow = term->window;

    refresh = NULL;
    if (screen) {
	refresh = screen->funcs.refresh;
	if (refresh == NULL) {
	    
	    return;
	}
    }

    app_get_desktop_window(window->app);
    
    if (window == window->app->desktopwindow) {
	if (mainwindow->stateflags & R_WINDOW_TRANSPARENT_FLAG) {
	    rootpmapatom = XInternAtom(window->app->display,
				       ROOT_PIXMAP_ATOM_STRING, True);
	    if (rootpmapatom == None) {
		
		return;
	    }
	    
	    if (event->xproperty.atom == rootpmapatom) {
		pixmap = app_get_desktop_pixmap(window->app);
		if (pixmap == None || pixmap == (Pixmap)1) {
		    
		    return;
		}
		
		/* desktop pixmap has changed. */
		
		R_render_background_imlib2(window->app->window->images[R_WINDOW_BACKGROUND_IMAGE],
                    window->app->window,
                    0);

		if (screen) {
		    R_render_background_imlib2(NULL, screen->window, 0);
		    R_render_background_imlib2(NULL, term->scrollbar->window, 0);
		    
		    Rterm_touch_screen(screen);
		    
		    refresh(screen, RTERM_SCREEN_DRAW_ALL);
		}
	    }
	}
    } else {
	/* window == mainwindow */
	if (mainwindow->stateflags & R_WINDOW_TRANSPARENT_FLAG) {
	    if (event->xproperty.window == mainwindow->id) {	
		workspaceatom = XInternAtom(window->app->display,
					    WORKSPACE_ATOM_STRING, True);
		if (workspaceatom == None) {
		    
		    return;
		}
		
		if (event->xproperty.atom == workspaceatom) {
		    desktopwindow = app_get_desktop_window(window->app);
		    if (desktopwindow == NULL
			|| desktopwindow == (struct R_window *)1) {
			
			return;
		    }
		    
		    /* desktop window has changed. */
		    
		    pixmap = app_get_desktop_pixmap(window->app);
		    if (pixmap == None || pixmap == (Pixmap)1) {
			
			return;
		    }

		    /* desktop pixmap has changed. */
		    
		    R_render_background_imlib2(window->app->window->images[R_WINDOW_BACKGROUND_IMAGE],
                        window->app->window,
                        0);

		    if (screen) {
			R_render_background_imlib2(NULL, screen->window, 0);
			R_render_background_imlib2(NULL, term->scrollbar->window, 0);

			Rterm_touch_screen(screen);
			
			refresh(screen, RTERM_SCREEN_DRAW_ALL);
		    }
		    
		    return;
		}
	    }
	}
    }

    return;
}
#endif

#if 0
void
window_button_event(void *arg, XEvent *event)
{
    int action;

    if (window == NULL) {
	
	return;
    }

    action = -1;
    
    if (event->type == ButtonPress) {
	switch (event->xbutton.button) {
            case Button1:
		action = R_BUTTON1_PRESS;

		break;
            case Button2:
		action = R_BUTTON2_PRESS;

		break;
            case Button3:
		action = R_BUTTON3_PRESS;

		break;
            case Button4:
		action = R_BUTTON4_PRESS;

		break;
            case Button5:
		action = R_BUTTON5_PRESS;

		break;
            default:

		return;
	}
    } else {
	switch (event->xbutton.button) {
            case Button1:
		action = R_BUTTON1_RELEASE;

		break;
            case Button2:
		action = R_BUTTON2_RELEASE;

		break;
            case Button3:
		action = R_BUTTON3_RELEASE;

		break;
            case Button4:
		action = R_BUTTON4_RELEASE;

		break;
            case Button5:
		action = R_BUTTON5_RELEASE;

		break;
            default:

		return;
	}
    }

    window_button_action(window, event, action);

    return;
}
#endif

#if 0
void
menu_buttonpress(void *arg, XEvent *event)
{
    if (window == NULL
	|| event == NULL) {

	return;
    }

    window->app->flags |= APP_MENU_OPEN;
    if (image_render_imlib2(window->id,
                            &window->images[R_WINDOW_CLICKED_IMAGE],
                            -1, -1) < 0) {
        menu_draw_rectangle(window);
    } else {
        window_apply_pixmap(window,
                            &window->images[R_WINDOW_CLICKED_IMAGE],
                            0, 0,
                            R_WINDOW_IMAGE_TILE | R_WINDOW_IMAGE_MASK);
    }
    
#if (SUPPORT_TRUETYPE_IMLIB2)
    menu_draw_string_imlib2(window);
#else
    menu_draw_string(window);
#endif

#if 0
    window_button_event(window, event);
#endif

    return;
}

void
menu_buttonrelease(void *arg, XEvent *event)
{
    struct R_window *childwindow;
    void (*handler)();

    if (window == NULL
	|| event == NULL) {

	return;
    }

    menu_close(0);

    if (window->menu->type != WIDGET_POPMENU) {
        if (image_render_imlib2(window->id,
                                &window->images[R_WINDOW_SELECTED_IMAGE],
                                -1, -1) < 0) {
            menu_draw_rectangle(window);
        } else {
            window_apply_pixmap(window,
                                &window->images[R_WINDOW_SELECTED_IMAGE],
                                0, 0,
                                R_WINDOW_IMAGE_TILE | R_WINDOW_IMAGE_MASK);
        }
        
#if (SUPPORT_TRUETYPE_IMLIB2)
        menu_draw_string_imlib2(window);
#else
        menu_draw_string(window);
#endif
    }
    if (window->menuitem) {
	handler = window->menuitem->option.handler;
	if (handler) {
	    (*handler)(window, event);
	}
    }
    window->app->flags &= ~APP_MENU_OPEN;
    XUngrabPointer(window->app->display,
		   CurrentTime);

    return;
}

void
menu_enternotify(void *arg, XEvent *event)
{
    if (window == NULL
	|| event == NULL) {
        
	return;
    }
    
    if (window->menu->type == WIDGET_MENU) {
	if (image_render_imlib2(window->id,
				&window->images[R_WINDOW_SELECTED_IMAGE],
				-1, -1) < 0) {
	    menu_draw_rectangle(window);
	} else {
	    window_apply_pixmap(window,
				&window->images[R_WINDOW_SELECTED_IMAGE],
				0, 0,
				R_WINDOW_IMAGE_TILE | R_WINDOW_IMAGE_MASK);
	}
#if (SUPPORT_TRUETYPE_IMLIB2)
        menu_draw_string_imlib2(window);
#else
        menu_draw_string(window);
#endif
    } else if (event->xcrossing.state & Button1Mask) {
	if (R_render_background_imlib2(window->images[R_WINDOW_CLICKED_IMAGE],
                                    window,
                                    0);
	    < 0) {
	    menu_draw_rectangle(window);
	} else {
            window_apply_pixmap(window,
                                &window->images[R_WINDOW_CLICKED_IMAGE],
                                0, 0,
                                R_WINDOW_IMAGE_TILE | R_WINDOW_IMAGE_MASK);
            
        }

#if (SUPPORT_TRUETYPE_IMLIB2)
        menu_draw_string_imlib2(window);
#else
        menu_draw_string(window);
#endif
//	window_button_action(window, event, R_BUTTON1_PRESS);
    } else {
	if (R_render_background_imlib2(window->images[R_WINDOW_SELECTED_IMAGE],
                                    window,
                                    0);
	    < 0) {
	    menu_draw_rectangle(window);
	} else {
            window_apply_pixmap(window,
                                &window->images[R_WINDOW_SELECTED_IMAGE],
                                0, 0,
                                R_WINDOW_IMAGE_TILE | R_WINDOW_IMAGE_MASK);
        }

#if (SUPPORT_TRUETYPE_IMLIB2)
        menu_draw_string_imlib2(window);
#else
        menu_draw_string(window);
#endif
    }

    return;
}

void
menu_leavenotify(void *arg, XEvent *event)
{
    if (window == NULL
	|| event == NULL) {

	return;
    }

    menu_draw_window(window);
    menu_close(window->menu->level);

    return;
}

void
menu_expose(void *arg, XEvent *event)
{
    if (window == NULL
	|| event == NULL) {

	return;
    }

    if (event->xexpose.count == 0) {
	menu_draw_window(window);
    }

    return;
}

void
menu_open(void *arg, XEvent *event)
{
    int screen;
    unsigned int winw, winh, dummyui;
    int winx, winy, dummyi;
    Window rootwin;
    Window pointerwin;
    Window dummywin;
    Rmenu_t *menu = NULL;
    Rmenuitem_t *menuitem = NULL;
    
    if (window == NULL) {
	
	return;

    }

    menu = window->menu;
    screen = DefaultScreen(event->xany.display);
    if (menu->type == WIDGET_POPMENU) {
	XQueryPointer(event->xany.display,
		      window->id,
		      &rootwin, &dummywin,
		      &dummyi, &dummyi,
		      &winx, &winy,
		      &dummyui);

	XTranslateCoordinates(event->xany.display,
			      window->id,
                              rootwin,
			      winx, winy, &winx, &winy, &dummywin);

	window_move(menu->window, winx, winy);
    } else if (menu->type == WIDGET_MENU) {
	XGetGeometry(event->xany.display, window->id, &dummywin,
		     &winx, &winy, &winw, &winh,
		     &dummyui, &dummyui);

	XTranslateCoordinates(event->xany.display,
			      window->id,
			      RootWindow(event->xany.display, screen),
			      winx, winy, &winx, &winy, &dummywin);

	window_move(menu->window, winx, winy + winh);
    } else if (menu->type == WIDGET_SUBMENU) {
	XGetGeometry(event->xany.display, window->id, &dummywin,
		     &winx, &winy, &winw, &winh,
		     &dummyui, &dummyui);
        
	XTranslateCoordinates(event->xany.display,
			      window->id,
			      RootWindow(event->xany.display, screen),
			      winx, winy, &winx, &winy, &dummywin);
        
	window_move(menu->window, winx + winw, winy);
    } else {
	XQueryPointer(event->xany.display,
		      window->parent->id,
		      &rootwin, &pointerwin,
		      &dummyi, &dummyi,
		      &winx, &winy,
		      &dummyui);

        XGetGeometry(event->xany.display, pointerwin, &dummywin,
                     &winx, &winy, &winw, &winh,
                     &dummyui, &dummyui);
        
        XTranslateCoordinates(event->xany.display,
                              pointerwin,
                              rootwin,
                              0, 0, &winx, &winy, &dummywin);
        
        window_move(menu->window, winx, winy + winh);
    }
    Rglobals.menulevel = menu->level;
    if (Rglobals.firstmenu == NULL) {
        Rglobals.firstmenu = menu;
    }
    if (Rglobals.lastmenu) {
        Rglobals.lastmenu->nextlevel = menu;
    }
    Rglobals.lastmenu = menu;
    window_map_raised(menu->window);

    return;
}
#endif

void
Rterm_screen_buttonpress(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;
    struct R_termscreen *screen;
    Window dummyw;
    int winx, winy;
    int row;
    int column;
    int dummyi;
    unsigned int dummyui;
    int owner;

    window = arg;
    term = R_global.app->client;
    screen = term->screens[term->curscreen];
    owner = R_selection_owner(term->window);
    if (term->flags & RTERM_BORDERLESS) {
	XSetInputFocus(event->xany.display,
		       term->window->id,
		       RevertToPointerRoot,
		       CurrentTime);
    }
    if (event->xbutton.button == Button1) {
        Rterm_clear_screen_selection(screen);
        Rterm_start_selection(window, event, NULL);
    } else if (event->xbutton.button == Button2) {
        Rterm_request_selection(window, event, NULL);
    } else if (event->xbutton.button == Button3 && (owner)) {
        XQueryPointer(window->app->display,
                      window->id,
                      &dummyw, &dummyw,
                      &dummyi, &dummyi,
                      &winx, &winy,
                      &dummyui);
        row = Rterm_screen_y_row(screen, winy) + screen->viewrow;
        column = Rterm_screen_x_column(screen, winx);
        Rterm_change_selection(term, row, column);
    } else if (event->xbutton.button == Button4) {
        Rterm_touch_screen(screen);
        if (event->xbutton.state & ControlMask) {
            Rterm_scroll_screen(screen, -(5 * (screen->rows - 1)));
        } else if (event->xbutton.state & ShiftMask) {
            Rterm_scroll_screen(screen, -1);
        } else {
            Rterm_scroll_screen(screen, -(screen->rows - 1));
        }
#if 0
        if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
            Rterm_update_scrollbar(term->scrollbar);
        }
#endif
    } else if (event->xbutton.button == Button5) {
        Rterm_touch_screen(screen);
        if (event->xbutton.state & ControlMask) {
            Rterm_scroll_screen(screen, 5 * (screen->rows - 1));
        } else if (event->xbutton.state & ShiftMask) {
            Rterm_scroll_screen(screen, 1);
        } else {
            Rterm_scroll_screen(screen, screen->rows - 1);
        }
    }
#if 0
        if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
            Rterm_update_scrollbar(term->scrollbar);
        }
#endif
#if 0
    if (event->xbutton.button == Button1) {
        if (event->xbutton.state & ControlMask) {
            menu_open(screen->window, event);
            XGrabPointer(window->app->display,
                         window->menu->window->id,
                         True,
                         ButtonReleaseMask,
                         GrabModeAsync,
                         GrabModeAsync,
                         None,
                         None,
                         CurrentTime);
        }
    }
#endif

//    window_button_event(window, event);

    return;
}

void
Rterm_screen_buttonrelease(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;
    struct R_termscreen *screen;
    Window dummyw;
    int winx, winy;
    int row;
    int column;
    int dummyi;
    unsigned int dummyui;

    window = arg;
    XUngrabPointer(window->app->display,
                   CurrentTime);
    term = R_global.app->client;
    screen = term->screens[term->curscreen];
#if 0
    term = R_global.app->client;
    if (event->xbutton.button == Button1
	&& (window->app->flags & APP_MENU_OPEN)) {
        if (term->privmodes & RTERM_PRIVMODE_MENUBAR) {
            menu_close(0);
        }
        menu_close(0);
	window->app->flags &= ~APP_MENU_OPEN;
    }
#endif

#if 0
    if (event->xbutton.button == Button1 || event->xbutton.button == Button3) {
	XQueryPointer(window->app->display,
		      window->id,
		      &dummyw, &dummyw,
		      &dummyi, &dummyi,
		      &winx, &winy,
		      &dummyui);
	row = Rterm_screen_y_row(screen, winy) + screen->viewrow;
	column = Rterm_screen_x_column(screen, winx);
#if 0
	row = Rterm_screen_y_row(screen, event->xmotion.y) + screen->viewrow;
	column = Rterm_screen_x_column(screen, event->xmotion.x);
#endif
    }
#endif
    if (event->xbutton.button == Button1 || event->xbutton.button == Button3) {
#if 0
	row = Rterm_screen_y_row(screen, event->xmotion.y) + screen->viewrow;
	column = Rterm_screen_x_column(screen, event->xmotion.x);
#endif
        if (event->xbutton.button == Button1) {
            XQueryPointer(window->app->display,
                          window->id,
                          &dummyw, &dummyw,
                          &dummyi, &dummyi,
                          &winx, &winy,
                          &dummyui);
            row = Rterm_screen_y_row(screen, winy) + screen->viewrow;
            column = Rterm_screen_x_column(screen, winx);
            Rterm_change_selection(term, row, column);
        }
        R_set_selection(term->window, event->xbutton.time);
        Rterm_highlight_selection(term);
    }
#if 0
    if (event->xbutton.button == Button1) {
        XQueryPointer(window->app->display,
                      window->id,
                      &dummyw, &dummyw,
                      &dummyi, &dummyi,
                      &winx, &winy,
                      &dummyui);
        row = Rterm_screen_y_row(screen, winy) + screen->viewrow;
        column = Rterm_screen_x_column(screen, winx);
        Rterm_change_selection(term, row, column);
    }
#endif

//    window_button_event(window, event);

    return;
}

void
Rterm_screen_motionnotify(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;
    struct R_termscreen *screen;
    Window dummyw;
    int winx, winy;
    int row;
    int column;
    int dummyi;
    unsigned int dummyui;

    window = arg;
    term = R_global.app->client;
    /* FIXME: make dragging selections work */
    screen = term->screens[term->curscreen];
    if (event->xmotion.state == Button1Mask) {
	XQueryPointer(window->app->display,
		      window->id,
		      &dummyw, &dummyw,
		      &dummyi, &dummyi,
		      &winx, &winy,
		      &dummyui);
	row = max(Rterm_screen_y_row(screen, winy) + screen->viewrow, screen->rows - 1);
	column = max(Rterm_screen_x_column(screen, winx), screen->columns - 1);
#if 0
	row = Rterm_screen_y_row(screen, event->xmotion.y) + screen->viewrow;
	column = Rterm_screen_x_column(screen, event->xmotion.x);
#endif
	Rterm_change_selection(term, row, column);
        R_set_selection(term->window, event->xbutton.time);
        Rterm_highlight_selection(term);
    }

    return;
}

void
Rterm_screen_expose(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;
    int x, y, w, h, row;
    int firstrow, lastrow;
    int firstcolumn, lastcolumn;
    struct R_termscreen *screen;
    R_text_t *textptr;
    R_textflags_t *textflagsptr;
    void (*blank_drawn)();
#if (SUPPORT_RTERM_UTF8_CHARS)
    Rterm_wchar_t *wtextptr;
#endif

    window = arg;
    term = R_global.app->client;
    screen = term->screens[term->curscreen];
#if 0
    if (screen->buf) {

	return;
    }
#endif

    x = event->xexpose.x;
    y = event->xexpose.y;
    w = event->xexpose.width;
    h = event->xexpose.height;

    blank_drawn = screen->funcs.blank_drawn;

    firstrow = Rterm_screen_y_row(screen, y);
    lastrow = Rterm_screen_y_row(screen, y + h - 1);
    lastrow = MIN(lastrow, screen->rows - 1);

    firstcolumn = Rterm_screen_x_column(screen, x);
    lastcolumn =  Rterm_screen_x_column(screen, x + w - 1);
    lastcolumn = MIN(lastcolumn, screen->columns - 1);

    screen = term->screens[term->curscreen];

#if 0
    if (!screen->buf) {
#endif
        for (row = firstrow ; row <= lastrow ; row++) {
#if (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                wtextptr = &(screen->textbuf.wdata[(screen->viewrow + row)][firstcolumn]);
            } else {
                textptr = &(screen->textbuf.data[(screen->viewrow + row)][firstcolumn]);
            }
#else
            textptr = &(screen->textbuf.data[(screen->viewrow + row)][firstcolumn]);
#endif
            textflagsptr = &(screen->textbuf.renddata[(screen->viewrow + row)][firstcolumn]);
            
            screen->drawbuf.rowcols[row] = firstcolumn;
            
            blank_drawn(screen, row, firstcolumn,
                        lastcolumn - firstcolumn + 1);

#if (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                Rterm_draw_screen_string_wc(screen, row, firstcolumn, wtextptr, textflagsptr,
                                            MIN(lastcolumn - firstcolumn + 1,
                                                screen->textbuf.rowcols[(screen->viewrow + row)]
                                                - firstcolumn),
                                            RTERM_SCREEN_DRAW_ALL,
                                            FALSE);
            } else {
                Rterm_draw_screen_string_8bit(screen, row, firstcolumn, textptr, textflagsptr,
                                              MIN(lastcolumn - firstcolumn + 1,
                                                  screen->textbuf.rowcols[(screen->viewrow + row)]
                                                  - firstcolumn),
                                              RTERM_SCREEN_DRAW_ALL,
                                              FALSE);
            }
#else            
            Rterm_draw_screen_string_8bit(screen, row, firstcolumn, textptr, textflagsptr,
                                          MIN(lastcolumn - firstcolumn + 1,
                                              screen->textbuf.rowcols[(screen->viewrow + row)]
                                              - firstcolumn),
                                          RTERM_SCREEN_DRAW_ALL,
                                          FALSE);
#endif
        }
#if 0
    }
#endif

    if (event->xexpose.count == 0) {
	if (screen->buf) {
	    Rterm_sync_screen(screen, 0, 0, 0, RTERM_SCREEN_SYNC);
	}
//	XFlush(window->app->display);
        XSync(window->app->display, False);
    }

    return;
}

void
Rterm_scrollbar_buttonpress(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;
    struct R_termscreen *screen;

    window = arg;
    term = R_global.app->client;
    if (event->xbutton.button == Button1) {
	XGrabPointer(window->app->display,
		     window->id,
		     False,
		     ButtonReleaseMask,
		     GrabModeAsync,
		     GrabModeAsync,
		     window->id,
		     None,
		     CurrentTime);
	term->scrollbar->flags |= RTERM_SCROLLBAR_PRESSED;

        screen = term->screens[term->curscreen];
	if (event->xbutton.y < term->scrollbar->anchortop) {
	    Rterm_scroll_screen(screen, -(screen->rows - 1));
	} else if (event->xbutton.y > term->scrollbar->anchorbottom) {
	    Rterm_scroll_screen(screen, screen->rows - 1);
	}

//	Rterm_update_scrollbar(term->scrollbar);
    }

    return;
}

void
Rterm_scrollbar_buttonrelease(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;

    window = arg;
    if (event->xbutton.button == Button1) {
        term = R_global.app->client;
	term->scrollbar->flags &= ~RTERM_SCROLLBAR_PRESSED;
	XUngrabPointer(window->app->display,
		       CurrentTime);
    }

    return;
}

void
Rterm_scrollbar_leavenotify(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;

    window = arg;
    term = R_global.app->client;
    term->scrollbar->flags &= ~RTERM_SCROLLBAR_PRESSED;
}

void
Rterm_scrollbar_anchor_buttonpress(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;

    window = arg;
    if (event->xbutton.button == Button1) {
        term = R_global.app->client;
	XGrabPointer(window->app->display,
		     window->id,
		     False,
		     ButtonReleaseMask
		     | Button1MotionMask
		     | PointerMotionHintMask,
		     GrabModeAsync,
		     GrabModeAsync,
		     term->scrollbar->window->id,
		     None,
		     CurrentTime);
	term->scrollbar->flags |= RTERM_SCROLLBAR_ANCHOR_PRESSED;
#if 0
	R_render_background_imlib2(window->images[R_WINDOW_CLICKED_IMAGE],
                                window,
                                0);
#endif
    }

    return;
}

void
Rterm_scrollbar_anchor_buttonrelease(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;

    window = arg;
    if (event->xbutton.button == Button1) {
        term = R_global.app->client;
	term->scrollbar->flags &= ~RTERM_SCROLLBAR_ANCHOR_PRESSED;
	XUngrabPointer(window->app->display, CurrentTime);

#if 0
	R_render_background_imlib2(window->images[R_WINDOW_SELECTED_IMAGE],
                                window,
                                0);
#endif
    }

    return;
}

void
Rterm_scrollbar_anchor_enternotify(void *arg, XEvent *event)
{
    struct R_window *window;

#if 0
    window = arg;
    R_render_background_imlib2(window->images[R_WINDOW_SELECTED_IMAGE],
                            window,
                            0);
#endif

    return;
}

void
Rterm_scrollbar_anchor_leavenotify(void *arg, XEvent *event)
{
    struct R_window *window;

#if 0
    window = arg;
    R_render_background_imlib2(window->images[R_WINDOW_BACKGROUND_IMAGE],
                            window,
                            0);
#endif

    return;
}

void
Rterm_scrollbar_anchor_motionnotify(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;
    int y, dummyi;
    unsigned int h, dummyui;
    double pixelsperrow;
    struct R_termscreen *screen;
    Window dummywin;

    window = arg;
    term = R_global.app->client;
    if (!(term->scrollbar->flags & RTERM_SCROLLBAR_ANCHOR_PRESSED)) {

	return;
    }

    screen = term->screens[term->curscreen];

    XQueryPointer(window->app->display, term->scrollbar->anchor->id,
		  &dummywin, &dummywin, &dummyi, &dummyi,
		  &dummyi, &y, &dummyui);

    XGetGeometry(window->app->display, term->scrollbar->anchor->id,
		 &dummywin, &dummyi, &dummyi, &dummyui, &h,
		 &dummyui, &dummyui);

    pixelsperrow = MAX((double)h / (screen->nsavelines + screen->rows), 1);

    screen->viewrow = MAX(screen->viewrow + y * pixelsperrow, 0);
    screen->viewrow = MIN(screen->viewrow, screen->savelines);
    screen->viewrow = MAX(screen->viewrow,
			  screen->savelines - screen->nsavelines);

//    Rterm_update_scrollbar(term->scrollbar);

    return;
}

void
Rterm_scrollbar_up_buttonpress(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;

    window = arg;
    term = R_global.app->client;
    if (event->xbutton.button == Button1) {
	XGrabPointer(window->app->display,
		     window->id,
		     False,
		     ButtonReleaseMask,
		     GrabModeAsync,
		     GrabModeAsync,
		     window->id,
		     None,
		     CurrentTime);
	term->scrollbar->flags |= RTERM_SCROLLBAR_UP_ARROW_PRESSED;

#if 0
	R_render_background_imlib2(window->images[R_WINDOW_CLICKED_IMAGE],
                                window,
                                0);
#endif
    }

    Rterm_scroll_screen(term->screens[term->curscreen], -1);

    return;
}

void
Rterm_scrollbar_up_buttonrelease(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;

    window = arg;
    if (event->xbutton.button == Button1) {
        term = R_global.app->client;
	XUngrabPointer(window->app->display,
		       CurrentTime);
	term->scrollbar->flags &= ~RTERM_SCROLLBAR_UP_ARROW_PRESSED;

#if 0
	R_render_background_imlib2(window->images[R_WINDOW_SELECTED_IMAGE],
                                window,
                                0);
#endif
    }

    return;
}

void
Rterm_scrollbar_up_enternotify(void *arg, XEvent *event)
{
    struct R_window *window;

#if 0
    window = arg;
    R_render_background_imlib2(window->images[R_WINDOW_SELECTED_IMAGE],
                            window,
                            0);
#endif

    return;
}

void
Rterm_scrollbar_up_leavenotify(void *arg, XEvent *event)
{
    struct R_window *window;

#if 0
    window = arg;
    R_render_background_imlib2(window->images[R_WINDOW_BACKGROUND_IMAGE],
                            window,
                            0);
#endif

    return;
}

void
Rterm_scrollbar_down_buttonpress(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;

    window = arg;
    term = R_global.app->client;
    if (event->xbutton.button == Button1) {
	XGrabPointer(window->app->display,
		     window->id,
		     False,
		     ButtonReleaseMask,
		     GrabModeAsync,
		     GrabModeAsync,
		     window->id,
		     None,
		     CurrentTime);
	term->scrollbar->flags |= RTERM_SCROLLBAR_DOWN_ARROW_PRESSED;

#if 0
	R_render_background_imlib2(window->images[R_WINDOW_CLICKED_IMAGE],
                                window,
                                0);
#endif
    }

    Rterm_scroll_screen(term->screens[term->curscreen], 1);

    return;
}

void
Rterm_scrollbar_down_buttonrelease(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;

    window = arg;
    term = R_global.app->client;
    if (event->xbutton.button == Button1) {
	XUngrabPointer(window->app->display,
		       CurrentTime);
	term->scrollbar->flags &= ~RTERM_SCROLLBAR_DOWN_ARROW_PRESSED;

#if 0
	R_render_background_imlib2(window->images[R_WINDOW_SELECTED_IMAGE],
                                window,
                                0);
#endif
    }

    return;
}

void
Rterm_scrollbar_down_enternotify(void *arg, XEvent *event)
{
    struct R_window *window;

#if 0
    window = arg;
    R_render_background_imlib2(window->images[R_WINDOW_SELECTED_IMAGE],
                            window,
                            0);
#endif

    return;
}

void
Rterm_scrollbar_down_leavenotify(void *arg, XEvent *event)
{
    struct R_window *window;

#if 0
    window = arg;
    R_render_background_imlib2(window->images[R_WINDOW_BACKGROUND_IMAGE],
                            window,
                            0);
#endif

    return;
}

/* FIXME: allow private mode for application keypad and finish this. */

void
Rterm_keypress(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;
    struct R_termscreen *screen;
#if (SUPPORT_RTERM_TABS)
    struct R_termscreen *curscreen;
    int i;
#endif
#if (SUPPORT_RTERM_INPUT_NUMERIC)
    static int ch = 0;
    int clen;
#if (_BYTE_ORDER == _LITTLE_ENDIAN)
    static int tmp = 0;
#endif
    R_text_t *cp;
#endif
    int shift, ctrl, meta, numlock;
    size_t len;
    KeySym keysym;
    XComposeStatus compose;
    char str[RTERM_MAX_KEY_STRING_LENGTH];
#if (SUPPORT_RTERM_UTF8_CHARS)
    R_text_t *convstr;
    void (*refresh)(struct R_termscreen *, int);
    R_text_t keystr[8 * RTERM_MAX_KEY_STRING_LENGTH];
#endif

    window = arg;
    term = R_global.app->client;
    memset(str, 0, sizeof(str));

    shift = event->xkey.state & ShiftMask;
    ctrl = event->xkey.state & ControlMask;
    meta = event->xkey.state & term->metamask;
    numlock = event->xkey.state & term->numlockmask;

#if 0
    if (!term->blink) {
        Rterm_clear_screen_cursor(screen);
    }
#endif

    screen = term->screens[term->curscreen];
    screen->viewrow = screen->savelines;

    if (numlock) {
	term->privmodes &= ~RTERM_PRIVMODE_APPLICATION_KEYPAD;
    }

    screen->refreshcnt = -1;

    len = XLookupString(&(event->xkey), str, RTERM_MAX_KEY_STRING_LENGTH,
			&keysym, &compose);

#if (DEBUG_KEYSYM)
    fprintf(stderr, "SYM == %lx, shift == %d, ctrl == %d\n",
            (long)keysym, shift, ctrl);
#endif

    /* map unmapped Latin[2-4] entries to Latin1. */

    if (!len && (keysym > 0x0100) && (keysym < 0x0400)) {
	len = 1;
	str[0] = keysym & 0xff;
    }

#if (SUPPORT_RTERM_INPUT_NUMERIC)
    if ((ctrl) && (shift)) {
        if (((ch) && keysym == XK_Return) || keysym == XK_KP_Enter) {
            if ((ch) <= 0xff
                || ((term->privmodes & RTERM_PRIVMODE_UTF8_CHARS)
                    && ch <= 0xffff)) {
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
#if (_BYTEORDER == _LITTLE_ENDIAN)
                    tmp = bswap_32(ch);
                    cp = &tmp;
#else
                    cp = &ch;
#endif
                    clen = 2;
                    RTERM_UTF8_ENC(cp, str, &len, clen);
                } else {
                    str[0] = (unsigned char)ch;
                    clen = 1;
                }
                RTERM_WRITE_PTY(term, screen->pty.masterfd, str, clen);
            }

            return;
        } else if (keysym >= XK_KP_0 && keysym <= XK_KP_9) {
            ch = 10 * ch + 9 - (XK_KP_9 - keysym);

            return;
        } else {
            ch = 0;
        }
    } else {
        ch = 0;
    }
#endif

#if 0 && (SUPPORT_RTERM_UTF8_CHARS)
    if ((term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) && len == 1) {
        uint8_t ch = str[0];
        if (ch < 0x80) {
            str[0] = (unsigned char)ch;
        } else {
            str[0] = (unsigned char )(0xc0 | ((ch & 0xc0) >> 6));
            str[1] = (unsigned char)(0x80 | (ch & 0x3f));
            len = 2;
        }
    }
#endif

#if 0
    if (len && (term->privmodes & RTERM_PRIVMODE_HOME_ON_INPUT)) {
	screen->viewrow = screen->savelines;
    }
#endif

#if (SUPPORT_RTERM_TABS)
    if (ctrl) {
        if (shift) {
            switch (keysym) {
#if (SUPPORT_RTERM_TABS)
                case XK_N:
                    for (i = 0 ; i < term->nscreens - 1 ; i += 2) {
                        if (!term->screens[i]->cmdpid) {

                            break;
                        }
                    }
                    if (i == term->nscreens) {
                        term->screens = realloc(term->screens, (term->nscreens + 2) * sizeof(struct R_termscreen *));
                        Rterm_new_screen(term, i);
                        Rterm_new_screen(term, i + 1);
                        term->nscreens += 2;
                    }
                    if (Rterm_start_tab(term, i >> 1) < 0) {

                        return;
                    }
                    
                    return;
                case XK_X:
                    fprintf(stderr, "KILL: %ld\n", (long)screen->cmdpid);
                    kill(screen->cmdpid, SIGKILL);
                    
                    return;
#endif
            }          
        } else {
            switch (keysym) {
                case XK_BackSpace:
                case XK_KP_Down:
                case XK_Down:
                    curscreen = term->screens[term->curscreen];
                    R_unmap_window(curscreen->window);
                    if (Rtermtabs) {
                        if (term->curscreen == 0) {
                            term->curscreen = term->nscreens - 1;
                        } else {
                            term->curscreen--;
                        }
                        curscreen = term->screens[term->curscreen];
                        while (curscreen->pty.masterfd == -1) {
                            if (term->curscreen == 0) {
                                term->curscreen = term->nscreens - 1;
                            } else {
                                term->curscreen--;
                            }
                            curscreen = term->screens[term->curscreen];
                        }
                    }
                    refresh = curscreen->funcs.refresh;
                    R_map_window(curscreen->window);
                    XSync(screen->window->app->display, False);
//                    Rterm_sync_screen(curscreen, 0, 0, 0, RTERM_SCREEN_DRAW_ALL);
                    curscreen->refreshcnt = -1;
#if 0
                    curscreen->refreshed = TRUE;
//                Rterm_touch_screen(curscreen);
                    Rterm_touch_screen(curscreen);
                    Rterm_clear_screen_buffer(curscreen, 0, 0, 0, RTERM_SCREEN_CLEAR_BUFFER);
                    refresh(curscreen, RTERM_SCREEN_DRAW_ALL);
                    Rterm_sync_screen(curscreen, 0, 0, 0, RTERM_SCREEN_SYNC);
#endif
                    
                    return;
                case XK_Tab:
                case XK_KP_Up:
                case XK_Up:
                    curscreen = term->screens[term->curscreen];
                    R_unmap_window(curscreen->window);
                    if (Rtermtabs) {
                        if (term->curscreen == term->nscreens - 1) {
                            term->curscreen = 0;
                        } else {
                            term->curscreen++;
                        }
                        curscreen = term->screens[term->curscreen];
                        while (curscreen->pty.masterfd == -1) {
                            if (term->curscreen == term->nscreens - 1) {
                                term->curscreen = 0;
                            } else {
                                term->curscreen++;
                            }
                            curscreen = term->screens[term->curscreen];
                        }
                    }
                    refresh = curscreen->funcs.refresh;
                    R_map_window(curscreen->window);
                    XSync(screen->window->app->display, False);
//                    Rterm_sync_screen(curscreen, 0, 0, 0, RTERM_SCREEN_DRAW_ALL);
                    curscreen->refreshcnt = -1;
#if 0
                    curscreen->refreshcnt = 0;
                    curscreen->refreshed = TRUE;
//                Rterm_touch_screen(curscreen);
                    Rterm_touch_screen(curscreen);
                    Rterm_clear_screen_buffer(curscreen, 0, 0, 0, RTERM_SCREEN_CLEAR_BUFFER);
                    refresh(curscreen, RTERM_SCREEN_DRAW_ALL);
                    Rterm_sync_screen(curscreen, 0, 0, 0, RTERM_SCREEN_SYNC);
#endif
                    
                    return;
            }
        }
    }
#endif /* SUPPORT_RTERM_TABS */
    
    if ((term->flags & RTERM_REPORT_AS_KEYSYMS) && (keysym >= 0xff00)) {
	len = snprintf(str, sizeof(str), RTERM_ETERM_REPORT_KEYSYM_FORMAT,
		       (unsigned int)(event->xkey.state & 0xff),
		       (unsigned int)(keysym & 0xff));
        RTERM_WRITE_PTY(term, screen->pty.masterfd, str, len);

	return;
    }

    if (shift) {
	/* Shift + F1 - F10 generate F11 - F20. */
	if (keysym >= XK_F1 && keysym <= XK_F10) {
	    keysym += XK_F11 - XK_F1;
	    shift = 0;
	} else if (!ctrl && !meta
		   && (term->privmodes & RTERM_PRIVMODE_SHIFT_SEQUENCES)) {
	    switch (keysym) {
		/* normal xterm key bindings. */
	        case XK_Prior: /* Shift + Prior = scroll back. */
		    Rterm_scroll_screen(screen, -(screen->rows - 1));

#if 0
		    if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
			Rterm_update_scrollbar(term->scrollbar);
		    }
#endif

		    return;
	        case XK_Next: /* Shift + Next = scroll forward. */
		    Rterm_scroll_screen(screen, screen->rows - 1);

#if 0
		    if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
			Rterm_update_scrollbar(term->scrollbar);
		    }
#endif

		    return;
	        case XK_Insert:
		    /* TODO: paste selection. */

		    return;
	        case XK_KP_Add:
		    /* TODO: switch to bigger font. */

		    return;
	        case XK_KP_Subtract:
		    /* TODO: switch to smaller font. */

		    return;
	        default:

		    break;
	    }
	}
    }
#if (USE_UNSHIFTED_SCROLL_KEYS)
    else if (!ctrl && !meta) {
	switch (keysym) {
	    case XK_Prior:
		Rterm_scroll_screen(screen, -(screen->rows - 1));

#if 0
		if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
		    Rterm_update_scrollbar(term->scrollbar);
		}
#endif

		return;
	    case XK_Next:
		Rterm_scroll_screen(screen, screen->rows - 1);

#if 0
		if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
		    Rterm_update_scrollbar(term->scrollbar);
		}
#endif

		return;
	}
    }
#endif /* USE_UNSHIFTED_SCROLL_KEYS */

    switch (keysym) {
#if defined(PRINT_PIPE)
        case XK_Print:
	    screen_print(screen,
			 ((ctrl | shift)
			  ? RTERM_SCREEN_PRINT_ALL_ROWS
			  : FALSE));

	    break;
#endif /* defined(PRINT_PIPE) */
#if (SUPPORT_GREEK_KEYBOARDS)
        case XK_Mode_switch:
	    /* TODO: implement this */

	    break;
#endif /* SUPPORT_GREEK_KEYBOARDS */
        default:

	    break;
    }

    if (keysym >= 0xff00 && keysym <= 0xffff) {
	switch (keysym) {
            case XK_BackSpace:
		str[0] = (((term->privmodes & RTERM_PRIVMODE_BACKSPACE)
			   ? !(shift | ctrl)
			   : (shift | ctrl))
			  ? R_BACKSPACE_CHAR
			  : R_DEL_CHAR);

		len = 1;

		break;
            case XK_Return:
		    len = 1;

		    str[0] = '\n';

                    break;
	    case XK_Tab:
		if (shift) {
		    len = sizeof(RTERM_SHIFT_TAB_STRING) - 1;

		    strncpy(str, RTERM_SHIFT_TAB_STRING, len);
		}

		break;
	    case XK_KP_Home:
		if ((term->privmodes & RTERM_PRIVMODE_APPLICATION_KEYPAD)
		    ? !shift
		    : shift) {
		    len = sizeof(RTERM_KEYPAD_HOME_STRING) - 1;

		    strncpy(str, RTERM_KEYPAD_HOME_STRING, len);

		    break;
		}

		/* FALLTHROUGH */

	    case XK_Home:
		len = sizeof(RTERM_HOME_STRING) - 1;

		strncpy(str, RTERM_HOME_STRING, len);

		break;
            case XK_KP_Left:        /* \033Ot or standard */
            case XK_KP_Up:          /* \033Ox or standard */
            case XK_KP_Right:       /* \033Ov or standard */
            case XK_KP_Down:        /* \033Ow or standard */
                if (!term->blink) {
                    Rterm_clear_screen_cursor(screen);
                }

		if ((term->privmodes & RTERM_PRIVMODE_APPLICATION_KEYPAD)
		    ? !shift
		    : shift) {
		    len = sizeof(RTERM_KEYPAD_ARROW_PREFIX);

		    strncpy(str, RTERM_KEYPAD_ARROW_PREFIX, len);

		    str[2] = "txvr"[(keysym - XK_KP_Left)];

		    break;
		} else {
		    /* translate to standard cursor key. */
		    keysym = XK_Left + keysym - XK_KP_Left;
		}

		/* FALLTHROUGH */

            case XK_Left:           /* "\033[D" */
            case XK_Up:             /* "\033[A" */
            case XK_Right:          /* "\033[C" */
            case XK_Down:           /* "\033[B" */
		len = sizeof(RTERM_ARROW_PREFIX);

		strncpy(str, RTERM_ARROW_PREFIX, len);

		str[2] = "DACB"[(keysym - XK_Left)];

                if (!term->blink) {
                    Rterm_clear_screen_cursor(screen);
                }

		if (term->privmodes
		    & RTERM_PRIVMODE_APPLICATION_CURSOR_KEYS) {
		    str[1] = 'O';
		} else if (shift) {
		    str[2] = "dacb"[(keysym - XK_Left)];
		} else if (ctrl) {
		    str[1] = 'O';
		    str[2] = "dacb"[(keysym - XK_Left)];
		}

		break;
#if (!USE_UNSHIFTED_SCROLL_KEYS)
	    case XK_KP_Prior:
		if ((term->privmodes & RTERM_PRIVMODE_APPLICATION_KEYPAD)
		    ? !shift
		    : shift) {
		    len = sizeof(RTERM_KEYPAD_PRIOR_STRING) - 1;

		    strncpy(str, RTERM_KEYPAD_PRIOR_STRING, len);

		    break;
		}
		/* FALLTHROUGH */
	    case XK_Prior:
		len = sizeof(RTERM_PRIOR_STRING) - 1;

		strncpy(str, RTERM_PRIOR_STRING, len);

		break;
	    case XK_KP_Next:
		if ((term->privmodes & RTERM_PRIVMODE_APPLICATION_KEYPAD)
		    ? !shift
		    : shift) {
		    len = sizeof(RTERM_KEYPAD_NEXT_STRING) - 1;

		    strncpy(str, RTERM_KEYPAD_NEXT_STRING, len);

		    break;
		}

		/* FALLTHROUGH */

	    case XK_Next:
		len = sizeof(RTERM_NEXT_STRING) - 1;

		strncpy(str, RTERM_NEXT_STRING, len);

		break;
#endif /* !USE_UNSHIFTED_SCROLL_KEYS */
	    case XK_KP_End:
		if ((term->privmodes & RTERM_PRIVMODE_APPLICATION_KEYPAD)
		    ? !shift
		    : shift) {
		    len = sizeof(RTERM_KEYPAD_END_STRING) - 1;

		    strncpy(str, RTERM_KEYPAD_END_STRING, len);

		    break;
		}
		/* FALLTHROUGH */
	    case XK_End:
		len = sizeof(RTERM_END_STRING) - 1;

		strncpy(str, RTERM_END_STRING, len);

		break;
	    case XK_Select:
		len = sizeof(RTERM_SELECT_STRING) - 1;

		strncpy(str, RTERM_SELECT_STRING, len);

		break;
#if defined(DXK_Remove)
/* TODO: implement this. */
	    case DXK_Remove:
		
		break;
#endif /* DXK_Remove */
	    case XK_Execute:
		len = sizeof(RTERM_EXECUTE_STRING) - 1;

		strncpy(str, RTERM_EXECUTE_STRING, len);

		break;
	    case XK_KP_Insert:
		if ((term->privmodes & RTERM_PRIVMODE_APPLICATION_KEYPAD)
		    ? !shift
		    : shift) {
		    len = sizeof(RTERM_KEYPAD_INSERT_STRING) - 1;

		    strncpy(str, RTERM_KEYPAD_INSERT_STRING, len);

		    break;
		}

		/* FALLTHROUGH */

	    case XK_Insert:
		len = sizeof(RTERM_INSERT_STRING) - 1;

		strncpy(str, RTERM_INSERT_STRING, len);

		break;
 	    case XK_KP_Delete:
		if ((term->privmodes & RTERM_PRIVMODE_APPLICATION_KEYPAD)
		    ? !shift
		    : shift) {
		    len = sizeof(RTERM_KEYPAD_DELETE_STRING) - 1;

		    strncpy(str, RTERM_KEYPAD_DELETE_STRING, len);

		    break;
		}

		/* FALLTHROUGH */

	    case XK_Delete:
		/* FIXME: how to do this right? */
#if 0
#if (USE_DELETE_KEY)
		len = sizeof(RTERM_DELETE_STRING) - 1;

		strncpy(str, RTERM_DELETE_STRING, len);
#endif (USE_DELETE_KEY)

		break;
#endif
		len = 1;

		str[0] = R_BACKSPACE_CHAR;

		break;
	    case XK_Menu:
		len = sizeof(RTERM_MENU_STRING) - 1;

		strncpy(str, RTERM_MENU_STRING, len);

		break;
	    case XK_Find:
		len = sizeof(RTERM_FIND_STRING) - 1;

		strncpy(str, RTERM_FIND_STRING, len);

		break;
	    case XK_Help:
		len = sizeof(RTERM_HELP_STRING) - 1;

		strncpy(str, RTERM_HELP_STRING, len);

		break;
	    case XK_KP_Enter:
		if ((term->privmodes & RTERM_PRIVMODE_APPLICATION_KEYPAD)
		    ? !shift
		    : shift) {
		    len = sizeof(RTERM_KEYPAD_ENTER_STRING) - 1;

		    strncpy(str, RTERM_KEYPAD_ENTER_STRING, len);

		    break;
		} else {
		    len = 1;

		    str[0] = R_CARRIAGE_RETURN_CHAR;
		}

		break;
	    case XK_KP_Begin:
		len = sizeof(RTERM_KEYPAD_BEGIN_STRING) - 1;

		strncpy(str, RTERM_KEYPAD_BEGIN_STRING, len);

		break;
            case XK_KP_F1:          /* "\033OP" */
            case XK_KP_F2:          /* "\033OQ" */
            case XK_KP_F3:          /* "\033OR" */
            case XK_KP_F4:          /* "\033OS" */
		len = sizeof(RTERM_KEYPAD_FUNCTION_KEY_PREFIX);

		strncpy(str, RTERM_KEYPAD_FUNCTION_KEY_PREFIX, len);

		str[2] = "PQRS"[(keysym - XK_KP_F1)];

		break;
            case XK_KP_Multiply:    /* "\033Oj" : "*" */
            case XK_KP_Add:         /* "\033Ok" : "+" */
            case XK_KP_Separator:   /* "\033Ol" : "," */
            case XK_KP_Subtract:    /* "\033Om" : "-" */
            case XK_KP_Decimal:     /* "\033On" : "." */
            case XK_KP_Divide:      /* "\033Oo" : "/" */
            case XK_KP_0:           /* "\033Op" : "0" */
            case XK_KP_1:           /* "\033Oq" : "1" */
            case XK_KP_2:           /* "\033Or" : "2" */
            case XK_KP_3:           /* "\033Os" : "3" */
            case XK_KP_4:           /* "\033Ot" : "4" */
            case XK_KP_5:           /* "\033Ou" : "5" */
            case XK_KP_6:           /* "\033Ov" : "6" */
            case XK_KP_7:           /* "\033Ow" : "7" */
            case XK_KP_8:           /* "\033Ox" : "8" */
            case XK_KP_9:           /* "\033Oy" : "9" */
		if ((term->privmodes & RTERM_PRIVMODE_APPLICATION_KEYPAD)
		    ? !shift
		    : shift) {
		    len = sizeof(RTERM_KEYPAD_PREFIX);

		    strncpy(str, RTERM_KEYPAD_PREFIX, len);

		    str[2] = "jklmnopqrstuvwxy"[(keysym - XK_KP_Multiply)];
		} else {
		    len = 1;

		    str[0] = '*' + keysym - XK_KP_Multiply;
		}

		break;
            case XK_F1:             /* "\033[11~" */
            case XK_F2:             /* "\033[12~" */
            case XK_F3:             /* "\033[13~" */
            case XK_F4:             /* "\033[14~" */
            case XK_F5:             /* "\033[15~" */
		len = RTERM_FUNCTION_KEY_STRING_LENGTH;

		snprintf(str, RTERM_FUNCTION_KEY_STRING_LENGTH, "\033[%02d~",
			 (int)(11 + keysym - XK_F1)); \

							  break;
            case XK_F6:             /* "\033[17~" */
            case XK_F7:             /* "\033[18~" */
            case XK_F8:             /* "\033[19~" */
            case XK_F9:             /* "\033[20~" */
            case XK_F10:            /* "\033[21~" */
		len = RTERM_FUNCTION_KEY_STRING_LENGTH;

		snprintf(str, RTERM_FUNCTION_KEY_STRING_LENGTH, "\033[%02d~",
			 (int)(17 + keysym - XK_F6));

		break;
            case XK_F11:            /* "\033[23~" */
            case XK_F12:            /* "\033[24~" */
            case XK_F13:            /* "\033[25~" */
            case XK_F14:            /* "\033[26~" */
		len = RTERM_FUNCTION_KEY_STRING_LENGTH;

		snprintf(str, RTERM_FUNCTION_KEY_STRING_LENGTH, "\033[%02d~",
			 (int)(23 + keysym - XK_F11));

		break;
            case XK_F15:            /* "\033[28~" */
            case XK_F16:            /* "\033[29~" */
		len = RTERM_FUNCTION_KEY_STRING_LENGTH;

		snprintf(str, RTERM_FUNCTION_KEY_STRING_LENGTH, "\033[%02d~",
			 (int)(28 + keysym - XK_F15));

		break;
            case XK_F17:            /* "\033[31~" */
            case XK_F18:            /* "\033[32~" */
            case XK_F19:            /* "\033[33~" */
            case XK_F20:            /* "\033[34~" */
            case XK_F21:            /* "\033[35~" */
            case XK_F22:            /* "\033[36~" */
            case XK_F23:            /* "\033[37~" */
            case XK_F24:            /* "\033[38~" */
            case XK_F25:            /* "\033[39~" */
            case XK_F26:            /* "\033[40~" */
            case XK_F27:            /* "\033[41~" */
            case XK_F28:            /* "\033[42~" */
            case XK_F29:            /* "\033[43~" */
            case XK_F30:            /* "\033[44~" */
            case XK_F31:            /* "\033[45~" */
            case XK_F32:            /* "\033[46~" */
            case XK_F33:            /* "\033[47~" */
            case XK_F34:            /* "\033[48~" */
            case XK_F35:            /* "\033[49~" */
		len = RTERM_FUNCTION_KEY_STRING_LENGTH;

		snprintf(str, RTERM_FUNCTION_KEY_STRING_LENGTH, "\033[%02ld~",
			 31 + keysym - XK_F17);

		break;
	    default:

		break;
	}
    } else if ((ctrl) && keysym == XK_minus) {
	len = sizeof(RTERM_CTRL_MINUS_STRING) - 1;

	strncpy(str, RTERM_CTRL_MINUS_STRING, len);
    } else {
	/* TODO: implement META8_OPTION and SUPPORT_GREEK_KEYBOARDS */
	;
    }
       
    if (str[0] == '\033' && str[1] == '['
	&& str[(len - 1)] == '~') {
	/* pass shift/control indicators for function keys
	 * ending with '~'
	 */
	if (shift) {
	    if (ctrl) {
		str[(len - 1)] = '@';
	    } else {
		str[(len - 1)] = '$';
	    }
	} else if (ctrl) {
	    str[(len - 1)] = '^';
	}
    }

    RTERM_WRITE_PTY(term, screen->pty.masterfd, str, len);

//    screen->refreshed = FALSE;
    screen->refreshcnt = -1;

    return;
}

void
Rterm_mapnotify(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;

    window = arg;
    term = R_global.app->client;

    fprintf(stderr, "MAP\n");

    term->screens[term->curscreen]->refreshcnt = -1;
#if 0
    Rterm_touch_screen(term->screens[term->curscreen]);
    Rterm_refresh_screen(term->screens[term->curscreen], RTERM_SCREEN_DRAW_ALL);
#endif

#if 0
    XSetInputFocus(event->xany.display,
                   term->window->id,
                   RevertToPointerRoot,
                   CurrentTime);
#endif

    return;
}

void
Rterm_enternotify(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;

    window = arg;
    term = R_global.app->client;

    if (!(term->flags & RTERM_START_UNMAPPED)
        && ((term->flags & RTERM_BORDERLESS) || event->xcrossing.focus)) {
	XSetInputFocus(event->xany.display,
		       term->window->id,
		       RevertToPointerRoot,
		       CurrentTime);
    }
#if 0
    if (term->flags & RTERM_ENTER_RESIZE) {
        Rterm_resize(term,
                     RTERM_DEFAULT_WIDTH_COLUMNS, RTERM_DEFAULT_HEIGHT_ROWS);
    }
#endif

    return;
}

void
Rterm_leavenotify(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;

    window = arg;
    term = R_global.app->client;
    if (term->flags & RTERM_UNMAP_LEAVE) {
        R_unmap_window(window);
    }
#if 0
    if (term->flags & RTERM_ENTER_RESIZE) {
        Rterm_resize(term, term->w, term->h);
    }
#endif
    XSync(window->app->display, False);

    return;
}

void
Rterm_expose(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;

    window = arg;
    term = R_global.app->client;
    if (term->screens[term->curscreen]) {
	Rterm_screen_expose(window, event);
    }

    return;
}

void
Rterm_focusin(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;
    struct R_termscreen *screen;

    window = arg;
    term = R_global.app->client;
    screen = term->screens[term->curscreen];
#if 0
    XSetInputFocus(event->xany.display,
		   term->screen->window->id,
		   RevertToPointerRoot,
		   CurrentTime);
#endif
//    term->window->stateflags |= R_WINDOW_FOCUSED_FLAG;
//    screen->flags |= RTERM_SCREEN_HAS_FOCUS;

    return;
}

void
Rterm_focusout(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;
    struct R_termscreen *screen;
    void (*refresh)(struct R_termscreen *, int);

    window = arg;
    term = R_global.app->client;
    screen = term->screens[term->curscreen];
    refresh = screen->funcs.refresh;
//    term->window->stateflags &= ~R_WINDOW_FOCUSED_FLAG;
//    screen->flags &= ~RTERM_SCREEN_HAS_FOCUS;

#if 0
    if (termptr->flags & RTERM_POPUP_SCROLLBAR) {
	term_show_scrollbar(termptr);
    }
#endif

#if (SUPPORT_RTERM_BLINKING_CHARS)
    term->blink = TRUE;
    refresh(screen, RTERM_SCREEN_DRAW_BLINKING_ONLY);
#endif /* SUPPORT_RTERM_BLINKING_CHARS */

#if (SUPPORT_RTERM_BLINKING_CURSOR)
//    Rterm_draw_screen_cursor(screen);
#endif

    if (term->flags & RTERM_UNMAP_FOCUS) {
        R_unmap_window(term->window);
    }

    return;
}

#if 0
void
Rterm_map_on_enter(void *arg, XEvent *event)
{
    struct R_term *term = R_global.app->client;

    fprintf(stderr, "MAP_ON_ENTER\n");

    R_map_window_raised(term->window);

    return;
}
#endif

void
Rterm_configurenotify(void *arg, XEvent *event)
{
    struct R_app *app;
    struct R_window *window;
    int rows, columns;
    struct R_term *term;
    struct R_termscreen *screen;
    XEvent configurenotifyevent;
    void (*refresh)(struct R_termscreen *, int);
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    Pixmap pixmap;
    R_text_t *textptr;
    R_textflags_t *textflagsptr;
    int row;
    int len;
#endif
    int i;

    window = arg;
#if 0
    memset(&configurenotifyevent, 0, sizeof(configurenotifyevent));
#endif
    configurenotifyevent.xconfigure.x = event->xconfigure.x;
    configurenotifyevent.xconfigure.y = event->xconfigure.y;
    configurenotifyevent.xconfigure.width = event->xconfigure.width;
    configurenotifyevent.xconfigure.height = event->xconfigure.height;

    app = R_global.app;
    term = app->client;
    screen = term->screens[term->curscreen];
    refresh = screen->funcs.refresh;
    if (refresh == NULL) {

	return;
    }

    while (XCheckTypedWindowEvent(app->display, window->id,
				  ConfigureNotify, &configurenotifyevent)) {
	/* get rid of out-of-sync ConfigureNotify events. */
	;
    }

    rows = (configurenotifyevent.xconfigure.height
            -  ((term->privmodes & RTERM_PRIVMODE_MENUBAR)
                ? RTERM_MENU_DEFAULT_HEIGHT
                : 0))
        / screen->charh;
    
    columns = (configurenotifyevent.xconfigure.width
               - ((term->privmodes & RTERM_PRIVMODE_SCROLLBAR)
                  ? RTERM_SCROLLBAR_DEFAULT_WIDTH
                  : 0))
        / screen->charw;

#if 0
    rows = (event->xconfigure.height
            -  ((term->privmodes & RTERM_PRIVMODE_MENUBAR)
                ? RTERM_MENU_DEFAULT_HEIGHT
                : 0))
        / screen->charh;
    
    columns = (event->xconfigure.width
               - ((term->privmodes & RTERM_PRIVMODE_SCROLLBAR)
                  ? RTERM_SCROLLBAR_DEFAULT_WIDTH
                  : 0))
        / screen->charw;
#endif

#if 0    
    columns = MIN(columns, RTERM_SCREEN_MAX_COLUMNS);
#endif

#if 0
    fprintf(stderr, "term_configurenotify(): x = %d, y = %d, w = %d, h = %d\n",
	    event->xconfigure.x,
	    event->xconfigure.y,
	    event->xconfigure.width,
	    event->xconfigure.height);
#endif

    if (rows < 0
        || columns < 0) {

        return;
    }
    
    if (rows != screen->rows
	|| columns != screen->columns) {
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

//    Rterm_update_scrollbar(term->scrollbar);

    return;
}

void
Rterm_clientmessage(void *arg, XEvent *event)
{
    struct R_term *term = R_global.app->client;

    if (event->xclient.data.l[0] == term->wmdeletewindow) {

        exit(0);
    }

    return;
}
