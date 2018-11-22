/* This software is in the public domain
 * and is provided AS IS, with NO WARRANTY. */

#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(void)
{
    Display * display;

    /*If we can't open display return 1*/    
    if(!(display = XOpenDisplay(0x0))) {
        return 1;
    }

//TODO: read up on this
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("q")), Mod1Mask,
        DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("d")), Mod1Mask,
        DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("r")), Mod1Mask,
        DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("0")), Mod1Mask,
        DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("Return")), Mod1Mask,
        DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("f")), Mod1Mask,
        DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);

    XGrabButton(display, 1, Mod1Mask, DefaultRootWindow(display), True,
        ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(display, 3, Mod1Mask, DefaultRootWindow(display), True,
        ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);


    const unsigned int  BORDER_WIDTH  = 1;
    const unsigned long BORDER_COLOR = 0xff0000;
    const unsigned long BG_COLOR     = 0x00ff00;

    XWindowAttributes attr;
    XWindowAttributes ib;
    XButtonEvent start;
    XEvent event; 
    Window foc;

    start.subwindow = None;  

    int revert_to;

    for(;;)
    {
        XNextEvent(display, &event);
	    XGetInputFocus(display, &foc, &revert_to);

        switch(event.type) {

            case KeyPress:
                if(event.xbutton.subwindow != None) {   
                    if(event.xkey.keycode == XKeysymToKeycode(display, XStringToKeysym("q"))) { 
                        XDestroyWindow(display, foc);
					}
                    else if (event.xkey.keycode == XKeysymToKeycode(display, XStringToKeysym("d"))) { 
                        XLowerWindow(display, foc);
					}
                    else if (event.xkey.keycode == XKeysymToKeycode(display, XStringToKeysym("r"))) { 
                        XRaiseWindow(display, foc);
					}
                }
                else {
                    if(event.xkey.keycode == XKeysymToKeycode(display, XStringToKeysym("d"))) { 
                        system("rofi -show run");
					}
                    else if(event.xkey.keycode == XKeysymToKeycode(display, XStringToKeysym("0"))) { 
                        XCloseDisplay(display);
					}
                }

            case ButtonPress:
                if(event.xbutton.subwindow != None) {
                    XGetWindowAttributes(display, event.xbutton.subwindow, &ib);

                    if(!ib.override_redirect) {
                        /*Save the windows size and location before doing anything*/ 
                        XGetWindowAttributes(display, event.xbutton.subwindow, &attr);
                        if(foc != event.xbutton.subwindow)
                        XSetInputFocus(display, event.xbutton.subwindow, RevertToParent, CurrentTime); 
                        XRaiseWindow(display, start.subwindow);
                    }
                  
                }

            case ButtonRelease:
                start.subwindow = None;

            case MotionNotify:
                if(start.subwindow != None) {
                    int xdiff = event.xbutton.x_root - start.x_root;
                    int ydiff = event.xbutton.y_root - start.y_root;
                    XMoveResizeWindow(display, start.subwindow,
                        attr.x + (start.button==1 ? xdiff : 0),
                        attr.y + (start.button==1 ? ydiff : 0),
                        /*Minimum window size when resizing is 100x50 pixels because
                        *I don't think there is any need for smaller windows*/
                        MAX(100, attr.width + (start.button==3 ? xdiff : 0)),
                        MAX(50, attr.height + (start.button==3 ? ydiff : 0)));
                }

            case MapRequest:
                XWindowAttributes xWindowAttrs;
                XGetWindowAttributes(display, event.xmaprequest.window, &xWindowAttrs);               
//////////////////////////////////////////////////////////////////////////////////////////////////
            case ConfigureNotify:
            {
                XWindowChanges changes;
                XConfigureRequestEvent eCfg= event.xconfigurerequest;
                break;
            }
            case CreateNotify:
            ;
            case DestroyNotify: 
            ;
            case ReparentNotify:
            ;
        }
   }
}

