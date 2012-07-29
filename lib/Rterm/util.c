/*
 * util.c - utility functions for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#define X_VISUAL_CLASSES 6
static const char *visualclassnames[X_VISUAL_CLASSES] =
{
    "StaticGray",
    "GrayScale",
    "StaticColor",
    "PseudoColor",
    "TrueColor",
    "DirectColor"
};

/* return a newly allocated buffer containing home directory name. */
char *
get_homedir(void)
{
    char *dirname;
    char *envstr;
    uid_t uid;
    struct passwd *passwd;

    if ((dirname = calloc(1, PATH_MAX + 1)) == NULL) {

	return NULL;
    }
    
    if ((envstr = getenv("HOME"))) {
	strncpy(dirname, envstr, PATH_MAX);
    } else {
	if ((envstr = getenv("USER"))) {
	    passwd = getpwnam(envstr);
	} else if ((envstr = getenv("LOGNAME"))) {
	    passwd = getpwnam(envstr);
	} else {
	    uid = getuid();
	    passwd = getpwuid(uid);
	}

	if (passwd) {
	    strncpy(dirname, passwd->pw_dir, PATH_MAX);

	    free(passwd);
	}
    }

    return dirname;
}

/*
 * NOTE: R_system.h defines OPEN_MAX to NOFILE or _NFILE if it's not defined
 * on the system. R_system.h also defines getdtablesize() for systems that
 * don't have it.
 */

int
open_max(void)
{
    int maxfds;
#if (USE_GETRLIMIT)
    struct rlimit rlimit;
#endif /* USE_GETRLIMIT */

#if (USE_SYSCONF)
    if ((maxfds = sysconf(_SC_OPEN_MAX)) >= 0) {

	return maxfds;
    }
#endif /* USE_SYSCONF */

#if (USE_GETRLIMIT)
    if (getrlimit(RLIMIT_NOFILE, &rlimit) == 0
	&& rlimit.rlim_cur != RLIM_INFINITY) {

	return (rlimit.rlim_cur);
    }
#endif /* USE_GETRLIMIT */

    if ((maxfds = getdtablesize()) >= 0) {

	return maxfds;
    }

    return -1;
}

int
cube_root(int x)
{
    int bits, n;
    
    if (x < 0) {
	
	return -1;
    }
    
    bits = 0;
    n = x;
    
    while (n) {
	bits++;
	
	n >>= 1;
    }
    
    return (cube_root_with_bits(x, bits));
}

int
cube_root_with_bits(int x, int bits)
{
    return (cube_root_with_guess(x, x >> 2 * bits / 3));
}

int
cube_root_with_guess(int x, int guess)
{
    int delta;
    
    if (x < 0) {
	
	return -1;
    } else if (x == 0) {
	
	return 0;
    }
    
    if (guess < 1) {
	guess = 1;
    }

    do {
	delta = (guess - x / (guess * guess)) / 3;
	
	guess -= delta;
    } while (delta != 0);
    
    if (guess * guess * guess > x) {
	guess--;
    }
    
    return guess;
}

int
compare_pixels(const void *color1, const void *color2)
{
    R_color_t *c1, *c2;

    if (color1 == NULL
	|| color2 == NULL) {
	/* FIXME: what should we return here? */

	return 0;
    }

    c1 = (R_color_t *)color1;
    c2 = (R_color_t *)color2;

    if (*c1 < *c2) {
	return -1;
    } else if (*c1 > *c2) {
	return 1;
    }

    return 0;
}

int
pixels_contiguous(R_color_t *pixels, int npixels, int colors, int delta,
		  int *first, int *remainder)
{
    int i, count;

    if (pixels == NULL
	|| npixels <= 0
	|| colors <= 0
	|| delta <= 0
	|| first == NULL
	|| remainder == NULL) {

	return 0;
    }

    *first = 0;

    if (npixels == colors) {
	*remainder = 0;

	return 1;
    }

    *remainder = npixels - 1;

    i = 1;

    count = 1;

    while (count < colors && colors - count <= *remainder) {
	if (pixels[(i - 1)] + delta == pixels[i]) {
	    count++;
	} else {
	    count = 1;

	    *first = i;
	}

	i++;

	(*remainder)--;
    }

    if (count != colors) {
	
	return 0;
    }

    return 1;
}

int
contiguous_one_bits(unsigned long mask)
{
    int i, ones;
    unsigned long bit;

    ones = 0;

    for (i = 0, bit = 1 ;
	 i < sizeof(unsigned long) * 8 ;
	 i++, bit <<= 1) {
	if ((mask & bit) == 0) {

	    continue;
	}

	while ((mask & bit) && i < sizeof(unsigned long) * 8 - 1) {
	    i++;

	    ones++;

	    bit <<= 1;
	}

	return ones;
    }

    return ones;
}

void
print_visualinfo(XVisualInfo *visualinfo)
{
    printf("Visual ID == %ld\n", visualinfo->visualid);
    printf("    depth == %u\n", visualinfo->depth);
    printf("    class == %s\n", visualclassnames[(visualinfo->class)]);
    printf("    red_mask == %lx\n", visualinfo->red_mask);
    printf("    green_mask == %lx\n", visualinfo->green_mask);
    printf("    blue_mask == %lx\n", visualinfo->blue_mask);
    printf("    colormap_size == %d\n", visualinfo->colormap_size);
    printf("    bits_per_rgb == %d\n", visualinfo->bits_per_rgb);

    return;
}

