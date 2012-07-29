/*
 * imlib2.c - Resurrection Imlib2 interface.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#include <assert.h>

#if (USE_IMLIB2)

#define IMLIB2_CACHE_SIZE      (65536 * 1024)
#define IMLIB2_FONT_CACHE_SIZE (4096 * 1024)

GC _thumbgc;

void
R_init_imlib2(struct R_app *app)
{
    imlib_context_set_display(app->display);
    imlib_context_set_visual(app->visual);
    imlib_context_set_colormap(app->colormap);
    imlib_context_set_blend(0);
    imlib_context_set_mask(0);
    imlib_set_cache_size(IMLIB2_CACHE_SIZE);
    imlib_set_font_cache_size(IMLIB2_FONT_CACHE_SIZE);
    imlib_add_path_to_font_path(RESURRECTION_FONT_SEARCH_PATH "ttf");

    return;
}

struct R_image *
R_load_image_imlib2(struct R_app *app,
                    const char *filename,
                    struct R_image *image)
{
    Imlib_Image *img;

    if (!image) {
        image = R_alloc_image();
    }
    if (image) {
        img = imlib_load_image(filename);
        if (img) {
            imlib_context_set_image(img);
            image->app = app;
            image->filename = (char *)filename;
            image->origw = imlib_image_get_width();
            image->origh = imlib_image_get_height();
            image->orig = img;
            image->pixmap = image->mask = None;
            image->tpixmap = image->tmask = None;
            image->xpixmap = None;
#if (USE_XSHM)
            image->hasshm = 0;
#endif
        }
    }

    return image;
}

int
R_reload_image_imlib2(struct R_image *image)
{
    R_free_image_imlib2(image);
    if (!image->orig) {
        R_load_image_imlib2(image->app,
                            image->filename,
                            image);
    }

    return TRUE;
}

void
R_free_image_imlib2(struct R_image *image)
{
#if (USE_XSHM)
    XShmSegmentInfo *shmseg;
#endif

    if (image && !(image->flags & R_IMAGE_STATIC_FLAG)) {
//        image->filename = NULL;
        if (image->orig) {
            imlib_context_set_image(image->orig);
            imlib_free_image();
            image->img = NULL;
        }
        if (image->pixmap) {
#if (USE_XSHM)
            if (image->hasshm) {
                shmseg = &image->shmseg;
                XShmDetach(image->app->display, shmseg);
                shmdt(shmseg->shmaddr);
                shmctl(shmseg->shmid, IPC_RMID, NULL);
                shmseg->shmid = -1;
                shmseg->shmaddr = NULL;
                image->hasshm = 0;
            } else {
                imlib_free_pixmap_and_mask(image->pixmap);
            }
#else
            imlib_free_pixmap_and_mask(image->pixmap);
#endif
            image->pixmap = image->mask = None;
        }
    }
    if (image->img) {
        imlib_context_set_image(image->img);
        imlib_free_image();
        image->img = NULL;
    }
    if (image->overlay) {
        imlib_context_set_image(image->overlay);
        imlib_free_image();
        image->overlay = NULL;
    }
    if (!(image->flags & R_IMAGE_STATIC_THUMB_FLAG) && (image->timg)) {
        imlib_context_set_image(image->timg);
        imlib_free_image();
        image->timg = NULL;
    }
    if (image->tpixmap) {
        imlib_free_pixmap_and_mask(image->tpixmap);
        image->tpixmap = image->tmask = None;
    }
    if (image->xpixmap) {
        XFreePixmap(R_global.app->display,
                    image->xpixmap);
        image->xpixmap = None;
    }

    return;
}

void
R_destroy_image_imlib2(struct R_image *image)
{
    if (image->orig) {
        imlib_context_set_image(image->orig);
        imlib_free_image();
        image->orig = NULL;
    }
    R_free_image_imlib2(image);

    return;
}

void
R_free_thumb_imlib2(struct R_image *image)
{
    if (image) {
//        image->filename = NULL;
        if (image->timg) {
            imlib_context_set_image(image->timg);
            imlib_free_image();
            image->timg = NULL;
        }
        if (image->tpixmap) {
            imlib_free_pixmap_and_mask(image->tpixmap);
            image->tpixmap = image->tmask = None;
        }
        if (image->xpixmap) {
            XFreePixmap(image->app->display,
                        image->xpixmap);
            image->xpixmap = None;
        }
    }

    return;
}

/* FIXME: check for errors. */
int
R_render_image_imlib2(struct R_image *image,
                      struct R_window *window,
                      int w,
                      int h,
                      int flags)
{
    unsigned int rendw = 0;
    unsigned int rendh = 0;
    int winw = 0;
    int winh = 0;
    int copyx = 0;
    int copyy = 0;
    int copyw;
    int copyh;
    int dummyi;
    int nx;
    int ny;
    unsigned int dummyui;
    Window dummywin;
    Pixmap pixmap;
    int x = 0;
    int y = 0;
    static GC rendgc = NULL;
#if (USE_XSHM)
    int shmid;
    XShmSegmentInfo *shmseg;
#endif
    long factor;
    int xofs = 0;
    int yofs = 0;

#if 0
    if (!image || !image->orig || !window->id) {

        return -1;
    }
#endif
    R_free_image_imlib2(image);
    if (image->flags & R_IMAGE_STATIC_FLAG) {
        if (image->pixmap) {
#if (USE_XSHM)
            if (image->hasshm) {
                shmseg = &image->shmseg;
                XShmDetach(image->app->display, shmseg);
                shmdt(shmseg->shmaddr);
                shmctl(shmseg->shmid, IPC_RMID, NULL);
                shmseg->shmid = -1;
                shmseg->shmaddr = NULL;
                image->hasshm = 0;
            } else {
                imlib_free_pixmap_and_mask(image->pixmap);
            }
#else
            imlib_free_pixmap_and_mask(image->pixmap);
#endif
            image->pixmap = image->mask = None;
        }
    }
    imlib_context_set_drawable(window->id);
    imlib_context_set_image(image->orig);
    imlib_context_set_mask(image->mask);
    imlib_context_set_blend(image->flags & R_IMAGE_BLEND_FLAG);

    if ((flags & R_IMAGE_STRETCH_FLAG)
        || (flags & R_IMAGE_FILL_FLAG)
        || w == R_TILE_FIT_IMAGE || h == R_TILE_FIT_IMAGE
        || w == R_INTEGER_TILE_IMAGE || h == R_INTEGER_TILE_IMAGE
        || w == R_FIT_IMAGE || h == R_FIT_IMAGE
        || (flags & R_IMAGE_FIT_FLAG)) {
        XGetGeometry(image->app->display,
                     window->id,
                     &dummywin,
                     &dummyi,
                     &dummyi,
                     &winw,
                     &winh,
                     &dummyui,
                     &dummyui);
    }
    if (flags & R_IMAGE_STRETCH_FLAG) {
        rendw = winw;
        rendh = winh;
    } else {
        if (w == R_INTEGER_TILE_IMAGE) {
            long tmp1 = winw / image->origw;
            long tmp2 = winh / image->origh;
            factor = MIN(tmp1, tmp2);
            rendw =  image->origw;
            xofs = (winw - (factor * rendw)) >> 1;
        } else if (w == R_TILE_FIT_IMAGE) {
            xofs = 0;
            rendw = image->origw;
            flags |= R_IMAGE_FIT_FLAG;
//            nx = winw / rendw;
//            copyx = (winw - nx * rendw) >> 1;
            copyx = 0;
        } else if (w == R_TILE_IMAGE) {
            rendw = image->origw;
        } else if (w == R_FIT_IMAGE) {
            rendw = winw;
        } else {
            rendw = w;
        }
        if (h == R_INTEGER_TILE_IMAGE) {
            long tmp1 = winw / image->origw;
            long tmp2 = winh / image->origh;
            factor = MIN(tmp1, tmp2);
            rendh = image->origh;
            yofs = (winh - (factor * rendh)) >> 1;
        } else if (h == R_TILE_FIT_IMAGE) {
            yofs = 0;
            rendh = image->origh;
            flags |= R_IMAGE_FIT_FLAG;
//            ny = winh / rendh;
//            copyy = (winh - ny * rendh) >> 1;
//            copyx = 0;
        } else if (h == R_TILE_IMAGE) {
            rendh = image->origh;
        } else if (h == R_FIT_IMAGE) {
            rendh = winh;
        } else {
            rendh = h;
        }
    }
    if (!rendw) {
        rendw = w;
    }
    if (!rendh) {
        rendh = h;
    }
    if (flags & R_IMAGE_FILL_FLAG) {
        rendw = image->origw;
        rendh = image->origh;
    }
    image->img = imlib_create_cropped_scaled_image(0,
                                                   0,
                                                   image->origw,
                                                   image->origh,
                                                   rendw,
                                                   rendh);
    if (!image->img) {

        return -1;
    }

    imlib_context_set_image(image->img);
    imlib_render_pixmaps_for_whole_image_at_size(&image->pixmap,
                                                 &image->mask,
                                                 rendw,
                                                 rendh);
    if (!image->pixmap) {
        
        return -1;
    }
    imlib_free_image();

    if ((flags & R_IMAGE_STRETCH_FLAG)
        || (flags & R_IMAGE_FILL_FLAG)
        || w == R_FIT_IMAGE
        || h == R_FIT_IMAGE
        || w == R_INTEGER_TILE_IMAGE
        || h == R_INTEGER_TILE_IMAGE) {
        if (!rendgc) {
            XGCValues gcvalues;
            
            gcvalues.function = GXcopy;
            rendgc = XCreateGC(window->app->display,
                               window->id,
                               GCFunction,
                               &gcvalues);
        }
#if (USE_XSHM)
        shmseg = &image->shmseg;
        shmid = shmget(IPC_PRIVATE,
                       winw * winh * 4,
                       0777 | IPC_CREAT);
        if (shmid >= 0) {
            shmseg->shmaddr = shmat(shmid, NULL, 0);
            if (shmseg->shmaddr == ((void *)-1)) {
                shmid = -1;
            } else {
                shmseg->shmid = shmid;
                shmseg->readOnly = False;
            }
        }
        if (shmid >= 0) {
            XShmAttach(image->app->display, shmseg);
            pixmap = XShmCreatePixmap(window->app->display,
                                      window->id,
                                      shmseg->shmaddr,
                                      shmseg,
                                      winw,
                                      winh,
                                      window->app->depth);
            image->hasshm = 1;
        } else {
            if (shmseg->shmid >= 0) {
                shmctl(shmseg->shmid, IPC_RMID, NULL);
            }
            shmseg->shmid = -1;
            shmseg->shmaddr = NULL;
            assert(image->app != NULL);
            pixmap = XCreatePixmap(window->app->display,
                                   window->id,
                                   winw,
                                   winh,
                                   window->app->depth);
            image->hasshm = 0;
        }
#else
//        fprintf(stderr, "WINW: %d, WINH: %d\n", winw, winh);
        pixmap = XCreatePixmap(window->app->display,
                               window->id,
                               winw,
                               winh,
                               window->app->depth);
#endif
        if (!pixmap) {
            
            return -1;
        }
//        fprintf(stderr, "XOFS = %d, YOFS = %d, WINW = %d, WINH = %d, RENDW = %d, RENDH = %d\n", xofs, yofs, winw, winh, rendw, rendh);
        for (x = xofs ; x < winw ; x += rendw) {
            rendw = MIN(rendw, winw - x);
            for (y = yofs ; y < winh ; y += rendh) {
                rendh = MIN(rendh, winh - y);
                XCopyArea(image->app->display,
                          image->pixmap,
                          pixmap,
                          rendgc,
                          0,
                          0,
                          MIN(rendw, winw - x),
                          MIN(rendh, winh - y),
                          x,
                          y);
            }
        }
        if (image->pixmap) {
            imlib_free_pixmap_and_mask(image->pixmap);
        }
        image->pixmap = pixmap;
#if 0
        image->w = winw;
        image->h = winh;
#endif
    } else {
        image->w = rendw;
        image->h = rendh;
    }

    return 0;
}

int
R_render_thumb_imlib2(struct R_image *image,
                      struct R_window *window,
                      int w,
                      int h,
                      int flags)
{
    unsigned int destw;
    unsigned int desth;
    int screen;
    Pixmap pixmap;
    Pixmap tpixmap;
    float f;
    float fw;
    float fh;
    XGCValues gcval;

    if (!image || !image->orig || !window->id) {

        return -1;
    }

    if (image->tpixmap) {
        imlib_free_pixmap_and_mask(image->tpixmap);
    }
    destw = w;
    desth = h;
#if 0
    destw = desth = 0;
    f = 0.0;
    fw = (float)w / (float)image->origw;
    fh = (float)h / (float)image->origh;
    f = min2(fw, fh);
    destw = (int)(f * (float)image->origw);
    desth = (int)(f * (float)image->origh);
#endif
//    R_free_image_imlib2(image);
    R_free_thumb_imlib2(image);
    imlib_context_set_drawable(window->id);
    imlib_context_set_image(image->orig);
    imlib_context_set_mask(image->mask);
//    imlib_context_set_blend(image->flags & R_IMAGE_BLEND_FLAG);
    imlib_image_set_has_alpha(1);
    imlib_context_set_blend(1);
    image->timg = imlib_create_cropped_scaled_image(0,
                                                    0,
                                                    image->origw,
                                                    image->origh,
                                                    destw,
                                                    desth);
    if (!image->timg) {

        return -1;
    }
    imlib_context_set_image(image->timg);
    imlib_render_pixmaps_for_whole_image_at_size(&image->tpixmap,
                                                 &image->tmask,
                                                 destw,
                                                 desth);
    if (image->xpixmap) {
        XFreePixmap(window->app->display,
                    image->xpixmap);
        image->xpixmap = None;
    }
    if (!_thumbgc) {
        screen = DefaultScreen(window->app->display);
        gcval.foreground = BlackPixel(window->app->display,
                                      screen);
        _thumbgc = XCreateGC(window->app->display,
                             window->id,
                             GCForeground,
                             &gcval);
        if (!_thumbgc) {
            
            return FALSE;
        }
        
    }
    pixmap = XCreatePixmap(window->app->display,
                           window->id,
                           w,
                           h,
                           window->app->depth);
    tpixmap = image->tpixmap;
    XFillRectangle(window->app->display,
                   pixmap,
                   _thumbgc,
                   0,
                   0,
                   w,
                   h);
    imlib_context_set_drawable(pixmap);
    imlib_context_set_mask(image->tmask);
    imlib_render_image_on_drawable(0, 0);
#if 0
    XCopyArea(window->app->display,
              tpixmap,
              pixmap,
              _thumbgc,
              0,
              0,
              destw,
              desth,
              (w - destw) / 2,
              (h - desth) / 2);
#endif
    imlib_free_pixmap_and_mask(tpixmap);
#if 0
    image->tpixmap = None;
    image->tmask = None;
#endif
    image->xpixmap = pixmap;
    image->w = destw;
    image->h = desth;

    return TRUE;
}

void
R_set_background_imlib2(struct R_image *image,
                        struct R_window *window,
                        int w,
                        int h,
                        int flags)
{
    Pixmap pixmap;

    if (!image->orig) {

        return;
    }
    pixmap = image->pixmap;
    if (!pixmap) {
        pixmap = image->xpixmap;
    }
    if (!pixmap) {
        R_render_image_imlib2(image,
                              window,
                              w,
                              h,
                              flags);
        pixmap = image->pixmap;
    }
    if (pixmap) {
        XSetWindowBackgroundPixmap(window->app->display,
                                   window->id,
                                   pixmap);
        XClearWindow(window->app->display, window->id);
        imlib_free_pixmap_and_mask(pixmap);
        if (flags & R_IMAGE_STATIC_FLAG) {
            image->flags |= R_IMAGE_STATIC_FLAG;
        }
        R_free_image_imlib2(image);
    }

    return;
}

void
R_set_thumb_imlib2(struct R_image *image,
                   struct R_window *window,
                   int w,
                   int h,
                   int flags)
{
    Pixmap pixmap;

    pixmap = image->xpixmap;
    if (!pixmap) {
        pixmap = image->tpixmap;
    }
    if (!pixmap) {
        R_render_thumb_imlib2(image,
                              window,
                              w,
                              h,
                              flags);
        pixmap = image->tpixmap;
    }
    if (pixmap) {
        XSetWindowBackgroundPixmap(image->app->display,
                                   window->id,
                                   pixmap);
        XClearWindow(image->app->display, window->id);
        XSync(window->app->display, False);
    }
    R_free_image_imlib2(image);

    return;
}


void
R_set_image_border_imlib2(struct R_image *image,
                          int top,
                          int bottom,
                          int left,
                          int right)
{
    Imlib_Border border;

    border.top = top;
    border.bottom = bottom;
    border.left = left;
    border.right = right;
#if 0
    imlib_context_set_image(image->img);
#endif
    imlib_context_set_image(image->orig);
    imlib_image_set_border(&border);

    return;
}

Imlib_Font
R_load_font_imlib2(const char *str)
{
    Imlib_Font font;

//    fprintf(stderr, "font: %s\n", str);
    font = imlib_load_font(str);

    return font;
}

void
R_draw_text_imlib2(struct R_image *image,
                   const char *str,
                   R_argb32_t color,
                   Imlib_Font font,
                   int x,
                   int y)
{
    imlib_context_set_image(image->img);
    imlib_context_set_color(R_ARGB32_RED(color),
                            R_ARGB32_GREEN(color),
                            R_ARGB32_BLUE(color),
                            R_ARGB32_ALPHA(color));
    imlib_text_draw(x, y, str);

    return;
}

#endif /* USE_IMLIB2 */

