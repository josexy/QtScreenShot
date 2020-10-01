#ifndef ARROWDRAWER_H
#define ARROWDRAWER_H

#include "drawer.h"
#include "core/core.h"

class ArrowDrawer : public Drawer
{
public:
    ArrowDrawer(const QPoint& from, const QPoint& to,
                int lineSize, QColor lineColor,
                QPainter& p, ShapeType shapeType=ShapeType::TriArrow);
};

#endif
