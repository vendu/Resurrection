/*
 * selection.c - selection handling for Resurrection.
 * Copyright (C) 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

//void screen_remove_selection(struct R_termscreen *screen, Time time);

Window
R_get_selection_owner(struct R_window *window, Atom atom)
{
    Window retw;

    retw = XGetSelectionOwner(window->app->display, atom);

    return retw;
}

void
R_set_selection_owner(struct R_window *window, Atom atom, Time timestamp)
{
    int retval;

    if (timestamp == 0) {
	retval = XSetSelectionOwner(window->app->display, atom, None,
				    CurrentTime);
    } else {
	retval = XSetSelectionOwner(window->app->display, atom, window->id,
				    timestamp);
    }
    XFlush(window->app->display);

    return;
}

int
R_convert_selection(struct R_window *window, Window requestor,
		  Atom atom, Atom target, Atom property, Time timestamp)
{
    int retval;

    retval = XConvertSelection(window->app->display, atom, target, property,
			       requestor, timestamp);

    return retval;
			       
}

void
Rterm_clear_screen_selection(struct R_termscreen *screen)
{
    struct R_term *term;
    R_textflags_t *textflags;
    void (*blank_string)(struct R_termscreen *, int, int, int);
    void (*blank_drawn)(struct R_termscreen *, int, int, int);
    int row, column;
    int firstrow, firstcolumn;
    int lastrow, lastcolumn;
    int startrow, startcolumn;
    int endrow, endcolumn;
    int drawrow;
    
    if (screen == NULL) {
	
	return;
    }

    term = R_global.app->client;
    firstrow = term->selection.firstrow;
    firstcolumn = term->selection.firstcolumn;
//    lastrow = MIN(term->selection.lastrow, screen->rows + screen->savelines - 1);
    lastrow = term->selection.lastrow;
    lastcolumn = term->selection.lastcolumn;
    if (firstrow < 0
	|| firstcolumn < 0
	|| lastrow < 0
	|| lastcolumn < 0) {
	
	return;
    }

#if 0
    if (term->selection.stringdata) {
	free(term->selection.stringdata);
	term->selection.stringdata = NULL;
	term->selection.stringlen = 0;
    }
    if (term->selection.utf8data) {
	free(term->selection.utf8data);
	term->selection.utf8data = NULL;
	term->selection.utf8len = 0;
    }

#endif

    blank_string = screen->funcs.blank_string;
    if (blank_string == NULL) {
	
	return;
    }
    blank_drawn = screen->funcs.blank_drawn;
    if (blank_drawn == NULL) {
	
	return;
    }

    term->selection.firstrow = -1;
    term->selection.firstcolumn = -1;
    term->selection.lastrow = -1;
    term->selection.lastcolumn = -1;

    row = firstrow;
    if (row == lastrow) {
	row = firstrow;
	column = firstcolumn;
	endcolumn = lastcolumn + 1;
        if (screen->textbuf.renddata[row]) {
            textflags = screen->textbuf.renddata[row] + column;
            while (column < endcolumn) {
                *textflags++ &= ~RTERM_CHAR_SELECTED;
                column++;
            }
            if (row >= screen->viewrow
                && row < screen->viewrow + screen->rows) {
                column = firstcolumn;
                drawrow = row - screen->viewrow;
                blank_drawn(screen,
                            drawrow, column,
                            endcolumn - column);
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    Rterm_draw_screen_string_wc(screen, drawrow, column,
                                                screen->textbuf.wdata[row] + column,
                                                screen->textbuf.renddata[row] + column,
                                                endcolumn - column,
                                                RTERM_SCREEN_DRAW_ALL,
                                                FALSE);
                } else {
                    Rterm_draw_screen_string_8bit(screen, drawrow, column,
                                                  screen->textbuf.data[row] + column,
                                                  screen->textbuf.renddata[row] + column,
                                                  endcolumn - column,
                                                  RTERM_SCREEN_DRAW_ALL,
                                                  FALSE);
                }
#else
                Rterm_draw_screen_string_8bit(screen, drawrow, column,
                                              screen->textbuf.data[row] + column,
                                              screen->textbuf.renddata[row] + column,
                                              endcolumn - column,
                                              RTERM_SCREEN_DRAW_ALL,
                                              FALSE);
#endif
            }
        }
    } else {
	row = firstrow;
	column = firstcolumn;
	endcolumn = screen->textbuf.rowcols[row];
        if (screen->textbuf.renddata[row]) {
            textflags = screen->textbuf.renddata[row] + column;
            while (column < endcolumn) {
                *textflags++ &= ~RTERM_CHAR_SELECTED;
                column++;
            }
            if (row >= screen->viewrow
                && row < screen->viewrow + screen->rows) {
                column = firstcolumn;
                drawrow = row - screen->viewrow;
                blank_drawn(screen,
                            drawrow, column,
                            screen->drawbuf.rowlens[drawrow] - column);
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    Rterm_draw_screen_string_wc(screen, drawrow, column,
                                                screen->textbuf.wdata[row] + column,
                                                screen->textbuf.renddata[row] + column,
                                                screen->textbuf.rowlens[row] - column,
                                                RTERM_SCREEN_DRAW_ALL,
                                                FALSE);
                } else {
                    Rterm_draw_screen_string_8bit(screen, drawrow, column,
                                                  screen->textbuf.data[row] + column,
                                                  screen->textbuf.renddata[row] + column,
                                                  screen->textbuf.rowlens[row] - column,
                                                  RTERM_SCREEN_DRAW_ALL,
                                                  FALSE);
            }
#else
                Rterm_draw_screen_string_8bit(screen, drawrow, column,
                                              screen->textbuf.data[row] + column,
                                              screen->textbuf.renddata[row] + column,
                                              screen->textbuf.rowlens[row] - column,
                                              RTERM_SCREEN_DRAW_ALL,
                                              FALSE);
#endif
            }
        }
	row++;
	while (row < lastrow) {
	    column = 0;
	    endcolumn = screen->textbuf.rowcols[row];
            if (screen->textbuf.renddata[row]) {
                textflags = screen->textbuf.renddata[row];
                while (column < endcolumn) {
                    *textflags++ &= ~RTERM_CHAR_SELECTED;
                    column++;
                }
                if (row >= screen->viewrow
                    && row < screen->viewrow + screen->rows) {
                    drawrow = row - screen->viewrow;
                    blank_drawn(screen,
                                drawrow, 0,
                                screen->drawbuf.rowcols[drawrow]);
#if (SUPPORT_RTERM_UTF8_CHARS)
                    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                        Rterm_draw_screen_string_wc(screen, drawrow, 0,
                                                    screen->textbuf.wdata[row],
                                                    screen->textbuf.renddata[row],
                                                    screen->textbuf.rowcols[row],
                                                    RTERM_SCREEN_DRAW_ALL,
                                                    FALSE);
                    } else {
                        Rterm_draw_screen_string_8bit(screen, drawrow, 0,
                                                      screen->textbuf.data[row],
                                                      screen->textbuf.renddata[row],
                                                      screen->textbuf.rowcols[row],
                                                      RTERM_SCREEN_DRAW_ALL,
                                                      FALSE);
                    }
#else
                    Rterm_draw_screen_string_8bit(screen, drawrow, 0,
                                                  screen->textbuf.data[row],
                                                  screen->textbuf.renddata[row],
                                                  screen->textbuf.rowcols[row],
                                                  RTERM_SCREEN_DRAW_ALL,
                                                  FALSE);
#endif
                }
            }
	    row++;
	}
	column = 0;
        if (screen->textbuf.rowlens[row]) {
            endcolumn = max(lastcolumn + 1, screen->textbuf.rowlens[row] - 1);
            textflags = screen->textbuf.renddata[row];
            while (column < endcolumn) {
                *textflags++ &= ~RTERM_CHAR_SELECTED;
                column++;
            }
            if (row >= screen->viewrow
                && row < screen->viewrow + screen->rows) {
                drawrow = row - screen->viewrow;
                blank_drawn(screen,
                            drawrow, 0,
                            endcolumn);
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    Rterm_draw_screen_string_8bit(screen, drawrow, 0,
                                                  screen->textbuf.wdata[row],
                                                  screen->textbuf.renddata[row],
                                                  endcolumn,
                                                  RTERM_SCREEN_DRAW_ALL,
                                                  FALSE);
                } else {
                    Rterm_draw_screen_string_8bit(screen, drawrow, 0,
                                                  screen->textbuf.data[row],
                                                  screen->textbuf.renddata[row],
                                                  endcolumn,
                                                  RTERM_SCREEN_DRAW_ALL,
                                                  FALSE);
                }
#else
                Rterm_draw_screen_string_8bit(screen, drawrow, 0,
                                              screen->textbuf.data[row],
                                              screen->textbuf.renddata[row],
                                              endcolumn,
                                              RTERM_SCREEN_DRAW_ALL,
                                              FALSE);
#endif
            }
        }
    }

    if (screen->buf) {
	Rterm_sync_screen(screen, 0, 0, 0, RTERM_SCREEN_SYNC);
    }
    XFlush(screen->window->app->display);
    
    return;
}

void
Rterm_highlight_selection(struct R_term *term)
{
    struct R_termscreen *screen;
    R_textflags_t *textflags;
    void (*blank_string)(struct R_termscreen *, int, int, int);
    void (*blank_drawn)(struct R_termscreen *, int, int, int);
    int row, column;
    int firstrow, firstcolumn;
    int lastrow, lastcolumn;
    int startrow, startcolumn;
    int endrow, endcolumn;
    int drawrow;

    
    screen = term->selection.screen;
    if (!screen) {

        return;
    }
    firstrow = term->selection.firstrow;
    firstcolumn = term->selection.firstcolumn;
    lastrow = MIN(term->selection.lastrow, screen->rows + screen->savelines - 1);
    lastcolumn = term->selection.lastcolumn;
    if (firstrow < 0
	|| firstcolumn < 0
	|| lastrow < 0
	|| lastcolumn < 0) {

	return;
    }

    blank_string = screen->funcs.blank_string;
    if (blank_string == NULL) {
	
	return;
    }
    blank_drawn = screen->funcs.blank_drawn;
    if (blank_drawn == NULL) {
	
	return;
    }

    row = firstrow;
    if (row == lastrow) {
	row = firstrow;
	column = firstcolumn;
	endcolumn = lastcolumn + 1;
        if (screen->textbuf.renddata[row]) {
            textflags = screen->textbuf.renddata[row] + column;
            while (column < endcolumn) {
                *textflags++ |= RTERM_CHAR_SELECTED;
                column++;
            }
            if (row >= screen->viewrow
                && row < screen->viewrow + screen->rows) {
                column = firstcolumn;
                drawrow = row - screen->viewrow;
                blank_drawn(screen,
                            drawrow, column,
                            endcolumn - column);
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    Rterm_draw_screen_string_wc(screen, drawrow, column,
                                                screen->textbuf.wdata[row] + column,
                                                screen->textbuf.renddata[row] + column,
                                                endcolumn - column,
                                                RTERM_SCREEN_DRAW_ALL,
                                                FALSE);
                } else {
                    Rterm_draw_screen_string_8bit(screen, drawrow, column,
                                                  screen->textbuf.data[row] + column,
                                                  screen->textbuf.renddata[row] + column,
                                                  endcolumn - column,
                                                  RTERM_SCREEN_DRAW_ALL,
                                                  FALSE);
                }
#else
                Rterm_draw_screen_string_8bit(screen, drawrow, column,
                                              screen->textbuf.data[row] + column,
                                              screen->textbuf.renddata[row] + column,
                                              endcolumn - column,
                                              RTERM_SCREEN_DRAW_ALL,
                                              FALSE);
#endif
            }
	}
    } else {
	row = firstrow;
	column = firstcolumn;
	endcolumn = screen->textbuf.rowcols[row];
        if (screen->textbuf.renddata[row]) {
            textflags = screen->textbuf.renddata[row] + column;
            while (column < endcolumn) {
                *textflags++ |= RTERM_CHAR_SELECTED;
                column++;
            }
            if (row >= screen->viewrow
                && row < screen->viewrow + screen->rows) {
                column = firstcolumn;
                drawrow = row - screen->viewrow;
                blank_drawn(screen,
                            drawrow, column,
                            screen->drawbuf.rowlens[drawrow] - column);
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    Rterm_draw_screen_string_8bit(screen, drawrow, column,
                                                  screen->textbuf.wdata[row] + column,
                                                  screen->textbuf.renddata[row] + column,
                                                  screen->textbuf.rowlens[row] - column,
                                                  RTERM_SCREEN_DRAW_ALL,
                                                  FALSE);
                } else {
                    Rterm_draw_screen_string_8bit(screen, drawrow, column,
                                                  screen->textbuf.data[row] + column,
                                                  screen->textbuf.renddata[row] + column,
                                                  screen->textbuf.rowlens[row] - column,
                                                  RTERM_SCREEN_DRAW_ALL,
                                                  FALSE);
                }
#else
                Rterm_draw_screen_string_8bit(screen, drawrow, column,
                                              screen->textbuf.data[row] + column,
                                              screen->textbuf.renddata[row] + column,
                                              screen->textbuf.rowlens[row] - column,
                                              RTERM_SCREEN_DRAW_ALL,
                                              FALSE);
#endif
            }
	}
	row++;
	while (row < lastrow) {
	    column = 0;
	    endcolumn = screen->textbuf.rowcols[row];
            if (screen->textbuf.renddata[row]) {
                textflags = screen->textbuf.renddata[row];
                while (column < endcolumn) {
                    *textflags++ |= RTERM_CHAR_SELECTED;
                    column++;
                }
                if (row >= screen->viewrow
                    && row < screen->viewrow + screen->rows) {
                    drawrow = row - screen->viewrow;
                    blank_drawn(screen,
                                drawrow, 0,
                                screen->drawbuf.rowcols[drawrow]);
#if (SUPPORT_RTERM_UTF8_CHARS)
                    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                        Rterm_draw_screen_string_wc(screen, drawrow, 0,
                                                    screen->textbuf.wdata[row],
                                                    screen->textbuf.renddata[row],
                                                    screen->textbuf.rowcols[row],
                                                    RTERM_SCREEN_DRAW_ALL,
                                                    FALSE);
                    } else {
                        Rterm_draw_screen_string_8bit(screen, drawrow, 0,
                                                      screen->textbuf.data[row],
                                                      screen->textbuf.renddata[row],
                                                      screen->textbuf.rowcols[row],
                                                      RTERM_SCREEN_DRAW_ALL,
                                                      FALSE);
                    }
#else
                    Rterm_draw_screen_string_8bit(screen, drawrow, 0,
                                              screen->textbuf.data[row],
                                                  screen->textbuf.renddata[row],
                                                  screen->textbuf.rowcols[row],
                                                  RTERM_SCREEN_DRAW_ALL,
                                                  FALSE);
#endif
                }
            }
            row++;
	}
	column = 0;
	endcolumn = lastcolumn + 1;
        if (screen->textbuf.renddata[row]) {
            textflags = screen->textbuf.renddata[row];
            while (column < endcolumn) {
                *textflags++ |= RTERM_CHAR_SELECTED;
                column++;
            }
            if (row >= screen->viewrow
                && row < screen->viewrow + screen->rows) {
                drawrow = row - screen->viewrow;
                blank_drawn(screen,
			drawrow, 0,
                            endcolumn);
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    Rterm_draw_screen_string_wc(screen, drawrow, 0,
                                                screen->textbuf.wdata[row],
                                                screen->textbuf.renddata[row],
                                                endcolumn,
                                                RTERM_SCREEN_DRAW_ALL,
                                                FALSE);
                } else {
                    Rterm_draw_screen_string_8bit(screen, drawrow, 0,
                                                  screen->textbuf.data[row],
                                                  screen->textbuf.renddata[row],
                                                  endcolumn,
                                                  RTERM_SCREEN_DRAW_ALL,
                                                  FALSE);
                }
#else
	    Rterm_draw_screen_string_8bit(screen, drawrow, 0,
                                          screen->textbuf.data[row],
                                          screen->textbuf.renddata[row],
                                          endcolumn,
                                          RTERM_SCREEN_DRAW_ALL,
                                          FALSE);
#endif
            }
        }
    }
    if (screen->buf) {
	Rterm_sync_screen(screen, 0, 0, 0, RTERM_SCREEN_SYNC);
    }
#if 0
    XFlush(screen->window->app->display);
#endif
    
    return;
}

void
Rterm_change_selection(struct R_term *term, int row, int column)
{
    struct R_termscreen *screen;

    if (term == NULL) {
#if 0
        || (term->selection.baserow == row
            && term->selection.basecolumn == column)) {
#endif
	
	return;
    }

    screen = term->selection.screen;
    if (screen) {
        Rterm_clear_screen_selection(screen);
        if (row < screen->savelines + screen->rows
            && column < screen->columns) {
            if (row < term->selection.baserow) {
                term->selection.firstrow = row;
                term->selection.firstcolumn = column;
                term->selection.lastrow = term->selection.baserow;
                term->selection.lastcolumn = term->selection.basecolumn;
            } else if (row == term->selection.baserow) {
                if (column < term->selection.firstcolumn) {
                    term->selection.firstrow = row;
                    term->selection.firstcolumn = column;
                    term->selection.lastrow = term->selection.baserow;
                    term->selection.lastcolumn = term->selection.basecolumn;
                } else {
                    term->selection.firstrow = term->selection.baserow;
                    term->selection.firstcolumn = term->selection.basecolumn;
                    term->selection.lastrow = row;
                    term->selection.lastcolumn = column;
                }
            } else if (row > term->selection.baserow) {
                term->selection.firstrow = term->selection.baserow;
                term->selection.firstcolumn = term->selection.basecolumn;
                term->selection.lastcolumn = column;
                term->selection.lastrow = row;
            }
#if 0
            fprintf(stderr,  "SELECTION: %d, %d -> %d, %d on %p\n",
                    term->selection.firstrow,
                    term->selection.firstcolumn,
                    term->selection.lastrow,
                    term->selection.lastcolumn,
                    term);
#endif
        }
//    screen_highlight_selection(screen);
//        Rterm_highlight_selection(term);
    }
    
    return;
}

void
Rterm_remove_selection(struct R_term *term, Time time)
{
    if (term == NULL) {

	return;
    }

    if (R_selection_owner(term->window) == term->window->id) {
	R_unset_selection(term->window);

        XFlush(term->window->app->display);
    }

    return;
}

void
Rterm_free_screen_selection(struct R_termscreen *screen)
{
    struct R_term *term;

    term = R_global.app->client;
    if (term->selection.screen == screen) {
        if ((term->selection.stringdata) && (term->selection.stringlen)) {
            free(term->selection.stringdata);
            term->selection.stringdata = NULL;
            term->selection.stringlen = 0;
        }
        if ((term->selection.utf8data) && (term->selection.utf8data)) {
            free(term->selection.utf8data);
            term->selection.utf8data = NULL;
            term->selection.utf8len = 0;
        }

        term->selection.screen = NULL;
        term->selection.firstrow = -1;
        term->selection.firstcolumn = -1;
        term->selection.lastrow = -1;
        term->selection.lastcolumn = -1;

        Rterm_remove_selection(term, CurrentTime);
    }

    return;
}

int
Rterm_prepare_selection(struct R_term *term, int format)
{
    struct R_termscreen *screen;
    int row, column;
    int lastrow, lastcolumn;
    uint8_t *data;
    uint8_t *dataptr;
    R_text_t *bufferptr;
    size_t datalen;
    long len;

    if (term->selection.firstrow < 0
	|| term->selection.firstcolumn < 0
	|| term->selection.lastrow < 0
	|| term->selection.lastcolumn < 0) {

	return -1;
    }

    screen = term->selection.screen;

    row = term->selection.firstrow;
    lastrow = term->selection.lastrow;
    datalen = (lastrow - row + 1) * (screen->columns + 1);
    data = calloc(1, datalen);
    if (data == NULL) {

	return -1;
    }
    len = 0;
    if (term->selection.stringdata) {
	free(term->selection.stringdata);
	term->selection.stringlen = 0;
	term->selection.stringdata = NULL;
    }
    if (term->selection.utf8data) {
	free(term->selection.utf8data);
	term->selection.utf8len = 0;
	term->selection.utf8data = NULL;
    }
    if (format == R_STRING_SELECTION) {
	row = term->selection.firstrow;
	column = term->selection.firstcolumn;
	lastrow = term->selection.lastrow;
	dataptr = data;
        bufferptr = screen->textbuf.data[row] + column;
        if (row == lastrow) {
            lastcolumn = term->selection.lastcolumn;
            while (column <= lastcolumn) {
                *dataptr++ = *bufferptr++;
                column++;
                len++;
            }
        } else {
            lastcolumn = screen->textbuf.rowcols[row];
            while (column < lastcolumn) {
                *dataptr++ = *bufferptr++;
                column++;
                len++;
            }
            *dataptr++ = '\n';
            len++;
            row++;
            while (row < lastrow) {
                column = 0;
                lastcolumn = screen->textbuf.rowcols[row];
                bufferptr = screen->textbuf.data[row];
                while (column < lastcolumn) {
                    *dataptr++ = *bufferptr++;
                    column++;
                    len++;
                }
                *dataptr++ = '\n';
                len++;
                row++;
            }
            column = 0;
            lastcolumn = term->selection.lastcolumn;
            bufferptr = screen->textbuf.data[row];
            while (column < lastcolumn) {
                *dataptr++ = *bufferptr++;
                column++;
                len++;
            }
            if (column == screen->textbuf.rowcols[row]) {
                *dataptr = '\n';
                len++;
            }
        }
        term->selection.stringlen = len;
        term->selection.stringdata = data;
    }

    return 0;
}

#if 0
void
screen_highlight_selection(struct R_termscreen *screen)
{
    int row;
    int column;
    int len;

    row = term->selection.firstrow;
    column = term->selection.firstcolumn;
    if (term->selection.firstrow == term->selection.lastrow) {
        len = MIN(term->selection.lastcolumn,
                  screen->textbuf.rowcols[row]) - column;
	if (row >= screen->viewrow
	    && row < screen->viewrow + screen->rows) {
            screen_draw_selection(screen, row, column, len);
        }
    } else {
        len = screen->textbuf.rowcols[row] - column;
        if (row >= screen->viewrow
            && row < screen->viewrow + screen->rows) {
            screen_draw_selection(screen, row, column, len);
        }
        while (row < term->selection.lastrow) {
            if (row >= screen->viewrow
                && row < screen->viewrow + screen->rows) {
                len = screen->textbuf.rowcols[row];
                screen_draw_selection(screen, row, column, len);
            }
            row++;
        }
        len = term->selection.lastcolumn;
        if (row >= screen->viewrow
	    && row < screen->viewrow + screen->rows) {
            len = screen->textbuf.rowcols[row];
            screen_draw_selection(screen, row, column, len);
        }
    }

    return;
}
#endif

