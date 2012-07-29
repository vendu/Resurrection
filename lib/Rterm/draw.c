/*
 * draw.c - drawing routines for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#define KLUDGE 1

#include <Resurrection/Resurrection.h>

#if 0
void
menu_draw_rectangle(Rwindow_t *window)
{
    unsigned int winw, winh;
    unsigned int dummyui;
    int dummyi;
    Window dummywin;

    if (window == NULL) {

	return;
    }

    XGetGeometry(window->app->display, window->id, &dummywin,
		 &dummyi, &dummyi, &winw, &winh, &dummyui, &dummyui);
    
    XDrawRectangle(window->app->display, window->id, window->gc,
		   MENU_RECTANGLE_OFFSET, MENU_RECTANGLE_OFFSET,
		   winw - 2 * MENU_RECTANGLE_OFFSET - 1,
		   winh - 2 * MENU_RECTANGLE_OFFSET - 1);
    
    return;
}

#if (SUPPORT_TRUETYPE_IMLIB2)

void
menu_draw_string_imlib2(Rwindow_t *window)
{
    int textw, texth;
    unsigned int winw, winh;
    unsigned int dummyui;
    int dummyi;
    size_t len;
    char *str;
    Window dummywin;
    Imlib_Image textimg;
    char blend;

    if (window->im2title) {
        XGetGeometry(window->app->display, window->id, &dummywin,
                     &dummyi, &dummyi, &winw, &winh, &dummyui, &dummyui);
        imlib_context_set_image(window->im2title);
        imlib_context_set_drawable(window->id);
        blend = imlib_context_get_blend();
        imlib_context_set_blend(1);
        imlib_render_image_on_drawable(window->menu->lefttextoffset,
                                       (winh - window->im2titleh) / 2);
        if (window->menuitem) {
            textimg = NULL;
            if (window->menuitem->type == MENUITEM_TOGGLE) {
                if (!window->menuitem->im2true) {
                    imlib_context_set_font(window->im2font);
                    imlib_get_text_size(window->menuitem->truetext,
                                        &textw, &texth);
                    textimg = imlib_create_image(textw, texth);
                    if (textimg) {
                        imlib_context_set_image(textimg);
                        imlib_image_set_has_alpha(1);
                        imlib_image_clear();
                        imlib_context_set_color(204, 181, 59, 255);
                        imlib_text_draw(0, 0, window->menuitem->truetext);
                        window->menuitem->im2true = textimg;
                        window->menuitem->im2truew = textw;
                        window->menuitem->im2trueh = texth;
                    }
                }
                if (!window->menuitem->im2false) {
                    imlib_context_set_font(window->im2font);
                    imlib_get_text_size(window->menuitem->falsetext,
                                        &textw, &texth);
                    textimg = imlib_create_image(textw, texth);
                    if (textimg) {
                        imlib_context_set_image(textimg);
                        imlib_image_set_has_alpha(1);
                        imlib_image_clear();
                        imlib_context_set_color(204, 181, 59, 255);
                        imlib_text_draw(0, 0, window->menuitem->falsetext);
                        window->menuitem->im2false = textimg;
                        window->menuitem->im2falsew = textw;
                        window->menuitem->im2falseh = texth;
                    }
                }
                if (*((unsigned long *)window->menuitem->option.valueptr)
                    & window->menuitem->option.flag) {
                    str = window->menuitem->truetext;
                    textimg = window->menuitem->im2true;
                    textw = window->menuitem->im2truew;
                    texth = window->menuitem->im2trueh;
                } else {
                    str = window->menuitem->falsetext;
                    textimg = window->menuitem->im2false;
                    textw = window->menuitem->im2falsew;
                    texth = window->menuitem->im2falseh;
                }
            } else if (window->menuitem->righttext) {
                if (!window->menuitem->im2right) {
                    imlib_context_set_font(window->im2font);
                    imlib_get_text_size(window->menuitem->righttext,
                                        &textw, &texth);
                    textimg = imlib_create_image(textw, texth);
                    if (textimg) {
                        imlib_context_set_image(textimg);
                        imlib_image_set_has_alpha(1);
                        imlib_image_clear();
                        imlib_context_set_color(204, 181, 59, 255);
                        imlib_text_draw(0, 0, window->menuitem->righttext);
                        window->menuitem->im2right = textimg;
                        window->menuitem->im2rightw = textw;
                        window->menuitem->im2righth = texth;
                    }
                }
                str = window->menuitem->righttext;
                textimg = window->menuitem->im2right;
                textw = window->menuitem->im2rightw;
                texth = window->menuitem->im2righth;
            }
            if (textimg) {
                imlib_context_set_image(textimg);
                imlib_context_set_drawable(window->id);
                imlib_render_image_on_drawable(winw - window->menu->righttextoffset - textw,
                                               (winh - texth) / 2);
            } else {
#if 0
                len = strlen(str);
                if (len) {
                    XDrawString(window->app->display, window->id, window->gc,
                                winw - window->menu->righttextoffset
                                - len * FONT_WIDTH(window->fontinfo),
                                (winh + FONT_HEIGHT(window->fontinfo)) / 2,
                                str, len);
                }
#endif
            }
        }
        imlib_context_set_blend(blend);
    } else {
        menu_draw_string(window);
    }

    return;
}

#endif /* SUPPORT_TRUETYPE_IMLIB2 */

void
menu_draw_string(Rwindow_t *window)
{
    unsigned int winw, winh;
    unsigned int dummyui;
    int dummyi;
    size_t len;
    char *str;
    Window dummywin;

    if (window == NULL) {

	return;
    }

    XGetGeometry(window->app->display, window->id, &dummywin,
		 &dummyi, &dummyi, &winw, &winh, &dummyui, &dummyui);

    XDrawString(window->app->display, window->id, window->gc,
		window->menu->lefttextoffset,
		(winh + FONT_HEIGHT(window->fontinfo)) / 2, 
		window->title, window->titlelen);

    if (window->menuitem) {
	if (window->menuitem->type == MENUITEM_TOGGLE) {
	    if (*((unsigned long *)window->menuitem->option.valueptr)
		& window->menuitem->option.flag) {
		str = window->menuitem->truetext;
	    } else {
		str = window->menuitem->falsetext;
	    }
	    
	    len = strlen(str);
	    if (len) {
		XDrawString(window->app->display, window->id, window->gc,
			    winw - window->menu->righttextoffset
			    - len * FONT_WIDTH(window->fontinfo),
			    (winh + FONT_HEIGHT(window->fontinfo)) / 2,
			    str, len);
	    }
	} else if (window->menuitem->righttext) {
	    str = window->menuitem->righttext;

	    len = strlen(str);
	    if (len) {
		XDrawString(window->app->display, window->id, window->gc,
			    winw - window->menu->righttextoffset
			    - len * FONT_WIDTH(window->fontinfo),
			    (winh + FONT_HEIGHT(window->fontinfo)) / 2,
			    str, len);
	    }
	}
    }

    return;
}

void
menu_draw_arrow(Rwindow_t *window)
{
    unsigned int winw, winh;
    unsigned int dummyui;
    int dummyi;
    Window dummywin;
    XPoint points[3];

    if (window == NULL) {

	return;
    }

    XGetGeometry(window->app->display, window->id, &dummywin,
		 &dummyi, &dummyi, &winw, &winh, &dummyui, &dummyui);
    
    points[0].x = winw - winh / 4
	- MENU_RECTANGLE_OFFSET - MENU_DEFAULT_LINE_WIDTH - 4;
    points[0].y = MENU_RECTANGLE_OFFSET + MENU_DEFAULT_LINE_WIDTH + 4;

    points[1].x = winw - MENU_RECTANGLE_OFFSET - MENU_DEFAULT_LINE_WIDTH - 4;
    points[1].y = winh / 2;

    points[2].x = points[0].x;
    points[2].y = winh - MENU_RECTANGLE_OFFSET - MENU_DEFAULT_LINE_WIDTH - 5;
    
    XFillPolygon(window->app->display, window->id, window->gc,
		 points, 3, Convex, CoordModeOrigin);
    
    return;
}

void
menu_draw_window(Rwindow_t *window)
{
    Rmenuitem_t *menuitem;
    int state;

    if ((window_render_background_imlib2(window,
					 &window->images[WINDOW_BACKGROUND_IMAGE])
	 < 0)
	&& (window->menu) && (window->menu->nitems)) {
	menu_draw_arrow(window);
    } else {
        window_apply_pixmap(window,
                            &window->images[WINDOW_BACKGROUND_IMAGE],
                            0, 0,
                            WINDOW_IMAGE_TILE | WINDOW_IMAGE_MASK);
    }
#if (SUPPORT_TRUETYPE_IMLIB2)
    menu_draw_string_imlib2(window);
#else
    menu_draw_string(window);
#endif
}
#endif /* 0 */

void
Rterm_clear_screen_buffer(struct R_termscreen *screen, int row, int column, int len, int mode)
{
    Pixmap pixmap;
    if (screen == NULL) {

	return;
    }

    if (screen->buf) {
        pixmap = screen->drawpixmap;
	if (mode == RTERM_SCREEN_CLEAR_BUFFER) {
            if (pixmap) {
                XCopyArea(screen->window->app->display,
                          pixmap,
                          screen->buf,
                          screen->bufgc,
                          0, 0,
                          screen->winw, screen->winh,
                          0, 0);
            } else {
		XFillRectangle(screen->window->app->display,
			       screen->buf,
			       screen->bufgc,
			       0, 0,
			       screen->winw,
			       screen->winh);
	    }
	} else {
            if (pixmap) {
                XCopyArea(screen->window->app->display,
                          pixmap,
                          screen->buf,
                          screen->bufgc,
                          Rterm_screen_column_x(screen, column),
                          Rterm_screen_row_y(screen, row),
                          screen->charw * len, screen->charh,
                          Rterm_screen_column_x(screen, column),
                          Rterm_screen_row_y(screen, row));
	    } else {
		XFillRectangle(screen->window->app->display,
			       screen->buf,
			       screen->bufgc,
			       Rterm_screen_column_x(screen, column),
			       Rterm_screen_row_y(screen, row),
			       screen->charw * len,
			       screen->charh);
	    }
	}
    }

    return;
}

void
Rterm_sync_screen(struct R_termscreen *screen, int row, int column, int len, int mode)
{
    if (screen == NULL) {

	return;
    }

    if (screen->buf) {
	if (mode == RTERM_SCREEN_SYNC) {
	    XSetWindowBackgroundPixmap(screen->window->app->display,
				       screen->window->id,
				       screen->buf);
	    XClearWindow(screen->window->app->display,
			 screen->window->id);
            XSync(screen->window->app->display, False);
	} else {
	    XCopyArea(screen->window->app->display,
		      screen->buf,
		      screen->window->id,
		      screen->bufgc,
		      Rterm_screen_column_x(screen, column),
		      Rterm_screen_row_y(screen, row),
		      screen->charw * len,
		      screen->charh,
		      Rterm_screen_column_x(screen, column),
		      Rterm_screen_row_y(screen, row));
	}
    }

    return;
}

void
Rterm_draw_screen_selection(struct R_termscreen *screen, int row, int column, int len)
{
    struct R_term *term;
    Drawable buffer;
    void (*blank_drawn)(struct R_termscreen *, int, int, int);

    if (screen == NULL
	|| row < 0
	|| row > screen->rows - 1
	|| column < 0
	|| column > screen->columns - 1
	|| len < 0) {

	return;
    }

    term = R_global.app->client;
    blank_drawn = screen->funcs.blank_drawn;
    if (blank_drawn == NULL) {

	return;
    }
    if (screen->buf) {
	buffer = screen->buf;
    } else {
	buffer = screen->window->id;
    }
    XFillRectangle(screen->window->app->display,
		   buffer,
		   term->selectiongc,
		   Rterm_screen_column_x(screen, column),
		   Rterm_screen_row_y(screen, row),
		   screen->charw * len,
		   screen->charh);
    if (screen->buf) {
	Rterm_sync_screen(screen, row, column, len, RTERM_SCREEN_SYNC_STRING);
    }

    return;
}

#if (SUPPORT_RTERM_UTF8_CHARS)
#if (SUPPORT_TRUETYPE_FONTS)
void
Rterm_draw_screen_string_freetype_wc(struct R_termscreen *screen,
                                     GC gc,
                                     Drawable drawable,
                                     int row, int column, int len)
{
#if (SUPPORT_RTERM_UTF8_CHARS)
    struct R_term *term = R_global.app->client;
#endif
    struct R_app *app;
    Rterm_wchar_t *text;
    R_text_t *textflags;
    Pixmap bitmap;
    int c;
    int bufferrow;
    int x, y;
    int fgcolor, bgcolor;
    int renderx, rendery;
    int charw, charh;
    int renderw, renderh;

    app = screen->window->app;
    bufferrow = screen->viewrow + row;
#if (SUPPORT_RTERM_UTF8_CHARS)
    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
        text = screen->textbuf.wdata[bufferrow] + column;
    } else {
        text = screen->textbuf.data[bufferrow] + column;
    }
#else
    text = screen->textbuf.data[bufferrow] + column;
#endif
//    textflags = screen->textbuf.renddata[bufferrow] + column;
    charw = screen->charw;
    charh = screen->charh;
    x = column * charw;
    y = Rterm_screen_row_y(screen, row);
    renderw = charw * len;
    renderh = charh;
    while (len--) {
	c = *text;
	bitmap = screen->ftfont->bitmaps[c];
	if (bitmap) {
#if (!SUPPORT_TRUETYPE_ANTIALIAS)
	    XSetClipMask(app->display, gc, bitmap);
	    XSetClipOrigin(app->display, gc, x, y);
	    XCopyPlane(app->display,
		       bitmap,
		       drawable,
		       gc,
		       0, 0,
		       charw, charh,
		       x, y,
		       1UL);
#else
            XCopyArea(app->display,
                      bitmap,
                      drawable,
                      gc,
                      0,
                      0,
                      charw, charh,
                      x, y);
#endif
	}
	x += charw;
	column++;
	text++;
    }
    XSetClipMask(app->display, gc, None);

    return;
}

void
Rterm_draw_screen_string_freetype(struct R_termscreen *screen,
                                  GC gc,
                                  Drawable drawable,
                                  int row, int column, int len)
{
    struct R_app *app;
    R_text_t *text;
    R_text_t *textflags;
    Pixmap bitmap;
    int c;
    int bufferrow;
    int x, y;
    int fgcolor, bgcolor;
    int renderx, rendery;
    int charw, charh;
    int renderw, renderh;

    app = screen->window->app;
    bufferrow = screen->viewrow + row;
    text = screen->textbuf.data[bufferrow] + column;
//    textflags = screen->textbuf.renddata[bufferrow] + column;
    charw = screen->charw;
    charh = screen->charh;
    x = column * charw;
    y = Rterm_screen_row_y(screen, row);
    renderw = charw * len;
    renderh = charh;
    while (len--) {
	c = *text;
	bitmap = screen->ftfont->bitmaps[c];
	if (bitmap) {
#if (!SUPPORT_TRUETYPE_ANTIALIAS)
	    XSetClipMask(app->display, gc, bitmap);
	    XSetClipOrigin(app->display, gc, x, y);
	    XCopyPlane(app->display,
		       bitmap,
		       drawable,
		       gc,
		       0, 0,
		       charw, charh,
		       x, y,
		       1UL);
#else
            XCopyArea(app->display,
                      bitmap,
                      drawable,
                      gc,
                      0,
                      0,
                      charw, charh,
                      x, y);
#endif
	}
	x += charw;
	column++;
	text++;
    }
    XSetClipMask(app->display, gc, None);

    return;
}

#endif

void
Rterm_draw_screen_string_wc(struct R_termscreen *screen, int row, int column,
                            Rterm_wchar_t *text, R_textflags_t *textflags, int len,
                            int mode,
                            int sync)
{
    struct R_term *term;
    Rterm_wchar_t *drawntext;
    R_textflags_t *drawnflags;
    int stringlen;
#if (SUPPORT_RTERM_BLINKING_CHARS)
    int blink;
#endif /* SUPPORT_RTERM_BLINKING_CHARS) */
    int (*draw_string)(Display *, Drawable, GC, int, int, _Xconst char *, int);
    Drawable buffer;
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    Pixmap pixmap;
    int firstcolumn;
#if !KLUDGE
    int copylen;
#endif
#endif
#if (SUPPORT_RTERM_UTF8_CHARS)
    XChar2b drawbuf[512];
    Rterm_wchar_t *textptr;
    int i;
    XChar2b *drawstr = drawbuf;
#endif
#if (SUPPORT_TRUETYPE_FONTS)
    int drawbg = 0;
#endif

    if (screen == NULL
	|| row < 0
	|| row > screen->rows - 1
	|| column < 0
	|| column > screen->columns - 1
	|| text == NULL
	|| textflags == NULL
	|| len < 0) {

	return;
    }

    len = MIN(len, screen->columns - column);
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    if (screen->buf) {
	buffer = screen->buf;
    } else {
	buffer = screen->window->id;
    }
#else
    buffer = screen->window->id;
#endif

#if (SUPPORT_RTERM_BLINKING_CHARS)
    term = R_global.app->client;
    blink = term->blink;
#endif /* SUPPORT_RTERM_BLINKING_CHARS */

    textptr = text;
    for (i = 0 ; i < len ; i++) {
        drawbuf[i].byte1 = (*textptr & 0xff00) >> 8;
        drawbuf[i].byte2 = *textptr & 0xff;
        textptr++;
    }
    while (len) {
	stringlen = 0;
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
        firstcolumn = column;
#if !KLUDGE
        copylen = 0;
#endif
#endif

#if (SUPPORT_RTERM_UTF8_CHARS)
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
            if (!(*textflags & RTERM_CHAR_NO_BACKGROUND)) {
#if (SUPPORT_TRUETYPE_FONTS)
                XSetForeground(screen->window->app->display, screen->bggc,
                               screen->colors[RTERM_CHAR_BG_COLOR(*textflags)]);
                drawbg = 1;
#endif            
                draw_string = XDrawImageString16;
            } else {
                draw_string = XDrawString16;
            }
        } else 	if (!(*textflags & RTERM_CHAR_NO_BACKGROUND)) {
	    draw_string = XDrawImageString;
	} else {
	    draw_string = XDrawString;
	}
#else
	if (!(*textflags & RTERM_CHAR_NO_BACKGROUND)) {
	    draw_string = XDrawImageString;
	} else {
	    draw_string = XDrawString;
	}
#endif

	drawntext = screen->drawbuf.wdata[row];
	drawnflags = screen->drawbuf.renddata[row];
	if (mode == RTERM_SCREEN_DRAW_ALL) {
	    while ((len)
		   && *text == *drawntext++
		   && *textflags == *drawnflags++) {
#if (SUPPORT_RTERM_BLINKING_CHARS)
		if (*textflags & RTERM_CHAR_BLINKING) {
		    screen->flags |= RTERM_SCREEN_HAS_BLINKING_CHARS;
		}
#endif /* SUPPORT_RTERM_BLINKING_CHARS */

		column++;
		text++;
		textflags++;
		len--;
                drawstr++;
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
#if !KLUDGE
		copylen++;
#endif
#endif
	    }
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
#if !KLUDGE
            if ((sync) && (term->flags & RTERM_DOUBLE_BUFFER)) {
                Rterm_sync_screen(screen, row, firstcolumn, copylen, RTERM_SCREEN_SYNC_STRING);
            }
#endif
#endif

	    if (len == 0) {
		
		return;
	    }

	    screen->drawbuf.rowdirty[row] = 1;
	}
	
	if (*textflags == RTERM_SCREEN_DEFAULT_TEXTFLAGS) {
#if (SUPPORT_RTERM_BLINKING_CHARS)
	    if (mode == RTERM_SCREEN_DRAW_BLINKING_ONLY) {
		while (len && *textflags == RTERM_SCREEN_DEFAULT_TEXTFLAGS) {
		    column++;
		    text++;
		    textflags++;
		    len--;
                    drawstr++;
		}
	    }

	    if (len == 0) {
		screen->drawbuf.rowdirty[row] = 0;

		return;
	    }
#endif

	    while (len && *textflags == RTERM_SCREEN_DEFAULT_TEXTFLAGS) {
		textflags++;
		len--;
		stringlen++;
	    }

#if (SUPPORT_RTERM_BLINKING_CHARS)
	    if (mode == RTERM_SCREEN_DRAW_ALL) {
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
		if (!screen->buf) {
		    Rterm_clear_screen_string(screen, row, column, stringlen);
		} else {
		    Rterm_clear_screen_buffer(screen, row, column, stringlen,
                                              RTERM_SCREEN_CLEAR_STRING);
		}
#else
		Rterm_clear_screen_string(screen, row, column, stringlen);
#endif
                RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, screen->defaultgc, drawbg);
	    }
#else /* !SUPPORT_RTERM_BLINKING_CHARS */
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
	    if (!screen->buf) {
		Rterm_clear_screen_string(screen, row, column, stringlen);
	    } else {
		Rterm_clear_screen_buffer(screen, row, column, stringlen,
                                          RTERM_SCREEN_CLEAR_STRING);
	    }
#else
	    Rterm_clear_screen_string(screen, row, column, stringlen);
#endif
            RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, screen->defaultgc, drawbg);
#if !KLUDGE
	    if (sync) {
		Rterm_sync_screen(screen, row, column, stringlen, RTERM_SCREEN_SYNC_STRING);
	    }
#endif
#endif /* SUPPORT_RTERM_BLINKING_CHARS */
	} else {
#if (SUPPORT_RTERM_BLINKING_CHARS)
	    if (mode == RTERM_SCREEN_DRAW_BLINKING_ONLY) {
		while (len && !(*textflags & RTERM_CHAR_BLINKING)) {
		    column++;
		    text++;
		    textflags++;
		    len--;
                    drawstr++;
		}
	    }
	    
	    if (len == 0) {
		screen->drawbuf.rowdirty[row] = 0;
		
		return;
	    }
#endif /* SUPPORT_RTERM_BLINKING_CHARS */

	    if (*textflags != screen->gctextflags) {
		Rterm_set_screen_gc(screen, *textflags);
	    }
	    
#if (SUPPORT_RTERM_BLINKING_CHARS)
	    if (*textflags & RTERM_CHAR_BLINKING) {
		screen->flags |= RTERM_SCREEN_HAS_BLINKING_CHARS;
	    }
#endif /* SUPPORT_RTERM_BLINKING_CHARS */
	    
	    while (len && *textflags == screen->gctextflags) {
		textflags++;
		len--;
		stringlen++;
	    }
	    
#if (SUPPORT_RTERM_BLINKING_CHARS)
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
	    if (screen->buf) {
		if (!(screen->gctextflags & RTERM_CHAR_BLINKING) || (blink)) {
		    Rterm_clear_screen_buffer(screen, row, column, stringlen,
					RTERM_SCREEN_CLEAR_STRING);
		} else {
		    Rterm_clear_screen_buffer(screen, row, column, stringlen,
					RTERM_SCREEN_CLEAR_STRING);
		}
                RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, screen->gc, drawbg);
	    } else {
		Rterm_clear_screen_string(screen, row, column, stringlen);
		
		if (!(screen->gctextflags & RTERM_CHAR_BLINKING) || (blink)) {
                    RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, screen->gc, drawbg);
                }
	    }

#if !KLUDGE
	    if (sync) {
		Rterm_sync_screen(screen, row, column, stringlen, RTERM_SCREEN_SYNC_STRING);
	    }
#endif
#else
	    Rterm_clear_screen_string(screen, row, column, stringlen);
	    
	    if (!(screen->gctextflags & RTERM_CHAR_BLINKING) || (blink)) {
                RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, drawbg);
#if !KLUDGE
		if (sync) {
		    Rterm_sync_screen(screen, row, column, stringlen, RTERM_SCREEN_SYNC_STRING);
		}
#endif
	    }
#endif
#else /* !SUPPORT_RTERM_BLINKING_CHARS */
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
	    if (!screen->buf) {
		Rterm_clear_screen_string(screen, row, column, stringlen);
	    } else {
		Rterm_clear_screen_buffer(screen, row, column, stringlen,
				    RTERM_SCREEN_CLEAR_STRING);
	    }
#else
	    Rterm_clear_screen_string(screen, row, column, stringlen);
#endif
            RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, screen->gc, drawbg); 
#if !KLUDGE
	    if (sync) {
		Rterm_sync_screen(screen, row, column, stringlen, RTERM_SCREEN_SYNC_STRING);
	    }
#endif
#endif /* SUPPORT_RTERM_BLINKING_CHARS */
	}
	if (screen->gctextflags & RTERM_CHAR_SELECTED) {
	    Rterm_draw_screen_selection(screen, row, column, stringlen);
	    screen->gctextflags &= ~RTERM_CHAR_SELECTED;
	}
	
	column += stringlen;
	text += stringlen;
	stringlen = 0;
    }

    return;
}
#endif

void
Rterm_draw_screen_string_8bit(struct R_termscreen *screen, int row, int column,
                              R_text_t *text, R_textflags_t *textflags, int len,
                              int mode,
                              int sync)
{
    struct R_term *term;
    R_text_t *drawntext;
    R_textflags_t *drawnflags;
    int stringlen;
#if (SUPPORT_RTERM_BLINKING_CHARS)
    int blink;
#endif /* SUPPORT_RTERM_BLINKING_CHARS) */
    int (*draw_string)(Display *, Drawable, GC, int, int, _Xconst char *, int);
    Drawable buffer;
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    Pixmap pixmap;
    int firstcolumn;
#if !KLUDGE
    int copylen;
#endif
#endif
#if (SUPPORT_TRUETYPE_FONTS)
    int drawbg = 0;
#endif
    XChar2b *drawstr;

    if (screen == NULL
	|| row < 0
	|| row > screen->rows - 1
	|| column < 0
	|| column > screen->columns - 1
	|| text == NULL
	|| textflags == NULL
	|| len < 0) {

	return;
    }

    len = MIN(len, screen->columns - column);
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    if (screen->buf) {
	buffer = screen->buf;
    } else {
	buffer = screen->window->id;
    }
#else
    buffer = screen->window->id;
#endif

#if (SUPPORT_RTERM_BLINKING_CHARS) || (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    term = R_global.app->client;
#endif
#if (SUPPORT_RTERM_BLINKING_CHARS) || (SUPPORT_RTERM_BLINKING_CURSOR)
    blink = term->blink;
#endif /* SUPPORT_RTERM_BLINKING_CHARS */

    while (len) {
	stringlen = 0;
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
	firstcolumn = column;
#if !KLUDGE
	copylen = 0;
#endif
#endif

        if (!(*textflags & RTERM_CHAR_NO_BACKGROUND)) {
#if (SUPPORT_TRUETYPE_FONTS)
            XSetForeground(screen->window->app->display, screen->bggc,
                           screen->colors[RTERM_CHAR_BG_COLOR(*textflags)]);
            drawbg = 1;
#endif            
            draw_string = XDrawImageString;
        } else {
            draw_string = XDrawString;
        }

	drawntext = screen->drawbuf.data[row];
	drawnflags = screen->drawbuf.renddata[row];
	if (mode == RTERM_SCREEN_DRAW_ALL) {
	    while ((len)
		   && *text == *drawntext++
		   && *textflags == *drawnflags++) {
#if (SUPPORT_RTERM_BLINKING_CHARS)
		if (*textflags & RTERM_CHAR_BLINKING) {
		    screen->flags |= RTERM_SCREEN_HAS_BLINKING_CHARS;
		}
#endif /* SUPPORT_RTERM_BLINKING_CHARS */

		column++;
		text++;
		textflags++;
		len--;
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
#if !KLUDGE
		copylen++;
#endif
#endif
	    }
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
#if !KLUDGE
            if ((sync) && term->flags & RTERM_DOUBLE_BUFFER) {
                Rterm_sync_screen(screen, row, firstcolumn, copylen, RTERM_SCREEN_SYNC_STRING);
            }
#endif
#endif

	    if (len == 0) {
		
		return;
	    }

	    screen->drawbuf.rowdirty[row] = 1;
	}
	
	if (*textflags == RTERM_SCREEN_DEFAULT_TEXTFLAGS) {
#if (SUPPORT_RTERM_BLINKING_CHARS)
	    if (mode == RTERM_SCREEN_DRAW_BLINKING_ONLY) {
		while (len && *textflags == RTERM_SCREEN_DEFAULT_TEXTFLAGS) {
		    column++;
		    text++;
		    textflags++;
		    len--;
		}
	    }

	    if (len == 0) {
		screen->drawbuf.rowdirty[row] = 0;

		return;
	    }
#endif

	    while (len && *textflags == RTERM_SCREEN_DEFAULT_TEXTFLAGS) {
		textflags++;
		len--;
		stringlen++;
	    }

#if (SUPPORT_RTERM_BLINKING_CHARS)
	    if (mode == RTERM_SCREEN_DRAW_ALL) {
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
		if (!screen->buf) {
		    Rterm_clear_screen_string(screen, row, column, stringlen);
		} else {
		    Rterm_clear_screen_buffer(screen, row, column, stringlen,
                                              RTERM_SCREEN_CLEAR_STRING);
		}
#else
		Rterm_clear_screen_string(screen, row, column, stringlen);
#endif
                RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, screen->defaultgc, drawbg);
	    }
#else /* !SUPPORT_RTERM_BLINKING_CHARS */
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
	    if (!screen->buf) {
		Rterm_clear_screen_string(screen, row, column, stringlen);
	    } else {
		Rterm_clear_screen_buffer(screen, row, column, stringlen,
				    RTERM_SCREEN_CLEAR_STRING);
	    }
#else
	    Rterm_clear_screen_string(screen, row, column, stringlen);
#endif
            RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, screen->defaultgc, drawbg);
#if (!KLUDGE)
	    if (sync) {
		Rterm_sync_screen(screen, row, column, stringlen, RTERM_SCREEN_SYNC_STRING);
	    }
#endif
#endif /* SUPPORT_RTERM_BLINKING_CHARS */
	} else {
#if (SUPPORT_RTERM_BLINKING_CHARS)
	    if (mode == RTERM_SCREEN_DRAW_BLINKING_ONLY) {
		while (len && !(*textflags & RTERM_CHAR_BLINKING)) {
		    column++;
		    text++;
		    textflags++;
		    len--;
		}
	    }
	    
	    if (len == 0) {
		screen->drawbuf.rowdirty[row] = 0;
		
		return;
	    }
#endif /* SUPPORT_RTERM_BLINKING_CHARS */

	    if (*textflags != screen->gctextflags) {
		Rterm_set_screen_gc(screen, *textflags);
	    }
	    
#if (SUPPORT_RTERM_BLINKING_CHARS)
	    if (*textflags & RTERM_CHAR_BLINKING) {
		screen->flags |= RTERM_SCREEN_HAS_BLINKING_CHARS;
	    }
#endif /* SUPPORT_RTERM_BLINKING_CHARS */
	    
	    while (len && *textflags == screen->gctextflags) {
		textflags++;
		len--;
		stringlen++;
	    }
	    
#if (SUPPORT_RTERM_BLINKING_CHARS)
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
	    if (screen->buf) {
		if (!(screen->gctextflags & RTERM_CHAR_BLINKING) || (blink)) {
		    Rterm_clear_screen_buffer(screen, row, column, stringlen,
					RTERM_SCREEN_CLEAR_STRING);
                    RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, screen->gc, drawbg);
		} else {
		    Rterm_clear_screen_buffer(screen, row, column, stringlen,
					RTERM_SCREEN_CLEAR_STRING);
		}
	    } else {
		Rterm_clear_screen_string(screen, row, column, stringlen);
		
		if (!(screen->gctextflags & RTERM_CHAR_BLINKING) || (blink)) {
                    RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, screen->gc, drawbg);
		}
	    }

#if !KLUDGE
	    if (sync) {
		Rterm_sync_screen(screen, row, column, stringlen, RTERM_SCREEN_SYNC_STRING);
#endif
	    }
#else
	    Rterm_clear_screen_string(screen, row, column, stringlen);
	    
	    if (!(screen->gctextflags & RTERM_CHAR_BLINKING) || (blink)) {
                RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, screen->gc, drawbg);
#if !KLUDGE
		if (sync) {
		    Rterm_sync_screen(screen, row, column, stringlen, RTERM_SCREEN_SYNC_STRING);
		}
#endif
	    }
#endif
#else /* !SUPPORT_RTERM_BLINKING_CHARS */
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
	    if (!screen->buf) {
		Rterm_clear_screen_string(screen, row, column, stringlen);
	    } else {
		Rterm_clear_screen_buffer(screen, row, column, stringlen,
				    RTERM_SCREEN_CLEAR_STRING);
	    }
#else
	    Rterm_clear_screen_string(screen, row, column, stringlen);
#endif
            RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, screen->gc, drawbg);
#if !KLUDGE
	    if (sync) {
		Rterm_sync_screen(screen, row, column, stringlen, RTERM_SCREEN_SYNC_STRING);
	    }
#endif
#endif /* SUPPORT_RTERM_BLINKING_CHARS */
	}
	if (screen->gctextflags & RTERM_CHAR_SELECTED) {
	    Rterm_draw_screen_selection(screen, row, column, stringlen);
	    screen->gctextflags &= ~RTERM_CHAR_SELECTED;
	}
	
	column += stringlen;
	text += stringlen;
	stringlen = 0;
    }

    return;
}

#if (SUPPORT_RTERM_UTF8_CHARS)

#if 0
void
Rterm_draw_screen_string_freetype(struct R_termscreen *screen, Drawable drawable,
			    int row, int column, int len)
{
    struct R_app *app;
    R_text_t *text;
    R_text_t *textflags;
    Pixmap pixmap, bitmap;
    GC gc;
    int c;
    int bufferrow;
    int x, y;
    int renderx, rendery;
    int charw, charh;
    int renderw, renderh;

    app = screen->window->app;
    bufferrow = screen->viewrow + row;
    text = screen->textbuf.data[bufferrow] + column;
    textflags = screen->textbuf.renddata[bufferrow] + column;
    charw = screen->charw;
    charh = screen->charh;
    x = column * charw;
    renderx = x;
    y = Rterm_screen_row_y(screen, row);
    rendery = y;
    renderw = charw * len;
    renderh = charh;
    bitmap = XCreatePixmap(app->display,
			   screen->window->id,
			   charw * len, charh,
			   1);
    if (*textflags) {
	gc = screen->gc;
    } else {
	gc = screen->defaultgc;
    }
    while (len--) {
	c = *text;
	XCopyArea(app->display,
		  screen->ftfont->bitmaps[c],
		  bitmap,
		  gc,
		  0, 0,
		  screen->ftfont->heights[c],
		  screen->ftfont->widths[c],
		  (charh - screen->ftfont->heights[c]) / 2,
		  (charh - screen->ftfont->heights[c]) / 2);
	x += charw;
	column++;
	text++;
    }
#if 0
    XSetClipMask(app->display, gc, bitmap);
    XSetClipOrigin(app->display, gc, renderx, rendery);
    fprintf(stderr, "renderw == %d, renderh == %d\n", renderw, renderh);
#endif
    XCopyArea(app->display,
	      bitmap,
	      drawable,
	      gc,
	      0, 0,
	      renderw, renderh,
	      renderx, rendery);
    XFreePixmap(app->display, bitmap);

    return;
}
#endif

#endif

#if (SUPPORT_TRUETYPE_IMLIB2)
void
Rterm_draw_screen_string_imlib2(struct R_termscreen *screen, int row, int column, int len)
{
    R_text_t *text;
    R_textflags_t *textflags;
    int bufferrow;
    int bgcolor, fgcolor;
    int textw, texth;
    struct imlib2color *color;
    Imlib_Image *textimg;

    if (screen == NULL) {

	return;
    }
    text = screen->textbuf.data[screen->viewrow + row] + column;
    textflags = screen->textbuf.renddata[screen->viewrow + row] + column;
    memcpy(screen->stringbuffer, text, len);
    screen->stringbuffer[len] = '\0';
    fprintf(stderr, "string: %s\n", screen->stringbuffer);
    fgcolor = RTERM_CHAR_FG_COLOR(*textflags);
    if (fgcolor) {
	color = &screen->im2colors[fgcolor];
    } else {
	color = &screen->im2colors[RTERM_SCREEN_FOREGROUND];
    }
#if 0
    fprintf(stderr, "red == %d, green == %d, blue == %d, alpha == %d\n",
	    color->red, color->green, color->blue, color->alpha);
#endif
    imlib_context_set_font(screen->im2font);
    imlib_context_set_color(color->red, color->green, color->blue,
			    color->alpha);
    imlib_context_set_drawable(screen->buf);
    imlib_get_text_size(screen->stringbuffer,
                        &textw, &texth);
    textimg = imlib_create_image(textw, texth);
    imlib_context_set_image(textimg);
    imlib_image_clear();
    imlib_image_set_has_alpha(1);
    imlib_context_set_blend(0);
    imlib_context_set_anti_alias(0);
    imlib_text_draw(0,
                    0,
		    screen->stringbuffer);
    imlib_render_image_on_drawable(Rterm_screen_column_x(screen, column),
                                   Rterm_screen_row_y(screen, column));
    imlib_free_image();

    return;
}
#endif

void
Rterm_clear_screen_cursor(struct R_termscreen *screen)
{
    if (screen == NULL) {

	return;
    }

    XFillRectangle(screen->window->app->display, screen->window->id,
                   screen->cursorgc,
                   Rterm_screen_cursor_x(screen), Rterm_screen_cursor_y(screen),
                   screen->charw, screen->charh);

    return;
}

void
Rterm_draw_screen_cursor(struct R_termscreen *screen)
{
    if (screen == NULL) {

	return;
    }

    XFillRectangle(screen->window->app->display, screen->window->id,
                   screen->cursorgc,
                   Rterm_screen_cursor_x(screen), Rterm_screen_cursor_y(screen),
                   screen->charw, screen->charh);

    return;
}

