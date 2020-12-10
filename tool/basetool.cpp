#include "basetool.h"

BaseTool::BaseTool(QWidget *parent) : QWidget(parent)
{
    setWindowFlag(Qt::FramelessWindowHint);

    if(qCore->getShadowEffect()){
        setAttribute(Qt::WA_TranslucentBackground);

        auto dsEffect = new QGraphicsDropShadowEffect(this);
        dsEffect->setBlurRadius(5);
        dsEffect->setOffset(0);
        dsEffect->setColor(QColor(Qt::black));
        setGraphicsEffect(dsEffect);
    }
}

void BaseTool::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    p.setBrush(QBrush(qCore->getWidgetBgColor()));
    p.setPen(Qt::transparent);

    if(qCore->getShadowEffect())
        p.drawRoundedRect(rect(), 5, 5);
    else
        p.drawRect(rect());

    QWidget::paintEvent(event);
}
