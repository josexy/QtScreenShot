#include "arrowdrawer.h"
#include "core/core_system.h"
#include <QPainterPath>
#include <cmath>

ArrowDrawer::ArrowDrawer(const QPoint &from, const QPoint &to,
                         int lineSize,QColor lineColor,
                         QPainter &p, ShapeType shapeType)
{

    if(from.x()==-1||from.y()==-1||to.x()==-1||to.y()==-1) return;
    QLineF line(from,to);
    if(shapeType==ShapeType::TriArrow){
        QPainterPath pp;
        QLineF l1=line.unitVector();

        if(line.length()>=lineSize){
            line.setLength(line.length()-lineSize);
        }else{
            line.setLength(0);
        }
        l1.setLength(lineSize);
        l1.translate(line.dx(),line.dy());

        QLineF l2=l1.normalVector();
        l2.setLength(l2.length()*0.5);

        QLineF l3=l2.normalVector().normalVector();
        pp.addPolygon(QPolygonF({l1.p2(),l2.p2(),l3.p2()}));

        p.drawLine(line);
        p.fillPath(pp,lineColor);
    }
}
