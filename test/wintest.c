#include <Resurrection/Resurrection.h>

void expose_func(void *arg,
                 XEvent *event);
void configure_func(void *arg,
                    XEvent *event);

#define WINDOW_FLAGS R_WINDOW_OVERRIDE_FLAG

int
main(int argc, char *argv[])
{
    struct R_app app;
    struct R_image *image;
    struct R_window *window;
    
    app.argc = argc;
    app.argv = argv;
    R_init(&app,
           argc,
           argv);
    image = R_load_image_imlib2(&app,
                                "Resurrection.jpg",
                                NULL);
    window = R_create_window(&app,
                             NULL,
                             WINDOW_FLAGS);
    R_resize_window(window,
                    640,
                    480);
    window->image = image;
    R_set_window_event_handler(window,
                               Expose,
                               expose_func);
    R_set_window_event_handler(window,
                               ConfigureNotify,
                               configure_func);
    R_add_window_events(window,
                        ExposureMask
                        | StructureNotifyMask);
    R_add_window(window);
    R_map_window(window);
    while (TRUE) {
        R_handle_events(&app);
    }
    
    exit(0);
}

void
expose_func(void *arg,
            XEvent *event)
{
    struct R_window *window;
    
    if (event->xexpose.count == 0) {
        window = arg;
        window->sysflags |= R_WINDOW_EXPOSED_FLAG;
        R_clear_window(window);
    }

    return;
}

void
configure_func(void *arg,
               XEvent *event)
{
    struct R_window *window;

    window = arg;
    window->x = event->xconfigure.x;
    window->y = event->xconfigure.y;
    window->w = event->xconfigure.width;
    window->h = event->xconfigure.height;
    fprintf(stderr, "%dx%d @ (%d,%d)\n",
            window->w, window->h, window->x, window->y);
    R_set_background_imlib2(window->image,
                            window,
                            -1,
                            -1,
                            0);

    return;
}

