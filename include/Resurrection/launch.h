#ifndef __RESURRECTION_LAUNCH_H__
#define __RESURRECTION_LAUNCH_H__

#include <Resurrection/conf/launch.h>

#define RL_EXEC_RTERM 1
#define RL_TERM_TABS  0
#define RL_RTERM_IPC  0
#define RL_OLDE_THEME 0
#define RL_MENU       1
#define RL_MAX_TABS   8
#define RL_SMALL      0 /* half-size images */
#define RL_LABEL      0
#define RL_BAR        0 /* all buttons visible */
#define RL_HORIZONTAL 0 /* otherwise vertical menu/bar */
#undef RL_TOOLTIPS
//#define RL_TOOLTIPS   1
#define NEW_TOOLTIPS  1
#define RL_BUTTONS    3

#if (RL_PULSE_THEME)
#define RL_COMMANDS      8
#elif (RL_OLDE_THEME)
#define RL_COMMANDS      4
#else
#define RL_PAINT_COMMAND "gimp"
#define RL_COMMANDS      8
#endif
#define RL_SUBWINDOWS    (RL_COMMANDS + 1) /* label */
#if (RL_LABEL)
#define RL_LABEL_WIDTH   RL_BUTTON_WIDTH
#define RL_LABEL_HEIGHT  16
#else
#define RL_LABEL_WIDTH   0
#define RL_LABEL_HEIGHT  0
#endif
#define RL_RULER_WIDTH   5
#define RL_RULER_HEIGHT  5
#if (RL_TOOLTIPS)
//#define RL_TOOLTIP_WIDTH  213
#define RL_TOOLTIP_WIDTH  384
#if (RL_SMALL)
#if (RL_OLDE_THEME)
#define RL_TOOLTIP_HEIGHT 21
#elif (RL_PULSE_THEME)
#define RL_TOOLTIP_HEIGHT 64
#else
#define RL_TOOLTIP_HEIGHT 20
#endif
#else
//#define RL_TOOLTIP_HEIGHT 24
#define RL_TOOLTIP_HEIGHT RL_BUTTON_HEIGHT
#endif
#endif

#if (RL_PULSE_THEME)
#define RL_BUTTON_WIDTH  64
#define RL_BUTTON_HEIGHT 64
#elif (RL_OLDE_THEME)
#if (RL_SMALL)
#define RL_BUTTON_WIDTH  24
#define RL_BUTTON_HEIGHT 21
#else
#define RL_BUTTON_WIDTH  48
#define RL_BUTTON_HEIGHT 43
#endif
#else
#if (RL_SMALL)
#define RL_BUTTON_WIDTH  20
#define RL_BUTTON_HEIGHT 20
#else
#define RL_BUTTON_WIDTH  40
#define RL_BUTTON_HEIGHT 40
#endif
#endif

#define RL_NAVI_BUTTONS 4

#define RL_BUTTON_NORMAL  0
#define RL_BUTTON_ACTIVE  1
#define RL_BUTTON_CLICKED 2
#define RL_BUTTON_STATES  3

#define RL_EXIT            1
#define RL_RESTART         2
#define RL_ACTIONS         255
#define RL_EXIT_COMMAND    ((char *)RL_EXIT)
#define RL_RESTART_COMMAND ((char *)RL_RESTART)
#define RL_MAX_ACTION      ((char *)RL_ACTIONS)

#endif /* __RESURRECTION_LAUNCH_H__ */

