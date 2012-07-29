#include <Resurrection/Resurrection.h>

#define TEST_WIDTH  320
#define TEST_HEIGHT 240

struct R_window *
Rwm_init_test(struct R_app *app)
{
    struct R_window *window;
    struct R_window *frame;
    struct R_wm     *wm;

    wm = app->client;
    window = R_create_window(app,
                             app->window,
                             0);
    R_add_window(window);
    window->x = 0;
    window->y = 64;
    window->w = TEST_WIDTH;
    window->h = TEST_HEIGHT;

    window->str = "test";
    frame = Rwm_frame_window(window);
    Rwm_reparent_window(frame,
                        wm->desktop,
                        window->x,
                        window->y);
    Rwm_init_client_events(window);
    R_map_subwindows(frame);
    R_map_window(frame);

    return window;
}




