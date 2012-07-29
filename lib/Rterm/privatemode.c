/*
 * privatemode.c - private mode sequence handling for Rterm.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

typedef void R_termprivmodehandler_t(struct R_term *, struct R_termscreen *, int);

static R_termprivmodehandler_t *
privatemodehandlers[RTERM_PRIVATE_MODE_HANDLER_ARRAY_SIZE];

void Rterm_private_mode_cursor_key_mode(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_us_ascii_charsets(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_132_column_mode(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_scroll_mode(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_reverse_video_mode(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_origin_mode(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_autowrap_mode(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_auto_repeat_keys(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_start_blinking_cursor(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_xterm_x10_mouse_reporting(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_eterm_menubar_mode(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_cursor_visibility(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_eterm_scrollbar_mode(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_eterm_shift_sequences(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_tektronix_mode(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_allow_132_column_mode(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_xterm_more_fix(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_xterm_margin_bell(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_xterm_reverse_autowrap(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_xterm_logging(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_screen_mode(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_eterm_keypad_mode(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_eterm_backspace_mode(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_xterm_x11_mouse_reporting(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_xterm_highlight_mouse_tracking(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_eterm_home_on_echo(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_eterm_home_on_refresh(struct R_term *term, struct R_termscreen *screen, int mode);
void Rterm_private_mode_eterm_home_on_input(struct R_term *term, struct R_termscreen *screen, int mode);

int
Rterm_register_private_mode_handler(int operation,
                                    R_termprivmodehandler_t *handler)
{
    if (operation < 0
	|| operation > RTERM_PRIVATE_MODE_HANDLER_ARRAY_SIZE - 1
	|| handler == NULL) {

	return -1;
    }

    privatemodehandlers[operation] = handler;

    return 0;
}

int
Rterm_register_private_mode_handlers(void)
{
    if (Rterm_register_private_mode_handler(RTERM_CURSOR_KEY_MODE,
					   Rterm_private_mode_cursor_key_mode)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_US_ASCII_CHARSETS,
					   Rterm_private_mode_us_ascii_charsets)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_132_COLUMN_MODE,
					   Rterm_private_mode_132_column_mode)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_SCROLL_MODE,
					   Rterm_private_mode_scroll_mode)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_REVERSE_VIDEO_MODE,
					   Rterm_private_mode_reverse_video_mode)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_ORIGIN_MODE,
					   Rterm_private_mode_origin_mode)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_AUTOWRAP_MODE,
					   Rterm_private_mode_autowrap_mode)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_AUTO_REPEAT_KEYS,
					   Rterm_private_mode_auto_repeat_keys)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_START_BLINKING_CURSOR,
					   Rterm_private_mode_start_blinking_cursor)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_XTERM_X10_MOUSE_REPORTING,
					   Rterm_private_mode_xterm_x10_mouse_reporting)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_ETERM_MENUBAR_MODE,
					   Rterm_private_mode_eterm_menubar_mode)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_CURSOR_VISIBILITY,
					   Rterm_private_mode_cursor_visibility)
	< 0) {

	return -1;
    }

#if 0
    if (Rterm_register_private_mode_handler(RTERM_ETERM_SCROLLBAR_MODE,
					   Rterm_private_mode_eterm_scrollbar_mode)
	< 0) {

	return -1;
    }
#endif

    if (Rterm_register_private_mode_handler(RTERM_ETERM_SHIFT_SEQUENCES,
					   Rterm_private_mode_eterm_shift_sequences)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_TEKTRONIX_MODE,
					   Rterm_private_mode_tektronix_mode)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_ALLOW_132_COLUMN_MODE,
					   Rterm_private_mode_allow_132_column_mode)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_XTERM_MORE_FIX,
					   Rterm_private_mode_xterm_more_fix)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_XTERM_MARGIN_BELL,
					   Rterm_private_mode_xterm_margin_bell)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_XTERM_REVERSE_AUTOWRAP,
					   Rterm_private_mode_xterm_reverse_autowrap)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_XTERM_LOGGING,
					   Rterm_private_mode_xterm_logging)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_SCREEN_MODE,
					   Rterm_private_mode_screen_mode)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_ETERM_KEYPAD_MODE,
					   Rterm_private_mode_eterm_keypad_mode)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_ETERM_BACKSPACE_MODE,
					   Rterm_private_mode_eterm_backspace_mode)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_XTERM_X11_MOUSE_REPORTING,
					   Rterm_private_mode_xterm_x11_mouse_reporting)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_XTERM_HIGHLIGHT_MOUSE_TRACKING,
					   Rterm_private_mode_xterm_highlight_mouse_tracking)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_ETERM_HOME_ON_ECHO,
					   Rterm_private_mode_eterm_home_on_echo)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_ETERM_HOME_ON_REFRESH,
					   Rterm_private_mode_eterm_home_on_refresh)
	< 0) {

	return -1;
    }

    if (Rterm_register_private_mode_handler(RTERM_ETERM_HOME_ON_INPUT,
					   Rterm_private_mode_eterm_home_on_input)
	< 0) {

	return -1;
    }

    return 0;
}

void
Rterm_process_private_mode(struct R_term *term, struct R_termscreen *screen, int mode, int *args, int nargs)
{
    struct R_app *app;
    R_termprivmodehandler_t *handler;
    int i, operation;

    if (term == NULL
	|| args == NULL
	|| nargs <= 0) {

	return;
    }

    for (i = 0 ; i < nargs ; i++) {
	operation = args[i];

	if (operation < 0
	    || operation > RTERM_PRIVATE_MODE_HANDLER_ARRAY_SIZE - 1) {

	    continue;
	}

	if ((handler = privatemodehandlers[operation])) {
	    handler(term, screen, mode);
	} else {
            app = R_global.app;
	    fprintf(stderr, "%s: Rterm_process_private_mode(): unknown command: %d\n", app->name, operation);
	}
    }

    return;
}

void
Rterm_private_mode_cursor_key_mode(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_APPLICATION_CURSOR_KEYS,
                       mode);

    return;
}

void
Rterm_private_mode_us_ascii_charsets(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    /* TODO: implement this. */

    return;
}

void
Rterm_private_mode_132_column_mode(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    if (term->privmodes & RTERM_PRIVMODE_ALLOW_132) {
	RTERM_SET_PRIVMODE(term,
                           RTERM_PRIVMODE_132,
                           mode);

	if (term->privmodes & RTERM_PRIVMODE_132) {
	    Rterm_resize(term,
			screen->rows,
			132);
	} else {
	    Rterm_resize(term,
			screen->rows,
			80);
	}
    }

    return;
}

void
Rterm_private_mode_scroll_mode(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    /* TODO: implement this. */

    return;
}

void
Rterm_private_mode_reverse_video_mode(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_REVERSE_VIDEO,
                       mode);

    FLAG_SET(screen->flags, RTERM_SCREEN_REVERSE_VIDEO,
	     ((term->privmodes & RTERM_PRIVMODE_REVERSE_VIDEO) ? TRUE : FALSE));
    
    Rterm_set_screen_reverse_video(screen);

    return;
}

void
Rterm_private_mode_origin_mode(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_RELATIVE_ORIGIN,
                       mode);

    FLAG_SET(screen->flags, RTERM_SCREEN_RELATIVE_ORIGIN,
	     ((term->privmodes & RTERM_PRIVMODE_RELATIVE_ORIGIN)
	      ? TRUE
	      : FALSE));

    return;
}

void
Rterm_private_mode_autowrap_mode(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_AUTOWRAP,
                       mode);

#if 0
    FLAG_SET(screen->flags, RTERM_SCREEN_AUTOWRAP,
	     ((term->privmodes & RTERM_PRIVMODE_AUTOWRAP) ? TRUE : FALSE));
#endif

    return;
}

void
Rterm_private_mode_auto_repeat_keys(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    /* TODO: implement this. */

    return;
}

void
Rterm_private_mode_start_blinking_cursor(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    /* TODO: implement this. */

    return;
}

void
Rterm_private_mode_xterm_x10_mouse_reporting(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }
    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_MOUSE_X10,
                       mode);
    
    term->privmodes &= ~RTERM_PRIVMODE_MOUSE_X11;

    return;
}

void
Rterm_private_mode_eterm_menubar_mode(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_MENUBAR,
                       mode);
    
    FLAG_SET(term->privmodes, RTERM_PRIVMODE_MENUBAR,
	     ((term->privmodes & RTERM_PRIVMODE_MENUBAR) ? TRUE : FALSE));
    
    /* FIXME: toggle menubar visibility. */

    return;
}

void
Rterm_private_mode_cursor_visibility(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_VISIBLE_CURSOR,
                       mode);

    FLAG_SET(screen->flags, RTERM_SCREEN_VISIBLE_CURSOR,
	     ((term->privmodes & RTERM_PRIVMODE_VISIBLE_CURSOR)
	      ? TRUE
	      : FALSE));
    
    /* FIXME: toggle cursor visibility. */

    return;
}

#if 0
void
Rterm_private_mode_eterm_scrollbar_mode(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_SCROLLBAR,
                       mode);

    FLAG_SET(term->privmodes, RTERM_PRIVMODE_SCROLLBAR,
	     ((term->privmodes & RTERM_PRIVMODE_SCROLLBAR) ? TRUE : FALSE));
    
    Rterm_show_scrollbar(term);

    return;
}
#endif

void
Rterm_private_mode_eterm_shift_sequences(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_SHIFT_SEQUENCES,
                       mode);

    return;
}

void
Rterm_private_mode_tektronix_mode(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    /* TODO: implement this? */

    return;
}

void
Rterm_private_mode_allow_132_column_mode(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_ALLOW_132,
                       mode);

    return;
}

void
Rterm_private_mode_xterm_more_fix(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    /* TODO: implement this? */

    return;
}

void
Rterm_private_mode_xterm_margin_bell(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    /* TODO: implement this. */

    return;
}

void
Rterm_private_mode_xterm_reverse_autowrap(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    /* TODO: implement this. */

    return;
}

void
Rterm_private_mode_xterm_logging(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    /* TODO: implement this? */

    return;
}

void
Rterm_private_mode_screen_mode(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_SECONDARY_SCREEN,
                       mode);
    
    Rterm_set_screen(term, screen);

    return;
}

void
Rterm_private_mode_eterm_keypad_mode(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_APPLICATION_KEYPAD,
                       mode);

    return;
}

void
Rterm_private_mode_eterm_backspace_mode(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_BACKSPACE,
                       mode);

    return;
}

void
Rterm_private_mode_xterm_x11_mouse_reporting(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_MOUSE_X11,
                       mode);
                        
    term->privmodes &= ~RTERM_PRIVMODE_MOUSE_X10;

    return;
}

void
Rterm_private_mode_xterm_highlight_mouse_tracking(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    /* TODO: implement this. */

    return;
}

void
Rterm_private_mode_eterm_home_on_echo(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_HOME_ON_ECHO,
                       mode);

    return;
}

void
Rterm_private_mode_eterm_home_on_refresh(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_HOME_ON_REFRESH,
                       mode);

    return;
}

void
Rterm_private_mode_eterm_home_on_input(struct R_term *term, struct R_termscreen *screen, int mode)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_PRIVMODE(term,
                       RTERM_PRIVMODE_HOME_ON_INPUT,
                       mode);

    return;
}

