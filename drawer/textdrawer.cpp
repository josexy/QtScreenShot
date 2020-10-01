#include "textdrawer.h"

TextDrawer::TextDrawer(const shape_object &object, QPainter &p, QObject *parent):
    Drawer(parent)
{
    p.save();
    p.setFont(object.font);
    p.setPen(object.color);
    for(int j=0; j<object.ts.size(); j++){
        p.drawText(object.ps[j],object.ts[j]);
    }
    p.restore();
}
