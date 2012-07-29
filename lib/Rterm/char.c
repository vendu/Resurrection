/*
 * char.c - single-byte character routines for Rterm.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#define FIXES  1
#define KLUDGE 0

extern volatile int Rtermtabs;

#if (SUPPORT_RTERM_UTF8_CHARS)
Rterm_wchar_t
Rterm_decode_char_utf8(unsigned char *str, int *len)
{
    unsigned char *cp = str;
    int nb = 0;
    Rterm_wchar_t wch = *cp;
    Rterm_wchar_t tmp;
    
    if (wch > 0x7f) {
        nb = 1;
        while ((*++cp & 0xc0) == 0xc0) {
            nb++;
        }
        cp = str;
        wch = 0;
        if (nb == 2) {
            tmp = *cp++;
            wch = (tmp & 0x1f) << 6;
            tmp = *cp++;
            wch |= tmp & 0x3f;
        } else if (nb == 3) {
            /* nb == 3 */
            tmp = *cp++;
            wch = (tmp & 0x0f) << 12;
            tmp = *cp++;
            wch |= (tmp & 0x3f) << 6;
            tmp = *cp++;
            wch |= (tmp & 0x3f);
        }
    } else if (wch) {
        nb = 1;
    }
    *len = nb;

    return wch;
}

Rterm_wchar_t
Rterm_read_char_utf8(struct R_term *term)
{
    Rterm_wchar_t ch = Rterm_read_char(term);
    Rterm_wchar_t wch = ch;

    if (ch > 0x7f) {
        switch (ch & 0xf0) {
            case 0xe0:
                wch = (ch & 0x0f) << 12;
                if ((ch = Rterm_read_char(term)) > 0 && (ch & 0xc0) == 0xc0) {
                    wch |= (ch & 0x3f) << 6;
                    if ((ch = Rterm_read_char(term)) > 0 && (ch & 0xc0) == 0xc0) {
                        wch |= (ch & 0x3f);
                    } else {
                        wch = 0;
                    }
                } else {
                    wch = 0;
                }

                break;
            case 0xc0:
                wch = (ch & 0xf1) << 6;
                if ((ch = Rterm_read_char(term)) > 0 && (ch & 0xc0) == 0xc0) {
                    wch |= ch & 0x3f;
                } else {
                    wch = 0;
                }

                break;
            default:
                wch = 0;

                break;
        }
    }

    return wch;
}
#endif

int
Rterm_alloc_screen_textbuffers(struct R_termscreen *screen)
{
    struct R_term *term = R_global.app->client;
    int row, rows, bufferrows;
//    void (*destroy_buffers)(struct R_termscreen *);
    void (*blank_string)(struct R_termscreen *, int, int, int);
    void (*blank_drawn)(struct R_termscreen *, int, int, int);
    
//    destroy_buffers = screen->funcs.destroy_buffers;
    blank_string = screen->funcs.blank_string;
    blank_drawn = screen->funcs.blank_drawn;
    
    screen->rows = MIN(screen->rows, RTERM_SCREEN_MAX_ROWS);
    screen->columns = MIN(screen->columns, RTERM_SCREEN_MAX_COLUMNS);
    bufferrows = screen->savelines + screen->rows;
    screen->textbuf.nrows = bufferrows;
    rows = screen->rows;
    screen->drawbuf.nrows = rows;
    
    if ((screen->textbuf.rowlens =
	 calloc(bufferrows, sizeof(unsigned int)))
	== NULL) {
//	destroy_buffers(screen);
        
	return -1;
    }
    
    if ((screen->textbuf.rowcols =
	 calloc(bufferrows, sizeof(unsigned int)))
	== NULL) {
//	destroy_buffers(screen);
        
	return -1;
    }
    
    if ((screen->drawbuf.rowlens =
         calloc(screen->drawbuf.nrows, sizeof(unsigned int)))
        == NULL) {
//	destroy_buffers(screen);
        
        return -1;
    }
        
    if ((screen->drawbuf.rowcols =
         calloc(screen->drawbuf.nrows, sizeof(unsigned int)))
            == NULL) {
//	destroy_buffers(screen);
        
        return -1;
    }
    
#if (SUPPORT_RTERM_UTF8_CHARS)
    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
        if ((screen->textbuf.wdata
             = calloc(bufferrows, sizeof(Rterm_wchar_t *)))
            == NULL) {
            
            return -1;
        }
    } else if ((screen->textbuf.data =
                calloc(bufferrows, sizeof(R_text_t *)))
               == NULL) {
//	destroy_buffers(screen);
        
	return -1;
    }
#else    
    if ((screen->textbuf.data =
	 calloc(bufferrows, sizeof(R_text_t *)))
	== NULL) {
//	destroy_buffers(screen);
        
	return -1;
    }
#endif

#if (SUPPORT_RTERM_UTF8_CHARS)
    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
        if ((screen->drawbuf.wdata =
             calloc(screen->drawbuf.nrows, sizeof(Rterm_wchar_t *)))
            == NULL) {
//	destroy_buffers(screen);
            
            return -1;
        }
    } else if ((screen->drawbuf.data =
                calloc(screen->drawbuf.nrows, sizeof(R_text_t *)))
               == NULL) {
//	destroy_buffers(screen);
        
        return -1;
    }
#else
    if ((screen->drawbuf.data =
         calloc(screen->drawbuf.nrows, sizeof(R_text_t *)))
        == NULL) {
//	destroy_buffers(screen);
        
        return -1;
    }
#endif
    
    if ((screen->drawbuf.renddata =
         calloc(screen->drawbuf.nrows, sizeof(R_textflags_t *)))
        == NULL) {
//	destroy_buffers(screen);
        
        return -1;
    }
    
    if ((screen->drawbuf.rowdirty =
         calloc(screen->drawbuf.nrows, sizeof(int)))
        == NULL) {
//        destroy_buffers(screen);
        
        return -1;
    }
    
    
    if ((screen->textbuf.renddata =
	 calloc(bufferrows, sizeof(R_textflags_t *)))
	== NULL) {
//	destroy_buffers(screen);
	
	return -1;
    }
    
    for (row = screen->savelines ;
         row < bufferrows ;
         row++) {
#if (SUPPORT_RTERM_UTF8_CHARS)
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
            if ((screen->textbuf.wdata[row]
                 = calloc(screen->columns, sizeof(Rterm_wchar_t)))
                == NULL) {
                
                return -1;
            }
        } else if ((screen->textbuf.data[row] =
                    calloc(screen->columns, sizeof(R_text_t)))
                   == NULL) {
//	    destroy_buffers(screen);
            
	    return -1;
        }
#else
	if ((screen->textbuf.data[row] =
             calloc(screen->columns, sizeof(R_text_t)))
	    == NULL) {
//	    destroy_buffers(screen);
            
            return -1;
        }
#endif
        
        if ((screen->textbuf.renddata[row] =
             calloc(screen->columns, sizeof(R_textflags_t)))
            == NULL) {
//	    destroy_buffers(screen);
            
            return -1;
        }
        
        screen->textbuf.rowlens[row] = screen->columns;
        screen->textbuf.rowcols[row] = 0;
        blank_string(screen, row, 0, screen->textbuf.rowlens[row]);
    }
        
    for (row = 0 ;
         row < screen->drawbuf.nrows ;
         row++) {
#if (SUPPORT_RTERM_UTF8_CHARS)
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
            if ((screen->drawbuf.wdata[row]
                 = calloc(screen->columns, sizeof(Rterm_wchar_t)))
                == NULL) {
                
                return -1;
            }
        } else if ((screen->drawbuf.data[row] =
                    calloc(screen->columns, sizeof(R_text_t)))
                   == NULL) {
//	    destroy_buffers(screen);
            
            return -1;
        }
#else
        if ((screen->drawbuf.data[row] =
             calloc(screen->columns, sizeof(R_text_t)))
            == NULL) {
//	    destroy_buffers(screen);
            
            return -1;
        }
#endif
        
        if ((screen->drawbuf.renddata[row] =
             calloc(screen->columns, sizeof(R_textflags_t)))
            == NULL) {
//	    destroy_buffers(screen);
            
            return -1;
        }
        
        screen->drawbuf.rowlens[row] = screen->columns;
        screen->drawbuf.rowdirty[row] = 1;
        
        blank_drawn(screen, row, 0,
                    screen->drawbuf.rowlens[row]);
        screen->drawbuf.rowcols[row] = 0;
    }

    return 0;
}

/*
 * NOTE: everything below really belongs into Rterm/screen.c and it should only
 * be called through function pointers from the screen structure...
 */

int
Rterm_resize_screen_textbuffers(struct R_termscreen *screen,
                                int rows,
                                int columns)
{
#if (SUPPORT_RTERM_UTF8_CHARS)
    struct R_term *term = R_global.app->client;
#endif
    int row, oldrows, newrows, oldcolumns;
    int drawrow;
    int (*scroll_text)(struct R_termscreen *, int, int, int, int);
    void (*blank_string)(struct R_termscreen *, int, int, int);
    void (*blank_drawn)(struct R_termscreen *, int, int, int);

    if (screen == NULL
	|| rows <= 0
	|| columns <= 0
	|| (rows == screen->rows
	    && columns == screen->columns)) {

	return -1;
    }
    scroll_text = screen->funcs.scroll_text;
    blank_string = screen->funcs.blank_string;
    blank_drawn = screen->funcs.blank_drawn;

    oldrows = screen->rows;
    oldcolumns = screen->columns;

    screen->rows = rows;
    screen->columns = columns;
    screen->textbuf.nrows = screen->savelines + rows;
    screen->drawbuf.nrows = rows;

#if 0
    rows = MIN(rows, SCREEN_MAX_ROWS);
    columns = MIN(columns, SCREEN_MAX_COLUMNS);
#endif

    if (rows != oldrows) {
	if (rows < oldrows) {
	    if (screen->row > rows - 1) {
		scroll_text(screen, 0, screen->rows - 1,
			    screen->row - rows + 1,
			    TRUE);

		screen->row = rows - 1;
	    }

	    for (row = screen->savelines + rows ;
		 row < screen->savelines + oldrows ;
		 row++) {
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    if (screen->textbuf.wdata[row]) {
                        drawrow = row - screen->savelines;
                        free(screen->textbuf.wdata[row]);
                        screen->textbuf.wdata[row] = NULL;
                        free(screen->textbuf.renddata[row]);
                        screen->textbuf.renddata[row] = NULL;
                        free(screen->drawbuf.wdata[drawrow]);
                        screen->drawbuf.wdata[drawrow] = NULL;
                        free(screen->drawbuf.renddata[drawrow]);
                        screen->drawbuf.renddata[drawrow] = NULL;
                    }
                } else if (screen->textbuf.data[row]) {
		    drawrow = row - screen->savelines;
		    free(screen->textbuf.data[row]);
                    screen->textbuf.data[row] = NULL;
		    free(screen->textbuf.renddata[row]);
                    screen->textbuf.renddata[row] = NULL;
		    free(screen->drawbuf.data[drawrow]);
                    screen->drawbuf.data[drawrow] = NULL;
		    free(screen->drawbuf.renddata[drawrow]);
                    screen->drawbuf.renddata[drawrow] = NULL;
		}
#else
		if (screen->textbuf.data[row]) {
		    drawrow = row - screen->savelines;
		    free(screen->textbuf.data[row]);
                    screen->textbuf.data[row] = NULL;
		    free(screen->textbuf.renddata[row]);
                    screen->textbuf.renddata[row] = NULL;
		    free(screen->drawbuf.data[drawrow]);
                    screen->drawbuf.data[drawrow] = NULL;
		    free(screen->drawbuf.renddata[drawrow]);
                    screen->drawbuf.renddata[drawrow] = NULL;
		}
#endif
	    }
	}

	newrows = screen->savelines + rows;

	if ((screen->textbuf.rowlens
	     = realloc(screen->textbuf.rowlens,
		       newrows * sizeof(unsigned int)))
	    == NULL) {
	    
	    return -1;
	}

	if ((screen->textbuf.rowcols
	     = realloc(screen->textbuf.rowcols,
		       newrows * sizeof(unsigned int)))
	    == NULL) {
	    
	    return -1;
	}

#if (SUPPORT_RTERM_UTF8_CHARS)
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
            if ((screen->textbuf.wdata
                 = realloc(screen->textbuf.wdata,
                           newrows * sizeof(Rterm_wchar_t *)))
                == NULL) {
                
                return -1;
            }
        } else if ((screen->textbuf.data
	     = realloc(screen->textbuf.data,
		       newrows * sizeof(R_text_t *)))
	    == NULL) {
	    
	    return -1;
	}
#else	
	if ((screen->textbuf.data
	     = realloc(screen->textbuf.data,
		       newrows * sizeof(R_text_t *)))
	    == NULL) {
	    
	    return -1;
	}
#endif

	if ((screen->textbuf.renddata
	     = realloc(screen->textbuf.renddata,
		       newrows * sizeof(R_textflags_t *)))
	    == NULL) {
	    
	    return -1;
	}
	
	if ((screen->drawbuf.rowlens
	     = realloc(screen->drawbuf.rowlens,
		       rows * sizeof(unsigned int)))
	    == NULL) {
	    
	    return -1;
	}

	if ((screen->drawbuf.rowcols
	     = realloc(screen->drawbuf.rowcols,
		       rows * sizeof(unsigned int)))
	    == NULL) {
	    
	    return -1;
	}

#if (SUPPORT_RTERM_UTF8_CHARS)
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
            if ((screen->drawbuf.wdata
                 = realloc(screen->drawbuf.wdata,
                           rows * sizeof(Rterm_wchar_t *)))
                == NULL) {
                
                return -1;
            }
        } else if ((screen->drawbuf.data
	     = realloc(screen->drawbuf.data,
		       rows * sizeof(R_text_t *)))
	    == NULL) {
	    
	    return -1;
	}
#else	
	if ((screen->drawbuf.data
	     = realloc(screen->drawbuf.data,
		       rows * sizeof(R_text_t *)))
	    == NULL) {
	    
	    return -1;
	}
#endif

	if ((screen->drawbuf.renddata
	     = realloc(screen->drawbuf.renddata,
		       rows * sizeof(R_textflags_t *)))
	    == NULL) {
	    
	    return -1;
	}

	if ((screen->drawbuf.rowdirty
	     = realloc(screen->drawbuf.rowdirty,
		       rows * sizeof(int)))
	    == NULL) {
	    
	    return -1;
	}

	if (oldcolumns != columns) {
	    for (row = screen->savelines ;
		 row < screen->savelines + MIN(rows, oldrows) ;
		 row ++) {
		drawrow = row - screen->savelines;

#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    if ((screen->textbuf.wdata[row]
                         = realloc(screen->textbuf.wdata[row],
                                   columns * sizeof(Rterm_wchar_t)))
                        == NULL) {

                        return -1;
                    }
                } else if ((screen->textbuf.data[row] =
                            realloc(screen->textbuf.data[row],
                                    columns * sizeof(R_text_t)))
                           == NULL) {
                    
                    return -1;
                }
#else
		if ((screen->textbuf.data[row] =
                     realloc(screen->textbuf.data[row],
                             columns * sizeof(R_text_t)))
		    == NULL) {
		    
		    return -1;
		}
#endif
		
		if ((screen->textbuf.renddata[row]
		     = realloc(screen->textbuf.renddata[row],
			       columns * sizeof(R_textflags_t)))
		    == NULL) {
		    
		    return -1;
		}

#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    if ((screen->drawbuf.wdata[drawrow]
                         = realloc(screen->drawbuf.wdata[drawrow],
                                   columns * sizeof(Rterm_wchar_t)))
                        == NULL) {

                        return -1;
                    }
		} else if ((screen->drawbuf.data[drawrow] =
                     realloc(screen->drawbuf.data[drawrow],
                             columns * sizeof(R_text_t)))
		    == NULL) {

		    return -1;
		}
#else
		if ((screen->drawbuf.data[drawrow] =
                     realloc(screen->drawbuf.data[drawrow],
                             columns * sizeof(R_text_t)))
		    == NULL) {

		    return -1;
		}
#endif
		
		if ((screen->drawbuf.renddata[drawrow]
		     = realloc(screen->drawbuf.renddata[drawrow],
			       columns * sizeof(R_textflags_t)))
		    == NULL) {

		    return -1;
		}
		
		screen->textbuf.rowlens[row] = columns;
		screen->textbuf.rowcols[row]
		    = MIN(columns, screen->textbuf.rowcols[row]);
		screen->drawbuf.rowlens[drawrow] = columns;
		screen->drawbuf.rowcols[drawrow]
		    = MIN(columns, screen->drawbuf.rowcols[drawrow]);

		if (columns > oldcolumns) {
		    blank_string(screen, row, oldcolumns,
				 columns - oldcolumns);
		    blank_drawn(screen, drawrow, oldcolumns,
				columns - oldcolumns);
		}
	    }
	}
	    
	for (row = screen->savelines + oldrows ;
	     row < screen->savelines + rows ;
	     row++) {
	    drawrow = row - screen->savelines;

#if (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                if ((screen->textbuf.wdata[row]
                     = calloc(1, columns * sizeof(Rterm_wchar_t)))
                    == NULL) {
                    
                    return -1;
                }
                
                if ((screen->drawbuf.wdata[drawrow]
                     = calloc(1, columns * sizeof(Rterm_wchar_t)))
                    == NULL) {
                    
                    return -1;
                }
            } else {
                if ((screen->textbuf.data[row]
                     = calloc(1, columns * sizeof(R_text_t)))
                    == NULL) {
                    
                    return -1;
                }
                
                if ((screen->drawbuf.data[drawrow]
                     = calloc(1, columns * sizeof(R_text_t)))
                    == NULL) {
                    
                    return -1;
                }
            }
#else
	    if ((screen->textbuf.data[row]
		 = calloc(1, columns * sizeof(R_text_t)))
		== NULL) {
		
		return -1;
	    }
	    
	    if ((screen->drawbuf.data[drawrow]
		 = calloc(1, columns * sizeof(R_text_t)))
		== NULL) {
		
		return -1;
	    }
#endif
	    
	    if ((screen->textbuf.renddata[row]
		 = calloc(1, columns * sizeof(R_textflags_t)))
		== NULL) {
		
		return -1;
	    }

	    if ((screen->drawbuf.renddata[drawrow]
		 = calloc(1, columns * sizeof(R_textflags_t)))
		== NULL) {
		
		return -1;
	    }

	    screen->textbuf.rowlens[row] = columns;
	    screen->textbuf.rowcols[row] = 0;
	    screen->drawbuf.rowlens[drawrow] = columns;
	    screen->drawbuf.rowcols[drawrow] = 0;
            screen->drawbuf.rowdirty[drawrow] = 1;
	    blank_string(screen, row, 0, columns);
	    blank_drawn(screen, drawrow, 0, columns);
	}
    } else {
	/* rows == oldrows && columns != oldcolumns */

	for (row = screen->savelines ;
	     row < screen->savelines + rows ;
	     row++) {
#if (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                if ((screen->textbuf.wdata[row]
                     = realloc(screen->textbuf.wdata[row],
                               columns * sizeof(Rterm_wchar_t)))
                    == NULL) {
                    
                    return -1;
                }
            } else if ((screen->textbuf.data[row]
                        = realloc(screen->textbuf.data[row],
                                  columns * sizeof(R_text_t)))
                       == NULL) {

		return -1;
	    }
            
#else
	    if ((screen->textbuf.data[row]
		 = realloc(screen->textbuf.data[row],
			   columns * sizeof(R_text_t)))
		== NULL) {
                
		return -1;
	    }
#endif
            
	    if ((screen->textbuf.renddata[row]
		 = realloc(screen->textbuf.renddata[row],
			   columns * sizeof(R_textflags_t)))
		== NULL) {

		return -1;
	    }

	    screen->textbuf.rowcols[row]
		= MIN(columns, screen->textbuf.rowcols[row]);
	    screen->textbuf.rowlens[row] = columns;
	}

	for (row = 0 ; row < rows ; row++) {
#if (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                if ((screen->drawbuf.wdata[row]
                     = realloc(screen->drawbuf.wdata[row],
                               columns * sizeof(Rterm_wchar_t)))
                    == NULL) {
                    
                    return -1;
                }
            } else if ((screen->drawbuf.data[row]
		 = realloc(screen->drawbuf.data[row],
			   columns * sizeof(R_text_t)))
		== NULL) {

		return -1;
	    }

#else
	    if ((screen->drawbuf.data[row]
		 = realloc(screen->drawbuf.data[row],
			   columns * sizeof(R_text_t)))
		== NULL) {

		return -1;
	    }
#endif

	    if ((screen->drawbuf.renddata[row]
		 = realloc(screen->drawbuf.renddata[row],
			   columns * sizeof(R_textflags_t)))
		== NULL) {

		return -1;
	    }

	    screen->drawbuf.rowlens[row] = columns;
	    screen->drawbuf.rowcols[row]
		= MIN(columns, screen->drawbuf.rowcols[row]);

	    if (columns > oldcolumns) {
		blank_string(screen, row + screen->savelines,
			     oldcolumns, columns - oldcolumns);
		blank_drawn(screen, row,
			    oldcolumns, columns - oldcolumns);
	    }
	}
    }
    if (screen->buf) {
        Rterm_clear_screen_buffer(screen, 0, 0, 0, RTERM_SCREEN_CLEAR_BUFFER);
    }

    return 0;
}

/* NOTE: this may make screen->row < 0, but it doesn't matter as long as
 * you don't use screen->row for indexing arrays. This is a nice speedup. :)
 */
void
Rterm_add_screen_text(struct R_termscreen *screen,
                      const R_text_t *str,
                      int len,
                      int rows)
{
    struct R_term *term;
    int i, row, column;
    int drawrow;
    R_text_t **bufferrows = NULL;
    R_textflags_t **bufferflags;
    unsigned int *bufferlens;
    unsigned int *buffercolumns;
    R_text_t **drawnrows = NULL;
    R_textflags_t **drawnflags;
    unsigned int *drawnlens;
    unsigned int *drawncolumns;
    int (*scroll_text)(struct R_termscreen *, int, int, int, int);
    void (*insert_blank)(struct R_termscreen *, int);
    void (*blank_string)(struct R_termscreen *, int, int, int);
    void (*blank_drawn)(struct R_termscreen *, int, int, int);
#if (SUPPORT_RTERM_UTF8_CHARS)
    Rterm_wchar_t **wbufferrows = NULL;
    Rterm_wchar_t **wdrawnrows = NULL;
    Rterm_wchar_t wch = 0;
    int clen = 1;
    int wlen;
#else
    int clen = 1;
    int wlen = 1;
#endif
    R_text_t ch = 0;
    int cval;

    if (screen == NULL
        || screen->pty.masterfd == -1
	|| str == NULL
	|| len <= 0
	|| rows < 0) {

        return;
    }

    term = R_global.app->client;
#if 0
    if (!term->blink) {
        Rterm_clear_screen_cursor(screen);
    }
#endif

#if (SUPPORT_RTERM_UTF8_CHARS)
    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
        wbufferrows = screen->textbuf.wdata;
        wdrawnrows = screen->drawbuf.wdata;
    } else {
        bufferrows = screen->textbuf.data;
        drawnrows = screen->drawbuf.data;
    }
#else
    bufferrows = screen->textbuf.data;
    drawnrows = screen->drawbuf.data;
#endif
    bufferflags = screen->textbuf.renddata;
    bufferlens = screen->textbuf.rowlens;
    buffercolumns = screen->textbuf.rowcols;
#if 0
    drawnflags = screen->drawbuf.renddata;
    drawnlens = screen->drawbuf.rowlens;
    drawncolumns = screen->drawbuf.rowcols;
#endif

    scroll_text = screen->funcs.scroll_text;
    insert_blank = screen->funcs.insert_blank;
    blank_string = screen->funcs.blank_string;
    blank_drawn = screen->funcs.blank_drawn;

#if 0
    if (termptr->privmodes & RTERM_PRIVMODE_HOME_ON_ECHO) {
	termptr->window->screen->viewrow =
	    termptr->window->screen->savelines;
    }
#endif

#if 0
    if (!term->blink) {
        Rterm_clear_screen_cursor(screen);
    }
#endif

    rows -= screen->scrollbottom - screen->row;

    if ((rows > 0)
	&& (screen->savelines + screen->row - rows
	    >= screen->savelines - screen->nsavelines)) {
	scroll_text(screen,
		    screen->scrolltop, screen->scrollbottom,
		    rows,
		    TRUE);
	screen->row -= rows;
    }

    row = screen->savelines + screen->row;
#if 0
    firstrow = screen->row;
    firstcol = screen->column;
#endif

#if 0 && (SUPPORT_RTERM_UTF8_CHARS)
    if (!(term->privmodes & RTERM_PRIVMODE_UTF8_CHARS)) {
        clen = 1;
    }
#endif
    for (i = 0 ; i < len ; i += clen) {
#if (SUPPORT_RTERM_UTF8_CHARS)
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
            wch = Rterm_decode_char_utf8(str + i, &clen);
            if (!wch) {

                return;
            }
            cval = wch;
        } else {
            ch = str[i];
            cval = ch;
        }
#else        
	cval = ch = str[i];
#endif
	switch (cval) {
    	    case R_DEL_CHAR:

		continue;
    	    case '\t':
		Rterm_screen_tab(screen, 1);

		continue;
	    case '\n':
                screen->refreshcnt++;
                buffercolumns[row] = MAX(screen->column, buffercolumns[row]);
                screen->flags &= ~RTERM_SCREEN_WRAP_NEXT_CHAR;
		if (screen->row == screen->scrollbottom) {
		    scroll_text(screen, screen->scrolltop,
				screen->scrollbottom,
				1,
				TRUE);
                    blank_string(screen, screen->row, 0,
                                 screen->textbuf.rowlens[row]);
		} else if (screen->row < screen->rows - 1) {
		    screen->row++;
		    row++;
		}
                if (screen->row > 0) {
                    blank_drawn(screen, screen->row, 0, screen->textbuf.rowlens[row]);
                    screen->drawbuf.rowdirty[screen->row] = 1;
                }
		screen->column = 0;
//                screen->refreshed = FALSE;
                screen->refreshcnt = -1;

                continue;
	    case '\r':
                buffercolumns[row] = MAX(screen->column, buffercolumns[row]);
                screen->flags &= ~RTERM_SCREEN_WRAP_NEXT_CHAR;
#if 0
                if (screen->row > 0) {
                    blank_string(screen, row, 0, screen->textbuf.rowlens[row]);
                    blank_drawn(screen, screen->row, 0, screen->drawbuf.rowlens[screen->row]);
                    screen->drawbuf.rowdirty[screen->row] = 1;
                }
#endif
                screen->column = 0;
#if 0
                if (row > screen->savelines
                    && row < screen->savelines + screen->rows) {
                    screen->drawbuf.rowdirty[screen->row] = 1;
                }
#endif
                
                continue;
	    default:
#if 0
                if (screen->cmdbuf.firstcol < 0) {
                    screen->cmdbuf.firstcol = column;
                    screen->cmdbuf.firstrow = row;
                    screen->cmdbuf.lastcol = column;
                    screen->cmdbuf.lastrow = row;
                }
#endif

		break;
	}
	if (screen->flags & RTERM_SCREEN_WRAP_NEXT_CHAR) {
            buffercolumns[row] = MAX(screen->columns, buffercolumns[row]);
	    screen->flags &= ~RTERM_SCREEN_WRAP_NEXT_CHAR;
	    if (screen->row == screen->scrollbottom) {
		scroll_text(screen, screen->scrolltop,
			    screen->scrollbottom,
			    1,
			    TRUE);
	    } else if (screen->row < screen->rows - 1) {
		screen->row++;
		row++;
	    }
            blank_string(screen, row, 0, screen->textbuf.rowlens[row]);
	    screen->column = 0;
            if (row > screen->savelines
                && row < screen->savelines + screen->rows) {
                screen->drawbuf.rowdirty[row - screen->savelines] = 1;
            }
            buffercolumns[row] = 0;
        }
	if (term->privmodes & RTERM_PRIVMODE_INSERT
            && !(screen->flags & RTERM_SCREEN_WRAP_NEXT_CHAR)) {
	    insert_blank(screen, 1);
	}
        column = screen->column;
//        column = screen->column;
#if (SUPPORT_RTERM_UTF8_CHARS)
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
            wbufferrows[row][column] = wch;
        } else {
            bufferrows[row][column] = ch;
        }
#else
        bufferrows[row][column] = ch;
#endif
        bufferflags[row][column] = screen->textflags;
        buffercolumns[row] = MAX(column, buffercolumns[row]);
#if (SUPPORT_RTERM_UTF8_CHARS)
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
            wlen = wcwidth(wch);
        } else {
            wlen = 1;
        }
#endif
        if (column < screen->columns - wlen) {
            screen->column += wlen;
        } else {
            buffercolumns[row] = column;
//            buffercolumns[row] = MAX(column, buffercolumns[row]);
            if (term->privmodes & RTERM_PRIVMODE_AUTOWRAP) {
                screen->flags |= RTERM_SCREEN_WRAP_NEXT_CHAR;
            } else {
                screen->flags &= ~RTERM_SCREEN_WRAP_NEXT_CHAR;
            }
        }
//            column = screen->column;
        buffercolumns[row] = MAX(column, buffercolumns[row]);
    }
    column = screen->column;
    buffercolumns[row] = MAX(column, buffercolumns[row]);
    if (row > screen->savelines
        && row < screen->savelines + screen->rows) {
        screen->drawbuf.rowdirty[row - screen->savelines] = 1;
    }

    if (rows > 0 && (term->privmodes & RTERM_PRIVMODE_SCROLLBAR)) {
	term->flags &= ~RTERM_SCROLLBAR_REFRESHED;
    }
    if (screen != term->screens[term->curscreen]) {
        screen->refreshcnt = -1;
    }

    return;
}

int
Rterm_scroll_screen_text(struct R_termscreen *screen,
                         int toprow, int bottomrow,
                         int count,
                         int saverows)
{
    struct R_term *term;
    int row1, row2;
    int scrollrows;
    void (*blank_string)(struct R_termscreen *, int, int, int);
    void (*blank_drawn)(struct R_termscreen *, int, int, int);

    if (screen == NULL
	|| toprow < 0
	|| toprow > bottomrow
	|| toprow > screen->rows - 1
	|| bottomrow < 0
	|| bottomrow > screen->rows - 1
	|| count == 0) {

	return -1;
    }
    term = R_global.app->client;
    blank_string = screen->funcs.blank_string;
    blank_drawn = screen->funcs.blank_drawn;

    if (screen->type == RTERM_SECONDARY_SCREEN) {
	saverows = FALSE;
    }

//    Rterm_clear_screen_selection(screen);

    for (row1 = toprow ; row1 <= bottomrow ; row1++) {
	blank_drawn(screen, row1, 0, screen->drawbuf.rowlens[row1]);
        screen->drawbuf.rowdirty[row1] = 1;
    }

#if 0
    if (toprow == 0
	&& bottomrow == screen->rows - 1
	&& count > 0) {
	if (term->selection.firstrow >= 0
	    && term->selection.firstcolumn >= 0
	    && term->selection.lastrow >= 0
	    && term->selection.lastcolumn >= 0) {
	    term->selection.firstrow -= count;
	    term->selection.lastrow -= count;
            Rterm_change_selection(term);
	}
    }
#endif

    toprow += screen->savelines;
    bottomrow += screen->savelines;

#if 0
    if (count < 0) {
	count = MAX(count, -(bottomrow - toprow + 1));
    }
#endif

    if (count > 0) {
        if (term->selection.lastrow - count >= 0) {
            term->selection.firstrow -= count;
            term->selection.lastrow -= count;
            Rterm_clear_screen_selection(screen);
        }
	/* scroll up */
	if (saverows) {
	    /* move rows up into scrollback buffer. */
            
            for (row1 = 0 ; row1 < count ; row1++) {
		/* free rows in scrollback buffer. */
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    if (screen->textbuf.wdata[row1]) {
                        free(screen->textbuf.wdata[row1]);
                        screen->textbuf.wdata[row1] = NULL;
                    }
                } else if (screen->textbuf.data[row1]) {
		    free(screen->textbuf.data[row1]);
                    screen->textbuf.data[row1] = NULL;
		}
#else
		if (screen->textbuf.data[row1]) {
		    free(screen->textbuf.data[row1]);
                    screen->textbuf.data[row1] = NULL;
		}
#endif
		if (screen->textbuf.renddata[row1]) {
		    free(screen->textbuf.renddata[row1]);
                    screen->textbuf.renddata[row1] = NULL;
		}
//                screen->textbuf.rowcols[row1] = 0;
	    }

            /* move row pointers */
            
	    scrollrows = bottomrow - count + 1;
            
	    memmove(screen->textbuf.rowlens,
		    screen->textbuf.rowlens + count,
		    scrollrows * sizeof(unsigned int));
            
	    memmove(screen->textbuf.rowcols,
		    screen->textbuf.rowcols + count,
		    scrollrows * sizeof(unsigned int));
            
#if (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                memmove(screen->textbuf.wdata,
                        screen->textbuf.wdata + count,
                        scrollrows * sizeof(Rterm_wchar_t *));
            } else {
                memmove(screen->textbuf.data,
                        screen->textbuf.data + count,
                        scrollrows * sizeof(R_text_t *));
            }
#else
	    memmove(screen->textbuf.data,
		    screen->textbuf.data + count,
		    scrollrows * sizeof(R_text_t *));
#endif
            
	    memmove(screen->textbuf.renddata,
		    screen->textbuf.renddata + count,
		    scrollrows * sizeof(R_textflags_t *));
	} else {
	    /* !saverows */
            
	    for (row1 = toprow ; row1 < toprow + count ; row1++) {
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    if (screen->textbuf.wdata[row1]) {
                        free(screen->textbuf.wdata[row1]);
                        screen->textbuf.wdata[row1] = NULL;
                    }
                } else if (screen->textbuf.data[row1]) {
		    free(screen->textbuf.data[row1]);
		    screen->textbuf.data[row1] = NULL;
		}
#else
		if (screen->textbuf.data[row1]) {
		    free(screen->textbuf.data[row1]);
		    screen->textbuf.data[row1] = NULL;
		}
#endif
                
		if (screen->textbuf.renddata[row1]) {
		    free(screen->textbuf.renddata[row1]);
		    screen->textbuf.renddata[row1] = NULL;
		}
//                screen->textbuf.rowcols[row1] = 0;
	    }

            /* copy row pointers */
	    for (row1 = toprow, row2 = toprow + count ; row2 <= bottomrow ;
		 row1++, row2++) {
		/* copy rows */

		screen->textbuf.rowlens[row1] =
		    screen->textbuf.rowlens[row2];

		screen->textbuf.rowcols[row1] =
		    screen->textbuf.rowcols[row2];
                
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    screen->textbuf.wdata[row1] = screen->textbuf.wdata[row2];
                } else {
                    screen->textbuf.data[row1] = screen->textbuf.data[row2];
                }
#else
		screen->textbuf.data[row1] = screen->textbuf.data[row2];
#endif
                
		screen->textbuf.renddata[row1] =
		    screen->textbuf.renddata[row2];
	    }
	}

	for (row1 = bottomrow - count + 1 ; row1 <= bottomrow ; row1++) {
#if (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                if ((screen->textbuf.wdata[row1] = malloc(screen->columns
                                                          * sizeof(Rterm_wchar_t)))
                    == NULL) {
                    
                    return -1;
                }
            } else if ((screen->textbuf.data[row1] = malloc(screen->columns
                                                            * sizeof(R_text_t)))
                       == NULL) {
		
		return -1;
	    }
#else
	    if ((screen->textbuf.data[row1] = malloc(screen->columns
                                                     * sizeof(R_text_t)))
		== NULL) {
                
		return -1;
	    }
#endif
            
	    if ((screen->textbuf.renddata[row1] = calloc(screen->columns,
							 sizeof(R_textflags_t)))
		== NULL) {
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    free(screen->textbuf.wdata[row1]);
                    screen->textbuf.wdata[row1] = NULL;
                } else {
                    free(screen->textbuf.data[row1]);
                    screen->textbuf.data[row1] = NULL;
                }
                
#else
		free(screen->textbuf.data[row1]);
		screen->textbuf.data[row1] = NULL;
#endif
                
		return -1;
	    }
	    
	    screen->textbuf.rowlens[row1] = screen->columns;
	    blank_string(screen, row1, 0, screen->textbuf.rowlens[row1]);
	    screen->textbuf.rowcols[row1] = 0;
	}
    } else {
	/* count < 0, scroll down */
        
        if (term->selection.lastrow >= 0) {
            Rterm_clear_screen_selection(screen);
            if (term->selection.lastrow + count
                < screen->savelines + screen->rows) {
                term->selection.firstrow += count;
                term->selection.lastrow += count;
            }
        }
	count = -count;
        
	for (row1 = bottomrow - count + 1; row1 <= bottomrow; row1++) {
	    /* free rows */
#if (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                if (screen->textbuf.wdata[row1]) {
                    free(screen->textbuf.wdata[row1]);
                    screen->textbuf.wdata[row1] = NULL;
                }
            } else if (screen->textbuf.data[row1]) {
		free(screen->textbuf.data[row1]);
		screen->textbuf.data[row1] = NULL;
	    }
#else
	    if (screen->textbuf.data[row1]) {
		free(screen->textbuf.data[row1]);
		screen->textbuf.data[row1] = NULL;
	    }
#endif
            
	    if (screen->textbuf.renddata[row1]) {
		free(screen->textbuf.renddata[row1]);
		screen->textbuf.renddata[row1] = NULL;
	    }
//            screen->textbuf.rowcols[row1] = 0;
	}
#if 0
	for (row1 = bottomrow ;
	     row1 > bottomrow + count ;
	     row1--) {
	    /* free rows */
            
	    if (screen->textbuf.data[row1]) {
		free(screen->textbuf.data[row1]);
		screen->textbuf.data[row1] = NULL;
	    }
            
	    if (screen->textbuf.renddata[row1]) {
		free(screen->textbuf.renddata[row1]);
		screen->textbuf.renddata[row1] = NULL;
	    }
	}
#endif

	for (row1 = bottomrow - count, row2 = bottomrow ;
	     row1 >= toprow;
	     row1--, row2--) {
	    /* copy rows */
            
	    screen->textbuf.rowlens[row2] =
		screen->textbuf.rowlens[row1];
            
	    screen->textbuf.rowcols[row2] =
		screen->textbuf.rowcols[row1];
            
#if (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                screen->textbuf.wdata[row2] =
                    screen->textbuf.wdata[row1];
            } else {
                screen->textbuf.data[row2] =
                    screen->textbuf.data[row1];
            }
#else	    
	    screen->textbuf.data[row2] =
		screen->textbuf.data[row1];
#endif
            
	    screen->textbuf.renddata[row2] =
		screen->textbuf.renddata[row1];
	}
        
	for (row1 = toprow ; row1 < toprow + count ; row1++) {
#if (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                if ((screen->textbuf.wdata[row1] =
                     calloc(screen->columns, sizeof(Rterm_wchar_t)))
                    == NULL) {
                    
                    return -1;
                }
            } else if ((screen->textbuf.data[row1] =
                        calloc(screen->columns, sizeof(R_text_t)))
                       == NULL) {
                
		return -1;
	    }
#else
	    if ((screen->textbuf.data[row1] =
		 calloc(screen->columns, sizeof(R_text_t)))
		== NULL) {

		return -1;
	    }
#endif

	    if ((screen->textbuf.renddata[row1] =
		 calloc(screen->columns, sizeof(R_textflags_t)))
		== NULL) {
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    free(screen->textbuf.wdata[row1]);
                    screen->textbuf.wdata[row1] = NULL;
                } else {
                    free(screen->textbuf.data[row1]);
                    screen->textbuf.data[row1] = NULL;
                }
#else
		free(screen->textbuf.data[row1]);
		screen->textbuf.data[row1] = NULL;
#endif

		return -1;
	    }

	    screen->textbuf.rowlens[row1] = screen->columns;
	    blank_string(screen, row1, 0, screen->textbuf.rowlens[row1]);
//	    screen->textbuf.rowcols[row1] = 0;
	}
    }
    Rterm_change_selection(term);
    if (term->selection.firstrow >= screen->row
        && term->selection.firstrow < screen->row + screen->rows) {
        Rterm_highlight_selection(term);
    }

    if (count > 0 && saverows) {
	screen->nsavelines = MIN(screen->nsavelines + count,
				 screen->savelines);
    }
#if (KLUDGE)
    screen->refreshed = FALSE;
//    screen->refreshcnt = -1;
#endif
#if 0
    if (term->privmodes & RTERM_PRIVMODE_SMOOTH_SCROLL) {
        screen->refreshed = FALSE;
        screen->refreshcnt = -1;
    }
#endif

    return 0;
}

void
Rterm_insert_blank_screen_string(struct R_termscreen *screen, int len)
{
    int row, column;
    void (*blank_string)(struct R_termscreen *, int, int, int);
#if (SUPPORT_RTERM_UTF8_CHARS)
    struct R_term *term = R_global.app->client;
#endif

    if (screen == NULL
	|| len <= 0) {

	return;
    }
    blank_string = screen->funcs.blank_string;

    len = MIN(len, screen->columns - screen->column);

    row = screen->savelines + screen->row;

    screen->flags &= ~RTERM_SCREEN_WRAP_NEXT_CHAR;

#if 0
    if (termptr->privmodes & RTERM_PRIVMODE_HOME_ON_ECHO) {
	termptr->window->screen->viewrow =
	    termptr->window->screen->savelines;
    }
#endif

    for (column = screen->columns - 1 ; column - len >= screen->column ;
	 column--) {
#if (SUPPORT_RTERM_UTF8_CHARS)
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
            screen->textbuf.wdata[row][column] =
                screen->textbuf.wdata[row][(column - len)];
        } else {
            screen->textbuf.data[row][column] =
                screen->textbuf.data[row][(column - len)];
        }
#else
	screen->textbuf.data[row][column] =
	    screen->textbuf.data[row][(column - len)];
#endif
	screen->textbuf.renddata[row][column] =
	    screen->textbuf.renddata[row][(column - len)];
    }

    blank_string(screen, row, screen->column, len);
    screen->textbuf.rowcols[row] += len;

#if (!KLUDGE)
    screen->refreshed = FALSE;
#endif
//    screen->refreshcnt = -1;

    return;
}

void
Rterm_delete_screen_string(struct R_termscreen *screen, int len)
{
    int row, column;
    void (*blank_string)(struct R_termscreen *, int, int, int);
#if (SUPPORT_RTERM_UTF8_CHARS)
    struct R_term *term = R_global.app->client;
#endif

    if (screen == NULL
	|| len <= 0) {

	return;
    }
    blank_string = screen->funcs.blank_string;

    len = MIN(len, screen->columns - screen->column);

    row = screen->savelines + screen->row;

    if (len == 0) {

	return;
    }

    screen->flags &= ~RTERM_SCREEN_WRAP_NEXT_CHAR;

#if 0
    if (termptr->privmodes & RTERM_PRIVMODE_HOME_ON_ECHO) {
	termptr->window->screen->viewrow =
	    termptr->window->screen->savelines;
    }
#endif

    for (column = screen->column ; column + len < screen->columns ; column++) {
#if (SUPPORT_RTERM_UTF8_CHARS)
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
            screen->textbuf.wdata[row][column] =
                screen->textbuf.wdata[row][column + len];
        } else {
            screen->textbuf.data[row][column] =
                screen->textbuf.data[row][column + len];
        }
#else
	screen->textbuf.data[row][column] =
	    screen->textbuf.data[row][column + len];
#endif
	screen->textbuf.renddata[row][column] =
	    screen->textbuf.renddata[row][column + len];
    }
    blank_string(screen, row, column, len);
    screen->textbuf.rowcols[row] -= len;

    /* FIXME: added */
    screen->drawbuf.rowdirty[screen->row] = 1;
#if (!KLUDGE)
    screen->refreshed = FALSE;
#endif
//    screen->refreshcnt = -1;

    return;
}

/* FIXME: should I really check for RTERM_PRIVMODE_ORIGIN_MODE with
 * screen->row > screen->scrollbottom in screen_insert_blank_rows()
 * and screen_delete_rows()?
 */

void
Rterm_blank_screen_string(struct R_termscreen *screen,
                          int row,
                          int column,
                          int len)
{
    R_text_t *textptr;
    R_textflags_t *textflagsptr;
    R_textflags_t textflags;
#if (SUPPORT_RTERM_UTF8_CHARS)
    struct R_term *term = R_global.app->client;
    Rterm_wchar_t *wtextptr;
#endif

    if (screen == NULL
	|| row < 0
	|| row > screen->savelines + screen->rows - 1
	|| column < 0
	|| column > screen->textbuf.rowlens[row] - 1
	|| len <= 0) {

	return;
    }

    len = MIN(len, screen->textbuf.rowlens[row] - column);
#if (SUPPORT_RTERM_UTF8_CHARS)
    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
        if (screen->textbuf.wdata[row]) {
            wtextptr = &(screen->textbuf.wdata[row][column]);
            textflagsptr = &(screen->textbuf.renddata[row][column]);
            textflags = screen->textflags;
            
//            memset(textptr, ' ', len * sizeof(Rterm_wchar_t));
            
            textflags = RTERM_SCREEN_DEFAULT_TEXTFLAGS;
            while (len--) {
                *wtextptr++ = ' ';
                *textflagsptr++ = textflags;
            }
        }
    } else if (screen->textbuf.data[row]) {
        textptr = &(screen->textbuf.data[row][column]);
        textflagsptr = &(screen->textbuf.renddata[row][column]);
        textflags = screen->textflags;
        
//        memset(textptr, ' ', len);
        
        textflags = RTERM_SCREEN_DEFAULT_TEXTFLAGS;
        while (len--) {
            *textptr++ = ' ';
            *textflagsptr++ = textflags;
        }
    }
#else
    textptr = &(screen->textbuf.data[row][column]);
    textflagsptr = &(screen->textbuf.renddata[row][column]);
    textflags = screen->textflags;
    
//    memset(textptr, ' ', len);
    
    textflags = RTERM_SCREEN_DEFAULT_TEXTFLAGS;
    while (len--) {
        *textflagsptr++ = textflags;
    }
#endif
#if (!KLUDGE)
    screen->refreshed = FALSE;
#endif

    return;
}

void
Rterm_blank_screen_drawn_string(struct R_termscreen *screen, int row, int column, int len)
{
    R_text_t *textptr;
    R_textflags_t *textflagsptr;
#if (SUPPORT_RTERM_UTF8_CHARS)
    struct R_term *term = R_global.app->client;
    Rterm_wchar_t *wtextptr;
#endif

    if (screen == NULL
	|| row < 0
	|| row > screen->savelines + screen->rows - 1
	|| column < 0
	|| column > screen->drawbuf.rowlens[row] - 1
	|| len <= 0) {

	return;
    }

    len = MIN(len, screen->drawbuf.rowlens[row] - column);

#if (SUPPORT_RTERM_UTF8_CHARS)
    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
        wtextptr = &(screen->drawbuf.wdata[row][column]);
        
//        memset(textptr, 0, len);
        
        textflagsptr = &(screen->drawbuf.renddata[row][column]);
        
        while (len--) {
            *wtextptr++ = 0;
            *textflagsptr++ = RTERM_SCREEN_DEFAULT_TEXTFLAGS;
        }
    } else {
        textptr = &(screen->drawbuf.data[row][column]);
        
        memset(textptr, 0, len);
        
        textflagsptr = &(screen->drawbuf.renddata[row][column]);
        
        while (len--) {
            *textflagsptr++ = RTERM_SCREEN_DEFAULT_TEXTFLAGS;
        }
    }
#else
    textptr = &(screen->drawbuf.data[row][column]);

    memset(textptr, 0, len);

    textflagsptr = &(screen->drawbuf.renddata[row][column]);

    while (len--) {
	*textflagsptr++ = RTERM_SCREEN_DEFAULT_TEXTFLAGS;
    }
#endif

    return;
}

void
Rterm_refresh_screen(struct R_termscreen *screen, int mode)
{
    struct R_term *term;
    int row, column, len;
    R_text_t *textptr = NULL;
    R_textflags_t *textflagsptr;
    void (*blank_drawn)(struct R_termscreen *, int, int, int);
    void (*blank_string)(struct R_termscreen *, int, int, int);
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    Pixmap pixmap;
#endif
#if (SUPPORT_RTERM_UTF8_CHARS)
    Rterm_wchar_t *wtextptr = NULL;
#endif
    
    if (screen == NULL
	|| mode < 0
	|| mode > RTERM_SCREEN_DRAW_MODES
	|| screen->row < 0) {
	
	return;
    }

    term = R_global.app->client;
    len = 0;
    blank_drawn = screen->funcs.blank_drawn;
    blank_string = screen->funcs.blank_string;

#if 0
    if (!term->blink) {
        Rterm_clear_screen_cursor(screen);
    }
#endif

#if 0
#if (SUPPORT_RTERM_BLINKING_CHARS)
    if (!(screen->flags & RTERM_SCREEN_HAS_BLINKING_CHARS)) {
	term->blink = TRUE;
    }
#endif /* SUPPORT_RTERM_BLINKING_CHARS */
#endif

    if (screen->buf) {
        Rterm_clear_screen_buffer(screen, 0, 0, 0,
                                  RTERM_SCREEN_CLEAR_BUFFER);
    }
    for (row = 0; row < screen->rows ; row++) {
        len = MIN(screen->columns,
                  screen->textbuf.rowcols[(screen->viewrow + row)]);
	if (len) {
#if (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                wtextptr = screen->textbuf.wdata[(screen->viewrow + row)];
            } else {
                textptr = screen->textbuf.data[(screen->viewrow + row)];
            }
#else
            textptr = screen->textbuf.data[(screen->viewrow + row)];
#endif
	    textflagsptr = screen->textbuf.renddata[(screen->viewrow + row)];

#if (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                Rterm_draw_screen_string_wc(screen, row, 0, wtextptr, textflagsptr, len,
                                            mode,
                                            TRUE);
            } else {
                Rterm_draw_screen_string_8bit(screen, row, 0, textptr, textflagsptr, len,
                                              mode,
                                              TRUE);
            }
#else
            Rterm_draw_screen_string_8bit(screen, row, 0, textptr, textflagsptr, len,
                                          mode,
                                          TRUE);
#endif

#if 0

	    if (mode == RTERM_SCREEN_DRAW_BLINKING_ONLY) {
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    Rterm_draw_screen_string_wc(screen, row, 0, wtextptr, textflagsptr, len,
                                                RTERM_SCREEN_DRAW_BLINKING_ONLY,
                                                TRUE);
                } else {
                    Rterm_draw_screen_string_8bit(screen, row, 0, textptr, textflagsptr, len,
                                                  RTERM_SCREEN_DRAW_BLINKING_ONLY,
                                                  TRUE);
                }
#else
		Rterm_draw_screen_string_8bit(screen, row, 0, textptr, textflagsptr, len,
                                              RTERM_SCREEN_DRAW_BLINKING_ONLY,
                                              TRUE);
#endif

                return;
	    } else {
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    Rterm_draw_screen_string_wc(screen, row, 0, wtextptr, textflagsptr, len,
                                                RTERM_SCREEN_DRAW_ALL,
                                                TRUE);
                } else {
                    Rterm_draw_screen_string_8bit(screen, row, 0, textptr, textflagsptr, len,
                                                  RTERM_SCREEN_DRAW_ALL,
                                                  TRUE);
                }
#else
                Rterm_draw_screen_string_8bit(screen, row, 0, textptr, textflagsptr, len,
                                         RTERM_SCREEN_DRAW_ALL,
                                         TRUE);
#endif
	    }

#endif

	    screen->drawbuf.rowcols[row] = len;

            if (len > 0 && screen == term->screens[term->curscreen]) {
#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS && (wtextptr)) {
                    memcpy(screen->drawbuf.wdata[row],
                           wtextptr,
                           len * sizeof(Rterm_wchar_t));
                } else if (textptr) {
                    memcpy(screen->drawbuf.data[row],
                           textptr,
                           len * sizeof(R_text_t));
                }
#else
                if (textptr) {
                    memcpy(screen->drawbuf.data[row],
                           textptr,
                           len * sizeof(R_text_t));
                }
#endif
                
                memcpy(screen->drawbuf.renddata[row],
                       textflagsptr,
                       len * sizeof(R_textflags_t));
            }
	}
        if (len < screen->textbuf.rowlens[screen->viewrow + row]) {
            column = len;
            len =  screen->textbuf.rowlens[screen->viewrow + row] - column;
            blank_string(screen, row + screen->savelines, column, len);
            blank_drawn(screen, row, column, len);
            Rterm_clear_screen_string(screen, row, column, len);
        }
        screen->drawbuf.rowdirty[row] = 1;
    }
    if (screen->buf) {
        Rterm_sync_screen(screen, 0, 0, 0, RTERM_SCREEN_SYNC);
#if !SUPPORT_RTERM_BLINKING_CURSOR
    } else {
        Rterm_draw_screen_cursor(screen);
    }
#endif
    screen->refreshed = TRUE;
//    XSync(screen->window->app->display, False);
    
    return;
}

int
Rterm_read_char(struct R_term *term)
{
    struct R_app *app;
    long evtype;
    unsigned int scrollbary, dummyui;
    int dummyi, i, nfdsready;
    ssize_t nbread;
    fd_set readfds;
    struct timeval timeval;
    struct R_termscreen *screen, *curscreen;
    Window dummywin;
    void (*refresh)(struct R_termscreen *, int);
    void (*ptyrefresh)(struct R_termscreen *, int);
    XEvent event;
#if (SUPPORT_RTERM_UTF8_CHARS)
    R_text_t *convstr;
    int len;
#endif
#if (SUPPORT_RTERM_TABS)
    void (*add_text)(struct R_term *, struct R_termscreen *);
#endif
    
    if (term == NULL) {
	
	return -1;
    }
#if (SUPPORT_RTERM_TABS)
    add_text = term->funcs.add_text;
#endif    

    app = R_global.app;
#if (SUPPORT_RTERM_TABS)
    curscreen = term->screens[term->curscreen];
    if (curscreen->pty.masterfd == -1) {
        
        return -1;
    }

    if (curscreen->inptr < curscreen->inend) {
	curscreen->refreshed = FALSE;
        
	return (*curscreen->inptr++);
    }
    
    for (i = 0 ; i < term->nscreens ; i++) {
        screen = term->screens[i];
        refresh = screen->funcs.refresh;
        if (screen->pty.masterfd < 0) {

            return -1;
        }
        if (screen->refreshcnt < 0
            || screen->refreshcnt >= screen->rows * RTERM_REFRESH_THRESHOLD_SCREENS) {
            screen->refreshcnt = 0;
            screen->refreshed = TRUE;
            refresh(screen, RTERM_SCREEN_DRAW_ALL);
        }
    }
#else
    if (!(term->privmodes & RTERM_PRIVMODE_SECONDARY_SCREEN)) {
        term->screen = term->screens[0];
    } else {
        term->screen = term->screens[1];
    }
    curscreen = term->screen;
#endif
    refresh = curscreen->funcs.refresh;

#if 0
    if (!term->blink) {
        Rterm_clear_screen_cursor(screen);
    }
#endif

    if (curscreen->refreshcnt >= curscreen->rows * RTERM_REFRESH_THRESHOLD_SCREENS) {
        Rterm_touch_screen(curscreen);
	refresh(curscreen, RTERM_SCREEN_DRAW_ALL);
	curscreen->refreshcnt = 0;
	curscreen->refreshed = TRUE;
//        Rterm_update_scrollbar(term);
    }
    
    if (curscreen->inptr < curscreen->inend) {
	curscreen->refreshed = FALSE;
        
	return (*curscreen->inptr++);
    }
    
    for ( ; ; ) {
	for (i = 0 ; i < term->nscreens ; i++) {
            if (curscreen->inptr < curscreen->inend) {
                curscreen->refreshed = FALSE;
                
                return (*curscreen->inptr++);
            }
    
            screen = term->screens[i];
            if (screen->pty.masterfd >= 0) {
                if (screen->outend > screen->outbuf) {
                    RTERM_WRITE_PTY(term,
                                    screen->pty.masterfd, screen->outbuf,
                                    screen->outend - screen->outbuf);
                    screen->outptr = screen->outbuf;
                    screen->outend = screen->outbuf;
                    
                    if (screen->outsize > RTERM_OUTPUT_BUFFER_MIN_SIZE) {
                        if ((screen->outbuf = realloc(screen->outbuf,
                                                      RTERM_OUTPUT_BUFFER_MIN_SIZE))
                            == NULL) {
                            fprintf(stderr, "term_read_char(): failed to allocate output buffer\n");
                            
                            return -1;
                        }
                        
                        screen->outptr = screen->outbuf;
                        screen->outend = screen->outbuf;
                        screen->outsize = RTERM_OUTPUT_BUFFER_MIN_SIZE;
                    }
                }
            }
        }
        
        /* block to wait for event if none queued. */
        while(XPending(app->display)) {
            if (curscreen->pty.masterfd == -1) {

                return -1;
            }
            evtype = R_handle_next_event(app);
            if (evtype == KeyPress) {
                curscreen->refreshcnt = -1;
#if (SUPPORT_RTERM_TABS)
                
//                return 0;
#endif
//                refresh(screen, RTERM_SCREEN_DRAW_ALL);
//        Rterm_update_scrollbar(term);
            }
            if (curscreen->pty.masterfd >= 0) {
                if (curscreen->inptr < curscreen->inend) {
                    curscreen->refreshed = FALSE;
                    
                    return (*curscreen->inptr++);
                }
            } else {
                
                return 0;
            }
        }

#if 0
        if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
	    if ((term->scrollbar->flags & RTERM_SCROLLBAR_UP_ARROW_PRESSED)
		|| (term->scrollbar->flags
		    & RTERM_SCROLLBAR_DOWN_ARROW_PRESSED)
		|| (term->scrollbar->flags & RTERM_SCROLLBAR_PRESSED)) {
		curscreen->refreshed = FALSE;
	    }
	}
#endif

	FD_ZERO(&readfds);

#if (SUPPORT_RTERM_TABS)
	FD_SET(term->xconnfd, &readfds);
#if 0
        timeval.tv_sec = 0;
        timeval.tv_usec = RTERM_SELECT_TIMEOUT_USEC;
	
        nfdsready = Rselect(term->nfds, &readfds, NULL, NULL, &timeval);
        if (FD_ISSET(term->xconnfd, &readfds)) {

            return 0;
        }
#endif
#endif

#if (SUPPORT_RTERM_TABS)
	for (i = 0 ; i < term->nscreens ; i++) {
            if (term->screens[i]->pty.masterfd >= 0) {
                FD_SET(term->screens[i]->pty.masterfd, &readfds);
            }
	}
#else
        FD_SET(curscreen->pty.masterfd, &readfds);
#endif

#if 0
        if ((term->privmodes & RTERM_PRIVMODE_SCROLLBAR)
	    && ((term->scrollbar->flags & RTERM_SCROLLBAR_UP_ARROW_PRESSED)
		|| (term->scrollbar->flags
		    & RTERM_SCROLLBAR_DOWN_ARROW_PRESSED))) {
	    timeval.tv_sec = 0;
	    timeval.tv_usec = RTERM_SELECT_SCROLL_TIMEOUT_USEC;
	    
	    nfdsready = Rselect(term->nfds, &readfds, NULL, NULL, &timeval);
	} else if ((term->privmodes & RTERM_PRIVMODE_SCROLLBAR)
		   && (term->scrollbar->flags & RTERM_SCROLLBAR_PRESSED)) {
	    timeval.tv_sec = 0;
	    timeval.tv_usec = RTERM_SELECT_SCROLLBAR_PRESSED_TIMEOUT_USEC;
	    
	    nfdsready = Rselect(term->nfds, &readfds, NULL, NULL, &timeval);
	} else
#endif
        if (!curscreen->refreshed) {
	    timeval.tv_sec = 0;
	    timeval.tv_usec = RTERM_SELECT_TIMEOUT_USEC;
	    
	    nfdsready = Rselect(term->nfds, &readfds, NULL, NULL, &timeval);
	}
#if (SUPPORT_RTERM_BLINKING_CURSOR) || (SUPPORT_RTERM_BLINKING_CHARS)
	else if ((term->window->stateflags & R_WINDOW_FOCUSED_FLAG)
                 && (curscreen->flags & RTERM_SCREEN_HAS_BLINKING_CURSOR
                     || (curscreen->flags & RTERM_SCREEN_HAS_BLINKING_CHARS))) {
	    timeval.tv_sec = 0;
	    timeval.tv_usec = RTERM_SELECT_BLINK_TIMEOUT_USEC;
	    
	    nfdsready = Rselect(term->nfds, &readfds, NULL, NULL, &timeval);
	}
#endif
        else {
	    nfdsready = Rselect(term->nfds, &readfds, NULL, NULL, NULL);
	}

        if (nfdsready == -1 && errno != EINTR) {

                return -1;
        }

        if (nfdsready) {
            if (curscreen->inptr < curscreen->inend) {
                curscreen->refreshed = FALSE;
                
                return (*curscreen->inptr++);
            }
            for (i = 0 ; i < term->nscreens ; i++) {
                screen = term->screens[i];
                if (screen->pty.masterfd >= 0) {
                    if (FD_ISSET(screen->pty.masterfd, &readfds)) {
                        screen->inptr = screen->inbuf;
                        screen->inend = screen->inbuf;
                        
                        if ((nbread = R_read_pty(screen->pty.masterfd, screen->inend,
                                                 RTERM_INPUT_BUFFER_SIZE)) < 0) {
                            
                            return -1;
                        }
#if 0
                        if (errno == EIO) {

                            return -1;
                        }
#endif
                        screen->inend += nbread;

#if (SUPPORT_RTERM_TABS)
                        if (screen != curscreen && screen->inptr < screen->inend) {
                            add_text(term, screen);
#if 0
                            Rterm_touch_screen(screen);
                            refresh(screen, RTERM_SCREEN_DRAW_ALL);
                            screen->refreshcnt = 0;
                            screen->refreshed = TRUE;
#endif
#if (!KLUDGE)
                            screen->refreshcnt = -1;
#endif
                        }
#endif
                    }
                } else if (screen->cmdpid) {
                    
                    return -1;
                }
#if 0
                if (curscreen->inptr < curscreen->inend) {
                    curscreen->refreshed = FALSE;
                    
                    return (*curscreen->inptr++);
                }
#endif
            }
        } else {
            curscreen->refreshcnt = -1;
#if 0
            if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
                if (term->scrollbar->flags
                    & RTERM_SCROLLBAR_UP_ARROW_PRESSED) {
                    Rterm_scroll_screen(curscreen, -1);
                } else if (term->scrollbar->flags
                           & RTERM_SCROLLBAR_DOWN_ARROW_PRESSED) {
                    Rterm_scroll_screen(curscreen, 1);
                } else if (term->scrollbar->flags
                       & RTERM_SCROLLBAR_PRESSED) {
                    XQueryPointer(app->display,
                                  term->scrollbar->window->id,
                                  &dummywin, &dummywin,
                                  &dummyi, &dummyi,
                                  &dummyui, &scrollbary,
                                  &dummyui);
                    if (scrollbary < term->scrollbar->anchortop) {
                        Rterm_scroll_screen(curscreen, -(curscreen->rows - 1));
                    } else if (scrollbary
                               > term->scrollbar->anchorbottom) {
                        Rterm_scroll_screen(curscreen, curscreen->rows - 1);
                    }
                }
#endif
                
//        Rterm_update_scrollbar(term);
            }
            
            return 0;
        }

    if (!curscreen->refreshed) {
	curscreen->refreshcnt = 0;
	curscreen->refreshed = TRUE;
        if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
            if (term->scrollbar->flags
                & RTERM_SCROLLBAR_UP_ARROW_PRESSED) {
                Rterm_scroll_screen(curscreen, -1);
            } else if (term->scrollbar->flags
                       & RTERM_SCROLLBAR_DOWN_ARROW_PRESSED) {
                Rterm_scroll_screen(curscreen, 1);
            } else if (term->scrollbar->flags
                       & RTERM_SCROLLBAR_PRESSED) {
                XQueryPointer(app->display,
                              term->scrollbar->window->id,
                              &dummywin, &dummywin,
                              &dummyi, &dummyi,
                              &dummyui, &scrollbary,
                              &dummyui);
                if (scrollbary < term->scrollbar->anchortop) {
                    Rterm_scroll_screen(curscreen, -(curscreen->rows - 1));
                } else if (scrollbary
                           > term->scrollbar->anchorbottom) {
                    Rterm_scroll_screen(curscreen, curscreen->rows - 1);
                }
            }
//            Rterm_update_scrollbar(term);
//        Rterm_update_scrollbar(term);
        }
    }
    
    return 0;
}

void
Rterm_add_text(struct R_term *term, struct R_termscreen *screen)
{
    struct R_app *app;
    int ch, rows;
    R_text_t *inputstr;
    void (*add_text)();
    int cval;
#if (SUPPORT_RTERM_UTF8_CHARS)
    Rterm_wchar_t wch;
    int len;
#endif

    if (term == NULL
	|| screen == NULL) {

	return;
    }
    add_text = screen->funcs.add_text;

#if 0
    term = R_global.app->client;
    if (!term->blink) {
        Rterm_clear_screen_cursor(screen);
    }
#endif

    app = term->window->app;
    screen->inptr--;
#if (SUPPORT_RTERM_UTF8_CHARS)
    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
        cval = Rterm_decode_char_utf8(screen->inptr, &len);
        if (!len) {
            
            return;
        }
        inputstr = screen->inptr;
        screen->inptr += len;
    } else {
        ch = *screen->inptr;
        inputstr = screen->inptr++;
        cval = ch;
    }
#else    
    cval = *screen->inptr;
    inputstr = screen->inptr++;
#endif

    if (cval >= ' ' || cval == '\t' || cval == '\n' || cval == '\r') {
	rows = 0;
	/* read string from input buffer. */
	
	while (screen->inptr < screen->inend) {
#if (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                cval = Rterm_decode_char_utf8(screen->inptr, &len);
                screen->inptr += len;
            } else {
                cval = *screen->inptr++;
            }
#else
	    cval = *screen->inptr++;
#endif
	    
	    if (cval >= ' ' || cval == '\t' || cval == '\r') {
                
                continue;
	    } else if (cval == '\n') {
		rows++;
                screen->refreshed = FALSE;
                
                break;
	    } else {
		/* unprintable. */

#if (SUPPORT_RTERM_UTF8_CHARS)
                if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                    screen->inptr -= len;
                } else {
                    screen->inptr--;
                }
#else                
		screen->inptr--;
#endif

		break;
	    }
	}
	
	add_text(screen, inputstr, screen->inptr - inputstr,
		 rows);
    } else if (cval) {
	switch (cval) {
	    case R_ENQ_CHAR:
#if (USE_VT100_ANSWER)
                RTERM_WRITE_PTY(term,
                                screen->pty.masterfd, RTERM_VT100_ANSWER,
                                sizeof(RTERM_VT100_ANSWER) - 1);
#endif
		
		break;
	    case R_BEL_CHAR:
		/* FIXME: write screen_bell()? */
		XBell(app->display, 50);
		
		break;
	    case R_BACKSPACE_CHAR:
		Rterm_screen_backspace(screen);
		
		break;
	    case R_VERTICAL_TAB_CHAR:
	    case R_FORM_FEED_CHAR:
		/* scroll one line up. */
		
		Rterm_scroll_screen(screen, 1);

#if 0		
		if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
		    Rterm_update_scrollbar(term);
		}
#endif
		
		break;
		/* TODO: make sure the charsets are right. */
	    case R_SO_CHAR:
		Rterm_choose_charset(term, RTERM_CHARSET_G1);
		
		break;
	    case R_SI_CHAR:
		Rterm_choose_charset(term, RTERM_CHARSET_G0);
		
		break;
	    case R_ESC_CHAR:
		Rterm_process_escape_sequence(term, screen);
		
		break;
	    default:
		
		break;
	}
    }

    return;
}

int
Rterm_queue_input(struct R_term *term, const char *str, int len)
{
    R_text_t *dest;
    struct R_termscreen *screen;

    if (term == NULL
	|| str == NULL
	|| len <= 0) {

	return -1;
    }

    screen = term->screens[term->curscreen];

    len = MIN(len,
	      screen->inbuf
	      + sizeof(screen->inbuf)
	      - screen->inend);

    if (len > 0) {
	dest = screen->inend;
	screen->inend += len;
	while (len--) {
	    *dest++ = *str++;
	}
    }

    return len;
}

int
Rterm_queue_output(struct R_term *term, const char *str, int len)
{
    int datalen;
    R_text_t *dest;
    struct R_termscreen *screen;

    if (term == NULL
	|| str == NULL
	|| len <= 0) {

	return -1;
    }

    len = MAX(len, strlen(str));

    screen = term->screens[term->curscreen];

    if (len > 0) {
	datalen = screen->outend - screen->outbuf;
	
	if (len > screen->outsize - datalen) {
#if 0 && (SUPPORT_RTERM_UTF8_CHARS)
            if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
                if ((screen->utf8buf = realloc(screen->utf8buf,
                                               4 * (datalen + len))) == NULL) {
		
                    return -1;
                }
            }
#endif
	    if ((screen->outbuf = realloc(screen->outbuf,
                                          datalen + len)) == NULL) {
		
		return -1;
	    }

	    screen->outend = screen->outbuf + datalen;
	    screen->outsize = datalen + len;
	}
	
	dest = screen->outend;
	screen->outend += len;
	while (len--) {
	    *dest++ = *str++;
	}
    }

    return len;
}

