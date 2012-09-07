/*
 * x11.c - Resurrection X11 window interface.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

void
R_free_window(struct R_window *window)
{
    XEvent event;
    struct R_image *image;
    int i;
    struct R_binding *binding1;
    struct R_binding *binding2;

    if (window) {
        while (XCheckWindowEvent(window->app->display,
                                 window->id,
                                 ~0L,
                                 &event)) {
            ;
        }
#if 0
        R_add_window_events(window,
                            NoEventMask);
#endif
        R_remove_window(window);
        window->id = None;
#if (USE_IMLIB2)
        if (window->images) {
            for (i = 0 ; i < R_WINDOW_STATES ; i++) {
                image = window->images[i];
                if (image) {
                    R_free_image_imlib2(image);
                }
            }
        }
#if 0
        } else {
            if (window->images) {
                for (i = 0 ; i < R_WINDOW_STATES ; i++) {
                    image = window->images[i];
                    if (image) {
                        R_free_image_imlib2(image);
                    }
                }
//                free(window->images[0]);
            }
        }
#endif
        for (i = 0 ; i < R_WINDOW_STATES ; i++) {
            window->images[i] = NULL;
        }
#endif
        if (!(window->sysflags & R_WINDOW_STATIC_EVENTS_FLAG)) {
            free(window->eventhandlers);
        }
        window->nevents = 0;
        window->eventhandlers = NULL;
        if (window->bindings) {
            free(window->bindings);
            window->bindings = NULL;
        }
        if (window->str) {
            free(window->str);
            window->str = NULL;
        }
        if (window->timestr) {
            /* statically allocated */
            window->timestr = NULL;
        }
        if (window->title) {
            free(window->title);
            window->title = NULL;
        }
        if (window->iconname) {
            free(window->iconname);
            window->iconname = NULL;
        }
        if (window->normalhints) {
            XFree(window->normalhints);
            window->normalhints = NULL;
        }
        if (window->sizehints) {
            XFree(window->sizehints);
            window->sizehints = NULL;
            }
        if (window->wmhints) {
            XFree(window->wmhints);
            window->wmhints = NULL;
        }
        if (window->classhint) {
            XFree(window->classhint);
            window->classhint = NULL;
        }
        free(window);
    }

    return;
}

void
R_add_window(struct R_window *window)
{
    struct R_window *hashwindow;
    int key;

    if (R_find_window(window->id)) {
        fprintf(stderr, "HASH: type == %x, parent->type == %x\n",
                window->typeflags, (window->parent) ? window->parent->typeflags : 0);
        fprintf(stderr, "window already hashed!\n");

//        abort();
        return;
    }

    key = R_HASH_WINDOW_ID(window->id);
    hashwindow = R_global.windowhash[key];
    window->prev = NULL;
    window->next = hashwindow;
    if (hashwindow) {
        hashwindow->prev = window;
    }
    R_global.windowhash[key] = window;

    return;
}

struct R_window *
R_find_window(Window id)
{
    struct R_window *window;
    int key;

    if (id == None) {

        return NULL;
    }
    key = R_HASH_WINDOW_ID(id);
    window = R_global.windowhash[key];
    while ((window) && window->id != id) {
        window = window->next;
    }

    return window;
}

struct R_window *
R_remove_window(struct R_window *window)
{
    struct R_window *hashwindow;
    int key;

    key = R_HASH_WINDOW_ID(window->id);
    hashwindow = R_global.windowhash[key];
    while (hashwindow) {
        if (hashwindow->id == window->id) {
            if (hashwindow->prev) {
                hashwindow->prev->next = hashwindow->next;
            } else {
                R_global.windowhash[key] = hashwindow->next;
            }
            if (hashwindow->next) {
                hashwindow->next->prev = hashwindow->prev;
            }

            return hashwindow;
        }
        hashwindow = hashwindow->next;
    }

    return NULL;
}

struct R_window *
R_create_window(struct R_app *app,
                struct R_window *parent,
                unsigned long flags)
{
    struct R_window *window;

    window = R_alloc_window();
    if (window) {
        R_init_window(app,
                      window,
                      parent,
                      flags);
        window->parent = parent;
    }

    return window;
}

void
R_init_window(struct R_app *app,
              struct R_window *window,
              struct R_window *parent,
              unsigned long flags)
{
    int i;
    struct R_image *images;
    R_eventhandler_t **eventhandlers;
    Window parentwin;
    Window win;
    int screen;
    int winw;
    int winh;
    XSetWindowAttributes attr;

    images = NULL;
    parentwin = None;
    if (flags & R_WINDOW_STATIC_IMAGES_FLAG) {
        window->sysflags |= R_WINDOW_STATIC_IMAGES_FLAG;
    } else {
#if (USE_IMLIB2)
        images = calloc(R_WINDOW_STATES,
                        sizeof(struct R_image));
        if (!images) {
            /* FIXME: don't fail softly, return error. */
            
            return;
        }
        for (i = 0 ; i < R_WINDOW_STATES ; i++) {
            window->images[i] = images;
            images++;
        }
    }
#endif
    window->str = NULL;
    if (!(flags & R_WINDOW_STATIC_EVENTS_FLAG)) {
        eventhandlers = calloc(LASTEvent,
                               sizeof(R_eventhandler_t *));
        if (!eventhandlers) {
            /* FIXME: don't fail softly, return error. */
            
            return;
        }
        window->eventhandlers = eventhandlers;
    } else {
        window->sysflags |= R_WINDOW_STATIC_EVENTS_FLAG;
    }
//    screen = DefaultScreen(app->display);
    screen = app->screen;
    memset(&attr, 0, sizeof(attr));
    attr.background_pixel = BlackPixel(app->display,
                                       screen);
    attr.colormap = app->colormap;
#if 0
    attr.save_under = (flags & R_WINDOW_SAVE_UNDER_FLAG);
    attr.backing_store = (flags & R_WINDOW_BACK_STORE_FLAG);
    attr.override_redirect = (flags & R_WINDOW_OVERRIDE_FLAG);
#endif
    if (flags & R_WINDOW_SAVE_UNDER_FLAG) {
        attr.save_under = True;
    }
    if (flags & R_WINDOW_BACK_STORE_FLAG) {
        attr.backing_store = True;
    }
    if (flags & R_WINDOW_OVERRIDE_REDIRECT_FLAG) {
        attr.override_redirect = True;
    }
    window->sysflags |= flags & R_WINDOW_SYS_MASK;
#if 0
    if (flags & R_WINDOW_TOPLEVEL_FLAG) {
        window->typeflags |= R_WINDOW_TOPLEVEL_FLAG;
    }
#endif
    if (flags & R_WINDOW_DO_NOT_PROPAGATE_FLAG) {
        attr.do_not_propagate_mask = ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask;
    }
    window->typeflags = 0;
    if (parent) {
        parentwin = parent->id;
    }
    winw = window->w;
    winh = window->h;
    if (!winw) {
        winw = 1;
    }
    if (!winh) {
        winh = 1;
    }
    if (!parent) {
        parentwin = RootWindow(app->display,
                               screen);
        fprintf(stderr, "PARENT: %lx (%lx)\n", (long)parentwin, screen);
    }
    win = XCreateWindow(app->display,
                        parentwin,
                        0,
                        0,
                        winw,
                        winh,
                        0,
                        app->depth,
                        InputOutput,
                        app->visual,
                        CWBackPixel
                        | CWColormap
                        | CWDontPropagate
                        | CWSaveUnder
                        | CWBackingStore
                        | CWOverrideRedirect,
                        &attr);
    window->id = win;
    window->app = app;
    window->nevents = LASTEvent;
#if (USE_IMLIB2)
    RWM_SET_WINDOW_IMAGE(window,
                         R_WINDOW_NORMAL_STATE);
#endif

#if 0
    R_add_window(window);
#endif
#if 0
    window->buttonactions = calloc(sizeof(R_buttonaction_t *),
                                   R_BUTTON_ACTIONS);
#endif

    return;
}

void
R_set_window_title(struct R_window *window, const char *title, int setwmname)
{
    size_t len;
#if (SUPPORT_TRUETYPE_IMLIB2)
    Imlib_Image textimage;
    int textw, texth;
#endif
    
    XTextProperty windowname;
    
    if (window == NULL
        || title == NULL) {
        
        return;
    }
    
    len = strlen(title);
    
    if (len) {
        if (window->title) {
            free(window->title);
        }
        if ((window->title = calloc(1, len + 1)) == NULL) {
            
            return;
        }
        
        strncpy(window->title, title, len);
        
        window->titlelen = len;
#if (SUPPORT_TRUETYPE_IMLIB2)
        if (window->im2font) {
            imlib_context_set_font(window->im2font);
            imlib_get_text_size(window->title, &textw, &texth);
            if (window->im2title) {
                imlib_context_set_image(window->im2title);
                imlib_free_image();
                window->im2title = NULL;
                window->im2titlew = 0;
                window->im2titleh = 0;
            }
            textimage = imlib_create_image(textw, texth);
            if (textimage) {
                imlib_context_set_image(textimage);
                imlib_image_set_has_alpha(1);
                imlib_image_clear();
//            imlib_context_set_color(204, 181, 59, 255);
                imlib_context_set_color(255, 255, 255, 255);
                imlib_text_draw(0, 0, window->title);
                window->im2title = textimage;
                window->im2titlew = textw;
                window->im2titleh = texth;
            }
        }
#endif
    }
    if (setwmname) {
        XStringListToTextProperty((char **)(&(window->title)), 1, &windowname);
        
        XSetWMName(window->app->display, window->id, &windowname);
        
        XFree(windowname.value);
    }
    
    return;
}

void
R_set_class_hint(struct R_window *window, const char *programname,
                 const char *classname)
{
    size_t len;

    if (!window->classhint) {
        window->classhint = XAllocClassHint();
    }
    if (programname) {
	len = strlen(programname);
	
	if (len) {
	    if ((window->classhint->res_name = calloc(1, len + 1)) == NULL) {
		
		return;
	    }
	    
	    strncpy(window->classhint->res_name, programname, len);
	}
    }

    if (classname) {
	len = strlen(classname);
	
	if (len) {
	    if ((window->classhint->res_class = calloc(1, len + 1)) == NULL) {
		
		return;
	    }
	    
	    strncpy(window->classhint->res_class, classname, len);
	}
    }

    XSetClassHint(window->app->display, window->id, window->classhint);

    return;
}

void
R_set_wm_hints(struct R_window *window,
               int input, int initialstate,
               Pixmap iconpixmap,
               struct R_window *iconwindow,
               int iconx, int icony,
               Pixmap iconmaskpixmap,
               XID windowgroup,
               int urgency)
{
    if (!window->wmhints) {
        window->wmhints = XAllocWMHints();
    }
    if (input > 0) {
	switch (input) {
	    case False:
		window->wmhints->input = False;

		break;
	    default:
		window->wmhints->input = True;

		break;
	}

	window->wmhints->flags |= InputHint;
    }

    switch (initialstate) {
        case WithdrawnState:
	    window->wmhints->initial_state = WithdrawnState;

	    window->wmhints->flags |= StateHint;

	    break;
        case NormalState:
	    window->wmhints->initial_state = NormalState;

	    window->wmhints->flags |= StateHint;

	    break;
        case IconicState:
	    window->wmhints->initial_state = IconicState;

	    window->wmhints->flags |= StateHint;

	    break;
        default:
	    break;
    }

    if (iconpixmap) {
	window->wmhints->icon_pixmap = iconpixmap;

	window->wmhints->flags |= IconPixmapHint;
#if (USE_IMLIB2)
    } else if ((iconwindow) &&
	       (iconwindow->images[R_WINDOW_BACKGROUND_IMAGE]->pixmap)) {
	window->wmhints->icon_pixmap =
	    iconwindow->images[R_WINDOW_BACKGROUND_IMAGE]->pixmap;

	window->wmhints->flags |= IconPixmapHint;
#endif
    }

    if ((iconwindow) && (iconwindow->id)) {
	window->wmhints->icon_window = iconwindow->id;

	window->wmhints->flags |= IconWindowHint;
    }

    if (iconx >= 0 && icony >= 0) {
	window->wmhints->icon_x = iconx;
	window->wmhints->icon_y = icony;

	window->wmhints->flags |= IconPositionHint;
    }

    if (iconmaskpixmap) {
	window->wmhints->icon_mask = iconmaskpixmap;

	window->wmhints->flags |= IconMaskHint;
#if (USE_IMLIB2)
    } else if ((iconwindow)
	       && (iconwindow->images[R_WINDOW_BACKGROUND_IMAGE]->mask)) {
	window->wmhints->icon_mask =
	    iconwindow->images[R_WINDOW_BACKGROUND_IMAGE]->mask;

	window->wmhints->flags |= IconMaskHint;
#endif
    }

    if (windowgroup) {
	window->wmhints->window_group = windowgroup;

	window->wmhints->flags |= WindowGroupHint;
    }

    if (urgency > 0) {
	switch (urgency) {
	    case False:
		window->wmhints->flags &= ~XUrgencyHint;

		break;
	    default:
		window->wmhints->flags |= XUrgencyHint;

		break;
	}
    }

    XSetWMHints(window->app->display, window->id, window->wmhints);
    
    return;
}

void
R_set_normal_hints(struct R_window *window,
                   int minwidth, int minheight,
                   int maxwidth, int maxheight,
                   int widthinc, int heightinc,
                   int minaspectnum, int minaspectdenom,
                   int maxaspectnum, int maxaspectdenom,
                   int basewidth, int baseheight, int gravity)
{
    if (!window->normalhints) {
        window->normalhints = XAllocSizeHints();
    }
    window->normalhints->flags |= PPosition | PSize;

    if (minwidth >= 0 && minheight >= 0) {
        window->normalhints->min_width = minwidth;
        window->normalhints->min_height = minheight;
        window->normalhints->flags |= PMinSize;
    }

    if (maxwidth >= 0 && maxheight >= 0) {
        window->normalhints->max_width = maxwidth;
        window->normalhints->max_height = maxheight;
        window->normalhints->flags |= PMaxSize;
    }

    if (widthinc >= 0 && heightinc >= 0) {
        window->normalhints->width_inc = widthinc;
        window->normalhints->height_inc = heightinc;
        window->normalhints->flags |= PResizeInc;
    }

    if (minaspectnum > 0 && minaspectdenom > 0
        && maxaspectnum > 0 && maxaspectdenom > 0) {
        window->normalhints->min_aspect.x = minaspectnum;
        window->normalhints->min_aspect.y = minaspectdenom;
        window->normalhints->max_aspect.x = maxaspectnum;
        window->normalhints->max_aspect.y = maxaspectdenom;
        window->normalhints->flags |= PAspect;
    }

    if (basewidth >= 0 && baseheight >= 0) {
        window->normalhints->base_width = basewidth;
        window->normalhints->base_height = baseheight;
        window->normalhints->flags |= PBaseSize;
    }

    switch (gravity) {
        case UnmapGravity:
#if 0
            if (window->type == WINDOW_TOPLEVEL) {
                break;
            }
#endif
            /* FALLTHROUGH */
        case NorthWestGravity:
        case NorthGravity:
        case NorthEastGravity:
        case WestGravity:
        case CenterGravity:
        case EastGravity:
        case SouthWestGravity:
        case SouthGravity:
        case SouthEastGravity:
        case StaticGravity:
            window->normalhints->win_gravity = gravity;

            window->normalhints->flags |= PWinGravity;

            break;
        default:

            break;
    }

    XSetWMNormalHints(window->app->display, window->id, window->normalhints);

    return;
}

int
R_set_window_event_handler(struct R_window *window,
                           int event,
                           R_eventhandler_t *handler)
{
    R_eventhandler_t **eventhandlers;

    if (event >= window->nevents) {
        eventhandlers = realloc(window->eventhandlers,
                                (event + 1) * sizeof(R_eventhandler_t *));
        if (eventhandlers == NULL) {

            return FALSE;
        }
        window->eventhandlers = eventhandlers;
    }
    window->eventhandlers[event] = handler;

    return TRUE;
}

#if 0
void
R_set_window_button_action(struct R_window *window,
                           int action,
                           R_buttonaction_t *handler)
{
    window->buttonactions[action] = handler;
}
#endif

void
R_set_window_text_property(struct R_window *window,
                           const char *propertyname,
                           const R_text_t *value)
{
    XTextProperty textproperty;
    Atom atom;
    
    if (window == NULL
	|| propertyname == NULL
	|| propertyname[0] == '\0') {
        
	return;
    }
    
    if (value == NULL) {
	if ((atom = XInternAtom(window->app->display, propertyname, True))
	    == None) {
            
	    return;
	}
        
	XDeleteProperty(window->app->display, window->id, atom);
    } else {
	if ((atom = XInternAtom(window->app->display, propertyname, True))
	    != None) {
            
	    XDeleteProperty(window->app->display, window->id, atom);
	}

	atom = XInternAtom(window->app->display, propertyname, False);

	textproperty.value = (unsigned char *)value;
	textproperty.encoding = XA_STRING;
	textproperty.format = 8;
	textproperty.nitems = strlen(value);

	XSetTextProperty(window->app->display, window->id,
			 &textproperty, atom);
    }

    return;
}

void
R_set_window_normal_hints(struct R_window *window,
                          int minwidth, int minheight,
                          int maxwidth, int maxheight,
                          int widthinc, int heightinc,
                          int minaspectnum, int minaspectdenom,
                          int maxaspectnum, int maxaspectdenom,
                          int basewidth, int baseheight, int gravity)
{
    if (window == NULL
	|| window->id == None) {

	return;
    }

    window->normalhints->flags |= PPosition | PSize;

    if (minwidth >= 0 && minheight >= 0) {
	window->normalhints->min_width = minwidth;
	window->normalhints->min_height = minheight;
	window->normalhints->flags |= PMinSize;
    }

    if (maxwidth >= 0 && maxheight >= 0) {
	window->normalhints->max_width = maxwidth;
	window->normalhints->max_height = maxheight;
	window->normalhints->flags |= PMaxSize;
    }

    if (widthinc >= 0 && heightinc >= 0) {
	window->normalhints->width_inc = widthinc;
	window->normalhints->height_inc = heightinc;
	window->normalhints->flags |= PResizeInc;
    }

    if (minaspectnum > 0 && minaspectdenom > 0
	&& maxaspectnum > 0 && maxaspectdenom > 0) {
	window->normalhints->min_aspect.x = minaspectnum;
	window->normalhints->min_aspect.y = minaspectdenom;
	window->normalhints->max_aspect.x = maxaspectnum;
	window->normalhints->max_aspect.y = maxaspectdenom;
	window->normalhints->flags |= PAspect;
    }

    if (basewidth >= 0 && baseheight >= 0) {
	window->normalhints->base_width = basewidth;
	window->normalhints->base_height = baseheight;
	window->normalhints->flags |= PBaseSize;
    }

    switch (gravity) {
        case UnmapGravity:
	    if (window->typeflags & R_WINDOW_TOPLEVEL_FLAG) {

		break;
	    }
	    /* FALLTHROUGH */
        case NorthWestGravity:
        case NorthGravity:
        case NorthEastGravity:
        case WestGravity:
        case CenterGravity:
        case EastGravity:
        case SouthWestGravity:
        case SouthGravity:
        case SouthEastGravity:
        case StaticGravity:
	    window->normalhints->win_gravity = gravity;

	    window->normalhints->flags |= PWinGravity;

	    break;
        default:

	    break;
    }

    XSetWMNormalHints(window->app->display, window->id, window->normalhints);

    return;
}

void
R_set_window_wm_hints(struct R_window *window,
                      int input, int initialstate,
                      Pixmap iconpixmap,
                      struct R_window *iconwindow,
                      int iconx, int icony,
                      Pixmap iconmaskpixmap,
                      XID windowgroup,
                      int urgency)
{
    if (window == NULL
	|| window->id == None) {

	return;
    }

    if (input > 0) {
	switch (input) {
	    case False:
		window->wmhints->input = False;

		break;
	    default:
		window->wmhints->input = True;

		break;
	}

	window->wmhints->flags |= InputHint;
    }

    switch (initialstate) {
        case WithdrawnState:
	    window->wmhints->initial_state = WithdrawnState;

	    window->wmhints->flags |= StateHint;

	    break;
        case NormalState:
	    window->wmhints->initial_state = NormalState;

	    window->wmhints->flags |= StateHint;

	    break;
        case IconicState:
	    window->wmhints->initial_state = IconicState;

	    window->wmhints->flags |= StateHint;

	    break;
        default:
	    break;
    }

    if (iconpixmap) {
	window->wmhints->icon_pixmap = iconpixmap;

	window->wmhints->flags |= IconPixmapHint;
#if (USE_IMLIB2)
    } else if ((iconwindow) &&
	       (iconwindow->images[R_WINDOW_BACKGROUND_IMAGE]->pixmap)) {
	window->wmhints->icon_pixmap =
	    iconwindow->images[R_WINDOW_BACKGROUND_IMAGE]->pixmap;

	window->wmhints->flags |= IconPixmapHint;
#endif
    }

    if ((iconwindow) && (iconwindow->id)) {
	window->wmhints->icon_window = iconwindow->id;

	window->wmhints->flags |= IconWindowHint;
    }

    if (iconx >= 0 && icony >= 0) {
	window->wmhints->icon_x = iconx;
	window->wmhints->icon_y = icony;

	window->wmhints->flags |= IconPositionHint;
    }

    if (iconmaskpixmap) {
	window->wmhints->icon_mask = iconmaskpixmap;

	window->wmhints->flags |= IconMaskHint;
#if (USE_IMLIB2)
    } else if ((iconwindow)
	       && (iconwindow->images[R_WINDOW_BACKGROUND_IMAGE]->mask)) {
	window->wmhints->icon_mask =
	    iconwindow->images[R_WINDOW_BACKGROUND_IMAGE]->mask;

	window->wmhints->flags |= IconMaskHint;
#endif
    }

    if (windowgroup) {
	window->wmhints->window_group = windowgroup;

	window->wmhints->flags |= WindowGroupHint;
    }

    if (urgency > 0) {
	switch (urgency) {
	    case False:
		window->wmhints->flags &= ~XUrgencyHint;

		break;
	    default:
		window->wmhints->flags |= XUrgencyHint;

		break;
	}
    }

    XSetWMHints(window->app->display, window->id, window->wmhints);
    
    return;
}

void
R_set_window_icon_name(struct R_window *window, const R_text_t *name)
{
    size_t len;
    XTextProperty iconname;

    len = strlen(name);

    if (len) {
	if ((window->iconname = calloc(1, len + 1)) == NULL) {

	    return;
	}

	strncpy(window->iconname, name, len);
    }

    XStringListToTextProperty((char **)(&(window->iconname)), 1, &iconname);

    XSetWMIconName(window->app->display, window->id, &iconname);

    XFree(iconname.value);

    return;
}

void
R_set_window_urgency_hint(struct R_window *window, int value)
{
    if (window == NULL) {

	return;
    }

    switch (value) {
        case False:
	    window->wmhints->flags &= ~XUrgencyHint;

	    break;
        default:
	    window->wmhints->flags |= XUrgencyHint;

	    break;
    }

    XSetWMHints(window->app->display, window->id, window->wmhints);

    return;
}

void
R_set_window_override_redirect(struct R_window *window, int value)
{
    if (window == NULL) {

	return;
    }

    switch (value) {
        case False:
	    window->attributes.override_redirect = False;
	    
	    break;
        default:
	    window->attributes.override_redirect = True;

	    break;
    }

    XChangeWindowAttributes(window->app->display, window->id,
			    CWOverrideRedirect, &(window->attributes));

    return;
}

void
R_set_window_save_under(struct R_window *window, int value)
{
    if (window == NULL) {

	return;
    }

    switch (value) {
        case False:
	    window->attributes.save_under = False;

	    break;
        default:
	    window->attributes.save_under = True;

	    break;
    }

    XChangeWindowAttributes(window->app->display, window->id,
			    CWSaveUnder, &(window->attributes));

    return;
}

void
R_set_text_property(struct R_window *window, const char *propertyname,
                    const R_text_t *value)
{
    XTextProperty textproperty;
    Atom atom;

    if (window == NULL
        || propertyname == NULL
        || propertyname[0] == '\0') {

        return;
    }

    if (value == NULL) {
        if ((atom = XInternAtom(window->app->display, propertyname, True))
            == None) {

            return;
        }

        XDeleteProperty(window->app->display, window->id, atom);
    } else {
        if ((atom = XInternAtom(window->app->display, propertyname, True))
            != None) {

            XDeleteProperty(window->app->display, window->id, atom);
        }

        atom = XInternAtom(window->app->display, propertyname, False);

        textproperty.value = (unsigned char *)value;
        textproperty.encoding = XA_STRING;
        textproperty.format = 8;
        textproperty.nitems = strlen(value);

        XSetTextProperty(window->app->display, window->id,
                         &textproperty, atom);
    }

    return;
}

long
_R_set_window_events(struct R_window *window,
                     long mask)
{
    XSelectInput(window->app->display,
                 window->id,
                 mask);
    window->eventmask = mask;

    return mask;
}

long
_R_add_window_events(struct R_window *window,
                     long mask)
{
    long eventmask;

    eventmask = window->eventmask | mask;
    XSelectInput(window->app->display,
                 window->id,
                 eventmask);
    window->eventmask = eventmask;

    return eventmask;
}

long
_R_remove_window_events(struct R_window *window,
                        long mask)
{
    long eventmask;

    eventmask = window->eventmask;
    if (mask != None) {
        eventmask &= ~mask;
        XSelectInput(window->app->display,
                     window->id,
                     eventmask);
        window->eventmask = eventmask;
    }

    return eventmask;
}

void
_R_add_save_window(struct R_window *window)
{
    _ignbadwindow = 1;
    XAddToSaveSet(window->app->display,
                  window->id);
    XSync(window->app->display, False);
    _ignbadwindow = 0;

    return;
}

void
_R_remove_save_window(struct R_window *window)
{
    _ignbadwindow = 1;
    XRemoveFromSaveSet(window->app->display,
                       window->id);
    XSync(window->app->display, False);
    _ignbadwindow = 0;

    return;
}

void
_R_reparent_window(struct R_window *window,
                   struct R_window *parent)
{
    XReparentWindow(window->app->display,
                    window->id,
                    parent->id,
                    window->x,
                    window->y);

    return;
}

void
_R_map_window(struct R_window *window)
{
    XMapWindow(window->app->display,
               window->id);

    return;
}

void
_R_map_subwindows(struct R_window *window)
{
    XMapSubwindows(window->app->display,
                   window->id);

    return;
}

void
_R_map_window_raised(struct R_window *window)
{
    XMapRaised(window->app->display,
               window->id);

    return;
}

void
_R_unmap_window(struct R_window *window)
{
    XUnmapWindow(window->app->display,
                 window->id);

    return;
}

void
_R_iconify_window(struct R_window *window)
{
    XIconifyWindow(window->app->display, window->id,
		   window->app->screen);

    return;
}

void
_R_withdraw_window(struct R_window *window)
{
    XWithdrawWindow(window->app->display, window->id,
		    window->app->screen);

    return;
}

void
_R_raise_window(struct R_window *window)
{
    XRaiseWindow(window->app->display,
                 window->id);

    return;
}

void
_R_lower_window(struct R_window *window)
{
    XLowerWindow(window->app->display,
                 window->id);

    return;
}

void
_R_clear_window(struct R_window *window)
{
    XClearWindow(window->app->display,
                 window->id);

    return;
}

void
_R_clear_area(struct R_window *window, int x, int y, int w, int h, int expose)
{
    XClearArea(window->app->display,
               window->id,
               x,
               y,
               w,
               h,
               expose);
}

void
_R_move_window(struct R_window *window,
               int x,
               int y)
{
    XMoveWindow(window->app->display,
                window->id,
                x,
                y);

    return;
}

void
_R_resize_window(struct R_window *window,
                 int w,
                 int h)
{
    XResizeWindow(window->app->display,
                  window->id,
                  w,
                  h);

    return;
}

void
_R_move_resize_window(struct R_window *window,
                      int x,
                      int y,
                      int w,
                      int h)
{
    XMoveResizeWindow(window->app->display,
                      window->id,
                      x,
                      y,
                      w,
                      h);

    return;
}

void
_R_get_window_attributes(struct R_window *window, XWindowAttributes *attributes)
{
    XGetWindowAttributes(window->app->display, window->id, attributes);

    return;
}

void
_R_set_input_focus(struct R_window *window, Time timestamp)
{
    _ignbadmatch = 1;
    XSetInputFocus(window->app->display, window->id,
		   RevertToPointerRoot, timestamp);
    XSync(window->app->display, False);
    _ignbadmatch = 0;

    return;
}

