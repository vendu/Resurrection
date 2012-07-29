/*
 * global.h - Resurrection globals.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef __RESURRECTION_GLOBAL_H__
#define __RESURRECTION_GLOBAL_H__

#define R_WINDOW_HASH_ITEMS  65536

#define R_WM_TOPLEVEL_FRAME  0
#define R_WM_TRANSIENT_FRAME 1
#define R_WM_FRAMES          2

struct R_global {
    struct R_window  *windowhash[R_WINDOW_HASH_ITEMS] __CC_PAGE_ALIGN__;
    struct R_app     *app;
    void             *appdata;
    volatile int      synch;
    void            (*leavefunc)();
    void             *leavearg;
};

struct Rwm_configure_info {
    struct timeval tv;
    int x;
    int y;
    int w;
    int h;
};

extern int _ignbadwindow;
extern int _ignbadmatch;
extern int _ignbadatom;
extern struct R_global R_global;

extern struct R_image Rwm_desktop_images[];

extern struct Rwm_binding_arg Rwm_exec_args[];
extern struct Rwm_binding_arg Rwm_desktop_args[];
extern struct R_binding Rwm_desktop_bindings[];
extern struct R_binding Rwm_frame_bindings[];

extern struct R_file *_Rvheadfile;
extern struct R_file *_Rvtailfile;
extern struct R_file *_Rvcurfile;
extern struct R_file *_Rvdestfile;

#endif /* __RESURRECTION_GLOBAL_H__ */

