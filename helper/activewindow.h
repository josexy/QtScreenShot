#ifndef ACTIVEWINDOW_H
#define ACTIVEWINDOW_H

#include <QRect>
#include <QVector>
#include <QDebug>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif
struct Rect{
    int top,left,bottom,right;
    Rect(int t,int l,int b,int r): top(t),left(l),bottom(b),right(r){}
    Rect(const QRect &r){ top=r.top(),left=r.left(),bottom=r.bottom(),right=r.right(); }

    int width() {return right-left;}
    int height() {return bottom-top;}

    operator QRect(){return QRect(left,top,width(),height());}
};

class ActiveWindow
{
public:
    ActiveWindow();
    ~ActiveWindow();
#ifdef Q_OS_WIN
    QVector<Rect> enum_window();
#endif
};

#endif
