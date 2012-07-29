/*
 * imlib2.c - Imlib2 interface for Resurrection.
 * Copyright (C) 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */
#include <Resurrection/Resurrection.h>

#if 0
int
app_init_imlib2(struct R_app *app)
{
    if (app == NULL) {

	return -1;
    }

    imlib_context_set_display(app->display);
    imlib_context_set_visual(app->visual);
    imlib_context_set_colormap(app->colormap);
    imlib_context_set_drawable(app->window->win);
    imlib_context_set_blend(0);
    imlib_context_set_mask(0);
    imlib_set_font_cache_size(1024 * 1024);
#if 0
    imlib_add_path_to_font_path(RESURRECTION_FONT_SEARCH_PATH "ttf/");
#endif
    imlib_set_cache_size(4096 * 1024);

    return 0;
}

int
image_load_imlib2(struct R_image *image, const char *filename)
{
    size_t len;
    Imlib_Image *img;
    Imlib_Color_Modifier colormod;
    int i;
    struct R_image loadimage;

    if (image == NULL
	|| filename == NULL) {

	return -1;
    }
    fprintf(stderr, "loading %s\n", filename);

    image_destroy_imlib2(image);
    len = strlen(filename);
    image->filename = calloc(1, len + 1);
    if (image->filename == NULL) {

	return -1;
    }
    strncpy(image->filename, filename, len);
    memset(&loadimage, 0, sizeof(loadimage));
    img = imlib_load_image(filename);
    if (img == NULL) {
	image_destroy_imlib2(image);

	return -1;
    }
    imlib_context_set_image(img);
    image->origw = imlib_image_get_width();
    image->origh = imlib_image_get_height();
    for (i = 0 ; i < IMAGE_COLOR_MODIFIERS ; i++) {
	colormod = imlib_create_color_modifier();
	if (colormod == NULL) {
	    image_destroy_imlib2(image);

	    return -1;
	}
	image->im2colormods[i] = colormod;
    }
    colormod = imlib_create_color_modifier();
    if (colormod == NULL) {
	image_destroy_imlib2(image);

	return -1;
    }
    image->im2colormods[IMAGE_COLOR_MODIFIER] = colormod;
    image->im2img = img;

    return 0;
}

void
image_load_data_imlib2(struct R_image *image)
{
    if (image == NULL) {

	return;
    }

    image->im2img = imlib_load_image(image->filename);

    return;
}

void
image_load_thumb_imlib2(struct R_image *image)
{
    if (image == NULL) {

	return;
    }

    image->im2thumb = imlib_load_image(image->filename);

    return;
}

void
image_destroy_imlib2(struct R_image *image)
{
    Imlib_Border *border;
    Imlib_Color_Modifier colormod;
    int i;

    if (image == NULL) {

	return;
    }

    if (image->filename) {
	free(image->filename);
	image->filename = NULL;
    }
    border = &image->im2border;
    memset(border, 0, sizeof(Imlib_Border));
    for (i = 0 ; i < IMAGE_COLOR_MODIFIERS ; i++) {
	colormod = image->im2colormods[i];
	if (colormod) {
	    imlib_context_set_color_modifier(colormod);
	    imlib_free_color_modifier();
	}
    }
    image->flags &= ~IMAGE_KEEP_PIXMAP;
    image_destroy_data_imlib2(image);

    return;
}

void
image_destroy_data_imlib2(struct R_image *image)
{
    if (image == NULL) {

	return;
    }
    
    if (image->im2img) {
	imlib_context_set_image(image->im2img);
	imlib_free_image();
	image->im2img = NULL;
    }
#if 0
    if (image->im2thumb) {
	imlib_context_set_image(image->im2thumb);
	imlib_free_image();
	image->im2thumb = NULL;
    }
#endif
    if (!(image->flags & IMAGE_KEEP_PIXMAP)) {
	image_destroy_pixmaps_imlib2(image);
    }
}

void
image_destroy_pixmaps_imlib2(struct R_image *image)
{
    if (image == NULL) {

	return;
    }

    if (image->pixmap) {
	imlib_free_pixmap_and_mask(image->pixmap);
	image->pixmap = None;
	image->maskpixmap = None;
    }

    return;
}

int
image_apply_modifier_imlib2(struct R_image *image, int which,
			    double gamma, double brightness, double contrast)
{
    Imlib_Image img;
    Imlib_Color_Modifier colormod;

    if (image == NULL
	|| which < 0
	|| which > IMAGE_COLOR_MODIFIERS - 1) {

	return -1;
    }

    img = image->im2img;
    if (img) {
	imlib_context_set_image(img);
	colormod = image->im2colormods[which];
	if (colormod == NULL) {
	    colormod = imlib_create_color_modifier();
	    if (colormod == NULL) {
		
		return -1;
	}
	}
	imlib_context_set_color_modifier(colormod);
	if (gamma >= 0.0) {
	    imlib_modify_color_modifier_gamma(gamma);
	} else {
	    imlib_modify_color_modifier_gamma(IMLIB2_NORMAL_MODIFIER);
	}
	if (brightness >= 0.0) {
	    imlib_modify_color_modifier_brightness(brightness);
	} else {
	    imlib_modify_color_modifier_brightness(IMLIB2_NORMAL_MODIFIER);
	}
	if (contrast >= 0.0) {
	    imlib_modify_color_modifier_contrast(contrast);
	} else {
	    imlib_modify_color_modifier_contrast(IMLIB2_NORMAL_MODIFIER);
	}
	imlib_apply_color_modifier();
    }

    return 0;
}

void
image_set_border_imlib2(struct R_image *image,
			int left, int right, int top, int bottom)
{
    Imlib_Image img;
    Imlib_Border *border;

    if (image == NULL
	|| top < 0
	|| bottom < 0
	|| left < 0
	|| right < 0) {

	return;
    }

    border = &image->im2border;
    border->top = top;
    border->bottom = bottom;
    border->left = left;
    border->right = right;

    return;
}

struct R_image *
image_clone_imlib2(Drawable drawable, struct R_image *image, int w, int h)
{
    struct R_image *newimage;
    Imlib_Image img, newimg;
    int imagew, imageh;
    int neww, newh;

    if (image == NULL) {

	return NULL;
    }

    newimage = NULL;
    img = image->im2img;
    if (img) {
	imlib_context_set_drawable(drawable);
	imlib_context_set_image(img);
	imagew = imlib_image_get_width();
	imageh = imlib_image_get_height();
	if (w == 0) {
	    neww = imagew;
	} else {
	    neww = w;
	}
	if (h == 0) {
	    newh = imageh;
	} else {
	    newh = h;
	}
	newimg = imlib_create_cropped_scaled_image(0, 0,
						   imagew, imageh,
						   neww, newh);
	if (newimg == NULL) {

	    return NULL;
	}
	newimage = image_alloc();
	if (newimage == NULL) {

	    return NULL;
	}
	newimage->w = neww;
	newimage->h = newh;
	newimage->im2img = newimg;
    }

    return newimage;
}

void
window_fade_imlib2(Rwindow_t *window, struct R_image *image, int usecs, int mode)
{
    double value, lastvalue;
    struct R_image *scaledimage;
    Imlib_Color_Modifier colormod;
    Pixmap pixmap;
    Pixmap mask;
    struct timeval start, stop;
    int usedtime;
    XWindowAttributes attributes;

    if (window == NULL
	|| image == NULL) {

	return;
    }

    if (mode == WINDOW_FADE_IN) {
	value = 0.0;
    } else {
	value = 1.0;
    }
    lastvalue = -1.0;
    usedtime = 0;
    memset(&attributes, 0, sizeof(attributes));
    XGetWindowAttributes(window->app->display, window->win, &attributes);
    scaledimage = image_clone_imlib2(window->win,
				     image,
				     attributes.width, attributes.height);
    if (scaledimage == NULL) {

	return;
    }
    colormod = imlib_create_color_modifier();
    if (colormod == NULL) {
	image_destroy_imlib2(scaledimage);

	return;
    }
    imlib_context_set_drawable(window->win);
    imlib_context_set_image(scaledimage->im2img);
    imlib_context_set_color_modifier(colormod);
    imlib_modify_color_modifier_gamma(1.0);
    imlib_modify_color_modifier_contrast(1.0);
    gettimeofday(&start, NULL);
    while (usedtime < usecs) {
	if (value != lastvalue) {
	    imlib_modify_color_modifier_brightness(value);
	    imlib_context_set_mask(0);
	    imlib_render_pixmaps_for_whole_image(&pixmap, &mask);
	    XSetWindowBackgroundPixmap(window->app->display,
				       window->win,
				       pixmap);
	    XClearWindow(window->app->display,
			 window->win);
	    XFlush(window->app->display);
	    imlib_free_pixmap_and_mask(pixmap);
	    lastvalue = value;
	}
	gettimeofday(&stop, NULL);
	usedtime = TIMEVAL_CMP_USEC(&start, &stop);
	value = IMAGE_NORMAL_MODIFIER * (usedtime / (double)usecs);
	if (mode == WINDOW_FADE_OUT) {
	    value = IMAGE_NORMAL_MODIFIER - value;
	}
    }
    if (mode == WINDOW_FADE_IN
	&& lastvalue != IMAGE_NORMAL_MODIFIER) {
	imlib_modify_color_modifier_brightness(IMAGE_NORMAL_MODIFIER);
	imlib_context_set_mask(0);
	imlib_render_pixmaps_for_whole_image(&pixmap, &mask);
	XSetWindowBackgroundPixmap(window->app->display,
				   window->win,
				   pixmap);
	XClearWindow(window->app->display,
		     window->win);
	XFlush(window->app->display);
	    imlib_free_pixmap_and_mask(pixmap);
    } else if (lastvalue != 0.0) {
	imlib_modify_color_modifier_brightness(0.0);
	imlib_context_set_mask(0);
	imlib_render_pixmaps_for_whole_image(&pixmap, &mask);
	XSetWindowBackgroundPixmap(window->app->display,
				   window->win,
				   pixmap);
	XClearWindow(window->app->display,
		     window->win);
	XFlush(window->app->display);
	imlib_free_pixmap_and_mask(pixmap);
    }
    image_destroy_imlib2(scaledimage);

    return;
}

int
image_render_imlib2(Drawable drawable, struct R_image *image, int w, int h)
{
    Window dummyw;
    Imlib_Image img;
    Imlib_Border *border;
    Imlib_Color_Modifier colormod;
    Pixmap pixmap;
    Pixmap mask;
    int origw, origh;
    int neww, newh;
    int dummyi;
    unsigned int drawablew, drawableh;
    unsigned int dummyui;

    if (image == NULL) {

	return -1;
    }

    if (image->im2img == NULL) {
	image_load_data_imlib2(image);
    }
    pixmap = None;
    mask = None;
    img = image->im2img;
    if (img) {
        imlib_context_set_drawable(drawable);
	imlib_context_set_image(img);
	imlib_context_set_mask(image->maskpixmap);
	imlib_context_set_blend(0);
	border = &image->im2border;
	imlib_image_set_border(border);
	if (w == -1 || h == -1) {
	    XGetGeometry(Rglobals.app->display,
			 drawable,
			 &dummyw,
			 &dummyi, &dummyi,
			 &drawablew, &drawableh,
			 &dummyui,
			 &dummyui);
	}
        origw = imlib_image_get_width();
        image->origw = origw;
	if (w == 0) {
	    neww = origw;
	} else if (w == -1) {
	    neww = drawablew;
	} else {
	    neww = w;
	}
        origh = imlib_image_get_height();
        image->origh = origh;
	if (h == 0) {
	    newh = origh;
	} else if (h == -1) {
	    newh = drawableh;
	} else {
	    newh = h;
	}
	colormod = image->im2colormods[IMAGE_COLOR_MODIFIER];
	if (colormod == NULL) {
	    colormod = imlib_create_color_modifier();
	    image->im2colormods[IMAGE_COLOR_MODIFIER] = colormod;
	}
	imlib_context_set_color_modifier(colormod);
	imlib_render_pixmaps_for_whole_image_at_size(&pixmap, &mask, neww, newh);
	if (image->pixmap) {
	    imlib_free_pixmap_and_mask(image->pixmap);
	    image->pixmap = None;
	    image->maskpixmap = None;
	}
	image->pixmap = pixmap;
	image->maskpixmap = mask;
#if 0
	image->flags |= IMAGE_KEEP_PIXMAP;
	image_destroy_data_imlib2(image);
#endif
    }

    return 0;
}

int
window_render_background_imlib2(Rwindow_t *window, struct R_image *image)
{
    Imlib_Image img;

    if (window == NULL) {

	return -1;
    }

    if (image == NULL) {
	XSetWindowBackgroundPixmap(window->app->display,
				   window->win,
				   ParentRelative);
	XClearWindow(window->app->display,
		     window->win);
	XFlush(window->app->display);

	return 0;
    }

    if (image->flags & IMAGE_TRANSPARENT) {
	window_set_transparent_background_pixmap(window);

	return 0;
    }

    img = image->im2img;
    if (img) {
	image_render_imlib2(window->win, image, image->w, image->h);
	window_set_background_pixmap(window, image);
    } else {

	return -1;
    }

    return 0;
}
#endif

