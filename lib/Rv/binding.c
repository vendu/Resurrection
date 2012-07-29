#include <Resurrection/Resurrection.h>

#define RV_VIEW_BINDINGS 7

void
Rv_exit_func(void *arg,
             XEvent *event,
             void *bindarg);
void
Rv_prev_func(void *arg,
             XEvent *event,
             void *bindarg);
void
Rv_next_func(void *arg,
             XEvent *event,
             void *bindarg);
void
Rv_slide_func(void *arg,
            XEvent *event,
            void *bindarg);
void
Rv_fit_func(void *arg,
            XEvent *event,
            void *bindarg);

struct R_binding Rv_view_bindings[RV_VIEW_BINDINGS + 1] = {
    {
        R_MAKE_BINDING(R_q_KEYSYM,
                       0),
        Rv_exit_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(R_space_KEYSYM,
                       0),
        Rv_next_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(R_backspace_KEYSYM,
                       0),
        Rv_prev_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON1_FLAG),
        Rv_next_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON2_FLAG),
        Rv_slide_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(0,
                       R_BINDING_BUTTON3_FLAG),
        Rv_prev_func,
        NULL,
        NULL
    },
    {
        R_MAKE_BINDING(R_f_KEYSYM,
                       0),
        Rv_fit_func,
        NULL,
        NULL
    },
    { 0, NULL, NULL, NULL }
};

int
Rv_init_view_bindings(struct R_app *app)
{
    struct R_window *view;
    struct R_binding *binding;
    struct R_binding *viewbinding;
    int i;
    
    for (i = 0 ; i < RV_VIEW_BINDINGS + 1 ; i++) {
        view = app->window;
        viewbinding = &Rv_view_bindings[0];
        while (viewbinding->mask) {
            binding = R_create_binding(view,
                                       viewbinding->mask,
                                       viewbinding->func,
                                       viewbinding->arg);
            if (binding == NULL) {
                
                return FALSE;
            }
            if (!R_add_binding(binding,
                               view)) {
                
                return FALSE;
            }
            viewbinding++;
        }
    }

    return TRUE;
}

void
Rv_exit_func(void *arg,
             XEvent *event,
             void *bindarg)
{
    exit(0);
}

void
Rv_prev_func(void *arg,
             XEvent *event,
             void *bindarg)
{
    struct R_app    *app;
    struct R_window *window;

    window = arg;
    app = window->app;
    Rv_set_file(app, -1);

    return;
}

void
Rv_next_func(void *arg,
             XEvent *event,
             void *bindarg)
{
    struct R_app    *app;
    struct R_window *window;

    window = arg;
    app = window->app;
    Rv_set_file(app, 1);

    return;
}

void
Rv_slide_func(void *arg,
             XEvent *event,
             void *bindarg)
{
    struct R_window *window;

#if 0
    fprintf(stderr, "SLIDE\n");
    window = arg;
    if (window->clientflags & RV_SLIDE_FLAG) {
        window->clientflags &= ~RV_SLIDE_FLAG;
    } else {
        window->clientflags |= RV_SLIDE_FLAG;
        alarm(5);
    }
#endif

    return;
}

void
Rv_fit_func(void *arg,
            XEvent *event,
            void *bindarg)
{
    struct R_window *window;
    struct R_file   *curfile;

    curfile = _Rvcurfile;

    window = arg;
    window->clientflags |= RV_FIT_FLAG;
    Rv_show_image(window,
                  curfile);

}

