#include "separator.h"
#include <QPainter>

Separator::Separator(QWidget *parent): QLabel(parent)
{
    setWindowFlag(Qt::FramelessWindowHint);
}

void Separator::paintEvent(QPaintEvent * e)
{
    QPainter p(this);
    int offset=qCore->borderPadding();
    p.setPen(QPen(Qt::darkGray,2));
    p.drawLine(QPoint(width()/2,offset),QPoint(width()/2,height()-offset));
    QWidget::paintEvent(e);
}
