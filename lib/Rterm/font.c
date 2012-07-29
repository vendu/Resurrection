/*
 * font.c - font routines for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

/* TODO: rename R_load_font() to app_load_and_query_font() ? */

#define RTERM_USE_XSHM_FONT_BITMAPS 0

#if (SUPPORT_TRUETYPE_FONTS)
Pixmap
R_render_bitmap_freetype(struct R_app *app, GC gc, unsigned char *data,
                         int boxw, int boxh, int w, int h,
                         int top, int descent,
                         int pitch,
                         struct ftfont *font,
                         int i);
#endif

int
Rterm_load_screen_font(struct R_termscreen *screen, const char *fontname)
{
#if (SUPPORT_RTERM_UTF8_CHARS)
    struct R_term *term = R_global.app->client;
#endif
    if (screen == NULL) {

	return -1;
    }

#if (SUPPORT_RTERM_UTF8_CHARS)
    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
        screen->fontinfo = R_load_font(screen->window->app,
                                       "-misc-freemono-medium-r-normal--0-0-0-0-p-0-iso10646-1");
        if (screen->fontinfo == NULL) {
            
            return -1;
        }
    } else
#endif
    if (term->fontname == NULL) {
	screen->fontinfo = R_load_font(screen->window->app,
                                       RTERM_DEFAULT_FONTNAME);
	if (screen->fontinfo == NULL) {
	    
	    return -1;
	}
    } else {
	screen->fontinfo = R_load_font(screen->window->app, fontname);
	if (screen->fontinfo == NULL) {
	    
	    return -1;
	}
    }

    screen->charw = X_FONT_WIDTH(screen->fontinfo);
    screen->charh = X_FONT_HEIGHT(screen->fontinfo);
    screen->charasc = screen->fontinfo->ascent;
    screen->chardesc = screen->fontinfo->descent;

    return 0;
}

#if 0
int
imageview_load_font(Rimageview_t *imageview, const char *fontname)
{
    if (imageview == NULL) {

	return -1;
    }

    if (fontname == NULL) {
	imageview->fontinfo = R_load_font(imageview->window->app,
                                          IMAGEVIEW_DEFAULT_FONT);
	if (imageview->fontinfo == NULL) {
	    
	    return -1;
	}
    } else {
	imageview->fontinfo = R_load_font(imageview->window->app, fontname);
	if (imageview->fontinfo == NULL) {
	    
	    return -1;
	}
    }

    imageview->charw = X_FONT_WIDTH(imageview->fontinfo);
    imageview->charh = X_FONT_HEIGHT(imageview->fontinfo);
    imageview->charasc = imageview->fontinfo->ascent;
    imageview->chardesc = imageview->fontinfo->descent;

    return 0;
}
#endif

#if (SUPPORT_TRUETYPE_FONTS)

ftfont_t *
R_load_font_freetype(struct R_app *app, const char *fontname, int size)
{
#if (SUPPORT_RTERM_UTF8_CHARS)
    struct R_term *term = R_global.app->client;
#if (RTERM_USE_XSHM)
#if (SUPPORT_XSHM_FONT_BITMAP)
    XShmSegmentInfo *shmseg;
    int shmid;
    uint8_t *shmaddr;
#endif
#endif
#endif
    ftfont_t *newfont;
    unsigned char *data;
    GC gc;
    Pixmap pixmap, bitmap;
    FT_Error error;
    FT_UInt glyphndx;
    FT_Glyph glyph;
    FT_BitmapGlyph bitmapglyph;
    FT_BBox bbox;
    int i, j;
    int charw, charh;
    int w, h;
    int top, descent;
    int pitch;
    XGCValues gcvalues;
    unsigned char filename[PATH_MAX + 1];

    snprintf(filename, sizeof(filename), "%sttf/%s.ttf",
	     RESURRECTION_FONT_SEARCH_PATH, fontname);
    newfont = calloc(1, sizeof(ftfont_t));
    error = FT_New_Face(app->ftlib, filename, 0, &newfont->face);
    if (error) {
	free(newfont);

	return NULL;
    }
#if (SUPPORT_RTERM_UTF8_CHARS) && (SUPPORT_SHM_FONT_BITMAP)
    shmseg = = &newfont->shminfo;
#endif
    error = FT_Set_Char_Size(newfont->face,
			     size * 64, size * 64, 72, 72);
    if (error) {
	error = FT_Set_Pixel_Sizes(newfont->face, size, size);
    }
    if (error) {
	free(newfont);

	return NULL;
    }
    charw = newfont->face->size->metrics.max_advance / 64;
    charh = newfont->face->size->metrics.height / 64;
    descent = -newfont->face->size->metrics.descender / 64;
#if (SUPPORT_TRUETYPE_ANTIALIAS)
    bitmap = XCreatePixmap(app->display,
			   app->window->id,
			   1, 1,
			   DefaultDepth(app->display,
                                        DefaultScreen(app->display)));
#else
    bitmap = XCreatePixmap(app->display,
			   app->window->id,
			   1, 1,
			   1);
#endif
    memset(&gcvalues, 0, sizeof(gcvalues));
    gcvalues.foreground = 1;
    gcvalues.function = GXcopy;
    gcvalues.graphics_exposures = False;
    gc = XCreateGC(app->display,
		   bitmap,
		   GCForeground | GCFunction | GCGraphicsExposures,
		   &gcvalues);
    XFreePixmap(app->display, bitmap);
#if (SUPPORT_RTERM_UTF8_CHARS)
    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
        j = 65536;
    } else {
        j = 256;
    }
#else
    j = 256;
#endif

#if (SUPPORT_RTERM_UTF8_CHARS) && (RTERM_USE_XSHM) && (SUPPORT_XSHM_FONT_BITMAP)
    bitmap = None;
    shmid = shmget(IPC_PRIVATE,
                   j * charw * charh * 4,
                   0777 | IPC_CREAT);
    if (shmid >= 0) {
        shmseg->shmaddr = shmat(shmid, NULL, 0);
        if (shmseg->shmaddr == ((void *)-1)) {
            fprintf(stderr, "shmat() failed\n");
            shmid = -1;
        } else {
            shmseg->shmid = shmid;
            shmseg->readOnly = False;
            XShmAttach(app->display, shmseg);
        }
    }
#endif
    for (i = 1 ; i < j ; i++) {
#if (SUPPORT_RTERM_UTF8_CHARS) && (RTERM_USE_XSHM) && (SUPPORT_XSHM_FONT_BITMAP)
        newfont->shmaddrs[i] = shmaddr;
        shmaddr += charw * charh * 4;
#endif
	glyphndx = FT_Get_Char_Index(newfont->face, i);
#if (SUPPORT_TRUETYPE_ANTIALIAS)
	error = FT_Load_Glyph(newfont->face, glyphndx, FT_LOAD_RENDER);
#else
	error = FT_Load_Glyph(newfont->face, glyphndx, FT_LOAD_MONOCHROME);
#endif
	if (!error) {
#if (!SUPPORT_TRUETYPE_ANTIALIAS)
	    if (newfont->face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
		error = FT_Render_Glyph(newfont->face->glyph,
					FT_RENDER_MODE_MONO);
	    }
#endif
	    if (!error) {
		error = FT_Get_Glyph(newfont->face->glyph, &glyph);
		if (!error) {
		    bitmapglyph = (FT_BitmapGlyph)glyph;
		    w = bitmapglyph->bitmap.width;
		    h = bitmapglyph->bitmap.rows;
		    top = bitmapglyph->top;
		    data = bitmapglyph->bitmap.buffer;
		    pitch = bitmapglyph->bitmap.pitch;
		    if ((w) && (h)) {
			bitmap = R_render_bitmap_freetype(app, gc, data,
                                                          charw, charh,
                                                          w, h,
                                                          top, descent,
                                                          pitch,
                                                          newfont,
                                                          i);
			newfont->bitmaps[i] = bitmap;
#if 0
			newfont->widths[i] = w;
			newfont->heights[i] = h;
#endif
		    }
		    FT_Done_Glyph(glyph);
		}
	    }
	}
    }
    XFreeGC(app->display, gc);
#if 0
    newfont->ascent = ascent;
#endif
    newfont->ascent = 0;
    newfont->w = charw;
    newfont->h = charh;

    return newfont;
}

#if 0 && (SUPPORT_TRUETYPE_ANTIALIAS)
Pixmap
R_render_bitmap_freetype(struct R_app *app, GC gc, unsigned char *data,
                         int boxw, int boxh, int w, int h,
                         int top, int descent,
                         int pitch,
                         struct ftfont *font,
                         int i)
{
    unsigned char *dataptr;
    Pixmap bitmap;
    int x, y;
    int xoffset, yoffset;
    int gray;
    unsigned char uc;
    unsigned int pixel;

    XSetForeground(app->display, gc, 0);
    XFillRectangle(app->display,
		   bitmap,
		   gc,
		   0, 0,
		   boxw, boxh);
//    XSetForeground(app->display, gc, 1);
    dataptr = data;
    xoffset = (boxw - w) / 2;
    yoffset = boxh - top - descent;
    for (y = 0 ; y < h ; y++) {
	for (x = 0 ; x < w ; x++) {
	    gray = *dataptr;
	    if (gray) {
                pixel = (gray << 16) | (gray << 8) | gray;
                XSetForeground(app->display, gc, pixel);
                XDrawPoint(app->display, bitmap, gc, x + xoffset, y + yoffset);
	    }
            dataptr++;
	}
//	dataptr += pitch;
    }

    return bitmap;
}
#else
Pixmap
R_render_bitmap_freetype(struct R_app *app, GC gc, unsigned char *data,
                         int boxw, int boxh, int w, int h,
                         int top, int descent,
                         int pitch,
                         struct ftfont *font,
                         int i)
{
    unsigned char *dataptr;
    Pixmap bitmap;
    int x, y;
    int xoffset, yoffset;
    int bit;
    unsigned char uc;
#if (SUPPORT_RTERM_UTF8_CHARS) && (RTERM_USE_XSHM) && (SUPPORT_XSHM_FONT_BITMAP)
    void *shmaddr = font->shmaddrs[i];
    XShmSegmentInfo *shmseg;
#elif (RTERM_USE_XSHM) && (RTERM_USE_XSHM_FONT_BITMAPS)
    XShmSegmentInfo *shmseg;
    int shmid;
#endif

#if !((SUPPORT_RTERM_UTF8_CHARS) && (SUPPORT_XSHM_FONT_BITMAP)) && (RTERM_USE_XSHM) && (RTERM_USE_XSHM_FONT_BITMAPS)
    bitmap = None;
    shmid = shmget(IPC_PRIVATE,
                   boxw * boxh * 4,
                   0777 | IPC_CREAT);
    if (shmid >= 0) {
        shmseg->shmaddr = shmat(shmid, NULL, 0);
        if (shmseg->shmaddr == ((void *)-1)) {
            fprintf(stderr, "shmat() failed\n");
            shmid = -1;
        } else {
            shmseg->shmid = shmid;
            shmseg->readOnly = False;
            XShmAttach(app->display, shmseg);
            bitmap = XShmCreatePixmap(app->display,
                                      app->window->id,
                                      shmseg->shmaddr,
                                      shmseg,
                                      boxw,
                                      boxh,
                                      1);
        }
    }
#elif (RTERM_USE_XSHM) && (SUPPORT_XSHM_FONT_BITMAP)
    bitmap = XShmCreatePixmap(app->display,
                              app->window->id,
                              shmaddr,
                              shmseg,
                              boxw,
                              boxh,
                              1);
    if (!bitmap) {
        bitmap = XCreatePixmap(app->display,
                               app->window->id,
                               boxw, boxh,
                               1);
    }
#else
    bitmap = XCreatePixmap(app->display,
			   app->window->id,
			   boxw, boxh,
			   1);
#endif
    XSetForeground(app->display, gc, 0);
    XFillRectangle(app->display,
		   bitmap,
		   gc,
		   0, 0,
		   boxw, boxh);
    XSetForeground(app->display, gc, 1);
    dataptr = data;
    xoffset = (boxw - w) / 2;
    yoffset = boxh - top - descent;
    for (y = 0 ; y < h ; y++) {
	for (x = 0 ; x < w ; x++) {
	    bit = dataptr[(x >> 3)] & (0x80 >> (x & 0x07));
	    if (bit) {
		XDrawPoint(app->display, bitmap, gc, x + xoffset, y + yoffset);
	    }
	}
	dataptr += pitch;
    }

    return bitmap;
}
#endif

int
Rterm_load_screen_font_freetype(struct R_termscreen *screen, const char *fontname,
                                int size)
{
    if (screen == NULL) {

	return -1;
    }

    if (fontname == NULL) {
	screen->ftfont = R_load_font_freetype(screen->window->app,
                                              RTERM_DEFAULT_TRUETYPE_FONT,
                                              size);
	if (screen->ftfont == NULL) {
	    
	    return -1;
	}
    } else {
	screen->ftfont = R_load_font_freetype(screen->window->app,
                                              fontname,
                                              size);
	if (screen->ftfont == NULL) {
	    
	    return -1;
	}
    }

    screen->charasc = screen->ftfont->ascent;
    screen->charw = screen->ftfont->w;
    screen->charh = screen->ftfont->h;

    return 0;
}
#endif /* SUPPORT_TRUETYPE_FONTS */

#if (SUPPORT_TRUETYPE_IMLIB2)
Imlib_Font
R_load_font_imlib2(struct R_app *app, const char *fontname, int size)
{
    Imlib_Font newfont;
    unsigned char imlibname[1024];

    snprintf(imlibname, sizeof(imlibname), "%s/%d", fontname, size);
    newfont = imlib_load_font(imlibname);

    return newfont;
}

int
Rterm_load_screen_font_imlib2(struct R_termscreen *screen, const char *fontname,
                              int size)
{
    if (screen == NULL) {

	return -1;
    }

    if (fontname == NULL) {
	screen->im2font = R_load_font_imlib2(screen->window->app,
                                             SCREEN_DEFAULT_TRUETYPE_FONT,
                                             size);
	if (screen->im2font == NULL) {
	    
	    return -1;
	}
    } else {
	screen->im2font = R_load_font_imlib2(screen->window->app,
                                             fontname,
                                             size);
	if (screen->im2font == NULL) {
	    
	    return -1;
	}
    }

    imlib_context_set_font(screen->im2font);
    imlib_get_text_size(" ", &screen->charw, &screen->charh);

    return 0;
}

#endif /* SUPPORT_TRUETYPE_IMLIB2 */

