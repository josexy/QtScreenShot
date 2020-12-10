#include "activewindow.h"
#include "core/core_system.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#include <vector>
#endif

QVector<Rect>rects;

ActiveWindow::ActiveWindow() {
    rects.clear();
}

ActiveWindow::~ActiveWindow()
{
}
QVector<Rect> ActiveWindow::enum_window()
{
    rects.clear();
#ifdef Q_OS_WIN
    HWND hwnd=::GetWindow(::GetDesktopWindow(),GW_CHILD);
    RECT r;
    GetWindowRect(hwnd,&r);
    rects.push_back(Rect(r.top,r.left,r.bottom,r.right));

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
    // unique
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

