/*
 * Copyright (c) 2001-2004 Torrey T. Lyons. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the sale,
 * use or other dealings in this Software without prior written authorization.
 */

#ifndef _DARWIN_H
#define _DARWIN_H

#include <IOKit/IOTypes.h>
#include "inputstr.h"
#include "scrnintstr.h"
#include <X11/extensions/XKB.h>
#include <assert.h>

typedef struct {
    void                *framebuffer;
    int                 x;
    int                 y;
    int                 width;
    int                 height;
    int                 pitch;
    int                 depth;
    int                 visuals;
    int                 bitsPerRGB;
    int                 bitsPerPixel;
    int                 preferredCVC;
    Pixel               redMask;
    Pixel               greenMask;
    Pixel               blueMask;
} DarwinFramebufferRec, *DarwinFramebufferPtr;

// From darwin.c
void DarwinPrintBanner(void);
int DarwinParseModifierList(const char *constmodifiers);
void DarwinAdjustScreenOrigins(ScreenInfo *pScreenInfo);
void xf86SetRootClip (ScreenPtr pScreen, BOOL enable);

#define SCREEN_PRIV(pScreen) \
    ((DarwinFramebufferPtr)pScreen->devPrivates[darwinScreenIndex].ptr)

/*
 * Global variables from darwin.c
 */
extern int              darwinScreenIndex; // index into pScreen.devPrivates
extern int              darwinScreensFound;
extern io_connect_t     darwinParamConnect;
extern int              darwinEventReadFD;
extern int              darwinEventWriteFD;
extern DeviceIntPtr     darwinPointer;
extern DeviceIntPtr     darwinKeyboard;

// User preferences
extern int              darwinMouseAccelChange;
extern int              darwinFakeButtons;
extern int              darwinFakeMouse2Mask;
extern int              darwinFakeMouse3Mask;
extern char            *darwinKeymapFile;
extern int              darwinSyncKeymap;
extern unsigned int     darwinDesiredWidth, darwinDesiredHeight;
extern int              darwinDesiredDepth;
extern int              darwinDesiredRefresh;

// location of X11's (0,0) point in global screen coordinates
extern int              darwinMainScreenX;
extern int              darwinMainScreenY;


/*
 * Special ddx events understood by the X server
 */
enum {
    kXquartzUpdateModifiers   // update all modifier keys
            = LASTEvent+1,    // (from X.h list of event names)
    kXquartzUpdateButtons,    // update state of mouse buttons 2 and up
    /*
     * Quartz-specific events -- not used in IOKit mode
     */
    kXquartzActivate,         // restore X drawing and cursor
    kXquartzDeactivate,       // clip X drawing and switch to Aqua cursor
    kXquartzSetRootClip,      // enable or disable drawing to the X screen
    kXquartzQuit,             // kill the X server and release the display
    kXquartzReadPasteboard,   // copy Mac OS X pasteboard into X cut buffer
    kXquartzWritePasteboard,  // copy X cut buffer onto Mac OS X pasteboard
    kXquartzBringAllToFront,  // bring all X windows to front
    kXquartzToggleFullscreen, // Enable/Disable fullscreen mode
    kXquartzSetRootless,      // Set rootless mode
    kXquartzSpaceChanged,     // Spaces changed
    /*
     * AppleWM events
     */
    kXquartzControllerNotify, // send an AppleWMControllerNotify event
    kXquartzPasteboardNotify, // notify the WM to copy or paste
    /*
     * Xplugin notification events
     */
    kXquartzDisplayChanged,   // display configuration has changed
    kXquartzWindowState,      // window visibility state has changed
    kXquartzWindowMoved,      // window has moved on screen
};

#define ENABLE_DEBUG_LOG 1

#ifdef ENABLE_DEBUG_LOG
extern FILE *debug_log_fp;
#define DEBUG_LOG_NAME "x11-debug.txt"
#define DEBUG_LOG(msg, args...) if (debug_log_fp) fprintf(debug_log_fp, "%s:%s:%d " msg, __FILE__, __FUNCTION__, __LINE__, ##args ); fflush(debug_log_fp);
#else
#define DEBUG_LOG(msg, args...) 
#endif

#define TRACE() DEBUG_LOG("\n")

#endif  /* _DARWIN_H */
