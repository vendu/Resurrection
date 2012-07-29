#include <stdlib.h>
#include <stdio.h>

#define TEST_FADE_IN     1

#define TEST_WIDTH       1024
#define TEST_HEIGHT      768

#define FALSE 0

#include <X11/Xlibint.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <X11/Xlocale.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <Imlib2.h>

void
gfxscale(struct img *src, struct img *dest,
         unsigned long srcx, unsigned long srcy,
         unsigned long destx, unsigned long desty,
         unsigned long w, unsigned long h)
{
    argb32_t *sptr;
    argb32_t *dptr;
    argb32_t *sptr1;
    argb32_t *sptr2;
    argb32_t *dptr1;
    unsigned long sx;
    unsigned long sy;
    unsigned long sw;
    unsigned long sh;
    unsigned long dx;
    unsigned long dy;
    unsigned long dw;
    unsigned long dh;
    unsigned long dxmax;
    unsigned long dymax;
    float         xmul;
    float         ymul;

    sptr = src->data;
    dptr = dest->data;
    sw = src->width;
    sh = src->height;
    dw = dest->width;
    dh = dest->height;
    xmul = (float)sw / w;
    ymul = (float)sh / h;
    dxmax = min(destx + w, dw);
    dymax = min(desty + h, dh);
    for (dy = desty ; dy < dymax ; dy++) {
        sy = dy * ymul;
        sptr1 = sptr + sy * sw;
        dptr1 = dptr + dy * dw;
        for (dx = destx ; dx < dxmax ; dx++) {
            sx = srcx + dx * xmul;
            sptr2 = sptr1 + sx;
            *dptr1++ = *sptr2;
        }
    }

    return;
}

/*
 * Here is a scaling algorithm that should eliminate aliasing artifacts; it's
 * a bit slower than the one above.
 */
void
scaleantialias(argb32_t *src, argb32_t *dest,
               unsigned long srcw, unsigned long srch,
               unsigned long destw, unsigned long desth)
{
    argb32_t *srcptr1;
    argb32_t *srcptr2;
    argb32_t *destptr;
    float xfactor;
    float yfactor;
    unsigned long xval;
    unsigned long yval;
    unsigned long srcx;
    unsigned long srcy;
    unsigned long destx;
    unsigned long desty;
    argb32_t destval;
    
    destptr = dest;
    xfactor = (float)srcw / destw;
    yfactor = (float)srch / desth;
    yval = 0;
    desty = 0;
    for (desty = 0 ; desty < desth ; desty++) {
        yval = (yval + srch) % destw;
        srcy = desty * yfactor;
        if (yval < srch) {
            xval = 0;
            srcptr1 = src + srcy * srcw;
            for (destx = 0 ; destx < destw ; destx++) {
                xval = (xval + srcw) % destw;
                srcx = destx * xfactor;
                if (xval < srcw) {
                    srcptr2 = srcptr1 + srcx;
                    destval = *srcptr2;
                    *destptr = destval;
                }
                destptr++;
            }
        } else {
            destptr += destw;
        }
    }

    return;
}

void
scalehalf(argb32_t *src, argb32_t *dest,
          unsigned long w, unsigned long h)
{
    argb32_t *srcptr1;
    argb32_t *srcptr2;
    argb32_t *destptr;
    unsigned long srcw;
    unsigned long srch;
    unsigned long srcx;
    unsigned long srcy;
    argb32_t srcval1;
    argb32_t srcval2;
    argb32_t srcval3;
    argb32_t srcval4;
    argb32_t destval;
    argb32_t redval;
    argb32_t greenval;
    argb32_t blueval;

    destptr = dest;
    srcw = w;
    srch = h;
    for (srcy = 0 ; srcy < srch ; srcy += 2) {
        srcptr1 = src + srcy * srcw;
        srcptr2 = srcptr1 + srcw;
        for (srcx = 0 ; srcx < srcw ; srcx += 2) {
            srcval1 = *srcptr1++;
            srcval2 = *srcptr1++;
            srcval3 = *srcptr2++;
            srcval4 = *srcptr2++;
            redval = gfxredval(srcval1);
            greenval = gfxgreenval(srcval1);
            blueval = gfxblueval(srcval1);
            redval += gfxredval(srcval2);
            greenval += gfxgreenval(srcval2);
            blueval += gfxblueval(srcval2);
            redval += gfxredval(srcval3);
            greenval += gfxgreenval(srcval3);
            blueval += gfxblueval(srcval3);
            redval += gfxredval(srcval4);
            greenval += gfxgreenval(srcval4);
            blueval += gfxblueval(srcval4);
            redval >>= 2;
            greenval >>= 2;
            blueval >>= 2;
            gfxsetpix_p(destptr, 0, redval, greenval, blueval);
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
scale2x(argb32_t *src, argb32_t *dest, unsigned long srcw, unsigned long srch)
{
    argb32_t *srcptr;
    argb32_t *destptr;
    unsigned long destw;
    unsigned long desth;
    unsigned long srcx;
    unsigned long srcy;
    argb32_t val1;
    argb32_t val2;
    argb32_t val3;
    argb32_t val4;
    argb32_t val5;
    argb32_t val6;
    argb32_t val7;
    argb32_t val8;
    argb32_t val9;

    srcptr = src;
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
    destptr = dest + 2;
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

    return;
}

void
gfxmorph1(struct img *src1, struct img *src2, struct img *dest, argb32_t val)
{
    argb32_t *sptr1;
    argb32_t *sptr2;
    argb32_t *dptr;
    unsigned long ul;

    sptr1 = (argb32_t *)src1->xim->data;
    sptr2 = (argb32_t *)src2->xim->data;
    dptr = (argb32_t *)dest->xim->data;
    for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
        gfxxfade2_jose(*sptr1, *sptr2, *dptr, val);
        sptr1++;
        sptr2++;
        dptr++;
    }

    return;
}

void
gfxmorph2(struct img *src1, struct img *src2, struct img *dest, argb32_t val)
{
    argb32_t *sptr1;
    argb32_t *sptr2;
    argb32_t *dptr;
    unsigned long ul;

    sptr1 = (argb32_t *)src1->xim->data;
    sptr2 = (argb32_t *)src2->xim->data;
    dptr = (argb32_t *)dest->xim->data;
    for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
        gfxxfade2(*sptr1, *sptr2, *dptr, val);
        sptr1++;
        sptr2++;
        dptr++;
    }

    return;
}

struct attr _attr;
struct img  _plasmaimg;
struct img  _swordimg;
struct img  _blackimg;
struct img  _winimg;
struct img  _srcimg;
struct img  _scaleimg;

void
init(void)
{
    unsigned long ul;
#if (SSE)
    struct _v128 *p128;
#endif

#if (SSE)
    ul = 256;
    while (ul--) {
        p128 = (struct _v128 *)(&_atab128[ul]);
        p128->w1 = (uint16_t)ul;
        p128->w2 = (uint16_t)ul;
        p128->w3 = (uint16_t)ul;
        p128->w4 = (uint16_t)ul;
        p128->w5 = (uint16_t)ul;
        p128->w6 = (uint16_t)ul;
        p128->w7 = (uint16_t)ul;
        p128->w8 = (uint16_t)ul;
    }
#endif
}

void
initx(void)
{
#if (USE_XV)
    XvAdaptorInfo *ai;
    int            n;

    XvImageFormatValues *fv;
#endif

    _attr.disp = XOpenDisplay(NULL);
    if (!_attr.disp) {
        fprintf(stderr, "failed to open display\n");
        
        exit(1);
    }
//    XSynchronize(_attr.disp, True);
    _attr.vis = DefaultVisual(_attr.disp, DefaultScreen(_attr.disp));
#if (USE_RENDER)
    _attr.rendfmt = XRenderVisualFormat(_attr.disp, _attr.vis);
#endif
    _attr.depth = DefaultDepth(_attr.disp, DefaultScreen(_attr.disp));
    fprintf(stderr, "DEPTH: %d\n", attr.depth);
    _attr.win = XCreateWindow(_attr.disp, DefaultRootWindow(_attr.disp),
                              0, 0,
                              TEST_WIDTH, TEST_HEIGHT,
                              0,
                              DefaultDepth(_attr.disp, DefaultScreen(_attr.disp)),
                              InputOutput,
                              _attr.vis,
                              0,
                              NULL);
    if (!_attr.win) {
        fprintf(stderr, "failed to create window\n");
        
        exit(1);
    }
    _attr.gc = XCreateGC(_attr.disp, _attr.win, 0, NULL);
    imlib_context_set_display(_attr.disp);
    imlib_context_set_visual(_attr.vis);
    imlib_context_set_drawable(_attr.win);
#if (USE_XV)
    XvQueryAdaptors(_attr.disp, _attr.win, &n, &ai);
    _attr.vport = ai->base_id;
    XvFreeAdaptorInfo(ai);

    fv = XvListImageFormats(_attr.disp, _attr.vport, &n);
    while (n--) {
        fprintf(stderr, "%s: %x %x %x\n",
                fv->type == XvRGB ? "RGB" : "YUV",
                fv->red_mask, fv->green_mask, fv->blue_mask);
        if (fv->red_mask == _attr.vis->red_mask
            && fv->green_mask == _attr.vis->green_mask
            && fv->blue_mask == _attr.vis->blue_mask) {
            fprintf(stderr, "RGB: %x %x %x\n",
                    fv->red_mask, fv->green_mask, fv->blue_mask);
        }
    }
    XFree(fv);

    exit(0);
#endif
    
    return;
}

#if (USE_SHM)
int
initshm(size_t size, XShmSegmentInfo *seg)
{
    int shmid;

    shmid = shmget(IPC_PRIVATE, size, 0777 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");

        return -1;
    }
    seg->shmaddr = shmat(shmid, NULL, 0);
    if (seg->shmaddr == ((void *)-1)) {
        perror("shmat");

        return -1;
    }
    seg->shmid = shmid;
    seg->readOnly = False;

    return shmid;
}

void
initimg(struct attr *attr, struct img *img, int pmap)
{
    img->xim = XShmCreateImage(attr->disp,
                               attr->vis,
                               attr->depth,
                               ZPixmap,
                               NULL,
                               &img->ximseg,
                               img->width,
                               img->height);
    if (!img->xim) {
        fprintf(stderr, "cannot allocate image structure\n");

        exit(1);
    }
    img->shmid = initshm(img->width * img->height * 4, &img->ximseg);
    img->xim->data = img->ximseg.shmaddr;
    if (img->shmid < 0) {

        exit(1);
    }
    XShmAttach(attr->disp,
               &img->ximseg);
    if (pmap) {
        img->pmapshmid = initshm(img->width * img->height * 4, &img->pmapseg);
        if (img->pmapshmid < 0) {
            
            exit(1);
        }
        XShmAttach(attr->disp,
                   &img->pmapseg);
        img->pmap = XShmCreatePixmap(attr->disp,
                                     attr->win,
                                     img->pmapseg.shmaddr,
                                     &img->pmapseg,
                                     img->width,
                                     img->height,
                                     attr->depth);
        if (!img->pmap) {
            fprintf(stderr, "cannot create pixmap\n");

            exit(1);
        }
    }

    return;
}
#else
void
initimg(struct attr *attr, struct img *img, int pmap)
{
    img->data = malloc(img->width * img->height * 4);
    if (!img->data) {
        fprintf(stderr, "cannot allocate image\n");

        exit(1);
    }
    img->xim = XCreateImage(attr->disp,
                            attr->vis,
                            attr->depth,
                            ZPixmap,
                            0,
                            (char *)img->data,
                            img->width,
                            img->height,
                            32,
                            0);
    if (pmap) {
        img->pmap = XCreatePixmap(attr->disp,
                                  attr->win,
                                  img->width,
                                  img->height,
                                  attr->depth);
    }

    return;
}
#endif

int
loadimg(struct attr *attr, struct img *img, char *filename)
{
    img->imlib2 = imlib_load_image(filename);
    if (!img->imlib2) {

        return -1;
    }
    img->fname = filename;
    imlib_context_set_image(img->imlib2);
    img->width = imlib_image_get_width();
    img->height = imlib_image_get_height();
    imlib_render_pixmaps_for_whole_image(&img->pmap, &img->mask);

    return 0;
}

#define _getimg(d, i) (i)->xim = XGetImage(_attr.disp, (d), 0, 0, (i)->width, (i)->height, ~0UL, ZPixmap)
#if (USE_SHM)
#define getimg(d, i) XShmGetImage(_attr.disp, (d), (i)->xim, 0, 0, ~0UL)
#define putimg(i, d) XShmPutImage(_attr.disp, (d), _attr.gc, (i)->xim, 0, 0, 0, 0, (i)->width, (i)->height, False);
#else
#define getimg(d, i) _getimg(d, i)
#define putimg(i, d) XPutImage(_attr.disp, (d), _attr.gc, (i)->xim, 0, 0, 0, 0, (i)->width, (i)->height);
#endif

void
initimgs(void)
{
    loadimg(&_attr, &_plasmaimg, "test.jpg");
    initimg(&_attr, &_plasmaimg, 0);
    _getimg(_plasmaimg.pmap, &_plasmaimg);
    imlib_free_pixmap_and_mask(_plasmaimg.pmap);
    _plasmaimg.pmap = _plasmaimg.mask = 0;
    loadimg(&_attr, &_swordimg, "sword.jpg");
    initimg(&_attr, &_swordimg, 0);
    _getimg(_swordimg.pmap, &_swordimg);
    imlib_free_pixmap_and_mask(_swordimg.pmap);
    _swordimg.pmap = _swordimg.mask = 0;
    _blackimg.width = _winimg.width = TEST_WIDTH;
    _blackimg.height = _winimg.height = TEST_HEIGHT;
    initimg(&_attr, &_blackimg, 1);
    XSetForeground(_attr.disp, _attr.gc, BlackPixel(_attr.disp, DefaultScreen(_attr.disp)));
    XFillRectangle(_attr.disp, _blackimg.pmap, _attr.gc, 0, 0, _blackimg.width, _blackimg.height);
    initimg(&_attr, &_winimg, 0);
    
    return;
}

void
initwin(void)
{
    unsigned long exposed = 0;
#if (USE_RENDER)
    XRenderPictureAttributes pattr;
#endif

#if (USE_COMPOSITE)
    XCompositeRedirectWindow(_attr.disp, _attr.win);
#endif
#if (USE_RENDER)
    pattr.subwindow_mode = IncludeInferiors;
    _attr.pict = XRenderCreatePicture(_attr.disp, _attr.win, _attr.rendfmt,
                                      CPSubwindowMode, &pattr);
#endif
    XSelectInput(_attr.disp, _attr.win, ExposureMask);
    XMapWindow(_attr.disp, _attr.win);
    while (!exposed) {
        XEvent event;

        XNextEvent(_attr.disp, &event);
        switch (event.type) {
            case Expose:
#if 0
                if (event.xexpose.count == 0) {
                    XClearWindow(_attr.disp, _attr.win);
                }
#endif
                exposed = 1;

                break;
            default:

                break;
        }
    }

    return;
}

void
testfadein(void)
{
    argb32_t      *ptr1;
    argb32_t      *ptr2;
    argb32_t       pix;
    unsigned long  val;
    unsigned long  ul;
    PROFCLK(clock);

    for (val = 0 ; val < 256 ; val += 4) {
        if (val > 0xfb) {
            val = 0xff;
        }
        profstartclk(clock);
        ptr1 = (argb32_t *)_winimg.xim->data;
        ptr2 = (argb32_t *)_plasmaimg.xim->data;
        for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
//            gfxfadein(*ptr2, *ptr1, val);
            ptr2++;
            ptr1++;
        }
        profstopclk(clock);
        fprintf(stderr,
                "fade: %dx%d, %d steps: %lu microsecs\n",
                TEST_WIDTH,
                TEST_HEIGHT,
                256, profclkdiff(clock));
        putimg(&_winimg, _attr.win);
        XFlush(_attr.disp);
    }
}

void
testfadeout(void)
{
    argb32_t      *ptr1;
    argb32_t      *ptr2;
    argb32_t       pix;
    unsigned long  val;
    unsigned long  ul;
    PROFCLK(clock);

    for (val = 0xff ; val > 0 ; val -= 4) {
        if (val < 4) {
            val = 0;
        }
        profstartclk(clock);
        ptr1 = (argb32_t *)_winimg.xim->data;
        ptr2 = (argb32_t *)_plasmaimg.xim->data;
        for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
//            gfxfadein(*ptr2, *ptr1, val);
            ptr2++;
            ptr1++;
        }
        profstopclk(clock);
        fprintf(stderr,
                "fade: %dx%d, %d steps: %lu microsecs\n",
                TEST_WIDTH,
                TEST_HEIGHT,
                256, profclkdiff(clock));
        putimg(&_winimg, _attr.win);
        XFlush(_attr.disp);
        if (!val) {

            break;
        }
    }
}

void
testfadeint(unsigned long usecs)
{
    argb32_t      *ptr1;
    argb32_t      *ptr2;
    argb32_t       tmp;
    argb32_t       val;
    argb32_t       lastval;
    long           diff;
    unsigned long  total;
    unsigned long  stop;
    unsigned long  ul;
    struct timeval start;
    struct timeval cur;
    struct timeval end;
    float          fmul;
    PROFCLK(clock);
    PROFCLK(fade);

    gettimeofday(&start, NULL);
    end = start;
    tvaddconst(&end, usecs);
    gettimeofday(&cur, NULL);
    val = 0xff;
    lastval = 0;
    profstartclk(clock);
    stop = 0;
    _mm_empty();
    while (!stop) {
        profstartclk(fade);
        getimg(_blackimg.pmap, &_winimg);
        ptr1 = (argb32_t *)_winimg.xim->data;
        ptr2 = (argb32_t *)_plasmaimg.xim->data;
        if (tvgt(&cur, &end)) {
            fprintf(stderr, "stopping fade\n");
            val = 0xff;
            lastval = 0;
            stop = 1;
        } else {
            diff = usecs - tvcmp(&cur, &end);
            diff = min(diff, usecs);
            fmul = (float)diff / (float)usecs;
            val = (argb32_t)(255.0 * fmul);
        }
        if (val != lastval) {
            for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
#if (USE_MMX)
                gfxalphablendloq_asm_mmx(*ptr2, *ptr1, val);
//                *ptr1 = tmp;
#else
//                gfxalphablendhiq_const(*ptr2, *ptr1, val);
                gfxalphablendhiq_const(*ptr2, *ptr1, val);
#endif
                ptr2++;
                ptr1++;
            }
            putimg(&_winimg, _attr.win);
            XFlush(_attr.disp);
            lastval = val;
        }
        profstopclk(fade);
        gettimeofday(&cur, NULL);
    }
    profstopclk(clock);
    fprintf(stderr,
            "fade: %dx%d, %d steps: %lu microsecs\n",
            TEST_WIDTH, TEST_HEIGHT,
            256, profclkdiff(clock));
}

void
testfadeoutt(unsigned long usecs)
{
    argb32_t      *ptr1;
    argb32_t      *ptr2;
    argb32_t       tmp;
    argb32_t       val;
    argb32_t       lastval;
    long           diff;
    unsigned long  total;
    unsigned long  stop;
    unsigned long  ul;
    struct timeval start;
    struct timeval cur;
    struct timeval end;
    float          fmul;
    PROFCLK(clock);
    PROFCLK(fade);

    gettimeofday(&start, NULL);
    end = start;
    tvaddconst(&end, usecs);
    gettimeofday(&cur, NULL);
    val = 0;
    lastval = 0xff;
    profstartclk(clock);
    stop = 0;
    while (!stop) {
        profstartclk(fade);
        getimg(_blackimg.pmap, &_winimg);
        ptr1 = (argb32_t *)_winimg.xim->data;
        ptr2 = (argb32_t *)_plasmaimg.xim->data;
        if (tvgt(&cur, &end)) {
            val = 0;
            lastval = 0xff;
            stop = 1;
        } else {
            diff = usecs - tvcmp(&cur, &end);
            diff = min(diff, usecs);
            fmul = (float)diff / usecs;
            val = 0xff - (argb32_t)(0xff * fmul);
        }
        if (val != lastval) {
            for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
#if (USE_MMX)
//                gfxalphablendfast_const_mmx(*ptr2, *ptr1, val);
                gfxalphablendloq_asm_mmx(*ptr2, *ptr1, val);
//                *ptr1 = tmp;
#else
//                gfxalphablendhiq_const(*ptr2, *ptr1, val);
                gfxalphablendhiq_const(*ptr2, *ptr1, val);
#endif
                ptr2++;
                ptr1++;
            }
            putimg(&_winimg, _attr.win);
            XFlush(_attr.disp);
            lastval = val;
        }
        profstopclk(fade);
        gettimeofday(&cur, NULL);
        fprintf(stderr,
                "step: %dx%d, %d steps: %lu microsecs\n",
                TEST_WIDTH, TEST_HEIGHT,
                256, profclkdiff(fade));
    }
    profstopclk(clock);
    fprintf(stderr,
            "fade: %dx%d, %d steps: %lu microsecs\n",
            TEST_WIDTH, TEST_HEIGHT,
            256, profclkdiff(clock));
}

void
testmorph1t(unsigned long usecs)
{
    argb32_t      *ptr1;
    argb32_t      *ptr2;
    argb32_t      *ptr3;
    argb32_t       val;
    argb32_t       lastval;
    long           diff;
    unsigned long  total;
    unsigned long  first;
    unsigned long  stop;
    unsigned long  last;
    unsigned long  ul;
    struct timeval start;
    struct timeval cur;
    struct timeval end;
    float          fmul;
    PROFCLK(clock);
    PROFCLK(fade);

    gettimeofday(&start, NULL);
    end = start;
    tvaddconst(&end, usecs);
    gettimeofday(&cur, NULL);
    val = 0;
    lastval = 0xff;
    profstartclk(clock);
    first = 1;
    stop = last = 0;
    getimg(_blackimg.pmap, &_winimg);
    while (!stop) {
        profstartclk(fade);
        if (first) {
            val = 0;
            first = 0;
        } else if (tvgt(&cur, &end) || tvcmp(&cur, &end) < last) {
            val = 0xff;
            lastval = 0xff;
            stop = 1;
        } else {
            diff = usecs - tvcmp(&cur, &end);
            diff = min(diff, usecs);
            fmul = (float)diff / usecs;
            val = (argb32_t)(0xff * fmul);
        }
        if (!stop) {
            ptr1 = (argb32_t *)_winimg.xim->data;
            ptr2 = (argb32_t *)_plasmaimg.xim->data;
            ptr3 = (argb32_t *)_swordimg.xim->data;
#if (USE_MMX)
            fprintf(stderr, "val: %x\n", val);
            _mm_empty();
            for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
                gfxxfade1_mmx(*ptr2, *ptr3, *ptr1, val);
                ptr2++;
                ptr3++;
                ptr1++;
            }
#else
            for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
                gfxxfade1_jose(*ptr2, *ptr3, *ptr1, val);
                ptr2++;
                ptr3++;
                ptr1++;
            }
#endif
            putimg(&_winimg, _attr.win);
            XFlush(_attr.disp);
            lastval = val;
        }
        profstopclk(fade);
        last = profclkdiff(fade);
        gettimeofday(&cur, NULL);
        fprintf(stderr,
                "step: %dx%d, %d steps: %lu microsecs\n",
                TEST_WIDTH, TEST_HEIGHT,
                256, last);
    }
    ptr1 = (argb32_t *)_winimg.xim->data;
    ptr2 = (argb32_t *)_plasmaimg.xim->data;
    for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
        *ptr1++ = *ptr2++;
    }
    profstopclk(clock);
    fprintf(stderr,
            "fade: %dx%d, %d steps: %lu microsecs\n",
            TEST_WIDTH, TEST_HEIGHT,
            256, profclkdiff(clock));
}

#if (USE_MMX)
void
testfadein_mmx(void)
{
    argb32_t      *ptr1;
    argb32_t      *ptr2;
    argb32_t       tmp;
    unsigned long  val;
    unsigned long  ul;

    for (val = 0 ; val < 256 ; val++) {
        ptr1 = (argb32_t *)_winimg.xim->data;
        ptr2 = (argb32_t *)_plasmaimg.xim->data;
        for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
//            gfxalphablendfast_const_mmx(*ptr2, *ptr1, val);
            gfxalphablendloq_asm_mmx(*ptr2, *ptr1, val);
//            *ptr1 = val;
            ptr2++;
            ptr1++;
        }
        putimg(&_winimg, _attr.win);
        XFlush(_attr.disp);
    }
}
#endif

void
testdith15(void)
{
    argb32_t *ptr1;
    rgb555_t *ptr2;
    unsigned long ul;
    
    ptr1 = _swordimg.xim->data;
    ptr2 = _winimg.xim->data;
    for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
        
    }
}

void
testscale(void)
{
    PROFCLK(clock);

    loadimg(&_attr, &_srcimg, "testhalf.jpg");
    initimg(&_attr, &_srcimg, 0);
    _getimg(_srcimg.pmap, &_srcimg);
    _scaleimg.width = TEST_WIDTH;
    _scaleimg.height = TEST_HEIGHT;
    initimg(&_attr, &_scaleimg, 0);
    profstartclk(clock);
    gfxscale(&_srcimg, &_scaleimg, 0, 0, 0, 0, TEST_WIDTH, TEST_HEIGHT);
    initimg(&_attr, &_plasmaimg, 0);
#if 0
    scaleantialias(_srcimg.data, _scaleimg.data,
                   TEST_WIDTH / 2, TEST_HEIGHT / 2,
                   TEST_WIDTH, TEST_HEIGHT);
#endif
#if 0
    scale2x(_srcimg.data, _scaleimg.data,
            TEST_WIDTH / 2, TEST_HEIGHT / 2);
#endif
    putimg(&_scaleimg, _attr.win);
    XFlush(_attr.disp);
    profstopclk(clock);
    fprintf(stderr, "scale %lux%lu -> %lux%lu: %lu microsecs\n",
            _plasmaimg.width, _plasmaimg.height, _winimg.width, _winimg.height, profclkdiff(clock));
}

void
testmorph1(void)
{
    argb32_t val;

    for (val = 0 ; val < 256 ; val += 5) {
        fprintf(stderr, "%d\n", val);
        gfxmorph1(&_plasmaimg, &_swordimg, &_winimg, val);
        putimg(&_winimg, _attr.win);
        XFlush(_attr.disp);
    }
}

#if (USE_SHM)
void
dtimg(struct attr *attr, struct img *img, int pmap)
{
    XShmDetach(attr->disp, &img->ximseg);
    shmdt(img->ximseg.shmaddr);
    shmctl(img->ximseg.shmid, IPC_RMID, NULL);
    if (pmap) {
        XShmDetach(attr->disp, &img->pmapseg);
        shmdt(img->pmapseg.shmaddr);
        shmctl(img->pmapseg.shmid, IPC_RMID, NULL);
    }

    return;
}
#endif

int
main(int argc,
     char *argv[])
{
    argb32_t val;
    unsigned long ul;

    PROFCLK(clock);

    init();
    initx();
    initimgs();
    initwin();

    testdith15();
#if 0
    testmorph1t(2500000);
    testfadeint(5000000);
    testmorph1();
    testfadeout();
    testscale();
#endif
//    testfadein_mmx();

#if (USE_SHM)
    dtimg(&_attr, &_plasmaimg, 0);
    dtimg(&_attr, &_swordimg, 0);
    dtimg(&_attr, &_blackimg, 1);
    dtimg(&_attr, &_winimg, 0);
#endif

    sleep(4);

    exit(0);
}

