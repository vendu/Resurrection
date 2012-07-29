#include <Resurrection/Resurrection.h>

void
R_print_app(struct R_app *app)
{
//    fprintf(stderr, "app: %p\n", app);
    fprintf(stderr, "APP: %p\n", app);
    if (app->argc) {
        fprintf(stderr, "argc: %d\n", app->argc);
        fprintf(stderr, "argv: %p\n", app->argv);
    }
    if (app->winid) {
        fprintf(stderr, "winid: %lu\n", app->winid);
    }
    if (app->window) {
        fprintf(stderr, "%dx%d@%d\n",
                app->window->w, app->window->h, app->depth);
    }

    return;
}

