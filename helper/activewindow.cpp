#include "activewindow.h"
#include "core/core_system.h"
#ifdef Q_OS_WIN
#include <Windows.h>
#elif defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
void __enum_window(Display*, Window);

#endif

QVector<Rect>rects;

ActiveWindow::ActiveWindow() { rects.clear(); }

QVector<Rect> ActiveWindow::enum_window()
{
    rects.clear();

#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX)
    Display *display=XOpenDisplay(0);
    Window win=XDefaultRootWindow(display);
    __enum_window(display,win);

#elif defined(Q_OS_WIN)
    HWND hwnd=::GetWindow(::GetDesktopWindow(),GW_CHILD);
    RECT r;
    GetWindowRect(hwnd,&r);
    rects.push_back(r);

    while(hwnd){
        hwnd=::GetWindow(hwnd,GW_HWNDNEXT);
        int len=GetWindowTextLengthA(hwnd);
        if(len<=0)continue;
        if(!IsWindowVisible(hwnd))continue;
        GetWindowRect(hwnd,&r);
        QRect rect(r.left,r.top,r.right-r.left,r.bottom-r.top);

        if(rect.left() >=0 && rect.top() >=0&&  rect.height() && rect.width()){
            int offset=qCore->getBorderWeight()*2;
            rects.push_back(Rect(r.top,r.left+offset,r.bottom-offset,r.right-offset));
        }
    }

#endif

    for(int i=0; i<rects.size(); i++){
        for(int j=i+1; j<rects.size(); j++){
            if(rects[i]==rects[j]){
                rects.remove(i);
                j--;
            }
        }
    }

    return rects;
}


#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX)

void __enum_window(Display* display, Window window)
{
    unsigned int i;

    Window root, parent;
    Window* children;
    unsigned int n;
    XQueryTree(display, window, &root, &parent, &children, &n);
    if (children != NULL) {
        XWindowAttributes attr;
        for (i = 0; i < n; i++) {
            XGetWindowAttributes(display,children[i],&attr);
            int x=attr.x,y=attr.y,w=attr.width,h=attr.height;
            if(x>=0 && y>=0 && w>3 && h>3){
                Rect r(y,x,y+h,x+w);
                rects.push_back(r);
            }
            __enum_window(display, children[i]);
        }
        XFree(children);
    }
}

#endif
