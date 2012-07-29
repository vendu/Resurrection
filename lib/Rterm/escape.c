/*
 * escape.c - escape sequence handling for Rterm.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

typedef void R_termescapehandler_t(struct R_term *, struct R_termscreen *screen);

static R_termescapehandler_t *escapehandlers[RTERM_ESCAPE_HANDLER_ARRAY_SIZE];

void Rterm_process_csi_sequence(struct R_term *term, struct R_termscreen *screen);
void Rterm_process_xterm_sequence(struct R_term *term, struct R_termscreen *screen);
#if (SUPPORT_DUALBYTE_CHARACTERS)
void Rterm_escape_charset_kanji(struct R_term *term, struct R_termscreen *screen);
#endif
void Rterm_escape_charset_g0(struct R_term *term, struct R_termscreen *screen);
void Rterm_escape_charset_g1(struct R_term *term, struct R_termscreen *screen);
void Rterm_escape_charset_g2(struct R_term *term, struct R_termscreen *screen);
void Rterm_escape_charset_g3(struct R_term *term, struct R_termscreen *screen);
void Rterm_escape_save_cursor(struct R_term *term, struct R_termscreen *screen);
void Rterm_escape_restore_cursor(struct R_term *term, struct R_termscreen *screen);
void Rterm_escape_application_keypad(struct R_term *term, struct R_termscreen *screen);
void Rterm_escape_normal_keypad(struct R_term *term, struct R_termscreen *screen);
void Rterm_escape_index(struct R_term *term, struct R_termscreen *screen);
void Rterm_escape_next_line(struct R_term *term, struct R_termscreen *screen);
void Rterm_escape_tab_set(struct R_term *term, struct R_termscreen *screen);
void Rterm_escape_reverse_index(struct R_term *term, struct R_termscreen *screen);
#if (USE_VT100_ANSWER)
void Rterm_escape_obsolete_attributes(struct R_term *term, struct R_termscreen *screen);
#endif
void Rterm_escape_full_reset(struct R_term *term, struct R_termscreen *screen);
void Rterm_escape_g2_charset(struct R_term *term, struct R_termscreen *screen);
void Rterm_escape_g3_charset(struct R_term *term, struct R_termscreen *screen);

/* FIXME: find out what to do with '@' and 'G' and check for others. */

int
Rterm_register_escape_handler(int operation, R_termescapehandler_t *handler)
{
    if (operation < 0
	|| operation > RTERM_ESCAPE_HANDLER_ARRAY_SIZE - 1
	|| handler == NULL) {

	return -1;
    }

    escapehandlers[operation] = handler;

    return 0;
}

int
Rterm_register_escape_handlers(void)
{
#if (SUPPORT_DUALBYTE_CHARACTERS)

    if (Rterm_register_escape_handler(RTERM_CHARSET_KANJI_ESCAPE,
                                      Rterm_escape_charset_kanji) < 0) {

	return -1;
    }

#endif /* (SUPPORT_DUALBYTE_CHARACTERS) */

    if (Rterm_register_escape_handler(RTERM_CHARSET_G0_ESCAPE,
                                      Rterm_escape_charset_g0) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_CHARSET_G1_ESCAPE,
                                      Rterm_escape_charset_g1) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_CHARSET_G2_ESCAPE,
                                      Rterm_escape_charset_g2) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_CHARSET_G3_ESCAPE,
                                      Rterm_escape_charset_g3) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_SAVE_CURSOR,
                                      Rterm_escape_save_cursor) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_RESTORE_CURSOR,
                                      Rterm_escape_restore_cursor) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_APPLICATION_KEYPAD,
                                      Rterm_escape_application_keypad) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_NORMAL_KEYPAD,
                                      Rterm_escape_normal_keypad) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_INDEX,
                                      Rterm_escape_index) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_NEXT_LINE,
                                      Rterm_escape_next_line) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_TAB_SET,
                                      Rterm_escape_tab_set) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_REVERSE_INDEX,
                                      Rterm_escape_reverse_index) < 0) {

	return -1;
    }

#if (USE_VT100_ANSWER)

    if (Rterm_register_escape_handler(RTERM_OBSOLETE_ATTRIBUTES,
                                      Rterm_escape_obsolete_attributes) < 0) {

	return -1;
    }

#endif /* USE_VT100_ANSWER */

    if (Rterm_register_escape_handler(RTERM_CSI_SEQUENCE,
                                      Rterm_process_csi_sequence) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_XTERM_SEQUENCE,
                                      Rterm_process_xterm_sequence) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_FULL_RESET,
                                      Rterm_escape_full_reset) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_G2_CHARSET,
                                      Rterm_escape_g2_charset) < 0) {

	return -1;
    }

    if (Rterm_register_escape_handler(RTERM_G3_CHARSET,
                                      Rterm_escape_g3_charset) < 0) {

	return -1;
    }

    return 0;
}

void
Rterm_process_escape_sequence(struct R_term *term, struct R_termscreen *screen)
{
    int ch;
    R_termescapehandler_t *handler;
    int (*read_char)(struct R_term *);
    
    if (term == NULL) {
	
	return;
    }
    
    read_char = term->funcs.read_char;
    if (read_char == NULL) {

	return;
    }
    ch = read_char(term);

    if (ch < 0 || ch > RTERM_ESCAPE_HANDLER_ARRAY_SIZE - 1) {

	return;
    }
    
    if ((handler = escapehandlers[ch])) {
	handler(term, screen);
    } else {
	fprintf(stderr, "Rterm_process_escape_sequence(): unknown command '%x'\n", ch);
    }

    return;
}

#if (SUPPORT_DUALBYTE_CHARACTERS)

void
Rterm_escape_charset_kanji(struct R_term *term, struct R_termscreen *screen)
{
    int ch;
    int (*read_char)(struct R_term *);

    if (term == NULL) {

	return;
    }
    read_char = term->funcs.read_char;
    if (read_char == NULL) {

	return;
    }

    if ((ch = read_char(term)) < 0) {

	return;
    }

    Rterm_set_charset(term, RTERM_CHARSET_KANJI, ch);

    return;
}

#endif /* SUPPORT_DUALBYTE_CHARACTERS */

void
Rterm_escape_charset_g0(struct R_term *term, struct R_termscreen *screen)
{
    int ch;
    int (*read_char)(struct R_term *);

    if (term == NULL) {

	return;
    }
    read_char = term->funcs.read_char;
    if (read_char == NULL) {

	return;
    }

    if ((ch = read_char(term)) < 0) {

	return;
    }

    Rterm_set_charset(term, RTERM_CHARSET_G0, ch);

    return;
}

void
Rterm_escape_charset_g1(struct R_term *term, struct R_termscreen *screen)
{
    int ch;

    if (term == NULL) {

	return;
    }

    if ((ch = Rterm_read_char(term)) < 0) {

	return;
    }

    Rterm_set_charset(term, RTERM_CHARSET_G1, ch);

    return;
}

void
Rterm_escape_charset_g2(struct R_term *term, struct R_termscreen *screen)
{
    int ch;
    int (*read_char)(struct R_term *);

    if (term == NULL) {

	return;
    }
    read_char = term->funcs.read_char;
    if (read_char == NULL) {

	return;
    }

    if ((ch = read_char(term)) < 0) {

	return;
    }

    Rterm_set_charset(term, RTERM_CHARSET_G2, ch);

    return;
}

void
Rterm_escape_charset_g3(struct R_term *term, struct R_termscreen *screen)
{
    int ch;
    int (*read_char)(struct R_term *);

    if (term == NULL) {

	return;
    }
    read_char = term->funcs.read_char;
    if (read_char == NULL) {

	return;
    }

    if ((ch = read_char(term)) < 0) {

	return;
    }

    Rterm_set_charset(term, RTERM_CHARSET_G3, ch);

    return;
}

void
Rterm_escape_save_cursor(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    screen->savedcursor.textflags = screen->textflags;
    screen->savedcursor.row = screen->row;
    screen->savedcursor.column = screen->column;
    screen->savedcursor.charset = screen->charset;
    screen->savedcursor.charset_char = term->charsets[screen->charset];

    return;
}

void
Rterm_escape_restore_cursor(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    screen->textflags =	screen->savedcursor.textflags;
    screen->row = screen->savedcursor.row;
    screen->column = screen->savedcursor.column;
    screen->charset = screen->savedcursor.charset;
    term->charsets[(screen->charset)] = screen->savedcursor.charset_char;
    
    Rterm_set_font_flag(term);

    return;
}

void
Rterm_escape_application_keypad(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    term->privmodes |= RTERM_PRIVMODE_APPLICATION_KEYPAD;

    return;
}

void
Rterm_escape_normal_keypad(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    term->privmodes &= ~RTERM_PRIVMODE_APPLICATION_KEYPAD;

    return;
}

void
Rterm_escape_index(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    Rterm_screen_index(screen, RTERM_SCREEN_INDEX_UP);

    return;
}

void
Rterm_escape_next_line(struct R_term *term, struct R_termscreen *screen)
{
    int (*add_text)();
    if (term == NULL) {

	return;
    }

    add_text = screen->funcs.add_text;
    if (add_text == NULL) {

	return;
    }

    add_text(screen, "\n\r", 2, 1);

#if 0
    if (term->privmodes & RTERM_PRIVMODE_SCROLLBAR) {
	Rterm_update_scrollbar(term);
    }
#endif

    return;
}

void
Rterm_escape_tab_set(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    Rterm_set_screen_tab(screen, RTERM_SCREEN_SET_TAB);

    return;
}

void
Rterm_escape_reverse_index(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    Rterm_screen_index(screen, RTERM_SCREEN_INDEX_DOWN);

    return;
}

#if (USE_VT100_ANSWER)

void
Rterm_escape_obsolete_attributes(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_WRITE_PTY(term, screen->pty.masterfd, RTERM_VT100_ANSWER, sizeof(RTERM_VT100_ANSWER) - 1);

    return;
}

#endif /* USE_VT100_ANSWER */

void
Rterm_escape_full_reset(struct R_term *term, struct R_termscreen *screen)
{
    int i;
    if (term == NULL) {
	
	return;
    }

    for (i = 0 ; i < term->nscreens ; i++) {
        Rterm_reset_screen(term->screens[i]);
    }

    return;
}

void
Rterm_escape_g2_charset(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    Rterm_choose_charset(term, RTERM_CHARSET_G2);

    return;
}

void
Rterm_escape_g3_charset(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    Rterm_choose_charset(term, RTERM_CHARSET_G3);

    return;
}

