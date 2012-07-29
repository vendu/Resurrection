/*
 * csi.c - CSI sequence handling for Rterm.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#define ETERM_BEHAVIOR 0

#include <Resurrection/Resurrection.h>

#define RTERM_CSI_HANDLER_ARRAY_SIZE    (UCHAR_MAX + 1)

typedef void R_termcsihandler_t(struct R_term *, struct R_termscreen *, int, int, int *, int);

static R_termcsihandler_t *csihandlers[RTERM_CSI_HANDLER_ARRAY_SIZE];

/* csi */
void Rterm_csi_insert_blanks(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2, int *args, int dummyi3);
void Rterm_csi_cursor_up(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                         int *args, int dummyi3);
void Rterm_csi_cursor_down(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                           int *args, int dummyi3);
void Rterm_csi_cursor_forward(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                              int *args, int dummyi3);
void Rterm_csi_cursor_backwards(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                                int *args, int dummyi3);
void Rterm_csi_cursor_down_and_column_0(struct R_term *term, struct R_termscreen *screen, int dummyi1,
                                        int dummyi2,
                                        int *args, int dummyi3);
void Rterm_csi_cursor_up_and_column_0(struct R_term *term, struct R_termscreen *screen, int dummyi1,
                                      int dummyi2,
                                      int *args, int dummyi3);
void Rterm_csi_cursor_to_column(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                                int *args, int dummyi3);
void Rterm_csi_cursor_position(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                               int *args, int nargs);
void Rterm_csi_forward_tab_stops(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                                 int *args, int dummyi3);
void Rterm_csi_erase_in_display(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                                int *args, int dummyi3);
void Rterm_csi_erase_in_line(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                             int *args, int dummyi3);
void Rterm_csi_insert_lines(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                            int *args, int dummyi3);
void Rterm_csi_delete_lines(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                            int *args, int dummyi3);
void Rterm_csi_delete_chars(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                            int *args, int dummyi3);
void Rterm_csi_scroll_up(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                            int *args, int dummyi3);
void Rterm_csi_scroll_down(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                            int *args, int dummyi3);
void Rterm_csi_tab_functions(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                             int *args, int dummyi3);
void Rterm_csi_backwards_tab_stops(struct R_term *term, struct R_termscreen *screen, int dummyi1,
                                   int dummyi2,
                                   int *args, int dummyi3);
#if (USE_VT100_ANSWER)
void Rterm_csi_device_attributes(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                                 int *args, int dummyi3);
#endif
void Rterm_csi_cursor_to_row(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                             int *args, int dummyi3);
void Rterm_csi_tab_clear(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                         int *args, int dummyi3);
void Rterm_csi_process_mode(struct R_term *term, struct R_termscreen *screen, int firstch, int mode,
                            int *args, int nargs);
void Rterm_csi_print_mode(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                          int *args, int dummyi3);
void Rterm_csi_device_status(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                             int *args, int nargs);
void Rterm_csi_set_scroll_region(struct R_term *term, struct R_termscreen *screen, int firstch, int ch,
                                 int *args, int nargs);
void Rterm_csi_save_cursor(struct R_term *term, struct R_termscreen *screen, int firstch, int ch,
                           int *args, int nargs);
void Rterm_csi_xterm_window_operation(struct R_term *term, struct R_termscreen *screen, int firstch, int ch,
                                      int *args, int nargs);
void Rterm_csi_restore_cursor(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                              int *dummyiptr, int dummyi3);
void Rterm_csi_request_parameters(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                                  int *dummyiptr, int dummyi3);
/* sgr */
void Rterm_process_sgr_attributes(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                                  int *args, int nargs);

int
Rterm_register_csi_handler(int operation, R_termcsihandler_t *handler)
{
    if (operation < 0
	|| operation > RTERM_CSI_HANDLER_ARRAY_SIZE - 1
	|| handler == NULL) {

	return -1;
    }

    csihandlers[operation] = handler;

    return 0;
}

int
Rterm_register_csi_handlers(void)
{
    if (Rterm_register_csi_handler(RTERM_INSERT_BLANKS,
                                   Rterm_csi_insert_blanks) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_CURSOR_UP,
                                   Rterm_csi_cursor_up) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_CURSOR_DOWN,
                                   Rterm_csi_cursor_down) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_CURSOR_FORWARD,
                                   Rterm_csi_cursor_forward) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_CURSOR_BACKWARDS,
                                   Rterm_csi_cursor_backwards) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_CURSOR_DOWN_AND_COLUMN_0,
                                   Rterm_csi_cursor_down_and_column_0) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_CURSOR_UP_AND_COLUMN_0,
                                   Rterm_csi_cursor_up_and_column_0) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_CURSOR_TO_COLUMN,
                                   Rterm_csi_cursor_to_column) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_CURSOR_POSITION,
                                   Rterm_csi_cursor_position) < 0) {

	return -1;
    }


    if (Rterm_register_csi_handler(RTERM_FORWARD_TAB_STOPS,
                                   Rterm_csi_forward_tab_stops) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_ERASE_IN_DISPLAY,
                                   Rterm_csi_erase_in_display) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_ERASE_IN_LINE,
                                   Rterm_csi_erase_in_line) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_INSERT_LINES,
                                   Rterm_csi_insert_lines) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_DELETE_LINES,
                                   Rterm_csi_delete_lines) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_DELETE_CHARS,
                                   Rterm_csi_delete_chars) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_SCROLL_UP,
                                   Rterm_csi_scroll_up) < 0) {

        return -1;
    }

    if (Rterm_register_csi_handler(RTERM_SCROLL_DOWN,
                                   Rterm_csi_scroll_up) < 0) {

        return -1;
    }

    if (Rterm_register_csi_handler(RTERM_TAB_FUNCTIONS,
                                   Rterm_csi_tab_functions) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_ERASE_CHARS,
                                   Rterm_csi_delete_chars) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_BACKWARDS_TAB_STOPS,
                                   Rterm_csi_backwards_tab_stops) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_MOVE_TO_COLUMN,
                                   Rterm_csi_cursor_to_column) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_MOVE_FORWARD,
                                   Rterm_csi_cursor_forward) < 0) {

	return -1;
    }

#if (USE_VT100_ANSWER)

    if (Rterm_register_csi_handler(RTERM_DEVICE_ATTRIBUTES,
                                   Rterm_csi_device_attributes) < 0) {

	return -1;
    }

#endif /* USE_VT100_ANSWER */

    if (Rterm_register_csi_handler(RTERM_CURSOR_TO_ROW,
                                   Rterm_csi_cursor_to_row) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_MOVE_UP,
                                   Rterm_csi_cursor_up) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_MOVE_TO,
                                   Rterm_csi_cursor_position) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_SET_SCROLL_REGION,
                                   Rterm_csi_set_scroll_region) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_TAB_CLEAR,
                                   Rterm_csi_tab_clear) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_SET_MODE,
                                   Rterm_csi_process_mode) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_PRINT_MODE,
                                   Rterm_csi_print_mode) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_RESET_MODE,
                                   Rterm_csi_process_mode) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_SGR_ATTRIBUTES,
                                   Rterm_process_sgr_attributes) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_DEVICE_STATUS,
                                   Rterm_csi_device_status) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_SAVE_CURSOR_2,
                                   Rterm_csi_save_cursor) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_XTERM_WINDOW_OPERATION,
                                   Rterm_csi_xterm_window_operation) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_RESTORE_CURSOR_2,
                                   Rterm_csi_restore_cursor) < 0) {

	return -1;
    }

    if (Rterm_register_csi_handler(RTERM_REQUEST_PARAMETERS,
                                   Rterm_csi_request_parameters) < 0) {

	return -1;
    }

    return 0;
}

void
Rterm_process_csi_sequence(struct R_term *term, struct R_termscreen *screen)
{
    int ch, firstch;
    int ignore;
    int nargs = 0, arg = 0;
    int args[RTERM_MAX_ESCAPE_ARGS];
    R_termcsihandler_t *handler;
    int (*read_char)(struct R_term *);
    void (*add_text)();

    if (term == NULL) {

	return;
    }
    read_char = term->funcs.read_char;
    add_text = screen->funcs.add_text;

    memset(args, 0, sizeof(args));

    ignore = 0;
    if ((ch = read_char(term)) < 0) {

	return;
    }

    firstch = RTERM_SET_MODES;

    if (ch >= '<' && ch <= '?') {
	firstch = ch;

	if ((ch = read_char(term)) < 0) {

	    return;
	}
    }

    /* read numerical arguments. */

    do {
	for (arg = 0 ; isdigit(ch) ; ch = read_char(term)) {
	    if (ch < 0) {

		return;
	    }

	    arg = arg * 10 + ch - '0';
	}

	if (nargs < RTERM_MAX_ESCAPE_ARGS) {
	    args[nargs++] = arg;
	}

	if (ch == R_BACKSPACE_CHAR) {
	    Rterm_screen_backspace(screen);
	} else if ((ch == R_ESC_CHAR)) {
#if !(ETERM_BEHAVIOR)
	    Rterm_process_escape_sequence(term);
#else
            screen->inptr--;

            return;
#endif

	    return;
	} else if (ch < ' ') {
	    add_text(screen, (R_text_t *)&ch, 1, 0);

	    return;
	}
        if (ch == '-') {
            ignore = 1;
        }

	if (ch < '@') {
	    if ((ch = read_char(term)) < 0) {

		return;
	    }
	}
    } while (ch >= ' ' && ch < '@');

    if (ch == R_ESC_CHAR) {
#if (!ETERM_BEHAVIOR)
	Rterm_process_escape_sequence(term);
#else
        screen->inptr--;

        return;
#endif

	return;
    } else if (ch < ' ') {

	return;
    }
    if (ignore) {

        return;
    }

    if (ch < 0 || ch > RTERM_CSI_HANDLER_ARRAY_SIZE - 1) {

	return;
    }

    if ((handler = csihandlers[ch])) {
	handler(term, screen, firstch, ch, args, nargs);
    } else {
	fprintf(stderr, "%s: Rterm_process_csi_sequence(): unknown command: firstch == %x, ch == %x\n", term->window->app->name, firstch, ch);
    }

    return;
}

void Rterm_csi_insert_blanks(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2, int *args, int dummyi3)
{
    void (*insert_blank)(struct R_termscreen *, int);
    if (term == NULL
	|| args == NULL) {

	return;
    }
    insert_blank = screen->funcs.insert_blank;
    if (insert_blank == NULL) {

	return;
    }

    insert_blank(screen, (args[0] ? args[0] : 1));

    return;
}

void
Rterm_csi_cursor_up(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                    int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_screen_goto(screen, (args[0] ? -args[0] : -1), 0,
		RTERM_SCREEN_RELATIVE);

    return;
}

void
Rterm_csi_cursor_down(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                      int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_screen_goto(screen, (args[0] ? args[0] : 1), 0,
		RTERM_SCREEN_RELATIVE);

    return;
}

void
Rterm_csi_cursor_forward(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                         int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_screen_goto(screen, 0, (args[0] ? args[0] : 1),
		RTERM_SCREEN_RELATIVE);

    return;
}

void
Rterm_csi_cursor_backwards(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                           int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_screen_goto(screen, 0, (args[0] ? -args[0] : -1),
		RTERM_SCREEN_RELATIVE);

    return;
}

void
Rterm_csi_cursor_down_and_column_0(struct R_term *term, struct R_termscreen *screen, int dummyi1,
                                   int dummyi2,
                                   int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_screen_goto(screen, (args[0] ? args[0] : 1), 0,
		RTERM_SCREEN_ROW_RELATIVE);

    return;
}

void
Rterm_csi_cursor_up_and_column_0(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                                 int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_screen_goto(screen, (args[0] ? -args[0] : -1), 0,
		RTERM_SCREEN_ROW_RELATIVE);

    return;
}

void
Rterm_csi_cursor_to_column(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                           int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_screen_goto(screen,
		0, (args[0] ? args[0] - 1 : screen->column),
		RTERM_SCREEN_ROW_RELATIVE);

    return;
}

void
Rterm_csi_cursor_position(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                          int *args, int nargs)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    switch (nargs) {
        case 0:
	    Rterm_screen_goto(screen,
			0, 0,
			0);

	    break;
        case 1:
	    Rterm_screen_goto(screen,
			(args[0] ? args[0] - 1 : 0), 0,
			0);

	    break;
        default:
	    Rterm_screen_goto(screen,
			args[0] - 1, args[1] - 1,
			0);

	    break;
    }

    return;
}

void
Rterm_csi_forward_tab_stops(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                            int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_screen_tab(screen, (args[0] ? args[0] : 1));

    return;
}

void
Rterm_csi_erase_in_display(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                           int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_blank_screen(screen, args[0]);

    return;
}

void
Rterm_csi_erase_in_line(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                        int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_blank_screen_row(screen, args[0]);

    return;
}

void
Rterm_csi_insert_lines(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                       int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_insert_blank_screen_rows(screen, (args[0] ? args[0] : 1));

    return;
}

void
Rterm_csi_delete_lines(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                       int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_delete_screen_rows(screen, (args[0] ? args[0] : 1));

    return;
}

void
Rterm_csi_delete_chars(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                       int *args, int dummyi3)
{
    void (*delete_string)(struct R_termscreen *, int);
    if (term == NULL
	|| args == NULL) {

	return;
    }
    delete_string = screen->funcs.delete_string;
    if (delete_string == NULL) {

	return;
    }

    delete_string(screen, (args[0] ? args[0] : 1));

    return;
}

void
Rterm_csi_scroll_up(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                       int *args, int dummyi3)
{
    int (*scroll_text)(struct R_termscreen *, int, int, int, int);
    if (term == NULL
	|| args == NULL) {

	return;
    }
    scroll_text = screen->funcs.scroll_text;

    scroll_text(screen, screen->scrolltop, screen->scrollbottom, (args[0] ? args[0] : 1), TRUE);

    return;
}

void
Rterm_csi_scroll_down(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                       int *args, int dummyi3)
{
    int (*scroll_text)(struct R_termscreen *, int, int, int, int);
    if (term == NULL
	|| args == NULL) {

	return;
    }
    scroll_text = screen->funcs.scroll_text;

    scroll_text(screen, screen->scrolltop, screen->scrollbottom, (args[0] ? -(args[0]) : -1), TRUE);

    return;
}

void
Rterm_csi_tab_functions(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                        int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    switch (args[0]) {
        case RTERM_TAB_FUNCTION_SET:
	    Rterm_set_screen_tab(screen, RTERM_SCREEN_SET_TAB);

	    break;
        case RTERM_TAB_FUNCTION_CLEAR:
	    Rterm_set_screen_tab(screen, RTERM_SCREEN_CLEAR_TAB);

	    break;
        case RTERM_TAB_FUNCTION_CLEAR_ALL:
	    Rterm_set_screen_tab(screen, RTERM_SCREEN_CLEAR_ALL_TABS);

	    break;
        default:

	    break;
    }

    return;
}

void
Rterm_csi_backwards_tab_stops(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                              int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_screen_tab(screen, (args[0] ? -args[0] : -1));

    return;
}

#if (USE_VT100_ANSWER)

void
Rterm_csi_device_attributes(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                            int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    RTERM_WRITE_PTY(term, screen->pty.masterfd, RTERM_VT100_ANSWER, sizeof(RTERM_VT100_ANSWER - 1);

    return;
}

#endif /* USE_VT100_ANSWER */

void
Rterm_csi_cursor_to_row(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                        int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    Rterm_screen_goto(screen,
		(args[0] ? args[0] - 1 : screen->row), 0,
		RTERM_SCREEN_COLUMN_RELATIVE);

    return;
}

void
Rterm_csi_tab_clear(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                    int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    switch (args[0]) {
        case RTERM_SCREEN_CLEAR_TAB:
	    Rterm_set_screen_tab(screen, RTERM_SCREEN_CLEAR_TAB);

	    break;
        case RTERM_SCREEN_CLEAR_ALL_TABS:
	    Rterm_set_screen_tab(screen, RTERM_SCREEN_CLEAR_ALL_TABS);

	    break;
        default:

	    break;
    }

    return;
}

void
Rterm_csi_process_mode(struct R_term *term, struct R_termscreen *screen, int firstch, int mode,
                       int *args, int nargs)
{
    int i;
    
    if (term == NULL
	|| args == NULL) {
	
	return;
    }

    switch (mode) {
        case RTERM_SET_MODE:
	    mode = 1;

	    break;
        case RTERM_RESET_MODE:
	    mode = 0;

	    break;
        default:

	    break;
    }

    switch (firstch) {
	/* FIXME: is this really necessary/correct? */
        case RTERM_SET_MODES:
	    if (mode && mode != 1) {
		
		return;
	    }

	    for (i = 0 ; i < nargs ; i++) {
		switch (args[i]) {
		    case RTERM_INSERT_MODE:

			/* FIXME should I just set, not toggle, flags here? */
			RTERM_SET_PRIVMODE(term,
                                           RTERM_PRIVMODE_INSERT,
                                           mode);

#if 0
			FLAG_SET(screen->flags, RTERM_SCREEN_INSERT,
				 ((term->privmodes & RTERM_PRIVMODE_INSERT)
				  ? TRUE
				  : FALSE));
#endif

			break;
		    default:

			break;
		}
	    }
	    break;
        case RTERM_PRIVATE_MODE:
	    Rterm_process_private_mode(term, screen, mode, args, nargs);

	    break;
        default:
	    fprintf(stderr, "%s: Rterm_csi_process_mode(): unknown commnd %c\n",
		    term->window->app->name, firstch);

	    break;
    }

    return;
}

void
Rterm_csi_print_mode(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                     int *args, int dummyi3)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    switch (args[0]) {
        case RTERM_PRINT_VISIBLE_ROWS:
#if 0
	    Rterm_print_screen(screen, RTERM_SCREEN_PRINT_VISIBLE_ROWS);
#endif

	    break;
        case RTERM_PRINT_DISABLE_TRANSPARENT:

	    break;
        case RTERM_PRINT_ENABLE_TRANSPARENT:
	    /* would this be RTERM_SCREEN_PRINT_ALL_ROWS? */

	    break;
        default:

	    break;
    }

    return;
}

void
Rterm_csi_device_status(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                        int *args, int nargs)
{
    char str[RTERM_ESC_REPLY_LENGTH];
#if (USE_DISPLAY_ANSWER)
    char *displayname;
#endif /* USE_DISPLAY_ANSWER */

    if (term == NULL
	|| args == NULL
	|| nargs < 1) {

	return;
    }

    switch (args[0]) {
        case RTERM_STATUS_REPORT:
            RTERM_WRITE_PTY(term, screen->pty.masterfd, RTERM_STATUS_STRING, sizeof(RTERM_STATUS_STRING - 1));

	    break;
        case RTERM_REPORT_CURSOR_POSITION:
	    snprintf(str, sizeof(str), "\033[%d;%dR",
		     screen->row + 1, screen->column + 1);
            RTERM_WRITE_PTY(term, screen->pty.masterfd, str, strlen(str));

	    break;
#if (USE_DISPLAY_ANSWER)
        case RTERM_XTERM_REQUEST_DISPLAY_NAME:
	    displayname = DisplayString(term->window->app->display);

            RTERM_WRITE_PTY(term, screen->pty.masterfd, displayname, strlen(displayname));
            RTERM_WRITE_PTY(term, screen->pty.masterfd, "\n", 1);

	    break;
#endif /* USE_DISPLAY_ANSWER */
        case RTERM_XTERM_REQUEST_VERSION_IN_TITLE:
	    Rterm_do_xterm_sequence(term, RTERM_XTERM_CHANGE_WINDOW_TITLE,
                              RTERM_NAME "-" RTERM_VERSION);

	    break;
        case RTERM_ETERM_IMAGE_ATTRIBUTES:
	    /* TODO: implement this. */

	    break;
        default:

	    break;
    }

    return;
}

void
Rterm_csi_set_scroll_region(struct R_term *term, struct R_termscreen *screen, int firstch, int ch,
                            int *args, int nargs)
{
    if (term == NULL
	|| args == NULL) {

	return;
    }

    if (firstch != '?') {
	if (nargs >= 2) {
	    Rterm_set_screen_scroll_region(screen,
                                           args[0] - 1, args[1] - 1);
	}

	return;
    }
    
    Rterm_csi_xterm_window_operation(term, screen, firstch, ch, args, nargs);

    return;
}

void
Rterm_csi_save_cursor(struct R_term *term, struct R_termscreen *screen, int firstch, int ch,
                      int *args, int nargs)
{
    if (term == NULL) {

	return;
    }

    if (ch == RTERM_SAVE_CURSOR_2 && nargs == 0) {
	Rterm_escape_save_cursor(term, screen);

	return;
    }

    Rterm_csi_process_mode(term, screen, firstch, ch, args, nargs);

    return;
}

void
Rterm_csi_xterm_window_operation(struct R_term *term, struct R_termscreen *screen, int firstch, int ch,
                                 int *args, int nargs)
{
    if (term == NULL) {

	return;
    }

    if (firstch != '?') {
	Rterm_process_xterm_window_operation(term, screen, args, nargs);
	
	return;
    }

    Rterm_csi_save_cursor(term, screen, firstch, ch, args, nargs);
    
    return;
}

void
Rterm_csi_restore_cursor(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                         int *dummyiptr, int dummyi3)
{
    if (term == NULL) {

	return;
    }

    Rterm_escape_restore_cursor(term);

    return;
}

/* TODO: should this be implemented? */

void
Rterm_csi_request_parameters(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2,
                             int *dummyiptr, int dummyi3)
{
    ;
}

