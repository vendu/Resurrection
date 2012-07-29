#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

int
R_init_shm_seg(size_t size, XShmSegmentInfo *seg)
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
R_init_shm_image(struct R_imgage *image, int pmap)
{
    img->xim = XShmCreateImage(attr->disp,
                               attr->vis,
                               attr->depth,
                               ZPixmap,
                               NULL,
                               &image->shmimg.ximseg,
                               image->width,
                               image->height);
    if (!image->shmimg->xim) {
        fprintf(stderr, "cannot allocate image structure\n");

        exit(1);
    }
    image->shmimg->shmid = R_init_shm_seg(image->w * image->h * 4, &image->shmimg.ximseg);
    img->xim->data = image->shmimg.ximseg.shmaddr;
    if (image->shmseg->shmid < 0) {

        exit(1);
    }
    XShmAttach(attr->disp,
               &image->shmseg->ximseg);
    if (pmap) {
        image->shmsegpmap->shmid = R_init_shm_seg(img->width * img->height * 4, &img->pmapseg);
        if (image->shmsegpmap->shmid < 0) {
            
            exit(1);
        }
        XShmAttach(attr->disp,
                   &image->shmseg.pmapseg);
        image->shmsegpmap = XShmCreatePixmap(attr->disp,
                                     attr->win,
                                     image->shmseg.pmapseg.shmaddr,
                                     &image->shmseg.pmapseg,
                                     image->shmsegwidth,
                                     image->shmsegheight,
                                     attr->depth);
        if (!image->shmseg.pmap) {
            fprintf(stderr, "cannot create pixmap\n");

            exit(1);
        }
    }

    return;
}

#define _getimg(d, i) (i)->xim = XGetImage(_attr.disp, (d), 0, 0, (i)->width, (i)->height, ~0UL, ZPixmap)
#if (USE_SHM)
#define getimg(d, i) XShmGetImage(_attr.disp, (d), (i)->xim, 0, 0, ~0UL)
#define putimg(i, d) XShmPutImage(_attr.disp, (d), _attr.gc, (i)->xim, 0, 0, 0, 0, (i)->width, (i)->height, False);
#else
#define getimg(d, i) _getimg(d, i)
#define putimg(i, d) XPutImage(_attr.disp, (d), _attr.gc, (i)->xim, 0, 0, 0, 0, (i)->width, (i)->height);
#endif

