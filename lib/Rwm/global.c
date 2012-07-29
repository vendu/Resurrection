/*
 * global.c - Resurrection window manager globals.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

struct Rwm_clock  _clock;
volatile struct R_window  *_focus;

Atom _wmatoms[RWM_ATOMS];
Atom _wmprotos[RWM_PROTOS];

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
struct R_color    _wmcolorconfs[] = {
    RWM_TEXT_COLOR
};
struct R_color    _wmcolors[1];
#else
struct R_color    _wmcolors[] = {
    RWM_TEXT_COLOR
};
#endif

#if (R_NEST_WM)
struct Rwm_binding_arg Rwm_exec_args[] = {
    { 0, RWM_TERM_COMMAND },
    { 0, RWM_EDIT_COMMAND },
    { 0, RWM_WEB_COMMAND },
    { 0, RWM_AUDIO_COMMAND },
    { 0, RWM_IM_COMMAND }
#if 0
    { 0, "emacs -display :1" },
    { 0, "firefox -display :1" }
#endif
};
#else
struct Rwm_binding_arg Rwm_exec_args[] = {
    { 0, RWM_TERM_COMMAND },
    { 0, RWM_EDIT_COMMAND },
    { 0, RWM_WEB_COMMAND },
    { 0, RWM_IMAGE_COMMAND },
    { 0, RWM_AUDIO_COMMAND },
    { 0, RWM_VIDEO_COMMAND },
    { 0, RWM_MSN_COMMAND },
    { RWM_EXIT, RWM_EXIT_LABEL }
};
#endif

struct Rwm_binding_arg Rwm_desktop_args[] = {
    { 0, NULL },
    { 1, NULL },
    { 2, NULL },
    { 3, NULL }
};

struct Rwm_binding_arg Rwm_frame_args[] = {
    { 0, NULL },
    { 1, NULL },
    { 2, NULL },
    { 3, NULL }
};

struct R_binding Rwm_desktop_bindings[] = {
    {
        R_MAKE_BINDING(R_F1_KEYSYM,
                       R_BINDING_SHIFT_FLAG),
        Rwm_switch_desktop_func,
        &Rwm_desktop_args[0],
        NULL
    },
    {
        R_MAKE_BINDING(R_F2_KEYSYM,
                       R_BINDING_SHIFT_FLAG),
        Rwm_switch_desktop_func,
        &Rwm_desktop_args[1],
        NULL
    },
    {
        R_MAKE_BINDING(R_F3_KEYSYM,
                       R_BINDING_SHIFT_FLAG),
        Rwm_switch_desktop_func,
        &Rwm_desktop_args[2],
        NULL
    },
    {
        R_MAKE_BINDING(R_F4_KEYSYM,
                       R_BINDING_SHIFT_FLAG),
        Rwm_switch_desktop_func,
        &Rwm_desktop_args[3],
        NULL
    },
    {
        R_MAKE_BINDING(R_F1_KEYSYM,
                       R_BINDING_CTRL_FLAG),
        Rwm_exec_func,
        &Rwm_exec_args[0],
        NULL
    },
    {
        R_MAKE_BINDING(R_F2_KEYSYM,
                       R_BINDING_CTRL_FLAG),
        Rwm_exec_func,
        &Rwm_exec_args[1],
        NULL
    },
    {
        R_MAKE_BINDING(R_F3_KEYSYM,
                       R_BINDING_CTRL_FLAG),
        Rwm_exec_func,
        &Rwm_exec_args[2],
        NULL
    },
    {
        R_MAKE_BINDING(R_F4_KEYSYM,
                       R_BINDING_CTRL_FLAG),
        Rwm_exec_func,
        &Rwm_exec_args[3],
        NULL
    },
    {
        R_MAKE_BINDING(R_F5_KEYSYM,
                       R_BINDING_CTRL_FLAG),
        Rwm_exec_func,
        &Rwm_exec_args[4],
        NULL
    },
    { 0, NULL, NULL, NULL }
};

struct R_window *Rwm_desktop_windows[RWM_DESKTOPS];

