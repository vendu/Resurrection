/*
 * protos.h - Resurrection function prototypes.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef __RESURRECTION_PROTOS_H__
#define __RESURRECTION_PROTOS_H__

char * R_parse_display(struct R_app *app);
int R_init(struct R_app *app,
           int argc,
           char **argv);

void * R_alloc_object(int zero);
void R_free_object(void *obj);

void R_handle_events(struct R_app *app);
long R_handle_next_event(struct R_app *app);

int R_handle_x_error(Display *display, XErrorEvent *event);

int R_add_binding(struct R_binding *binding,
                  struct R_window *window);
struct R_binding * R_find_binding(struct R_window *window,
                                  R_binding_t mask);
struct R_binding * R_create_binding(struct R_window *window,
                                    R_binding_t mask,
                                    R_bindingfunc_t *func,
                                    void *arg);

void R_add_window(struct R_window *window);
struct R_window * R_find_window(Window id);
struct R_window * R_remove_window(struct R_window *window);
struct R_window * R_create_window(struct R_app *app,
                                  struct R_window *parent,
                                  unsigned long flags);
void R_init_window(struct R_app *app,
                   struct R_window *window,
                   struct R_window *parent,
                   unsigned long flags);
int R_set_window_event_handler(struct R_window *window,
                               int event,
                               R_eventhandler_t *handler);
long _R_add_window_events(struct R_window *window,
                          long mask);
long _R_remove_window_events(struct R_window *window,
                             long mask);
void _R_map_window(struct R_window *window);
void _R_clear_window(struct R_window *window);
void _R_move_window(struct R_window *window,
                    int x,
                    int y);
void _R_resize_window(struct R_window *window,
                      int w,
                      int h);
void _R_move_resize_window(struct R_window *window,
                           int x,
                           int y,
                           int w,
                           int h);

char * R_get_home_dir(void);

void Rwm_reparent_window(struct R_window *window,
                         struct R_window *parent,
                         int x,
                         int y);

int Rwm_init(struct R_app *app,
             int argc,
             char **argv);
int Rwm_take_windows(struct R_app *app);

int Rwm_set_root_window(struct R_app *app);
void Rwm_init_root_events(struct R_app *app);
void Rwm_root_createnotify_handler(void *root,
                                   XEvent *event);
void Rwm_root_configurerequest_handler(void *window,
                                       XEvent *event);
void Rwm_root_maprequest_handler(void *arg,
                                 XEvent *event);
void Rwm_root_enternotify_handler(void *arg,
                                  XEvent *event);

void Rwm_init_client_events(struct R_window *window);

void Rwm_init_frame_events(struct R_window *frame);
int Rwm_init_frame_images(struct R_app *app);
int Rwm_init_frame_cursors(struct R_app *app);
struct R_window * Rwm_frame_window(struct R_window *window);
struct R_window * Rwm_create_frame(struct R_app *app,
                                   struct R_window *window);
int Rwm_init_frame(struct R_window *frame);
void Rwm_free_frame(struct R_window *window);
void Rwm_title_buttonpress_handler(void *arg,
                                   XEvent *event);

void Rwm_move_client(struct R_window *window,
                     int x,
                     int y);
void Rwm_resize_client(struct R_window *window,
                       int w,
                       int h);
void Rwm_move_resize_client(struct R_window *window,
                            int x,
                            int y,
                            int w,
                            int h);
void Rwm_client_maprequest_handler(void *arg,
                                   XEvent *event);
void Rwm_client_configurerequest_handler(void *arg,
                                         XEvent *event);
void Rwm_client_destroynotify_handler(void *arg,
                                      XEvent *event);
void Rwm_client_mapnotify_handler(void *arg,
                                  XEvent *event);
void Rwm_client_unmapnotify_handler(void *arg,
                                    XEvent *event);

void Rwm_move_window(struct R_window *window,
                     int x,
                     int y);
void Rwm_resize_window(struct R_window *window,
                       int w,
                       int h);
void Rwm_move_resize_window(struct R_window *window,
                            int x,
                            int y,
                            int w,
                            int h);
void Rwm_init_window_position(struct R_window *window);
void Rwm_set_input_focus(struct R_window *window, Time timestamp);

void Rwm_title_expose_handler(void *arg,
                              XEvent *event);

int Rwm_init_desktops(struct R_app *app);
int Rwm_init_desktop(struct R_app *app,
                     int id);
int Rwm_create_desktop_window(struct R_app *app);
int Rwm_init_desktop_images(struct R_app *app);
void Rwm_map_desktop(struct R_app *app,
                     int id);
void Rwm_switch_desktop(struct R_app *app,
                        struct R_window *desktop);

int Rwm_init_menu(struct R_app *app);
int Rwm_create_menu_window(struct R_app *app);
struct R_window * Rwm_create_menu_item(struct R_app *app,
                                       int id);
int Rwm_init_menu_item(struct R_window *item);
int Rwm_init_menu_bindings(struct R_app *app);
void Rwm_init_menu_events(struct R_app *app);
void Rwm_add_menu(struct R_app *app);
void Rwm_map_menu(struct R_app *app);
void Rwm_menu_enternotify_handler(void *arg,
                                  XEvent *event);
void Rwm_menu_leavenotify_handler(void *arg,
                                  XEvent *event);
void Rwm_menu_item_buttonpress_handler(void *arg,
                                       XEvent *event);

int Rwm_init_pager(struct R_app *app);
int Rwm_create_pager_windows(struct R_app *app);
int Rwm_create_pager_thumb(struct R_app *app,
                           int id);
int Rwm_init_pager_bindings(struct R_app *app);
void Rwm_init_pager_events(struct R_app *app);
void Rwm_add_pager(struct R_app *app);
void Rwm_map_pager(struct R_app *app);
void Rwm_pager_enternotify_handler(void *window,
                                   XEvent *event);
void Rwm_pager_leavenotify_handler(void *window,
                                   XEvent *event);
void Rwm_pager_thumb_buttonpress_handler(void *arg,
                                         XEvent *event);

int Rwm_init_bindings(struct R_app *app);
void Rwm_switch_desktop_func(void *arg,
                             XEvent *event,
                             void *bindarg);
void Rwm_exec_func(void *arg,
                   XEvent *event,
                   void *bindarg);
void Rwm_exit_func(void *arg,
                   XEvent *event,
                   void *bindarg);
void Rwm_shade_func(void *arg,
                    XEvent *event,
                    void *bindarg);
void Rwm_close_func(void *arg,
                    XEvent *event,
                    void *bindarg);
void Rwm_kill_func(void *arg,
                   XEvent *event,
                   void *bindarg);

void Rwm_start_move_func(void *arg,
                         XEvent *event,
                         void *bindarg);
void Rwm_move_func(void *arg,
                   XEvent *event,
                   void *bindarg);
void Rwm_finish_move_func(void *arg,
                          XEvent *event,
                          void *bindarg);
void Rwm_start_resize_func(void *arg,
                           XEvent *event,
                           void *bindarg);
void Rwm_finish_resize_func(void *arg,
                            XEvent *event,
                            void *bindarg);
void Rwm_resizev_func(void *arg,
                      XEvent *event,
                      void *bindarg);
void Rwm_resizeh_func(void *arg,
                      XEvent *event,
                      void *bindarg);

void R_init_imlib2(struct R_app *app);
struct R_image * R_load_image_imlib2(struct R_app *app,
                                     const char *filename,
                                     struct R_image *image);
int R_reload_image_imlib2(struct R_image *image);
void R_free_image_imlib2(struct R_image *image);
int R_render_image_imlib2(struct R_image *image,
                          struct R_window *window,
                          int w,
                          int h,
                          int flags);
void R_set_background_imlib2(struct R_image *image,
                             struct R_window *window,
                             int w,
                             int h,
                             int flags);
struct R_window * R_create_thumb_imlib2(struct R_window *parent,
                                        struct R_image *image,
                                        int w,
                                        int h,
                                        int flags);

#endif /* __RESURRECTION_PROTOS_H__ */

