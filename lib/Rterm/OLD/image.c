/*
 * image.c - image routines for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

struct R_image *
image_alloc(void)
{
    struct R_image *newimage;
    struct R_colormod *colormod;
    int i;

    newimage = calloc(1, sizeof(struct R_image));
    if (newimage == NULL) {

	return NULL;
    }

    for (i = 0 ; i < R_IMAGE_COLOR_MODIFIERS ; i++) {
	colormod = &newimage->colormods[i];
	colormod->tintmask = 0xffffffff;
	colormod->gamma = 1024;
	colormod->brightness = 1024;
	colormod->contrast = 1024;
    }

    return newimage;
}

void
image_set_color_modifier(struct R_image *image, int which,
			 R_colormod_t gamma,
                         R_colormod_t brightness,
                         R_colormod_t contrast)
{
    if (image == NULL
	|| which < 0
	|| which > R_IMAGE_COLOR_MODIFIERS - 1) {

	return;
    }

    if (gamma >= 0) {
	image->colormods[which].gamma = gamma;
    }
    if (brightness >= 0) {
	image->colormods[which].brightness = brightness;
    }
    if (contrast >= 0) {
	image->colormods[which].contrast = contrast;
    }

    return;
}

void
term_render_image(struct R_term *term, int which)
{
    if (term == NULL
	|| which < 0
	|| which > RTERM_IMAGES - 1) {

	return;
    }

    switch (which) {
        default:

	    break;
    }

    return;
}

