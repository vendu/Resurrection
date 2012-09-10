/*
 * macros.h - Resurrection macros.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef __RESURRECTION_MACROS_H__
#define __RESURRECTION_MACROS_H__

//#define R_init(app, argc, argv) (R_init_display(app, argc, argv), R_init_screen(app, argc, argv, DefaultScreen((app)->display))

#define X_FONT_WIDTH(fp) ((fp)->max_bounds.width)
#define X_FONT_HEIGHT(fp) ((fp)->ascent + (fp)->descent)

#define min2(a, b) ((a) < (b) ? (a) : (b))
#define max2(a, b) ((a) > (b) ? (a) : (b))

#define R_synch_display(a)   XFlushDisplay(app->display)

#define R_selection_owner(w) R_get_selection_owner((w), XA_PRIMARY)
#define R_set_selection(w,t) R_set_selection_owner((w), XA_PRIMARY, (t))
#define R_unset_selection(w) R_set_selection_owner((w), XA_PRIMARY, 0)

#define R_OBJECT_BUFFER_SIZE 131072
#define R_MEDIA_BUFFER_SIZE  1048576
/* object allocation */
#if 0
#define R_alloc_app(a)       R_alloc_object(TRUE)
#define R_alloc_string(a)    R_alloc_object(TRUE)
#define R_alloc_window(a)    R_alloc_object(TRUE)
#define R_alloc_image(a)     R_alloc_object(TRUE)
#define R_alloc_picture(a)   R_alloc_object(TRUE)
#endif
#define R_alloc_app(a)       calloc(1, sizeof(struct R_app));
#define R_alloc_window(a)    calloc(1, sizeof(struct R_window));
#define R_alloc_image(a)     calloc(1, sizeof(struct R_image));
#define R_free_app(a)        free(a)
#define R_free_string(s)     free(s)
#if 0
#define R_free_window(w)     (R_add_window_events(w, NoEventMask), \
                              R_remove_window(w), \
                              free(w))
#endif
#define R_free_image(i)      free(i)
/* I/O-buffer allocation */
#define R_alloc_buffer(n)    valloc(n)
#define R_alloc_media_buffer R_alloc_buffer(R_MEDIA_BUFFER_SIZE)

#define R_get_string_length(s) ((s)->size)

#define R_ARGB32_ALPHA(i32)  ((i32) >> 24)
#define R_ARGB32_RED(i32)    (((i32) >> 16) & 0xff)
#define R_ARGB32_GREEN(i32)  (((i32) >> 8) & 0xff)
#define R_ARGB32_BLUE(i32)   ((i32) & 0xff)

#define R_HASH_WINDOW_ID(id) \
    ((((id) & 0x00ff00ff) >> 8) | ((id) & 0x00ff))

#define R_BINDING_EVENT(event) \
    (((event)->type == KeyPress) \
     || ((event)->type == ButtonPress) \
     || ((event)->type == ButtonRelease) \
     || ((event)->type == MotionNotify))
#define R_MAKE_BINDING(keysym, state) \
    ((((state) & 0xffff) << 16) | ((keysym) & 0xffff))
#define R_GET_BUTTON_BINDING(button, state, release) \
    (!(release) \
     ? ((((state) & 0xffff) << 16) \
        | (1 << ((button) + 23))) \
     : (((((state) | R_BINDING_RELEASE_FLAG) & 0xffff) << 16) \
        | (1 << ((button) + 23))))
#define R_GET_MOTION_BINDING(state) \
    ((((state) | R_BINDING_MOTION_FLAG ) & 0xffff) << 16)
#define R_HASH_BINDING(mask) \
    (mask & 0x007f)

#define RWM_SET_WINDOW_IMAGE(win, s) ((win)->image = (win)->images[(s)])
#define RWM_SET_WINDOW_COORDS(win, wx, wy) \
do { \
    (win)->x = (wx); \
    (win)->y = (wy); \
} while (FALSE)
#define RWM_SET_WINDOW_SIZE(win, ww, wh)   \
do { \
    (win)->w = (ww); \
    (win)->h = (wh); \
} while (FALSE)

#define RV_SET_WINDOW_COORDS(win, wx, wy) \
do { \
    (win)->x = (wx); \
    (win)->y = (wy); \
} while (FALSE)
#define RV_SET_WINDOW_SIZE(win, ww, wh)   \
do { \
    (win)->w = (ww); \
    (win)->h = (wh); \
} while (FALSE)

#define R_set_window_button_action(window, action, func) \
    (window->buttonactions[(action)] = (func))

#if (R_OPTIMIZE_SPEED)
#   define R_set_window_events(window, mask) \
    (XSelectInput(window->app->display, \
                  window->id, \
                  mask), \
     (window->eventmask = (mask)))
#   define R_add_window_events(window, mask) \
    (window->eventmask |= (mask), \
     XSelectInput(window->app->display, \
                  window->id, \
                  window->eventmask), \
        (window->eventmask))
#   define R_remove_window_events(window, mask) \
    ((mask) \
     ? (window->eventmask &= (~(mask)), \
        XSelectInput(window->app->display, \
                     window->id, \
                     window->eventmask), \
        (window->eventmask)) \
     : (window->eventmask))
#   define R_add_save_window(window) \
    XAddToSaveSet(window->app->display, \
                  window->id)
#   define R_remove_save_window(window) \
    XRemoveFromSaveSet(window->app->display, \
                       window->id)
#   define R_reparent_window(window, parent) \
    XReparentWindow(window->app->display, \
                    window->id, \
                    parent->id, \
                    window->x, \
                    window->y)
#   define R_map_window(window) \
    XMapWindow(window->app->display, \
               window->id)
#   define R_map_subwindows(window) \
    XMapSubwindows(window->app->display, \
                   window->id)
#   define R_map_window_raised(window) \
    XMapRaised(window->app->display, \
               window->id)
#   define R_unmap_window(window) \
    XUnmapWindow(window->app->display, \
                 window->id)
#   define R_iconify_window(window) \
    XIconifyWindow(window->app->display, window->id, \
		   DefaultScreen(window->app->display));
#   define R_withdraw_window(window) \
    XWithdrawWindow(window->app->display, window->id, \
		    DefaultScreen(window->app->display));
#   define R_raise_window(window) \
    XRaiseWindow(window->app->display, \
                 window->id)
#   define R_lower_window(window) \
    XLowerWindow(window->app->display, \
                 window->id)
#   define R_clear_window(window) \
    XClearWindow(window->app->display, \
                 window->id)
#   define R_clear_area(window, x, y, w, h, e) \
    XClearArea(window->app->display, \
               window->id, \
               x, \
               y, \
               w, \
               h, \
               e)
#   define R_move_window(window, x, y) \
    XMoveWindow(window->app->display, \
                window->id, \
                x, \
                y)
#   define R_resize_window(window, w, h) \
    XResizeWindow(window->app->display, \
                  window->id, \
                  w, \
                  h)
#   define R_move_resize_window(window, x, y, w, h) \
    XMoveResizeWindow(window->app->display, \
                      window->id, \
                      x, \
                      y, \
                      w, \
                      h)
#   define R_get_window_attributes(window, attr) \
    XGetWindowAttributes(window->app->display, window->id, attr);
#   define R_set_input_focus(window, timestamp) \
    XSetInputFocus(window->app->display, window->id, \
		   RevertToPointerRoot, timestamp);
#else /* !R_OPTIMIZE_SPEED */
#   define _R_set_window_events \
    R_set_window_events
#   define _R_add_window_events \
    R_add_window_events
#   define _R_remove_window_events \
    R_remove_window_events
#   define _R_add_save_window
    R_add_save_window
#   define _R_remove_save_window
    R_remove_save_window
#   define _R_reparent_window \
    R_reparent_window
#   define _R_map_window \
    R_map_window
#   define _R_map_subwindows \
    R_map_subwindows
#   define _R_map_window_raised \
    R_map_window_raised
#   define _R_unmap_window \
    R_unmap_window
#   define _R_iconify_window \
    R_iconify_window
#   define _R_withdraw_window \
    R_withdraw_window
#   define _R_raise_window \
    R_raise_window
#   define _R_lower_window \
    R_lower_window
#   define _R_clear_window \
    R_clear_window
#   define _R_clear_area \
    R_clear_area
#   define _R_move_window \
    R_move_window
#   define _R_resize_window \
    R_resize_window
#   define _R_move_resize_window \
    R_move_resize_window
#   define _R_get_window_attributes \
    R_get_window_attributes
#   define _R_set_input_focus
    R_set_input_focus
#endif

#define R_fade_in(src, dest, fval)                                      \
do {                                                                    \
    argb32_t _rval;                                                     \
    argb32_t _gval;                                                     \
    argb32_t _bval;                                                     \
    float    _fmul;                                                     \
                                                                        \
    _fmul = (float)fval / 0xff;                                         \
    _rval = (argb32_t)(_fmul * _gfx_red_val(src));                      \
    _gval = (argb32_t)(_fmul * _gfx_green_val(src));                    \
    _bval = (argb32_t)(_fmul * _gfx_blue_val(src));                     \
    _gfx_set_pix(dest, 0, _rval, _gval, _bval);                         \
} while (FALSE)

#define R_fade_out(src, dest, fval)                                     \
    do {                                                                \
    argb32_t _rval;                                                     \
    argb32_t _gval;                                                     \
    argb32_t _bval;                                                     \
    float    _fmul;                                                     \
                                                                        \
    _fmul = (float)(0xff - fval) / 0xff;                                \
    _rval = (argb32_t)(_fmul * _gfx_red_val(src));                      \
    _gval = (argb32_t)(_fmul * _gfx_green_val(src));                    \
    _bval = (argb32_t)(_fmul * _gfx_blue_val(src));                     \
    _gfx_set_pix(dest, 0, _rval, _gval, _bval);                         \
} while (FALSE)

#define R_alphablend_nodiv(src, dest, a)                                \
    do {                                                                \
        argb32_t _mask = 0x00ff00ff;                                    \
        argb32_t _ss = (src) >> 8;                                      \
        argb32_t _ds = (dest) >> 8;                                     \
                                                                        \
        (dest) =  (((((((_ss) & 0xff00ff) - ((_ds) & 0xff00ff)) * (a))  \
                     + ((dest) & 0xff00ff00)) & 0xff00ff00)             \
                   + (((((((src) & 0xff00ff)                            \
                          - ((dest) & 0xff00ff)) * (a)) >> 8)           \
                       + ((dest) & 0xff00ff)) & 0xff00ff));             \
    } while (FALSE)

#define R_morph(src1, src2, dest, val)                                  \
    do {                                                                \
        R_alphablend_nodiv(src1, dest, 0xff - (val));                   \
        R_alphablend_nodiv(src2, dest, (val));                          \
    } while (FALSE)

#endif /* __RESURRECTION_MACROS_H__ */

