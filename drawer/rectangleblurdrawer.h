#ifndef RECTANGLEBLURDRAWER_H
#define RECTANGLEBLURDRAWER_H

#include "drawer.h"

class RectangleBlurDrawer : public Drawer
{
public:
    explicit RectangleBlurDrawer(const QPixmap& pixmap,const QRect&rect,QPainter&p,qreal r=10, QObject* parent=nullptr);
};

#endif
