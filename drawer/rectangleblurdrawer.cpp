#include "rectangleblurdrawer.h"

#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

RectangleBlurDrawer::RectangleBlurDrawer(const QPixmap &pixmap, const QRect &rect, QPainter &p, qreal r, QObject *parent)
    :Drawer(parent)
{
    QPoint p0=rect.topLeft();
    QPoint p1=rect.bottomRight();
    QRect selection = QRect(p0,p1).normalized();

    QGraphicsScene scene;
    QGraphicsPixmapItem item;

    qreal pixelRatio=pixmap.devicePixelRatio();
    QRect selectionScaled=QRect(p0 * pixelRatio, p1 * pixelRatio).normalized();
    QGraphicsBlurEffect *blur=new QGraphicsBlurEffect(this);
    blur->setBlurRadius(r);
    item.setPixmap(pixmap.copy(selectionScaled));
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    scene.render(&p,selection);
}

