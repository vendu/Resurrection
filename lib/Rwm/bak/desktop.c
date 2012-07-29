/*
 * desktop.c - Resurrection X11 window manager desktop routines.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

void Rwm_set_desktop_backgrounds(struct R_app *app);

const char *Rwm_desktop_image_names[RWM_DESKTOPS] = {
    RESURRECTION_IMAGE_SEARCH_PATH "background/sword.jpg",
    RESURRECTION_IMAGE_SEARCH_PATH "background/background.jpg",
    RESURRECTION_IMAGE_SEARCH_PATH "background/cavefire.jpg",
    RESURRECTION_IMAGE_SEARCH_PATH "background/wasteland.jpg"
};

#if (USE_IMLIB2)
struct R_image Rwm_desktop_images[RWM_DESKTOPS];
#endif

int
Rwm_init_desktops(struct R_app *app)
{
    struct R_wm *wm;
    struct R_window **desktops;
    struct R_window *desktop;
    int i;

    wm = app->client;
    if (!Rwm_init_desktop_images(app)) {

        return FALSE;
    }
    desktops = calloc(sizeof(struct R_window *),
                      RWM_DESKTOPS);
    if (desktops == NULL) {

        return FALSE;
    }
    wm->desktops = desktops;
    i = 0;
    while (i < RWM_DESKTOPS) {
        desktop = R_alloc_window();
        desktop->app = app;
        wm->desktops[i] = desktop;
        i++;
    }
    i = 0;
    while (i < RWM_DESKTOPS) {
        if (!Rwm_init_desktop(app,
                              i)) {

            return FALSE;
        }
        i++;
    }
    i = 0;
    while (i < RWM_DESKTOPS) {
        R_add_window(desktops[i]);
        i++;
    }
    Rwm_set_desktop_backgrounds(app);
    i = RWM_DESKTOPS;
    while (i) {
        i--;
        R_map_window_raised(desktops[i]);
#if 0
        Rwm_map_desktop(app, i);
#endif
    }
    wm->desktop = wm->desktops[0];

    return TRUE;
}

int
Rwm_init_desktop(struct R_app *app,
                 int id)
{
    struct R_wm *wm;
    struct R_window *desktop;
    R_eventhandler_t **eventhandlers;

    wm = app->client;
    desktop = Rwm_create_desktop_window(app);
    if (!desktop) {

        return FALSE;
    }
    eventhandlers = calloc(sizeof(R_eventhandler_t *),
                           LASTEvent);
    if (!eventhandlers) {
        
        return FALSE;
    }
    desktop->eventhandlers = eventhandlers;
    desktop->nevents = LASTEvent;
    R_set_window_events(desktop,
                        KeyPressMask);
#if 0
    R_set_window_events(desktop,
                        SubstructureNotifyMask
                        | SubstructureRedirectMask
                        | ResizeRedirectMask);
#endif
#if (USE_IMLIB2)
    desktop->image = &Rwm_desktop_images[id];
#endif
    wm->desktops[id] = desktop;

    return TRUE;
}

int
Rwm_create_desktop_window(struct R_app *app)
{
    struct R_window *root;
    struct R_window *desktop;

    root = app->window;
    desktop = R_create_window(app,
                              root,
                              R_WINDOW_OVERRIDE_REDIRECT_FLAG);
    if (desktop == NULL) {
        
        return desktop;
    }
    RWM_SET_WINDOW_SIZE(desktop, root->w, root->h);
    Rwm_resize_window(desktop,
                      root->w,
                      root->h);
    desktop->typeflags = R_WINDOW_DESKTOP_FLAG;
    Rwm_resize_window(desktop,
                      app->window->w,
                      app->window->h);

    return desktop;
}

int
Rwm_init_desktop_images(struct R_app *app)
{
    struct R_window *root;
    struct R_window *desktop;
    struct R_image *image;
    int i;

    root = app->window;
#if (USE_IMLIB2)
    for (i = 0 ; i < RWM_DESKTOPS ; i++) {
        if (!R_load_image_imlib2(app,
                                 Rwm_desktop_image_names[i],
                                 &Rwm_desktop_images[i])) {
            
            return FALSE;
        }
    }
#endif
#if 0
    for (i = 0 ; i < RWM_DESKTOPS ; i++) {
        desktop = wm->desktops[i];
        desktop->image = &Rwm_desktop_images[i];
        R_set_background_imlib2(desktop->image,
                                desktop,
                                root->w,
                                root->h,
                                0);
    }
#endif

    return TRUE;
}

void
Rwm_set_desktop_backgrounds(struct R_app *app)
{
    struct R_wm *wm;
    struct R_window *root;
    struct R_window *desktop;
    int i;

    wm = app->client;
    root = app->window;
#if (USE_IMLIB2)
    for (i = 0 ; i < RWM_DESKTOPS ; i++) {
        desktop = wm->desktops[i];
        desktop->image = &Rwm_desktop_images[i];
        R_set_background_imlib2(desktop->image,
                                desktop,
                                root->w,
                                root->h,
                                R_IMAGE_STATIC_FLAG);
//        R_destroy_image_imlib2(desktop->image);
    }
#endif
}

#if 0
void
Rwm_map_desktop(struct R_app *app,
                int id)
{
    struct R_wm *wm;

    struct R_window *desktop;

    wm = app->client;
    desktop = wm->desktops[id];
    R_map_window_raised(desktop);

    return;
}
#endif

void
Rwm_switch_desktop(struct R_app *app,
                   struct R_window *desktop)
{
    struct R_wm *wm;

    wm = app->client;
    wm->desktop = desktop;
    Rwm_reparent_window(wm->menu,
                        wm->desktop,
                        0,
                        0);
    Rwm_resize_window(wm->pager,
                      RWM_MENU_ITEM_WIDTH + 2 * RWM_MENU_RULER_WIDTH,
                      RWM_MENU_ITEM_HEIGHT);
    Rwm_reparent_window(wm->pager,
                        wm->desktop,
                        RWM_MENU_ITEM_WIDTH + 2 * RWM_MENU_RULER_WIDTH,
                        0);
    Rwm_reparent_window(wm->clock,
                        wm->desktop,
                        2 * RWM_MENU_ITEM_WIDTH + 4 * RWM_MENU_RULER_WIDTH,
                        0);
//    R_map_subwindows(desktop);
    R_map_window_raised(desktop);

    return;
}

