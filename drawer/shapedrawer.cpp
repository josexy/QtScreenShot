#include "shapedrawer.h"

#include <QPainterPath>

ShapeDrawer::ShapeDrawer(const shape_object &object, QPainter &p, QObject *parent)
    : Drawer(parent)
{
    if(object.fill) {
         p.setBrush(object.bgcolor);
    }
    if(object.pensize) {
        if(object.st==ShapeType::Rectangle){
            p.drawRect(object.rs.front());
        }else if(object.st==ShapeType::Circle){
            p.drawEllipse(object.rs.front());
        }else if(object.st==ShapeType::Triangle){
            QPainterPath pp;
            pp.addPolygon(QPolygonF(object.ps));
            if(object.fill) {
                p.fillPath(pp,object.bgcolor);
            }
            p.drawPath(pp);
        }
    }
    p.setBrush(Qt::NoBrush);
}
