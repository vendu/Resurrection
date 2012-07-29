/*
 * file.c - Rv file management.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#define RV_ADD_FILE(f) \
    do {                                                                \
        if (_Rvtailfile) {                                              \
            _Rvtailfile->next = (f);                                    \
            (f)->prev = _Rvtailfile;                                    \
            (f)->next = _Rvheadfile;                                    \
            if (_Rvheadfile) {                                          \
                _Rvheadfile->prev = (f);                                \
            }                                                           \
            _Rvtailfile = (f);                                          \
        } else if (_Rvheadfile) {                                       \
            _Rvheadfile->next = (f);                                    \
            (f)->prev = _Rvheadfile;                                    \
            (f)->next = _Rvheadfile;                                    \
            _Rvtailfile = (f);                                          \
        } else {                                                        \
            _Rvheadfile = (f);                                          \
        }                                                               \
    } while (FALSE);

int
Rv_add_file(const char *filename)
{
//    char *dirname;
    struct R_file *file;
    struct stat statbuf;
    Imlib_Image img;

#if 0
    dirname = getcwd(NULL, 0);
    if (stat(filename,
             &statbuf) < 0) {

        return FALSE;
    }
    strcat(file->name, dirname);
#endif
    file = calloc(sizeof(struct R_file),
                  1);
    file->name = calloc(1, 2 * PATH_MAX + 2);
    strcat(file->name, filename);
    if (S_ISDIR(statbuf.st_mode)) {
        fprintf(stderr, "DIR: %s\n", filename);
        if (!Rv_add_dir(filename)) {

            return FALSE;
        }
    } else {
        RV_ADD_FILE(file);
    }

    return TRUE;
}

int
Rv_add_dir(const char *dirname)
{
    struct R_file *file;
    DIR *dp;
    struct dirent *dirent;
    Imlib_Image img;
    int loaded;
    struct stat statbuf;

    loaded = FALSE;
    dp = opendir(dirname);
    if (!dp) {

        return FALSE;
    }
    dirent = readdir(dp);
    while (!strcmp(dirent->d_name, ".")
           || !strcmp(dirent->d_name, "..")) {
        dirent = readdir(dp);
    }
    if (dirent) {
        file = calloc(sizeof(struct R_file),
                      1);
        file->name = calloc(1, 2 * PATH_MAX + 2);
        strcat(file->name, dirname);
        strcat(file->name, dirent->d_name);
        if (stat(file->name,
                 &statbuf) < 0) {
            
            return FALSE;
        }
        if (S_ISDIR(statbuf.st_mode)) {
            if (!Rv_add_dir(file->name)) {

                return FALSE;
            }
            free(file->name);
            free(file);
        } else {
            RV_ADD_FILE(file);
        }
    } else {
        
        return FALSE;
    }
    dirent = readdir(dp);
    while (dirent) {
        file = calloc(sizeof(struct R_file),
                      1);
        file->name = calloc(1, 2 * PATH_MAX + 2);
        strcat(file->name, dirname);
        strcat(file->name, dirent->d_name);
        if (stat(file->name,
                 &statbuf) < 0) {
            
            return FALSE;
        }
        if (S_ISDIR(statbuf.st_mode)) {
            if (!Rv_add_dir(file->name)) {

                return FALSE;
            }
            free(file->name);
            free(file);
        } else {
            RV_ADD_FILE(file);
        }
        dirent = readdir(dp);
    }

    return TRUE;
}

void
Rv_set_file(struct R_app *app,
            int offset)
{
    struct R_file *oldfile;
    struct R_file *curfile;

    oldfile = curfile = _Rvcurfile;
    if (offset < 0) {
        while ((offset++) && curfile) {
            curfile = curfile->prev;
        }
    } else {
        while ((offset--) && curfile) {
            curfile = curfile->next;
        }
    }
    _Rvcurfile = curfile;
    Rv_show(app);

    return;
}

