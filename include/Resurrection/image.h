/*
 * image.h - Resurrection image support
 * Copyright (C) 2007 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef __RESURRECTION_IMAGE_H__
#define __RESURRECTION_IMAGE_H__

#if (USE_IMLIB2)

#if (USE_XSHM)
#   include <X11/Xlib.h>
#   include <sys/ipc.h>
#   include <sys/shm.h>
#   include <X11/extensions/XShm.h>
#endif

#if 0
#define R_IMAGE_NORMAL_WIDTH  0
#define R_IMAGE_NORMAL_HEIGHT 0
#endif

#define R_TILE_IMAGE          0
#define R_FIT_IMAGE          -1
#define R_TILE_FIT_IMAGE     -2
#define R_INTEGER_TILE_IMAGE -3

#if (USE_XSHM)
struct R_shm_image {
    int              shmid;
    int              pmapshmid;
    XShmSegmentInfo  ximseg;
    XShmSegmentInfo  pmapseg;
};
#endif

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
struct R_color {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
} __CC_PACK__;
#else
struct R_color {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} __CC_PACK__;
#endif
#define R_SET_COLOR(cp, alpha, red, green, blue) \
    (((cp)->a = (alpha), \
      (cp)->r = (red), \
      (cp)->g = (green), \
      (cp)->b = (blue)), \
     *(cp))

#define R_IMAGE_NORMAL_MODIFIER   256
#define R_IMAGE_NORMAL_GAMMA      R_IMAGE_NORMAL_MODIFIER
#define R_IMAGE_NORMAL_BRIGHTNESS R_IMAGE_NORMAL_MODIFIER
#define R_IMAGE_NORMAL_CONTRAST   R_IMAGE_NORMAL_MODIFIER

#define R_IMLIB2_NORMAL_MODIFIER   1.0
#define R_IMLIB2_NORMAL_GAMMA      R_IMLIB2_NORMAL_MODIFIER
#define R_IMLIB2_NORMAL_BRIGHTNESS R_IMLIB2_NORMAL_MODIFIER
#define R_IMLIB2_NORMAL_CONTRAST   R_IMLIB2_NORMAL_MODIFIER

#define R_IMAGE_COLOR_MODIFIER 0
#define R_IMAGE_RED_MODIFIER   1
#define R_IMAGE_GREEN_MODIFIER 2
#define R_IMAGE_BLUE_MODIFIER  3
#define R_IMAGE_COLOR_MODIFIERS 4

struct R_colormod {
    R_color_t    tintmask;
    R_colormod_t gamma;
    R_colormod_t brightness;
    R_colormod_t contrast;
};

#define R_IMAGE_STATIC_FLAG       0x00000001
#define R_IMAGE_STATIC_THUMB_FLAG 0x00000002
#define R_IMAGE_FIT_FLAG          0x00000004
#define R_IMAGE_BLEND_FLAG        0x00000008
#define R_IMAGE_STRETCH_FLAG      0x00000010
#define R_IMAGE_FILL_FLAG         0x00000020
struct R_image {
    struct R_app   *app;
    unsigned long   flags;
    struct R_file  *file;

    char           *filename;

    Imlib_Image    *img;
    Imlib_Image    *timg;
    Pixmap          pixmap;
    Pixmap          mask;
    Pixmap          tpixmap;
    Pixmap          tmask;
    Pixmap          xpixmap;
    int             w;
    int             h;
    Imlib_Image    *overlay;
    Imlib_Image    *orig;
    int             origw;
    int             origh;
#if 0
    Pixmap          origpixmap;
    Pixmap          origmask;
#endif
    struct R_colormod colormods[R_IMAGE_COLOR_MODIFIERS];
    struct R_image *prev;
    struct R_image *next;
#if (USE_XSHM)
    int                hasshm;
    XShmSegmentInfo    shmseg;
    struct R_shm_image shmimg;
#endif
    XImage          *xim;
} __CC_CACHE_ALIGN__;

#endif /* USE_IMLIB2 */

#endif /* __RESURRECTION_IMAGE_H__ */

