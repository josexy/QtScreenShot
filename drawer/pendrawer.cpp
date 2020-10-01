#include "pendrawer.h"

#include <QPainterPath>

PenDrawer::PenDrawer(const shape_object &object, QPainter &p, QObject *parent)
    :Drawer(parent)
{
    if(object.st==ShapeType::Line){
        QLineF line(object.ps.front(),object.ps.back());
        p.drawLine(line);
    }else if(object.st==ShapeType::Point){
        p.drawPoint(object.ps.front());
    }else if(object.st==ShapeType::Curve){
        QPainterPath pp;
        for(int j=0; j+1<object.ps.size(); j++){
            pp.moveTo(object.ps[j]);
            pp.lineTo(object.ps[j+1]);
        }
        p.drawPath(pp);
    }
}
