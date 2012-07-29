/*
 * event.c - event routines for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#if 0

#include <Resurrection/Resurrection.h>

#define X_EVENTS LASTEvent

#if (DEBUG_EVENT)

static const char *eventnames[X_EVENTS] =
{
    "",
    "",
    "KeyPress",
    "KeyRelease",
    "ButtonPress",
    "ButtonRelease",
    "MotionNotify",
    "EnterNotify",
    "LeaveNotify",
    "FocusIn",
    "FocusOut",
    "KeymapNotify",
    "Expose",
    "GraphicsExpose",
    "NoExpose",
    "VisibilityNotify",
    "CreateNotify",
    "DestroyNotify",
    "UnmapNotify",
    "MapNotify",
    "MapRequest",
    "ReparentNotify",
    "ConfigureNotify",
    "ConfigureRequest",
    "GravityNotify",
    "ResizeRequest",
    "CirculateNotify",
    "CirculateRequest",
    "PropertyNotify",
    "SelectionClear",
    "SelectionRequest",
    "SelectionNotify",
    "ColormapNotify",
    "ClientMessage",
    "MappingNotify"
};

#define PROPERTIES 69
static const char *propertynames[PROPERTIES] =
{
    NULL,
    "XA_PRIMARY",
    "XA_SECONDARY",
    "XA_ARC",
    "XA_ATOM",
    "XA_BITMAP",
    "XA_CARDINAL",
    "XA_COLORMAP",
    "XA_CURSOR",
    "XA_CUT_BUFFER0",
    "XA_CUT_BUFFER1",
    "XA_CUT_BUFFER2",
    "XA_CUT_BUFFER3",
    "XA_CUT_BUFFER4",
    "XA_CUT_BUFFER5",
    "XA_CUT_BUFFER6",
    "XA_CUT_BUFFER7",
    "XA_DRAWABLE",
    "XA_FONT",
    "XA_INTEGER",
    "XA_PIXMAP",
    "XA_POINT",
    "XA_RECTANGLE",
    "XA_RESOURCE_MANAGER",
    "XA_RGB_COLOR_MAP",
    "XA_RGB_BEST_MAP",
    "XA_RGB_BLUE_MAP",
    "XA_RGB_DEFAULT_MAP",
    "XA_RGB_GRAY_MAP",
    "XA_RGB_GREEN_MAP",
    "XA_RGB_RED_MAP",
    "XA_STRING",
    "XA_VISUALID",
    "XA_WINDOW",
    "XA_WM_COMMAND",
    "XA_WM_HINTS",
    "XA_WM_CLIENT_MACHINE",
    "XA_WM_ICON_NAME",
    "XA_WM_ICON_SIZE",
    "XA_WM_NAME",
    "XA_WM_NORMAL_HINTS",
    "XA_WM_SIZE_HINTS",
    "XA_WM_ZOOM_HINTS",
    "XA_MIN_SPACE",
    "XA_NORM_SPACE",
    "XA_MAX_SPACE",
    "XA_END_SPACE",
    "XA_SUPERSCRIPT_X",
    "XA_SUPERSCRIPT_Y",
    "XA_SUBSCRIPT_X",
    "XA_SUBSCRIPT_Y",
    "XA_UNDERLINE_POSITION",
    "XA_UNDERLINE_THICKNESS",
    "XA_STRIKEOUT_ASCENT",
    "XA_STRIKEOUT_DESCENT",
    "XA_ITALIC_ANGLE",
    "XA_X_HEIGHT",
    "XA_QUAD_WIDTH",
    "XA_WEIGHT",
    "XA_POINT_SIZE",
    "XA_RESOLUTION",
    "XA_COPYRIGHT",
    "XA_NOTICE",
    "XA_FONT_NAME",
    "XA_FAMILY_NAME",
    "XA_FULL_NAME",
    "XA_CAP_HEIGHT",
    "XA_WM_CLASS",
    "XA_WM_TRANSIENT_FOR"
};

static const char *windowtypenames[WINDOW_TYPES] =
{
    "WINDOW_UNMAPPED",
    "WINDOW_ROOT",
    "WINDOW_DESKTOP",
    "WINDOW_OVERRIDE",
    "WINDOW_FRAME",
    "WINDOW_TOPLEVEL",
    "WINDOW_TRANSIENT",
    "WINDOW_SUBWINDOW"
};

typedef void eventdebugfunc(struct R_window *, XEvent *);

static eventdebugfunc *debugfuncs[X_EVENTS] =
{
    NULL,
    NULL,
    event_debug_keypress,
    event_debug_keyrelease,
    event_debug_buttonpress,
    event_debug_buttonrelease,
    event_debug_motionnotify,
    event_debug_enternotify,
    event_debug_leavenotify,
    event_debug_focusin,
    event_debug_focusout,
    event_debug_keymapnotify,
    event_debug_expose,
    event_debug_graphicsexpose,
    event_debug_noexpose,
    event_debug_visibilitynotify,
    event_debug_createnotify,
    event_debug_destroynotify,
    event_debug_unmapnotify,
    event_debug_mapnotify,
    event_debug_maprequest,
    event_debug_reparentnotify,
    event_debug_configurenotify,
    event_debug_configurerequest,
    event_debug_gravitynotify,
    event_debug_resizerequest,
    event_debug_circulatenotify,
    event_debug_circulaterequest,
    event_debug_propertynotify,
    event_debug_selectionclear,
    event_debug_selectionrequest,
    event_debug_selectionnotify,
    event_debug_colormapnotify,
    event_debug_clientmessage,
    event_debug_mappingnotify
};

void
event_debug_common(struct R_window *window, XEvent *event)
{
    fprintf(stderr, "EVENT\t%s\n", eventnames[event->type]);
    fprintf(stderr, "\tserial: %lu\n", event->xany.serial);
    fprintf(stderr, "\tsend_event: %d\n", event->xany.send_event);
    fprintf(stderr, "\twindow: %lu, type == %s\n",
	    event->xany.window, windowtypenames[window->type]);

    return;
}

void
event_debug_keypress(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_keyrelease(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_buttonpress(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_buttonrelease(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_motionnotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_enternotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_leavenotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_focusin(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_focusout(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_keymapnotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_expose(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_graphicsexpose(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_noexpose(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_visibilitynotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_createnotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_destroynotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_unmapnotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_mapnotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_maprequest(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_reparentnotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_configurenotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_configurerequest(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
    fprintf(stderr, "\tx: %d\n", event->xconfigurerequest.x);
    fprintf(stderr, "\ty: %d\n", event->xconfigurerequest.y);
    fprintf(stderr, "\twidth: %d\n", event->xconfigurerequest.width);
    fprintf(stderr, "\theight: %d\n", event->xconfigurerequest.height);
}

void
event_debug_gravitynotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_resizerequest(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
    fprintf(stderr, "\twidth: %d\n", event->xresizerequest.width);
    fprintf(stderr, "\theight: %d\n", event->xresizerequest.height);
}

void
event_debug_circulatenotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_circulaterequest(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_propertynotify(struct R_window *window, XEvent *event)
{
    Atom atom;
    event_debug_common(window, event);
    atom = event->xproperty.atom;
    if (atom < PROPERTIES) {
	fprintf(stderr, "\tname: %s\n", propertynames[atom]);
    } else {
	fprintf(stderr, "\tatom: %lu", atom);
    }
}

void
event_debug_selectionclear(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_selectionrequest(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_selectionnotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_colormapnotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_clientmessage(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

void
event_debug_mappingnotify(struct R_window *window, XEvent *event)
{
    event_debug_common(window, event);
}

#endif /* DEBUG_EVENT */

static Drawable
event_key_dest(XEvent *event)
{
    return (event->xkey.window);
}

static Drawable
event_button_dest(XEvent *event)
{
    return (event->xbutton.window);
}

static Drawable
event_motion_dest(XEvent *event)
{
    return (event->xmotion.window);
}

static Drawable
event_crossing_dest(XEvent *event)
{
    return (event->xcrossing.window);
}

static Drawable
event_focus_dest(XEvent *event)
{
    return (event->xfocus.window);
}

static Drawable
event_keymap_dest(XEvent *event)
{
    return (event->xkeymap.window);
}

static Drawable
event_expose_dest(XEvent *event)
{
    return (event->xfocus.window);
}

static Drawable
event_graphicsexpose_dest(XEvent *event)
{
    return (event->xgraphicsexpose.drawable);
}

static Drawable
event_noexpose_dest(XEvent *event)
{
    return (event->xnoexpose.drawable);
}

static Drawable
event_visibility_dest(XEvent *event)
{
    return (event->xvisibility.window);
}

static Drawable
event_create_dest(XEvent *event)
{
    return (event->xcreatewindow.parent);
}

static Drawable
event_destroy_dest(XEvent *event)
{
    return (event->xdestroywindow.window);
}

static Drawable
event_unmap_dest(XEvent *event)
{
    return (event->xunmap.window);
}

static Drawable
event_map_dest(XEvent *event)
{
    return (event->xmap.window);
}

static Drawable
event_maprequest_dest(XEvent *event)
{
#if 0
    return (event->xmaprequest.parent);
#endif
    return (event->xmaprequest.window);
}

static Drawable
event_reparent_dest(XEvent *event)
{
    return (event->xreparent.window);
}

static Drawable
event_configure_dest(XEvent *event)
{
    return (event->xconfigure.window);
}

static Drawable
event_gravity_dest(XEvent *event)
{
    return (event->xgravity.window);
}

static Drawable
event_resizerequest_dest(XEvent *event)
{
    return (event->xresizerequest.window);
}

static Drawable
event_configurerequest_dest(XEvent *event)
{
#if 0
    return (event->xconfigurerequest.parent);
#endif
    return (event->xconfigurerequest.window);
}

static Drawable
event_circulate_dest(XEvent *event)
{
    return (event->xcirculate.window);
}

static Drawable
event_circulaterequest_dest(XEvent *event)
{
    return (event->xcirculaterequest.window);
}

static Drawable
event_property_dest(XEvent *event)
{
    return (event->xproperty.window);
}

static Drawable
event_selectionclear_dest(XEvent *event)
{
    return (event->xselectionclear.window);
}

static Drawable
event_selectionrequest_dest(XEvent *event)
{
    return (event->xselectionrequest.owner);
}

static Drawable
event_selection_dest(XEvent *event)
{
    return (event->xselection.requestor);
}

static Drawable
event_colormap_dest(XEvent *event)
{
    return (event->xcolormap.window);
}

static Drawable
event_client_dest(XEvent *event)
{
    return (event->xclient.window);
}

static Drawable
event_mapping_dest(XEvent *event)
{
    return (event->xmapping.window);
}

typedef Drawable eventdrawablefunc(XEvent *);
static eventdrawablefunc *eventdrawablefuncs[X_EVENTS] =
{
    NULL,
    NULL,
    event_key_dest,
    event_key_dest,
    event_button_dest,
    event_button_dest,
    event_motion_dest,
    event_crossing_dest,
    event_crossing_dest,
    event_focus_dest,
    event_focus_dest,
    event_keymap_dest,
    event_expose_dest,
    event_graphicsexpose_dest,
    event_noexpose_dest,
    event_visibility_dest,
    event_create_dest,
    event_destroy_dest,
    event_unmap_dest,
    event_map_dest,
    event_maprequest_dest,
    event_reparent_dest,
    event_configure_dest,
    event_configurerequest_dest,
    event_gravity_dest,
    event_resizerequest_dest,
    event_circulate_dest,
    event_circulaterequest_dest,
    event_property_dest,
    event_selectionclear_dest,
    event_selectionrequest_dest,
    event_selection_dest,
    event_colormap_dest,
    event_client_dest,
    event_mapping_dest
};

Drawable
event_get_dest(XEvent *event)
{
    eventdrawablefunc *func;
    Drawable retd;

    func = eventdrawablefuncs[(event->type)];
    retd = func(event);
    
    return retd;
}

void
event_handle_next(struct R_app *app)
{
    struct R_window *window;
    R_eventhandler_t *handler;
//    Reventhandlerlist_t *listhandler;
#if (DEBUG_EVENT)
    void (*debugfunc)(struct R_window *, XEvent *);
#endif
    Drawable drawable;
    XEvent event;

    if (app == NULL) {

	return;
    }

    XNextEvent(app->display, &event);

    if (XFilterEvent(&event, None)) {
	/* input method filters event. */

	XUngrabKeyboard(app->display, CurrentTime);
    } else {
	drawable = event_get_dest(&event);
	window = app_find_window(app, drawable);
	if (window) {
#if (DEBUG_EVENT)
	    if (app->debugeventflags & EVENT_DEBUG_FLAG(&event)) {
		debugfunc = debugfuncs[(event.type)];
		debugfunc(window, &event);
	    }
#endif
#if 0
	    listhandler = &(window->eventhandlers[(event.type)]);
	    handler = listhandler->handler;
#endif
            handler = window->eventhandlers[event.type];
	    if (handler) {
#if (DEBUG_EVENT)
		if (app->debugeventflags & EVENT_DEBUG_FLAG(&event)) {
		    fprintf(stderr, "HANDLED\n");
		}
#endif
		handler(window, &event);
#if 0
		while (listhandler->next) {
		    listhandler = listhandler->next;
		    handler = listhandler->handler;
		    handler(window, &event);
		}
#endif
	    } else {
#if (DEBUG_EVENT)
		if (app->debugeventflags & EVENT_DEBUG_FLAG(&event)) {
		    fprintf(stderr, "NO HANDLER\n");
		}
#endif
	    }
	} else {
#if (DEBUG_EVENT)
	    if (app->debugeventflags & EVENT_DEBUG_FLAG(&event)) {
		fprintf(stderr, "NO WINDOW\n");
	    }
#endif
	}
    }
    
    return;
}

void
events_handle(struct R_app *app)
{
    if (app == NULL) {

	return;
    }

    while (XPending(app->display)) {
	event_handle_next(app);
    }

    return;
}

#endif
