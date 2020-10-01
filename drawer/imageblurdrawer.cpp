#include "imageblurdrawer.h"

#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>

ImageBlurDrawer::ImageBlurDrawer(const QPixmap &pixmap, QGraphicsEffect *effect,QObject* parent): Drawer(parent)
{
    __setBlur(pixmap,effect);
}

ImageBlurDrawer::ImageBlurDrawer(const QPixmap &pixmap, qreal r,QObject* parent): Drawer(parent)
{
    QGraphicsBlurEffect *blur=new QGraphicsBlurEffect(this);
    blur->setBlurRadius(r);
    __setBlur(pixmap,blur);
}

ImageBlurDrawer::ImageBlurDrawer(const QImage &image, QGraphicsEffect *effect,QObject* parent)
    : ImageBlurDrawer(QPixmap::fromImage(image),effect,parent) { }

ImageBlurDrawer::ImageBlurDrawer(const QImage &image, qreal r,QObject* parent)
    : ImageBlurDrawer(QPixmap::fromImage(image),r,parent) { }

QImage &ImageBlurDrawer::getImage()
{
    return __Img;
}

QPixmap ImageBlurDrawer::getPixmap()
{
    return QPixmap::fromImage(__Img);
}

void ImageBlurDrawer::__setBlur(const QPixmap &pixmap, QGraphicsEffect *effect)
{
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(pixmap);
    item.setGraphicsEffect(effect);
    scene.addItem(&item);
    __Img=QImage(pixmap.size(), QImage::Format_ARGB32);
    __Img.fill(Qt::transparent);
    QPainter ptr(&__Img);
    scene.render(&ptr, QRectF(), QRectF(0, 0, pixmap.width(), pixmap.height()));
}
