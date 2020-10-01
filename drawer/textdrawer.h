#ifndef TEXTDRAWER_H
#define TEXTDRAWER_H

#include "drawer.h"
#include "core/core.h"

class TextDrawer : public Drawer
{
public:
    TextDrawer(const shape_object& object,QPainter &p,QObject *parent=nullptr);
};

#endif
