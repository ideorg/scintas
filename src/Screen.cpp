#include "Screen.h"
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <unistd.h>
#include <vector>

class WindowsMatchingPid
{
public:
    WindowsMatchingPid(Display *display, Window wRoot, unsigned long pid)
            : _display(display)
            , _pid(pid)
    {
        // Get the PID property atom.
        _atomPID = XInternAtom(display, "_NET_WM_PID", True);
        if(_atomPID == None)
        {
            //cout << "No such atom" << endl;
            return;
        }

        search(wRoot);
    }

    const std::vector<Window> &result() const { return _result; }

private:
    unsigned long  _pid;
    Atom           _atomPID;
    Display       *_display;
    std::vector<Window>   _result;

    void search(Window w)
    {
        // Get the PID for the current Window.
        Atom           type;
        int            format;
        unsigned long  nItems;
        unsigned long  bytesAfter;
        unsigned char *propPID = 0;
        if(Success == XGetWindowProperty(_display, w, _atomPID, 0, 1, False, XA_CARDINAL,
                                         &type, &format, &nItems, &bytesAfter, &propPID))
        {
            if(propPID != 0)
            {
                // If the PID matches, add this window to the result set.
                if(_pid == *((unsigned long *)propPID))
                    _result.push_back(w);

                XFree(propPID);
            }
        }

        // Recurse into child windows.
        Window    wRoot;
        Window    wParent;
        Window   *wChild;
        unsigned  nChildren;
        if(0 != XQueryTree(_display, w, &wRoot, &wParent, &wChild, &nChildren))
        {
            for(unsigned i = 0; i < nChildren; i++)
                search(wChild[i]);
        }
    }
};

void BringToFront() {
    int pid = getpid();
// Start with the root window.
    Display *display = XOpenDisplay(0);
    WindowsMatchingPid match(display, XDefaultRootWindow(display), pid);
    Display *dsp = XOpenDisplay(NULL);
    long id = match.result()[0];
    XRaiseWindow ( dsp, id );
    XCloseDisplay ( dsp );
}
