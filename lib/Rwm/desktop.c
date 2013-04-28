/*
 * desktop.c - Resurrection X11 window manager desktop routines.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#define FADEHACKS 0

#include <Resurrection/Resurrection.h>

void Rwm_desktop_leavenotify_handler(void *arg,
                                     XEvent *event);
void Rwm_set_desktop_backgrounds(struct R_app *app);

#if (FADEHACKS)
void Rwm_fade_desktop_in(void *arg, XEvent *event);
#endif

const char *Rwm_desktop_image_names[RWM_DESKTOPS]
= {
    RESURRECTION_IMAGE_SEARCH_PATH "background/sword.png",
    RESURRECTION_IMAGE_SEARCH_PATH "background/background.jpg",
    RESURRECTION_IMAGE_SEARCH_PATH "background/cavefire.jpg",
    RESURRECTION_IMAGE_SEARCH_PATH "background/redmetal.jpg"
};

const int Rwm_desktop_image_flags[RWM_DESKTOPS]
= {
    R_IMAGE_FIT_FLAG,
    R_IMAGE_FIT_FLAG,
    R_IMAGE_FIT_FLAG,
    0
};

#if (USE_IMLIB2)
struct R_image Rwm_desktop_images[RWM_DESKTOPS];
#endif
extern struct R_window **Rwm_desktop_windows;

int
Rwm_init_desktops(struct R_app *app)
{
    struct R_wm *wm;
    struct R_window **desktops;
    struct R_window *desktop;
    int i;
#if (FADEHACKS)
    XEvent ev;
#endif

    wm = app->client;
    desktops = calloc(sizeof(struct R_window *),
                      RWM_DESKTOPS);
    Rwm_desktop_windows = desktops;
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
    if (!Rwm_init_desktop_images(app)) {

        return FALSE;
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
    Rwm_set_desktop_cursors(app);
//    Rwm_set_desktop_backgrounds(app);
    i = RWM_DESKTOPS;
    while (i) {
        i--;
#if (FADEHACKS)
        if (!i) {
            R_set_window_event_handler(desktops[i], Expose,
                                       Rwm_fade_desktop_in);
        }
#else
//        R_free_image_imlib2(&Rwm_desktop_images[i]);
#endif
        R_map_window_raised(desktops[i]);
#if 0
        Rwm_map_desktop(app, i);
#endif
    }
    Rwm_set_desktop_backgrounds(app);
    wm->desktop = wm->desktops[0];

    return TRUE;
}


#if (FADEHACKS)
#if (USE_IMLIB2)
void
Rwm_fade_desktop_in(void *arg, XEvent *event)
{
    struct R_wm *wm;
    struct R_window *desktop;
    struct R_image *image;
    Imlib_Image img;
    Pixmap pmap;
    Pixmap mask;
    Imlib_Color_Modifier colormod;
    double brite;
    int i;

    if (event->xexpose.count) {

        return;
    }
    desktop = arg;
    wm = desktop->app->client;
    colormod = imlib_create_color_modifier();
    image = &Rwm_desktop_images[0];
    if (colormod && R_load_image_imlib2(desktop->app, Rwm_desktop_image_names[0], image)) {
        imlib_context_set_drawable(desktop->id);
        imlib_context_set_image(image->orig);
        imlib_context_set_color_modifier(colormod);
        img = imlib_create_cropped_scaled_image(0,
                                                0,
                                                desktop->image->origw,
                                                desktop->image->origh,
                                                desktop->w,
                                                desktop->h);
        imlib_context_set_image(img);
        for (i = 0 ; i <= 256 ; i++) {
            brite = i;
            imlib_modify_color_modifier_brightness(brite);
            imlib_render_pixmaps_for_whole_image_at_size(&pmap, &mask,
                                                         desktop->w,
                                                         desktop->h);
            XSetWindowBackgroundPixmap(desktop->app->display,
                                       desktop->id,
                                       pmap);
            imlib_free_pixmap_and_mask(pmap);
            XClearWindow(desktop->app->display, desktop->id);
            XSync(desktop->app->display, False);
        }
        imlib_free_image();
    }
    XSelectInput(desktop->app->display, desktop->id, NoEventMask);
    XLowerWindow(desktop->app->display, desktop->id);
    XSync(desktop->app->display, False);

    return;
}
#endif
#endif

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
    if (id == 0) {
        R_add_window_events(desktop,
                            ExposureMask);
    }
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
    struct R_wm *wm;
    struct R_window *root;
    struct R_window *desktop;
    struct R_image *image;
    int i;

    wm = app->client;
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

    return TRUE;
}

void
Rwm_set_desktop_cursors(struct R_app *app)
{
    struct R_wm *wm;
    struct R_window *desktop;
    Cursor cursor;
    int i;

    wm = app->client;
    cursor = XCreateFontCursor(app->display, XC_left_ptr);
    for (i = 0 ; i < RWM_DESKTOPS ; i++) {
        desktop = wm->desktops[i];
        XDefineCursor(app->display, desktop->id, cursor);
    }

    return;
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
#if (FADEHACKS)
    for (i = 1 ; i < RWM_DESKTOPS ; i++) {
#else
    for (i = 0 ; i < RWM_DESKTOPS ; i++) {
#endif
        desktop = wm->desktops[i];
        desktop->image = &Rwm_desktop_images[i];
        if (Rwm_desktop_image_flags[i]) {
            R_set_background_imlib2(desktop->image,
                                    desktop,
#if 0
                                    R_FIT_IMAGE,
                                    R_FIT_IMAGE,
                                    root->w,
                                    root->h,
#endif
                                    R_TILE_IMAGE,
                                    R_TILE_IMAGE,
                                    Rwm_desktop_image_flags[i]);
        } else {
            R_set_background_imlib2(desktop->image,
                                    desktop,
                                    R_TILE_IMAGE,
                                    R_TILE_IMAGE,
                                    R_IMAGE_FIT_FLAG);
        }
        R_render_thumb_imlib2(desktop->image,
                              desktop,
                              RWM_PAGER_THUMB_WIDTH,
                              RWM_PAGER_THUMB_HEIGHT,
                              R_IMAGE_FIT_FLAG);
#endif
//        R_destroy_image_imlib2(desktop->image);
//        R_free_image_imlib2(desktop->image);
    }
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

void Rwm_desktop_leavenotify_handler(void *arg,
                                     XEvent *event)
{
    struct R_window *desktop;
    struct R_window *window;

    desktop = arg;
    while (XCheckTypedEvent(desktop->app->display,
                            EnterNotify,
                            event)) {
        ;
    }
    window = R_find_window(event->xcrossing.window);
    if (window && window->child) {
        Rwm_set_input_focus(window->child, CurrentTime);
    }

    return;
}
