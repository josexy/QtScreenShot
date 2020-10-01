#ifndef IMAGEBLURDRAWER_H
#define IMAGEBLURDRAWER_H

#include "drawer.h"

class QGraphicsEffect;
class ImageBlurDrawer : public Drawer{
public:
    explicit ImageBlurDrawer(const QPixmap& pixmap,QGraphicsEffect*effect,QObject* parent=nullptr);
    explicit ImageBlurDrawer(const QPixmap& pixmap,qreal r,QObject* parent=nullptr);
    explicit ImageBlurDrawer(const QImage& image,QGraphicsEffect*effec,QObject* parent=nullptr);
    explicit ImageBlurDrawer(const QImage& image,qreal r,QObject* parent=nullptr);
    QImage & getImage();
    QPixmap getPixmap();
private:
    void __setBlur(const QPixmap& pixmap,QGraphicsEffect *effect);
    QImage __Img;
};

#endif
