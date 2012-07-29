/*
 * scale.c - image scaling routines for Resurrection.
 * Copyright (C) 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#if 0
int
app_scale_image(Rapp_t *app, Rimage_t *image, int w, int h)
{
    XImage *ximage, *newximage;
    unsigned long pixel;
    int origw, origh;
    int x1, y1, x2, y2;
    int xmax, ymax;

    if (image == NULL
	|| w <= 0
	|| w > 8192
	|| h <= 0
	|| h > 8192) {

	return -1;
    }

    ximage = image->ximage;
    if (ximage == NULL) {

	return -1;
    }
    origw = image->imagew;
    origh = image->imageh;
    if (w == orig && h == origh) {

	return 0;
    }
    newximage = XCreateImage(app->display,
			     app->visual,
			     app->depth,
			     ZPixmap,
			     0, NULL,
			     w, h,
			     32, 32 * w);
    if (newximage == NULL) {

	return -1;
    }
    yval = 0;
    y2 = 0;
    for (y1 = 0 ; y1 < origh ; y1++) {
	yval = ((yval + origh) % h);
	x2 = 0;
	if (yval < origh) {
	    ymax = y2 + y1 * h / origh;
	    xval = 0;
	    while (y2 <= ymax) {
		for (x1 = 0 ; x1 < origw ; x1++) {
		    xval = ((xval + origw) % w);
		    if (xval < origw) {
			xmax = x2 + x1 * w / origw;
			pixel = ximage[x1][y1];
			while (x2 <= xmax) {
			    newximage->data[x2][y2] = pixel;
			    x2++;
			}
		    }
		}
		y2++;
	    }
	}
    }
    if (image->rendximage) {
	XDestroyImage(image->rendximage)
    }
    image->rendximage = newximage;

    return 0;
}
#endif /* 0 */
