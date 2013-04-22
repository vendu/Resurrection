/*
 * xterm.c - xterm compatibility routines for Rterm.
 * Copyright (C) 2003 - 2007 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

extern const char *defaultcolornames[];

#if (SUPPORT_XTERM_256_COLORS)
void
Rterm_reset_color(struct R_term *term, struct R_termscreen *screen, char *str)
{
    XColor color;
    int colorid = 0, ch;

    if ((str = strchr(str, ';')) == NULL) {

        return;
    }
    if (isdigit(str)) {
        while ((ch = isdigit(*str++))) {
            colorid = colorid * 10 + ch - '0';
        }
        if (colorid < RTERM_SCREEN_COLORS && screen->colornames[colorid]) {
            if (!XParseColor(screen->window->app->display,
                             screen->window->app->colormap,
                             screen->colornames[colorid],
                             &color)) {
                
                return;
            }
            if (!XAllocColor(screen->window->app->display,
                             screen->window->app->colormap,
                             &color)) {
                
                return;
            }
            screen->colors[colorid] = color.pixel;
        }
    }

    return;
}

void
Rterm_set_color(struct R_term *term, struct R_termscreen *screen, char *str)
{
    char *valuestr;
    int colorid = 0;
    int ch;
    XColor color;

    while ((valuestr = strchr(str, ';')) != NULL) {
        while (isdigit(ch = *valuestr++)) {
            colorid = colorid * 10 + ch - '0';
        }
        --valuestr;
        if (*valuestr++ == ';' && colorid < RTERM_SCREEN_COLORS) {
            if (!XParseColor(screen->window->app->display,
                             screen->window->app->colormap,
                             valuestr,
                             &color)) {
                
                return;
            }
            if (!XAllocColor(screen->window->app->display,
                             screen->window->app->colormap,
                             &color)) {
                
                return;
            }
            screen->colors[colorid] = color.pixel;
        }
    }

    return;
}
#endif

/* TODO: implement this fully. */
void
Rterm_do_xterm_sequence(struct R_term *term, struct R_termscreen *screen,
                        int operation, char *str)
{
    struct R_app *app;
    char *origstr, *tmpstr, *valuestr, *value;
    int arg = 0, ch;
    XColor color;

    app = R_global.app;
    origstr = tmpstr = strdup(str);
    
    switch (operation) {
        case RTERM_XTERM_CHANGE_ICON_NAME_AND_WINDOW_TITLE:
#if 0
	    window_set_icon_name(term->window, tmpstr);
#endif

	    R_set_window_title(term->window, tmpstr, TRUE);

	    break;
        case RTERM_XTERM_CHANGE_ICON_NAME:
#if 0
	    window_set_icon_name(term->window, tmpstr);
#endif

	    break;
        case RTERM_XTERM_CHANGE_WINDOW_TITLE:
	    R_set_window_title(term->window, tmpstr, TRUE);

	    break;
        case RTERM_XTERM_SET_TEXT_PROPERTY:
	    if ((valuestr = strchr(tmpstr, ';')) == NULL) {

		break;
	    }

	    if ((value = strchr(valuestr, '=')) != NULL) {
		*(value++) = '\0';
	    }
	    
	    R_set_text_property(term->window, valuestr, value);

	    break;
#if (SUPPORT_XTERM_256_COLORS)
        case RTERM_XTERM_SET_COLOR:
        case RTERM_XTERM_SET_SPECIAL_COLOR:

	    Rterm_set_color(term, screen, tmpstr);

            break;
#endif
        case RTERM_XTERM_SET_VT100_FOREGROUND_COLOR:
        case RTERM_XTERM_SET_VT100_BACKGROUND_COLOR:
	    if ((valuestr = strchr(tmpstr, ';')) == NULL) {

		break;
	    }
            if (isdigit(*valuestr)) {
                while (isdigit(ch = *valuestr++)) {
                    arg = arg * 10 + ch - '0';
                }
                if (arg < RTERM_SCREEN_COLORS) {
                    if (operation == RTERM_XTERM_SET_VT100_FOREGROUND_COLOR) {
                        RTERM_SET_CHAR_FG_COLOR(screen->textflags, arg);
                    } else {
                        RTERM_SET_CHAR_BG_COLOR(screen->textflags, arg);
                    }
                }
            }

            break;
#if 0
        case RTERM_ETERM_TAKEOVER:
	    R_set_input_focus(term->window);

	    R_raise_window(term->window);

	    break;
        case RTERM_ETERM_SEQUENCE:
#if (SUPPORT_ETERM_SEQUENCES)
	    eterm_process_escape_sequence(term, tmpstr);

	    break;
        case RTERM_ETERM_IPC:
	    eterm_process_ipc_sequence(term, tmpstr);

	    break;
	    /* TODO: implement these. */
        case RTERM_ETERM_MENUBAR_COMMAND:

	    break;
        case RTERM_ETERM_PIXMAP_COMMAND:

	    break;
#endif
#endif
        case RTERM_XTERM_SET_DEFAULT_FOREGROUND_COLOR:
        case RTERM_XTERM_SET_DEFAULT_BACKGROUND_COLOR:
	    if ((valuestr = strchr(tmpstr, ';')) == NULL) {

		break;
	    }
            if (isdigit(*valuestr)) {
                while (isdigit(ch = *valuestr++)) {
                    arg = arg * 10 + ch - '0';
                }
                if (arg < RTERM_SCREEN_COLORS) {
                    if (operation == RTERM_XTERM_SET_DEFAULT_FOREGROUND_COLOR) {
                        Rterm_set_default_foreground_color(term, screen, arg);
                    } else {
                        Rterm_set_default_background_color(term, screen, arg);
                    }
                }
            }
//            fprintf(stderr, "xterm: set foreground color %s ignored\n", tmpstr);

	    break;
        case RTERM_XTERM_CHANGE_LOG_FILE:

	    break;
        case RTERM_XTERM_SET_FONT:

	    break;
#if (SUPPORT_XTERM_256_COLORS)
        case RTERM_XTERM_RESET_COLOR:
        case RTERM_XTERM_RESET_SPECIAL_COLOR:
            Rterm_reset_color(term, screen, tmpstr);

            break;
        case RTERM_XTERM_RESET_VT100_FOREGROUND_COLOR:
            if (!XParseColor(screen->window->app->display,
                             screen->window->app->colormap,
                             defaultcolornames[RTERM_SCREEN_FOREGROUND_COLOR],
                             &color)) {
                
                return;
            }
            if (!XAllocColor(screen->window->app->display,
                             screen->window->app->colormap,
                             &color)) {
                
                return;
            }
            screen->defcolors[RTERM_SCREEN_FOREGROUND_COLOR] = color.pixel;
            Rterm_set_default_foreground_color(term, screen, RTERM_SCREEN_DEFAULT_FOREGROUND_COLOR);
            screen->textflags &= ~RTERM_CHAR_FG_COLOR_MASK;
            
            break;
        case RTERM_XTERM_RESET_VT100_BACKGROUND_COLOR:
            if (!XParseColor(screen->window->app->display,
                             screen->window->app->colormap,
                             defaultcolornames[RTERM_SCREEN_BACKGROUND_COLOR],
                             &color)) {
                
                return;
            }
            if (!XAllocColor(screen->window->app->display,
                             screen->window->app->colormap,
                             &color)) {
                
                return;
            }
            screen->defcolors[RTERM_SCREEN_BACKGROUND_COLOR] = color.pixel;
            Rterm_set_default_background_color(term, screen, RTERM_SCREEN_DEFAULT_BACKGROUND_COLOR);
            screen->textflags &= ~RTERM_CHAR_BG_COLOR_MASK;

            break;
#endif

            break;
        default:
	    fprintf(stderr, "%s: Rterm_do_xterm_sequence(): operation == %d, str == %s\n", app->name, operation, str);

	    break;
    }

    free(origstr);
    
    return;
}

void
Rterm_process_xterm_sequence(struct R_term *term, struct R_termscreen *screen)
{
    struct R_app *app;
    int ch, arg, i;
    char str[RTERM_XTERM_SEQUENCE_STRING_LENGTH];
    int (*read_char)(struct R_term *);

    app = R_global.app;
    read_char = term->funcs.read_char;

    if ((ch = read_char(term)) < 0) {

	return;
    }

    if (isdigit(ch)) {
        arg = ch - '0';
        if ((ch = read_char(term)) < 0) {
            
            return;
        }
        
        for ( ; isdigit(ch) ; ch = read_char(term)) {
            if (ch < 0) {
                
                return;
            }
            
            arg = arg * 10 + ch - '0';
        }
    } else if (ch == ';') {
        arg = 0;
    } else {
        arg = ch;
        
        if ((ch = read_char(term)) < 0) {
            
            return;
        }
    }
    
    if (ch == ';') {
        i = 0;
        
        while ((ch = read_char(term)) != R_BEL_CHAR) {
            if (ch < 0) {
                
                return;
            } else if (ch == '\t') {
                ch = ' ';
            } else if (ch < ' ') {
                /* control character. */
                if (ch == 27 && (ch = read_char(term)) == '\\') {
                    
                    break;
                }
                
                return;
            }
            
            if (i < sizeof(str) - 1) {
                str[i++] = (char)ch;
            }
        }
        
        str[i] = '\0';
        
        Rterm_do_xterm_sequence(term, screen, arg, str);
    } else {
	i = 0;

	for ( ; ch != '\033' ; ch = read_char(term)) {
	    if (ch) {
		if (ch < 0) {
		    
		    return;
		} else if (ch == '\t') {
		    ch = ' ';
		} else if (ch < ' ') {
		    /* control character. */
		    
		    return;
		}
		
		if (i < sizeof(str) - 1) {
		    str[i++] = (char)ch;
		}
	    }
	}
	
	str[i] = '\0';
	
	if ((ch = read_char(term)) != '\\') {
	    if (ch < 0) {

		return;
	    }
	    
	    return;
	}
	
	switch (arg) {
	    case RTERM_XTERM_SET_WINDOW_TITLE:
		Rterm_do_xterm_sequence(term, screen, RTERM_XTERM_CHANGE_WINDOW_TITLE,
				  str);

		break;
	    case RTERM_XTERM_SET_ICON_NAME:
		Rterm_do_xterm_sequence(term, screen, RTERM_XTERM_CHANGE_ICON_NAME, str);

		break;
	    case RTERM_XTERM_SET_ICON_PIXMAP:
#if 0
#if (!(USE_IMLIB2))
		window_load_icon_pixmap(term->window, str);
#endif
#endif

		break;
	    default:
		fprintf(stderr, "%s: Rterm_process_xterm_sequence(): arg == %d, str = %s\n", app->name, arg, str);

		break;
	} 
    }

    return;
}

/* TODO: a new function for each case. */
void
Rterm_process_xterm_window_operation(struct R_term *term, struct R_termscreen *screen, int args[], int nargs)
{
    struct R_app *app;
    int screennum;
    unsigned int dummyui, w, h;
    int i, dummyi, x, y;
    char str[RTERM_XTERM_WINDOW_OPERATION_STRING_LENGTH], *name;
    Window dummywin;
    Screen *screenptr;
    void (*refresh)(struct R_termscreen *, int);

    if (term == NULL
	|| nargs <= 0) {

	return;
    }

    app = R_global.app;
    refresh = screen->funcs.refresh;

    screennum = DefaultScreen(app->display);
    screenptr = ScreenOfDisplay(app->display, screennum);

    for (i = 0 ; i < nargs ; i++) {
	switch (args[i]) {
	    case RTERM_XTERM_UNICONIFY_WINDOW:
		R_map_window(term->window);

		break;
	    case RTERM_XTERM_ICONIFY_WINDOW:
//		R_iconify_window(term->window);

		break;
	    case RTERM_XTERM_MOVE_WINDOW:
		if (i >= nargs - 2) {

		    return;
		}
		
		x = args[++i];
		y = args[++i];

		if (x > WidthOfScreen(screenptr)
		    || y > HeightOfScreen(screenptr)) {

		    continue;
		}

		R_move_window(term->window, x, y);

		break;
	    case RTERM_XTERM_RESIZE_WINDOW:
		if (i >= nargs - 2) {

		    return;
		}

		w = (unsigned int)args[++i];
		h = (unsigned int)args[++i];

		R_resize_window(term->window, w, h);

		break;
	    case RTERM_XTERM_RAISE_WINDOW:
		R_raise_window(term->window);

		break;
	    case RTERM_XTERM_LOWER_WINDOW:
		R_lower_window(term->window);

		break;
	    case RTERM_XTERM_REFRESH_WINDOW:
		Rterm_touch_screen(screen);

		refresh(screen, RTERM_SCREEN_DRAW_ALL);

		break;
	    case RTERM_XTERM_RESIZE_SCREEN:
		if (i >= nargs - 2) {

		    return;
		}

		w = (unsigned int)args[++i];
		h = (unsigned int)args[++i];

		Rterm_resize(term, w, h);

		break;
	    case RTERM_XTERM_REPORT_ICON_STATE:
		/* TODO: implement this. */

		break;
	    case RTERM_XTERM_REPORT_WINDOW_POSITION:
		XTranslateCoordinates(app->display,
				      term->window->id,
				      RootWindow(app->display,
						 screennum),
				      0, 0,
				      &x, &y, &dummywin);

		snprintf(str, sizeof(str), RTERM_XTERM_WINDOW_POSITION_FORMAT,
			 x, y);

                RTERM_WRITE_PTY(term, screen->pty.masterfd, str, strlen(str));

		break;
	    case RTERM_XTERM_REPORT_WINDOW_SIZE:
		XGetGeometry(app->display, term->window->id,
			     &dummywin, &dummyi, &dummyi, &w, &h,
			     &dummyui, &dummyui);

		snprintf(str, sizeof(str), RTERM_XTERM_WINDOW_SIZE_FORMAT,
			 h, w);

#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    R_write_pty_utf8(screen->pty.masterfd, str, strlen(str));
                } else {
                    R_write_pty(screen->pty.masterfd, str, strlen(str));
                }
#else
		R_write_pty(screen->pty.masterfd, str, strlen(str));
#endif

		break;
	    case RTERM_XTERM_REPORT_SCREEN_SIZE:
		snprintf(str, sizeof(str), RTERM_XTERM_SCREEN_SIZE_FORMAT,
			 screen->rows, screen->columns);

#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    R_write_pty_utf8(screen->pty.masterfd, str, strlen(str));
                } else {
                    R_write_pty(screen->pty.masterfd, str, strlen(str));
                }
#else
		R_write_pty(screen->pty.masterfd, str, strlen(str));
#endif

		break;
	    case RTERM_XTERM_REPORT_ICON_NAME:
		/* FIXME: use XGetWMIconName and XGetWMName here. */
		XGetIconName(app->display, term->window->id,
			     &name);

		snprintf(str, sizeof(str), RTERM_XTERM_ICON_NAME_FORMAT,
			 name);

		XFree(name);

#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    R_write_pty_utf8(screen->pty.masterfd, str, strlen(str));
                } else {
                    R_write_pty(screen->pty.masterfd, str, strlen(str));
                }
#else
		R_write_pty(screen->pty.masterfd, str, strlen(str));
#endif

		break;
	    case RTERM_XTERM_REPORT_WINDOW_TITLE:
		XFetchName(app->display, term->window->id,
			   &name);

		snprintf(str, sizeof(str), RTERM_XTERM_WINDOW_TITLE_FORMAT,
			 name);

		XFree(name);

#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    R_write_pty_utf8(screen->pty.masterfd, str, strlen(str));
                } else {
                    R_write_pty(screen->pty.masterfd, str, strlen(str));
                }
#else
		R_write_pty(screen->pty.masterfd, str, strlen(str));
#endif

		break;
	    default:
		fprintf(stderr, "%s: Rterm_process_xterm_window_operation(): arg == %d\n", app->name, args[i]);

		break;
	}
    }

    return;
}

