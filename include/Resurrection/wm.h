/*
 * wm.h - Rwm header file.
 * Copyright (C) 2007 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef __RESURRECTION_WM_H__
#define __RESURRECTION_WM_H__

#include <Resurrection/conf/wm.h>

#include <Resurrection/global.h>
extern struct Rwm_configure_info Rwm_action_info;

#define RWM_DELETE_WINDOW_PROTO   0
#define RWM_TAKE_FOCUS_PROTO      1
//#define RWM_SYNCH_PROTO           2
#define RWM_PROTOS                2

#define RWM_DELETE_WINDOW_FLAG    0x00000001
#define RWM_TAKE_FOCUS_FLAG       0x00000002

#define RWM_PROTOCOLS_ATOM        0
#define RWM_MOTIF_WM_HINTS_ATOM   1
#define RWM_NET_WM_NAME_ATOM      2
#define RWM_NET_WM_ICON_NAME_ATOM 3
#define RWM_NET_WM_USER_TIME_ATOM 4
#define RWM_ATOMS                 5

/* conf.h options for wm.optflags. */
#define RWM_CLICK_FOCUS_FLAG         0x00000001
#define RWM_POINTER_FOCUS_FLAG       0x00000002
#define RWM_SLOPPY_FOCUS_FLAG        0x00000004
#define RWM_ASYNCHRONOUS_MOVE_FLAG   0x00000008
#define RWM_ASYNCHRONOUS_RESIZE_FLAG 0x00000010
#define RWM_CLICK_RAISE_FLAG         0x00000020
#define RWM_CLICK_LOWER_FLAG         0x00000040
#define RWM_CLICK_MENU_FLAG          0x00000080
#define RWM_DESKTOP_CLOCK_FLAG       0x00000100
#define RWM_FOCUS_CLOCK_FLAG         0x00000200
#define RWM_CLOCK_12_FLAG            0x00000400
#define RWM_SAVE_UNDER_FLAG          0x00000020
#define RWM_DEFAULT_FLAGS            (RWM_CLICK_RAISE_FLAG | RWM_FOCUS_CLOCK_FLAG)

/* wm.optflags */
#define RWM_SHAPE_EXTENSION_FLAG     0x00000001
#define RWM_COMPOSITE_EXTENSION_FLAG 0x00000002
#define RWM_XRENDER_EXTENSION_FLAG   0x00000004

/* wm.sysflags */
#define RWM_VISUAL_ALPHA_FLAG        0x00000001

/* misc kludges. */
#define USE_NEW_MENU          1

#define RWM_DESKTOPS           4
#define RWM_MENU_ITEMS         7
#define RWM_MENU_RULER_WIDTH   4
#define RWM_MENU_RULER_HEIGHT  4
#define RWM_MENU_ITEM_WIDTH    96
#define RWM_MENU_ITEM_HEIGHT   24
#if 0
#define RWM_MENU_ITEM_HEIGHT   40
#endif
#define RWM_PAGER_THUMB_WIDTH  96
#define RWM_PAGER_THUMB_HEIGHT 72

#define RWM_TEXT_COLOR_INDEX   0

#define RWM_TOP_FRAME        0
#define RWM_BOTTOM_FRAME     1
#define RWM_LEFT_FRAME       2
#define RWM_RIGHT_FRAME      3
#define RWM_FRAME_SUBWINDOWS 4
#define RWM_FRAME_WINDOWS    5
#define RWM_MAIN_FRAME       5

typedef void Rwm_propertyhandler_t(void *, XEvent *);

struct Rwm_property {
    Rwm_propertyhandler_t *handler;
    Atom atom;
    struct Rwm_property *prev;
    struct Rwm_property *next;
};

#define RWM_PROPERTY_HASH_ITEMS 256
struct R_wm {
    unsigned long     optflags;
    unsigned long     extflags;
    unsigned long     sysflags;

    int               damagenotify;

    struct R_window  *desktop;
    struct R_window **desktops;
    struct R_window  *info;
    struct R_window  *menu;
    struct R_window  *pager;
    struct R_window  *clock;

    struct Rwm_property *propertyhash[RWM_PROPERTY_HASH_ITEMS] __CC_PAGE_ALIGN__;
    void                *pictformat;
};

#define RWM_CLOCK_STR_LENGTH 8
struct Rwm_clock {
    struct R_window *window;
    struct tm       *tm;
    XFontStruct     *fontinfo;
    char             str[RWM_CLOCK_STR_LENGTH];
    GC               gc;
    int              x;
    int              y;
    int              w;
    int              h;
};

#define MWM_HINTS_FUNCTIONS   (1L << 0)
#define MWM_HINTS_DECORATIONS (1L << 1)
#define MWM_HINTS_INPUT_MODE  (1L << 2)
#define MWM_HINTS_STATUS      (1L << 3)
#define MWM_DECOR_ALL         (1L << 0)
#define MWM_DECOR_BORDER      (1L << 1)
#define MWM_DECOR_RESIZEH     (1L << 2)
#define MWM_DECOR_TITLE       (1L << 3)
#define MWM_DECOR_MENU        (1L << 4)
#define MWM_DECOR_MINIMIZE    (1L << 5)
#define MWM_DECOR_MAXIMIZE    (1L << 6)

extern Imlib_Font                _titlefont;
extern Imlib_Font                _menufont;
extern Atom                      _wmatoms[];
extern Atom                      _wmprotos[];
extern struct Rwm_clock          _clock;
extern volatile struct R_window *_focus;
extern volatile int              _updateclock;
//extern int                       _synchdisplay;
extern struct R_color            _wmcolors[];

#endif /* __RESURRECTION_WM_H__ */

