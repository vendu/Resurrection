/*
 * menu.c - menu routines for Resurrection.
 * Copyright (C) 2003 - 2005 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#if 0
/*
 * NOTES
 * - this code is one heck of a mess - I'll try to do something about it. :)
 */

#include <Resurrection/Resurrection.h>

/*
 * TODO: make menu_init() check for existence of window->menu. Is
 * menu_load_button_item_images() necessary?
 */

Rmenu_t *
menu_alloc(void)
{
    return (calloc(1, sizeof(Rmenu_t)));
}

Rmenuitem_t *
menu_alloc_item(void)
{
    return (calloc(1, sizeof(Rmenuitem_t)));
}

void
menu_copy_item_data(Rmenuitem_t *src, Rmenuitem_t *dest)
{
    memcpy(dest, src, sizeof(Rmenuitem_t));
}

Rmenu_t *
menu_init(Rwindow_t *window, Rtext_t *title)
{
    Rwindow_t *parentwindow;
    Rmenu_t *newmenu;

    if (window == NULL) {

	return NULL;
    }
 
    if (!window->gc) {
	window->gc = menu_create_gc(window);
    }
#if (SUPPORT_TRUETYPE_IMLIB2)
    if (!window->im2font) {
        window->im2font = app_load_font_imlib2(window->app, "VeraMono", 8);
    }
#endif

    newmenu = menu_alloc();
    if (newmenu == NULL) {
	
	return NULL;
    }
    if (!title) {
        newmenu->type = WIDGET_POPMENU;
    } else {
        newmenu->type = WIDGET_MENU;
    }
    parentwindow = NULL;
    if (newmenu->type == WIDGET_MENU) {
        window_set_title(window, title, False);
        parentwindow = window;
    }
    newmenu->window = window_create(window->app,
                                    parentwindow,
                                    MENU_DEFAULT_WIDTH,
                                    MENU_DEFAULT_HEIGHT);
    if (newmenu->type == WIDGET_POPMENU) {
        window_set_override_redirect(newmenu->window, True);
        window_set_save_under(newmenu->window, True);
    }
    newmenu->level = 1;
    newmenu->itemw = MENU_DEFAULT_WIDTH;
    newmenu->itemh = MENU_DEFAULT_HEIGHT;
    window->menu = newmenu;

    return newmenu;
}

Rmenuitem_t *
menu_add_item(Rmenu_t *menu, Rmenuitem_t *menuitem)
{
    Rwindow_t *window;
    Rwindow_t *parentwindow;
    Rmenuitem_t *currentitem;
    Rmenuitem_t *newmenuitem;

    window = menu->window;
    menuitem->window = window_create(window->app,
                                     window,
                                     MENU_DEFAULT_WIDTH,
                                     MENU_DEFAULT_HEIGHT);
    menuitem->window->gc = window->gc;
#if 0
    menuitem->window->im2font = window->im2font;
#endif
    if (menuitem->type == MENUITEM_SUBMENU) {
        menu_init(menuitem->window, NULL);
        menuitem->window->menu->level = menu->level + 1;
        menuitem->window->menu->prevlevel = menu;
    }
    if (menu->firstitem == NULL) {
        menu->firstitem = menuitem;
    }
    if (menu->lastitem) {
        menu->lastitem->next = menuitem;
    }
    menu->lastitem = menuitem;
    menu->nitems++;
    menu_load_images(menuitem->window);
    menu_set_item_event_handlers(menuitem->window);

    return menuitem;
}

void
menu_load_images(Rwindow_t *window)
{
    if (window == NULL) {

        return;
    }

    image_load_imlib2(&window->images[WINDOW_BACKGROUND_IMAGE],
                      RESURRECTION_IMAGE_SEARCH_PATH "misc/hbar_bronze.png");
    window->images[WINDOW_BACKGROUND_IMAGE].flags |= IMAGE_KEEP_PIXMAP;
    image_load_imlib2(&window->images[WINDOW_SELECTED_IMAGE],
                      RESURRECTION_IMAGE_SEARCH_PATH "misc/hbar_gold.png");
    window->images[WINDOW_SELECTED_IMAGE].flags |= IMAGE_KEEP_PIXMAP;
    image_load_imlib2(&window->images[WINDOW_CLICKED_IMAGE],
                      RESURRECTION_IMAGE_SEARCH_PATH "misc/hbar_gold.png");
    window->images[WINDOW_CLICKED_IMAGE].flags |= IMAGE_KEEP_PIXMAP;

    image_set_border_imlib2(&window->images[WINDOW_BACKGROUND_IMAGE],
                            3, 3, 1, 1);
    image_set_border_imlib2(&window->images[WINDOW_SELECTED_IMAGE],
                            3, 3, 1, 1);
    image_set_border_imlib2(&window->images[WINDOW_CLICKED_IMAGE],
                            3, 3, 1, 1);

    window->images[WINDOW_BACKGROUND_IMAGE].w = -1;
    window->images[WINDOW_BACKGROUND_IMAGE].h = -1;
    window->images[WINDOW_SELECTED_IMAGE].w = -1;
    window->images[WINDOW_SELECTED_IMAGE].h = -1;
    window->images[WINDOW_CLICKED_IMAGE].w = -1;
    window->images[WINDOW_CLICKED_IMAGE].h = -1;

    return;
}

void
menu_set_pop_event_handlers(Rwindow_t *window)
{
    window_add_event_handler(window, ButtonRelease, menu_buttonrelease);
    window_add_event_handler(window, EnterNotify, menu_enternotify);
    window_add_event_handler(window, LeaveNotify, menu_leavenotify);
    window_add_event_handler(window, Expose, menu_expose);
    window_add_button_action(window, BUTTON1_PRESS, menu_open);
    window_select_events(window, MENU_EVENT_MASK);

    return;
}

void
menu_set_button_event_handlers(Rwindow_t *window)
{
    if (window == NULL) {

        return;
    }

    window_add_event_handler(window, ButtonPress, menu_buttonpress);
    window_add_event_handler(window, ButtonRelease, menu_buttonrelease);
    window_add_event_handler(window, EnterNotify, menu_enternotify);
    window_add_event_handler(window, LeaveNotify, menu_leavenotify);
    window_add_event_handler(window, Expose, menu_expose);
    window_add_button_action(window, BUTTON1_PRESS, menu_open);
    window_select_events(window, MENU_EVENT_MASK);

    return;
}

/* FIXME: should implement window_destroy() and use free() more. */

void
menu_set_item_event_handlers(Rwindow_t *window)
{
    if (window == NULL) {

        return;
    }

    window_add_event_handler(window, ButtonRelease, menu_buttonrelease);
    window_add_event_handler(window, EnterNotify, menu_enternotify);
    window_add_event_handler(window, LeaveNotify, menu_leavenotify);
    window_add_event_handler(window, Expose, menu_expose);
    window_select_events(window, MENU_EVENT_MASK);

    return;
}

void
menu_finish(Rmenu_t *menu)
{
    int ndx;
    int itemw, itemh;
    Rmenuitem_t *menuitem;

    ndx = 0;
    itemw = menu->itemw;
    itemh = menu->itemh;
    menuitem = menu->firstitem;
    while (menuitem) {
        window_move_resize(menuitem->window,
                           0, ndx * itemh,
                           itemw, itemh);
        menuitem = menuitem->next;
        ndx++;
    }
    window_resize(menu->window, itemw, menu->nitems * itemh);
    window_map_subwindows(menu->window);

    return;
}

void
menu_close(int level)
{
    Rmenu_t *menu;

#if 0
    menu = Rglobals.firstmenu;
    while (level--) {
        menu = menu->next;
    }
    if (level == -1) {
        Rglobals.firstmenu = NULL;
        Rglobals.lastmenu = NULL;
    } else {
        Rglobals.lastmenu = menu;
    }
    while (menu) {
        window_unmap(menu->window);
        menu = menu->next;
    }
#endif
    menu = Rglobals.lastmenu;
    if (menu) {
        while (level > Rglobals.menulevel) {
            window_unmap(menu->window);
            menu = menu->prevlevel;
            level--;
        }
        Rglobals.menulevel = level;
        Rglobals.lastmenu = menu;
        if (menu == NULL) {
            Rglobals.firstmenu = NULL;
        }
    }

    return;
}

void
menu_set_background_color(Rmenu_t *menu, Rcolor_t color)
{
    Rmenuitem_t *menuitem;

    if (menu == NULL) {

        return;
    }

    window_set_background_color(menu->window, color);

    for (menuitem = menu->firstitem ;
         menuitem ;
         menuitem = menuitem->next) {
        window_set_background_color(menuitem->window, color);
    }

    return;
}

void
term_menu_action(Rwindow_t *window, XEvent *event)
{
    Rterm_t *term;
    Rtermscreen_t *screen;
    Rmenuitem_t *menuitem;

    if (window == NULL) {
	
        return;
    }

    menuitem = window->menuitem;
    if (menuitem == NULL) {

        return;
    }

    term = Rglobals.term;
    screen = term->window->screen;

    switch (menuitem->action) {
        case ACTION_EXEC:
            action_exec(window, &menuitem->option);

            break;
        case ACTION_TERMINAL_STRING:
            action_tty_string(window, &menuitem->option);

            break;
        case ACTION_TERMINAL_ECHO:
            action_tty_echo(window, &menuitem->option);

            break;
        default:

            break;
    }

    return;
}

int
term_init_menubar(Rterm_t *term)
{
    Rmenu_t *menu;
    
    if (term == NULL) {
	
        return -1;
    }
    
    if ((menu = menu_init(term->menuwindow, "Rterm")) == NULL) {
	
        return -1;
    }
//    menu_load_parent_images(term->menuwindow);
    window_render_background_imlib2(term->menuwindow,
                                    &term->menuwindow->images[WINDOW_BACKGROUND_IMAGE]);
    if ((menu = menu_init(term->menuwindow, TERM_NAME)) == NULL) {
	
        return -1;
    }
    if (term_init_private_modes_menu(term, menu) < 0) {

        return -1;
    }
    if (term_init_flags_menu(term, menu) < 0) {

        return -1;
    }

    return 0;
}

void
term_resize_menubar(Rterm_t *term)
{
    Rtermscreen_t *screen;

    if (term == NULL) {

        return;
    }

    screen = term->window->screen;
    window_resize(term->menuwindow,
                  screen_width(screen)
                  + ((TERM_SCROLLBAR_VISIBLE(term))
                     ? SCROLLBAR_DEFAULT_WIDTH
                     : 0),
                  MENU_DEFAULT_HEIGHT);
    window_render_background_imlib2(term->menuwindow,
                                    &term->menuwindow->images[WINDOW_BACKGROUND_IMAGE]);
    
    return;
}

int
term_init_pop_menu(Rterm_t *term)
{
    Rmenu_t *menu;
    Rtermscreen_t *screen;
    
    if (term == NULL) {
	
        return -1;
    }
    
    screen = term->screens[(term->screen)];
    if ((menu = menu_init(screen->window, NULL)) == NULL) {
	
        return -1;
    }
    if (term_init_private_modes_menu(term, menu) < 0) {

        return -1;
    }
    if (term_init_flags_menu(term, menu) < 0) {

        return -1;
    }
    menu_finish(screen->window->menu);

    return 0;
}

int
term_init_private_modes_menu(Rterm_t *term, Rmenu_t *menu)
{
    Rmenu_t *newmenu;
    Rmenuitem_t *newmenuitem;
    Rmenuitem_t *menuitem;
    
    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_SUBMENU;
    newmenuitem->text = "Private Modes";
    if ((menuitem = menu_add_item(menu, newmenuitem)) == NULL) {

        return -1;
    }

    newmenu = menuitem->window->menu;

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Application/Normal Cursor Keys";
    newmenuitem->falsetext = "(Normal)";
    newmenuitem->truetext = "(Application)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_APPLICATION_CURSOR_KEYS;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_APPLICATION_CURSOR_KEYS_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {
        
        return -1;
    }
    
    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "80/132 Column Mode";
    newmenuitem->falsetext = "(80)";
    newmenuitem->truetext = "(132)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_132;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_132_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {
	
        return -1;
    }
    
    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Normal/Reverse Video";
    newmenuitem->falsetext = "(Normal)";
    newmenuitem->truetext = "(Reverse)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_REVERSE_VIDEO;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_REVERSE_VIDEO_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {
        
        return -1;
    }
    
    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Normal/Relative Origin Mode";
    newmenuitem->falsetext = "(Normal)";
    newmenuitem->truetext = "(Relative)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_RELATIVE_ORIGIN;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_RELATIVE_ORIGIN_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {
        
        return -1;
    }
    
#if 0
    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Autowrap";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_AUTOWRAP;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_AUTOWRAP_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {
        
        return -1;
    }
#endif
    
    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Menubar";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_MENUBAR;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_MENUBAR_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Visible Cursor";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_VISIBLE_CURSOR;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_VISIBLE_CURSOR_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Scrollbar";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_SCROLLBAR;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_SCROLLBAR_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Xterm Shift + Key Sequences";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_SHIFT_SEQUENCES;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_SHIFT_SEQUENCES_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Allow 132 Column Mode";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_ALLOW_132;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_ALLOW_132_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Primary/Secondary Screen";
    newmenuitem->falsetext = "(Primary)";
    newmenuitem->truetext = "(Secondary)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_SCREEN;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_SCREEN_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Normal/Application Keypad";
    newmenuitem->falsetext = "(Normal)";
    newmenuitem->truetext = "(Application)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_APPLICATION_KEYPAD;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_APPLICATION_KEYPAD_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Home On Echo";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_HOME_ON_ECHO;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_HOME_ON_ECHO_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Home On Refresh";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_HOME_ON_REFRESH;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_HOME_ON_REFRESH_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Home On Input";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_PRIVMODE_HOME_ON_INPUT;
    newmenuitem->option.valueptr = &(term->privmodes);
    newmenuitem->option.value.str = TERM_PRIVMODE_HOME_ON_INPUT_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }
    menu_finish(newmenu);

    return 0;
}

/* TODO: implement missing escape sequences. */
int
term_init_flags_menu(Rterm_t *term, Rmenu_t *menu)
{
    Rmenu_t *newmenu;
    Rmenuitem_t *newmenuitem;
    Rmenuitem_t *menuitem;

    newmenuitem = menu_alloc_item();

    newmenuitem->type = MENUITEM_SUBMENU;
    newmenuitem->text = "Flags";
    
    if ((menuitem = menu_add_item(menu, newmenuitem)) == NULL) {

        return -1;
    }

    newmenu = menuitem->window->menu;
    
    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Transparent Background";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_TRANSPARENT;
    newmenuitem->option.valueptr = &(term->flags);
    newmenuitem->option.value.str = TERM_TRANSPARENT_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {
	
        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Scrollbar Side";
    newmenuitem->falsetext = "(Left)";
    newmenuitem->truetext = "(Right)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_SCROLLBAR_RIGHT;
    newmenuitem->option.valueptr = &(term->flags);
    newmenuitem->option.value.str = TERM_SCROLLBAR_RIGHT_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {
	
        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Floating Scrollbar";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_FLOATING_SCROLLBAR;
    newmenuitem->option.valueptr = &(term->flags);
    newmenuitem->option.value.str = TERM_FLOATING_SCROLLBAR_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {
	
        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Popup Scrollbar";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_POPUP_SCROLLBAR;
    newmenuitem->option.valueptr = &(term->flags);
    newmenuitem->option.value.str = TERM_POPUP_SCROLLBAR_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {
	
        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Visual Bell";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_VISUAL_BELL;
    newmenuitem->option.valueptr = &(term->flags);
    newmenuitem->option.value.str = TERM_VISUAL_BELL_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Map Alert";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_MAP_ALERT;
    newmenuitem->option.valueptr = &(term->flags);
    newmenuitem->option.value.str = TERM_MAP_ALERT_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Xterm Selection Behavior";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_XTERM_SELECT;
    newmenuitem->option.valueptr = &(term->flags);
    newmenuitem->option.value.str = TERM_XTERM_SELECT_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Select Whole Line on Triple-click";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_SELECT_LINE;
    newmenuitem->option.valueptr = &(term->flags);
    newmenuitem->option.value.str = TERM_SELECT_LINE_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Viewport Mode";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_VIEWPORT_MODE;
    newmenuitem->option.valueptr = &(term->flags);
    newmenuitem->option.value.str = TERM_VIEWPORT_MODE_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Report As Keysyms";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_REPORT_AS_KEYSYMS;
    newmenuitem->option.valueptr = &(term->flags);
    newmenuitem->option.value.str = TERM_REPORT_AS_KEYSYMS_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }

    newmenuitem = menu_alloc_item();
    newmenuitem->type = MENUITEM_TOGGLE;
    newmenuitem->action = ACTION_TERMINAL_STRING;
    newmenuitem->text = "Select Trailing Spaces";
    newmenuitem->falsetext = "(Off)";
    newmenuitem->truetext = "(On)";
    newmenuitem->option.type = OPTION_STRING;
    newmenuitem->option.flag = TERM_SELECT_TRAILING_SPACES;
    newmenuitem->option.valueptr = &(term->flags);
    newmenuitem->option.value.str = TERM_SELECT_TRAILING_SPACES_STRING;
    newmenuitem->option.handler = term_menu_action;
    if ((menuitem = menu_add_item(newmenu, newmenuitem))
        == NULL) {

        return -1;
    }
    menu_finish(newmenu);

    return 0;
}
#endif /* 0 */
