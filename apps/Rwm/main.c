#include <Resurrection/Resurrection.h>
//#include <valgrind/valgrind.h>
#if defined(__CINT__)
#   include <G__ci.h>
#endif

int
main(int argc, char *argv[])
{
    int          nscreen;
    struct R_app app;
//    pid_t        pid;
    int          xfd;
    fd_set       readfds;

#if 0
    pid = fork();
    if (!pid) {
        char *arg[1] = { "Rl" };
        execvp("Rl", arg);
    }
#endif
#if 0
    memset(&app,
           0,
           sizeof(app));
    app.name = "Rwm";
#endif
    if (!Rwm_init(&app,
                  argc,
                  argv)) {
        fprintf(stderr, "wm initialization failed\n");
        fflush(stderr);
        
        exit(1);
    }
#if 0
    xfd = XConnectionNumber(app.display);
    FD_SET(xfd, &readfds);
    while (TRUE) {
        R_handle_events(&app);
#if 0
        if (_updateclock) {
            Rwm_update_clock(&_clock);
            _updateclock = 0;
        }
#endif
        Rselect(xfd + 1, &readfds, NULL, NULL, NULL);
    }
#endif
    while (TRUE) {
        pause();
    }

    exit(0);
}

