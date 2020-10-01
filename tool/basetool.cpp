#include "basetool.h"

BaseTool::BaseTool(QWidget *parent) : QWidget(parent)
{
    setWindowFlag(Qt::FramelessWindowHint);
}

void BaseTool::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    p.setPen(QPen(qCore->getWidgetBgColor(),qCore->borderPadding()));
    p.fillRect(rect(),Qt::white);
    p.drawRect(rect());

    QWidget::paintEvent(event);
}
