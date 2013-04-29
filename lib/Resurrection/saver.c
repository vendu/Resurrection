/*
 * saver.c - screen saver module for Resurrection.
 * Copyright (C) 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

void hexdump_expose(void *arg, XEvent *event);
void hexdump_exit(void *arg, XEvent *event);
void hexdump_dummy(void);
void alien_expose(void *arg, XEvent *event);
void alien_exit(void *arg, XEvent *event);
void alien_dummy(void);

static struct R_saver *Rsaver;

static const char *hex2ascii[] = {
    "00",
    "01",
    "02",
    "03",
    "04",
    "05",
    "06",
    "07",
    "08",
    "09",
    "0a",
    "0b",
    "0c",
    "0d",
    "0e",
    "0f",
    "10",
    "11",
    "12",
    "13",
    "14",
    "15",
    "16",
    "17",
    "18",
    "19",
    "1a",
    "1b",
    "1c",
    "1d",
    "1e",
    "1f",
    "20",
    "21",
    "22",
    "23",
    "24",
    "25",
    "26",
    "27",
    "28",
    "29",
    "2a",
    "2b",
    "2c",
    "2d",
    "2e",
    "2f",
    "30",
    "31",
    "32",
    "33",
    "34",
    "35",
    "36",
    "37",
    "38",
    "39",
    "3a",
    "3b",
    "3c",
    "3d",
    "3e",
    "3f",
    "40",
    "41",
    "42",
    "43",
    "44",
    "45",
    "46",
    "47",
    "48",
    "49",
    "4a",
    "4b",
    "4c",
    "4d",
    "4e",
    "4f",
    "50",
    "51",
    "52",
    "53",
    "54",
    "55",
    "56",
    "57",
    "58",
    "59",
    "5a",
    "5b",
    "5c",
    "5d",
    "5e",
    "5f",
    "60",
    "61",
    "62",
    "63",
    "64",
    "65",
    "66",
    "67",
    "68",
    "69",
    "6a",
    "6b",
    "6c",
    "6d",
    "6e",
    "6f",
    "70",
    "71",
    "72",
    "73",
    "74",
    "75",
    "76",
    "77",
    "78",
    "79",
    "7a",
    "7b",
    "7c",
    "7d",
    "7e",
    "7f",
    "80",
    "81",
    "82",
    "83",
    "84",
    "85",
    "86",
    "87",
    "88",
    "89",
    "8a",
    "8b",
    "8c",
    "8d",
    "8e",
    "8f",
    "90",
    "91",
    "92",
    "93",
    "94",
    "95",
    "96",
    "97",
    "98",
    "99",
    "9a",
    "9b",
    "9c",
    "9d",
    "9e",
    "9f",
    "a0",
    "a1",
    "a2",
    "a3",
    "a4",
    "a5",
    "a6",
    "a7",
    "a8",
    "a9",
    "aa",
    "ab",
    "ac",
    "ad",
    "ae",
    "af",
    "b0",
    "b1",
    "b2",
    "b3",
    "b4",
    "b5",
    "b6",
    "b7",
    "b8",
    "b9",
    "ba",
    "bb",
    "bc",
    "bd",
    "be",
    "bf",
    "c0",
    "c1",
    "c2",
    "c3",
    "c4",
    "c5",
    "c6",
    "c7",
    "c8",
    "c9",
    "ca",
    "cb",
    "cc",
    "cd",
    "ce",
    "cf",
    "d0",
    "d1",
    "d2",
    "d3",
    "d4",
    "d5",
    "d6",
    "d7",
    "d8",
    "d9",
    "da",
    "db",
    "dc",
    "dd",
    "de",
    "df",
    "e0",
    "e1",
    "e2",
    "e3",
    "e4",
    "e5",
    "e6",
    "e7",
    "e8",
    "e9",
    "ea",
    "eb",
    "ec",
    "ed",
    "ee",
    "ef",
    "f0",
    "f1",
    "f2",
    "f3",
    "f4",
    "f5",
    "f6",
    "f7",
    "f8",
    "f9",
    "fa",
    "fb",
    "fc",
    "fd",
    "fe",
    "ff"
};

uint8_t *
hexdump_convert(void *mem, size_t size, size_t *retsize)
{
    uint8_t *retptr;
    uint8_t *ptr, *end;
    uint8_t *data;
    const uint8_t *str;
    size_t len;

    ptr = (uint8_t *)mem;
    end = ptr + size;
    len = 3 * size;
    data = calloc(1, len);
    if (data == NULL) {

	return NULL;
    }
    retptr = data;
    while (ptr < end) {
	str = hex2ascii[*ptr];
	*data++ = *str++;
	*data++ = *str;
	*data++ = ' ';
	ptr++;
    }
    *retsize = len;

    return retptr;
}

struct R_saver *
hexdump_alloc(void)
{
    struct R_saver *newsaver;

    newsaver = calloc(1, sizeof(struct R_saver));

    return newsaver;
}

int
hexdump_main(int argc, char *argv[])
{
    struct R_app app;
    struct R_saver *saver;

    if (!R_init(&app, argc, argv)) {

        return -1;
    }

    saver = hexdump_alloc();
    if (saver == NULL) {

	return -1;
    }
    app.client = saver;
    if (hexdump_init(&app) < 0) {

	return -1;
    }
    while (TRUE) {
	R_handle_events(&app);
	if (saver->candraw) {
	    hexdump_draw(&app);
	}
	usleep(40000);
    }

    return 0;
}

int
hexdump_init(struct R_app *app)
{
    XFontStruct *fontinfo;
    struct R_saver *saver;

    saver = app->client;
    if (hexdump_init_data(app) < 0) {

	return -1;
    }

    fontinfo = R_load_font(app, "fixed");
    if (fontinfo == NULL) {
	
	return -1;
    }
    saver->fontinfo = fontinfo;
    saver->charasc = fontinfo->ascent;
    saver->chardesc = fontinfo->descent;
    saver->charw = X_FONT_WIDTH(fontinfo);
    saver->charh = X_FONT_HEIGHT(fontinfo) - saver->chardesc;
    
    hexdump_init_windows(app);
    if (hexdump_init_gcs(app) < 0) {

	return -1;
    }
    if (hexdump_init_buffer(app) < 0) {

	return -1;
    }
    hexdump_set_event_handlers(app);
    R_map_window(app->window);
    
    return 0;
}

int
hexdump_init_data(struct R_app *app)
{
    uint8_t *hexdata;
    size_t hexsize;
    struct R_saver *saver;

    saver = app->client;
    hexdata = hexdump_convert(&hexdump_main,
			      (size_t)((unsigned long)&hexdump_dummy
				       - (unsigned long)&hexdump_main),
			      &hexsize);
    if (hexdata == NULL) {

	return -1;
    }
    saver->hexdata = hexdata;
    saver->datasize = hexsize;

    return 0;
}

void
hexdump_init_windows(struct R_app *app)
{
    struct R_saver *saver;

    saver = app->client;
    app->window = R_create_window(app,
                                  NULL,
                                  0);
    R_add_window(app->window);
    R_resize_window(app->window,
                    HEXDUMP_WIDTH(saver),
                    HEXDUMP_HEIGHT(saver));

    return;
}

int
hexdump_init_gcs(struct R_app *app)
{
    GC newgc;
    XGCValues gcvalues;
    XColor newcolor;
    struct R_saver *saver;


    saver = app->client;
    memset(&gcvalues, 0, sizeof(gcvalues));
    if (!XParseColor(app->display,
		     app->colormap,
		     "green",
		     &newcolor)) {

	return -1;
    }
    if (!XAllocColor(app->display,
		     app->colormap,
		     &newcolor)) {

	return -1;
    }
    gcvalues.foreground = newcolor.pixel;
    gcvalues.function = GXcopy;
    gcvalues.font = saver->fontinfo->fid;
    newgc = XCreateGC(app->display,
		      app->window->id,
		      GCForeground | GCFunction | GCFont,
		      &gcvalues);
    if (newgc == NULL) {

	return -1;
    }
    saver->fggc = newgc;
    gcvalues.foreground = BlackPixel(app->display,
				     DefaultScreen(app->display));
    newgc = XCreateGC(app->display,
		      app->window->id,
		      GCForeground | GCFunction,
		      &gcvalues);
    if (newgc == NULL) {

	return -1;
    }
    saver->bggc = newgc;
    
    return 0;
}

int
hexdump_init_buffer(struct R_app *app)
{
    Pixmap pixmap;
    struct R_saver *saver;

    saver = app->client;
    pixmap = XCreatePixmap(app->display,
			   app->window->id,
			   HEXDUMP_WIDTH(saver),
			   HEXDUMP_HEIGHT(saver),
			   app->depth);
    if (pixmap == None) {

	return -1;
    }
    saver->background = pixmap;
    pixmap = XCreatePixmap(app->display,
			   app->window->id,
			   HEXDUMP_WIDTH(saver),
			   HEXDUMP_HEIGHT(saver),
			   app->depth);
    if (pixmap == None) {

	return -1;
    }
    saver->drawbuffer = pixmap;
    saver->nrows = saver->datasize / HEXDUMP_COLUMNS;
    if (saver->nrows * HEXDUMP_COLUMNS != saver->datasize) {
	saver->nrows++;
    }

    return 0;
}

void
hexdump_draw(struct R_app *app)
{
    struct R_saver *saver;

    saver = app->client;
    hexdump_clear_buffer(app);
    hexdump_draw_buffer(app);
    hexdump_sync(app);

    return;
}

void
hexdump_clear_buffer(struct R_app *app)
{
    struct R_saver *saver;

    saver = app->client;
    XFillRectangle(app->display,
		   saver->drawbuffer,
		   saver->bggc,
		   0, 0,
		   HEXDUMP_WIDTH(saver),
		   HEXDUMP_HEIGHT(saver));

    return;
}

void
hexdump_draw_buffer(struct R_app *app)
{
    int i, row, nrows;
    int len;
    struct R_saver *saver;

    saver = app->client;
    row = saver->row;
    nrows = saver->nrows;
    for (i = 0 ; i < HEXDUMP_ROWS ; i++) {
	if (row == nrows - 1) {
	    len = saver->datasize - row * HEXDUMP_COLUMNS;
	    len = MAX(len, HEXDUMP_COLUMNS - 1);
	} else {
	    len = HEXDUMP_COLUMNS - 1;
	}
	XDrawString(app->display,
		    saver->drawbuffer,
		    saver->fggc,
		    0, (i + 1) * saver->charasc,
		    saver->hexdata + row * HEXDUMP_COLUMNS,
		    len);
	row++;
	if (row == nrows) {
	    row = 0;
	}
    }
    saver->row = row;

    return;
}

void
hexdump_sync(struct R_app *app)
{
    struct R_saver *saver;

    saver = app->client;
    XSetWindowBackgroundPixmap(app->display,
			       app->window->id,
			       saver->drawbuffer);
    XClearWindow(app->display,
		 app->window->id);
    XFlush(app->display);

    return;
}

void
hexdump_set_event_handlers(struct R_app *app)
{
    R_set_window_event_handler(app->window, Expose,
                               hexdump_expose);
    R_set_window_event_handler(app->window, KeyPress,
                               hexdump_exit);
    R_set_window_event_handler(app->window, ButtonPress,
                               hexdump_exit);
    R_add_window_events(app->window,
                        ExposureMask
                        | KeyPressMask
                        | ButtonPressMask);

    return;
}

void
hexdump_expose(void *arg, XEvent *event)
{
    struct R_saver *saver;

    fprintf(stderr, "EXPOSE\n");

    saver = R_global.app->client;
    saver->candraw = 1;

    return;
}

void
hexdump_exit(void *arg, XEvent *event)
{
    exit(0);
}

void
hexdump_dummy(void)
{
    return;
}

#define ALIEN_HAS_GLYPH(c) \
    (((c) >= '0' && c <= '9') \
     || ((c) >= 'A' && c <= 'Z') \
     || ((c) >= 'a' && c <= 'Z'))

uint8_t *
alien_convert(void *mem, size_t size, size_t *retsize)
{
    uint8_t *retptr;
    uint8_t *ptr, *end;
    uint8_t *data;
    size_t len;

    ptr = (uint8_t *)mem;
    end = ptr + size;
    data = calloc(1, size);
    if (data == NULL) {

	return NULL;
    }
    retptr = data;
    len = 0;
    while (ptr < end) {
	if (ALIEN_HAS_GLYPH(*ptr)) {
	    *data++ = *ptr;
	    len++;
	}
	ptr++;
    }
    *retsize = len;

    return retptr;
}

struct R_saver *
alien_alloc(void)
{
    struct R_saver *newsaver;

    newsaver = calloc(1, sizeof(struct R_saver));

    return newsaver;
}

int
alien_main(int argc, char *argv[])
{
    struct R_app app;

    if (!R_init(&app, argc, argv)) {

        return -1;
    }
    Rsaver = alien_alloc();
    if (Rsaver == NULL) {

	return -1;
    }

    if (alien_init(Rsaver) < 0) {

	return -1;
    }

    while (TRUE) {
	R_handle_events(&app);
	if (Rsaver->candraw) {
	    alien_draw(Rsaver);
	}
	usleep(40000);
    }

    return 0;
}

int
alien_init(struct R_saver *saver)
{
    struct R_app *app;
    XFontStruct *fontinfo;

    app = R_global.app;
    if (R_init(app, "Rsaver", 0, NULL) < 0) {
	
	return -1;
    }
    
    if (alien_init_data(saver) < 0) {

	return -1;
    }
#if 0
    fontinfo = R_load_font(app, "fixed");
    if (fontinfo == NULL) {
	
	return -1;
    }
    saver->fontinfo = fontinfo;
    saver->charasc = fontinfo->ascent;
    saver->chardesc = fontinfo->descent;
    saver->charw = FONT_WIDTH(fontinfo);
    saver->charh = FONT_HEIGHT(fontinfo) - saver->chardesc;
#endif
    if (alien_init_font(saver) < 0) {

	return -1;
    }
    
    alien_init_windows(saver);
    if (alien_init_gcs(saver) < 0) {

	return -1;
    }
    if (alien_init_colors(saver) < 0) {

	return -1;
    }
    if (alien_init_buffer(saver) < 0) {

	return -1;
    }
    alien_set_event_handlers(saver);
    R_map_window(app->window);
    Rsaver = saver;
    
    return 0;
}

int
alien_init_data(struct R_saver *saver)
{
    int row;
    uint8_t *aliendata, *screendata;
    size_t datasize;

    aliendata = alien_convert(&alien_main,
			      (size_t)((unsigned long)&alien_dummy
				       - (unsigned long)&alien_main),
			      &datasize);
    if (aliendata == NULL) {

	return -1;
    }
    screendata = calloc(1, ALIEN_ROWS * ALIEN_COLUMNS);
    if (screendata == NULL) {

	return -1;
    }
    memcpy(screendata, aliendata, ALIEN_COLUMNS);
    memset(screendata + ALIEN_COLUMNS, ' ', (ALIEN_ROWS - 1) * ALIEN_COLUMNS);
    saver->aliendata = aliendata;
    saver->screendata = screendata;
    saver->datasize = datasize;

    return 0;
}

int
alien_init_font(struct R_saver *saver)
{
    struct R_image image;

    memset(&image, 0, sizeof(image));
    if (R_load_image_imlib2(R_global.app,
                            RESURRECTION_IMAGE_SEARCH_PATH "misc/japanese.png",
                            &image)
	< 0) {

	return -1;
    }
    R_render_image_imlib2(&image,
                          R_global.app->window,
                          ALIEN_FONT_WIDTH,
                          ALIEN_FONT_HEIGHT,
                          0);
    saver->fontpixmap = image.pixmap;
    fprintf(stderr, "saver->fontpixmap == %lu\n", saver->fontpixmap);
    image.pixmap = None;
    R_destroy_image_imlib2(&image);

    return 0;
}

void
alien_init_windows(struct R_saver *saver)
{
    R_resize_window(R_global.app->window,
                    ALIEN_WIDTH(saver),
                    ALIEN_HEIGHT(saver));

    return;
}

int
alien_init_gcs(struct R_saver *saver)
{
    GC newgc;
    XGCValues gcvalues;
    XColor newcolor;

    memset(&gcvalues, 0, sizeof(gcvalues));
    gcvalues.function = GXcopy;
    newgc = XCreateGC(R_global.app->display,
		      saver->app->window->id,
		      GCFunction,
		      &gcvalues);
#if 0
    gcvalues.font = saver->fontinfo->fid;
    newgc = XCreateGC(saver->app->display,
		      saver->app->window->id,
		      GCFunction | GCFont,
		      &gcvalues);
#endif
    if (newgc == NULL) {

	return -1;
    }
    saver->fggc = newgc;
    gcvalues.foreground = BlackPixel(saver->app->display,
				     DefaultScreen(saver->app->display));
    newgc = XCreateGC(saver->app->display,
		      saver->app->window->id,
		      GCForeground | GCFunction,
		      &gcvalues);
    if (newgc == NULL) {

	return -1;
    }
    saver->bggc = newgc;
    
    return 0;
}

int
alien_init_colors(struct R_saver *saver)
{
    unsigned long *pixels;
    int i;
    XColor newcolor;

    pixels = saver->pixels;
    memset(&newcolor, 0, sizeof(newcolor));
    for (i = 0 ; i < ALIEN_ROWS ; i++) {
	newcolor.green = 0 + (i + 1) * (65535 / ALIEN_ROWS);
	if (!XAllocColor(saver->app->display,
			 saver->app->colormap,
			 &newcolor)) {
	    
	    return -1;
	}
	*pixels = newcolor.pixel;
	pixels++;
    }

    return 0;
}

int
alien_init_buffer(struct R_saver *saver)
{
    Pixmap pixmap;

    pixmap = XCreatePixmap(saver->app->display,
			   saver->app->window->id,
			   ALIEN_WIDTH(saver),
			   ALIEN_HEIGHT(saver),
			   saver->app->depth);
    if (pixmap == None) {

	return -1;
    }
    saver->background = pixmap;
    pixmap = XCreatePixmap(saver->app->display,
			   saver->app->window->id,
			   ALIEN_WIDTH(saver),
			   ALIEN_HEIGHT(saver),
			   saver->app->depth);
    if (pixmap == None) {

	return -1;
    }
    saver->drawbuffer = pixmap;
    saver->nrows = ALIEN_ROWS;
    saver->nextchar = saver->aliendata + ALIEN_COLUMNS;

    return 0;
}

void
alien_draw(struct R_saver *saver)
{
    alien_update_screen(saver);
    alien_clear_buffer(saver);
    alien_draw_buffer(saver);
    alien_sync(saver);
}

#define ALIEN_SPEED(c, r) (((c) >> 5) + ((r) >> 3))

void
alien_update_screen(struct R_saver *saver)
{
    uint8_t *screen, *end, *next;
    int *bottom;
    int nrows;
    int row, column;
    int c, speed;
    int newrow;
    int lastrow;

    screen = saver->screendata;
    end = saver->aliendata + saver->datasize;
    next = saver->nextchar;
    bottom = saver->bottomrows;
    nrows = saver->nrows;
    for (column = 0 ; column < ALIEN_COLUMNS ; column++) {
	row = *bottom;
#if 0
	for (newrow = 0 ; newrow < row ; newrow++) {
	    screen[newrow * ALIEN_COLUMNS + column] = ' ';
	}
#endif
	c = screen[row * ALIEN_COLUMNS + column];
	speed = ALIEN_SPEED(c, row);
	speed = MAX(speed, 1);
	newrow = row + speed;
	lastrow = MIN(newrow, nrows - 1);
	while (row <= lastrow) {
	    screen[row * ALIEN_COLUMNS + column] = c;
	    row++;
	}
	if (newrow > nrows - 1) {
	    screen[column] = *next;
	    next++;
	    if (next == end) {
		next = saver->aliendata;
	    }
	    for (row = 1 ; row < nrows ; row++) {
		screen[row * ALIEN_COLUMNS + column] = ' ';
	    }
	    *bottom = 0;
	} else {
	    *bottom = lastrow;
	}
	bottom++;
    }
    saver->nextchar = next;

    return;
}

void
alien_clear_buffer(struct R_saver *saver)
{
    XFillRectangle(saver->app->display,
		   saver->drawbuffer,
		   saver->bggc,
		   0, 0,
		   ALIEN_WIDTH(saver),
		   ALIEN_HEIGHT(saver));

    return;
}

#if 0
void
alien_draw_buffer(struct R_saver *saver)
{
    unsigned long *pixels;
    int c;
    int row, column, nrows;
    int glyph;

    row = saver->row;
    nrows = saver->nrows;
    for (row = 0 ; row < nrows ; row++) {
	for (column = 0 ; column < ALIEN_COLUMNS ; column++) {
	    c = saver->screendata[row * ALIEN_COLUMNS + column];
	    if (c == ' ') {

		continue;
	    }
	    glyph = 0;
	    if (c >= '0' && c <= '9') {
		glyph = c - '0';
	    } else if (c >= 'a' && c <= 'z') {
		glyph = 10 + c - 'a';
	    } else if (c >= 'A' && c <= 'Z') {
		glyph = 10 + 'z' - 'a' + 1 + c - 'A';
	    }
	    if ((glyph * ALIEN_GLYPH_WIDTH > ALIEN_GLYPHS * ALIEN_GLYPH_WIDTH - ALIEN_GLYPH_WIDTH)
		|| row * ALIEN_GLYPH_HEIGHT > ALIEN_GLYPHS * ALIEN_GLYPH_HEIGHT - ALIEN_GLYPH_HEIGHT) {
		fprintf(stderr, "BAH!\n");
	    }
	    XCopyArea(saver->app->display,
		      saver->fontpixmap,
		      saver->drawbuffer,
		      saver->fggc,
		      glyph * ALIEN_GLYPH_WIDTH,
		      row * ALIEN_GLYPH_HEIGHT,
		      ALIEN_GLYPH_WIDTH,
		      ALIEN_GLYPH_HEIGHT,
		      column * ALIEN_GLYPH_WIDTH,
		      row * ALIEN_GLYPH_HEIGHT);
	}
    }

    return;
}
#endif

void
alien_draw_buffer(struct R_saver *saver)
{
    unsigned long *pixels;
    int row, nrows;

    pixels = saver->pixels;
    row = saver->row;
    nrows = saver->nrows;
    for (row = 0 ; row < nrows ; row++) {
	XSetForeground(saver->app->display,
		       saver->fggc,
		       *pixels);
	XDrawString(saver->app->display,
		    saver->drawbuffer,
		    saver->fggc,
		    0, (row + 1) * saver->charasc,
		    saver->screendata + row * ALIEN_COLUMNS,
		    ALIEN_COLUMNS);
	pixels++;
    }

    return;
}

void
alien_sync(struct R_saver *saver)
{
    XSetWindowBackgroundPixmap(saver->app->display,
			       saver->app->window->id,
			       saver->drawbuffer);
    XClearWindow(saver->app->display,
		 saver->app->window->id);
    XFlush(saver->app->display);

    return;
}

void
alien_set_event_handlers(struct R_saver *saver)
{
    R_set_window_event_handler(saver->app->window, Expose,
                               alien_expose);
    R_set_window_event_handler(saver->app->window, KeyPress,
                               alien_exit);
    R_set_window_event_handler(saver->app->window, ButtonPress,
                               alien_exit);
    R_add_window_events(saver->app->window,
                        ExposureMask
                        | KeyPressMask
                        | ButtonPressMask);
    
    return;
}

void
alien_expose(void *arg, XEvent *event)
{
    Rsaver->candraw = 1;

    return;
}

void
alien_exit(void *arg, XEvent *event)
{
    exit(0);
}

void
alien_dummy(void)
{
    return;
}


