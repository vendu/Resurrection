/*
 * sgr.c - SGR sequence handling for Rterm.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

typedef void R_termsgrhandler_t(struct R_term *, struct R_termscreen *screen);
static R_termsgrhandler_t *sgrhandlers[RTERM_SGR_HANDLER_ARRAY_SIZE];

void Rterm_sgr_normal(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_bold_on(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_underline_on(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_blink_on(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_reverse_on(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_bold_off(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_underline_off(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_blink_off(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_reverse_off(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_visible(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_foreground_color(struct R_term *term, struct R_termscreen *screen, int arg);
void Rterm_sgr_background_color(struct R_term *term, struct R_termscreen *screen, int arg);
void Rterm_sgr_foreground_black(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_foreground_red(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_foreground_green(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_foreground_yellow(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_foreground_blue(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_foreground_magenta(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_foreground_cyan(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_foreground_white(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_foreground_default(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_black(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_red(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_green(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_yellow(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_blue(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_magenta(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_cyan(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_white(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_default(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_green(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_yellow(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_blue(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_magenta(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_cyan(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_white(struct R_term *term, struct R_termscreen *screen);
void Rterm_sgr_background_default(struct R_term *term, struct R_termscreen *screen);

int
Rterm_register_sgr_handler(int operation, R_termsgrhandler_t *handler)
{
    if (operation < 0
	|| operation > RTERM_SGR_HANDLER_ARRAY_SIZE - 1
	|| handler == NULL) {

	return -1;
    }

    sgrhandlers[operation] = handler;

    return 0;
}

int
Rterm_register_sgr_handlers(void)
{
    if (Rterm_register_sgr_handler(RTERM_NORMAL,
                                   Rterm_sgr_normal) < 0) {
	
	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BOLD_ON,
                                   Rterm_sgr_bold_on) < 0) {
	
	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_UNDERLINE_ON,
                                   Rterm_sgr_underline_on) < 0) {
	
	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BLINK_ON,
                                   Rterm_sgr_blink_on) < 0) {
	
	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_REVERSE_ON,
                                   Rterm_sgr_reverse_on) < 0) {
	
	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BOLD_OFF,
                                   Rterm_sgr_bold_off) < 0) {
	
	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_UNDERLINE_OFF,
                                   Rterm_sgr_underline_off) < 0) {
	
	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BLINK_OFF,
                                   Rterm_sgr_blink_off) < 0) {
	
	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_REVERSE_OFF,
                                   Rterm_sgr_reverse_off) < 0) {
	
	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_VISIBLE,
                                   Rterm_sgr_visible) < 0) {
	
	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_BLACK,
                                   Rterm_sgr_foreground_black) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_RED,
                                   Rterm_sgr_foreground_red) < 0) {

	return -1;
    }
    
    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_GREEN,
                                   Rterm_sgr_foreground_green) < 0) {
        
	return -1;
    }
    
    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_YELLOW,
                                   Rterm_sgr_foreground_yellow) < 0) {
        
	return -1;
    }
    
    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_BLUE,
                                   Rterm_sgr_foreground_blue) < 0) {
        
	return -1;
    }
    
    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_MAGENTA,
                                   Rterm_sgr_foreground_magenta) < 0) {
        
	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_CYAN,
                                   Rterm_sgr_foreground_cyan) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_WHITE,
                                   Rterm_sgr_foreground_white) < 0) {

	return -1;
    }

#if 0
    /* TODO */
    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_EXTRA,
                                   Rterm_sgr_foreground_extra) < 0) {

	return -1;
    }
#endif

    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_DEFAULT,
                                   Rterm_sgr_foreground_default) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_BLACK,
                                   Rterm_sgr_background_black) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_RED,
                                   Rterm_sgr_background_red) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_GREEN,
                                   Rterm_sgr_background_green) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_YELLOW,
                                   Rterm_sgr_background_yellow) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_BLUE,
                                   Rterm_sgr_background_blue) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_MAGENTA,
                                   Rterm_sgr_background_magenta) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_CYAN,
                                   Rterm_sgr_background_cyan) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_WHITE,
                                   Rterm_sgr_background_white) < 0) {

	return -1;
    }

    /* TODO */
#if 0
    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_EXTRA,
                                   Rterm_sgr_background_default) < 0) {

	return -1;
    }
#endif

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_DEFAULT,
                                   Rterm_sgr_background_default) < 0) {

	return -1;
    }

    /* 16-color support */

    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_BLACK2,
                                   Rterm_sgr_foreground_black) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_RED2,
                                   Rterm_sgr_foreground_red) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_GREEN2,
                                   Rterm_sgr_foreground_green) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_YELLOW2,
                                   Rterm_sgr_foreground_yellow) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_BLUE2,
                                   Rterm_sgr_foreground_blue) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_MAGENTA2,
                                   Rterm_sgr_foreground_magenta) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_CYAN2,
                                   Rterm_sgr_foreground_cyan) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_FOREGROUND_WHITE2,
                                   Rterm_sgr_foreground_white) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_BLACK2,
                                   Rterm_sgr_background_black) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_RED2,
                                   Rterm_sgr_background_red) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_GREEN2,
                                   Rterm_sgr_background_green) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_YELLOW2,
                                   Rterm_sgr_background_yellow) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_BLUE2,
                                   Rterm_sgr_background_blue) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_MAGENTA2,
                                   Rterm_sgr_background_magenta) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_CYAN2,
                                   Rterm_sgr_background_cyan) < 0) {

	return -1;
    }

    if (Rterm_register_sgr_handler(RTERM_BACKGROUND_WHITE2,
                                   Rterm_sgr_background_white) < 0) {

	return -1;
    }

    return 0;
}

void
Rterm_process_sgr_attributes(struct R_term *term, struct R_termscreen *screen, int dummyi1, int dummyi2, int *args, int nargs)
{
    struct R_app *app;
    int i, operation, arg;
    R_termsgrhandler_t *handler;
    
    if (term == NULL
	|| args == NULL
	|| nargs < 0) {
	
	return;
    }
    
    if (nargs == 0) {
	screen->textflags = RTERM_SCREEN_DEFAULT_TEXTFLAGS;
	
	return;
    }

    for (i = 0 ; i < nargs ; i++) {
	operation = args[i];

	if (operation < 0 || operation > RTERM_SGR_HANDLER_ARRAY_SIZE - 1) {

	    continue;
	}

#if (SUPPORT_RTERM_256_COLORS)
        if (operation == RTERM_FOREGROUND_EXTRA) {
            if (i < nargs - 2 && args[i + 1] == 5) {
                i += 2;
                arg = args[i];
                Rterm_sgr_foreground_color(term, screen, arg);
            }
        } else if (operation == RTERM_BACKGROUND_EXTRA) {
            if (i < nargs - 2 && args[i + 1] == 5) {
                i += 2;
                arg = args[i];
                Rterm_sgr_background_color(term, screen, arg);
            }
        } else
#endif
            if ((handler = sgrhandlers[operation])) {
                handler(term, screen);
            } else {
                app = R_global.app;
                fprintf(stderr, "%s: Rterm_process_sgr_attributes(): unknown command %x\n", app->name, operation);
            }
    }
    
    return;
}

void
Rterm_sgr_normal(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    screen->textflags = RTERM_SCREEN_DEFAULT_TEXTFLAGS;

    return;
}

void
Rterm_sgr_bold_on(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    screen->textflags |= RTERM_CHAR_BOLD;

    return;
}

void
Rterm_sgr_underline_on(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    screen->textflags |= RTERM_CHAR_UNDERLINED;

    return;
}

void
Rterm_sgr_blink_on(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    screen->textflags |= RTERM_CHAR_BLINKING;

    return;
}

void
Rterm_sgr_reverse_on(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    screen->textflags |= RTERM_CHAR_REVERSE;

    return;
}

void
Rterm_sgr_bold_off(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    screen->textflags &= ~RTERM_CHAR_BOLD;

    return;
}

void
Rterm_sgr_underline_off(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    screen->textflags &= ~RTERM_CHAR_UNDERLINED;

    return;
}

void
Rterm_sgr_blink_off(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    screen->textflags &= ~RTERM_CHAR_BLINKING;

    return;
}

void
Rterm_sgr_reverse_off(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    screen->textflags &= ~RTERM_CHAR_REVERSE;

    return;
}

void
Rterm_sgr_visible(struct R_term *term, struct R_termscreen *screen)
{
    return;
}

void
Rterm_sgr_foreground_color(struct R_term *term, struct R_termscreen *screen,
                           int arg)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_FG_COLOR(screen->textflags, arg);

    return;
}

void
Rterm_sgr_background_color(struct R_term *term, struct R_termscreen *screen,
                           int arg)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_BG_COLOR(screen->textflags, arg);

    return;
}

void
Rterm_sgr_foreground_black(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_FG_COLOR(screen->textflags, RTERM_SCREEN_BLACK_COLOR);

    return;
}

void
Rterm_sgr_foreground_red(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_FG_COLOR(screen->textflags, RTERM_SCREEN_RED_COLOR);

    return;
}

void
Rterm_sgr_foreground_green(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_FG_COLOR(screen->textflags, RTERM_SCREEN_GREEN_COLOR);

    return;
}

void
Rterm_sgr_foreground_yellow(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_FG_COLOR(screen->textflags, RTERM_SCREEN_YELLOW_COLOR);

    return;
}

void
Rterm_sgr_foreground_blue(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_FG_COLOR(screen->textflags, RTERM_SCREEN_BLUE_COLOR);

    return;
}

void
Rterm_sgr_foreground_magenta(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_FG_COLOR(screen->textflags, RTERM_SCREEN_MAGENTA_COLOR);

    return;
}

void
Rterm_sgr_foreground_cyan(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_FG_COLOR(screen->textflags, RTERM_SCREEN_CYAN_COLOR);

    return;
}

void
Rterm_sgr_foreground_white(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_FG_COLOR(screen->textflags, RTERM_SCREEN_WHITE_COLOR);

    return;
}

void
Rterm_sgr_foreground_default(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_FG_COLOR(screen->textflags, RTERM_SCREEN_FOREGROUND_COLOR);

    return;
}

void
Rterm_sgr_background_black(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_BG_COLOR(screen->textflags, RTERM_SCREEN_BLACK_COLOR);

    return;
}

void
Rterm_sgr_background_red(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_BG_COLOR(screen->textflags, RTERM_SCREEN_RED_COLOR);

    return;
}

void
Rterm_sgr_background_green(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_BG_COLOR(screen->textflags, RTERM_SCREEN_GREEN_COLOR);

    return;
}

void
Rterm_sgr_background_yellow(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_BG_COLOR(screen->textflags, RTERM_SCREEN_YELLOW_COLOR);

    return;
}

void
Rterm_sgr_background_blue(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_BG_COLOR(screen->textflags, RTERM_SCREEN_BLUE_COLOR);

    return;
}

void
Rterm_sgr_background_magenta(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_BG_COLOR(screen->textflags, RTERM_SCREEN_MAGENTA_COLOR);

    return;
}

void
Rterm_sgr_background_cyan(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_BG_COLOR(screen->textflags, RTERM_SCREEN_CYAN_COLOR);

    return;
}

void
Rterm_sgr_background_white(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

    RTERM_SET_CHAR_BG_COLOR(screen->textflags, RTERM_SCREEN_WHITE_COLOR);

    return;
}

void
Rterm_sgr_background_default(struct R_term *term, struct R_termscreen *screen)
{
    if (term == NULL) {

	return;
    }

//    RTERM_SET_CHAR_BG_COLOR(screen->textflags,
//                            RTERM_SCREEN_NO_BACKGROUND_COLOR);
    screen->textflags |= RTERM_CHAR_NO_BACKGROUND;

    return;
}

