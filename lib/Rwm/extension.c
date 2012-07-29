/*
 * extension.c - Resurrection window manager X11 extension interface.
 * Copyright (C) 2008 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

void
Rwm_check_composite(struct R_app *app)
{
    struct R_wm *wm;
    int major = 0;
    int minor = 2;

    wm = app->client;
    XCompositeQueryExtension(app->display,
                             &major,
                             &minor);
    if (major > 0 || minor >= 2) {
        wm->extflags |= RWM_COMPOSITE_EXTENSION_FLAG;
    }

    return;
}

void
Rwm_check_damage(struct R_app *app)
{
    struct R_wm *wm;
    int event = -1;
    int error = -1;

    wm = app->client;
    XDamageQueryExtension(app->display,
                          &event,
                          &error);
    if (event >= 0) {
        wm->extflags = RWM_DAMAGE_EXTENSION_FLAG;
        app->damagenotify = event;
    }

    return;
}

void
Rwm_redirect_composite_subwindows(struct R_app *app)
{
    int i;

    for (i = 0 ; i < ScreenCount(app->display) ; i++) {
        XCompositeRedirectSubwindows(app->display,
                                     RootWindow(app->display,
                                                i),
                                     CompositeRedirectAutomatic);
    }

    return;
}

XRenderPictFormat *
Rwm_probe_picture_format(struct R_window *window)
{
    XRenderPictFormat *fmt;
    XWindowAttributes  attr;

    if (!window->id) {

        return;
    }

    XGetWindowAttributes(window->app->display,
                         window->id,
                         &attr);
    fmt = XRenderFindVisualFormat(window->app->display,
                                  attr.visual);

    return fmt;
}

int
Rwm_create_window_picture(struct R_window *window)
{
    int retval = FALSE;
    struct R_app *app;
    struct R_wm *wm;
    struct R_picture *picture;
    Picture pict;
    XRenderPictureAttributes attr;
    XserverRegion reg;

    app = window->app;
    wm = app->client;
    attr.subwindow_mode = IncludeInferiors;
    _ignbadwindow = 1;
    pict = XRenderCreatePicture(app->display,
                                window->id,
                                wm->pictformat,
                                CPSubwindowMode,
                                &attr);
    XSync(app->display,
          False);
    _ignbadwindow = 0;
    if (pict) {
        picture = R_alloc_picture();
        if (picture) {
            reg = XFixesCreateRegionFromWindow(app->display,
                                               window->id,
                                               WindowRegionBounding);
            XFixesTranslateRegion(app->display,
                                  reg,
                                  -(window->x),
                                  -(window->y));
            XFixesSetPictureClipRegion(app->display,
                                       pict,
                                       0,
                                       0,
                                       reg);
            XFixesDestroyRegion(app->display,
                                reg);
            fmt = Rwm_probe_window_format(window);
            if (fmt->type == PictTypeDirect && fmt->direct.alphaMask) {
                picture->flags |= R_PICTURE_ALPHA_FLAG;
            }
            picture->pict = pict;

            retval = TRUE;
        }
    }
    if (window->picture) {
        R_free_object(window->picture);
    }
    window->picture = picture;

    return retval;
}

void
Rwm_render_picture(struct R_app *app,
                   struct R_picture *src,
                   struct R_window *window
                   int x,
                   int y,
                   int w,
                   int h)
{
    int op = PictOpSrc;

    if (src->flags & R_PICTURE_ALPHA_FLAG) {
        op = PictOpOver;
    }
    if (window->picture) {
        R_free_object(window->picture);
    }
    if (Rwm_create_window_picture(window)) {
        XRenderComposite(app->display,
                         op,
                         src->pict,
                         window->pict,
                         0,
                         0,
                         0,
                         0,
                         x,
                         y,
                         w,
                         h);
    }
}

