/*
 * gscale.c - scaling routines for Thunder Desktop Environment.
 * Copyright (C) 2005 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

void
R_scale_image(R_argb32_t *src, R_argb32_t *dest,
              int srcw, int srch,
              int destw, int desth)
{
    R_argb32_t *srcptr1;
    R_argb32_t *srcptr2;
    R_argb32_t *destptr;
    float xfactor;
    float yfactor;
    int srcx;
    int srcy;
    int destx;
    int desty;
    R_argb32_t destval;

    destptr = dest;
    xfactor = (float)srcw / destw;
    yfactor = (float)srch / desth;
    for (desty = 0 ; desty < desth ; desty++) {
        srcy = desty * yfactor;
        srcptr1 = src + srcy * srcw;
        for (destx = 0 ; destx < destw ; destx++) {
            srcx = destx * xfactor;
            srcptr2 = srcptr1 + srcx;
            destval = *srcptr2;
            *destptr = destval;
            destptr++;
        }
    }

    return;
}

#if 0
/*
 * Here is a scaling algorithm that should eliminate aliasing artifacts; it's
 * a bit slower than the one above.
 */
void
Tscaleantialias(struct Timage *srcimage, struct Timage *destimage,
                int srcw, int srch, int destw, int desth)
{
    Tuargb_t *src;
    Tuargb_t *dest;
    Tuargb_t *srcptr1;
    Tuargb_t *srcptr2;
    Tuargb_t *destptr;
    float xfactor;
    float yfactor;
    Tdim_t srcw;
    Tdim_t srch;
    Tdim_t xval;
    Tdim_t yval;
    Tcoord_t srcx;
    Tcoord_t srcy;
    Tcoord_t destx;
    Tcoord_t desty;
    Tuargb_t destval;
    PROFILE_CLOCK_VARS;
    
    PROFILE_CLOCK_START();
    src = srcimage->data;
    dest = destimage->data;
    destptr = dest;
    srcw = srcimage->w;
    srch = srcimage->h;
    xfactor = (float)srcw / w;
    yfactor = (float)srch / h;
    yval = 0;
    desty = 0;
    for (desty = 0 ; desty < h ; desty++) {
        yval = (yval + srch) % w;
        srcy = desty * yfactor;
        if (yval < srch) {
            xval = 0;
            srcptr1 = src + srcy * srcw;
            for (destx = 0 ; destx < w ; destx++) {
                xval = (xval + srcw) % w;
                srcx = destx * xfactor;
                if (xval < srcw) {
                    srcptr2 = srcptr1 + srcx;
                    destval = *srcptr2;
                    *destptr = destval;
                }
                destptr++;
            }
        } else {
            destptr += w;
        }
    }
    PROFILE_CLOCK_STOP();
    fprintf(stderr, "SCALEAA: %lu\n", PROFILE_CLOCK_TIME());

    return;
}

void
Tscalehalf(struct Timage *srcimage, struct Timage *destimage)
{
    Tuargb_t *src;
    Tuargb_t *dest;
    Tuargb_t *srcptr1;
    Tuargb_t *srcptr2;
    Tuargb_t *destptr;
    Tdim_t srcw;
    Tdim_t srch;
    Tcoord_t srcx;
    Tcoord_t srcy;
    Tuargb_t srcval1;
    Tuargb_t srcval2;
    Tuargb_t srcval3;
    Tuargb_t srcval4;
    Tuargb_t destval;
    Tuargb_t redval;
    Tuargb_t greenval;
    Tuargb_t blueval;

    src = srcimage->data;
    dest = destimage->data;
    destptr = dest;
    srcw = srcimage->w;
    srch = srcimage->h;
    for (srcy = 0 ; srcy < srch ; srcy += 2) {
        srcptr1 = src + srcy * srcw;
        srcptr2 = srcptr1 + srcw;
        for (srcx = 0 ; srcx < srcw ; srcx += 2) {
            srcval1 = *srcptr1++;
            srcval2 = *srcptr1++;
            srcval3 = *srcptr2++;
            srcval4 = *srcptr2++;
            redval = __THUNDER_ARGB_RED_VALUE(srcval1);
            greenval = __THUNDER_ARGB_GREEN_VALUE(srcval1);
            blueval = __THUNDER_ARGB_BLUE_VALUE(srcval1);
            redval += __THUNDER_ARGB_RED_VALUE(srcval2);
            greenval += __THUNDER_ARGB_GREEN_VALUE(srcval2);
            blueval += __THUNDER_ARGB_BLUE_VALUE(srcval2);
            redval += __THUNDER_ARGB_RED_VALUE(srcval3);
            greenval += __THUNDER_ARGB_GREEN_VALUE(srcval3);
            blueval += __THUNDER_ARGB_BLUE_VALUE(srcval3);
            redval += __THUNDER_ARGB_RED_VALUE(srcval4);
            greenval += __THUNDER_ARGB_GREEN_VALUE(srcval4);
            blueval += __THUNDER_ARGB_BLUE_VALUE(srcval4);
            redval >>= 2;
            greenval >>= 2;
            blueval >>= 2;
            destval = __THUNDER_ARGB_SET_VALUE(0, redval, greenval, blueval);
            *destptr = destval;
            destptr++;
        }
    }

    return;
}

/*
 * Here is a high-quality double-size scaler known as AdvMame2x. More
 * information can be found at http://scale2x.sourceforge.net.
 */
void
Tscaledouble(Tid_t src, Tid_t dest)
{
#if 0
    Tuargb_t *src;
    Tuargb_t *dest;
#endif
    struct Tobject *srcobj;
    struct Tobject *destobj;
    struct Timage *srcimg;
    struct Timage *destimg;
    Tuargb_t *srcptr;
    Tuargb_t *destptr;
    Tdim_t srcw;
    Tdim_t srch;
    Tdim_t destw;
    Tdim_t desth;
    Tcoord_t srcx;
    Tcoord_t srcy;
    Tuargb_t val1;
    Tuargb_t val2;
    Tuargb_t val3;
    Tuargb_t val4;
    Tuargb_t val5;
    Tuargb_t val6;
    Tuargb_t val7;
    Tuargb_t val8;
    Tuargb_t val9;
    PROFILE_CLOCK_VARS;

    PROFILE_CLOCK_START();
#if 0
    src = srcimage->data;
    dest = destimage->data;
#endif
    srcobj = Tfindobject(src);
    destobj = Tfindobject(dest);
    srcimg = srcobj->data;
    destimg = destobj->data;
    srcptr = srcimg->data;
    srcw = srcimg->w;
    srch = srcimg->h;
    destw = srcw << 1;
    desth = srch << 1;
    /* top row */
    val1 = 0;
    val2 = 0;
    val3 = 0;
    val4 = 0;
    val5 = srcptr[0];
    val6 = srcptr[1];
    val7 = 0;
    val8 = srcptr[srcw];
    val9 = srcptr[srcw + 1];
    srcptr++;
    destptr = destimg->data + 2;
    for (srcx = 1 ; srcx < srcw ; srcx++) {
        destptr[0] = (val4 == val2 && val2 != val6 && val4 != val8)
            ? val4
            : val5;
        destptr[1] = (val2 == val6 && val2 != val4 && val6 != val8)
            ? val6
            : val5;
        destptr[destw] = (val4 == val8 && val4 != val2 && val8 != val6)
            ? val4
            : val5;
        destptr[destw + 1] = (val8 == val6 && val4 != val8 && val2 != val6)
            ? val6
            : val5;
        val4 = val5;
        val5 = srcptr[0];
        val6 = srcptr[1];
        val7 = val8;
        val8 = val9;
        val9 = srcptr[srcw + 1];
        srcptr++;
        destptr += 2;
    }
    /* middle rows */
    destptr += destw - 2;
    for (srcy = 1 ; srcy < srch - 1; srcy++) {
        val1 = 0;
        val2 = srcptr[-srcw];
        val3 = srcptr[-srcw + 1];
        val4 = 0;
        val5 = srcptr[0];
        val6 = srcptr[1];
        val7 = 0;
        val8 = srcptr[srcw];
        val9 = srcptr[srcw + 1];
        srcptr++;
        for (srcx = 1 ; srcx < srcw ; srcx++) {
            destptr[0] = (val4 == val2 && val2 != val6 && val4 != val8)
                ? val4
                : val5;
            destptr[1] = (val2 == val6 && val2 != val4 && val6 != val8)
                ? val6
                : val5;
            destptr[destw] = (val4 == val8 && val4 != val2 && val8 != val6)
                ? val4
                : val5;
            destptr[destw + 1] = (val8 == val6 && val4 != val8 && val2 != val6)
                ? val6
                : val5;
            val1 = val2;
            val2 = val3;
            val3 = srcptr[-srcw + 1];
            val4 = val5;
            val5 = srcptr[0];
            val6 = srcptr[1];
            val7 = val8;
            val8 = val9;
            val9 = srcptr[srcw + 1];
            srcptr++;
            destptr += 2;
        }
        destptr += destw + 2;
    }
    /* bottom row */
    val1 = 0;
    val2 = srcptr[-srcw];
    val3 = srcptr[-srcw + 1];
    val4 = 0;
    val5 = srcptr[0];
    val6 = srcptr[1];
    val7 = 0;
    val8 = 0;
    val9 = 0;
    srcptr++;
    for (srcx = 1 ; srcx < srcw ; srcx++) {
        destptr[0] = (val4 == val2 && val2 != val6 && val4 != val8)
            ? val4
            : val5;
        destptr[1] = (val2 == val6 && val2 != val4 && val6 != val8)
            ? val6
            : val5;
        destptr[destw] = (val4 == val8 && val4 != val2 && val8 != val6)
            ? val4
            : val5;
        destptr[destw + 1] = (val8 == val6 && val4 != val8 && val2 != val6)
            ? val6
            : val5;
        val1 = val2;
        val2 = val3;
        val3 = srcptr[-srcw + 1];
        val4 = val5;
        val5 = srcptr[0];
        val6 = srcptr[1];
        srcptr++;
        destptr += 2;
    }
    PROFILE_CLOCK_STOP();
    fprintf(stderr, "ADVMAME2x: %lu\n", PROFILE_CLOCK_TIME());

    return;
}

#endif /* 0 */

