/*
 * pixmap.c - pixmap manipulation for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#if 0
void
window_apply_pixmap(Rwindow_t *window, Rimage_t *image,
		    int x, int y,
		    unsigned long flags)
{
    int screen;
    int pixmapx, pixmapy;
    int pixmapw, pixmaph;
    int winw, winh;
    int dummyi;
    unsigned int dummyui;
    Window dummyw;
    GC gc;
    XGCValues gcvalues;

    if (window == NULL
	|| image == NULL) {

	return;
    }

    screen = DefaultScreen(window->app->display);
    if ((flags & WINDOW_IMAGE_SHAPE)
	&& (image->maskpixmap)
	&& (XQueryExtension(window->app->display,
			   "SHAPE",
			   &dummyi, &dummyi, &dummyi))) {
	XSetWindowBackgroundPixmap(window->app->display,
				   window->win,
				   image->pixmap);
	XShapeCombineMask(window->app->display,
			  window->win,
			  ShapeBounding,
			  x, y,
			  image->maskpixmap,
			  ShapeSet);
	XClearWindow(window->app->display,
		     window->win);
    } else if ((flags & WINDOW_IMAGE_MASK)
	       && (image->maskpixmap)) {
	XGetGeometry(window->app->display,
		     window->win,
		     &dummyw,
		     &dummyi, &dummyi,
		     &winw, &winh,
		     &dummyui, &dummyui);
	XGetGeometry(window->app->display,
		     image->pixmap,
		     &dummyw,
		     &dummyi, &dummyi,
		     &pixmapw, &pixmaph,
		     &dummyui, &dummyui);
	memset(&gcvalues, 0, sizeof(gcvalues));
	gcvalues.foreground = WhitePixel(window->app->display, screen);
	gcvalues.background = BlackPixel(window->app->display, screen);
	gc = XCreateGC(window->app->display,
		       window->win,
		       GCForeground | GCBackground,
		       &gcvalues);
	XSetClipMask(window->app->display, gc, image->maskpixmap);
	if (flags & WINDOW_IMAGE_TILE) {
	    for (pixmapx = x ; pixmapx < winw ; pixmapx += pixmapw) {
		for (pixmapy = y ; pixmapy < winh ; pixmapy += pixmaph) {
		    XSetClipOrigin(window->app->display,
				   gc,
				   pixmapx, pixmapy);
		    XCopyArea(window->app->display,
			      image->pixmap,
			      window->win,
			      gc,
			      0, 0,
			      pixmapw, pixmaph,
			      pixmapx, pixmapy);
		}
	    }
	} else {
	    XSetClipOrigin(window->app->display,
			   gc,
			   x, y);
	    XCopyArea(window->app->display,
		      image->pixmap,
		      window->win,
		      gc,
		      0, 0,
		      pixmapw, pixmaph,
		      x, y);
	}
	XFreeGC(window->app->display, gc);
    } else if (image->pixmap) {
	XGetGeometry(window->app->display,
		     image->pixmap,
		     &dummyw,
		     &dummyi, &dummyi,
		     &pixmapw, &pixmaph,
		     &dummyui, &dummyui);
	memset(&gcvalues, 0, sizeof(gcvalues));
	gcvalues.foreground = WhitePixel(window->app->display, screen);
	gcvalues.background = BlackPixel(window->app->display, screen);
	gc = XCreateGC(window->app->display,
		       window->win,
		       GCForeground | GCBackground,
		       &gcvalues);
	XCopyArea(window->app->display,
		  image->pixmap,
		  window->win,
		  gc,
		  0, 0,
		  pixmapw, pixmaph,
		  x, y);
	XFreeGC(window->app->display, gc);
    }
    XFlush(window->app->display);
    
    return;
}

void
window_set_background_pixmap(Rwindow_t *window, Rimage_t *image)
{
    if (window == NULL
	|| image == NULL) {

	return;
    }

    XSetWindowBackgroundPixmap(window->app->display,
			       window->win,
			       image->pixmap);
    XClearWindow(window->app->display,
		 window->win);
    XFlush(window->app->display);

    return;
}

void
window_set_transparent_background_pixmap(Rwindow_t *window)
{
    unsigned long imageflags;
    int x, y, dummyi;
    unsigned int winw, winh, pixmapw, pixmaph, dummyui;
    Rimage_t *image;
    Window dummywin;
    GC gc;
    XGCValues gcvalues;
    Screen *screenptr;
    
    if (window == NULL) {
	
	return;
    }

    if ((app_get_desktop_window(window->app) == NULL)
	|| (app_get_desktop_pixmap(window->app) == None)) {
	
	return;
    }

    image = &(window->images[WINDOW_BACKGROUND_IMAGE]);
    
    screenptr = ScreenOfDisplay(window->app->display,
				DefaultScreen(window->app->display));

    if (image->flags & IMAGE_TRANSPARENT) {
	image->flags &= ~IMAGE_KEEP_PIXMAP;
	image_destroy_data_imlib2(image);

	XTranslateCoordinates(window->app->display,
			      window->win,
			      window->app->desktopwindow->win,
			      0, 0,
			      &x, &y, &dummywin);

	XGetGeometry(window->app->display, window->win, &dummywin,
		     &dummyi, &dummyi, &winw, &winh, &dummyui, &dummyui);

	image->pixmap = XCreatePixmap(window->app->display, window->win,
				      winw, winh, window->app->depth);

	XGetGeometry(window->app->display, window->app->desktoppixmap,
		     &dummywin, &dummyi, &dummyi, &pixmapw, &pixmaph,
		     &dummyui, &dummyui);

	/* Some window managers (Enlightenment 0.16.4 in particular) may
	 * screw the background pixmap property up. :(
	 */

	if (pixmapw == 0 || pixmaph == 0) {
	    image_destroy_imlib2(image);

	    return;
	}

	gc = XCreateGC(window->app->display, window->win, FLAGS_NONE,
		       &gcvalues);

	if (pixmapw < WidthOfScreen(screenptr)
	    || pixmaph < HeightOfScreen(screenptr)) {
	    XSetTile(window->app->display, gc, window->app->desktoppixmap);
	    XSetTSOrigin(window->app->display, gc,
			 pixmapw - (x % pixmapw), pixmaph - (y % pixmaph));
	    XSetFillStyle(window->app->display, gc, FillTiled);
	    XFillRectangle(window->app->display, image->pixmap, gc,
			   0, 0, winw, winh);
	} else {
	    XCopyArea(window->app->display, window->app->desktoppixmap,
		      image->pixmap,
		      gc, x, y, winw, winh, 0, 0);
	}

	XFreeGC(window->app->display, gc);

#if (!(USE_IMLIB2))
	app_shade_pixmap(window->app, image);
#endif

	if (window->flags & WINDOW_DOUBLE_BUFFER) {
	    image->flags |= IMAGE_KEEP_PIXMAP;
	} else {
	    window_set_background_pixmap(window, image);
	}

	image_destroy_data_imlib2(image);
    }
    
    return;
}

Pixmap
app_get_desktop_pixmap(Rapp_t *app)
{
    int screen, format;
    unsigned int tilew, tileh;
    unsigned long nitems = 0, nleft = 0;
    unsigned char *data;
    Rcolor_t color;
    Pixmap pixmap = None;
    Atom rootpmapatom = None, rootpixelatom = None, typeatom = None;
    Screen *screenptr;
    GC gc;
    XGCValues gcvalues;

    if (app == NULL) {

	return None;
    }

    if (app_get_desktop_window(app) == NULL) {
	if (app->flags & APP_DESKTOP_PIXMAP_IS_MINE) {
	    if (app->desktoppixmap) {
		XFreePixmap(app->display, app->desktoppixmap);
		
		app->desktoppixmap = None;
	    }
	}

	return None;
    }

    screen = DefaultScreen(app->display);
    screenptr = ScreenOfDisplay(app->display, screen);

    rootpmapatom = XInternAtom(app->display, ROOT_PIXMAP_ATOM_STRING, True);
    rootpixelatom = XInternAtom(app->display, ROOT_PIXEL_ATOM_STRING, True);

    if (rootpmapatom == None && rootpixelatom == None) {
	if (app->flags & APP_DESKTOP_PIXMAP_IS_MINE) {
	    if (app->desktoppixmap) {
		XFreePixmap(app->display, app->desktoppixmap);
		
		app->desktoppixmap = None;
	    }
	}

	return None;
    }

    if (rootpmapatom != None) {
	XGetWindowProperty(app->display, app->desktopwindow->win, rootpmapatom,
			   0, 1, False, AnyPropertyType, &typeatom, &format,
			   &nitems, &nleft, &data);

	if (typeatom == XA_PIXMAP) {
	    pixmap = *((Pixmap *)data);

	    XFree(data);

	    if (pixmap != None) {
		if (pixmap == app->desktoppixmap) {
		    /* desktop pixmap hasn't changed. */

		    return ((Pixmap)1);
		}
		
		/* desktop pixmap has changed. */

		if (app->flags & APP_DESKTOP_PIXMAP_IS_MINE) {
		    if (app->desktoppixmap) {
			XFreePixmap(app->display, app->desktoppixmap);
		    }
		}

		app->desktopcolor = None;

		app->desktoppixmap = pixmap;

		return pixmap;
	    }
	}
    } else {
	/* rootpixelatom != None */

	XGetWindowProperty(app->display, app->desktopwindow->win, rootpixelatom,
			   0, 1, False, AnyPropertyType, &typeatom, &format,
			   &nitems, &nleft, &data);

	if (typeatom == XA_CARDINAL) {
	    color = *((Rcolor_t *)data);

	    if (color == app->desktopcolor) {
		/* background color hasn't changed. */

		return ((Pixmap)1);
	    }

	    /* background color has changed. */
	    
	    gcvalues.foreground = color;
	    gcvalues.background = color;

	    gc = XCreateGC(app->display, app->window->win,
			   GCForeground | GCBackground, &gcvalues);

	    XQueryBestSize(app->display, TileShape, app->window->win, 16, 16,
			   &tilew, &tileh);

	    pixmap = XCreatePixmap(app->display, app->window->win,
				   tilew, tileh, app->depth);

	    XFillRectangle(app->display, pixmap, gc, 0, 0, tilew, tileh);

	    if (app->flags & APP_DESKTOP_PIXMAP_IS_MINE) {
		if (app->desktoppixmap) {
		    XFreePixmap(app->display, app->desktoppixmap);
		}
	    }

	    app->flags |= APP_DESKTOP_PIXMAP_IS_MINE;

	    app->desktopcolor = color;

	    app->desktoppixmap = pixmap;

	    return pixmap;
	}
    }

    if (app->flags & APP_DESKTOP_PIXMAP_IS_MINE) {
	if (app->desktoppixmap) {
	    XFreePixmap(app->display, app->desktoppixmap);

	    app->desktoppixmap = None;
	}
    }

    return None;
}

#if (!(USE_IMLIB2))
Pixmap
app_render_viewport_pixmap(Rapp_t *app, Rimage_t *image)
{
    int screen;
    unsigned int pixmapw, pixmaph;
    Pixmap tmppixmap;
    Screen *screenptr;

    screen = DefaultScreen(app->display);
    screenptr = ScreenOfDisplay(app->display, screen);
    
    if (image->w == 0) {
	pixmapw = image->img->rgb_width;
    } else if (image->w < 0) {
	pixmapw = WidthOfScreen(screenptr);
    } else {
	pixmapw = image->w;
    }
    
    if (image->h == 0) {
	pixmaph = image->img->rgb_height;
    } else if (image->h < 0) {
	pixmaph = HeightOfScreen(screenptr);
    } else {
	pixmaph = image->h;
    }

    if (!Imlib_render(app->imlibid, image->img, pixmapw, pixmaph)) {

	return None;
    }

    app->viewportpixmap = Imlib_move_image(app->imlibid, image->img);

    tmppixmap = Imlib_move_mask(app->imlibid, image->img);

    if (tmppixmap) {
	Imlib_free_pixmap(app->imlibid, tmppixmap);
    }

    return (app->viewportpixmap);
}
#endif /* !USE_IMLIB2 */
#endif /* 0 */

