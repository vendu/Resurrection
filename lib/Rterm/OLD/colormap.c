/*
 * colormap.c - colormap routines for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#if 0
/* FIXME: debugging, make app_create_rgb_colormaps really grab the server... */

static const char *visualclassnames[VISUAL_CLASSES] =
{
    "StaticGray",
    "GrayScale",
    "StaticColor",
    "PseudoColor",
    "TrueColor",
    "DirectColor"
};

void
app_create_rgb_colormaps(Rapp_t *app)
{
    int screen, dummyi;
    Display *display, *origdisplay;
    XStandardColormap *standardcolormap;

    if (app == NULL) {

	return;
    }

    screen = DefaultScreen(app->display);
    origdisplay = app->display;

    display = XOpenDisplay(app->displayname);
    if (display) {
	app->display = display;

#if 0
	XGrabServer(display);
#endif
	
	if (XGetRGBColormaps(app->display,
			     RootWindow(app->display, screen),
			     &standardcolormap,
			     &dummyi,
			     XA_RGB_DEFAULT_MAP) == False) {
	    standardcolormap = app_create_rgb_colormap(app,
						       XA_RGB_DEFAULT_MAP);
	    if (standardcolormap) {
		app_set_rgb_colormap(app, standardcolormap,
				     XA_RGB_DEFAULT_MAP);

		XFree(standardcolormap);
	    }
	} else if (!app_rgb_colormap_values_valid(app, standardcolormap,
						  XA_RGB_DEFAULT_MAP)) {
	    switch (standardcolormap->killid) {
	        case 0:
		    break;
	        case ReleaseByFreeingColormap:
		    XFreeColormap(app->display, standardcolormap->colormap);
		    break;
	        default:
		    XKillClient(app->display, standardcolormap->killid);
		    break;
	    }
	    
	    XFree(standardcolormap);

	    standardcolormap = app_create_rgb_colormap(app,
						       XA_RGB_DEFAULT_MAP);
	    if (standardcolormap) {
		app_set_rgb_colormap(app, standardcolormap,
				     XA_RGB_DEFAULT_MAP);

		XFree(standardcolormap);
	    }
	}

	if (XGetRGBColormaps(app->display,
			     RootWindow(app->display, screen),
			     &standardcolormap,
			     &dummyi,
			     XA_RGB_BEST_MAP) == False) {
	    standardcolormap = app_create_rgb_colormap(app,
						       XA_RGB_BEST_MAP);
	    if (standardcolormap) {
		app_set_rgb_colormap(app, standardcolormap,
				     XA_RGB_BEST_MAP);

		XFree(standardcolormap);
	    }
	} else if (!app_rgb_colormap_values_valid(app, standardcolormap,
						  XA_RGB_BEST_MAP)) {
	    switch (standardcolormap->killid) {
	        case 0:

		    break;
	        case ReleaseByFreeingColormap:
		    XFreeColormap(app->display, standardcolormap->colormap);

		    break;
	        default:
		    XKillClient(app->display, standardcolormap->killid);

		    break;
	    }
	    
	    XFree(standardcolormap);

	    standardcolormap = app_create_rgb_colormap(app,
						       XA_RGB_BEST_MAP);
	    if (standardcolormap) {
		app_set_rgb_colormap(app, standardcolormap,
				     XA_RGB_BEST_MAP);
		
		XFree(standardcolormap);
	    }
	}

	if (XGetRGBColormaps(app->display,
			     RootWindow(app->display, screen),
			     &standardcolormap,
			     &dummyi,
			     XA_RGB_GRAY_MAP) == False) {
	    standardcolormap = app_create_rgb_colormap(app,
						       XA_RGB_GRAY_MAP);
	    if (standardcolormap) {
		app_set_rgb_colormap(app, standardcolormap,
				     XA_RGB_GRAY_MAP);

		XFree(standardcolormap);
	    }
	} else if (!app_rgb_colormap_values_valid(app, standardcolormap,
						  XA_RGB_GRAY_MAP)) {
	    switch (standardcolormap->killid) {
	        case 0:
		    break;
	        case ReleaseByFreeingColormap:
		    XFreeColormap(app->display, standardcolormap->colormap);

		    break;
	        default:
		    XKillClient(app->display, standardcolormap->killid);

		    break;
	    }
	    
	    XFree(standardcolormap);

	    standardcolormap = app_create_rgb_colormap(app,
						       XA_RGB_GRAY_MAP);
	    if (standardcolormap) {
		app_set_rgb_colormap(app, standardcolormap,
				     XA_RGB_GRAY_MAP);
		
		XFree(standardcolormap);
	    }
	}

	if (XGetRGBColormaps(app->display,
			     RootWindow(app->display, screen),
			     &standardcolormap,
			     &dummyi,
			     XA_RGB_RED_MAP) == False) {
	    standardcolormap = app_create_rgb_colormap(app,
						       XA_RGB_RED_MAP);
	    if (standardcolormap) {
		app_set_rgb_colormap(app, standardcolormap,
				     XA_RGB_RED_MAP);

		XFree(standardcolormap);
	    }
	} else if (!app_rgb_colormap_values_valid(app, standardcolormap,
						  XA_RGB_RED_MAP)) {
	    switch (standardcolormap->killid) {
	        case 0:

		    break;
	        case ReleaseByFreeingColormap:
		    XFreeColormap(app->display, standardcolormap->colormap);

		    break;
	        default:
		    XKillClient(app->display, standardcolormap->killid);

		    break;
	    }
	    
	    XFree(standardcolormap);

	    standardcolormap = app_create_rgb_colormap(app,
						       XA_RGB_RED_MAP);
	    if (standardcolormap) {
		app_set_rgb_colormap(app, standardcolormap,
				     XA_RGB_RED_MAP);
		
		XFree(standardcolormap);
	    }
	}

	if (XGetRGBColormaps(app->display,
			     RootWindow(app->display, screen),
			     &standardcolormap,
			     &dummyi,
			     XA_RGB_GREEN_MAP) == False) {
	    standardcolormap = app_create_rgb_colormap(app,
						       XA_RGB_GREEN_MAP);
	    if (standardcolormap) {
		app_set_rgb_colormap(app, standardcolormap,
				     XA_RGB_GREEN_MAP);

		XFree(standardcolormap);
	    }
	} else if (!app_rgb_colormap_values_valid(app, standardcolormap,
						  XA_RGB_GREEN_MAP)) {
	    switch (standardcolormap->killid) {
	        case 0:

		    break;
	        case ReleaseByFreeingColormap:
		    XFreeColormap(app->display, standardcolormap->colormap);

		    break;
	        default:
		    XKillClient(app->display, standardcolormap->killid);

		    break;
	    }
	    
	    XFree(standardcolormap);

	    standardcolormap = app_create_rgb_colormap(app,
						       XA_RGB_GREEN_MAP);
	    if (standardcolormap) {
		app_set_rgb_colormap(app, standardcolormap,
				     XA_RGB_GREEN_MAP);

		XFree(standardcolormap);
	    }
	}

	if (XGetRGBColormaps(app->display,
			     RootWindow(app->display, screen),
			     &standardcolormap,
			     &dummyi,
			     XA_RGB_BLUE_MAP) == False) {
	    standardcolormap = app_create_rgb_colormap(app,
						       XA_RGB_BLUE_MAP);
	    if (standardcolormap) {
		app_set_rgb_colormap(app, standardcolormap,
				     XA_RGB_BLUE_MAP);

		XFree(standardcolormap);
	    }
	} else if (!app_rgb_colormap_values_valid(app, standardcolormap,
						  XA_RGB_BLUE_MAP)) {
	    switch (standardcolormap->killid) {
	        case 0:

		    break;
	        case ReleaseByFreeingColormap:
		    XFreeColormap(app->display, standardcolormap->colormap);

		    break;
	        default:
		    XKillClient(app->display, standardcolormap->killid);

		    break;
	    }
	    
	    XFree(standardcolormap);

	    standardcolormap = app_create_rgb_colormap(app,
						       XA_RGB_BLUE_MAP);
	    if (standardcolormap) {
		app_set_rgb_colormap(app, standardcolormap,
				     XA_RGB_BLUE_MAP);

		XFree(standardcolormap);
	    }
	}

	XSetCloseDownMode(display, RetainPermanent);

#if 0
	XUngrabServer(display);
#endif

	XCloseDisplay(display);
	
	app->display = origdisplay;
    }

    return;
}

XStandardColormap *
app_create_rgb_colormap(Rapp_t *app, Atom property)
{
    int screen, visuals, i;
    int defaultcolormap;
    XVisualInfo visualinfo;
    XVisualInfo *visualinfoptr1, *visualinfoptr2, *visualinfoptr3;
    XStandardColormap *newmap, *retval;
    
    if (app == NULL) {
	
	return NULL;
    }
    
    screen = DefaultScreen(app->display);
    visualinfo.screen = screen;
    
    visualinfoptr1 = XGetVisualInfo(app->display,
				    VisualScreenMask,
				    &visualinfo,
				    &visuals);
    if (visualinfoptr1 == NULL) {
	
	return NULL;
    }
    
    visualinfoptr2 = app_get_best_visual(app, visualinfoptr1, visuals,
					 property);
    if (visualinfoptr2 == NULL) {
	XFree(visualinfoptr1);

	return NULL;
    }
    
    newmap = XAllocStandardColormap();
    if (newmap == NULL) {
	XFree(visualinfoptr1);

	free(visualinfoptr2);
	
	return NULL;
    }
    
    newmap->colormap = XCreateColormap(app->display,
				       RootWindow(app->display, screen),
				       visualinfoptr2->visual,
				       AllocNone);

    if (app_set_rgb_allocation(app, newmap, visualinfoptr2, property) < 0) {
	XFree(visualinfoptr1);

	free(visualinfoptr2);
	
	XFreeColormap(app->display, newmap->colormap);

	XFree(newmap);

	return NULL;
    }
    
    defaultcolormap = False;
    
    for (screen = 0 ; screen < ScreenCount(app->display) ; screen++) {
	if (newmap->colormap == DefaultColormap(app->display, screen)) {
	    defaultcolormap = True;
	    
	    break;
	}
    }
    
    retval = NULL;

    visualinfoptr3 = visualinfoptr1;
    
    if (defaultcolormap) {
	for (i = 0 ; i < visuals ; i++, visualinfoptr3++) {
	    if (visualinfoptr3->visual == DefaultVisual(app->display,
							DefaultScreen(app->display))							
		&& visualinfoptr3->screen == DefaultScreen(app->display)) {
		newmap->visualid = visualinfoptr3->visualid;

		break;
	    }
	}

	if (newmap->visualid == 0) {
	    newmap->visualid = visualinfoptr2->visualid;
	}
	
	retval = newmap;
    } else {
	if (visualinfoptr2->class == GrayScale
	    || visualinfoptr2->class == PseudoColor
	    || visualinfoptr2->class == DirectColor) {
	    if (app_alloc_read_write_rgb_colormap(app,
						  newmap,
						  visualinfoptr2) < 0) {

		retval = NULL;
	    } else {

		retval = newmap;
	    }
	} else if (visualinfoptr2->class == TrueColor) {
	    if (COLORMAP_VALUES_MATCH(newmap, visualinfoptr2)) {

		retval = newmap;
	    } else {

		retval = NULL;
	    }
	} else {
	    if (app_alloc_read_only_rgb_colormap(app,
						 newmap,
						 visualinfoptr2) < 0) {

		retval = NULL;
	    } else {

		retval = newmap;
	    }
	}

	newmap->visualid = visualinfoptr2->visualid;
    }

    if (retval == NULL) {
	XFree(visualinfoptr1);

	free(visualinfoptr2);
	
	XFreeColormap(app->display, newmap->colormap);

	XFree(newmap);

	return NULL;
    } else {
	if (defaultcolormap) {
	    newmap->killid = (XID)XCreatePixmap(app->display,
						DefaultRootWindow(app->display),
						1, 1, app->depth);
	} else {
	    newmap->killid = ReleaseByFreeingColormap;
	}
    }	
    
    XFree(visualinfoptr1);

    free(visualinfoptr2);
    
    return retval;
}

XVisualInfo *
app_get_best_visual(Rapp_t *app, XVisualInfo *visualinfo, int visuals,
		    Atom property)
{
    Visual *visualptr;
    XVisualInfo *visualinfoptr1, *visualinfoptr2, *visualinfoptr3;
    XVisualInfo *visualinfoptr4;
    XVisualInfo *retval, *newvisualinfo;

    if (app == NULL
	|| visualinfo == NULL
	|| visuals <= 0) {

	return NULL;
    }

    visualinfoptr1 = app_get_deepest_visual(app, visualinfo, visuals,
					    PseudoColor);

    visualinfoptr2 = app_get_deepest_visual(app, visualinfo, visuals,
					    DirectColor);

    retval = NULL;

    if ((visualinfoptr2)
	&& (!(visualinfoptr1)
	    || (visualinfoptr2->colormap_size
		>= ((visualinfoptr1->red_mask
		     | visualinfoptr1->green_mask
		     | visualinfoptr1->blue_mask)
		    + 1)))) {

	retval = visualinfoptr2;
    } else {

	retval = visualinfoptr1;
    }

    if (property == XA_RGB_BEST_MAP) {
	if (!(retval) || retval == visualinfoptr1) {
	    visualinfoptr3 = app_get_deepest_visual(app, visualinfo,
						    visuals, TrueColor);
	    visualinfoptr4 = app_get_deepest_visual(app, visualinfo,
						    visuals, StaticColor);
	    if ((visualinfoptr3) || (visualinfoptr4)) {

		retval = ((visualinfoptr3)
			  ? visualinfoptr3
			  : visualinfoptr4);
	    }
	}
    } else if (property == XA_RGB_GRAY_MAP) {
	if (!(retval)) {
	    visualinfoptr3 = app_get_deepest_visual(app, visualinfo,
						    visuals, GrayScale);
	    visualinfoptr4 = app_get_deepest_visual(app, visualinfo,
						    visuals, StaticGray);

	    if ((visualinfoptr3) || (visualinfoptr4)) {

		retval = ((visualinfoptr3)
			  ? visualinfoptr3
			  : visualinfoptr4);
	    }
	}
    } else if (property == XA_RGB_DEFAULT_MAP) {
	visualptr = DefaultVisual(app->display,
				  DefaultScreen(app->display));

	visualinfoptr3 = visualinfo;
	while (visualinfoptr3->visual != visualptr
	       || visualinfoptr3->screen != DefaultScreen(app->display)) {
	    visualinfoptr3++;
	}
	
	retval = visualinfoptr3;
    }

    newvisualinfo = calloc(1, sizeof(XVisualInfo));
    if (newvisualinfo == NULL) {

	return NULL;
    }

    if (retval) {
	/* structure copy. */

	*newvisualinfo = *retval;
    } else {
	free(newvisualinfo);

	return NULL;
    }

    return newvisualinfo;
}

XVisualInfo *
app_get_deepest_visual(Rapp_t *app, XVisualInfo *visualinfo, int visuals,
		       int class)
{
    int maxdepth, i;
    XVisualInfo *retval;

    if (app == NULL
	|| visualinfo == NULL
	|| class < 0
	|| class > VISUAL_CLASSES - 1
	|| visuals <= 0) {

	return NULL;
    }

    maxdepth = 0;

    retval = NULL;

    for (i = 0 ; i < visuals ; i++, visualinfo++) {
	if (visualinfo->class == class
	    && visualinfo->screen == DefaultScreen(app->display)
	    && visualinfo->depth > maxdepth) {
	    maxdepth = visualinfo->depth;
	    
	    retval = visualinfo;
	}
    }

    return retval;
}

int
app_set_rgb_allocation(Rapp_t *app,
		       XStandardColormap *standardcolormap,
		       XVisualInfo *visualinfo,
		       Atom property)
{
    int retval;

    if (app == NULL
	|| standardcolormap == NULL
	|| visualinfo == NULL) {

	return -1;
    }

    switch (property) {
        case XA_RGB_DEFAULT_MAP:
	    retval = app_set_rgb_default_allocation(app,
						    standardcolormap,
						    visualinfo,
						    property);

	    break;
        case XA_RGB_BEST_MAP:
	    retval = app_set_rgb_best_allocation(app,
						 standardcolormap,
						 visualinfo,
						 property);

	    break;
        case XA_RGB_GRAY_MAP:
	    retval = app_set_rgb_grayscale_allocation(app,
						      standardcolormap,
						      visualinfo,
						      property);

	    break;
        case XA_RGB_RED_MAP:
	    standardcolormap->red_max = visualinfo->colormap_size - 1;
	    standardcolormap->green_max = 0;
	    standardcolormap->blue_max = 0;
	    
	    retval = 0;

	    break;
        case XA_RGB_GREEN_MAP:
	    standardcolormap->red_max = 0;
	    standardcolormap->green_max = visualinfo->colormap_size - 1;
	    standardcolormap->blue_max = 0;

	    retval = 0;

	    break;
        case XA_RGB_BLUE_MAP:
	    standardcolormap->red_max = 0;
	    standardcolormap->green_max = 0;
	    standardcolormap->blue_max = visualinfo->colormap_size - 1;

	    retval = 0;

	    break;
        default:

	    retval = -1;

	    break;
    }

    return retval;
}

int
app_set_rgb_default_allocation(Rapp_t *app,
			       XStandardColormap *standardcolormap,
			       XVisualInfo *visualinfo,
			       Atom property)
{
    int retval;

    if (app == NULL
	|| standardcolormap == NULL
	|| visualinfo == NULL) {
	
	return -1;
    }
    
    switch (visualinfo->class) {
        case GrayScale:
	    retval = app_set_rgb_grayscale_allocation(app,
						      standardcolormap,
						      visualinfo,
						      property);

	    break;
        case PseudoColor:
	    retval = app_set_rgb_pseudocolor_allocation(app,
							standardcolormap,
							visualinfo,
							property);

	    break;
        case TrueColor:
	    retval = app_set_rgb_truecolor_allocation(app,
						      standardcolormap,
						      visualinfo,
						      property);

	    break;
        case DirectColor:
	    retval = app_set_rgb_directcolor_allocation(app,
							standardcolormap,
							visualinfo,
							property);

	    break;
        default:
	    
	    retval = -1;

	    break;
    }

    return retval;
}

int
app_set_rgb_best_allocation(Rapp_t *app,
			    XStandardColormap *standardcolormap,
			    XVisualInfo *visualinfo,
			    Atom property)
{
    int n, bits, shiftred, shiftgreen, shiftblue;

    if (app == NULL
	|| standardcolormap == NULL
	|| visualinfo == NULL) {

	return -1;
    }

    if (visualinfo->class == TrueColor || visualinfo->class == DirectColor) {
	standardcolormap->red_max = visualinfo->red_mask;

	while ((standardcolormap->red_max & 0x1) == 0) {
	    standardcolormap->red_max >>= 1;
	}

	standardcolormap->green_max = visualinfo->green_mask;

	while ((standardcolormap->green_max & 0x1) == 0) {
	    standardcolormap->green_max >>= 1;
	}

	standardcolormap->blue_max = visualinfo->blue_mask;

	while ((standardcolormap->blue_max & 0x1) == 0) {
	    standardcolormap->blue_max >>= 1;
	}

	standardcolormap->red_mult = LOWBIT(visualinfo->red_mask);
	standardcolormap->green_mult = LOWBIT(visualinfo->green_mask);
	standardcolormap->blue_mult = LOWBIT(visualinfo->blue_mask);
    } else {
	n = 1;

	bits = 0;

	while (n < visualinfo->colormap_size) {
	    n <<= 1;

	    bits++;
	}

	if (n == visualinfo->colormap_size) {
	    shiftblue = bits / 3;
	    shiftgreen = shiftblue + ((bits % 3) ? 1 : 0);
	    shiftred = shiftblue + (((bits % 3) == 2) ? 1 : 0);

	    standardcolormap->red_max = 1UL << shiftred;
	    standardcolormap->green_max = 1UL << shiftgreen;
	    standardcolormap->blue_max = 1UL << shiftblue;
	} else {
	    standardcolormap->red_max
		= cube_root_with_bits(visualinfo->colormap_size, bits);
	    standardcolormap->blue_max = standardcolormap->red_max;
	    standardcolormap->green_max
		= visualinfo->colormap_size
		/ (standardcolormap->red_max * standardcolormap->blue_max);
	}

	standardcolormap->red_max--;
	standardcolormap->green_max--;
	standardcolormap->blue_max--;

	standardcolormap->red_mult
	    = ((standardcolormap->red_max > 0)
	       ? ((standardcolormap->green_max + 1)
		  * (standardcolormap->blue_max + 1))
	       : 0);
	standardcolormap->green_mult
	    = ((standardcolormap->green_max > 0)
	       ? (standardcolormap->blue_max + 1)
	       : 0);
	standardcolormap->blue_mult
	    = ((standardcolormap->blue_max > 0)
	       ? 1
	       : 0);
    }

    standardcolormap->base_pixel = 0;

    return 0;
}

int
app_set_rgb_grayscale_allocation(Rapp_t *app,
				 XStandardColormap *standardcolormap,
				 XVisualInfo *visualinfo,
				 Atom property)
{
    int grays;

    if (app == NULL
	|| standardcolormap == NULL
	|| visualinfo == NULL) {

	return -1;
    }

    if (visualinfo->colormap_size > 65000) {
	/* at least 16 planes. */

	grays = 4096;
    } else if (visualinfo->colormap_size > 4000) {
	/* at least 12 planes. */

	grays = 512;
    } else if (visualinfo->colormap_size >= 250) {
	/* at least 8 planes. */

	grays = 12;
    } else {
	
	return -1;
    }
    
    standardcolormap->red_max = grays * 30 / 100;
    standardcolormap->green_max = grays * 59 / 100;
    standardcolormap->blue_max = grays * 11 / 100;
    
    standardcolormap->green_max
	+= grays
	- 1
	- standardcolormap->red_max
	- standardcolormap->green_max
	- standardcolormap->blue_max;

    standardcolormap->red_mult
	= standardcolormap->green_mult
	= standardcolormap->blue_mult
	= 1;

    standardcolormap->base_pixel = 0;

    return 0;
}

int
app_set_rgb_pseudocolor_allocation(Rapp_t *app,
				   XStandardColormap *standardcolormap,
				   XVisualInfo *visualinfo,
				   Atom property)
{
    if (app == NULL
	|| standardcolormap == NULL
	|| visualinfo == NULL) {

	return -1;
    }

    if (visualinfo->colormap_size > 65000) {
	/* at least 16 planes. */
	
	standardcolormap->red_max
	    = standardcolormap->green_max
	    = standardcolormap->blue_max
	    = 27;
    } else if (visualinfo->colormap_size > 4000) {
	/* at least12 planes. */
	
	standardcolormap->red_max
	    = standardcolormap->green_max
	    = standardcolormap->blue_max
	    = 12;
    } else if (visualinfo->colormap_size >= 250) {
	/* 8 planes */
	
	standardcolormap->red_max
	    = standardcolormap->green_max
	    = standardcolormap->blue_max
	    = cube_root(visualinfo->colormap_size - 125) - 1;
    } else {
	
	return -1;
    }

    if (property == XA_RGB_GRAY_MAP) {
	standardcolormap->red_mult
	    = standardcolormap->green_mult
	    = standardcolormap->blue_mult
	    = 1;
    } else {
	standardcolormap->red_mult
	    = ((standardcolormap->red_max > 0)
	       ? ((standardcolormap->green_max + 1)
		  * (standardcolormap->blue_max + 1))
	       : 0);
	standardcolormap->green_mult
	    = ((standardcolormap->green_max > 0)
	       ? (standardcolormap->blue_max + 1)
	       : 0);
	standardcolormap->blue_mult
	    = ((standardcolormap->blue_max > 0)
	       ? 1
	       : 0);
    }

    standardcolormap->base_pixel = 0;

    return 0;
}

int
app_set_rgb_truecolor_allocation(Rapp_t *app,
				 XStandardColormap *standardcolormap,
				 XVisualInfo *visualinfo,
				 Atom property)
{
    if (app == NULL
	|| standardcolormap == NULL
	|| visualinfo == NULL) {

	return -1;
    }

    standardcolormap->red_max
	= visualinfo->red_mask / LOWBIT(visualinfo->red_mask);
    standardcolormap->green_max
	= visualinfo->green_mask / LOWBIT(visualinfo->green_mask);
    standardcolormap->blue_max
	= visualinfo->blue_mask / LOWBIT(visualinfo->blue_mask);

    if (property == XA_RGB_GRAY_MAP) {
	standardcolormap->red_mult
	    = standardcolormap->green_mult
	    = standardcolormap->blue_mult
	    = 1;
    } else {
	standardcolormap->red_mult = LOWBIT(visualinfo->red_mask);
	standardcolormap->green_mult = LOWBIT(visualinfo->green_mask);
	standardcolormap->blue_mult = LOWBIT(visualinfo->blue_mask);
    }

    standardcolormap->base_pixel = 0;

    return 0;
}

int
app_set_rgb_directcolor_allocation(Rapp_t *app,
				   XStandardColormap *standardcolormap,
				   XVisualInfo *visualinfo,
				   Atom property)
{
    if (app == NULL
	|| standardcolormap == NULL
	|| visualinfo == NULL) {

	return -1;
    }

    if (visualinfo->colormap_size < 10) {
	
	return -1;
    }

    standardcolormap->red_max
	= standardcolormap->green_max
	= standardcolormap->blue_max
	= visualinfo->colormap_size / 2 - 1;

    if (property == XA_RGB_GRAY_MAP) {
	standardcolormap->red_mult
	    = standardcolormap->green_mult
	    = standardcolormap->blue_mult
	    = 1;
    } else {
	standardcolormap->red_mult = LOWBIT(visualinfo->red_mask);
	standardcolormap->green_mult = LOWBIT(visualinfo->green_mask);
	standardcolormap->blue_mult = LOWBIT(visualinfo->blue_mask);
    }

    standardcolormap->base_pixel = 0;

    return 0;
}

void
app_set_rgb_colormap(Rapp_t *app, XStandardColormap *standardcolormap,
		     Atom property)
{
    XSetRGBColormaps(app->display, DefaultRootWindow(app->display),
		     standardcolormap, 1, property);

    return;
}

int
app_alloc_read_write_rgb_colormap(Rapp_t *app,
				  XStandardColormap *standardcolormap,
				  XVisualInfo *visualinfo)
{
    int colors, delta, npixels, firstindex, remainder, i;
    Rcolor_t *pixels, pixel;
    XColor color;

    if (app == NULL
	|| standardcolormap == NULL
	|| visualinfo == NULL) {

	return -1;
    }

    if (visualinfo->class == DirectColor) {
	colors = standardcolormap->red_max;

	colors = MAX(colors, standardcolormap->green_max);

	colors = MAX(colors, standardcolormap->blue_max);

	colors++;

	delta = LOWBIT(visualinfo->red_mask)
	    + LOWBIT(visualinfo->green_mask)
	    + LOWBIT(visualinfo->blue_mask);
    } else {
	colors = standardcolormap->red_max * standardcolormap->red_mult
	    + standardcolormap->green_max * standardcolormap->green_mult
	    + standardcolormap->blue_max * standardcolormap->blue_mult
	    + 1;

	delta = 1;
    }

    if (colors <= 1 || colors > visualinfo->colormap_size) {

	return -1;
    }

    pixels = calloc(visualinfo->colormap_size, sizeof(Rcolor_t));
    if (pixels == NULL) {

	return -1;
    }

    npixels = app_alloc_read_write_colors(app, standardcolormap,
					  pixels, visualinfo->colormap_size,
					  colors);
    if (npixels < 0) {
	free(pixels);

	return -1;
    }

    qsort(pixels, npixels, sizeof(Rcolor_t), compare_pixels);

    if (!pixels_contiguous(pixels, npixels, colors, delta,
			   &firstindex, &remainder)) {
	XFreeColors(app->display, standardcolormap->colormap,
		    pixels, npixels, 0);

	free(pixels);

	return -1;
    }

    standardcolormap->base_pixel = pixels[firstindex];

    if (standardcolormap->red_mult == 1
	&& standardcolormap->green_mult == 1
	&& standardcolormap->blue_mult == 1) {
	/* gray map. */

	for (i = 0, pixel = standardcolormap->base_pixel ;
	     i < colors ;
	     i++, pixel += delta) {
	    color.pixel = pixel;

	    color.red = color.green = color.blue =
		(unsigned short)((i * 65535) / (standardcolormap->red_max
						+ standardcolormap->green_max
						+ standardcolormap->blue_max));

	    if (app_alloc_color(app, standardcolormap, pixels, npixels,
				&color, firstindex + i) < 0) {
		free(pixels);

		return -1;
	    }
	}
    } else if (standardcolormap->green_max == 0
	       && standardcolormap->blue_max == 0) {
	/* red map. */

	for (i = 0, pixel = standardcolormap->base_pixel ;
	     i < colors ;
	     i++, pixel += delta) {
	    color.pixel = pixel;

	    color.red = (unsigned short)((i * 65535)
					 / standardcolormap->red_max);

	    color.green = color.blue = 0;

	    if (app_alloc_color(app, standardcolormap, pixels, npixels,
				&color, firstindex + i) < 0) {
		free(pixels);

		return -1;
	    }
	}
    } else if (standardcolormap->red_max == 0
	       && standardcolormap->blue_max == 0) {
	/* green map. */

	for (i = 0, pixel = standardcolormap->base_pixel ;
	     i < colors ;
	     i++, pixel += delta) {
	    color.pixel = pixel;

	    color.green = (unsigned short)((i * 65535)
					   / standardcolormap->green_max);

	    color.red = color.blue = 0;

	    if (app_alloc_color(app, standardcolormap, pixels, npixels,
				&color, firstindex + i) < 0) {
		free(pixels);

		return -1;
	    }
	}
    } else if (standardcolormap->red_max == 0
	       && standardcolormap->green_max == 0) {
	/* blue map. */

	for (i = 0, pixel = standardcolormap->base_pixel ;
	     i < colors ;
	     i++, pixel += delta) {
	    color.pixel = pixel;

	    color.blue = (unsigned short)((i * 65535)
					  / standardcolormap->blue_max);

	    color.red = color.green = 0;

	    if (app_alloc_color(app, standardcolormap, pixels, npixels,
				&color, firstindex + i) < 0) {
		free(pixels);

		return -1;
	    }
	}
    } else {
	/* standard RGB cube map. */

	for (i = 0, pixel = 0 ;
	     i < colors ;
	     i++, pixel += delta) {
	    color.pixel = standardcolormap->base_pixel + pixel;

	    color.red = COLORMAP_VALUE(standardcolormap,
				       red_max, red_mult,
				       pixel);
	    color.green = COLORMAP_VALUE(standardcolormap,
					 green_max, green_mult,
					 pixel);
	    color.blue = COLORMAP_VALUE(standardcolormap,
					blue_max, blue_mult,
					pixel);

	    if (app_alloc_color(app, standardcolormap, pixels, npixels,
				&color, firstindex + i) < 0) {
		free(pixels);

		return -1;
	    }
	}
    }

    if (firstindex) {
	XFreeColors(app->display, standardcolormap->colormap,
		    pixels, firstindex, 0);
    }

    if (remainder) {
	XFreeColors(app->display, standardcolormap->colormap,
		    &(pixels[(firstindex + colors)]), remainder, 0);
    }

    free(pixels);

    return 0;
}

int
app_alloc_read_only_rgb_colormap(Rapp_t *app,
				 XStandardColormap *standardcolormap,
				 XVisualInfo *visualinfo)
{
    int lastpixel;
    Rcolor_t c;
    XColor color;

    if (app == NULL
	|| standardcolormap == NULL
	|| visualinfo == NULL) {

	return -1;
    }

    lastpixel
	= (standardcolormap->red_max + 1)
	* (standardcolormap->green_max + 1)
	* (standardcolormap->blue_max + 1)
	+ standardcolormap->base_pixel
	- 1;

    for (c = standardcolormap->base_pixel ; c <= lastpixel ; c++) {
	color.pixel = c;

	color.red = (unsigned short)(((c / standardcolormap->red_mult)
				      * 65535)
				     / standardcolormap->red_max);
	
	if (visualinfo->class == StaticColor) {
	    color.green = (unsigned short)((((c / standardcolormap->green_mult)
					     % (standardcolormap->green_max + 1))
					    * 65535)
					   / standardcolormap->green_max);
	    
	    /* FIXME: is this really green_mult? */

	    color.blue = (unsigned short)(((c % standardcolormap->green_mult)
					   * 65535)
					  / standardcolormap->blue_max);
	} else {
	    /* visualinfo->class == GrayScale. */
	    
	    color.green = color.blue = color.red;
	}

	XAllocColor(app->display, standardcolormap->colormap, &color);

	if (color.pixel != c) {
	    
	    return -1;
	}
    }
    
    return 0;
}

int
app_alloc_color(Rapp_t *app, XStandardColormap *standardcolormap,
		Rcolor_t *pixels, int npixels, XColor *color, int index)
{
    Rcolor_t pixel;
    XColor request;

    pixel = color->pixel;

    request.red = color->red;
    request.green = color->green;
    request.blue = color->blue;

    XFreeColors(app->display, standardcolormap->colormap, &pixel, 1, 0);

    if (!XAllocColor(app->display, standardcolormap->colormap, color)
	|| color->pixel != pixel
	|| app_alloc_read_write_color(app, standardcolormap, color,
				      &request, &pixel) < 0) {
	app_free_rgb_color_cells(app, standardcolormap, pixels,
				 npixels, index);

	return -1;
    }

    return 0;
}

int
app_alloc_read_write_color(Rapp_t *app, XStandardColormap *standardcolormap,
			   XColor *color, XColor *request, Rcolor_t *pixel)
{
    Rcolor_t c = *pixel;

    XFreeColors(app->display, standardcolormap->colormap,
		&(color->pixel), 1, 0);

    if (!XAllocColorCells(app->display, standardcolormap->colormap, False,
			  NULL, 0, pixel, 1)) {

	return -1;
    }

    if (*pixel != c) {
	XFreeColors(app->display, standardcolormap->colormap, pixel, 1, 0);

	return -1;
    }

    color->pixel = *pixel;
    color->flags = DoRed | DoGreen | DoBlue;
    color->red = request->red;
    color->green = request->green;
    color->blue = request->blue;

    XStoreColors(app->display, standardcolormap->colormap, color, 1);

    return 0;
}

int
app_alloc_read_write_colors(Rapp_t *app, XStandardColormap *standardcolormap,
			    Rcolor_t *pixels, int colormapsize, int colors)
{
    int n;

    if (app == NULL
	|| standardcolormap == NULL
	|| pixels == NULL
	|| colormapsize <= 0
	|| colors <= 0) {

	return -1;
    }

    if (XAllocColorCells(app->display, standardcolormap->colormap,
			 True, NULL, 0, pixels, colormapsize)) {

	return colormapsize;
    }

    colormapsize--;

    while (colors < colormapsize) {
	n = colors + (colormapsize - colors + 1) / 2;

	if (XAllocColorCells(app->display, standardcolormap->colormap,
			     True, NULL, 0, pixels, n)) {
	    if (n == colormapsize) {

		return n;
	    } else {
		XFreeColors(app->display, standardcolormap->colormap,
			    pixels, n, 0);

		colors = n;
	    }
	} else {
	    colormapsize = n - 1;
	}
    }

    return -1;
}

void
app_free_rgb_color_cells(Rapp_t *app, XStandardColormap *standardcolormap,
			 Rcolor_t *pixels, int npixels, int index)
{
    XFreeColors(app->display, standardcolormap->colormap,
		pixels, index, 0);

    XFreeColors(app->display, standardcolormap->colormap,
		&(pixels[(index + 1)]), npixels - index - 1, 0);

    free(pixels);

    return;
}

int
app_rgb_colormap_values_valid(Rapp_t *app, XStandardColormap *standardcolormap,
			      Atom property)
{
    int colors, visuals;
    Rcolor_t mask, red_max, green_max, blue_max;
    XVisualInfo visualinfo, *visualinfoptr;

    if (app == NULL
	|| standardcolormap == NULL) {

	return -1;
    }

    visualinfo.visualid = standardcolormap->visualid;
    visualinfo.screen = DefaultScreen(app->display);

    visualinfoptr = XGetVisualInfo(app->display,
				   VisualIDMask | VisualScreenMask,
				   &visualinfo,
				   &visuals);

    if (visualinfoptr == NULL) {

	return FALSE;
    } else {
	red_max = standardcolormap->red_max;
	green_max = standardcolormap->green_max;
	blue_max = standardcolormap->blue_max;

	if (visualinfoptr->class == TrueColor
	    || visualinfoptr->class == DirectColor) {
	    mask = visualinfoptr->red_mask;
	    
	    while (!(mask & 0x1)) {
		mask >>= 1;
	    }

	    if (red_max > mask) {

		return FALSE;
	    }

	    mask = visualinfoptr->green_mask;

	    while (!(mask & 0x1)) {
		mask >>= 1;
	    }

	    if (green_max > mask) {

		return FALSE;
	    }

	    mask = visualinfoptr->blue_mask;

	    while (!(mask & 0x1)) {
		mask >>= 1;
	    }

	    if (blue_max > mask) {

		return FALSE;
	    }
	} else if (property == XA_RGB_GRAY_MAP) {
	    colors = red_max + green_max + blue_max + 1;

	    if (colors > visualinfoptr->colormap_size) {

		return FALSE;
	    }
	} else {
	    colors = (red_max + 1) * (green_max + 1) * (blue_max + 1);

	    if (colors > visualinfoptr->colormap_size) {

		return FALSE;
	    }
	}

	switch (property) {
	    case XA_RGB_BEST_MAP:
	    case XA_RGB_DEFAULT_MAP:
	    case XA_RGB_GRAY_MAP:
		if (red_max == 0 || green_max == 0 || blue_max == 0) {

		    return FALSE;
		}

		break;
  	    case XA_RGB_RED_MAP:
		if (red_max == 0) {
		    
		    return FALSE;
		}

		break;
	    case XA_RGB_GREEN_MAP:
		if (green_max == 0) {

		    return FALSE;
		}

		break;
	    case XA_RGB_BLUE_MAP:
		if (blue_max == 0) {

		    return FALSE;
		}

		break;
	    default:
		
		return FALSE;
	}
    }

    return TRUE;
}

int
app_set_colormap(Rapp_t *app)
{
    int retval, dummyi;
    Status status;
    XStandardColormap *standardcolormap;
    XVisualInfo visualinfo, *visualinfoptr;

    if (app == NULL) {

	return -1;
    }

    retval = -1;

#if 0
    status = XGetRGBColormaps(app->display,
			      DefaultRootWindow(app->display),
			      &standardcolormap,
			      &dummyi,
			      XA_RGB_BEST_MAP);
    if (!status) {
	status = XGetRGBColormaps(app->display,
				  DefaultRootWindow(app->display),
				  &standardcolormap,
				  &dummyi,
				  XA_RGB_DEFAULT_MAP);
    }
    if (status) {
	visualinfo.screen = DefaultScreen(app->display);

	visualinfo.visualid = standardcolormap->visualid;

	visualinfoptr = XGetVisualInfo(app->display,
				       VisualIDMask | VisualScreenMask,
				       &visualinfo,
				       &dummyi);
	if (visualinfoptr) {
	    app->depth = visualinfoptr->depth;
	    app->visual = visualinfoptr->visual;
	    app->colormap = standardcolormap->colormap;

	    XFree(standardcolormap);

	    XFree(visualinfoptr);
	    
	    return 0;
	}

	XFree(standardcolormap);

	XFree(visualinfoptr);
    }
#endif

    app->depth = DefaultDepth(app->display,
			      DefaultScreen(app->display));
    app->visual = DefaultVisual(app->display,
				DefaultScreen(app->display));
    app->colormap = DefaultColormap(app->display,
				    DefaultScreen(app->display));
    
    retval = 0;

#if 0
    if (DefaultDepth(app->display, DefaultScreen(app->display)) == 32) {
	/* use 32-plane default visual. */

	app->depth = DefaultDepth(app->display, DefaultScreen(app->display));
	app->visual = DefaultVisual(app->display,
				    DefaultScreen(app->display));
	app->colormap = DefaultColormap(app->display,
					DefaultScreen(app->display));

	retval = 0;
    } else if (DefaultDepth(app->display, DefaultScreen(app->display)) == 24) {
	/* use 24-plane default visual. */

	app->depth = DefaultDepth(app->display, DefaultScreen(app->display));
	app->visual = DefaultVisual(app->display,
				    DefaultScreen(app->display));
	app->colormap = DefaultColormap(app->display,
					DefaultScreen(app->display));

	retval = 0;
    } else {
	retval = app_get_colormap(app, 32);

	if (retval < 0) {
	    retval = app_get_colormap(app, 24);
	}

	if (retval < 0) {
	    retval = app_get_colormap(app, 16);
	}

	if (retval < 0) {
	    retval = app_get_colormap(app, 15);
	}

	if (retval < 0) {
	    retval = app_get_colormap(app, 8);
	}

	if (retval < 0) {
	    app->depth = DefaultDepth(app->display,
				      DefaultScreen(app->display));
	    app->visual = DefaultVisual(app->display,
					DefaultScreen(app->display));
	    app->colormap = DefaultColormap(app->display,
					    DefaultScreen(app->display));

	    retval = 0;
	}
    }
#endif

    fprintf(stderr, "%s: app_set_colormap(): depth == %d, visual class == %s\n", app->name, app->depth, visualclassnames[(app->visual->class)]);

    return retval;
}

int
app_get_colormap(Rapp_t *app, int depth)
{
    XVisualInfo visualinfo;

    if (app == NULL
	|| depth < 0
	|| depth > 32) {

	return -1;
    }

    /* TODO: break this into several ifs. */

    if (XMatchVisualInfo(app->display, DefaultScreen(app->display),
			 depth, DirectColor, &visualinfo)
	|| XMatchVisualInfo(app->display, DefaultScreen(app->display),
			    depth, TrueColor, &visualinfo)
	|| XMatchVisualInfo(app->display, DefaultScreen(app->display),
			    depth, PseudoColor, &visualinfo)
	|| XMatchVisualInfo(app->display, DefaultScreen(app->display),
			    depth, StaticColor, &visualinfo)
	|| XMatchVisualInfo(app->display, DefaultScreen(app->display),
			    depth, GrayScale, &visualinfo)
	|| XMatchVisualInfo(app->display, DefaultScreen(app->display),
			    depth, StaticGray, &visualinfo)) {
	app->depth = depth;
	app->visual = visualinfo.visual;
	app->colormap = XCreateColormap(app->display,
					DefaultRootWindow(app->display),
					app->visual,
					AllocNone);
    } else {
	
	return -1;
    }

    return 0;
}

#endif /* 0 */
