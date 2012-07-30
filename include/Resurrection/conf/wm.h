/*
 * commands launched from apps menu.
 */
#define RWM_TERM_COMMAND   "Rterm"
#define RWM_EDIT_COMMAND   "emacs"
#define RWM_WEB_COMMAND    "firefox"
#define RWM_IMAGE_COMMAND  "Rv"
#define RWM_AUDIO_COMMAND  "deadbeef"
#define RWM_VIDEO_COMMAND  "gnome-mplayer"
#define RWM_MSN_COMMAND    "pidgin"
#define RWM_EXIT_LABEL     "EXIT"

#define RWM_LAUNCH_WIDTH 20

/*
 * number of desktops.
 */
#define RWM_DESKTOPS 4

/*
 * RWM_SLOPPY_FOCUS - client only loses focus when another one gets it.
 */
#define RWM_FOCUS_STYLE (RWM_SLOPPY_FOCUS | RWM_CLICK_FOCUS)

#define RWM_MOVE_STYLE   0
#define RWM_RESIZE_STYLE 0

/*
 * RWM_CLICK_MENU - menus open by clicking labels (otherwise, they open when the
 * pointer enters labels).
 */
#define RWM_MENU_STYLE  (RWM_CLICK_MENU)

/*
 * RWM_DESKTOP_CLOCK - have a clock 'menu' window on the desktop.
 * RWM_FOCUS_CLOCK   - show clock on the title of the focused (active) window.
 * RWM_CLOCK_12      - use 12-hour format instead of the default 24.
 */
#define RWM_CLOCK_STYLE (RWM_DESKTOP_CLOCK)

/*
 * RWM_TEXT_COLOR { a, r, g, b } - the color used in menus etc.
 */
//#define RWM_TEXT_COLOR { 255, 255, 165, 0 }
//#define RWM_TEXT_COLOR { 0xff, 255, 215, 0 }
#define RWM_TEXT_COLOR { 0, 0xd7, 0, 0 }

/* define to zero to use the new (unpolished) theme. */
#define RWM_NEW_THEME      0
#define RWM_ORIGINAL_THEME 1

/* DO NOT TOUCH ANYTHING BELOW THIS. :) */

#define RWM_SLOPPY_FOCUS       (RWM_SLOPPY_FOCUS_FLAG)
#define RWM_SYNCHRONOUS_MOVE   (RWM_SYNCHRONOUS_MOVE_FLAG)
#define RWM_SYNCHRONOUS_RESIZE (RWM_SYNCHRONOUS_RESIZE_FLAG)
#define RWM_CLICK_FOCUS        (RWM_CLICK_FOCUS_FLAG)
#define RWM_CLICK_MENU         (RWM_CLICK_MENU_FLAG)
#define RWM_DESKTOP_CLOCK      (RWM_DESKTOP_CLOCK_FLAG)
#define RWM_FOCUS_CLOCK        (RWM_FOCUS_CLOCK_FLAG)
#define RWM_CLOCK_12           (RWM_CLOCK_12_FLAG)

