#ifndef PENDRAWER_H
#define PENDRAWER_H

#include "drawer.h"
#include "core/core.h"
class PenDrawer : public Drawer
{
public:
    PenDrawer(const shape_object &object, QPainter &p, QObject *parent);
};

#endif
