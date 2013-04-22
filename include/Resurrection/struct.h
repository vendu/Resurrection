/*
 * struct.h - Resurrection data structures.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef __RESURRECTION_STRUCT_H__
#define __RESURRECTION_STRUCT_H__

#if 0
typedef uint8_t  R_text_t;
typedef uint32_t R_binding_t;
#endif

#define R_MULTICLICK_DELAY_USEC 250000

#define R_BUTTON1_PRESS    0
#define R_BUTTON2_PRESS    1
#define R_BUTTON3_PRESS    2
#define R_BUTTON4_PRESS    3
#define R_BUTTON5_PRESS    4
#define R_BUTTON1_RELEASE  5
#define R_BUTTON2_RELEASE  6
#define R_BUTTON3_RELEASE  7
#define R_BUTTON4_RELEASE  8
#define R_BUTTON5_RELEASE  9
#define R_BUTTON_ACTIONS  10

typedef void R_eventhandler_t(void *, XEvent *);
typedef void R_bindingfunc_t(void *, XEvent *, void *);
//typedef void R_buttonaction_t(void *, XEvent *);

#define R_WINDOW_COLOR_SPACE 0
#define R_BINDING_HASH_ITEMS 256

struct R_app {
    unsigned long      flags;
    R_text_t          *name;

    /* extension events */
    int                damagenotify;

    /* v */
    struct R_window   *navi;

    void             **wintree;

    /* client-specific */
    void              *client;

    int                argc;
    char             **argv;
    Display           *display;
    Window             winid;
    struct R_window   *window;
    int                screen;
    int                depth;
    Visual            *visual;
    Colormap           colormap;
#if (SUPPORT_FREETYPE2)
    FT_Library         ftlib;
#endif
} __CC_CACHE_ALIGN__;

#define R_STRING_ISO1 0
#define R_STRING_UCS2 1
#define R_STRING_UCS4 2
struct R_string {
    void *data;
    size_t size;
    int format;
};

struct R_stream {
    off_t   seekpos;
    char   *filename;
    void   *buf;
    size_t  bufsize;
    int     fd;
};

#define R_INVALID_WIDGET  0
#define R_BUTTON_WIDGET   1
#define R_MENU_WIDGET     2
#define R_MENUITEM_WIDGET 3
struct R_widget {
    unsigned long type;
};

#define R_BINDING_KEYSYM_MASK   0x0000ffff
#define R_BINDING_SHIFT_FLAG    ShiftMask
#define R_BINDING_LOCK_FLAG     LockMask
#define R_BINDING_CTRL_FLAG     ControlMask
#define R_BINDING_MOD1_FLAG     Mod1Mask
#define R_BINDING_MOD2_FLAG     Mod2Mask
#define R_BINDING_MOD3_FLAG     Mod3Mask
#define R_BINDING_MOD4_FLAG     Mod4Mask
#define R_BINDING_MOD5_FLAG     Mod5Mask
#define R_BINDING_BUTTON1_FLAG  Button1Mask
#define R_BINDING_BUTTON2_FLAG  Button2Mask
#define R_BINDING_BUTTON3_FLAG  Button3Mask
#define R_BINDING_BUTTON4_FLAG  Button4Mask
#define R_BINDING_BUTTON5_FLAG  Button5Mask
#define R_BINDING_RELEASE_FLAG  (R_BINDING_BUTTON5_FLAG << 1)
#define R_BINDING_MOTION_FLAG   (R_BINDING_BUTTON5_FLAG << 2)
//#define R_BINDING_MODIFIER_FLAG AnyModifier
#define R_BINDING_MODIFIER_MASK 0xffff0000
struct R_binding {
//    struct R_window  *window;
    R_binding_t       mask;
    R_bindingfunc_t  *func;
    void             *arg;
    struct R_binding *next;
};

#define RWM_EXIT (-1)
struct Rwm_binding_arg {
    int val;
    char *cmd;
};

/* sysflags */
#define R_WINDOW_SAVE_UNDER_FLAG        0x00000001 /* save unders */
#define R_WINDOW_BACK_STORE_FLAG        0x00000002 /* backing store */
#define R_WINDOW_OVERRIDE_REDIRECT_FLAG 0x00000004
#define R_WINDOW_EXPOSED_FLAG           0x00000008 /* can be drawn onto */
#define R_WINDOW_SYS_MASK               0x0000000f
#define R_WINDOW_STATIC_EVENTS_FLAG     0x00000020
#define R_WINDOW_STATIC_IMAGES_FLAG     0x00000040
/* typeflags */
#define R_WINDOW_ROOT_FLAG              0x00000001 /* root window */
#define R_WINDOW_DESKTOP_FLAG           0x00000002 /* desktop window */
#define R_WINDOW_MENU_FLAG              0x00000004 /* menu window */
#define R_WINDOW_PAGER_FLAG             0x00000008 /* pager window */
#define R_WINDOW_TOPLEVEL_FLAG          0x00000010 /* toplevel window */
#define R_WINDOW_TRANSIENT_FLAG         0x00000020 /* transient window */
#define R_WINDOW_OVERRIDE_FLAG          0x00000040
#define R_WINDOW_FRAME_FLAG             0x00000080 /* frame/border window */
#define R_WINDOW_CONTAINER_FLAG         0x00000100 /* container */
#define R_WINDOW_WIDGET_FLAG            0x00000200 /* widget */
#define R_WINDOW_CLIENT_MASK \
    (R_WINDOW_TOPLEVEL_FLAG | R_WINDOW_TRANSIENT_FLAG)
#define R_WINDOW_DO_NOT_PROPAGATE_FLAG  0x00000400
/* stateflags */
#define R_WINDOW_MOVE_FLAG       0x00000001
#define R_WINDOW_RESIZE_FLAG     0x00000002
#define R_WINDOW_SHADED_FLAG     0x00000004
#define R_WINDOW_MAPPED_FLAG     0x00000008
#define R_WINDOW_NORMAL_FLAG     0x00000010
#define R_WINDOW_DISABLED_FLAG   0x00000020
#define R_WINDOW_ACTIVE_FLAG     0x00000040
#define R_WINDOW_CLICKED_FLAG    0x00000080
#define R_WINDOW_FOCUSED_FLAG    0x00000100
#define R_WINDOW_CLOSED_FLAG     0x00000200
#define R_WINDOW_DESTROYED_FLAG  0x00000400
#define R_WINDOW_BORDERLESS_FLAG 0x00000800
#if 0
/* widgetflags */
#define R_WIDGET_DISABLED_FLAG       0x00000001
#define R_WIDGET_CLOSE_ON_CLICK_FLAG 0x00000002
#endif
/* window states (for switching image) */
#define R_WINDOW_NORMAL_STATE    0x00
#define R_WINDOW_ACTIVE_STATE    0x01
#define R_WINDOW_CLICKED_STATE   0x02
#define R_WINDOW_FOCUSED_STATE   0x03
#define R_WINDOW_DISABLED_STATE  0x04
#define R_WINDOW_STATES          0x05
/* image indices */
#define R_WINDOW_BACKGROUND_IMAGE 0
#define R_WINDOW_SELECTED_IMAGE   1
#define R_WINDOW_CLICKED_IMAGE    2
#define R_WINDOW_IMAGES           3

/* FIXME: elsewhere */
#define RV_FIT_FLAG               0x00000001
#define RV_SLIDE_FLAG             0x00000002

#define RV_IMAGE_FADEIN           1
#define RV_IMAGE_FADEOUT          2
#define RV_IMAGE_MORPH            3

struct R_windowclicks {
    int            clicks;
    struct timeval lasttv;
};

struct R_window {
    struct R_app          *app;
    /* wm stuff */
    unsigned long          sysflags;
    unsigned long          typeflags;
    unsigned long          stateflags;
    unsigned long          protoflags;

    unsigned long          clientflags; /* client-specific */
#if 0
    unsigned long          widgetflags;
#endif
    int                    x;
    int                    y;
    int                    w;
    int                    h;
    Window                 above;
    int                    border;
    int                    top;
    int                    bottom;
    int                    left;
    int                    right;
#if 0
    int                    reqx;
    int                    reqy;
    int                    reqw;
    int                    reqh;
#endif
    char                  *str;
    char                  *timestr;
//    unsigned long          state;
#if (USE_IMLIB2)
#if (R_FIXED_STATE_IMAGES)
    struct R_image        *image;
#endif
#if (USE_XRENDER)
    struct R_picture       picture;
#endif
#endif
    struct R_binding     **bindings;
#if (USE_IMLIB2)
#if !(SUPPORT_TRUETYPE_FONTS)
    Imlib_Font             font;
#endif
    struct R_image        *images[R_WINDOW_STATES];
#endif
    struct R_window       *desktop;
    struct R_window       *parent;
    struct R_window       *child;
    Window                 id;
    int                    num;
#if (R_WINDOW_COLOR_SPACE)
    Visual                *vis;
    Colormap               cmap;
#endif
#if (SUPPORT_TRUETYPE_FONTS)
#if (SUPPORT_TRUETYPE_IMLIB2)
    Imlib_Font            *im2font;
    Imlib_Image           *im2title;
    int                    im2titlew;
    int                    im2titleh;
#else
    ftfont_t              *font;
#endif
#endif
    long                   eventmask;
    int                    nevents;
    R_eventhandler_t     **eventhandlers;
//    R_buttonaction_t     **buttonactions;
    struct R_windowclicks  clicks;
    struct R_window       *prev;
    struct R_window       *next;
    struct R_window       *chain; /* top, bottom, left, right */
    struct R_window       *last;

    R_text_t              *title;
    int                    titlelen;
    R_text_t              *iconname;
    XWMHints              *wmhints;
    XSizeHints            *normalhints; /* normal hints for window managers */
    XSizeHints            *sizehints;
    XClassHint            *classhint;   /* yet more hints for window managers */
    uint32_t              *motifwmhints;
    XSetWindowAttributes   attributes;
} __CC_CACHE_ALIGN__;

#define R_MOTIF_DECOR_FLAG 0x02
struct R_motif_wm_hints {
    uint32_t flags;
    uint32_t funcs;
    uint32_t decor;
    int32_t  input;
    uint32_t status;
};

#if (USE_XRENDER)
#define R_PICTURE_ALPHA_FLAG 0x00000001
struct R_picture {
    unsigned long flags;
    Picture       pict;
    Picture       mask;
};
#endif

struct R_frame {
    struct R_window *top;
    struct R_window *bottom;
    struct R_window *left;
    struct R_window *right;
    int             x;
    int             y;
    int             w;
    int             h;
#if (USE_IMLIB2)
    struct R_image *image;
#endif
    struct R_frame *next;
} __CC_CACHE_ALIGN__;

struct R_object {
    union {
        struct R_app     app;
        struct R_string  string;
        struct R_window  window;
#if (USE_IMLIB2)
        struct R_image   image;
#endif
#if (USE_XRENDER)
        struct R_picture picture;
#endif
    } data;
};

struct R_file {
    char          *name;
    struct R_file *prev;
    struct R_file *next;
    Pixmap         thumb;
};

#if 0
struct R_magazine {
    unsigned long       n;
    unsigned long       ndx;
    void              **stk;
    struct R_magazine  *next;
};
#endif

/* MODULE: saver */

#define HEXDUMP_ROWS       24
#define HEXDUMP_COLUMNS    80
#define HEXDUMP_WIDTH(s)   ((s)->charw * (HEXDUMP_COLUMNS - 1))
#define HEXDUMP_HEIGHT(s)  ((s)->charh * HEXDUMP_ROWS + 4)

#define ALIEN_ROWS         32
#define ALIEN_COLUMNS      32
#define ALIEN_GLYPHS       62
#define ALIEN_GLYPH_WIDTH  25
#define ALIEN_GLYPH_HEIGHT 25
#define ALIEN_FONT_WIDTH   (ALIEN_GLYPHS * ALIEN_GLYPH_WIDTH)
#define ALIEN_FONT_HEIGHT  (ALIEN_GLYPHS * ALIEN_GLYPH_HEIGHT)
#define ALIEN_WIDTH(s)     (ALIEN_GLYPH_WIDTH * ALIEN_COLUMNS)
#define ALIEN_HEIGHT(s)    (ALIEN_GLYPH_HEIGHT * ALIEN_ROWS)

struct R_saver {
    struct R_app *app;
    unsigned long candraw;
    uint8_t *hexdata;
    uint8_t *aliendata;
    uint8_t *screendata;
    size_t datasize;
    XFontStruct *fontinfo;
    GC fggc;
    GC bggc;
    Pixmap background;
    Pixmap fontpixmap;
    Pixmap drawbuffer;
    unsigned long black;
    uint8_t *nextchar;
    int nrows;
    int row;
    int charw;
    int charh;
    int charasc;
    int chardesc;
    int bottomrows[ALIEN_COLUMNS];
    unsigned long pixels[ALIEN_ROWS];
};

#endif /* __RESURRECTION_STRUCT_H__ */

