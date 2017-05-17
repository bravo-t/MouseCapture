#include <stdio.h>
#include <X11/Xlib.h>
#include <time.h>

char *key_name[] = {
    "left key",
    "middle key",
    "right key",
    "wheel up",  // :D
    "wheel down"    // :|
};
int main(int argc, char **argv)
{
    Display *display;
    XEvent xevent;
    Window window;

    if( (display = XOpenDisplay(NULL)) == NULL )
        return -1;


    //window = DefaultRootWindow(display);
    int revert;
    XGetInputFocus(display, &window, &revert);
    XAllowEvents(display, AsyncBoth, CurrentTime);

    XGrabPointer(display, 
                 window,
                 0, 
                 PointerMotionMask | ButtonPressMask | ButtonReleaseMask | FocusChangeMask | EnterWindowMask | LeaveWindowMask | PropertyChangeMask, 
                 GrabModeAsync,
                 GrabModeAsync, 
                 None,
                 None,
                 CurrentTime);

    while(1) {
        XNextEvent(display, &xevent);
        switch (xevent.type) {
            case MotionNotify:
                printf("Mouse move      : [%d, %d]\n", xevent.xmotion.x_root, xevent.xmotion.y_root);
                break;
            case ButtonPress:
                printf("Button pressed  : %s\n", key_name[xevent.xbutton.button - 1]);
                break;
            case ButtonRelease:
                printf("Button released : %s\n", key_name[xevent.xbutton.button - 1]);
                break;
            case FocusIn:
                printf("Gain user focus\n");
                break;
            case FocusOut:
                printf("Lost user focus\n");
                break;
            case EnterNotify:
                printf("Mouse just moved into this window\n");
                XSetInputFocus(display,window,RevertToNone,CurrentTime);
                break;
            case LeaveNotify:
                printf("Mouse just left this window\n");
                XSetInputFocus(display,DefaultRootWindow(display),RevertToNone,CurrentTime);
                //Window rootWindow = DefaultRootWindow(display);
                //XGetInputFocus(display, &rootWindow, &revert);
                XUngrabPointer(display,CurrentTime);
                //XFree(&window);
                Window root_return, child_return;
                int root_x_return, root_y_return, win_x_return, win_y_return;
                unsigned int mask_return;
                struct timespec spec,tim;
                spec.tv_sec = 0;
                spec.tv_nsec = 10E6;
                while(1) {
                    //Window tmp = DefaultRootWindow(display);
                    XQueryPointer(display, window, &root_return, &child_return, &root_x_return, &root_y_return, &win_x_return,&win_y_return, &mask_return);
                    if (root_return == DefaultRootWindow(display)) printf("root_return is the root window\n");
                    if (child_return == window + 6) printf("child_return is current window\n");
                    printf("child_return = %lu, window = %lu, root_return = %lu, root = %lu, X:Y = %d:%d\n",child_return, window, root_return,DefaultRootWindow(display), win_x_return, win_y_return);
                    if (child_return == window + 6) {
                        XGrabPointer(display, 
                            window,
                            0, 
                            PointerMotionMask | ButtonPressMask | ButtonReleaseMask | FocusChangeMask | EnterWindowMask | LeaveWindowMask | PropertyChangeMask, 
                            GrabModeAsync,
                            GrabModeAsync, 
                            None,
                            None,
                            CurrentTime);
                        break;
                    } else {
                        nanosleep(&spec,&tim);
                    }
                }
                break;
            case PropertyNotify:
                printf("PropertyNotify received\n");
                break;
        }
    }

    return 0;
}

