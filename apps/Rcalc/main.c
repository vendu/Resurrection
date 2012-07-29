#include <Resurrection/Resurrection.h>

#define RCALC_BUTTON_WIDTH  36
#define RCALC_BUTTON_HEIGHT 24

/* arithmetic ops */
#define RCALC_OP_ADD        '+'
#define RCALC_OP_SUB        '-'
#define RCALC_OP_MUL        '*'
#define RCALC_OP_DIV        '/'
#define RCALC_OP_MOD        '%'
/* logical ops */
#define RCALC_OP_AND        '&'
#define RCALC_OP_OR         '|'
#define RCALC_OP_XOR        '^'
#define RCALC_OP_CMPL       '~'
/* other ops */
#define RCALC_OP_SHL        '<'
#define RCALC_OP_SHR        '>'

int
Rcalc_main(struct R_app *app,
           int argc,
           char *argv[])
{
}

int
main(int argc,
     char *argv[])
{
    struct R_app app;

    memset(&app, 0, sizeof(app));
    if (!R_init(&app, argc, argv)) {

        exit(1);
    }

    exit(Rcalc_main(&app, argc, argv));
}

