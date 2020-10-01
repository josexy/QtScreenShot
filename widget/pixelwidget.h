#ifndef PIXELWIDGET_H
#define PIXELWIDGET_H

#include <QWidget>
#include "../core/core_system.h"

class PixelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PixelWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);

    void setPixmap(const QPixmap&);
    void setColor(QColor);
    void setPos(const QPoint &pos);
private:
    QPixmap pixmap;
    QColor color;
    QPoint pos;
};

#endif
