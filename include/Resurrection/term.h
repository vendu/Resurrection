/*
 * term.h - Resurrection terminal header.
 * Copyright (C) 2007 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef __RESURRECTION_TERM_H__
#define __RESURRECTION_TERM_H__

#define RTERM_NO_COLOR NULL

#define RTERM_USE_XSHM 0
#define RTERM_CURSOR_HACKS 1

#include <Resurrection/conf/term.h>

#define USE_LINUX_KEYS            0
#define USE_DELETE_KEY            1
#define USE_UNSHIFTED_SCROLL_KEYS 0

#define R_render_background_imlib2(img, win, flg) R_set_background_imlib2(img, win, (img)->w, (img)->h, flg)

#define RTERM_NAME "Rterm"
#define RTERM_VERSION "0.0.18"

#define RTERM_PRINT_PIPE "lp"

//#define RTERM_DEFAULT_TRUETYPE_FONT        "VeraMono"
#define RTERM_DEFAULT_TRUETYPE_FONT        "ProggyCleanSZ"

#define SUPPORT_RTERM_INPUT_NUMERIC        1
#define SUPPORT_XSHM_FONT_BITMAP           0
#define SUPPORT_TRUETYPE_FONTS             0
#define SUPPORT_TRUETYPE_ANTIALIAS         0
#define SUPPORT_RTERM_SCREEN_DOUBLE_BUFFER 1
#define SUPPORT_RTERM_BLINKING_CHARS       1
#define SUPPORT_RTERM_BLINKING_CURSOR      0
#define SUPPORT_RTERM_TABS                 1
#define SUPPORT_XTERM_256_COLORS           1
#define SUPPORT_RTERM_UTF8_CHARS           1
#define RTERM_TABS                         1

#if (SUPPORT_RTERM_UTF8_CHARS)
typedef uint16_t Rterm_wchar_t;
#define RTERM_MAX_UTF8_LEN        6
#endif

#define RTERM_MENU_DEFAULT_HEIGHT 24
#define RTERM_SCROLLBAR_DEFAULT_WIDTH   16

#define RTERM_SCREEN_DEFAULT_ROWS 24
#define RTERM_SCREEN_DEFAULT_COLUMNS 80

#define RTERM_SCREEN_MAX_ROWS    1024
#define RTERM_SCREEN_MAX_COLUMNS 1024

#define NONE   0
#define FALSE  0
#define TRUE   1
#define TOGGLE 2

#if (SUPPORT_TRUETYPE_FONTS)
#if (SUPPORT_TRUETYPE_IMLIB2)
#define RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen)        \
    Rterm_draw_screen_string_imlib2(screen, row, column, stringlen);
#else
#define RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, gc, drawbg) \
    do {                                                                \
        if (drawbg) {                                                   \
            XFillRectangle(screen->window->app->display,                \
                           buffer,                                      \
                           screen->bggc,                                \
                           Rterm_screen_column_x(screen, column),       \
                           Rterm_screen_row_y(screen, row),             \
                           screen->charw * stringlen,                   \
                           screen->charh);                              \
        }                                                               \
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {              \
            Rterm_draw_screen_string_freetype_wc(screen, gc,            \
                                                 buffer, row, column,   \
                                                 stringlen);            \
        } else {                                                        \
            Rterm_draw_screen_string_freetype(screen, gc,               \
                                              buffer, row, column,      \
                                              stringlen);               \
        }                                                               \
    } while (FALSE)
#endif
#else
#if (SUPPORT_RTERM_UTF8_CHARS)
#define RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen, gc, drawbg) \
    do {                                                                \
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {              \
            draw_string(screen->window->app->display,                   \
                        buffer,                                         \
                        gc,                                             \
                        Rterm_screen_column_x(screen, column),          \
                        Rterm_screen_row_y(screen, row) + screen->charasc, \
                        drawstr, stringlen);                            \
        } else {                                                        \
            draw_string(screen->window->app->display,                   \
                        buffer,                                         \
                        gc,                                             \
                        Rterm_screen_column_x(screen, column),          \
                        Rterm_screen_row_y(screen, row) + screen->charasc, \
                        text, stringlen);                               \
        }                                                               \
    } while (FALSE)
#else
#define RTERM_DRAW_SCREEN_STRING(screen, row, column, stringlen)        \
    draw_string(screen->window->app->display,                           \
                buffer,                                                 \
                screen->defaultgc,                                      \
                Rterm_screen_column_x(screen, column),                  \
                Rterm_screen_row_y(screen, row) + screen->charasc,      \
                text, stringlen);                                       \
#endif
#endif
#endif

#if (!SUPPORT_RTERM_UTF8_CHARS)

#define RTERM_WRITE_PTY(term, fd, buf, len)                             \
    R_write_pty(fd, buf, len)

#else

#if (SUPPORT_RTERM_UTF8_CHARS)

#define RTERM_WRITE_PTY(term, fd, buf, len)                             \
    do {                                                                \
        if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {              \
            R_write_pty_utf8(fd, buf, len);                             \
        } else {                                                        \
            R_write_pty(fd, buf,                                        \
                        len);                                           \
        }                                                               \
    } while (FALSE)

#endif
                    
#define RTERM_UTF8_ENC(src, dest, lenp, len)                            \
    do {                                                                \
        unsigned char *__dest = (unsigned char *)(dest);                \
        int __len = 0;                                                  \
                                                                        \
        while ((len--) && (*src)) {                                     \
            Rterm_wchar_t __wc = *(src)++;                              \
                                                                        \
            if (__wc < 0x80) {                                          \
                *__dest++ = (unsigned char)__wc;                        \
                __len++;                                                \
            } else if (__wc < 0x0800) {                                 \
                *__dest++ = (unsigned char)(0xc0 | ((__wc >> 6) & 0x1f)); \
                *__dest++ = (unsigned char)(0x80 | (__wc & 0x3f));      \
                __len += 2;                                             \
            } else if (__wc < 0x10000) {                                \
                *__dest++ = (unsigned char)(0xe0 | ((__wc >> 12) & 0x0f)); \
                *__dest++ = (unsigned char)(0x80 | ((__wc >> 6) & 0x3f)); \
                *__dest++ = (unsigned char)(0x80 | (__wc & 0x3f));      \
                __len += 3;                                             \
            } else {                                                    \
                ; /* perhaps in the future. */                          \
            }                                                           \
        }                                                               \
        *(lenp) = __len;                                                \
    } while (0)

#define RTERM_UTF8_ENCSTR(src, dest, lenp)                              \
    do {                                                                \
        unsigned char *__dest = (unsigned char *)(dest);                \
        int __len = 0;                                                  \
                                                                        \
        while (*src) {                                                  \
            Rterm_wchar_t __wc = *(src)++;                              \
                                                                        \
            if (__wc < 0x80) {                                          \
                *__dest++ = (unsigned char)__wc;                        \
                __len++;                                                \
            } else if (__wc < 0x0800) {                                 \
                *__dest++ = (unsigned char)(0xc0 | ((__wc >> 6) & 0x1f)); \
                *__dest++ = (unsigned char)(0x80 | (__wc & 0x3f));      \
                __len += 2;                                             \
            } else if (__wc < 0x10000) {                                \
                *__dest++ = (unsigned char)(0xe0 | ((__wc >> 12) & 0x0f)); \
                *__dest++ = (unsigned char)(0x80 | ((__wc >> 6) & 0x3f)); \
                *__dest++ = (unsigned char)(0x80 | (__wc & 0x3f));      \
                __len += 3;                                             \
            } else {                                                    \
                ; /* perhaps in the future. */                          \
            }                                                           \
        }                                                               \
        *(lenp) = __len;                                                \
    } while (0)

#endif /* SUPPORT_RTERM_UTF8_CHARS */

#define RTERM_SET_PRIVMODE(t,f,m) \
do { \
    int origstate = (t)->privmodes & (f); \
\
    if ((m) == 't') { \
        if ((t)->privmodes & (f)) { \
            (t)->privmodes &= ~(f); \
        } else { \
            (t)->privmodes |= (f); \
        } \
    } else { \
        if ((m)) { \
            (t)->privmodes |= (f); \
        } else { \
            (t)->privmodes &= ~(f); \
        } \
    } \
\
    if (((t)->privmodes & (f)) == origstate) { \
\
        return; \
    } \
} while (FALSE)

#define FLAG_SET_TRUE(ul,f) ((ul) |= (f))
#define FLAG_SET_FALSE(ul,f) ((ul) &= ~(f))

#define FLAG_SET(ul,f,s) \
do { \
    if ((s) == FALSE) { \
	FLAG_SET_FALSE((ul),(f)); \
    } else if ((s) == TRUE) { \
	FLAG_SET_TRUE((ul),(f)); \
    } else if ((s) == TOGGLE) { \
	if ((ul) & (f)) { \
	    FLAG_SET_FALSE((ul), (f)); \
	} else { \
	    FLAG_SET_TRUE((ul), (f)); \
	} \
    } \
} while (FALSE)

#define RTERM_BACKGROUND_IMAGE           0
#define RTERM_MENU_BACKGROUND_IMAGE      1
#define RTERM_MENUITEM_IMAGE             2
#define RTERM_SUBMENU_IMAGE              3
#define RTERM_BUTTON_BACKGROUND_IMAGE    4
#define RTERM_BUTTONBAR_BACKGROUND_IMAGE 5

/* TODO: image_gbar, image_dialog. */

#define RTERM_IMAGES                     6

#if 0
#define RTERM_SCROLLBAR_IMAGE            1
#define RTERM_SCROLLBAR_ANCHOR_IMAGE     2
#define RTERM_SCROLLBAR_UP_ARROW_IMAGE   3
#define RTERM_SCROLLBAR_DOWN_ARROW_IMAGE 4
#define RTERM_SCROLLBAR_THUMB_IMAGE      5
#endif

#define RTERM_DEFAULT_FONTNAME "fixed"
#define RTERM_DEFAULT_FONT 0
#define RTERM_FONT_1       1
#define RTERM_FONT_2       2
#define RTERM_FONT_3       3
#define RTERM_FONT_4       4
#define RTERM_FONTS        5

#define RTERM_DEFAULT_HEIGHT_ROWS    24
#define RTERM_DEFAULT_WIDTH_COLUMNS  80

/* color indices. */

//#define RTERM_SCREEN_NO_BACKGROUND_COLOR        0
#define RTERM_SCREEN_NO_BACKGROUND_COLOR        0xff

#define RTERM_SCREEN_BLACK_COLOR                0
#define RTERM_SCREEN_RED_COLOR                  1
#define RTERM_SCREEN_GREEN_COLOR                2
#define RTERM_SCREEN_YELLOW_COLOR               3
#define RTERM_SCREEN_BLUE_COLOR                 4
#define RTERM_SCREEN_MAGENTA_COLOR              5
#define RTERM_SCREEN_CYAN_COLOR                 6
#define RTERM_SCREEN_WHITE_COLOR                7

#define RTERM_SCREEN_BRIGHT_BLACK_COLOR         8
#define RTERM_SCREEN_BRIGHT_RED_COLOR           9
#define RTERM_SCREEN_BRIGHT_GREEN_COLOR         10
#define RTERM_SCREEN_BRIGHT_YELLOW_COLOR        11
#define RTERM_SCREEN_BRIGHT_BLUE_COLOR          12
#define RTERM_SCREEN_BRIGHT_MAGENTA_COLOR       13
#define RTERM_SCREEN_BRIGHT_CYAN_COLOR          14
#define RTERM_SCREEN_BRIGHT_WHITE_COLOR         15

#define RTERM_SCREEN_COLORS                     256
#define RTERM_ANSI_COLORS                       16

#define RTERM_SCREEN_DEFAULT_FOREGROUND_COLOR   RTERM_SCREEN_BRIGHT_WHITE_COLOR
#define RTERM_SCREEN_DEFAULT_BACKGROUND_COLOR   RTERM_SCREEN_NO_BACKGROUND_COLOR

#define RTERM_SCREEN_HIGHLIGHT_BACKGROUND_COLOR RTERM_SCREEN_YELLOW_COLOR
#define RTERM_SCREEN_BOLD_COLOR                 RTERM_SCREEN_CYAN_COLOR
#define RTERM_SCREEN_UNDERLINED_COLOR           RTERM_SCREEN_BRIGHT_YELLOW_COLOR

#define RTERM_SCREEN_CURSOR_COLOR               RTERM_SCREEN_GREEN_COLOR
#define RTERM_SCREEN_CURSOR_TEXT_COLOR          RTERM_SCREEN_GREEN_COLOR

#define RTERM_SCREEN_MIN_BRIGHT_COLOR           RTERM_SCREEN_BRIGHT_BLACK_COLOR

#define RTERM_SCREEN_BRIGHT_OFFSET 8

/* default color pixel values. */

#if (SUPPORT_XTERM_256_COLORS)
#define RTERM_COLOR_0 "#000000"
#define RTERM_COLOR_1 "#cd0000"
#define RTERM_COLOR_2 "#00cd00"
#define RTERM_COLOR_3 "#cdcd00"
#define RTERM_COLOR_4 "#0000ee"
#define RTERM_COLOR_5 "#cd00cd"
#define RTERM_COLOR_6 "#00cdcd"
#define RTERM_COLOR_7 "#e5e5e5"
#define RTERM_COLOR_8 "#7f7f7f"
#define RTERM_COLOR_9 "#ff0000"
#define RTERM_COLOR_10 "#00ff00"
#define RTERM_COLOR_11 "#ffff00"
#define RTERM_COLOR_12 "#5c5cff"
#define RTERM_COLOR_13 "#ff00ff"
#define RTERM_COLOR_14 "#00ffff"
#define RTERM_COLOR_15 "#ffffff"
#define RTERM_COLOR_16 "#000000"
#define RTERM_COLOR_17 "#00005f"
#define RTERM_COLOR_18 "#000087"
#define RTERM_COLOR_19 "#0000af"
#define RTERM_COLOR_20 "#0000d7"
#define RTERM_COLOR_21 "#0000ff"
#define RTERM_COLOR_22 "#005f00"
#define RTERM_COLOR_23 "#005f5f"
#define RTERM_COLOR_24 "#005f87"
#define RTERM_COLOR_25 "#005faf"
#define RTERM_COLOR_26 "#005fd7"
#define RTERM_COLOR_27 "#005fff"
#define RTERM_COLOR_28 "#008700"
#define RTERM_COLOR_29 "#00875f"
#define RTERM_COLOR_30 "#008787"
#define RTERM_COLOR_31 "#0087af"
#define RTERM_COLOR_32 "#0087d7"
#define RTERM_COLOR_33 "#0087ff"
#define RTERM_COLOR_34 "#00af00"
#define RTERM_COLOR_35 "#00af5f"
#define RTERM_COLOR_36 "#00af87"
#define RTERM_COLOR_37 "#00afaf"
#define RTERM_COLOR_38 "#00afd7"
#define RTERM_COLOR_39 "#00afff"
#define RTERM_COLOR_40 "#00d700"
#define RTERM_COLOR_41 "#00d75f"
#define RTERM_COLOR_42 "#00d787"
#define RTERM_COLOR_43 "#00d7af"
#define RTERM_COLOR_44 "#00d7d7"
#define RTERM_COLOR_45 "#00d7ff"
#define RTERM_COLOR_46 "#00ff00"
#define RTERM_COLOR_47 "#00ff5f"
#define RTERM_COLOR_48 "#00ff87"
#define RTERM_COLOR_49 "#00ffaf"
#define RTERM_COLOR_50 "#00ffd7"
#define RTERM_COLOR_51 "#00ffff"
#define RTERM_COLOR_52 "#5f0000"
#define RTERM_COLOR_53 "#5f005f"
#define RTERM_COLOR_54 "#5f0087"
#define RTERM_COLOR_55 "#5f00af"
#define RTERM_COLOR_56 "#5f00d7"
#define RTERM_COLOR_57 "#5f00ff"
#define RTERM_COLOR_58 "#5f5f00"
#define RTERM_COLOR_59 "#5f5f5f"
#define RTERM_COLOR_60 "#5f5f87"
#define RTERM_COLOR_61 "#5f5faf"
#define RTERM_COLOR_62 "#5f5fd7"
#define RTERM_COLOR_63 "#5f5fff"
#define RTERM_COLOR_64 "#5f8700"
#define RTERM_COLOR_65 "#5f875f"
#define RTERM_COLOR_66 "#5f8787"
#define RTERM_COLOR_67 "#5f87af"
#define RTERM_COLOR_68 "#5f87d7"
#define RTERM_COLOR_69 "#5f87ff"
#define RTERM_COLOR_70 "#5faf00"
#define RTERM_COLOR_71 "#5faf5f"
#define RTERM_COLOR_72 "#5faf87"
#define RTERM_COLOR_73 "#5fafaf"
#define RTERM_COLOR_74 "#5fafd7"
#define RTERM_COLOR_75 "#5fafff"
#define RTERM_COLOR_76 "#5fd700"
#define RTERM_COLOR_77 "#5fd75f"
#define RTERM_COLOR_78 "#5fd787"
#define RTERM_COLOR_79 "#5fd7af"
#define RTERM_COLOR_80 "#5fd7d7"
#define RTERM_COLOR_81 "#5fffff"
#define RTERM_COLOR_82 "#5fff00"
#define RTERM_COLOR_83 "#5fff5f"
#define RTERM_COLOR_84 "#5fff87"
#define RTERM_COLOR_85 "#5fffaf"
#define RTERM_COLOR_86 "#5fffd7"
#define RTERM_COLOR_87 "#5fffff"
#define RTERM_COLOR_88 "#870000"
#define RTERM_COLOR_89 "#87005f"
#define RTERM_COLOR_90 "#870087"
#define RTERM_COLOR_91 "#8700af"
#define RTERM_COLOR_92 "#8700d7"
#define RTERM_COLOR_93 "#8700ff"
#define RTERM_COLOR_94 "#875f00"
#define RTERM_COLOR_95 "#875f5f"
#define RTERM_COLOR_96 "#875f87"
#define RTERM_COLOR_97 "#875faf"
#define RTERM_COLOR_98 "#875fd7"
#define RTERM_COLOR_99 "#875fff"
#define RTERM_COLOR_100 "#878700"
#define RTERM_COLOR_101 "#87875f"
#define RTERM_COLOR_102 "#878787"
#define RTERM_COLOR_103 "#8787af"
#define RTERM_COLOR_104 "#8787d7"
#define RTERM_COLOR_105 "#8787ff"
#define RTERM_COLOR_106 "#87af00"
#define RTERM_COLOR_107 "#87af5f"
#define RTERM_COLOR_108 "#87af87"
#define RTERM_COLOR_109 "#87afaf"
#define RTERM_COLOR_110 "#87afd7"
#define RTERM_COLOR_111 "#87afff"
#define RTERM_COLOR_112 "#87d700"
#define RTERM_COLOR_113 "#87d75f"
#define RTERM_COLOR_114 "#87d787"
#define RTERM_COLOR_115 "#87d7af"
#define RTERM_COLOR_116 "#87d7d7"
#define RTERM_COLOR_117 "#87d7ff"
#define RTERM_COLOR_118 "#87ff00"
#define RTERM_COLOR_119 "#87ff5f"
#define RTERM_COLOR_120 "#87ff87"
#define RTERM_COLOR_121 "#87ffaf"
#define RTERM_COLOR_122 "#87ffd7"
#define RTERM_COLOR_123 "#87ffff"
#define RTERM_COLOR_124 "#af0000"
#define RTERM_COLOR_125 "#af005f"
#define RTERM_COLOR_126 "#af0087"
#define RTERM_COLOR_127 "#af00af"
#define RTERM_COLOR_128 "#af00d7"
#define RTERM_COLOR_129 "#af00ff"
#define RTERM_COLOR_130 "#af5f00" 
#define RTERM_COLOR_131 "#af5f5f"
#define RTERM_COLOR_132 "#af5f87"
#define RTERM_COLOR_133 "#af5faf"
#define RTERM_COLOR_134 "#af5fd7"
#define RTERM_COLOR_135 "#af5fff"
#define RTERM_COLOR_136 "#af8700"
#define RTERM_COLOR_137 "#af875f"
#define RTERM_COLOR_138 "#af8787"
#define RTERM_COLOR_139 "#af87af"
#define RTERM_COLOR_140 "#af87d7"
#define RTERM_COLOR_141 "#af87ff"
#define RTERM_COLOR_142 "#afaf00"
#define RTERM_COLOR_143 "#afaf5f"
#define RTERM_COLOR_144 "#afaf87"
#define RTERM_COLOR_145 "#afafaf"
#define RTERM_COLOR_146 "#afafd7"
#define RTERM_COLOR_147 "#afafff"
#define RTERM_COLOR_148 "#afd700"
#define RTERM_COLOR_149 "#afd75f"
#define RTERM_COLOR_150 "#afd787"
#define RTERM_COLOR_151 "#afd7af"
#define RTERM_COLOR_152 "#afd7d7"
#define RTERM_COLOR_153 "#afd7ff"
#define RTERM_COLOR_154 "#afff00"
#define RTERM_COLOR_155 "#afff5f"
#define RTERM_COLOR_156 "#afff87"
#define RTERM_COLOR_157 "#afffaf"
#define RTERM_COLOR_158 "#afffd7"
#define RTERM_COLOR_159 "#afffff"
#define RTERM_COLOR_160 "#d70000"
#define RTERM_COLOR_161 "#d7005f"
#define RTERM_COLOR_162 "#d70087"
#define RTERM_COLOR_163 "#d700af"
#define RTERM_COLOR_164 "#d700d7"
#define RTERM_COLOR_165 "#d700ff"
#define RTERM_COLOR_166 "#d75f00"
#define RTERM_COLOR_167 "#d75f5f"
#define RTERM_COLOR_168 "#d75f87"
#define RTERM_COLOR_169 "#d75faf"
#define RTERM_COLOR_170 "#d75fd7"
#define RTERM_COLOR_171 "#d75fff"
#define RTERM_COLOR_172 "#d78700"
#define RTERM_COLOR_173 "#d7875f"
#define RTERM_COLOR_174 "#d78787"
#define RTERM_COLOR_175 "#d787af"
#define RTERM_COLOR_176 "#d787d7"
#define RTERM_COLOR_177 "#d787ff"
#define RTERM_COLOR_178 "#d7af00"
#define RTERM_COLOR_179 "#d7af5f"
#define RTERM_COLOR_180 "#d7af87"
#define RTERM_COLOR_181 "#d7afaf"
#define RTERM_COLOR_182 "#d7afd7"
#define RTERM_COLOR_183 "#d7afff"
#define RTERM_COLOR_184 "#d7d700"
#define RTERM_COLOR_185 "#d7d75f"
#define RTERM_COLOR_186 "#d7d787"
#define RTERM_COLOR_187 "#d7d7af"
#define RTERM_COLOR_188 "#d7d7d7"
#define RTERM_COLOR_189 "#d7d7ff"
#define RTERM_COLOR_190 "#d7ff00"
#define RTERM_COLOR_191 "#d7ff5f"
#define RTERM_COLOR_192 "#d7ff87"
#define RTERM_COLOR_193 "#d7ffaf"
#define RTERM_COLOR_194 "#d7ffd7"
#define RTERM_COLOR_195 "#d7ffff"
#define RTERM_COLOR_196 "#ff0000"
#define RTERM_COLOR_197 "#ff005f"
#define RTERM_COLOR_198 "#ff0087"
#define RTERM_COLOR_199 "#ff00af"
#define RTERM_COLOR_200 "#ff00d7"
#define RTERM_COLOR_201 "#ff00ff"
#define RTERM_COLOR_202 "#ff5f00"
#define RTERM_COLOR_203 "#ff5f5f"
#define RTERM_COLOR_204 "#ff5f87"
#define RTERM_COLOR_205 "#ff5faf"
#define RTERM_COLOR_206 "#ff5fd7"
#define RTERM_COLOR_207 "#ff5fff"
#define RTERM_COLOR_208 "#ff8700"
#define RTERM_COLOR_209 "#ff875f"
#define RTERM_COLOR_210 "#ff8787"
#define RTERM_COLOR_211 "#ff87af"
#define RTERM_COLOR_212 "#ff87d7"
#define RTERM_COLOR_213 "#ff87ff"
#define RTERM_COLOR_214 "#ffaf00"
#define RTERM_COLOR_215 "#ffaf5f"
#define RTERM_COLOR_216 "#ffaf87"
#define RTERM_COLOR_217 "#ffafaf"
#define RTERM_COLOR_218 "#ffafd7"
#define RTERM_COLOR_219 "#ffafff"
#define RTERM_COLOR_220 "#ffd700"
#define RTERM_COLOR_221 "#ffd75f"
#define RTERM_COLOR_222 "#ffd787"
#define RTERM_COLOR_223 "#ffd7af"
#define RTERM_COLOR_224 "#ffd7d7"
#define RTERM_COLOR_225 "#ffd7ff"
#define RTERM_COLOR_226 "#ffff00"
#define RTERM_COLOR_227 "#ffff5f"
#define RTERM_COLOR_228 "#ffff87"
#define RTERM_COLOR_229 "#ffffaf"
#define RTERM_COLOR_230 "#ffffd7"
#define RTERM_COLOR_231 "#ffffff"
/* gray shades */
#define RTERM_COLOR_232 "#000000"
#define RTERM_COLOR_233 "#121212"
#define RTERM_COLOR_234 "#1c1c1c"
#define RTERM_COLOR_235 "#262626"
#define RTERM_COLOR_236 "#303030"
#define RTERM_COLOR_237 "#3a3a3a"
#define RTERM_COLOR_238 "#444444"
#define RTERM_COLOR_239 "#4e4e4e"
#define RTERM_COLOR_240 "#585858"
#define RTERM_COLOR_241 "#626262"
#define RTERM_COLOR_242 "#6c6c6c"
#define RTERM_COLOR_243 "#767676"
#define RTERM_COLOR_244 "#808080"
#define RTERM_COLOR_245 "#8a8a8a"
#define RTERM_COLOR_246 "#949494"
#define RTERM_COLOR_247 "#9e9e9e"
#define RTERM_COLOR_248 "#a8a8a8"
#define RTERM_COLOR_249 "#b2b2b2"
#define RTERM_COLOR_250 "#bcbcbc"
#define RTERM_COLOR_251 "#c6c6c6"
#define RTERM_COLOR_252 "#d0d0d0"
#define RTERM_COLOR_253 "#dadada"
#define RTERM_COLOR_254 "#e4e4e4"
#define RTERM_COLOR_255 "#eeeeee"
#endif

#define RTERM_SCREEN_DEFAULT_BLACK          "#000000"
#define RTERM_SCREEN_DEFAULT_RED            "#aa0000"
#define RTERM_SCREEN_DEFAULT_GREEN          "#008800"
#define RTERM_SCREEN_DEFAULT_YELLOW         "#aa5522"
#define RTERM_SCREEN_DEFAULT_BLUE           "#0000aa"
#define RTERM_SCREEN_DEFAULT_MAGENTA        "#aa00aa"
#define RTERM_SCREEN_DEFAULT_CYAN           "#00aaaa"
#define RTERM_SCREEN_DEFAULT_WHITE          "#aaaaaa"

#define RTERM_SCREEN_DEFAULT_BRIGHT_BLACK   "#808080"
#define RTERM_SCREEN_DEFAULT_BRIGHT_RED     "#ff8080"
#define RTERM_SCREEN_DEFAULT_BRIGHT_GREEN   "#80ff80"
#define RTERM_SCREEN_DEFAULT_BRIGHT_YELLOW  "#ffff80"
#define RTERM_SCREEN_DEFAULT_BRIGHT_BLUE    "#8080ff"
#define RTERM_SCREEN_DEFAULT_BRIGHT_MAGENTA "#ff80ff"
#define RTERM_SCREEN_DEFAULT_BRIGHT_CYAN    "#80ffff"
#define RTERM_SCREEN_DEFAULT_BRIGHT_WHITE   "#ffffff"
#if 0
#define RTERM_SCREEN_DEFAULT_BRIGHT_BLACK   "#444444"
#define RTERM_SCREEN_DEFAULT_BRIGHT_RED     "#ff4444"
#define RTERM_SCREEN_DEFAULT_BRIGHT_GREEN   "#44ff44"
#define RTERM_SCREEN_DEFAULT_BRIGHT_YELLOW  "#ffff44"
#define RTERM_SCREEN_DEFAULT_BRIGHT_BLUE    "#4444ff"
#define RTERM_SCREEN_DEFAULT_BRIGHT_MAGENTA "#ff44ff"
#define RTERM_SCREEN_DEFAULT_BRIGHT_CYAN    "#44ffff"
#define RTERM_SCREEN_DEFAULT_BRIGHT_WHITE   "#ffffff"
#endif

#if 0
#define RTERM_SCREEN_DEFAULT_BLACK          "black"
#define RTERM_SCREEN_DEFAULT_RED            "dark red"
#define RTERM_SCREEN_DEFAULT_GREEN          "dark green"
//#define RTERM_SCREEN_DEFAULT_YELLOW         "yellow3"
#define RTERM_SCREEN_DEFAULT_YELLOW         "yellow"
//#define RTERM_SCREEN_DEFAULT_BLUE           "dark blue"
#define RTERM_SCREEN_DEFAULT_BLUE           "navy blue"
#define RTERM_SCREEN_DEFAULT_MAGENTA        "dark magenta"
#define RTERM_SCREEN_DEFAULT_CYAN           "dark cyan"
#define RTERM_SCREEN_DEFAULT_WHITE          "gray"
#endif

#if 0
#define RTERM_SCREEN_DEFAULT_BRIGHT_BLACK   "light slate gray"
#define RTERM_SCREEN_DEFAULT_BRIGHT_RED     "red"
//#define RTERM_SCREEN_DEFAULT_BRIGHT_GREEN   "light green"
#define RTERM_SCREEN_DEFAULT_BRIGHT_GREEN   "green"
#define RTERM_SCREEN_DEFAULT_BRIGHT_YELLOW  "light yellow"
//#define RTERM_SCREEN_DEFAULT_BRIGHT_BLUE    "light blue"
#define RTERM_SCREEN_DEFAULT_BRIGHT_BLUE    "light slate blue"
#define RTERM_SCREEN_DEFAULT_BRIGHT_MAGENTA "magenta"
//#define RTERM_SCREEN_DEFAULT_BRIGHT_CYAN    "light cyan"
#define RTERM_SCREEN_DEFAULT_BRIGHT_CYAN    "cyan"
#define RTERM_SCREEN_DEFAULT_BRIGHT_WHITE   "white"
#endif

#if 0
#define RTERM_SCREEN_DEFAULT_BRIGHT_BLACK   "dark slate gray"
#define RTERM_SCREEN_DEFAULT_BRIGHT_RED     "red"
#define RTERM_SCREEN_DEFAULT_BRIGHT_GREEN   "green"
#define RTERM_SCREEN_DEFAULT_BRIGHT_YELLOW  "yellow"
#define RTERM_SCREEN_DEFAULT_BRIGHT_BLUE    "blue"
#define RTERM_SCREEN_DEFAULT_BRIGHT_MAGENTA "magenta"
#define RTERM_SCREEN_DEFAULT_BRIGHT_CYAN    "cyan"
#define RTERM_SCREEN_DEFAULT_BRIGHT_WHITE   "white"
#endif

#define RTERM_SCREEN_DEFAULT_MENU                RTERM_SCREEN_DEFAULT_BLACK
#define RTERM_SCREEN_DEFAULT_MENU_TEXT           RTERM_SCREEN_DEFAULT_WHITE

#define RTERM_SCREEN_DEFAULT_SCROLLBAR           RTERM_SCREEN_DEFAULT_GREEN
#define RTERM_SCREEN_DEFAULT_UNFOCUSED_SCROLLBAR RTERM_SCREEN_DEFAULT_BLUE
#define RTERM_SCREEN_DEFAULT_POINTER             RTERM_SCREEN_DEFAULT_MAGENTA

#define RTERM_SCREEN_DEFAULT_CURSOR              RTERM_SCREEN_DEFAULT_GREEN
#define RTERM_SCREEN_DEFAULT_CURSOR_TEXT         RTERM_SCREEN_DEFAULT_YELLOW

#define RTERM_SCREEN_DEFAULT_FOREGROUND RTERM_SCREEN_DEFAULT_BRIGHT_WHITE
#define RTERM_SCREEN_DEFAULT_BACKGROUND RTERM_SCREEN_DEFAULT_BLACK

#define RTERM_SCREEN_FOREGROUND_COLOR       0
#define RTERM_SCREEN_BACKGROUND_COLOR       1
#define RTERM_SCREEN_DEFAULT_COLORS         2

#define RTERM_USERLOG_UTMPID_LENGTH 5
#define RTERM_USERLOG_UTMPLINE_LENGTH 32

#define RTERM_MAX_ESCAPE_ARGS 32

#define RTERM_ESCAPE_HANDLER_ARRAY_SIZE (UCHAR_MAX + 1)
#define RTERM_CSI_HANDLER_ARRAY_SIZE    (UCHAR_MAX + 1)
#define RTERM_SGR_HANDLER_ARRAY_SIZE    (UCHAR_MAX + 1)
#define RTERM_PRIVATE_MODE_HANDLER_ARRAY_SIZE 1024
#define RTERM_ETERM_ESCAPE_HANDLER_ARRAY_SIZE 128

#define R_ENQ_CHAR             '\005'
#define R_BEL_CHAR             '\a'
#define R_BACKSPACE_CHAR       '\b'
#define R_VERTICAL_TAB_CHAR    '\v'
#define R_FORM_FEED_CHAR       '\f'
#define R_CARRIAGE_RETURN_CHAR '\r'
#define R_SO_CHAR              '\016'
#define R_SI_CHAR              '\017'
#define R_ESC_CHAR             '\033'
#define R_DEL_CHAR             '\177'

#define RTERM_PRIMARY_SCREEN   0
#define RTERM_SECONDARY_SCREEN 1
#if (SUPPORT_RTERM_TABS)
#define RTERM_SCREENS          (2 * RTERM_TABS)
#else
#define RTERM_SCREENS          2
#endif

#define RTERM_SELECT_TIMEOUT_USEC                   100000
#define RTERM_SELECT_SCROLL_TIMEOUT_USEC            100000
#define RTERM_SELECT_SCROLLBAR_PRESSED_TIMEOUT_USEC 100000
#define RTERM_SELECT_BLINK_TIMEOUT_USEC             100000

#define RTERM_SCREEN_CLEAR_BUFFER 0
#define RTERM_SCREEN_CLEAR_STRING 1
#define RTERM_SCREEN_SYNC         0
#define RTERM_SCREEN_SYNC_STRING  1

#define RTERM_REFRESH_THRESHOLD_SCREENS 8

#define RTERM_ICONIC                  0x00000001U
#define RTERM_SCALE_BACKGROUND_PIXMAP 0x00000002U
#define RTERM_TRANSPARENT             0x00000004U
#define RTERM_LOGIN_SHELL             0x00000008U
#define RTERM_UTMP_LOGGING            0x00000010U
#define RTERM_SCROLLBAR_RIGHT         0x00000020U
#define RTERM_FLOATING_SCROLLBAR      0x00000040U
#define RTERM_POPUP_SCROLLBAR         0x00000080U
#define RTERM_BORDERLESS              0x00000100U
#define RTERM_VISUAL_BELL             0x00000200U
#define RTERM_MAP_ALERT               0x00000400U
#define RTERM_META_8                  0x00000800U
#define RTERM_SAVE_UNDER              0x00001000U
#define RTERM_PAUSE_ON_EXIT           0x00002000U
#define RTERM_XTERM_SELECT            0x00004000U
#define RTERM_SELECT_LINE             0x00008000U
#define RTERM_VIEWPORT_MODE           0x00010000U
#define RTERM_CONSOLE                 0x00020000U
#define RTERM_SELECT_TRAILING_SPACES  0x00040000U
#define RTERM_REPORT_AS_KEYSYMS       0x00080000U
#define RTERM_INSTALL_COLORMAP        0x00100000U
#define RTERM_DOUBLE_BUFFER           0x00200000U
#define RTERM_BLINKING_CURSOR         0x00400000U
#define RTERM_BLINKING_CHARS          0x00800000U
#define RTERM_CONFIRM_DESTROY         0x01000000U
#define RTERM_PLASMA_BACKGROUND       0x02000000U
#define RTERM_UNMAP_LEAVE             0x04000000U
#define RTERM_UNMAP_FOCUS             0x08000000U
#define RTERM_START_UNMAPPED          0x10000000U
#define RTERM_ENTER_RESIZE            0x20000000U
#define RTERM_ENTER_FOCUS             0x40000000U

#define RTERM_DEFAULT_FLAGS (RTERM_LOGIN_SHELL \
                             | RTERM_UTMP_LOGGING | RTERM_FLOATING_SCROLLBAR \
                             | RTERM_CONFIRM_DESTROY)
#if 0
#define RTERM_DEFAULT_FLAGS (RTERM_LOGIN_SHELL \
			    | RTERM_UTMP_LOGGING | RTERM_FLOATING_SCROLLBAR \
			    | RTERM_CONFIRM_DESTROY \
                            | RTERM_PLASMA_BACKGROUND)
#endif

#define RTERM_PRIVMODE_INSERT                  0x00000001U
#define RTERM_PRIVMODE_APPLICATION_CURSOR_KEYS 0x00000002U
#define RTERM_PRIVMODE_132                     0x00000004U
#define RTERM_PRIVMODE_US_ASCII_CHARSETS       0x00000008U
#define RTERM_PRIVMODE_SMOOTH_SCROLL           0x00000010U
#define RTERM_PRIVMODE_REVERSE_VIDEO           0x00000020U
#define RTERM_PRIVMODE_RELATIVE_ORIGIN         0x00000040U
#define RTERM_PRIVMODE_AUTOWRAP                0x00000080U
#define RTERM_PRIVMODE_AUTO_REPEAT_KEYS        0x00000100U
#define RTERM_PRIVMODE_START_BLINKING_CURSOR   0x00000200U
#define RTERM_PRIVMODE_MOUSE_X10               0x00000400U
#define RTERM_PRIVMODE_MENUBAR                 0x00000800U
#define RTERM_PRIVMODE_VISIBLE_CURSOR          0x00001000U
#define RTERM_PRIVMODE_SCROLLBAR               0x00002000U
#define RTERM_PRIVMODE_SHIFT_SEQUENCES         0x00004000U
#define RTERM_PRIVMODE_TEKTRONIX               0x00008000U
#define RTERM_PRIVMODE_ALLOW_132               0x00010000U
#define RTERM_PRIVMODE_MORE_FIX                0x00020000U
#define RTERM_PRIVMODE_MARGIN_BELL             0x00040000U
#define RTERM_PRIVMODE_REVERSE_AUTOWRAP        0x00080000U
#define RTERM_PRIVMODE_LOG                     0x00100000U
#define RTERM_PRIVMODE_SECONDARY_SCREEN        0x00200000U
#define RTERM_PRIVMODE_APPLICATION_KEYPAD      0x00400000U
#define RTERM_PRIVMODE_BACKSPACE               0x00800000U
#define RTERM_PRIVMODE_MOUSE_X11               0x01000000U
#define RTERM_PRIVMODE_HIGHLIGHT_MOUSE_TRACK   0x02000000U
#define RTERM_PRIVMODE_HOME_ON_ECHO            0x04000000U
#define RTERM_PRIVMODE_HOME_ON_REFRESH         0x08000000U
#define RTERM_PRIVMODE_HOME_ON_INPUT           0x10000000U
#define RTERM_PRIVMODE_UTF8_CHARS              0x20000000U

#define RTERM_PRIVMODE_MOUSE_REPORT            (RTERM_PRIVMODE_MOUSE_X10 | RTERM_PRIVMODE_MOUSE_X11 | RTERM_PRIVMODE_SMOOTH_SCROLL)

#define RTERM_DEFAULT_PRIVMODES (RTERM_PRIVMODE_AUTOWRAP | RTERM_PRIVMODE_VISIBLE_CURSOR | RTERM_PRIVMODE_SHIFT_SEQUENCES | RTERM_PRIVMODE_BACKSPACE)

#if 0
#define RTERM_DEFAULT_PRIVMODES (RTERM_PRIVMODE_AUTOWRAP | RTERM_PRIVMODE_MENUBAR | RTERM_PRIVMODE_VISIBLE_CURSOR | RTERM_PRIVMODE_SHIFT_SEQUENCES | RTERM_PRIVMODE_BACKSPACE)
#define RTERM_DEFAULT_PRIVMODES (RTERM_PRIVMODE_AUTOWRAP | RTERM_PRIVMODE_VISIBLE_CURSOR | RTERM_PRIVMODE_SCROLLBAR | RTERM_PRIVMODE_SHIFT_SEQUENCES | RTERM_PRIVMODE_BACKSPACE)
#endif

#define RTERM_PRIVMODE_APPLICATION_CURSOR_KEYS_STRING "\033[?1t"
#define RTERM_PRIVMODE_USASCII_CHARSETS_STRING        "\033[?2t"
#define RTERM_PRIVMODE_132_STRING                     "\033[?3t"
#define RTERM_PRIVMODE_SMOOTH_SCROLL_STRING           "\033[?4t"
#define RTERM_PRIVMODE_REVERSE_VIDEO_STRING           "\033[?5t"
#define RTERM_PRIVMODE_RELATIVE_ORIGIN_STRING         "\033[?6t"
#define RTERM_PRIVMODE_AUTOWRAP_STRING                "\033[?7t"
#define RTERM_PRIVMODE_AUTO_REPEAT_KEYS_STRING        "\033[?8t"
#define RTERM_PRIVMODE_MOUSE_X10_STRING               "\033[?9t"
#define RTERM_PRIVMODE_MENUBAR_STRING                 "\033[?10t"
#define RTERM_PRIVMODE_VISIBLE_CURSOR_STRING          "\033[?25t"
#define RTERM_PRIVMODE_SCROLLBAR_STRING               "\033[?30t"
#define RTERM_PRIVMODE_SHIFT_SEQUENCES_STRING         "\033[?35t"
#define RTERM_PRIVMODE_TEKTRONIX_STRING               "\033[?38t"
#define RTERM_PRIVMODE_ALLOW_132_STRING               "\033[?40t"
#define RTERM_PRIVMODE_MORE_FIX_STRING                "\033[?41t"
#define RTERM_PRIVMODE_MARGIN_BELL_STRING             "\033[?44t"
#define RTERM_PRIVMODE_REVERSE_AUTOWRAP_STRING        "\033[?45t"
#define RTERM_PRIVMODE_LOG_STRING                     "\033[?46t"
#define RTERM_PRIVMODE_SCREEN_STRING                  "\033[?47t"
#define RTERM_PRIVMODE_APPLICATION_KEYPAD_STRING      "\033[?66t"
#define RTERM_PRIVMODE_BACKSPACE_STRING               "\033[?67t"
#define RTERM_PRIVMODE_MOUSE_X11_STRING               "\033[?1000t"
#define RTERM_PRIVMODE_HIGHLIGHT_MOUSE_TRACK_STRING   "\033[?1001t"
#define RTERM_PRIVMODE_HOME_ON_ECHO_STRING            "\033[?1010t"
#define RTERM_PRIVMODE_HOME_ON_REFRESH_STRING         "\033[?1011t"
#define RTERM_PRIVMODE_HOME_ON_INPUT_STRING           "\033[?1012t"

#define RTERM_SCREEN_WRAP_NEXT_CHAR        0x00000001U
#define RTERM_SCREEN_HAS_SCROLLBACK_BUFFER 0x00000008U
#define RTERM_SCREEN_HAS_BLINKING_CURSOR   0x00000010U
#define RTERM_SCREEN_HAS_BLINKING_CHARS    0x00000020U
#define RTERM_SCREEN_REVERSE_VIDEO         0x00000040U
#define RTERM_SCREEN_RELATIVE_ORIGIN       0x00000080U
#define RTERM_SCREEN_VISIBLE_CURSOR        0x00000200U
#define RTERM_SCREEN_ACTIVE                0x00000400U
//#define RTERM_SCREEN_INSERT                0x00000002U
//#define RTERM_SCREEN_HAS_FOCUS             0x00000004U
//#define RTERM_SCREEN_AUTOWRAP              0x00000100U
//#define RTERM_SCREEN_SCROLLBAR             0x00000400U
//#define RTERM_SCREEN_SCROLLBAR_REFRESHED   0x00000800U

#define RTERM_SCREEN_COLUMN_RELATIVE       0x00000001U
#define RTERM_SCREEN_ROW_RELATIVE          0x00000002U
#define RTERM_SCREEN_RELATIVE              (RTERM_SCREEN_COLUMN_RELATIVE | RTERM_SCREEN_ROW_RELATIVE)

#define RTERM_SCREEN_DRAW_ALL           0
#define RTERM_SCREEN_DRAW_BLINKING_ONLY 1
#define RTERM_SCREEN_DRAW_MODES         2

#define RTERM_SCREEN_ERASE_TO_END       0
#define RTERM_SCREEN_ERASE_TO_BEGINNING 1
#define RTERM_SCREEN_ERASE_ALL          2

/* the low 10 bits are used for foreground and background colors,
 * 5 for each to get 32 colors for each.
 * in 256-color mode, the low 16 bits are used.
 */
#if (1)

#define RTERM_CHAR_FG_COLOR(tf) ((tf) & RTERM_CHAR_FG_COLOR_MASK)
#define RTERM_CHAR_BG_COLOR(tf) (((tf) & RTERM_CHAR_BG_COLOR_MASK) >> RTERM_CHAR_BG_COLOR_SHIFT)

#define RTERM_SET_CHAR_FG_COLOR(tf,c) (tf) = (((tf) & ~RTERM_CHAR_FG_COLOR_MASK) | (c))
#define RTERM_SET_CHAR_BG_COLOR(tf,c) (tf) = (((tf) & ~(RTERM_CHAR_NO_BACKGROUND | RTERM_CHAR_BG_COLOR_MASK)) | ((c) << RTERM_CHAR_BG_COLOR_SHIFT))

#define RTERM_CHAR_FG_COLOR_MASK     0x000000ffU
#define RTERM_CHAR_BG_COLOR_MASK     0x0000ff00U
#define RTERM_CHAR_BG_COLOR_SHIFT    8

//#define RTERM_SCREEN_DEFAULT_TEXTFLAGS 0x00000000U
#define RTERM_SCREEN_DEFAULT_TEXTFLAGS RTERM_CHAR_NO_BACKGROUND

#define RTERM_CHAR_SELECTED          0x00010000U /* part of selection. */
#define RTERM_CHAR_REVERSE           0x00020000U /* reverse video. */
#define RTERM_CHAR_UNDERLINED        0x00040000U /* underlined. */
#define RTERM_CHAR_BOLD              0x00080000U /* bold. */
#define RTERM_CHAR_BLINKING          0x00100000U /* blinking. */

#define RTERM_CHAR_REDRAW            0x00200000U /* force redrawing. */
#define RTERM_CHAR_DUALBYTE_1        0x00400000U /* 16-bit character, 1st byte. */
#define RTERM_CHAR_DUALBYTE_2        0x00800000U /* 16-bit character, 2nd byte. */
#define RTERM_CHAR_ACS_FONT          0x01000000U /* ACS graphics character set. */
#define RTERM_CHAR_UK_FONT           0x02000000U /* UK character set */
#define RTERM_CHAR_CURSOR            0x04000000U
#define RTERM_CHAR_NO_BACKGROUND     0x08000000U

#else
#define RTERM_CHAR_FG_COLOR(tf) ((tf) & RTERM_CHAR_FG_COLOR_MASK)
#define RTERM_CHAR_BG_COLOR(tf) (((tf) & RTERM_CHAR_BG_COLOR_MASK) >> RTERM_CHAR_BG_COLOR_SHIFT)

#define RTERM_SET_CHAR_FG_COLOR(tf,c) (tf) = (((tf) & ~RTERM_CHAR_FG_COLOR_MASK) | (c))
#define RTERM_SET_CHAR_BG_COLOR(tf,c) (tf) = (((tf) & ~(RTERM_CHAR_NO_BACKGROUND | RTERM_CHAR_BG_COLOR_MASK) | ((c) << RTERM_CHAR_BG_COLOR_SHIFT))

#define RTERM_CHAR_FG_COLOR_MASK     0x0000001fU
#define RTERM_CHAR_BG_COLOR_MASK     0x000003e0U
#define RTERM_CHAR_BG_COLOR_SHIFT    5

#define RTERM_SCREEN_DEFAULT_TEXTFLAGS 0x00000000U

#define RTERM_CHAR_SELECTED          0x00000400U /* part of selection. */
#define RTERM_CHAR_REVERSE           0x00000800U /* reverse video. */
#define RTERM_CHAR_UNDERLINED        0x00001000U /* underlined. */
#define RTERM_CHAR_BOLD              0x00002000U /* bold. */
#define RTERM_CHAR_BLINKING          0x00004000U /* blinking. */

#define RTERM_CHAR_REDRAW            0x00008000U /* force redrawing. */
#define RTERM_CHAR_DUALBYTE_1        0x00010000U /* 16-bit character, 1st byte. */
#define RTERM_CHAR_DUALBYTE_2        0x00020000U /* 16-bit character, 2nd byte. */
#define RTERM_CHAR_ACS_FONT          0x00040000U /* ACS graphics character set. */
#define RTERM_CHAR_UK_FONT           0x00080000U /* UK character set */
#endif

#define RTERM_CHAR_DUALBYTE_MASK     (RTERM_CHAR_DUALBYTE_1 | RTERM_CHAR_DUALBYTE_2)
#define RTERM_CHAR_FONT_MASK         (RTERM_CHAR_ACS_FONT | RTERM_CHAR_UK_FONT)

#define RTERM_SCROLLBAR_DEFAULT_WIDTH        16
#define RTERM_SCROLLBAR_DEFAULT_ARROW_WIDTH  RTERM_SCROLLBAR_DEFAULT_WIDTH
#define RTERM_SCROLLBAR_DEFAULT_ARROW_HEIGHT 16

#define RTERM_SCROLLBAR_INITIALIZED        0x00000001U
#define RTERM_SCROLLBAR_REFRESHED          0x00000002U
#define RTERM_SCROLLBAR_UP_ARROW_PRESSED   0x00000004U
#define RTERM_SCROLLBAR_DOWN_ARROW_PRESSED 0x00000008U
#define RTERM_SCROLLBAR_ANCHOR_PRESSED     0x00000010U
#define RTERM_SCROLLBAR_PRESSED            0x00000020U
#define RTERM_SCROLLBAR_PRESSED_MASK       \
    (RTERM_SCROLLBAR_UP_ARROW_PRESSED \
     | RTERM_SCROLLBAR_DOWN_ARROW_PRESSED \
     | RTERM_SCROLLBAR_ANCHOR_PRESSED \
     | RTERM_SCROLLBAR_PRESSED)

#define RTERM_SCROLLBAR_TROUGH_IMAGE     0
#define RTERM_SCROLLBAR_ANCHOR_IMAGE     1
#define RTERM_SCROLLBAR_UP_ARROW_IMAGE   2
#define RTERM_SCROLLBAR_DOWN_ARROW_IMAGE 3
#define RTERM_SCROLLBAR_IMAGES           4

/* TODO: menubar, pixmap, and Eterm extension sequences. */

/* RTERM_ESC_REPLY_LENGTH must be at least (PATH_MAX + 5). */

#define RTERM_ESC_REPLY_LENGTH (PATH_MAX + 5)

#define RTERM_XTERM_SEQUENCE_STRING_LENGTH 512

/* NOTE: TAB is used as short for tabulator, CHAR for character. */

/* escape sequences. */

/* sequences prefixed with ESC (Ctrl-[). */

/* escape sequence characters. */
#define RTERM_VISIBLE              28

#define RTERM_CHARSET_KANJI_ESCAPE '$'
#define RTERM_CHARSET_G0_ESCAPE	 '('
#define RTERM_CHARSET_G1_ESCAPE	 ')'
#define RTERM_CHARSET_G2_ESCAPE	 '*'
#define RTERM_CHARSET_G3_ESCAPE	 '+'

/* setting characters for charsets. */

#define RTERM_CHARSET_DEC_LINE	 '0'
#define RTERM_CHARSET_FINNISH    '5' /* currently not implemented */
#define RTERM_CHARSET_MULTINATIONAL '<' /* currently not implemented */
#define RTERM_CHARSET_UK	 'A'
#define RTERM_CHARSET_US	 'B' /* currently not implemented */
#define RTERM_CHARSET_FINNISH2   'C' /* currently not implemented */
#define RTERM_CHARSET_UTF8       'G' /* currently not implemented */
#define RTERM_CHARSET_GERMAN     'K' /* currently not implemented */

#define RTERM_SAVE_CURSOR	 '7' /* SC. */
#define RTERM_RESTORE_CURSOR	 '8' /* RC. */

#define RTERM_APPLICATION_KEYPAD	 '=' /* SMKX. */
#define RTERM_NORMAL_KEYPAD	 '>' /* RMKX. */
#define RTERM_INDEX		 'D' /* IND. */
#define RTERM_NEXT_LINE		 'E' /* NEL. */

#define RTERM_TAB_SET		 'H' /* HTS. */
#define RTERM_REVERSE_INDEX	 'M' /* RI. */

#if 0
/* TODO: from xterm. */
#define RTERM_SINGLE_SHIFT_OR_G2    'N'
#define RTERM_SINGLE_SHIFT_OR_G3    'O'
#endif

#define RTERM_OBSOLETE_ATTRIBUTES	 'Z' /* DA, obsoleted by "ESC[c". */

#define RTERM_CSI_SEQUENCE          '['
#define RTERM_XTERM_SEQUENCE        ']'

#define RTERM_FULL_RESET		 'c' /* RIS. */

#if 0
/* TODO: from xterm. */
#define RTERM_XTERM_MEMORY_LOCK        'l'
#define RTERM_XTERM_MEMORY_UNLOCK      'm'
#endif

#define RTERM_G2_CHARSET		 'n' /* LS2. */
#define RTERM_G3_CHARSET		 'o' /* LS3. */

#if 0
/* TODO: from xterm. */
#define RTERM_GR_G3_CHARSET         '|' /* LS3R. */
#define RTERM_GR_G2_CHARSET         '}' /* LS2R. */
#define RTERM_GR_G1_CHARSET         '~' /* LS1R. */
#endif

/* sequences from Eterm sources. Prefixed with ESC[? */

#define RTERM_SAVE_CURSOR_2         's'
#define RTERM_RESTORE_CURSOR_2      'u'

/* sequences prefixed with ESC[n. */

#define RTERM_INSERT_BLANKS	    '@' /* ICH, default 1. */

#define RTERM_CURSOR_UP		    'A' /* CUU, default 1. */
#define RTERM_CURSOR_DOWN		    'B' /* CUD, default 1. */
#define RTERM_CURSOR_FORWARD	    'C' /* CUF, default 1. */
#define RTERM_CURSOR_BACKWARDS	    'D' /* CUB, default 1. */

#define RTERM_CURSOR_DOWN_AND_COLUMN_0 'E' /* default 1. */
#define RTERM_CURSOR_UP_AND_COLUMN_0   'F' /* default 1. */
#define RTERM_CURSOR_TO_COLUMN	    'G' /* HPA. */

#define RTERM_FORWARD_TAB_STOPS	    'I' /* default 1. */
#define RTERM_ERASE_IN_DISPLAY	    'J' /* ED: 0 below, 1 above, 2 all. */
#define RTERM_ERASE_IN_LINE	    'K' /* EL: 0 - to right
					 *     1 - to left,
					 *     2 - all. */

#define RTERM_INSERT_LINES	    'L' /* IL, default 1. */
#define RTERM_DELETE_LINES	    'M' /* DL, default 1. */
#define RTERM_DELETE_CHARS	    'P' /* DCH, default 1. */

#define RTERM_SCROLL_UP             'S'
#define RTERM_SCROLL_DOWN           'T'

#define RTERM_TAB_FUNCTIONS	    'W' /* HTS: 0 - set
					 * TBC: 2 - clear current column
					 * TBC: 5 - clear all. */

/* values for RTERM_TAB_FUNCTIONS */

#define RTERM_TAB_FUNCTION_SET          0
#define RTERM_TAB_FUNCTION_CLEAR        2
#define RTERM_TAB_FUNCTION_CLEAR_ALL    5

#define RTERM_ERASE_CHARS  	    'X' /* ECH, same as 'P', default 1. */

#define RTERM_BACKWARDS_TAB_STOPS      'Z' /* default 1. */

#define RTERM_MOVE_TO_COLUMN	    '`' /* same as 'G'. */
#define RTERM_MOVE_FORWARD	            'a' /* same as 'C'. */

#define RTERM_DEVICE_ATTRIBUTES	    'c' /* DA, the default of 0 returns
					 * "ESC[?1;2c" indicating VT100
					 * with advanced video option. */

#define RTERM_CURSOR_TO_ROW	    'd' /* VPA. */

#define RTERM_MOVE_UP		    'e' /* same as 'A'. */

#define RTERM_TAB_CLEAR		    'g' /* TBC: 0 - clear current column.
					 * TBC: 3 - clear all. */

/* values for RTERM_TAB_CLEAR */

#define RTERM_SCREEN_CLEAR_TAB                 0
#define RTERM_SCREEN_CLEAR_ALL_TABS            3

#define RTERM_PRINT_MODE		    'i' /* 0 - print visible rows.
					 * 4 - disable transparent, MC4.
					 * 5 - enable transparent, MC5. */

#define RTERM_PRINT_VISIBLE_ROWS        0
#define RTERM_PRINT_DISABLE_TRANSPARENT 4
#define RTERM_PRINT_ENABLE_TRANSPARENT  5

/* values for RTERM_PRINT_MODE. */

#define RTERM_SCREEN_PRINT_VISIBLE_ROWS        0
#define RTERM_PRINT_DISABLE_TRANSPARENT 4
#define RTERM_PRINT_ENABLE_TRANSPARENT  5

/*
 * arguments and replies for RTERM_DEVICE_STATUS:
 * 5 - status report - returns "ESC[0n" ("OK").
 * 6 - report cursor position (CPR) - returns "ESC[r;cR" (row;column).
 * 7 - request display name - ignored by default for security reasons.
 * 8 - request version number in window title.
 */

#define RTERM_DEVICE_STATUS 'n' /* DSR */

#define RTERM_STATUS_STRING "\033[0n"

#define RTERM_STATUS_REPORT               5
#define RTERM_REPORT_CURSOR_POSITION      6
#define RTERM_XTERM_REQUEST_DISPLAY_NAME     7
#define RTERM_XTERM_REQUEST_VERSION_IN_TITLE 8
#define RTERM_ETERM_IMAGE_ATTRIBUTES         9 /* currently not implemented */

/* sequences prefixed with ESC[n;n. */

#define RTERM_CURSOR_POSITION    'H' /* CUP, row;column,
				      * default 1;1. */
#define RTERM_MOVE_TO	      'f' /* HVP, same as 'H'. */
#define RTERM_SET_SCROLL_REGION  'r' /* CSR, top;bottom row,
				   * default full screen. */
#define RTERM_REQUEST_PARAMETERS 'x' /* DECREQTPARM. */

/* sequences with varying number of arguments - ESC[n;n... */

/*
 * arguments for RTERM_SET_MODES, RTERM_SET_MODE and RTERM_RESET_MODE:
 *  4 - insert mode / replace mode.
 * 20 - automatic newline / normal linefeed.
 */

#define RTERM_SET_MODES  '\0'         
#define RTERM_SET_MODE   'h' /* SM. */
#define RTERM_RESET_MODE 'l' /* RM. */

#define RTERM_INSERT_MODE       4
#define RTERM_NEWLINE_MODE      20

/*
 * arguments for RTERM_SGR_ATTRIBUTES:
 *     0 - normal (default).
 *  1/22 - turn bold (bright foreground) on/off.
 *  4/24 - turn underline on/off.
 *  5/25 - turn "blink" (bright bg) on/off.
 *  7/27 - turn inverse on/off.
 * 30/40 - foreground/background black.
 * 31/41 - foreground/background red.
 * 32/42 - foreground/background green.
 * 33/43 - foreground/background yellow.
 * 34/44 - foreground/background blue.
 * 35/45 - foreground/background magenta.
 * 36/46 - foreground/background cyan.
 * 37/47 - foreground/background white.
 * 39/49 - foreground/background default.
 */

#define RTERM_SGR_ATTRIBUTES 'm' /* SGR. */

#define RTERM_NORMAL             0
#define RTERM_BOLD_ON            1
#define RTERM_UNDERLINE_ON       4
#define RTERM_BLINK_ON           5
#define RTERM_REVERSE_ON         7

#define RTERM_BOLD_OFF           22
#define RTERM_UNDERLINE_OFF      24
#define RTERM_BLINK_OFF          25
#define RTERM_REVERSE_OFF        27

#define RTERM_FOREGROUND_BLACK   30
#define RTERM_FOREGROUND_RED     31
#define RTERM_FOREGROUND_GREEN   32
#define RTERM_FOREGROUND_YELLOW  33
#define RTERM_FOREGROUND_BLUE    34
#define RTERM_FOREGROUND_MAGENTA 35
#define RTERM_FOREGROUND_CYAN    36
#define RTERM_FOREGROUND_WHITE   37
#define RTERM_FOREGROUND_EXTRA   38 /* 88- and 256-color modes */
#define RTERM_FOREGROUND_DEFAULT 39 

#define RTERM_BACKGROUND_BLACK   40
#define RTERM_BACKGROUND_RED     41
#define RTERM_BACKGROUND_GREEN   42
#define RTERM_BACKGROUND_YELLOW  43
#define RTERM_BACKGROUND_BLUE    44
#define RTERM_BACKGROUND_MAGENTA 45
#define RTERM_BACKGROUND_CYAN    46
#define RTERM_BACKGROUND_WHITE   47
#define RTERM_BACKGROUND_EXTRA   48 /* 88- and 256-color modes */
#define RTERM_BACKGROUND_DEFAULT 49

#define RTERM_FOREGROUND_BLACK2  90
#define RTERM_FOREGROUND_RED2    91
#define RTERM_FOREGROUND_GREEN2  92
#define RTERM_FOREGROUND_YELLOW2 93
#define RTERM_FOREGROUND_BLUE2   94
#define RTERM_FOREGROUND_MAGENTA2 95
#define RTERM_FOREGROUND_CYAN2   96
#define RTERM_FOREGROUND_WHITE2  97

#define RTERM_BACKGROUND_BLACK2  100
#define RTERM_BACKGROUND_RED2    101
#define RTERM_BACKGROUND_GREEN2  102
#define RTERM_BACKGROUND_YELLOW2 103
#define RTERM_BACKGROUND_BLUE2   104
#define RTERM_BACKGROUND_MAGENTA2 105
#define RTERM_BACKGROUND_CYAN2   106
#define RTERM_BACKGROUND_WHITE2  107

#define RTERM_FOREGROUND_OFFSET  30
#define RTERM_BACKGROUND_OFFSET  40

/* sequences with varying number of arguments - ESC[n[;n... */

/*
 * arguments for RTERM_XTERM_WINDOW_OPERATION:
 *  1 - uniconify window.
 *  2 - iconify window.
 *  3 - move window, must supply ;x;y.
 *  4 - resize window, must supply ;height;width.
 *  5 - raise window.
 *  6 - lower window.
 *  7 - refresh window.
 *  8 - resize text area, must supply ;height;width in characters.
 * 11 - report icon state.
 * 13 - report window position, returns "ESC[3;x;yt".
 * 14 - report window size in pixels, returns "ESC[4;height;widtht".
 * 18 - report text area size in characters, returns "ESC[8;height;widtht".
 * 20 - report icon name, returns "ESC]LnameESC\".
 * 21 - report window title, returns "ESC]ltitleESC\".
 */

#define RTERM_XTERM_WINDOW_OPERATION 't'

#define RTERM_XTERM_WINDOW_OPERATION_STRING_LENGTH RTERM_ESC_REPLY_LENGTH
#define RTERM_XTERM_WINDOW_POSITION_FORMAT         "\033[3;%d;%dt"
#define RTERM_XTERM_WINDOW_SIZE_FORMAT             "\033[4;%d;%dt"
/* FIXME: is this right? */
#define RTERM_XTERM_SCREEN_SIZE_FORMAT             "\033[8;%d;%dt"
#define RTERM_XTERM_ICON_NAME_FORMAT               "\033]L%s\033\\"
#define RTERM_XTERM_WINDOW_TITLE_FORMAT            "\033]l%s\033\\"

#define RTERM_XTERM_UNICONIFY_WINDOW       1
#define RTERM_XTERM_ICONIFY_WINDOW         2
#define RTERM_XTERM_MOVE_WINDOW            3
#define RTERM_XTERM_RESIZE_WINDOW          4
#define RTERM_XTERM_RAISE_WINDOW           5
#define RTERM_XTERM_LOWER_WINDOW           6
#define RTERM_XTERM_REFRESH_WINDOW         7
#define RTERM_XTERM_RESIZE_SCREEN          8
#define RTERM_XTERM_REPORT_ICON_STATE      11
#define RTERM_XTERM_REPORT_WINDOW_POSITION 13
#define RTERM_XTERM_REPORT_WINDOW_SIZE     14
#define RTERM_XTERM_REPORT_SCREEN_SIZE     18
#define RTERM_XTERM_REPORT_ICON_NAME       20
#define RTERM_XTERM_REPORT_WINDOW_TITLE    21

/* sequences prefixed with ESC[? */

/*
 * arguments for PRIVATE_MODE sequences, shown as set/reset:
 *    1 - application/normal cursor keys (DECCKM).
 *    3 - 132/80 column mode (DECCOLM).
 *    4 - smooth/jump scrolling.
 *    5 - reverse/normal video.
 *    6 - origin/normal cursor mode.
 *    7 - wraparound mode on/off.
 *    9 - X10 xterm mouse reporting, do/don't send pointer coordinates on
 *        button press.
 *   10 - toggle menubar on/off (rxvt/Eterm extension).
 *   25 - visible/invisible cursor.
 *   30 - toggle scrollbar on/off (rxvt/Eterm extension).
 *   35 - enable/disable xterm shift+key sequences (rxvt/Eterm extension).
 *   40 - enable/disable 80 <--> 132 column mode.
 *   47 - use alternate/normal screen buffer.
 *   66 - application/normal keypad (DECPAM).
 *   67 - backspace key sends BS/DEL (DECBKM).
 * 1000 - X11 xterm mouse reporting - do/don't send pointer coordinates on
 *        button press.
 */

#define RTERM_PRIVATE_MODE '?'

#define RTERM_SET_PRIVATE_MODE	  'h' /* DECSET. */
#define RTERM_RESET_PRIVATE_MODE	  'l' /* DECRST. */
#define RTERM_RESTORE_PRIVATE_MODE	  'r'
#define RTERM_SAVE_PRIVATE_MODES	  's'
#define RTERM_ETERM_TOGGLE_PRIVATE_MODE 't' /* rxvt/Eterm extension. */

#define RTERM_CURSOR_KEY_MODE             1 /* DECCKM. */
#define RTERM_US_ASCII_CHARSETS           2 /* DECANM, VT52 not supported. */
#define RTERM_132_COLUMN_MODE             3 /* DECCOLM. */
#define RTERM_SCROLL_MODE                 4 /* DECSCLM. */
#define RTERM_REVERSE_VIDEO_MODE          5 /* DECSCNM. */
#define RTERM_ORIGIN_MODE                 6 /* DECOM. */
#define RTERM_AUTOWRAP_MODE               7 /* DECAWM. */
#define RTERM_AUTO_REPEAT_KEYS            8 /* DECARM. */
#define RTERM_XTERM_X10_MOUSE_REPORTING      9 /* send mouse x and y on
					  * button press.
					  */
#define RTERM_ETERM_MENUBAR_MODE             10
#define RTERM_START_BLINKING_CURSOR       12
#define RTERM_CURSOR_VISIBILITY           25
#define RTERM_ETERM_SCROLLBAR_MODE           30
#define RTERM_ETERM_SHIFT_SEQUENCES          35
#define RTERM_TEKTRONIX_MODE              38 /* DECTEK. */
#define RTERM_ALLOW_132_COLUMN_MODE       40
#define RTERM_XTERM_MORE_FIX                 41
#define RTERM_XTERM_MARGIN_BELL              44
#define RTERM_XTERM_REVERSE_AUTOWRAP         45
#define RTERM_XTERM_LOGGING                  46
#define RTERM_SCREEN_MODE                 47
#define RTERM_ETERM_KEYPAD_MODE              66
#define RTERM_ETERM_BACKSPACE_MODE           67
#define RTERM_XTERM_X11_MOUSE_REPORTING      1000 /* send mouse x and y on button
					     *press and release.
					     */
#define RTERM_XTERM_HIGHLIGHT_MOUSE_TRACKING 1001
#define RTERM_ETERM_HOME_ON_ECHO             1010
#define RTERM_ETERM_HOME_ON_REFRESH          1011
#define RTERM_ETERM_HOME_ON_INPUT            1012

/* must be bigger than any action number. */

/* sequences prefixed with ESC]. */

/*
 * RTERM_XTERM_SET_TEXT_PARAMETERS is of the form "ESC]n;stringBEL".
 * attributes:
 *  0 - change icon name and window title.
 *  1 - change icon name.
 *  2 - change window title.
 *  3 - set text property.
 *  4 - change color.
 *  5 - change special color. // 5 - take keyboard focus and raise window.
 *  6 - Eterm sequence.
 *  7 - Eterm IPC sequence.
 * 10 - menubar commands.
 * 20 - pixmap commands.
 * 39 - set default foreground color.
 * 46 - change log file (currently not implemented)
 * 49 - set default background color.
 * 50 - set font. rxvt/Eterm extensions:
 *      string    empty: switch to font 0.
 *      string ==   "#n: switch to font n.
 *      string == "+#n": change font up by n, default 1.
 *      string == "-#n": change font down by n, default 1.
 */

#define RTERM_XTERM_SET_TEXT_PARAMETERS	 BEL

#define RTERM_XTERM_CHANGE_ICON_NAME_AND_WINDOW_TITLE 0
#define RTERM_XTERM_CHANGE_ICON_NAME                  1
#define RTERM_XTERM_CHANGE_WINDOW_TITLE               2
#define RTERM_XTERM_SET_TEXT_PROPERTY                 3
#if (SUPPORT_XTERM_256_COLORS)
#define RTERM_XTERM_SET_COLOR                         4
#endif
#define RTERM_XTERM_SET_SPECIAL_COLOR                 5
//#define RTERM_ETERM_TAKEOVER                          5
//#define RTERM_ETERM_SEQUENCE                          6
//#define RTERM_ETERM_IPC                               7
//#define RTERM_ETERM_MENUBAR_COMMAND                   10 /* FIXME: is this right? */
#define RTERM_XTERM_SET_VT100_FOREGROUND_COLOR        10
#define RTERM_XTERM_SET_VT100_BACKGROUND_COLOR        11
#define RTERM_ETERM_PIXMAP_COMMAND                    20
#define RTERM_XTERM_SET_DEFAULT_FOREGROUND_COLOR      39
#define RTERM_XTERM_CHANGE_LOG_FILE                   46
#define RTERM_XTERM_SET_DEFAULT_BACKGROUND_COLOR      49
#define RTERM_XTERM_SET_FONT                          50

#define RTERM_XTERM_RESET_COLOR                       104
#define RTERM_XTERM_RESET_SPECIAL_COLOR               105
#define RTERM_XTERM_RESET_VT100_FOREGROUND_COLOR      110
#define RTERM_XTERM_RESET_VT100_BACKGROUND_COLOR      111

/* Eterm sequences.
 * 0-9   - image class/mode configuration.
 * 10-19 - scrollbar/buttonbar/menu configuration.
 * 20-39 - miscellaneous toggles.
 * 40-49 - foreground/background color configuration.
 * 50-69 - window/window manager configuration/interaction.
 * 70+   - internal Eterm operations.
 */

#define RTERM_ETERM_SET_TRANSPARENCY         0
#define RTERM_ETERM_SET_COLOR_MODIFIERS      1
#define RTERM_ETERM_SHADE_OR_TINT            2
#define RTERM_ETERM_FORCE_BACKGROUND_UPDATE  3
#define RTERM_ETERM_CONFIGURE_SCROLLBAR      10
#define RTERM_ETERM_SCROLLBAR_RIGHT          11
#define RTERM_ETERM_SCROLLBAR_FLOATING       12
#define RTERM_ETERM_SCROLLBAR_POPUP          13
#define RTERM_ETERM_BUTTONBARS               14
#define RTERM_ETERM_VISUAL_BELL              20
#define RTERM_ETERM_MAP_ALERT                21
#define RTERM_ETERM_XTERM_SELECTION_BEHAVIOR 22
#define RTERM_ETERM_TRIPLE_CLICK_SELECT      23
#define RTERM_ETERM_VIEWPORT_MODE            24
#define RTERM_ETERM_SELECT_TRAILING_SPACES   25
#define RTERM_ETERM_REPORT_AS_KEYSYMS        26
#define RTERM_ETERM_REFUSE_INPUT_AND_FOCUS   27
#if 0
#define RTERM_ETERM_SET_FOREGROUND_COLOR     30
#define RTERM_ETERM_SET_BACKGROUND_COLOR     40
#endif
#define RTERM_ETERM_MOVE_TO_DESKTOP          50
#if 0
#define RTERM_ETERM_EXIT                     70
#define RTERM_ETERM_SAVE_CONFIG              71
#endif
#define RTERM_ETERM_SEARCH_FOR_STRING        72
#define RTERM_ETERM_SET_DEBUG_LEVEL          80

/* MUST be bigger than any action number. */

/* sequences of the form "ESC]cstringESC\". */

#define RTERM_XTERM_SET_WINDOW_TITLE	 'l'
#define RTERM_XTERM_SET_ICON_NAME	 'L'
#define RTERM_XTERM_SET_ICON_PIXMAP	 'I'

#define RTERM_SCALE_BACKGROUND_PIXMAP_STRING ""
#define RTERM_TRANSPARENT_STRING             "\033]6;0\a"
#define RTERM_UTMP_LOGGING_STRING            ""
#define RTERM_SCROLLBAR_RIGHT_STRING         "\033]6;11\a"
#define RTERM_FLOATING_SCROLLBAR_STRING      "\033]6;12\a"
#define RTERM_POPUP_SCROLLBAR_STRING         "\033]6;13\a"
#define RTERM_BORDERLESS_STRING              ""
#define RTERM_VISUAL_BELL_STRING             "\033]6;20\a"
#define RTERM_MAP_ALERT_STRING               "\033]6;21\a"
#define RTERM_META_8_STRING                  ""
#define RTERM_SAVE_UNDER_STRING              ""
#define RTERM_PAUSE_ON_EXIT_STRING           ""
#define RTERM_XTERM_SELECT_STRING            "\033]6;22\a"
#define RTERM_SELECT_LINE_STRING             "\033]6;23\a"
#define RTERM_VIEWPORT_MODE_STRING           "\033]6;24\a"
#define RTERM_CONSOLE_STRING                 ""
#define RTERM_SELECT_TRAILING_SPACES_STRING  "\033]6;25\a"
#define RTERM_REPORT_AS_KEYSYMS_STRING       "\033]6;26\a"
#define RTERM_DOUBLE_BUFFER_STRING           ""
#define RTERM_BLINKING_CURSOR_STRING         ""
#define RTERM_BLINKING_CHARS_STRING          ""
#define RTERM_CONFIRM_DESTROY_STRING         ""

#define RTERM_VT100_ANSWER "\033[?1;2c"

#define RTERM_ETERM_REPORT_KEYSYM_FORMAT "\033[k%X;%X~"

#define RTERM_MAX_ESCAPE_ARGS 32

#define RTERM_MAX_KEY_STRING_LENGTH 128

#define RTERM_FUNCTION_KEY_STRING_LENGTH 5

#define RTERM_SHIFT_TAB_STRING   "\033[Z"

#define RTERM_KEYPAD_HOME_STRING "\033Ow"

#if (USE_LINUX_KEYS)
#   define RTERM_HOME_STRING "\033[1~" /* Home = Find */
#   define RTERM_END_STRING  "\033[4~" /* End = Select */
#else /* !USE_LINUX_KEYS */
#   define RTERM_HOME_STRING "\033[7~"
#   define RTERM_END_STRING  "\033[8~"
#endif /* USE_LINUX_KEYS */

#define RTERM_KEYPAD_ARROW_PREFIX "\033O"
#define RTERM_ARROW_PREFIX        "\033["

#define RTERM_KEYPAD_PRIOR_STRING "\033Oy"
#define RTERM_PRIOR_STRING        "\033[5~"

#define RTERM_KEYPAD_NEXT_STRING "\033Os"
#define RTERM_NEXT_STRING        "\033[6~"

#define RTERM_KEYPAD_END_STRING "\033Oq"

#define RTERM_SELECT_STRING "\033[4~"

#define RTERM_EXECUTE_STRING "\033[3~"

#define RTERM_KEYPAD_INSERT_STRING "\033Op"
#define RTERM_INSERT_STRING        "\033[2~"

#define RTERM_KEYPAD_DELETE_STRING "\033On"

#if (USE_DELETE_KEY)
#   define RTERM_DELETE_STRING        "\033[3~" /* Delete = Execute */
#endif /* USE_DELETE_KEY */

#define RTERM_MENU_STRING "\033[29~"

#define RTERM_FIND_STRING "\033[1~"

#define RTERM_HELP_STRING "\033[28~"

#define RTERM_KEYPAD_ENTER_STRING "\033OM"

#define RTERM_KEYPAD_BEGIN_STRING "\033Ou"

#define RTERM_KEYPAD_FUNCTION_KEY_PREFIX "\033O"

#define RTERM_KEYPAD_PREFIX "\033O"

#define RTERM_CTRL_MINUS_STRING "\037" /* ^_ */

#define RTERM_SCREEN_CLEAR_TAB       0
#define RTERM_SCREEN_SET_TAB         1

#define RTERM_SCREEN_DRAW_ALL           0
#define RTERM_SCREEN_DRAW_BLINKING_ONLY 1
#define RTERM_SCREEN_DRAW_MODES         2

#define RTERM_SCREEN_INDEX_DOWN -1
#define RTERM_SCREEN_INDEX_UP    1

#if (SUPPORT_TRUETYPE_FONTS) && (SUPPORT_FREETYPE2)
struct ftfont {
    FT_Face face;
    int ascent;
    int w;
    int h;
#if (SUPPORT_RTERM_UTF8_CHARS)
    Pixmap bitmaps[65536];
    int widths[65536];
    int heights[65536];
#if (RTERM_USE_XSHM) && (SUPPORT_XSHM_FONT_BITMAP)
    XShmSegmentInfo shminfo;
    uint8_t *shmaddrs[65536];
#elif (RTERM_USE_XSHM)
    XShmSegmentInfo shminfos[256];
#endif
#else
    Pixmap bitmaps[256];
    int widths[256];
    int heights[256];
#if (RTERM_USE_XSHM)
    XShmSegmentInfo shminfos[256];
#endif
#endif
};
typedef struct ftfont ftfont_t;
#endif

struct R_user {
    uid_t realuid;      /* real user id of process. */
    uid_t effectiveuid; /* effective user id / set-user-id of process. */

    gid_t realgid;      /* real group id of process. */
    gid_t effectivegid; /* effective group id / set-group-id of process. */
};

struct R_userlog {
    int fd; /* master pty fd. */
    int utmppos; /* BSD utmp position. */
    char *ptyname; /* name of slave pty device. */
    char *hostname; /* name of the (remote) host. */
    char utmpid[RTERM_USERLOG_UTMPID_LENGTH];     /* utmp id. */
    char utmpline[RTERM_USERLOG_UTMPLINE_LENGTH]; /* utmp line. */
    struct R_user user; /* user information of process. */
};

#define USE_POSIX_TERMIOS 1

#if (USE_POSIX_TERMIOS) || (USE_TERMIOS)

typedef struct termios R_ptymode;

#elif (USE_SYSV_TERMIO)

struct Rptymode {
    struct termio tio;
#if (USE_LTCHARS)
    struct ltchars lc;
#endif /* USE_LTCHARS */
#if (USE_SYSV_TERMIO_WITH_LOCAL)
    unsigned int local;
#endif
};
typedef struct Rptymode R_ptymode;

#else

/* sgtty interface */

struct R_ptymode {
    struct sgttyb sg;
    struct tchars tc;
#if (USE_LTCHARS)
    struct ltchars lc;
#endif /* USE_LTCHARS */
    int line;
    int local;
#if defined(sony)
    long jmode;
    struct jtchars jc;
#endif /* defined(sony) */
};
typedef struct Rptymode R_ptymode;
#endif /* (USE_POSIX_TERMIOS) || (USE_TERMIOS) */

struct R_pty {
    int              masterfd;
    int              slavefd;
    char            *mastername;
    char            *slavename;
    struct stat      stat;
    struct R_userlog userlog;
    R_ptymode        mode;
};

struct R_termfuncs {
    int (*read_char)();
    int (*queue_input)();
    int (*queue_output)();
    void (*add_text)();
};

#define R_STRING_SELECTION 0
#define R_UTF8_SELECTION   1
struct R_termselection {
    struct R_termscreen *screen;
    int baserow;
    int basecolumn;
    int firstrow;
    int firstcolumn;
    int lastrow;
    int lastcolumn;
    long stringlen;
    uint8_t *stringdata;
    long utf8len;
    uint8_t *utf8data;
};

#define RTERM_CHARSET_KANJI -2
#define RTERM_CHARSET_G0     0
#define RTERM_CHARSET_G1     1
#define RTERM_CHARSET_G2     2
#define RTERM_CHARSET_G3     3
#define RTERM_MAX_CHARSET    3
struct R_term {
    pid_t                    pid;
    pid_t                    ppid;
    struct R_window         *window;
    struct R_window         *maponenter;
    struct R_termscrollbar  *scrollbar;
    char                    *bgname;
    char                    *fontname;
#if (SUPPORT_TRUETYPE_FONTS)
    ftfont_t                *font;
#endif
    int                      fgcolor;
    int                      x;
    int                      y;
    unsigned int             w;
    unsigned int             h;
    unsigned long            flags;
    unsigned long            privmodes;
    int                      blink;
    struct timeval           blinktv;
    /* screens */
    int                      nfds;
    int                      xconnfd;
    int                      nscreens;
    int                      curscreen;
#if (SUPPORT_RTERM_TABS)
//    int                      screenids[RTERM_SCREENS];
#endif
//    struct R_termscreen     *screen;
    struct R_termscreen    **screens;
//    struct R_termscreen    **fdscreens;
    Atom                     wmdeletewindow;
    GC                       selectiongc;
    int                      metamask;
    int                      altmask;
    int                      numlockmask;
    struct R_termfuncs       funcs;
    struct R_termselection   selection;
    struct R_user            user;
    int                      charset;
    char                     charsets[RTERM_MAX_CHARSET + 1];
    R_text_t                *path;
};

struct R_termscreenfuncs {
    int (*alloc_buffers)();
    void (*destroy_buffers)();
    int (*resize_buffers)();
    void (*add_text)();
    int (*scroll_text)();
    void (*insert_blank)();
    void (*delete_string)();
    void (*blank_string)();
    void (*blank_drawn)();
    void (*refresh)();
};

struct R_termlinebuf {
    int       firstrow;
    int       firstcol;
    int       lastrow;
    int       lastcol;
};

struct R_termscreenbuf {
    int             nrows;
    int            *rowlens;
    int            *rowdirty;
    int            *rowcols;
    R_text_t      **data;
#if (SUPPORT_RTERM_UTF8_CHARS)
    Rterm_wchar_t **wdata;
#endif
    R_textflags_t **renddata;
};

struct R_termsavedcursor {
    R_textflags_t textflags;
    int           row;
    int           column;
    int           charset;
    unsigned char charset_char;
};

#define RTERM_INPUT_BUFFER_SIZE      32768
#define RTERM_OUTPUT_BUFFER_MIN_SIZE 32768
struct R_termscreen {
    /* basic info */
#if (SUPPORT_RTERM_TABS)
    int           id;
#endif
    unsigned long flags;
    pid_t         cmdpid;
    int           type;
    int           savelines;
    int           nsavelines;
    int           viewrow;
    int           rows;
    int           columns;
    int           row;
    int           column;
    int           cursorrow;
    int           cursorcolumn;
    int           winw;
    int           winh;
    int           scrolltop;
    int           scrollbottom;
    /* state flags */
    R_textflags_t textflags;
    R_textflags_t gctextflags;
    /* draw info */
    int           charasc;
    int           chardesc;
    int           charw;
    int           charh;
    XFontStruct  *fontinfo;
    GC            defaultgc;
    GC            gc;
    /* double buffer */
    Pixmap        drawpixmap;
    Pixmap        buf;
    GC            bufgc;
#if (SUPPORT_TRUETYPE_FONTS)
    ftfont_t     *font;
    GC            bggc;
#endif
#if (RTERM_USE_XSHM)
    XShmSegmentInfo bufshminfo;
#endif
#if (USE_IMLIB2)
    struct R_image drawimage;
#endif
    /* draw state */
    int           refreshed;
    int           refreshcnt;
    /* cursor */
    GC            cursorgc;
    /* I/O buffers */
    struct R_window *window;
#if (SUPPORT_RTERM_UTF8_CHARS)
    R_text_t      *utf8buf;
#endif
    size_t         outsize;
    R_text_t      *outbuf;
    R_text_t      *outptr;
    R_text_t      *outend;
    struct R_pty  pty;
    struct R_termscreenfuncs funcs;
    /* screen buffers */
    struct R_termscreenbuf textbuf;
    struct R_termscreenbuf drawbuf;
    R_color_t colors[RTERM_SCREEN_COLORS];
    R_color_t defcolors[RTERM_SCREEN_DEFAULT_COLORS];
#if 0
    R_text_t *colornames[RTERM_SCREEN_COLORS];
#endif
    char **colornames;
    /* savedcursor */
    int charset;
    struct R_termsavedcursor savedcursor;
    int tabs[RTERM_SCREEN_MAX_COLUMNS];
    /* input buffer */
    R_text_t      *inptr;
    R_text_t      *inend;
    R_text_t      inbuf[RTERM_INPUT_BUFFER_SIZE] __CC_PAGE_ALIGN__;
#if (SUPPORT_TRUETYPE_FONTS) && (SUPPORT_FREETYPE2)
    struct ftfont *ftfont;
#endif
};

struct R_termscrollbar {
    unsigned long    flags;

    struct R_window *window;
    struct R_window *anchor;

    struct R_window *up;
    struct R_window *down;

    int              anchortop;
    int              anchorbottom;
#if (USE_IMLIB2)
    struct R_image   images[RTERM_SCROLLBAR_IMAGES]; /* scrollbar images. */
#endif
};

/* exit.c */
void Rterm_exit(void);

/* signalhandler.c */
RETSIGTYPE Rterm_exit_signal(int sig);
RETSIGTYPE Rterm_quit_signal(int sig);
RETSIGTYPE Rterm_child_signal(int sig);

/* char.c */
int Rterm_alloc_screen_textbuffers(struct R_termscreen *screen);
void Rterm_destroy_screen_textbuffers(struct R_termscreen *screen);
int Rterm_resize_screen_textbuffers(struct R_termscreen *screen,
                                    int rows,
                                    int columns);
void Rterm_add_screen_text(struct R_termscreen *screen,
                           const R_text_t *str,
                           int len,
                           int rows);
int Rterm_scroll_screen_text(struct R_termscreen *screen,
                             int toprow, int bottomrow,
                             int count,
                             int saverows);
void Rterm_insert_blank_screen_string(struct R_termscreen *screen, int len);
void Rterm_delete_screen_string(struct R_termscreen *screen, int len);
void Rterm_blank_screen_string(struct R_termscreen *screen,
                               int row,
                               int column,
                               int len);
void Rterm_blank_screen_drawn_string(struct R_termscreen *screen,
                                     int row, int column, int len);
void Rterm_refresh_screen(struct R_termscreen *screen, int mode);
#if (SUPPORT_RTERM_UTF8_CHARS)
Rterm_wchar_t Rterm_read_char_utf8(struct R_term *term);
#endif
int Rterm_read_char(struct R_term *term);
void Rterm_add_text(struct R_term *term, struct R_termscreen *screen);
int Rterm_queue_input(struct R_term *term, const char *str, int len);
int Rterm_queue_output(struct R_term *term, const char *str, int len);

/* eventhandler.c */
void Rterm_screen_buttonpress(void *arg, XEvent *event);
void Rterm_screen_buttonrelease(void *arg, XEvent *event);
void Rterm_screen_motionnotify(void *arg, XEvent *event);
void Rterm_screen_expose(void *arg, XEvent *event);

void Rterm_start_selection(void *arg, XEvent *event, void *dummy);
void Rterm_finish_selection(void *arg, XEvent *event, void *dummy);
void Rterm_request_selection(void *arg, XEvent *event, void *dummy);
void Rterm_paste_selection(void *arg, XEvent *event);
void Rterm_make_selection(void *arg, XEvent *event);
void Rterm_destroy_selection(void *arg, XEvent *event);

void Rterm_scrollbar_buttonpress(void *arg, XEvent *event);
void Rterm_scrollbar_buttonrelease(void *arg, XEvent *event);
void Rterm_scrollbar_leavenotify(void *arg, XEvent *event);
void Rterm_scrollbar_anchor_buttonpress(void *arg, XEvent *event);
void Rterm_scrollbar_anchor_buttonrelease(void *arg, XEvent *event);
void Rterm_scrollbar_anchor_enternotify(void *arg, XEvent *event);
void Rterm_scrollbar_anchor_leavenotify(void *arg, XEvent *event);
void Rterm_scrollbar_anchor_motionnotify(void *arg, XEvent *event);
void Rterm_scrollbar_up_buttonpress(void *arg, XEvent *event);
void Rterm_scrollbar_up_buttonrelease(void *arg, XEvent *event);
void Rterm_scrollbar_up_enternotify(void *arg, XEvent *event);
void Rterm_scrollbar_up_leavenotify(void *arg, XEvent *event);
void Rterm_scrollbar_down_buttonpress(void *arg, XEvent *event);
void Rterm_scrollbar_down_buttonrelease(void *arg, XEvent *event);
void Rterm_scrollbar_down_enternotify(void *arg, XEvent *event);
void Rterm_scrollbar_down_leavenotify(void *arg, XEvent *event);
void Rterm_keypress(void *arg, XEvent *event);
void Rterm_enternotify(void *arg, XEvent *event);
void Rterm_expose(void *arg, XEvent *event);
void Rterm_focusin(void *arg, XEvent *event);
void Rterm_focusout(void *arg, XEvent *event);
void Rterm_configurenotify(void *arg, XEvent *event);

#endif /* __RESURRECTION_TERM_H__ */

