/*
 * screen.c - terminal screen routines for Resurrection.
 * Copyright (C) 2003 - 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#define RTERM_SCREEN_DEFAULT_TAB_SIZE 8
#define RTERM_SCREEN_DEFAULT_SAVELINES 4096

#define RTERM_SCREEN_PRINT_VISIBLE_ROWS 0
#define RTERM_SCREEN_PRINT_ALL_ROWS     1

#include <Resurrection/Resurrection.h>

#if 0
	R_set_window_button_action(newscreen->window,
                                   R_BUTTON1_PRESS,
                                   Rterm_start_selection);
	R_set_window_button_action(newscreen->window,
                                   R_BUTTON1_RELEASE,
                                   Rterm_finish_selection);
	R_set_window_button_action(newscreen->window,
                                   R_BUTTON2_PRESS,
                                   Rterm_request_selection);
	R_set_window_button_action(newscreen->window,
                                   R_BUTTON3_PRESS,
                                   Rterm_finish_selection);
        
#define RTERM_SCREEN_BINDINGS 4
struct R_binding Rterm_screen_bindings[] = {
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON1_FLAG),
        Rterm_start_selection,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON1_FLAG | R_BINDING_RELEASE_FLAG),
        Rterm_finish_selection,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON2_FLAG),
        Rterm_request_selection,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON3_FLAG),
        Rterm_finish_selection,
        NULL,
        NULL
    }
};

void
Rterm_init_screen_bindings(struct R_termscreen *screen)
{
    struct R_window *window;
    struct R_binding *binding;
    int i;

    window = screen->window;
    binding = &Rterm_screen_bindings[0];
    i = RTERM_SCREEN_BINDINGS;
    while (i--) {
        R_add_binding(binding,
                      window);
        binding++;
    }
}

#endif

static const char *ansicolornames[RTERM_SCREEN_COLORS] =
{
    RTERM_BLACK,  
    RTERM_RED,
    RTERM_GREEN,
    RTERM_YELLOW,
    RTERM_BLUE,
    RTERM_MAGENTA,
    RTERM_CYAN,
    RTERM_WHITE,

    RTERM_BRIGHT_BLACK,
    RTERM_BRIGHT_RED,
    RTERM_BRIGHT_GREEN,
    RTERM_BRIGHT_YELLOW,
    RTERM_BRIGHT_BLUE,
    RTERM_BRIGHT_MAGENTA,
    RTERM_BRIGHT_CYAN,
    RTERM_BRIGHT_WHITE
};

const char *defaultcolornames[RTERM_SCREEN_DEFAULT_COLORS] =
{
    RTERM_SCREEN_DEFAULT_FOREGROUND,
    RTERM_SCREEN_DEFAULT_BACKGROUND
};

static char *xtermcolornames[RTERM_SCREEN_COLORS] =
{
    RTERM_COLOR_0, RTERM_COLOR_1, RTERM_COLOR_2, RTERM_COLOR_3,
    RTERM_COLOR_4, RTERM_COLOR_5, RTERM_COLOR_6, RTERM_COLOR_7,
    RTERM_COLOR_8, RTERM_COLOR_9, RTERM_COLOR_10, RTERM_COLOR_11,
    RTERM_COLOR_12, RTERM_COLOR_13, RTERM_COLOR_14, RTERM_COLOR_15,
    RTERM_COLOR_16, RTERM_COLOR_17, RTERM_COLOR_18, RTERM_COLOR_19,
    RTERM_COLOR_20, RTERM_COLOR_21, RTERM_COLOR_22, RTERM_COLOR_23,
    RTERM_COLOR_24, RTERM_COLOR_25, RTERM_COLOR_26, RTERM_COLOR_27,
    RTERM_COLOR_28, RTERM_COLOR_29, RTERM_COLOR_30, RTERM_COLOR_31,
    RTERM_COLOR_32, RTERM_COLOR_33, RTERM_COLOR_34, RTERM_COLOR_35,
    RTERM_COLOR_36, RTERM_COLOR_37, RTERM_COLOR_38, RTERM_COLOR_39,
    RTERM_COLOR_40, RTERM_COLOR_41, RTERM_COLOR_42, RTERM_COLOR_43,
    RTERM_COLOR_44, RTERM_COLOR_45, RTERM_COLOR_46, RTERM_COLOR_47,
    RTERM_COLOR_48, RTERM_COLOR_49, RTERM_COLOR_50, RTERM_COLOR_51,
    RTERM_COLOR_52, RTERM_COLOR_53, RTERM_COLOR_54, RTERM_COLOR_55,
    RTERM_COLOR_56, RTERM_COLOR_57, RTERM_COLOR_58, RTERM_COLOR_59,
    RTERM_COLOR_60, RTERM_COLOR_61, RTERM_COLOR_62, RTERM_COLOR_63,
    RTERM_COLOR_64, RTERM_COLOR_65, RTERM_COLOR_66, RTERM_COLOR_67,
    RTERM_COLOR_68, RTERM_COLOR_69, RTERM_COLOR_70, RTERM_COLOR_71,
    RTERM_COLOR_72, RTERM_COLOR_73, RTERM_COLOR_74, RTERM_COLOR_75,
    RTERM_COLOR_76, RTERM_COLOR_77, RTERM_COLOR_78, RTERM_COLOR_79,
    RTERM_COLOR_80, RTERM_COLOR_81, RTERM_COLOR_82, RTERM_COLOR_83,
    RTERM_COLOR_84, RTERM_COLOR_85, RTERM_COLOR_86, RTERM_COLOR_87,
    RTERM_COLOR_88, RTERM_COLOR_89, RTERM_COLOR_90, RTERM_COLOR_91,
    RTERM_COLOR_92, RTERM_COLOR_93, RTERM_COLOR_94, RTERM_COLOR_95,
    RTERM_COLOR_96, RTERM_COLOR_97, RTERM_COLOR_98, RTERM_COLOR_99,
    RTERM_COLOR_100, RTERM_COLOR_101, RTERM_COLOR_102, RTERM_COLOR_103,
    RTERM_COLOR_104, RTERM_COLOR_105, RTERM_COLOR_106, RTERM_COLOR_107,
    RTERM_COLOR_108, RTERM_COLOR_109, RTERM_COLOR_110, RTERM_COLOR_111,
    RTERM_COLOR_112, RTERM_COLOR_113, RTERM_COLOR_114, RTERM_COLOR_115,
    RTERM_COLOR_116, RTERM_COLOR_117, RTERM_COLOR_118, RTERM_COLOR_119,
    RTERM_COLOR_120, RTERM_COLOR_121, RTERM_COLOR_122, RTERM_COLOR_123,
    RTERM_COLOR_124, RTERM_COLOR_125, RTERM_COLOR_126, RTERM_COLOR_127,
    RTERM_COLOR_128, RTERM_COLOR_129, RTERM_COLOR_130, RTERM_COLOR_131,
    RTERM_COLOR_132, RTERM_COLOR_133, RTERM_COLOR_134, RTERM_COLOR_135,
    RTERM_COLOR_136, RTERM_COLOR_137, RTERM_COLOR_138, RTERM_COLOR_139,
    RTERM_COLOR_140, RTERM_COLOR_141, RTERM_COLOR_142, RTERM_COLOR_143,
    RTERM_COLOR_144, RTERM_COLOR_145, RTERM_COLOR_146, RTERM_COLOR_147,
    RTERM_COLOR_148, RTERM_COLOR_149, RTERM_COLOR_150, RTERM_COLOR_151,
    RTERM_COLOR_152, RTERM_COLOR_153, RTERM_COLOR_154, RTERM_COLOR_155,
    RTERM_COLOR_156, RTERM_COLOR_157, RTERM_COLOR_158, RTERM_COLOR_159,
    RTERM_COLOR_160, RTERM_COLOR_161, RTERM_COLOR_162, RTERM_COLOR_163,
    RTERM_COLOR_164, RTERM_COLOR_165, RTERM_COLOR_166, RTERM_COLOR_167,
    RTERM_COLOR_168, RTERM_COLOR_169, RTERM_COLOR_170, RTERM_COLOR_171,
    RTERM_COLOR_172, RTERM_COLOR_173, RTERM_COLOR_174, RTERM_COLOR_175,
    RTERM_COLOR_176, RTERM_COLOR_177, RTERM_COLOR_178, RTERM_COLOR_179,
    RTERM_COLOR_180, RTERM_COLOR_181, RTERM_COLOR_182, RTERM_COLOR_183,
    RTERM_COLOR_184, RTERM_COLOR_185, RTERM_COLOR_186, RTERM_COLOR_187,
    RTERM_COLOR_188, RTERM_COLOR_189, RTERM_COLOR_190, RTERM_COLOR_191,
    RTERM_COLOR_192, RTERM_COLOR_193, RTERM_COLOR_194, RTERM_COLOR_195,
    RTERM_COLOR_196, RTERM_COLOR_197, RTERM_COLOR_198, RTERM_COLOR_199,
    RTERM_COLOR_200, RTERM_COLOR_201, RTERM_COLOR_202, RTERM_COLOR_203,
    RTERM_COLOR_204, RTERM_COLOR_205, RTERM_COLOR_206, RTERM_COLOR_207,
    RTERM_COLOR_208, RTERM_COLOR_209, RTERM_COLOR_210, RTERM_COLOR_211,
    RTERM_COLOR_212, RTERM_COLOR_213, RTERM_COLOR_214, RTERM_COLOR_215,
    RTERM_COLOR_216, RTERM_COLOR_217, RTERM_COLOR_218, RTERM_COLOR_219,
    RTERM_COLOR_220, RTERM_COLOR_221, RTERM_COLOR_222, RTERM_COLOR_223,
    RTERM_COLOR_224, RTERM_COLOR_225, RTERM_COLOR_226, RTERM_COLOR_227,
    RTERM_COLOR_228, RTERM_COLOR_229, RTERM_COLOR_230, RTERM_COLOR_231,
    RTERM_COLOR_232, RTERM_COLOR_233, RTERM_COLOR_234, RTERM_COLOR_235,
    RTERM_COLOR_236, RTERM_COLOR_237, RTERM_COLOR_238, RTERM_COLOR_239,
    RTERM_COLOR_240, RTERM_COLOR_241, RTERM_COLOR_242, RTERM_COLOR_243,
    RTERM_COLOR_244, RTERM_COLOR_245, RTERM_COLOR_246, RTERM_COLOR_247,
    RTERM_COLOR_248, RTERM_COLOR_249, RTERM_COLOR_250, RTERM_COLOR_251,
    RTERM_COLOR_252, RTERM_COLOR_253, RTERM_COLOR_254, RTERM_COLOR_255
};

struct R_termscreen *
Rterm_alloc_screen(void)
{
    return (calloc(1, sizeof(struct R_termscreen)));
}

int
Rterm_load_screen_draw_image(struct R_termscreen *screen)
{
    struct R_term *term = R_global.app->client;
    struct R_app *app;
    int           retval = -1;

    app = R_global.app;
    if (term->bgname) {
        R_load_image_imlib2(app,
                            term->bgname,
                            &screen->drawimage);
                            
    } else {
        
        R_load_image_imlib2(app,
                            RESURRECTION_IMAGE_SEARCH_PATH "background/darkstone.png",
                            &screen->drawimage);
    }
    if (!(term->flags & RTERM_DOUBLE_BUFFER)) {
        if (R_render_image_imlib2(&screen->drawimage,
                                  screen->window,
                                  R_TILE_IMAGE,
                                  R_TILE_IMAGE,
                                  R_IMAGE_STATIC_FLAG) < 0) {                                          
            retval = -1;
        } else {
            
            retval = 0;
        }
    }
    if (screen->drawimage.orig) {

        retval = 0;
    }
    
    return retval;
}

#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)

void
Rterm_create_screen_draw_pixmap(struct R_termscreen *screen)
{
    struct R_term *term = R_global.app->client;
    
    if (!Rterm_load_screen_draw_image(screen)) {
        if (screen->drawimage.orig) {
            if (term->flags & RTERM_SCALE_BACKGROUND_PIXMAP) {
                if (R_render_image_imlib2(&screen->drawimage,
                                          screen->window,
                                          R_TILE_IMAGE,
                                          R_TILE_IMAGE,
                                          R_IMAGE_STATIC_FLAG) < 0) {                                          
                    
                    screen->drawpixmap = None;
                } else {
                    fprintf(stderr, "SET SCREEN PIXMAPS (%s)\n", term->bgname);
                    screen->drawpixmap = screen->drawimage.pixmap;
                }
            } else {
                if (R_render_image_imlib2(&screen->drawimage,
                                          screen->window,
#if 0
                                          screen->drawimage.origw,
                                          screen->drawimage.origh,
#endif
                                          R_TILE_IMAGE,
                                          R_TILE_IMAGE,
                                          R_IMAGE_FILL_FLAG | R_IMAGE_STATIC_FLAG) < 0) {
                    screen->drawpixmap = None;
                } else {
                    screen->drawpixmap = screen->drawimage.pixmap;
                }
            }
        }
    }

    return;
}

void
Rterm_alloc_screen_draw_buffer(struct R_termscreen *screen)
{
    struct R_app *app;
    Pixmap buffer;
    Pixmap pixmap;
#if (RTERM_USE_XSHM)
    XShmSegmentInfo *shmseg;
    int shmid;
#endif
    XWindowAttributes attributes;

    if (screen == NULL) {

	return;
    }

    app = R_global.app;
#if (RTERM_USE_XSHM)
    shmseg = &screen->bufshminfo;
#endif
#if 0
    memset(&attributes, 0, sizeof(attributes));
    XGetWindowAttributes(app->display, screen->window->id, &attributes);
#endif
    if (screen->buf) {
#if (RTERM_USE_XSHM)
        if (shmseg->shmaddr) {
            XShmDetach(app->display, shmseg);
            shmdt(shmseg->shmaddr);
            shmctl(shmseg->shmid, IPC_RMID, NULL);
            shmseg->shmid = -1;
            shmseg->shmaddr = NULL;
        } else {
            XFreePixmap(screen->window->app->display, screen->buf);
        }
#else
	XFreePixmap(screen->window->app->display, screen->buf);
#endif
        screen->buf = None;
    }
#if (RTERM_USE_XSHM)
#if 0
    shmid = shmget(IPC_PRIVATE,
                   attributes.width * attributes.height * 4,
                   0777 | IPC_CREAT);
#endif
    shmid = shmget(IPC_PRIVATE,
                   Rterm_screen_width(screen) * Rterm_screen_height(screen) * 4,
                   0777 | IPC_CREAT);
    if (shmid >= 0) {
        shmseg->shmaddr = shmat(shmid, NULL, 0);
        if (shmseg->shmaddr == ((void *)-1)) {
            shmid = -1;
            shmseg->shmaddr = NULL;
        } else {
            shmseg->shmid = shmid;
            shmseg->readOnly = False;
        }
    }
    if (shmid >= 0) {
        XShmAttach(app->display, shmseg);
        buffer = XShmCreatePixmap(app->display,
                                  app->window->id,
                                  shmseg->shmaddr,
                                  shmseg,
#if 0
                                  attributes.width,
                                  attributes.height,
#endif
                                  Rterm_screen_width(screen),
                                  Rterm_screen_height(screen),
                                  app->depth);
        XSync(app->display, False);
    } else {
        shmseg->shmid = -1;
        shmseg->shmaddr = NULL;
        buffer = XCreatePixmap(app->display, app->window->id,
                               attributes.width, attributes.height,
                               app->depth);
    }
#else
#if 0
    buffer = XCreatePixmap(app->display, app->window->id,
			   attributes.width, attributes.height,
			   app->depth);
#endif
    buffer = XCreatePixmap(app->display, app->window->id,
			   Rterm_screen_width(screen), Rterm_screen_height(screen),
			   app->depth);
#endif
#if 0
    screen->winw = attributes.width;
    screen->winh = attributes.height;
#endif
    screen->buf = buffer;

    Rterm_create_screen_draw_pixmap(screen);
    
    return;
}
#endif

void
Rterm_set_screen_tab(struct R_termscreen *screen, int mode)
{
    if (screen == NULL) {

	return;
    }

    switch (mode) {
        case RTERM_SCREEN_CLEAR_ALL_TABS:
	    memset(screen->tabs, 0, RTERM_SCREEN_MAX_COLUMNS);
	    break;
        case RTERM_SCREEN_CLEAR_TAB:
	    screen->tabs[(screen->column)] = 0;
	    break;
        case RTERM_SCREEN_SET_TAB:
	    screen->tabs[(screen->column)] = 1;
	    break;
        default:
	    break;
    }

    return;
}

void
Rterm_set_screen_tabs(struct R_termscreen *screen, int width)
{
    int i;

    Rterm_set_screen_tab(screen, RTERM_SCREEN_CLEAR_ALL_TABS);

    for (i = 0 ; i < RTERM_SCREEN_MAX_COLUMNS ; i += width) {
	screen->tabs[i] = '\1';
    }

    return;
}

void
Rterm_set_screen_scroll_region(struct R_termscreen *screen, int toprow, int bottomrow)
{
    if (screen == NULL
	|| toprow < 0
	|| bottomrow < 0) {

	return;
    }

    screen->scrolltop = MAX(toprow, 0);
    screen->scrollbottom = MIN(bottomrow, screen->rows - 1);

    return;
}

int
Rterm_init_screen(struct R_termscreen *screen, struct R_termscreen *copyfrom)
{
    struct R_term *term;
    int (*alloc_buffers)(struct R_termscreen *);

    if (screen == NULL) {
	
	return -1;
    }

    term = R_global.app->client;
    screen->inptr = screen->inbuf;
    screen->inend = screen->inbuf;

    if ((screen->outbuf = calloc(1, RTERM_OUTPUT_BUFFER_MIN_SIZE))
	== NULL) {

	return -1;
    }

    screen->outptr = screen->outbuf;
    screen->outend = screen->outbuf;
    screen->outsize = RTERM_OUTPUT_BUFFER_MIN_SIZE;

    if (copyfrom) {
        screen->rows = copyfrom->rows;
        screen->columns = copyfrom->columns;
	memcpy(screen->colors, copyfrom->colors, sizeof(copyfrom->colors));
	
	screen->gc = copyfrom->gc;
	screen->defaultgc = copyfrom->defaultgc;
	screen->cursorgc = copyfrom->cursorgc;
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
	screen->bufgc = copyfrom->bufgc;
#endif
#if (SUPPORT_TRUETYPE_FONTS)
#if (SUPPORT_TRUETYPE_IMLIB2)
	screen->stringbuffer = copyfrom->stringbuffer;
	screen->im2font = copyfrom->im2font;
        screen->im2textimg = copyfrom->im2textimg;
#else
        screen->font = copyfrom->font;
//	screen->ftfont = copyfrom->ftfont;
#endif
#endif
	screen->fontinfo = copyfrom->fontinfo;

	screen->charw = copyfrom->charw;
	screen->charh = copyfrom->charh;
	screen->charasc = copyfrom->charasc;
	screen->chardesc = copyfrom->chardesc;

	screen->funcs.alloc_buffers = copyfrom->funcs.alloc_buffers;
#if 0
	screen->funcs.destroy_buffers = copyfrom->funcs.destroy_buffers;
#endif
	screen->funcs.resize_buffers = copyfrom->funcs.resize_buffers;
	screen->funcs.add_text = copyfrom->funcs.add_text;
	screen->funcs.scroll_text = copyfrom->funcs.scroll_text;
	screen->funcs.insert_blank = copyfrom->funcs.insert_blank;
	screen->funcs.delete_string = copyfrom->funcs.delete_string;
	screen->funcs.blank_string = copyfrom->funcs.blank_string;
	screen->funcs.blank_drawn = copyfrom->funcs.blank_drawn;
	screen->funcs.refresh = copyfrom->funcs.refresh;
    } else {
        screen->rows = RTERM_SCREEN_DEFAULT_ROWS;
        screen->columns = RTERM_SCREEN_DEFAULT_COLUMNS;
        if (term->w) {
            screen->columns = term->w;
        }
        if (term->h) {
            screen->rows = term->h;
        }
	if (Rterm_init_screen_colors(screen) < 0) {
            fprintf(stderr, "Rterm_init_screen_colors failed\n");

	    return -1;
	}
        
	if (Rterm_init_screen_gcs(screen) < 0) {
            fprintf(stderr, "Rterm_init_screen_gcs failed\n");
            
	    return -1;
	}
        
#if (SUPPORT_TRUETYPE_FONTS)
#if (SUPPORT_TRUETYPE_IMLIB2)
	Rterm_load_screen_font_imlib2(screen, RTERM_DEFAULT_TRUETYPE_FONT);
	screen->stringbuffer = calloc(1,
				      (screen->columns + 1) * sizeof(R_text_t));
#else
        screen->font = Rterm_load_screen_font_freetype(screen,
                                                       RTERM_DEFAULT_TRUETYPE_FONT,
                                                       8);
        if (screen->font < 0) {
            
            return -1;
        }
#endif
#endif
        
	screen->funcs.alloc_buffers = Rterm_alloc_screen_textbuffers;
#if 0
	screen->funcs.destroy_buffers = Rterm_destroy_screen_textbuffers;
#endif
        screen->funcs.resize_buffers = Rterm_resize_screen_textbuffers;
        screen->funcs.add_text = Rterm_add_screen_text;
        screen->funcs.scroll_text = Rterm_scroll_screen_text;
        screen->funcs.insert_blank = Rterm_insert_blank_screen_string;
        screen->funcs.delete_string = Rterm_delete_screen_string;
        screen->funcs.blank_string = Rterm_blank_screen_string;
        screen->funcs.blank_drawn = Rterm_blank_screen_drawn_string;
        screen->funcs.refresh = Rterm_refresh_screen;
    }
    screen->textflags = RTERM_SCREEN_DEFAULT_TEXTFLAGS;
    if (screen->rows == 0) {
	screen->rows = RTERM_SCREEN_DEFAULT_ROWS;
    }

    if (screen->columns == 0) {
	screen->columns = RTERM_SCREEN_DEFAULT_COLUMNS;
    }

    if (screen->flags & RTERM_SCREEN_HAS_SCROLLBACK_BUFFER) {
	if (screen->savelines == 0) {
	    screen->savelines = RTERM_SCREEN_DEFAULT_SAVELINES;
	}
    }

    alloc_buffers = screen->funcs.alloc_buffers;
    if (alloc_buffers(screen) < 0) {

	return -1;
    }

    R_resize_window(screen->window, Rterm_screen_width(screen), Rterm_screen_height(screen));
    Rterm_load_screen_draw_image(screen);
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    if (term->flags & RTERM_DOUBLE_BUFFER) {
        Rterm_alloc_screen_draw_buffer(screen);
    }
#endif

    screen->viewrow = screen->savelines;
    screen->scrolltop = 0;
    screen->scrollbottom = screen->rows - 1;

    Rterm_set_screen_tabs(screen, RTERM_SCREEN_DEFAULT_TAB_SIZE);

    Rterm_set_screen_scroll_region(screen, 0, screen->rows - 1);

#if (SUPPORT_RTERM_BLINKING_CURSOR) || (SUPPORT_RTERM_BLINKING_CHARS)
    gettimeofday(&(term->blinktv), NULL);

    term->blink = FALSE;
#endif

#if 0
    screen->window->screen = screen;
#endif
    term->selection.firstrow = -1;
    term->selection.firstcolumn = -1;
    term->selection.lastrow = -1;
    term->selection.lastcolumn = -1;

    return 0;
}

/* TODO: this needs error handling */

#if (SUPPORT_XTERM_256_COLORS)
int
Rterm_init_screen_colors(struct R_termscreen *screen)
{
    struct R_term *term;
    int screennum, i;
    R_color_t tmpcolor;
    XColor color;

    if (screen == NULL) {

	return -1;
    }

    term = R_global.app->client;
    screennum = DefaultScreen(screen->window->app->display);

    if (screen->window->app->depth >= 8) {
        for (i = 0 ; i < RTERM_SCREEN_DEFAULT_COLORS ; i++) {
	    if (defaultcolornames[i]) {
		if (!XParseColor(screen->window->app->display,
				 screen->window->app->colormap,
				 defaultcolornames[i],
				 &color)) {

		    return -1;
		}
		if (!XAllocColor(screen->window->app->display,
				 screen->window->app->colormap,
				 &color)) {

		    return -1;
		}
		 
		screen->defcolors[i] = color.pixel;
            }
        }
        for (i = 0 ; i < RTERM_SCREEN_COLORS ; i++) {
            if (i < RTERM_ANSI_COLORS) {
                screen->colornames = ansicolornames;
            } else if (!screen->colornames) {
                screen->colornames = xtermcolornames;
            }
	    if (screen->colornames[i]) {
		if (!XParseColor(screen->window->app->display,
				 screen->window->app->colormap,
				 screen->colornames[i],
				 &color)) {

		    return -1;
		}
		if (!XAllocColor(screen->window->app->display,
				 screen->window->app->colormap,
				 &color)) {

		    return -1;
		}
		 
		screen->colors[i] = color.pixel;
#if (SUPPORT_TRUETYPE_FONTS)
#if (SUPPORT_TRUETYPE_IMLIB2)
		screen->im2colors[i].red = color.red >> 8;
		screen->im2colors[i].green = color.green >> 8;
		screen->im2colors[i].blue = color.blue >> 8;
		screen->im2colors[i].alpha = 255;
#endif
#endif
	    } else {
		if (!XParseColor(screen->window->app->display,
				 screen->window->app->colormap,
				 xtermcolornames[i],
				 &color)) {

		    return -1;
		}
		if (!XAllocColor(screen->window->app->display,
				 screen->window->app->colormap,
				 &color)) {

		    return -1;
		}

		screen->colors[i] = color.pixel;
#if (SUPPORT_TRUETYPE_FONTS)
#if (SUPPORT_TRUETYPE_IMLIB2)
		screen->im2colors[i].red = color.red >> 8;
		screen->im2colors[i].green = color.green >> 8;
		screen->im2colors[i].blue = color.blue >> 8;
		screen->im2colors[i].alpha = 255;
#endif
#endif
	    }
	}
    } else {
	/* fall back to black and white */

	screen->colors[RTERM_SCREEN_FOREGROUND_COLOR] = 
	    WhitePixel(screen->window->app->display,
		       RootWindow(screen->window->app->display, screennum));
	screen->colors[RTERM_SCREEN_BACKGROUND_COLOR] =
	    BlackPixel(screen->window->app->display,
		       RootWindow(screen->window->app->display, screennum));
    }
    if (term->fgcolor) {
        screen->colors[RTERM_SCREEN_FOREGROUND_COLOR] = term->fgcolor;
    }
    if (screen->flags & RTERM_SCREEN_REVERSE_VIDEO) {
	tmpcolor = screen->colors[RTERM_SCREEN_FOREGROUND_COLOR];
	screen->colors[RTERM_SCREEN_FOREGROUND_COLOR] = screen->colors[RTERM_SCREEN_BACKGROUND_COLOR];
	screen->colors[RTERM_SCREEN_BACKGROUND_COLOR] = tmpcolor;
    }

    return 0;
}
#else
int
Rterm_init_screen_colors(struct R_termscreen *screen)
{
    int screennum, i;
    R_color_t tmpcolor;
    XColor color;

    if (screen == NULL) {

	return -1;
    }

    screennum = DefaultScreen(screen->window->app->display);

    if (screen->window->app->depth >= 8) {
	for (i = 0 ; i < RTERM_SCREEN_COLORS ; i++) {
            if (!screen->colornames) {
                screen->colornames = defaultcolornames;
            }
	    if (screen->colornames[i]) {
		if (!XParseColor(screen->window->app->display,
				 screen->window->app->colormap,
				 screen->colornames[i],
				 &color)) {

		    return -1;
		}
		if (!XAllocColor(screen->window->app->display,
				 screen->window->app->colormap,
				 &color)) {

		    return -1;
		}
		 
		screen->colors[i] = color.pixel;
#if (SUPPORT_TRUETYPE_FONTS)
#if (SUPPORT_TRUETYPE_IMLIB2)
		screen->im2colors[i].red = color.red >> 8;
		screen->im2colors[i].green = color.green >> 8;
		screen->im2colors[i].blue = color.blue >> 8;
		screen->im2colors[i].alpha = 255;
#endif
#endif
	    } else {
		if (!XParseColor(screen->window->app->display,
				 screen->window->app->colormap,
				 defaultcolornames[i],
				 &color)) {

		    return -1;
		}
		if (!XAllocColor(screen->window->app->display,
				 screen->window->app->colormap,
				 &color)) {

		    return -1;
		}

		screen->colors[i] = color.pixel;
#if (SUPPORT_TRUETYPE_FONTS)
#if (SUPPORT_TRUETYPE_IMLIB2)
		screen->im2colors[i].red = color.red >> 8;
		screen->im2colors[i].green = color.green >> 8;
		screen->im2colors[i].blue = color.blue >> 8;
		screen->im2colors[i].alpha = 255;
#endif
#endif
	    }
	}
    } else {
	/* fall back to black and white */

	screen->colors[RTERM_SCREEN_FOREGROUND_COLOR] = 
	    WhitePixel(screen->window->app->display,
		       RootWindow(screen->window->app->display, screennum));
	screen->colors[RTERM_SCREEN_BACKGROUND_COLOR] =
	    BlackPixel(screen->window->app->display,
		       RootWindow(screen->window->app->display, screennum));
    }
    
    if (screen->flags & RTERM_SCREEN_REVERSE_VIDEO) {
	tmpcolor = screen->colors[RTERM_SCREEN_FOREGROUND_COLOR];
	screen->colors[RTERM_SCREEN_FOREGROUND_COLOR] = screen->colors[RTERM_SCREEN_BACKGROUND_COLOR];
	screen->colors[RTERM_SCREEN_BACKGROUND_COLOR] = tmpcolor;
    }

    return 0;
}
#endif

struct R_termscreen *
Rterm_add_screen(struct R_term *term, int x, int y, unsigned long flags)
{
    struct R_app *app;
    struct R_termscreen *newscreen;
    
    app = R_global.app;
    if ((newscreen = Rterm_alloc_screen()) == NULL) {

	return NULL;
    }
    newscreen->flags = flags;
#if (SUPPORT_RTERM_BLINKING_CURSOR)
    newscreen->flags |= RTERM_SCREEN_HAS_BLINKING_CURSOR;
#endif

#if 0
    if (term->screen) {
	newscreen->window = term->screen->window;
    } else if ((newscreen->window = R_create_window(app, term->window, 0))
	       == NULL) {
	free(newscreen);
	
	return NULL;
    } else {
	term->screen = newscreen;
#if 0
	newscreen->window->screen = newscreen;
#endif
    }
#endif
    newscreen->window = R_create_window(app, term->window, 0);
    R_add_window(newscreen->window);
    
    if (Rterm_init_screen(newscreen, term->screens[term->curscreen]) < 0) {
        fprintf(stderr, "Rterm_init_screen failed\n");
        free(newscreen);
	
        return NULL;
    }

#if 0    
    R_set_window_event_handler(newscreen->window,
                               Expose, Rterm_screen_expose);
    R_set_window_event_handler(newscreen->window,
                               ButtonPress, Rterm_screen_buttonpress);
    R_set_window_event_handler(newscreen->window,
                               ButtonRelease, Rterm_screen_buttonrelease);
    R_set_window_event_handler(newscreen->window,
                               FocusIn, Rterm_screen_focusin);
    R_set_window_event_handler(newscreen->window,
                               FocusOut, Rterm_screen_focusout);
    R_set_window_event_handler(newscreen->window,
                               MotionNotify, Rterm_screen_motionnotify);
    
    R_add_window_events(newscreen->window,
                        ExposureMask
                        | KeyPressMask
                        | ButtonPressMask
                        | ButtonReleaseMask
                        | Button1MotionMask
                        | PointerMotionHintMask
                        | OwnerGrabButtonMask);
#endif

#if 0	
    window_set_background_pixmap(newscreen->window, NULL);
#endif
#if 0
    R_map_window(newscreen->window);
#endif
    
    R_set_window_event_handler(newscreen->window,
                               Expose, Rterm_screen_expose);
#if 0
#endif
#if 0
    R_set_window_event_handler(newscreen->window,
                               FocusIn, Rterm_focusin);
    R_set_window_event_handler(newscreen->window,
                               FocusOut, Rterm_focusout);
#endif

#if 0
#endif

#if 0    
    R_set_window_button_action(newscreen->window,
                               R_BUTTON1_PRESS,
                               Rterm_start_selection);
    R_set_window_button_action(newscreen->window,
                               R_BUTTON1_RELEASE,
                               Rterm_finish_selection);
    R_set_window_button_action(newscreen->window,
                               R_BUTTON2_PRESS,
                               Rterm_request_selection);
    R_set_window_button_action(newscreen->window,
                               R_BUTTON3_PRESS,
                               Rterm_finish_selection);
#endif

    R_set_window_event_handler(newscreen->window,
                               KeyPress, Rterm_keypress);
    R_set_window_event_handler(newscreen->window,
                               ButtonPress, Rterm_screen_buttonpress);
    R_set_window_event_handler(newscreen->window,
                               ButtonRelease, Rterm_screen_buttonrelease);
    R_set_window_event_handler(newscreen->window,
                               MotionNotify, Rterm_screen_motionnotify);
    R_set_window_event_handler(newscreen->window,
                               SelectionNotify, Rterm_paste_selection);
    R_set_window_event_handler(newscreen->window,
                               SelectionClear, Rterm_destroy_selection);

    R_add_window_events(newscreen->window,
                        ButtonPressMask
                        | KeyPressMask
                        | ButtonReleaseMask
                        | Button1MotionMask
                        | PointerMotionHintMask
                        | OwnerGrabButtonMask
                        | ExposureMask);

#if 0
    newscreen->window->gc = newscreen->gc;
    newscreen->window->fontinfo = newscreen->fontinfo;
#endif
#if 0
    newscreen->window->screen = newscreen;
#endif

#if 0
    parent->flags |= WINDOW_HAS_SCREEN;
#endif

    return newscreen;
}

void
Rterm_screen_goto(struct R_termscreen *screen, int row, int column, int relative)
{
    struct R_term *term;

    if (screen == NULL
	|| screen->row < 0) {

	return;
    }

    screen->flags &= ~RTERM_SCREEN_WRAP_NEXT_CHAR;

    term = R_global.app->client;
#if 0
    if (!term->blink) {
        Rterm_clear_screen_cursor(screen);
    }
#endif
#if 0
    if (termptr->privmodes & RTERM_PRIVMODE_HOME_ON_ECHO) {
	termptr->window->screen->viewrow =
	    termptr->window->screen->savelines;
    }
#endif

    if (relative & RTERM_SCREEN_ROW_RELATIVE) {
	screen->row += row;
	screen->row = MAX(screen->row, screen->scrolltop);
	screen->row = MIN(screen->row, screen->scrollbottom);
    } else if (screen->flags & RTERM_SCREEN_RELATIVE_ORIGIN) {
	screen->row = screen->scrolltop + row;
	screen->row = MAX(screen->row, screen->scrolltop);
	screen->row = MIN(screen->row, screen->scrollbottom);
    } else {
	screen->row = row;
    }

    screen->row = MAX(screen->row, 0);
    screen->row = MIN(screen->row, screen->rows - 1);

    if (relative & RTERM_SCREEN_COLUMN_RELATIVE) {
	screen->column = MIN(screen->column + column, screen->columns - 1);
    } else {
	screen->column = MIN(column, screen->columns - 1);
    }

    screen->column = MAX(screen->column, 0);

//    Rterm_draw_screen_cursor(screen);

    return;
}

void
Rterm_screen_backspace(struct R_termscreen *screen)
{
    if (screen == NULL
	|| screen->row < 0) {

	return;
    }


    if (screen->column == 0 && screen->row > 0) {
	screen->column = screen->columns - 1;
	screen->row--;
    } else if (screen->flags & RTERM_SCREEN_WRAP_NEXT_CHAR) {
	screen->flags &= ~RTERM_SCREEN_WRAP_NEXT_CHAR;
    } else {
	Rterm_screen_goto(screen, 0, -1, RTERM_SCREEN_RELATIVE);
    }
#if 0
    if (screen->column == 0 && screen->row > 0) {
	screen->column = screen->columns - 1;
	screen->row--;
        screen->flags |= RTERM_SCREEN_WRAP_NEXT_CHAR;
    } else {
        Rterm_screen_goto(screen, 0, -1, RTERM_SCREEN_RELATIVE);
    }
#endif

    return;
}

void
Rterm_touch_screen(struct R_termscreen *screen)
{
    int row;
    int len;
    void (*blank_drawn)(struct R_termscreen *, int, int, int);
    void (*blank_string)(struct R_termscreen *, int, int, int);

    if (screen == NULL) {

	return;
    }
    blank_drawn = screen->funcs.blank_drawn;
    if (blank_drawn == NULL) {

	return;
    }
    blank_string = screen->funcs.blank_string;
    if (blank_string == NULL) {

	return;
    }

    Rterm_clear_screen_buffer(screen, 0, 0, 0, RTERM_SCREEN_CLEAR_BUFFER);

    for (row = 0 ; row < screen->rows ; row++) {
        len = screen->textbuf.rowcols[(screen->viewrow + row)] = screen->columns;
        if (screen->drawbuf.data[row]) {
            memset(screen->drawbuf.data[row], 0, screen->drawbuf.rowlens[row]);
#if 0
            blank_drawn(screen, row, 0, len);
//        len = screen->textbuf.rowlens[screen->viewrow + row];
//	blank_string(screen, row + screen->viewrow, 0, len);
//	screen->drawbuf.rowcols[row] = 0;
#endif
            screen->drawbuf.rowdirty[row] = 1;
        }
    }

    return;
}

int
Rterm_resize_screen(struct R_termscreen *screen, int rows, int columns)
{
    struct R_term *term;
    int (*resize_buffers)(struct R_termscreen *, int, int);

    if (screen == NULL
	|| rows <= 0
	|| columns <= 0
	|| (rows == screen->rows && columns == screen->columns)) {
	
	return -1;
    }
    resize_buffers = screen->funcs.resize_buffers;
    if (resize_buffers == NULL) {

	return -1;
    }

    if (resize_buffers(screen, rows, columns) < 0) {
	
	return -1;
    }

    term = R_global.app->client;

    screen->rows = rows;
    screen->columns = columns;

    Rterm_set_screen_scroll_region(screen, 0, screen->rows - 1);
    

    R_resize_window(screen->window, Rterm_screen_width(screen), Rterm_screen_height(screen));

#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    if (term->flags & RTERM_DOUBLE_BUFFER) {
        Rterm_alloc_screen_draw_buffer(screen);
        Rterm_clear_screen_buffer(screen, 0, 0, 0, RTERM_SCREEN_CLEAR_BUFFER);
    }
#endif

#if 0
    if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
        Rterm_resize_scrollbar(term,
                               RTERM_SCROLLBAR_DEFAULT_WIDTH,
                               Rterm_screen_height(screen));
        R_move_window(screen->window,
                      RTERM_SCROLLBAR_DEFAULT_WIDTH,
                      0);
    }
#endif
    screen->scrolltop = 0;
    screen->scrollbottom = rows - 1;
    Rterm_touch_screen(screen);
    Rterm_sync_screen(screen, 0, 0, 0, RTERM_SCREEN_SYNC);

    return 0;
}

void
Rterm_scroll_screen(struct R_termscreen *screen, int count)
{
    struct R_term *term;
    int oldviewrow;
    void (*refresh)(struct R_termscreen *, int);
    
    if (screen == NULL
	|| count == 0) {

	return;
    }
    term = R_global.app->client;

    refresh = screen->funcs.refresh;
    oldviewrow = screen->viewrow;

    screen->viewrow = MIN(screen->viewrow + count, screen->savelines);
    screen->viewrow = MAX(screen->viewrow, 0);
    screen->viewrow = MAX(screen->viewrow,
			  screen->savelines - screen->nsavelines);

    if (screen->viewrow != oldviewrow) {
//	if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
//	    Rterm_update_scrollbar(term);
//	} else {
	    Rterm_touch_screen(screen);

	    refresh(screen, RTERM_SCREEN_DRAW_ALL);
//	}
    }

    return;
}

/* FIXME: this has bugs. */
void
Rterm_screen_index(struct R_termscreen *screen, int count)
{
    int (*scroll_text)(struct R_termscreen *, int, int, int, int);
    if (screen == NULL
	|| screen->row < 0) {

	return;
    }
    scroll_text = screen->funcs.scroll_text;
    if (scroll_text == NULL) {

	return;
    }

#if 0
    if (termptr->privmodes & RTERM_PRIVMODE_HOME_ON_ECHO) {
	screen->viewrow = screen->savelines;
    }
#endif

    if (screen->flags & RTERM_SCREEN_WRAP_NEXT_CHAR) {
	screen->flags &= ~RTERM_SCREEN_WRAP_NEXT_CHAR;
    }

    switch (count) {
        case RTERM_SCREEN_INDEX_DOWN:
	    if (screen->row == screen->scrolltop) {
		scroll_text(screen, screen->scrolltop,
                            screen->scrollbottom,
                            count,
                            FALSE);
	    } else {
		screen->row--;
	    }
	    break;
        case RTERM_SCREEN_INDEX_UP:
	    if (screen->row == screen->scrollbottom) {
		scroll_text(screen, screen->scrolltop,
                            screen->scrollbottom,
                            count,
                            FALSE);
	    } else {
		screen->row++;
	    }
	    break;
        default:
	    break;
    }

    return;
}

/* TODO: implement this. */
void
Rterm_reset_screen(struct R_termscreen *screen)
{
    void (*refresh)(struct R_termscreen *, int);
    void (*blank_drawn)(struct R_termscreen *, int, int, int);
    void (*blank_string)(struct R_termscreen *, int, int, int);
    int row;

    if (screen == NULL) {

	return;
    }
    refresh = screen->funcs.refresh;
    blank_drawn = screen->funcs.blank_drawn;
    blank_string = screen->funcs.blank_string;

    screen->textflags = RTERM_SCREEN_DEFAULT_TEXTFLAGS;

    screen->scrolltop = 0;
    screen->scrollbottom = screen->rows - 1;

    for (row = 0 ; row < screen->rows - 1 ; row++) {
        blank_drawn(screen, row, 0, screen->columns);
        blank_string(screen, row, 0, screen->columns);
        screen->drawbuf.rowdirty[row] = 1;
    }
    refresh(screen, RTERM_SCREEN_DRAW_ALL);

    return;
}

void
Rterm_blank_screen_row(struct R_termscreen *screen, int mode)
{
    struct R_term *term;
    int row, column, len;
    void (*blank_string)(struct R_termscreen *, int, int, int);

    if (screen == NULL) {

	return;
    }
    blank_string = screen->funcs.blank_string;

    term = R_global.app->client;
    if (!term->blink) {
        Rterm_clear_screen_cursor(screen);
    }

    row = screen->savelines + screen->row;

    screen->flags &= ~RTERM_SCREEN_WRAP_NEXT_CHAR;

#if 0
    if (termptr->privmodes & RTERM_PRIVMODE_HOME_ON_ECHO) {
	termptr->window->screen->viewrow =
	    termptr->window->screen->savelines;
    }
#endif

    switch (mode) {
        case RTERM_SCREEN_ERASE_TO_END:
	    column = screen->column;
	    len = screen->columns - screen->column;
	    break;
        case RTERM_SCREEN_ERASE_TO_BEGINNING:
	    column = 0;
	    len = screen->column + 1;
	    break;
        case RTERM_SCREEN_ERASE_ALL:
	    column = 0;
	    len = screen->columns;
	    break;
        default:

	    return;
    }

    len = MIN(len, screen->textbuf.rowlens[row] - column);
    blank_string(screen, row - screen->savelines, column, len);
    screen->textbuf.rowcols[row] = column;

    screen->drawbuf.rowdirty[(row - screen->savelines)] = 1;

    screen->refreshed = FALSE;
    

    return;
}

void
Rterm_blank_screen(struct R_termscreen *screen, int mode)
{
    struct R_term *term;
    int row, rows;
    void (*blank_string)(struct R_termscreen *, int, int, int);

    if (screen == NULL
	|| screen->row < 0) {

	return;
    }
    blank_string = screen->funcs.blank_string;
    if (blank_string == NULL) {

	return;
    }

    switch (mode) {
        case RTERM_SCREEN_ERASE_TO_END:
	    Rterm_blank_screen_row(screen, RTERM_SCREEN_ERASE_TO_END);
	    row = screen->row + 1;
	    rows = screen->rows - row;
	    break;
        case RTERM_SCREEN_ERASE_TO_BEGINNING:
	    Rterm_blank_screen_row(screen, RTERM_SCREEN_ERASE_TO_BEGINNING);
	    row = 0;
	    rows = screen->row;
	    break;
        case RTERM_SCREEN_ERASE_ALL:
	    row = 0;
	    rows = screen->rows;
	    break;
        default:

	    return;
    }

    if ((row >= 0) && (row < screen->rows)) {
	rows = MIN(rows, screen->rows - row);

	for ( ; rows-- ; row++) {
	    blank_string(screen, screen->savelines + row, 0,
			 screen->textbuf.rowlens[(screen->savelines + row)]);
	    screen->textbuf.rowcols[(screen->savelines + row)] = 0;
            screen->drawbuf.rowdirty[row] = 1;
	}
    }

#if 0
    if (term->privmodes & RTERM_PRIVMODE_HOME_ON_REFRESH) {
	term->screen->viewrow = term->screen->savelines;
    }
#endif

//    Rterm_clear_screen_selection(screen);

    return;
}

#if 0 && defined(RTERM_PRINT_PIPE)

void
Rterm_print_screen(struct R_termscreen *screen, int mode)
{
    int i, j, rows, firstrow;
    FILE *fp;
    R_text_t *textptr;
    
    if (screen == NULL) {
	
	return;
    }
    
    if ((fp = popen_printer(NULL)) == NULL) {

	return;
    }
    
    if (mode == RTERM_SCREEN_PRINT_ALL_ROWS) {
	rows = screen->nsavelines + screen->rows;
	firstrow = screen->savelines - screen->nsavelines;
    } else {
	rows = screen->rows;
        firstrow = screen->viewrow;
    }
    
    for (i = 0 ; i < rows ; i++) {
	textptr = screen->textbuf.data[firstrow + i];
	
	for (j = screen->columns - 1 ; j >= 0 ; j--) {
	    if (!isspace(textptr[j])) {
		break;
	    }
	}

	fprintf(fp, "%.*s\n", j + 1, textptr);
    }

    pclose_printer(fp);
    
    return;
}

#endif /* defined(RTERM_PRINT_PIPE) */

void
Rterm_screen_tab(struct R_termscreen *screen, int tabs)
{
    struct R_term *term;

    int column, tabcolumn;

    if (screen == NULL
	|| tabs == 0) {

	return;
    }

    term = R_global.app->client;
    if (!term->blink) {
        Rterm_clear_screen_cursor(screen);
    }

    tabcolumn = screen->column;

    if (tabs > 0) {
	for (column = screen->column + 1 ;
             column < screen->columns ;
	     column++) {
	    if (screen->tabs[column]) {
		tabcolumn = column;
		
		if (--tabs == 0) {
		    break;
		}
	    }
	}
    } else {
	/* tabs < 0 */
	for (column = screen->column - 1 ; column >= 0 ; column--) {
	    if (screen->tabs[column]) {
		tabcolumn = column;

		if (++tabs == 0) {
		    break;
		}
	    }
	}
    }

    if (tabcolumn != screen->column) {
	Rterm_screen_goto(screen, 0, tabcolumn, RTERM_SCREEN_ROW_RELATIVE);
    }

    return;
}

void
Rterm_set_screen_foreground_color(struct R_termscreen *screen, int color)
{
    if (screen == NULL
	|| color < 0
	|| color > RTERM_SCREEN_COLORS - 1
	|| screen->window->app->depth < 8) {

	return;
    }

    XSetForeground(screen->window->app->display, screen->gc,
		   screen->colors[color]);

    return;
}

void
Rterm_set_screen_background_color(struct R_termscreen *screen, int color)
{
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    struct R_term *term = R_global.app->client;
#endif
    if (screen == NULL
	|| color < 0
	|| color > RTERM_SCREEN_COLORS - 1
	|| screen->window->app->depth < 8) {
	
	return;
    }

    XSetBackground(screen->window->app->display, screen->gc,
		   screen->colors[color]);
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    if (term->flags & RTERM_DOUBLE_BUFFER) {
        XSetForeground(screen->window->app->display, screen->bufgc,
                       screen->colors[color]);
    }
#endif

    return;
}

void
Rterm_set_screen_reverse_video(struct R_termscreen *screen)
{
    int row, column;
    void (*refresh)(struct R_termscreen *, int);

    if (screen == NULL) {

	return;
    }
    refresh = screen->funcs.refresh;
    if (refresh == NULL) {

	return;
    }

    if (screen->flags & RTERM_SCREEN_REVERSE_VIDEO) {
	for (row = screen->savelines ;
	     row < screen->savelines + screen->rows ;
	     row++) {
	    for (column = 0 ;
		 column < screen->textbuf.rowcols[row] ;
		 column++) {
		screen->textbuf.renddata[row][column] |= RTERM_CHAR_REVERSE;
	    }
#if 0
	    for (column = 0 ;
		 column < screen->textbuf.rowlens[row] ;
		 column++) {
		screen->textbuf.renddata[row][column] |= RTERM_CHAR_REVERSE;
	    }
#endif
	}

	screen->textflags |= RTERM_CHAR_REVERSE;
    } else {
	for (row = screen->savelines ;
	     row < screen->savelines + screen->rows ;
	     row++) {
	    for (column = 0 ;
		 column < screen->textbuf.rowcols[row] ;
		 column++) {
		screen->textbuf.renddata[row][column] &= ~RTERM_CHAR_REVERSE;
	    }
#if 0
	    for (column = 0 ;
		 column < screen->textbuf.rowlens[row] ;
		 column++) {
		screen->textbuf.renddata[row][column] &= ~RTERM_CHAR_REVERSE;
	    }
#endif
	}

	screen->textflags &= ~RTERM_CHAR_REVERSE;
    }

    refresh(screen, RTERM_SCREEN_DRAW_ALL);

    return;
}

int
Rterm_screen_column_x(struct R_termscreen *screen, int column)
{
    if (screen == NULL
	|| column < 0
	|| column > screen->columns - 1) {

	return -1;
    }

    return (column * screen->charw);
}

int
Rterm_screen_row_y(struct R_termscreen *screen, int row)
{
    if (screen == NULL
	|| row < 0
	|| row > screen->rows - 1) {

	return -1;
    }

    return (row * screen->charh);
}

int
Rterm_screen_x_column(struct R_termscreen *screen, int x)
{
    if (screen == NULL
	|| x < 0) {

	return -1;
    }

    return (x / screen->charw);
}

int
Rterm_screen_y_row(struct R_termscreen *screen, int y)
{
    if (screen == NULL
	|| y < 0) {

	return -1;
    }

    return (y / screen->charh);
}

int
Rterm_screen_width(struct R_termscreen *screen)
{
    if (screen == NULL) {
	
	return -1;
    }
    
    return (screen->charw * screen->columns);
}

int
Rterm_screen_height(struct R_termscreen *screen)
{
    if (screen == NULL) {
	
	return -1;
    }
    
    return (screen->charh * screen->rows);
}

int
Rterm_screen_cursor_x(struct R_termscreen *screen)
{
    if (screen == NULL) {

	return -1;
    }

    return (screen->charw * screen->cursorcolumn);
}

int
Rterm_screen_cursor_y(struct R_termscreen *screen)
{
    if (screen == NULL
	|| screen->row < 0) {

	return -1;
    }

    return (screen->charh * screen->cursorrow);
}

void
Rterm_insert_blank_screen_rows(struct R_termscreen *screen, int count)
{
    int (*scroll_text)(struct R_termscreen *, int, int, int, int);
    if (screen == NULL
	|| count <= 0) {
	
	return;
    }
    scroll_text = screen->funcs.scroll_text;
    if (scroll_text == NULL) {

	return;
    }

    screen->flags &= ~RTERM_SCREEN_WRAP_NEXT_CHAR;

#if 0
    if (termptr->privmodes & RTERM_PRIVMODE_HOME_ON_ECHO) {
	termptr->window->screen->viewrow =
	    termptr->window->screen->savelines;
    }
#endif

    count = MIN(count, screen->scrollbottom - screen->row + 1);

    scroll_text(screen, screen->row, screen->scrollbottom,
                -count,
                FALSE);

    screen->refreshed = FALSE;

    return;
}

void
Rterm_delete_screen_rows(struct R_termscreen *screen, int count)
{
    int (*scroll_text)(struct R_termscreen *, int, int, int, int);
    if (screen == NULL
	|| count <= 0) {

	return;
    }
    scroll_text = screen->funcs.scroll_text;
    if (scroll_text == NULL) {

	return;
    }

    screen->flags &= ~RTERM_SCREEN_WRAP_NEXT_CHAR;

#if 0
    if (termptr->privmodes & RTERM_PRIVMODE_HOME_ON_ECHO) {
	termptr->window->screen->viewrow =
	    termptr->window->screen->savelines;
    }
#endif

    count = MIN(count, screen->scrollbottom - screen->row + 1);

    scroll_text(screen, screen->row, screen->scrollbottom,
                count,
                FALSE);

    screen->refreshed = FALSE;

    return;
}

void
Rterm_clear_screen_string(struct R_termscreen *screen, int row, int column, int len)
{
    if (screen == NULL
	|| row < 0
	|| row > screen->rows - 1
	|| column < 0
	|| column > screen->columns - 1
	|| len <= 0) {

	return;
    }

    len = MIN(len, screen->columns - column);

    if (screen->buf) {
        Rterm_clear_screen_buffer(screen, row, column, len);
        Rterm_sync_screen(screen, row, column, len, RTERM_SCREEN_SYNC_STRING);
    } else {
        R_clear_area(screen->window,
                     Rterm_screen_column_x(screen, column),
                     Rterm_screen_row_y(screen, row),
                     screen->charw * len, screen->charh,
                     False);
    }

    screen->refreshed = FALSE;

    return;
}

int
Rterm_new_screen(struct R_term *term, int id)
{
    struct R_termscreen *screen;
    
    if ((screen = Rterm_add_screen(term,
                                   ((term->privmodes & RTERM_PRIVMODE_SCROLLBAR)
                                    && !(term->flags & RTERM_SCROLLBAR_RIGHT)
                                    ? RTERM_SCROLLBAR_DEFAULT_WIDTH
                                    : 0),
                                   ((term->privmodes
                                     & RTERM_PRIVMODE_MENUBAR)
                                    ? RTERM_MENU_DEFAULT_HEIGHT
                                    : 0),
                                   RTERM_SCREEN_HAS_SCROLLBACK_BUFFER))
        == NULL) {
        fprintf(stderr, "failed initializing screen %x\n", id);
        
        return -1;
    }
#if (SUPPORT_RTERM_TABS)
    screen->id = id;
#endif
//    screen->rows = term->screens[term->curscreen]->rows;
//    screen->columns = term->screens[term->curscreen]->columns;
    
//	if (i < screens) {
    if (id & 0x01) {
        screen->type = RTERM_SECONDARY_SCREEN;
    } else {
        screen->type = RTERM_PRIMARY_SCREEN;
    }
    
#if 0        
    if (term->flags & RTERM_TRANSPARENT) {
        screen->window->images[R_WINDOW_BACKGROUND_IMAGE].flags
            |= IMAGE_TRANSPARENT;
    }
#endif
    R_resize_window(screen->window,
                    Rterm_screen_width(screen),
                    Rterm_screen_height(screen));
    Rterm_load_screen_draw_image(screen);
#if (SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER)
    if (term->flags & RTERM_DOUBLE_BUFFER) {
        Rterm_alloc_screen_draw_buffer(screen);
    }
#endif
    R_move_window(screen->window,
                  (term->privmodes & RTERM_PRIVMODE_SCROLLBAR)
                  ? RTERM_SCROLLBAR_DEFAULT_WIDTH
                  : 0,
                  (term->privmodes & RTERM_PRIVMODE_MENUBAR) ? RTERM_MENU_DEFAULT_HEIGHT : 0);
    term->screens[id] = screen;
 
    return screen;
}

int
Rterm_init_screens(struct R_term *term, int screens)
{
    struct R_termscreen *screen;
    int i;

    fprintf(stderr, "initializing %d screens\n", screens);
    term->curscreen = 0;
    term->screens = calloc(screens, sizeof(struct R_termscreen *));
    for (i = 0 ; i < screens ; i++) {
        if (!Rterm_new_screen(term, i)) {

            return -1;
        }
    }
    term->nscreens = screens;

    return 0;
}

void
Rterm_switch_screen(struct R_term *term, struct R_termscreen *screen, int which)
{
    struct R_termscreen *curscreen, *oldscreen, *destscreen;
    void (*refresh)(struct R_termscreen *, int);
    int id = 0;

    oldscreen = screen;
#if 0
    oldscreen->active = FALSE;
#endif
#if (SUPPORT_RTERM_TABS)
    if (which == RTERM_PRIMARY_SCREEN) {
        if (screen->id & 0x01) {
            screen->flags &= ~RTERM_SCREEN_ACTIVE;
            for (id = 0 ; id < term->nscreens - 1 ; id++) {
                if (term->screens[id] == screen) {
                    id--;

                    break;
                }
            }
            screen->flags |= RTERM_SCREEN_ACTIVE;
        }
    } else if (!(screen->id & 0x01)) {
        screen->flags &= ~RTERM_SCREEN_ACTIVE;
        for (id = 1 ; id < term->nscreens - 1 ; id++) {
            if (term->screens[id] == screen) {
                id++;
                
                break;
            }
        }
        screen->flags |= RTERM_SCREEN_ACTIVE;
    }
#endif
    destscreen = term->screens[id];
//    term->screen = term->screens[which];
#if (SUPPORT_TRUETYPE_FONTS)
    term->window->font = destscreen->font;
#endif

    refresh = destscreen->funcs.refresh;
    if (refresh == NULL) {

	return;
    }

#if 0
    if (oldscreen->flags & RTERM_SCREEN_HAS_FOCUS) {
#if 0
	XSetInputFocus(term->app->display,
		       destscreen->window->win,
		       RevertToPointerRoot,
		       CurrentTime);
#endif

	oldscreen->flags &= ~RTERM_SCREEN_HAS_FOCUS;

	destscreen->flags |= RTERM_SCREEN_HAS_FOCUS;
    } else {
	destscreen->flags &= ~RTERM_SCREEN_HAS_FOCUS;
    }
#endif

    if (destscreen->rows != oldscreen->rows
	|| destscreen->columns != oldscreen->columns
	|| destscreen->fontinfo != oldscreen->fontinfo) {
	destscreen->fontinfo = oldscreen->fontinfo;

	Rterm_resize_screen(destscreen, oldscreen->rows, oldscreen->columns);
    }

//    destscreen->active = TRUE;
    destscreen->textflags = RTERM_SCREEN_DEFAULT_TEXTFLAGS;
    destscreen->gctextflags = oldscreen->gctextflags;

#if 0    
    window_set_background_pixmap(destscreen->window, NULL);
#endif
    R_render_background_imlib2(destscreen->window->images[R_WINDOW_BACKGROUND_IMAGE],
                               destscreen->window,
                               0);

#if (SUPPORT_RTERM_TABS)
    if (oldscreen->id == term->curscreen) {
        R_unmap_window(screen->window);
        term->curscreen = screen->id;
        R_map_window(screen->window);
        Rterm_clear_screen_buffer(screen, 0, 0, RTERM_SCREEN_CLEAR_BUFFER);
        Rterm_touch_screen(screen);
        Rterm_sync_screen(screen, 0, 0, 0, RTERM_SCREEN_SYNC);
    }
#endif

//    if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
#if 0
//	scrollbar_set_screen(term->scrollbar, destscreen);
#endif
//	Rterm_update_scrollbar(term);
//    } else {
//	refresh(destscreen, RTERM_SCREEN_DRAW_ALL);
//    }

    return;
}

void
Rterm_set_screen(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    if (term->privmodes & RTERM_PRIVMODE_SECONDARY_SCREEN) {
        Rterm_switch_screen(term, screen, RTERM_PRIMARY_SCREEN);
    } else {
	Rterm_switch_screen(term, screen, RTERM_SECONDARY_SCREEN);
    }

    return;
}

void
Rterm_start_selection(void *arg, XEvent *event, void *dummy)
{
    struct R_window *window;
    struct R_term *term;
    struct R_termscreen *screen;
    int row, column;
    int winx, winy, dummyi;
    unsigned int dummyui;
    Window rootwin;
    Window pointerwin;
    Window dummyw;

    window = arg;
    term = R_global.app->client;
    screen = term->screens[term->curscreen];
//    Rterm_clear_screen_selection(screen);
    term->selection.screen = screen;
    if (term->selection.screen != screen) {
        term->selection.screen = screen;
    }
    XQueryPointer(window->app->display,
                  window->id,
                  &dummyw, &dummyw,
                  &dummyi, &dummyi,
                  &winx, &winy,
                  &dummyui);
    row = Rterm_screen_y_row(screen, winy) + screen->viewrow;
    column = Rterm_screen_x_column(screen, winx);
#if 0
    row = Rterm_screen_y_row(screen, event->xbutton.y) + screen->viewrow;
    column = Rterm_screen_x_column(screen, event->xbutton.x);
#endif
    term->selection.baserow = row;
    term->selection.basecolumn = column;
#if 0
    term->selection.firstrow = row;
    term->selection.firstcolumn = column;
#endif
    Rterm_change_selection(term, row, column);
    R_set_selection(term->window, event->xbutton.time);

    return;
}

void
Rterm_finish_selection(void *arg, XEvent *event, void *dummy)
{
    struct R_window *window;
    struct R_term *term;
    struct R_termscreen *screen;
    int row, column;
    int winx, winy, dummyi;
    unsigned int dummyui;
    Window rootwin;
    Window pointerwin;
    Window dummyw;

    term = R_global.app->client;
    screen = term->selection.screen;
    if (term->selection.firstrow < 1
        || term->selection.firstcolumn < 1
        || term->selection.lastrow < 1
        || term->selection.lastcolumn < 1) {
        
        return;
    }
    window = arg;
    XQueryPointer(window->app->display,
                  window->id,
                  &dummyw, &dummyw,
                  &dummyi, &dummyi,
                  &winx, &winy,
                  &dummyui);
    row = Rterm_screen_y_row(screen, winy) + screen->viewrow;
    column = Rterm_screen_x_column(screen, winx);
#if 0
    row = Rterm_screen_y_row(screen, event->xbutton.y) + screen->viewrow;
    column = Rterm_screen_x_column(screen, event->xbutton.x);
#endif
//            screen_change_selection(screen, row, column);
    Rterm_change_selection(term, row, column);
    R_set_selection(term->window, event->xbutton.time);
    Rterm_highlight_selection(term);

    fprintf(stderr,  "SELECTION: %d, %d -> %d, %d on %p\n",
            term->selection.firstrow,
            term->selection.firstcolumn,
            term->selection.lastrow,
            term->selection.lastcolumn,
            term);
    
    return;
}

void
Rterm_request_selection(void *arg, XEvent *event, void *dummy)
{
    struct R_window *window;
    struct R_app *app;
    Window win;
    Atom prop;

    window = arg;
    app = R_global.app;
    win = R_get_selection_owner(window, XA_PRIMARY);
    if (win) {
        prop = XInternAtom(window->app->display, "RTERM_SELECTION", False);
        R_convert_selection(window, app->window->id,
                            XA_PRIMARY, XA_STRING,
                            prop, event->xbutton.time);
//        XFlush(window->app->display);
        XSync(window->app->display, False);
    }
}

void
Rterm_paste_selection(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;
    struct R_termscreen *screen;
    Atom prop;
    int ok;
    int nb;
    long nread;
    unsigned long loop;
    unsigned long nleft;
    XTextProperty textprop;
    const unsigned char *cp;
    const unsigned char cr = '\r';

    window = arg;
    term = R_global.app->client;
    screen = term->screens[term->curscreen];
    nread = 0;
    loop = 1;
    prop = XInternAtom(window->app->display, "RTERM_SELECTION", False);
    while (loop) {
        if (XGetWindowProperty(window->app->display,
                               term->window->id,
                               prop,
                               nread >> 2,
                               4096,
                               True,
                               AnyPropertyType,
                               &textprop.encoding,
                               &textprop.format,
                               &textprop.nitems,
                               &loop,
                               &textprop.value) != Success) {
            XFree(textprop.value);
            
            return;
        }
        if (!textprop.value) {
            
            continue;
        }
        cp = textprop.value;
        nleft = textprop.nitems;
        nb = 0;
        while (nleft) {
            if (*cp != '\n') {
                nb++;
            } else {
                RTERM_WRITE_PTY(term, screen->pty.masterfd, cp, nb);
                RTERM_WRITE_PTY(term, screen->pty.masterfd, &cr, 1);
                cp += nb + 1;
                nb = 0;
            }
            nleft--;
        }
        if (nb) {
            RTERM_WRITE_PTY(term, screen->pty.masterfd, cp, nb);
        }
        XFree(textprop.value);
        nread += textprop.nitems;
    }

    return;
}

void
Rterm_make_selection(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;
    struct R_termscreen *screen;
    XEvent notifyevent;
    Window win;

    window = arg;
    term = R_global.app->client;
    screen = term->selection.screen;
    if ((win = R_get_selection_owner(window,
                                     event->xselectionrequest.selection))
        != window->id) {

        return;
    }

    if (Rterm_prepare_selection(term, R_STRING_SELECTION) < 0) {

        return;
    }

    XChangeProperty(window->app->display,
                    event->xselectionrequest.requestor,
                    event->xselectionrequest.property,
                    XA_STRING,
                    8,
                    PropModeReplace,
                    (unsigned char *)term->selection.stringdata,
                    term->selection.stringlen);

    notifyevent.xselection.type = SelectionNotify;
    notifyevent.xselection.display = window->app->display;
    notifyevent.xselection.property = event->xselectionrequest.property;
    notifyevent.xselection.requestor = event->xselectionrequest.requestor;
    notifyevent.xselection.selection = event->xselectionrequest.selection;
    notifyevent.xselection.target = event->xselectionrequest.target;
    notifyevent.xselection.time = event->xselectionrequest.time;
    XSendEvent(window->app->display,
               event->xselectionrequest.requestor,
               0,
               0,
               &notifyevent);
//    XFlush(window->app->display);
    XSync(window->app->display, False);

    return;
}

void
Rterm_destroy_selection(void *arg, XEvent *event)
{
    struct R_window *window;
    struct R_term *term;

    window = arg;
    term = R_global.app->client;
    Rterm_free_screen_selection(term->selection.screen);

    return;
}

