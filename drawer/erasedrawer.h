#ifndef ERASEDRAWER_H
#define ERASEDRAWER_H

#include "drawer.h"
#include "core/core.h"

class EraseDrawer : public Drawer
{
public:
    EraseDrawer(const shape_object& object,QPainter &p,QPainter &pt,QObject *parent=nullptr);
};

#endif
