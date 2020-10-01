#include "pixelwidget.h"
#include <QPainter>

PixelWidget::PixelWidget(QWidget *parent) : QWidget(parent)
{
    setFixedSize(parent->width(),parent->height()/2);
}

void PixelWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    /*
     * x-WIDTH/scale/2
     * y-HEIGHT/scale/2
     * WIDTH/scale
     * HEIGTH/scale
    */
    int S=14;
    auto _NewPixmap=pixmap.copy( pos.x()-width()/S/2, pos.y()-height()/S/2, width()/S, height()/S);
    /*
     * 0
     * 0
     * WIDTH
     * HEIGHT
    */
    p.drawPixmap(0,0,width(),height(),_NewPixmap);

    QColor c=Qt::white;
    if(color.red()> 100 && color.green()>100 && color.blue()>100)
        c=Qt::black;
    p.setPen(QPen(c,1));
    int cx=x()+width()/2;
    int cy=y()+height()/2+S/2;
    p.drawLine(cx-S,cy,cx+S,cy);
    p.drawLine(cx,cy-S,cx,cy+S);

    p.setPen(QPen(c,2));
    p.drawRect(rect());

    QWidget::paintEvent(event);
}


void PixelWidget::setPos(const QPoint& point)
{
    pos=point;
}

void PixelWidget::setColor(QColor _c)
{
    color=_c;
}

void PixelWidget::setPixmap(const QPixmap &__pixmap)
{
    pixmap=__pixmap;
}
