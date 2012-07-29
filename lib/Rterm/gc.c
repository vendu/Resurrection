/*
 * gc.c - graphics context (GC) routines for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#define RTERM_SCREEN_DEFAULT_FONT "fixed"

#if 0
GC
imageview_create_gc(Rimageview_t *imageview)
{
    GC newgc;
    XGCValues gcvalues;

    gcvalues.foreground = BlackPixel(imageview->app->display,
				     DefaultScreen(imageview->app->display));
    if (imageview->fontinfo == NULL) {
	if (imageview_load_font(imageview, NULL) < 0) {

	    return NULL;
	}
    }
    gcvalues.font = imageview->fontinfo->fid;
    gcvalues.function = GXandReverse;
    newgc = XCreateGC(imageview->app->display,
		      imageview->window->win,
		      GCForeground | GCFont | GCFunction,
		      &gcvalues);

    return newgc;
}

GC
menu_create_gc(Rwindow_t *window)
{
    GC newgc;
    XGCValues gcvalues;

    if (window == NULL) {

	return NULL;
    }

    gcvalues.foreground = WhitePixel(window->app->display,
				     DefaultScreen(window->app->display));
    gcvalues.line_width = MENU_DEFAULT_LINE_WIDTH;
    
    if (window->fontinfo == NULL) {
	window->fontinfo = app_load_font(window->app, MENU_DEFAULT_FONT);
	if (window->fontinfo == NULL) {

	    return NULL;
	}
    }
    gcvalues.font = window->fontinfo->fid;
    
    gcvalues.graphics_exposures = False;
    
    newgc = XCreateGC(window->app->display, window->win,
		      GCForeground | GCLineWidth | GCFont
		      | GCGraphicsExposures,
		      &gcvalues);

    return newgc;
}
#endif

int
Rterm_init_selection_gc(struct R_term *term)
{
    XGCValues gcvalues;

    if (term == NULL) {

	return -1;
    }

    gcvalues.foreground = WhitePixel(term->window->app->display,
                                     DefaultScreen(term->window->app->display));
    gcvalues.function = GXandReverse;

    term->selectiongc = XCreateGC(term->window->app->display,
                                  term->window->id,
                                  GCForeground
                                  | GCFunction,
                                  &gcvalues);
    if (term->selectiongc == NULL) {

	return -1;
    }

    return 0;
}

void
Rterm_set_default_foreground_color(struct R_term *term, struct R_termscreen *screen, int arg)
{
    XGCValues gcvalues;

    screen->defcolors[RTERM_SCREEN_FOREGROUND_COLOR] = screen->colors[arg];
    gcvalues.foreground = screen->colors[arg];
    XChangeGC(screen->window->app->display,
              screen->defaultgc,
              GCForeground,
              &gcvalues);
    
    return;
}

void
Rterm_set_default_background_color(struct R_term *term, struct R_termscreen *screen, int arg)
{
    XGCValues gcvalues;
    
    screen->defcolors[RTERM_SCREEN_BACKGROUND_COLOR] = screen->colors[arg];
    gcvalues.background = screen->colors[arg];
    XChangeGC(screen->window->app->display,
              screen->defaultgc,
              GCBackground,
              &gcvalues);
    
    return;
}

int
Rterm_init_screen_gcs(struct R_termscreen *screen)
{
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    struct R_term *term = R_global.app->client;
#endif
    int screennum;
    XGCValues gcvalues;

    gcvalues.foreground = screen->defcolors[RTERM_SCREEN_FOREGROUND_COLOR];
    gcvalues.background = screen->defcolors[RTERM_SCREEN_BACKGROUND_COLOR];

    if (Rterm_load_screen_font(screen, RTERM_SCREEN_DEFAULT_FONT) < 0) {
        fprintf(stderr, "Rterm_load_screen_font failed\n");

	return -1;
    }

    gcvalues.font = screen->fontinfo->fid;

    gcvalues.graphics_exposures = False;

    screen->gc = XCreateGC(screen->window->app->display,
			   screen->window->id,
			   GCForeground | GCBackground | GCFont
			   | GCGraphicsExposures,
			   &gcvalues);
    if (screen->gc == NULL) {
        fprintf(stderr, "screen->gc failed\n");

	return -1;
    }

    screen->defaultgc = XCreateGC(screen->window->app->display,
				  screen->window->id,
				  GCForeground | GCBackground | GCFont
				  | GCGraphicsExposures,
				  &gcvalues);
    if (screen->defaultgc == NULL) {
        fprintf(stderr, "screen->defaultgc failed\n");
	XFreeGC(screen->window->app->display, screen->gc);

	return -1;
    }

    gcvalues.foreground = screen->colors[RTERM_SCREEN_CURSOR_COLOR];
//    gcvalues.background = screen->colors[RTERM_SCREEN_CURSOR_COLOR];
    gcvalues.function = GXxor;

    gcvalues.graphics_exposures = False;

    screen->cursorgc = XCreateGC(screen->window->app->display,
				 screen->window->id,
				 GCForeground | GCFunction | GCGraphicsExposures,
				 &gcvalues);
    if (screen->cursorgc == NULL) {
        fprintf(stderr, "screen->cursorgc failed\n");
	XFreeGC(screen->window->app->display, screen->gc);
	XFreeGC(screen->window->app->display, screen->defaultgc);

	return -1;
    }
    XSetForeground(screen->window->app->display, screen->cursorgc,
		   screen->colors[RTERM_SCREEN_CURSOR_TEXT_COLOR]);
    XSetFunction(screen->window->app->display, screen->cursorgc,
		 GXandReverse);

    memset(&gcvalues, 0, sizeof(gcvalues));
    screennum = DefaultScreen(screen->window->app->display);
    gcvalues.foreground = WhitePixel(screen->window->app->display, screennum);
    gcvalues.function = GXandReverse;
    gcvalues.graphics_exposures = False;

#if 0
    screen->selectiongc = XCreateGC(screen->window->app->display,
				    screen->window->id,
				    GCForeground | GCFunction,
				    &gcvalues);
    if (screen->selectiongc == NULL) {
	XFreeGC(screen->window->app->display, screen->gc);
	XFreeGC(screen->window->app->display, screen->defaultgc);
	XFreeGC(screen->window->app->display, screen->cursorgc);

	return -1;
    }
#endif

#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    if (term->flags & RTERM_DOUBLE_BUFFER) {
        memset(&gcvalues, 0, sizeof(gcvalues));
        gcvalues.foreground = screen->colors[RTERM_SCREEN_BACKGROUND_COLOR];
        gcvalues.function = GXcopy;
        gcvalues.graphics_exposures = False;
        
        screen->bufgc = XCreateGC(screen->window->app->display,
                                  screen->window->id,
                                  GCForeground | GCFunction | GCGraphicsExposures,
                              &gcvalues);
        if (screen->bufgc == NULL) {
            fprintf(stderr, "screen->bufgc failed\n");
            XFreeGC(screen->window->app->display, screen->gc);
            XFreeGC(screen->window->app->display, screen->defaultgc);
            XFreeGC(screen->window->app->display, screen->cursorgc);
//	XFreeGC(screen->window->app->display, screen->selectiongc);
            
            return -1;
        }
    }
#endif

#if (SUPPORT_TRUETYPE_FONTS)
    memset(&gcvalues, 0, sizeof(gcvalues));
    gcvalues.function = GXcopy;
    gcvalues.graphics_exposures = False;

    screen->bggc = XCreateGC(screen->window->app->display,
                             screen->window->id,
                             GCFunction | GCGraphicsExposures,
                             &gcvalues);
#endif

    return 0;
}

/*
 * NOTE: everything below really belongs into Rterm/screen.c and it should only
 * be called through function pointers from the screen structure...
 */

void
Rterm_set_screen_gc(struct R_termscreen *screen, R_textflags_t textflags)
{
    if (screen == NULL
	|| screen->gctextflags == textflags) {
	
	return;
    }

    if (textflags & RTERM_CHAR_BOLD) {
	if (RTERM_CHAR_FG_COLOR(textflags) < RTERM_SCREEN_MIN_BRIGHT_COLOR) {
	    if (RTERM_CHAR_FG_COLOR(textflags) + RTERM_SCREEN_BRIGHT_OFFSET
		!= RTERM_CHAR_FG_COLOR(screen->gctextflags)) {
		Rterm_set_screen_foreground_color(screen,
					    RTERM_CHAR_FG_COLOR(textflags)
					    + RTERM_SCREEN_BRIGHT_OFFSET);
	    }
	} else if ((textflags & RTERM_CHAR_FG_COLOR_MASK)
		   != (screen->gctextflags & RTERM_CHAR_FG_COLOR_MASK)) {
	    Rterm_set_screen_foreground_color(screen, RTERM_CHAR_FG_COLOR(textflags));
	}
    } else if ((textflags & RTERM_CHAR_FG_COLOR_MASK)
	       != (screen->gctextflags & RTERM_CHAR_FG_COLOR_MASK)) {
	Rterm_set_screen_foreground_color(screen, RTERM_CHAR_FG_COLOR(textflags));
    }
    
#if (SUPPORT_RTERM_BLINKING_CHARS)
    if ((textflags & RTERM_CHAR_BG_COLOR_MASK)
	!= (screen->gctextflags & RTERM_CHAR_BG_COLOR_MASK)) {
	Rterm_set_screen_background_color(screen, RTERM_CHAR_BG_COLOR(textflags));
    }
#else /* !SUPPORT_RTERM_BLINKING_CHARS */
    if (textflags & RTERM_CHAR_BLINKING) {
	if (RTERM_CHAR_BG_COLOR(textflags) < RTERM_SCREEN_MIN_BRIGHT_COLOR) {
	    Rterm_set_screen_background_color(screen,
					RTERM_CHAR_BG_COLOR(textflags)
					+ RTERM_SCREEN_BRIGHT_OFFSET);
	} else if ((textflags & RTERM_CHAR_BG_COLOR_MASK)
		   != (screen->gctextflags & RTERM_CHAR_BG_COLOR_MASK)) {
	    Rterm_set_screen_background_color(screen, RTERM_CHAR_BG_COLOR(textflags));
	}
    } else if ((textflags & RTERM_CHAR_BG_COLOR_MASK)
	       != (screen->gctextflags & RTERM_CHAR_BG_COLOR_MASK)) {
	Rterm_set_screen_background_color(screen, RTERM_CHAR_BG_COLOR(textflags));
    }
#endif
    
    if (textflags & RTERM_CHAR_REVERSE) {
	if (!(textflags & RTERM_CHAR_SELECTED)) {
	    Rterm_set_screen_foreground_color(screen, RTERM_CHAR_BG_COLOR(textflags));
	    Rterm_set_screen_background_color(screen, RTERM_CHAR_FG_COLOR(textflags));
	}
    }
    /* FIXME: finish this function. */
	
    screen->gctextflags = textflags;

    return;
}

