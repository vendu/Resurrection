/*
 * load.c - Rv image loader.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

void
Rv_show_image(struct R_window *window,
              char *filename)
{
    struct R_app    *app;
    struct R_window *view;
#if (USE_IMLIB2)
    struct R_image  *image;
#endif
    int vieww;
    int viewh;
    int screenw;
    int screenh;
    float f1;
    float f2;
    Screen *screenptr;

    app = window->app;
    view = window->app->window;
#if (USE_IMLIB2)
    image = R_load_image_imlib2(view->app,
                                filename,
                                NULL);
    if (image == NULL) {

        return;
    }
//    view->image = image;
    vieww = image->origw;
    viewh = image->origh;
    f1 = f2 = 0.0;
    screenptr = ScreenOfDisplay(app->display, DefaultScreen(app->display));
    screenw = WidthOfScreen(screenptr) - 64;
    screenh = HeightOfScreen(screenptr) - 64;
    if (vieww > screenw
        || viewh > screenh
        || (window->clientflags & RV_FIT_FLAG)) {
        fprintf(stderr, "BOO\n");
        f1 = (float)screenw / (float)vieww;
        f2 = (float)screenh / (float)viewh;
        if (f2 < f1) {
            f1 = f2;
        }
        vieww = (int)((float)vieww * f1);
        viewh = (int)((float)viewh * f1);
    } else {
        if (vieww > window->app->window->w) {
            f1 = window->app->window->w / vieww;
        }
        if (viewh > window->app->window->h) {
            f2 = window->app->window->h / viewh;
        }
        if (f1 != 0.0 || f2 != 0.0) {
            f1 = min(f1, f2);
            vieww = (int)((float)vieww * f1);
            viewh = (int)((float)viewh * f1);
        }
    }
    fprintf(stderr, "%dx%d(%dx%d) (%dx%d)\n", vieww, viewh, image->origw, image->origh, screenw, screenh);
    if ((vieww) && (viewh)) {
        R_resize_window(view,
                        vieww,
                        viewh);
        R_set_background_imlib2(image,
                                view,
                                vieww,
                                viewh,
                                0);
    }
#endif
    if (view->image) {
        R_free_image_imlib2(view->image);
    }
    view->image = image;
    R_set_window_title(view, filename, TRUE);
    if (view->clientflags & RV_SLIDE_FLAG) {
        alarm(5);
    }

    return;
}

