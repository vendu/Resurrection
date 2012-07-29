int
main(int argc,
     char *argv[])
{
    Display *disp;

    disp = XOpenDisplay(NULL);

    XCloseDisplay(disp);

    exit(0);
}
