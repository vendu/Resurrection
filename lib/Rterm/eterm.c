/*
 * eterm.c - Eterm compatibility routines for Rterm.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#if 0

#if 0
static Retermsequencehandlerlist_t
ipchandlers[ETERM_SEQUENCE_HANDLER_HASH_SIZE];

static Retermsequencehandlerlist_t
winophandlers[ETERM_SEQUENCE_HANDLER_HASH_SIZE];
#endif

Retermsequencehandler_t *escapehandlers[ETERM_ESCAPE_HANDLER_ARRAY_SIZE];

int
eterm_add_sequence_handler(Retermsequencehandlerlist_t *handlers,
			   const char *command,
			   Retermsequencehandler_t *handler)
{
    unsigned long key;
    Retermsequencehandlerlist_t *listhandler;

    if (handlers == NULL
	|| command == NULL
	|| handler == NULL) {

	return -1;
    }

    if ((key = hash_compute_key(command, ETERM_SEQUENCE_HANDLER_HASH_PRIME))
	< 0) {

	return -1;
    }

    if (handlers[key].handler == NULL) {
	handlers[key].handler = handler;
    } else {
	listhandler = &(handlers[key]);

	while (listhandler->next) {
	    listhandler = listhandler->next;
	}

	if ((listhandler->next = calloc(1,
					sizeof(Retermsequencehandlerlist_t)))
	    == NULL) {

	    return -1;
	}

	listhandler = listhandler->next;

	listhandler->command = command;
	listhandler->handler = handler;
    }

    return 0;
}

Retermsequencehandler_t *
eterm_find_sequence_handler(Retermsequencehandlerlist_t *handlers,
			    const char *command)
{
    unsigned long key;
    Retermsequencehandlerlist_t *listhandler = NULL;

    if (handlers == NULL
	|| command == NULL) {

	return NULL;
    }

    if ((key = hash_compute_key(command, ETERM_SEQUENCE_HANDLER_HASH_PRIME))
	< 0) {
	
	return NULL;
    }

    if (!strcmp(handlers[key].command, command)) {

	return (handlers[key].handler);
    } else {
	listhandler = handlers[key].next;

	if (!strcmp(listhandler->command, command)) {

	    return (listhandler->handler);
	}

	while ((listhandler = listhandler->next)) {
	    if (!strcmp(listhandler->command, command)) {
		
		return (listhandler->handler);
	    }
	}
    }

    return NULL;
}

int
eterm_register_escape_handlers(void)
{
    if (eterm_register_escape_handler(ETERM_SET_TRANSPARENCY,
				      eterm_escape_set_transparency) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_SET_COLOR_MODIFIERS,
				      eterm_escape_set_color_modifiers) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_SHADE_OR_TINT,
				      eterm_escape_shade_or_tint) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_FORCE_BACKGROUND_UPDATE,
				      eterm_escape_force_background_update)
	< 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_CONFIGURE_SCROLLBAR,
				      eterm_escape_configure_scrollbar) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_SCROLLBAR_RIGHT,
				      eterm_escape_scrollbar_right) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_SCROLLBAR_FLOATING,
				      eterm_escape_floating_scrollbar) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_SCROLLBAR_POPUP,
				      eterm_escape_popup_scrollbar) < 0) {
	
	return -1;
    }

    if (eterm_register_escape_handler(ETERM_BUTTONBARS,
				      eterm_escape_buttonbars) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_VISUAL_BELL,
				      eterm_escape_visual_bell) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_MAP_ALERT,
				      eterm_escape_map_alert) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_XTERM_SELECTION_BEHAVIOR,
				      eterm_escape_xterm_selection_behavior)
	< 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_TRIPLE_CLICK_SELECT,
				      eterm_escape_triple_click_select) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_VIEWPORT_MODE,
				      eterm_escape_viewport_mode) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_SELECT_TRAILING_SPACES,
				      eterm_escape_select_trailing_spaces)
	< 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_REPORT_AS_KEYSYMS,
				      eterm_escape_report_as_keysyms) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_REFUSE_INPUT_AND_FOCUS,
				      eterm_escape_refuse_input_and_focus) < 0) {
	
	return -1;
    }

#if 0
    if (eterm_register_escape_handler(ETERM_SET_FOREGROUND_COLOR,
				      eterm_escape_set_foreground_color) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_SET_BACKGROUND_COLOR,
				      eterm_escape_set_background_color) < 0) {

	return -1;
    }
#endif

    if (eterm_register_escape_handler(ETERM_MOVE_TO_DESKTOP,
				      eterm_escape_move_to_desktop) < 0) {

	return -1;
    }

#if 0
    if (eterm_register_escape_handler(ETERM_EXIT,
				      eterm_escape_exit) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_SAVE_CONFIG,
				      eterm_escape_save_config) < 0) {

	return -1;
    }
#endif

    if (eterm_register_escape_handler(ETERM_SET_DEBUG_LEVEL,
				      eterm_escape_set_debug_level) < 0) {

	return -1;
    }

    if (eterm_register_escape_handler(ETERM_SEARCH_FOR_STRING,
				      eterm_escape_search_for_string) < 0) {

	return -1;
    }

    return 0;
}

int
eterm_register_escape_handler(int operation, Retermsequencehandler_t *handler)
{
    if (operation < 0
	|| operation > ETERM_ESCAPE_HANDLER_ARRAY_SIZE - 1
	|| handler == NULL) {

	return -1;
    }

    escapehandlers[operation] = handler;

    return 0;
}

/* TODO: implement these */
void
eterm_process_escape_sequence(Rterm_t *term, char *str)
{
    unsigned long operation;
    Retermsequencehandler_t *handler;

    if (term == NULL
	|| str == NULL) {

	return;
    }

    operation = strtoul(str, NULL, 0);

    if (errno == ERANGE
	|| operation > ETERM_ESCAPE_HANDLER_ARRAY_SIZE - 1) {

	return;
    }

    str = strchr(str, ';');

    if (str) {
	str++;
    }

    if ((handler = escapehandlers[operation])) {
	handler(term, str);
    } else {
	fprintf(stderr, "%s: eterm_process_escape_sequence: unknown command \"%s\"\n", term->app->name, str);
    }

    return;
}

void
eterm_escape_set_transparency(Rterm_t *term, char *str)
{
    Rtermscreen_t *screen;
    void (*refresh)(struct Rtermscreen *, int);

    if (term == NULL) {

	return;
    }

    TERM_SET_FLAG(term, TERM_TRANSPARENT, str);

    screen = term->window->screen;
    refresh = screen->funcs.refresh;
    if (refresh == NULL) {

	return;
    }

    if (term->flags & TERM_TRANSPARENT) {
#if 0
	term->window->images[WINDOW_BACKGROUND_IMAGE].flags |= IMAGE_TRANSPARENT;
#endif
	
	term->window->screen->window->images[WINDOW_BACKGROUND_IMAGE].flags
	    |= IMAGE_TRANSPARENT;

	if (term->window->flags & WINDOW_HAS_SCROLLBAR) {
	    term->window->scrollbar->window->images[WINDOW_BACKGROUND_IMAGE].flags
		|= IMAGE_TRANSPARENT;
	}
    } else {
	term->window->images[WINDOW_BACKGROUND_IMAGE].flags &= ~IMAGE_TRANSPARENT;

#if 0	
	if (window_load_image(term->window,
			      WINDOW_BACKGROUND_IMAGE,
			      RESURRECTION_IMAGE_SEARCH_PATH "background/background.jpg")
	    < 0) {

	    return;
	}
#endif

#if 0
	term->window->images[WINDOW_BACKGROUND_IMAGE].w = -1;
	term->window->images[WINDOW_BACKGROUND_IMAGE].h = -1;
#endif

	term->window->screen->window->images[WINDOW_BACKGROUND_IMAGE].flags
	    &= ~IMAGE_TRANSPARENT;

	if (term->window->flags & WINDOW_HAS_SCROLLBAR) {
	    term->window->scrollbar->window->images[WINDOW_BACKGROUND_IMAGE].flags
		&= ~IMAGE_TRANSPARENT;
	}
    }

#if 0
    window_set_pixmap(term->window, WINDOW_BACKGROUND_IMAGE);
#endif

    window_render_background_imlib2(term->window->screen->window,
				    &term->window->screen->window->images[WINDOW_BACKGROUND_IMAGE]);

    if (term->window->flags & WINDOW_HAS_SCROLLBAR) {
	window_render_background_imlib2(term->window->scrollbar->window,
					&term->window->scrollbar->window->images[WINDOW_BACKGROUND_IMAGE]);

	scrollbar_update(term->window->scrollbar);
    } else {
	screen_touch(screen);
    
	refresh(screen, SCREEN_DRAW_ALL);
    }

    return;
}

/* TODO: finish this. */
void
eterm_escape_set_color_modifiers(Rterm_t *term, char *str)
{
    int imagetype, mod, changed;
    unsigned long value;
    char *type, *attribute;
    Rimage_t *image;
    
    if (term == NULL
	|| str == NULL) {
	
	return;
    }
    
#if (!(USE_IMLIB2))
    if ((imagetype = eterm_get_image_type(str)) < 0) {
	imagetype = TERM_BACKGROUND_IMAGE;
    } else {
	str = strchr(str, ';');

	if (str) {
	    *str = '\0';

	    str++;
	} else {

	    return;
	}
    }

    type = str;

    str = strchr(str, ';');

    if (str) {
	*str = '\0';

	str++;
    } else {
	
	return;
    }

    attribute = str;

    str = strchr(str, ';');

    if (str) {
	*str = '\0';
	
	str++;
    } else {

	return;
    }

    value = strtoul(str, NULL, 0);

    /* FIXME: we should probably have IMAGE_MAX_MODIFIER be 255 * 256. */
    if (value > INT_MAX || errno == ERANGE) {
	mod = INT_MAX;
    } else {
	mod = (int)value;
    }

    changed = 0;

    image = &(term->images[imagetype]);

    if (!strcasecmp(attribute, "clear")) {
	if (!strcasecmp(type, "image")) {
	    if (image->colormodifiers[IMAGE_COLOR_MODIFIER] == NULL) {
		if ((image->colormodifiers[IMAGE_COLOR_MODIFIER] =
		     calloc(1, sizeof(ImlibColorModifier))) == NULL) {
		    
		    return;
		}
	    }
	    
	    image->colormodifiers[IMAGE_COLOR_MODIFIER]->gamma =
		IMAGE_NORMAL_GAMMA;
	    image->colormodifiers[IMAGE_COLOR_MODIFIER]->brightness = 
		IMAGE_NORMAL_BRIGHTNESS;
	    image->colormodifiers[IMAGE_COLOR_MODIFIER]->contrast = 
		IMAGE_NORMAL_CONTRAST;
	} else if (!strcasecmp(type, "red")) {
	    if (image->colormodifiers[IMAGE_RED_MODIFIER] == NULL) {
		if ((image->colormodifiers[IMAGE_RED_MODIFIER] =
		     calloc(1, sizeof(ImlibColorModifier))) == NULL) {
		    
		    return;
		}
	    }
	    
	    image->colormodifiers[IMAGE_RED_MODIFIER]->gamma =
		IMAGE_NORMAL_GAMMA;
	    image->colormodifiers[IMAGE_RED_MODIFIER]->brightness = 
		IMAGE_NORMAL_BRIGHTNESS;
	    image->colormodifiers[IMAGE_RED_MODIFIER]->contrast = 
		IMAGE_NORMAL_CONTRAST;
	} else if (!strcasecmp(type, "green")) {
	    if (image->colormodifiers[IMAGE_GREEN_MODIFIER] == NULL) {
		if ((image->colormodifiers[IMAGE_GREEN_MODIFIER] =
		     calloc(1, sizeof(ImlibColorModifier))) == NULL) {
		    
		    return;
		}
	    }
	    
	    image->colormodifiers[IMAGE_GREEN_MODIFIER]->gamma =
		IMAGE_NORMAL_GAMMA;
	    image->colormodifiers[IMAGE_GREEN_MODIFIER]->brightness = 
		IMAGE_NORMAL_BRIGHTNESS;
	    image->colormodifiers[IMAGE_GREEN_MODIFIER]->contrast = 
		IMAGE_NORMAL_CONTRAST;
	} else if (!strcasecmp(type, "blue")) {
	    if (image->colormodifiers[IMAGE_BLUE_MODIFIER] == NULL) {
		if ((image->colormodifiers[IMAGE_BLUE_MODIFIER] =
		     calloc(1, sizeof(ImlibColorModifier))) == NULL) {
		    
		    return;
		}
	    }
	    
	    image->colormodifiers[IMAGE_BLUE_MODIFIER]->gamma =
		IMAGE_NORMAL_GAMMA;
	    image->colormodifiers[IMAGE_BLUE_MODIFIER]->brightness = 
		IMAGE_NORMAL_BRIGHTNESS;
	    image->colormodifiers[IMAGE_BLUE_MODIFIER]->contrast = 
		IMAGE_NORMAL_CONTRAST;
	} else {
	    
	    return;
	}
	
	app_apply_image_color_modifiers(term->app, image);
	
	changed = 1;
    } else if (!strcasecmp(type, "image")) {
	if (image->colormodifiers[IMAGE_COLOR_MODIFIER] == NULL) {
	    if ((image->colormodifiers[IMAGE_COLOR_MODIFIER] =
		 calloc(1, sizeof(ImlibColorModifier))) == NULL) {
		
		return;
	    }
	}
	
	image->colormodifiers[IMAGE_COLOR_MODIFIER]->gamma =
	    IMAGE_NORMAL_GAMMA;
	image->colormodifiers[IMAGE_COLOR_MODIFIER]->brightness = 
	    IMAGE_NORMAL_BRIGHTNESS;
	image->colormodifiers[IMAGE_COLOR_MODIFIER]->contrast = 
	    IMAGE_NORMAL_CONTRAST;
	
	if (!strcasecmp(attribute, "gamma")) {
	    image->colormodifiers[IMAGE_COLOR_MODIFIER]->gamma = value;
	} else if (!strcasecmp(attribute, "brightness")) {
	    image->colormodifiers[IMAGE_COLOR_MODIFIER]->brightness = value;
	} else if (!strcasecmp(attribute, "contrast")) {
	    image->colormodifiers[IMAGE_COLOR_MODIFIER]->contrast = value;
	} else {

	    return;
	}

	app_apply_image_color_modifiers(term->app, image);

	changed = 1;
    } else if (!strcasecmp(type, "red")) {
	if (image->colormodifiers[IMAGE_RED_MODIFIER] == NULL) {
	    if ((image->colormodifiers[IMAGE_RED_MODIFIER] =
		 calloc(1, sizeof(ImlibColorModifier))) == NULL) {
		
		return;
	    }
	}
	
	image->colormodifiers[IMAGE_RED_MODIFIER]->gamma =
	    IMAGE_NORMAL_GAMMA;
	image->colormodifiers[IMAGE_RED_MODIFIER]->brightness = 
	    IMAGE_NORMAL_BRIGHTNESS;
	image->colormodifiers[IMAGE_RED_MODIFIER]->contrast = 
	    IMAGE_NORMAL_CONTRAST;
	
	if (!strcasecmp(attribute, "gamma")) {
	    image->colormodifiers[IMAGE_RED_MODIFIER]->gamma = value;
	} else if (!strcasecmp(attribute, "brightness")) {
	    image->colormodifiers[IMAGE_RED_MODIFIER]->brightness = value;
	} else if (!strcasecmp(attribute, "contrast")) {
	    image->colormodifiers[IMAGE_RED_MODIFIER]->contrast = value;
	} else {

	    return;
	}

	app_apply_image_color_modifiers(term->app, image);

	changed = 1;
    } else if (!strcasecmp(type, "green")) {
	if (image->colormodifiers[IMAGE_GREEN_MODIFIER] == NULL) {
	    if ((image->colormodifiers[IMAGE_GREEN_MODIFIER] =
		 calloc(1, sizeof(ImlibColorModifier))) == NULL) {
		
		return;
	    }
	}
	
	image->colormodifiers[IMAGE_GREEN_MODIFIER]->gamma =
	    IMAGE_NORMAL_GAMMA;
	image->colormodifiers[IMAGE_GREEN_MODIFIER]->brightness = 
	    IMAGE_NORMAL_BRIGHTNESS;
	image->colormodifiers[IMAGE_GREEN_MODIFIER]->contrast = 
	    IMAGE_NORMAL_CONTRAST;
	
	if (!strcasecmp(attribute, "gamma")) {
	    image->colormodifiers[IMAGE_GREEN_MODIFIER]->gamma = value;
	} else if (!strcasecmp(attribute, "brightness")) {
	    image->colormodifiers[IMAGE_GREEN_MODIFIER]->brightness = value;
	} else if (!strcasecmp(attribute, "contrast")) {
	    image->colormodifiers[IMAGE_GREEN_MODIFIER]->contrast = value;
	} else {
	    
	    return;
	}
	
	app_apply_image_color_modifiers(term->app, image);
	
	changed = 1;
    } else if (!strcasecmp(type, "blue")) {
	if (image->colormodifiers[IMAGE_BLUE_MODIFIER] == NULL) {
	    if ((image->colormodifiers[IMAGE_BLUE_MODIFIER] =
		 calloc(1, sizeof(ImlibColorModifier))) == NULL) {
		
		return;
	    }
	}
	
	image->colormodifiers[IMAGE_BLUE_MODIFIER]->gamma =
	    IMAGE_NORMAL_GAMMA;
	image->colormodifiers[IMAGE_BLUE_MODIFIER]->brightness = 
	    IMAGE_NORMAL_BRIGHTNESS;
	image->colormodifiers[IMAGE_BLUE_MODIFIER]->contrast = 
	    IMAGE_NORMAL_CONTRAST;
	
	if (!strcasecmp(attribute, "gamma")) {
	    image->colormodifiers[IMAGE_BLUE_MODIFIER]->gamma = value;
	} else if (!strcasecmp(attribute, "brightness")) {
	    image->colormodifiers[IMAGE_BLUE_MODIFIER]->brightness = value;
	} else if (!strcasecmp(attribute, "contrast")) {
	    image->colormodifiers[IMAGE_BLUE_MODIFIER]->contrast = value;
	} else {
	    
	    return;
	}
	
	app_apply_image_color_modifiers(term->app, image);
	
	changed = 1;
    }
    
    if (changed) {
	term_render_image(term, imagetype);
    }
#endif /* !USE_IMLIB2 */

    return;
}

/* TODO: implement this. */
void
eterm_escape_shade_or_tint(Rterm_t *term, char *str)
{
    ;
}

void
eterm_escape_force_background_update(Rterm_t *term, char *str)
{
    Rtermscreen_t *screen;
    void (*refresh)(struct Rtermscreen *, int);

    if (term == NULL) {
	
	return;
    }
    
    if (term->flags & TERM_TRANSPARENT) {
	screen = term->window->screen;
	refresh = screen->funcs.refresh;
	if (refresh == NULL) {

	    return;
	}
	
#if 0
	window_set_background_pixmap(term->window, WINDOW_BACKGROUND_IMAGE);
#endif

	window_render_background_imlib2(term->window->screen->window,
					&term->window->screen->window->images[WINDOW_BACKGROUND_IMAGE]);
	
	if (term->privmodes & TERM_PRIVMODE_SCROLLBAR) {
	    window_render_background_imlib2(term->window->scrollbar->window,
					    &term->window->scrollbar->window->images[WINDOW_BACKGROUND_IMAGE]);
	}
	
	screen_touch(screen);
	
	refresh(screen, SCREEN_DRAW_ALL);
    }

    return;
}

void
eterm_escape_configure_scrollbar(Rterm_t *term, char *str)
{
    if (term == NULL
	|| str == NULL) {

	return;
    }

    if (!strcasecmp(str, "xterm")) {
	;
    } else if (!strcasecmp(str, "next")) {
	;
    } else if (!strcasecmp(str, "motif")) {
	;
    }
    
    if ((str = strchr(str, ';')) == NULL) {

	return;
    }
    
    /* TODO: set scrollbar width. */

    return;
}

void
eterm_escape_scrollbar_right(Rterm_t *term, char *str)
{
    if (term == NULL) {

	return;
    }

    TERM_SET_FLAG(term, TERM_SCROLLBAR_RIGHT, str);

    FLAG_SET(term->window->flags, WINDOW_SCROLLBAR_RIGHT,
	     ((term->flags & TERM_SCROLLBAR_RIGHT) ? TRUE : FALSE));
    
    term_move_scrollbar(term);

    return;
}

void
eterm_escape_floating_scrollbar(Rterm_t *term, char *str)
{
    if (term == NULL) {
	
	return;
    }

    TERM_SET_FLAG(term, TERM_FLOATING_SCROLLBAR, str);
    
    FLAG_SET(term->window->flags, WINDOW_FLOATING_SCROLLBAR,
	     ((term->flags & TERM_FLOATING_SCROLLBAR) ? TRUE : FALSE));
    
    term_floating_scrollbar(term);

    return;
}

void
eterm_escape_popup_scrollbar(Rterm_t *term, char *str)
{
    if (term == NULL) {

	return;
    }

    TERM_SET_FLAG(term, TERM_POPUP_SCROLLBAR, str);
    
    FLAG_SET(term->window->flags, WINDOW_POPUP_SCROLLBAR,
	     ((term->flags & TERM_POPUP_SCROLLBAR) ? TRUE : FALSE));
    
    term_show_scrollbar(term);

    return;
}

/* TODO: implement this. */
void
eterm_escape_buttonbars(Rterm_t *term, char *str)
{
    ;
}

void
eterm_escape_visual_bell(Rterm_t *term, char *str)
{
    if (term == NULL) {

	return;
    }

    TERM_SET_FLAG(term, TERM_VISUAL_BELL, str);

    return;
}

void
eterm_escape_map_alert(Rterm_t *term, char *str)
{
    if (term == NULL) {

	return;
    }

    TERM_SET_FLAG(term, TERM_MAP_ALERT, str);

    return;
}

void
eterm_escape_xterm_selection_behavior(Rterm_t *term, char *str)
{
    if (term == NULL) {

	return;
    }

    TERM_SET_FLAG(term, TERM_XTERM_SELECT, str);

    return;
}

void
eterm_escape_triple_click_select(Rterm_t *term, char *str)
{
    if (term == NULL) {

	return;
    }

    TERM_SET_FLAG(term, TERM_SELECT_LINE, str);

    return;
}

void
eterm_escape_viewport_mode(Rterm_t *term, char *str)
{
    if (term == NULL) {

	return;
    }

    TERM_SET_FLAG(term, TERM_VIEWPORT_MODE, str);

    return;
}

void
eterm_escape_select_trailing_spaces(Rterm_t *term, char *str)
{
    if (term == NULL) {

	return;
    }

    TERM_SET_FLAG(term, TERM_SELECT_TRAILING_SPACES, str);

    return;
}

void
eterm_escape_report_as_keysyms(Rterm_t *term, char *str)
{
    if (term == NULL) {

	return;
    }

    TERM_SET_FLAG(term, TERM_REPORT_AS_KEYSYMS, str);

    return;
}

/* TODO: implement this. */
void
eterm_escape_refuse_input_and_focus(Rterm_t *term, char *str)
{
    ;
}

void
eterm_escape_set_foreground_color(Rterm_t *term, char *str)
{
    if (term == NULL
	|| str == NULL) {

	return;
    }

    

    return;
}

void
eterm_escape_set_background_color(Rterm_t *term, char *str)
{
    if (term == NULL
	|| str == NULL) {

	return;
    }

    return;
}

void
eterm_escape_move_to_desktop(Rterm_t *term, char *str)
{
    if (term == NULL
	|| str == NULL) {

	return;
    }

    return;
}

#if 0
void
eterm_escape_exit(Rterm_t *term, char *str)
{
    if (term == NULL
	|| str == NULL) {

	return;
    }

    return;
}

void
eterm_escape_save_config(Rterm_t *term, char *str)
{
    if (term == NULL
	|| str == NULL) {

	return;
    }

    return;
}
#endif

void
eterm_escape_set_debug_level(Rterm_t *term, char *str)
{
    if (term == NULL
	|| str == NULL) {

	return;
    }

    return;
}

void
eterm_escape_search_for_string(Rterm_t *term, char *str)
{
    ;
}

int
eterm_get_image_type(const char *str)
{
    int type;
    
    if (!strcasecmp(str, "bg")) {
	type = TERM_BACKGROUND_IMAGE;
    } else if (!strcasecmp(str, "sb")) {
	type = TERM_SCROLLBAR_IMAGE;
    } else if (!strcasecmp(str, "sa")) {
	type = TERM_SCROLLBAR_ANCHOR_IMAGE;
    } else if (!strcasecmp(str, "up")) {
	type = TERM_SCROLLBAR_UP_ARROW_IMAGE;
    } else if (!strcasecmp(str, "down")) {
	type = TERM_SCROLLBAR_DOWN_ARROW_IMAGE;
    } else if (!strcasecmp(str, "st")) {
	type = TERM_SCROLLBAR_THUMB_IMAGE;
    } else if (!strcasecmp(str, "menu")) {
	type = TERM_MENU_BACKGROUND_IMAGE;
    } else if (!strcasecmp(str, "menuitem")) {
	type = TERM_MENUITEM_IMAGE;
    } else if (!strcasecmp(str, "submenu")) {
	type = TERM_SUBMENU_IMAGE;
    } else if (!strcasecmp(str, "button")) {
	type = TERM_BUTTON_BACKGROUND_IMAGE;
    } else if (!strcasecmp(str, "bbar")) {
	type = TERM_BUTTONBAR_BACKGROUND_IMAGE;
    } else {

	return -1;
    }

    return type;
}

int
eterm_register_ipc_handlers(void)
{
    if (eterm_register_ipc_handler("echo", eterm_ipc_echo) < 0) {

	return -1;
    }

    if (eterm_register_ipc_handler("tty_write", eterm_ipc_echo) < 0) {

	return -1;
    }

    if (eterm_register_ipc_handler("parse", eterm_ipc_parse) < 0) {

	return -1;
    }

    if (eterm_register_ipc_handler("enl_send", eterm_ipc_enl_send)
	< 0) {

	return -1;
    }

    if (eterm_register_ipc_handler("enl_query", eterm_ipc_enl_query)
	< 0) {
	
	return -1;
    }

    if (eterm_register_ipc_handler("winop", eterm_ipc_process_winop) < 0) {

	return -1;
    }

    if (eterm_register_ipc_handler("exit", eterm_ipc_exit) < 0) {

	return -1;
    }

    return 0;
}

int
eterm_register_ipc_handler(const char *command,
			   Retermsequencehandler_t *handler)
{
    if (command == NULL
	|| *command == '\0'
	|| handler == NULL) {

	return -1;
    }

    if (eterm_add_sequence_handler(ipchandlers, command, handler) < 0) {

	return -1;
    }

    return 0;
}

void
eterm_process_ipc_sequence(Rterm_t *term, char *command)
{
    char *args = NULL;
    Retermsequencehandler_t *handler = NULL;

    if (term == NULL
	|| args == NULL) {

	return;
    }

    args = strchr(command, ':');

    if (args) {
	*args++ = '\0';
    }

    if ((handler = eterm_find_sequence_handler(ipchandlers, command))) {
	handler(term, args);
    } else {
	fprintf(stderr, "%s: Eterm IPC error: unrecognized command \"%s\"\n",
		term->app->name, command);
    }

    return;
}

void
eterm_ipc_echo(Rterm_t *term, char *args)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    pty_write(term->ptyfd, args, strlen(args));

    return;
}

void
eterm_ipc_parse(Rterm_t *term, char *args)
{
    int (*queue_input)();
    if (term == NULL
	|| args == NULL) {

	return;
    }
    queue_input = term->funcs.queue_input;
    if (queue_input == NULL) {

	return;
    }

    queue_input(term, args, strlen(args));
    
    return;
}

void
eterm_ipc_enl_send(Rterm_t *term, char *args)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    e_send_ipc_message(term->app, args);

    return;
}

void
eterm_ipc_enl_query(Rterm_t *term, char *args)
{
    char *reply;
    char header[ETERM_IPC_HEADER_LENGTH];

    if (term == NULL
	|| args == NULL) {

	return;
    }

    reply = e_send_ipc_message_and_get_reply(term->app, args);
    
    snprintf(header, sizeof(header), "Enlightenment IPC reply to \"%s\":\n\n", args);
    
    pty_write(term->ptyfd, header, strlen(header));
    pty_write(term->ptyfd, reply, strlen(reply));
    pty_write(term->ptyfd, "\n", 1); /* FIXME: \n\r? */
    
    free(reply);

    return;
}

void
eterm_ipc_exit(Rterm_t *term, char *args)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    exit(EXIT_SUCCESS);
}

int
eterm_register_winop_handlers(void)
{
    if (eterm_register_winop_handler("raise", eterm_winop_raise) < 0) {

	return -1;
    }

    if (eterm_register_winop_handler("lower", eterm_winop_lower) < 0) {

	return -1;
    }

    if (eterm_register_winop_handler("map", eterm_winop_map) < 0) {

	return -1;
    }

    if (eterm_register_winop_handler("unmap", eterm_winop_unmap) < 0) {

	return -1;
    }

    if (eterm_register_winop_handler("kill", eterm_winop_kill) < 0) {

	return -1;
    }

    if (eterm_register_winop_handler("iconify", eterm_winop_iconify)
	< 0) {

	return -1;
    }

    return 0;
}

int
eterm_register_winop_handler(const char *command,
			     Retermsequencehandler_t *handler)
{
    if (command == NULL
	|| *command == '\0'
	|| handler == NULL) {

	return -1;
    }

    if (eterm_add_sequence_handler(winophandlers, command, handler) < 0) {

	return -1;
    }

    return 0;
}

void
eterm_ipc_process_winop(Rterm_t *term, char *command)
{
    Retermsequencehandler_t *handler;

    if (term == NULL
	|| command == NULL
	|| *command == '\0') {

	return;
    }

    if ((handler = eterm_find_sequence_handler(winophandlers, args))) {
	handler(term, args);
    } else {
	fprintf(stderr, "%s: Eterm IPC error: unrecognized window operation \"%s\"\n", term->app->name, args);
    }

    return;
}

void
eterm_winop_raise(Rterm_t *term, char *args)
{
    Rwindow_t *window;

    if (term == NULL
	|| args == NULL) {

	return;
    }

    if ((window = eterm_get_winop_window(term, args)) == NULL) {

	return;
    }

    window_raise(window);

    return;
}

void
eterm_winop_lower(Rterm_t *term, char *args)
{
    Rwindow_t *window;

    if (term == NULL
	|| args == NULL) {

	return;
    }

    if ((window = eterm_get_winop_window(term, args)) == NULL) {

	return;
    }

    window_lower(window);

    return;
}

void
eterm_winop_map(Rterm_t *term, char *args)
{
    Rwindow_t *window;

    if (term == NULL
	|| args == NULL) {

	return;
    }

    if ((window = eterm_get_winop_window(term, args)) == NULL) {

	return;
    }

    window_map(window);

    return;
}

void
eterm_winop_unmap(Rterm_t *term, char *args)
{
    Rwindow_t *window;

    if (term == NULL
	|| args == NULL) {

	return;
    }

    if ((window = eterm_get_winop_window(term, args)) == NULL) {

	return;
    }

    window_unmap(window);

    return;
}

void
eterm_winop_kill(Rterm_t *term, char *args)
{
    Rwindow_t *window;

    if (term == NULL
	|| args == NULL) {

	return;
    }

    if ((window = eterm_get_winop_window(term, args)) == NULL) {

	return;
    }

    window_kill_client(window);

    return;
}

void
eterm_winop_iconify(Rterm_t *term, char *args)
{
    Rwindow_t *window;

    if (term == NULL
	|| args == NULL) {

	return;
    }

    if ((window = eterm_get_winop_window(term, args)) == NULL) {

	return;
    }

    window_iconify(window);

    return;
}

Rwindow_t *
eterm_get_winop_window(Rterm_t *term, char *args)
{
    char *winid;
    Rwindow_t *window;
    Window win = None;

    if (term == NULL
	|| args == NULL) {

	return NULL;
    }

    winid = strchr(args, ' ');

    if (winid) {
	win = (Window)strtoul(winid + 1, NULL, 0);
    }

    if (errno == ERANGE) {

	return NULL;
    }

    if (win == None) {
	window = term->window;
    } else if ((window = windowhash_find(term->app->windowhash, win))
	       == NULL) {

	return NULL;
    }

    return window;
}

#endif /* 0 */
