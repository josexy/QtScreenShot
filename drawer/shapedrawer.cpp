#include "shapedrawer.h"
#include "core/core_system.h"
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
        }else if(object.st==ShapeType::RTriangle||object.st==ShapeType::Triangle){
            QPainterPath pp;
            pp.addPolygon(QPolygonF(object.ps));
            if(object.fill) {
                p.fillPath(pp,object.bgcolor);
            }
            p.drawPath(pp);
        }else if(object.st==ShapeType::Brush){
            QColor c=object.color;
            p.fillRect(object.rs.front(),QColor(c.red(),c.green(),c.blue(),qCore->brushOpacity()));
        }
    }
    p.setBrush(Qt::NoBrush);
}
