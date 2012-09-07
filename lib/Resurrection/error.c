/*
 * error.c - error handling for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

int _ignbadwindow = 0;
int _ignbadmatch = 0;
int _ignbadatom = 0;

#define X_REQUEST_CODES (X_NoOperation + 1)
#define X_ERROR_CODES   (BadImplementation + 1)
#define X_ERROR_STRING_LENGTH 4096

static const char *xrequestnames[] =
{
    "",
    "X_CreateWindow",
    "X_ChangeWindowAttributes",
    "X_GetWindowAttributes",
    "X_DestroyWindow",
    "X_DestroySubwindows",
    "X_ChangeSaveSet",
    "X_ReparentWindow",
    "X_MapWindow",
    "X_MapSubwindows",
    "X_UnmapWindow",
    "X_UnmapSubwindows",
    "X_ConfigureWindow",
    "X_CirculateWindow",
    "X_GetGeometry",
    "X_QueryTree",
    "X_InternAtom",
    "X_GetAtomName",
    "X_ChangeProperty",
    "X_DeleteProperty",
    "X_GetProperty",
    "X_ListProperties",
    "X_SetSelectionOwner",
    "X_GetSelectionOwner",
    "X_ConvertSelection",
    "X_SendEvent",
    "X_GrabPointer",
    "X_UngrabPointer",
    "X_GrabButton",
    "X_UngrabButton",
    "X_ChangeActivePointerGrab",
    "X_GrabKeyboard",
    "X_UngrabKeyboard",
    "X_GrabKey",
    "X_UngrabKey",
    "X_AllowEvents",
    "X_GrabServer",
    "X_UngrabServer",
    "X_QueryPointer",
    "X_GetMotionEvents",
    "X_TranslateCoords",
    "X_WarpPointer",
    "X_SetInputFocus",
    "X_GetInputFocus",
    "X_QueryKeymap",
    "X_OpenFont",
    "X_CloseFont",
    "X_QueryFont",
    "X_QueryTextExtents",
    "X_ListFonts",
    "X_ListFontsWithInfo",
    "X_SetFontPath",
    "X_GetFontPath",
    "X_CreatePixmap",
    "X_FreePixmap",
    "X_CreateGC",
    "X_ChangeGC",
    "X_CopyGC",
    "X_SetDashes",
    "X_SetClipRectangles",
    "X_FreeGC",
    "X_ClearArea",
    "X_CopyArea",
    "X_CopyPlane",
    "X_PolyPoint",
    "X_PolyLine",
    "X_PolySegment",
    "X_PolyRectangle",
    "X_PolyArc",
    "X_FillPoly",
    "X_PolyFillRectangle",
    "X_PolyFillArc",
    "X_PutImage",
    "X_GetImage",
    "X_PolyText8",
    "X_PolyText16",
    "X_ImageText8",
    "X_ImageText16",
    "X_CreateColormap",
    "X_FreeColormap",
    "X_CopyColormapAndFree",
    "X_InstallColormap",
    "X_UninstallColormap",
    "X_ListInstalledColormaps",
    "X_AllocColor",
    "X_AllocNamedColor",
    "X_AllocColorCells",
    "X_AllocColorPlanes",
    "X_FreeColors",
    "X_StoreColors",
    "X_StoreNamedColor",
    "X_QueryColors",
    "X_LookupColor",
    "X_CreateCursor",
    "X_CreateGlyphCursor",
    "X_FreeCursor",
    "X_RecolorCursor",
    "X_QueryBestSize",
    "X_QueryExtension",
    "X_ListExtensions",
    "X_ChangeKeyboardMapping",
    "X_GetKeyboardMapping",
    "X_ChangeKeyboardControl",
    "X_GetKeyboardControl",
    "X_Bell",
    "X_ChangePointerControl",
    "X_GetPointerControl",
    "X_SetScreenSaver",
    "X_GetScreenSaver",
    "X_ChangeHosts",
    "X_ListHosts",
    "X_SetAccessControl",
    "X_SetCloseDownMode",
    "X_KillClient",
    "X_RotateProperties",
    "X_ForceScreenSaver",
    "X_SetPointerMapping",
    "X_GetPointerMapping",
    "X_SetModifierMapping",
    "X_GetModifierMapping",
    "X_NoOperation"
};

const char *xerrornames[] =
{
    "Success",
    "BadRequest",
    "BadValue",
    "BadWindow",
    "BadPixmap",
    "BadAtom",
    "BadCursor",
    "BadFont",
    "BadMatch",
    "BadDrawable",
    "BadAccess",
    "BadAlloc",
    "BadColor",
    "BadGC",
    "BadIDChoice",
    "BadName",
    "BadLength",
    "BadImplementation"
};

int
R_handle_x_io_error(Display *display, XErrorEvent *event)
{
    abort();
}

int
R_handle_x_error(Display *display, XErrorEvent *event)
{
    char *requestcodestr;
    char *errorcodestr;
    char errorstr[X_ERROR_STRING_LENGTH];

#if 0
    if (event->error_code == BadImplementation) {

        return 0;
    }
#endif
    if (event->error_code == BadDrawable) {

        return 0;
    }
    if (event->error_code == BadPixmap) {

        return 0;
    }

    if (event->error_code == BadWindow) {

        return 0;
    }

#if 0
    if ((_ignbadwindow) && (event->error_code == BadWindow)) {

        return 0;
    }
#endif
    if ((_ignbadmatch) && (event->error_code == BadMatch)) {

        return 0;
    }
#if 0
    if ((_ignbadatom) && (event->error_code == BadAtom)) {

        return 0;
    }
#endif
    if (event->error_code == BadAtom) {

        return 0;
    }

    requestcodestr = NULL;
    errorcodestr = NULL;

    if (event->request_code < X_REQUEST_CODES - 1) {
	requestcodestr = (char *)xrequestnames[(event->request_code)];
    }
    
    if (event->error_code < X_ERROR_CODES - 1) {
	errorcodestr = (char *)xerrornames[(event->error_code)];
    }
    
    errorstr[0] = '\0';
    
    XGetErrorText(display, event->error_code, errorstr, sizeof(errorstr));
    
    fprintf(stderr, "%s: XError, request == %s, error == %s, resourceid == 0x%08lx, minor_code == %d: %s\n", R_global.app->name, ((requestcodestr) ? requestcodestr : "\"unknown\""), ((errorcodestr) ? errorstr : "\"unknown\""), (unsigned long)event->resourceid, event->minor_code, errorstr);
    
    if (event->error_code == BadImplementation) {

	return 0;
    }

#if (RWM_DEBUG_X_ERRORS)
    abort();
#else
    exit(EXIT_FAILURE);
#endif
}

